from scanClass import scanClass
from makeModels import makeModel
from cardMaker import makeCards

import multiprocessing
import os, sys
import subprocess
import itertools
import json

sys.path.append("../Loopers/")
import parallel_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to distinguish this optimization", type=str, default = "test")
parser.add_argument("--nCores", help = "number of cores to use", type=int, default = 12)
parser.add_argument("--channel", help = "leptonic or hadronic", type=str, default = "TTHHadronicTag")
parser.add_argument("--nPoints", help = "number of different cuts to try", type=int, default = 100)
parser.add_argument("--resonant_bkgs", help = "processes to consider for resonant background (csv list)", type=str, default = "ggH")
parser.add_argument("--signal", help = "process to consider for signal", default = "ttH")
parser.add_argument("--nBins", help = "number of bins", type=int, default = 2)
args = parser.parse_args()

process_dict = { "ttH" : [0], "ggH" : [14], "FCNC_Hut" : [22,24], "FCNC_Hct" : [23,25] }

def calculate_bins_significance(idx, scanConfig, scanner, cuts, results):
    tree = scanner.getTree()

    nBins = len(cuts)

    print "[BINNING_SCRIPT_INFO] Testing cut combo %d" % idx

    for i in range(nBins):
        # MVA score cut
        cut = "mva_score > " + str(cuts[i])
        if not (i == (nBins - 1)):
            cut += " && mva_score < " + str(cuts[i+1])

        # Make signal model
        signals = args.signal.split(",") + args.resonant_bkgs.split(",")
        for signal in signals:
            sigModelConfig = { "var" : "mass", "weightVar" : "weight", "plotpath" : scanConfig["plotpath"], "modelpath" : scanConfig["modelpath"] }
            sigModelConfig["savename"] = "CMS-HGG_sigfit_mva_" + signal + "_hgg_" + scanConfig["tag"] + "_" + str(i) + "_" + str(idx)
            sigModelConfig["tag"] = "hggpdfsmrel_" + signal + "_hgg_" + scanConfig["tag"] + "_" + str(i) + "_" + str(idx)
            processSelection = "("
            for j in range(len(process_dict[signal])):
                processSelection += "process_id == " + str(process_dict[signal][j])
                if j == (len(process_dict[signal]) - 1):
                    processSelection += ")"
                else:
                    processSelection += " || "
            sigModelConfig["selection"] = scanConfig["selection"] + " && " + cut + " && " + processSelection + " && signal_mass_category == 127"
            print "\n[BINNING_SCRIPT_INFO] Making signal model with selection %s" % sigModelConfig["selection"]
            print "\n[BINNING_SCRIPT_INFO] Saving as %s" % "CMS-HGG_sigfit_mva_" + signal + "_hgg_" + scanConfig["tag"] + "_" + str(i) + "_" + str(idx)
            print "\n\n"
            model = makeModel(sigModelConfig)
            model.getTree(tree)
            model.makeSignalModel("wsig_13TeV", {"replaceNorm":False, "norm_in":-1, "fixParameters":True})

        # Make background model
        bkgModelConfig = { "var" : "mass", "weightVar" : "weight", "plotpath" : scanConfig["plotpath"], "modelpath" : scanConfig["modelpath"] }
        bkgModelConfig["selection"] = scanConfig["selection"] + " && " + cut + " && ((mass > 100 && mass < 120) || ( mass > 130 && mass < 180)) && sample_id == 0 && process_id <= 10"
        bkgModelConfig["savename"] = "CMS-HGG_bkg_" + scanConfig["tag"] + "_" + str(i) + "_" + str(idx)
        bkgModelConfig["tag"] = "CMS_hgg_bkgshape_" + scanConfig["tag"] + "_" + str(i) + "_" + str(idx)

        print "[BINNING_SCRIPT_INFO] Making background model with selection %s" % bkgModelConfig["selection"]

        model = makeModel(bkgModelConfig)
        model.getTree(tree)
        model.makeBackgroundModel("wbkg_13TeV", scanConfig["tag"] + "_" + str(i) + "_" + str(idx))

    # Make datacard
    print "[BINNING_SCRIPT_INFO] Making datacard for cut combo %d" % idx
    sigList = [args.signal + "_hgg"]
    bkgList = ["bkg_mass"]
    for bkg in args.resonant_bkgs.split(","):
        bkgList.append(bkg + "_hgg")

    card = makeCards(scanConfig["modelpath"], "CMS-HGG_mva_13TeV_datacard_" + str(idx) + ".txt")
    tagList = [(tag + "_%d" % x) for x in range(nBins)] 
    card.WriteCard(sigList, bkgList, tagList, "_" + str(idx))

    combineConfig = { "combineOption" : "Significance --significance ", "combineOutName" : "sig_" + str(idx), "cardName" : "CMS-HGG_mva_13TeV_datacard_" + str(idx) + ".txt", "outtxtName" : "sig_" + str(idx) + ".txt", "grepContent" : "\"Significance\"" }

    significance = float(scanner.runCombine(combineConfig))
    print "[BINNING_SCRIPT_INFO] Significance for cut combo %d is %.6f" % (idx, significance)

    for cut in cuts:
        cut = float(cut)
    result = { "mva_cuts" : [str(cut) for cut in cuts], "significance" : significance }

    results[idx] = result
    print results
    return
    #return significance
    #os.chdir(scanConfig["modelpath"])
    #os.system("chmod u+x %s" % "combineCmd_sig_" + str(idx) + ".sh")
    #os.system("./%s" % "combineCmd_sig_" + str(idx) + ".sh")
    #os.chdir("~/ttH/Binning/")

    #significance = os.popen('grep "Significance:" %s | awk "{print $2}"' % (self.modelpath + outtxtName)).read().strip()
    #return significance


def calculate_cut_combos(nBins, nPoints, scanner, scanConfig):
    mvaScores = scanner.quantiles_to_mva_score(nPoints, scanConfig["mvaName"])
    cut_combos_unformatted = list(itertools.combinations(mvaScores, nBins)) 
    cut_combos = []

    for combo in cut_combos_unformatted:
        combo_formatted = [x[0] for x in combo]
        combo_formatted.reverse()
        cut_combos.append(combo_formatted)

    manager = multiprocessing.Manager()
    results = manager.dict()

    running_procs = []
    for i in range(len(cut_combos)):
        print "[BINNING_SCRIPT_INFO] %d" % i
        running_procs.append(multiprocessing.Process(target=calculate_bins_significance, args=(i, scanConfig, scanner, cut_combos[i], results)))
        running_procs[-1].start()
        while True:
            for i in range(len(running_procs)):
                if not running_procs[i].is_alive():
                    running_procs.pop(i)
                    break
            if len(running_procs) < args.nCores:
                break
            else:
                os.system("sleep 5s")
    while len(running_procs) > 0:
        for i in range(len(running_procs)):
            try:
                if not running_procs[i].is_alive():
                    running_procs.pop(i)
            except:
                continue

    print results
    with open("results_%s.json" % args.tag, "w") as f_out:
        json.dump(results.copy(), f_out, indent=4, sort_keys=True)

doLow =False
lowCut = 0.0
highCut = 1.0
date = "20200106"
postFix = "fcnc_test"
#tag = "TTHLeptonicTag"
tag = "TTHHadronicTag"
# setup scan
scanConfig= {\
"tag":tag,
#"selection":"global_features[7] > 0.33 && global_features[8] > 0.25 && signal_mass_category == 127 && mass > 100 && mass < 180 && mva_score < " + str(highCut)+ "&& mva_score > " + str(lowCut),
#"selection" : "mass > 100 && mass < 180 && mva_score < " + str(highCut)+ " && mva_score > " + str(lowCut),
"selection" : "mass > 100 && mass < 180",
#"selection":"global_features[7] > 0.33 && global_features[8] > 0.25 && signal_mass_category == 127 && mass > 100 && mass < 180 && mva_score < " + str(highCut)+ "&& mva_score > " + str(lowCut),
#"selection":"global_features[7] > 0.33 && global_features[8] > 0.25 && train_id == 1 && signal_mass_category == 127 && mass > 100 && mass < 180 && mva_score < " + str(highCut)+ "&& mva_score > " + str(lowCut),
"sigName":"ttH_hgg",
#"modelpath":"/home/users/hmei/ttH/MakeBDTBins/doScan/models/ttH_"+postFix+"_"+date+"/",
#"plotpath":"/home/users/hmei/public_html/2019/"+date+"_ttH_"+postFix+"/",
#"combineEnv":"/home/users/hmei/CMSSW_7_4_7/src/",
"modelpath" : "/home/users/sjmay/ttH/Binning/models/ttH_"+postFix+"_"+date+"/",
"plotpath" : "/home/users/sjmay/public_html/ttH/Binning/"+date+"_ttH_"+postFix+"/",
"combineEnv" : "/home/users/sjmay/ttH/Binning/CMSSW_10_2_13/src",
"var":"mass",
"weightVar":"weight",
"mvaName":"mva_score"
#"mvaName":"mva_score"
}

testScan = scanClass(scanConfig)
testScan.cleanDir()

calculate_cut_combos(args.nBins, args.nPoints, testScan, scanConfig)

#useNCores = 20
#nJobsPerCore =5
#jobs = []
#for i in range(useNCores):
#    lowIndex = nJobsPerCore*i
#    highIndex = nJobsPerCore*(i+1)
#    p = multiprocessing.Process(target=doAllFits_oneCore, args=(lowIndex, highIndex, scanConfig, lowCut, highCut))
#    jobs.append(p)
#    p.start()
#
#doAllFits(100, scanConfig, lowCut, highCut)
#subprocess.call('chmod -R 755 ' + scanConfig["plotpath"], shell=True)
