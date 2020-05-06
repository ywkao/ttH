from scanClass import scanClass
from makeModels import makeModel
from cardMaker import makeCards

import multiprocessing
import os, sys
import subprocess
import itertools
import json
import numpy

sys.path.append("../Loopers/")
import parallel_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to distinguish this optimization", type=str, default = "test")
parser.add_argument("--channel", help = "leptonic or hadronic", type=str, default = "TTHHadronicTag")
parser.add_argument("--file", help = "path to final fit tree", type=str)

parser.add_argument("--mvas", help = "list of mva branches to perform Nd optimization with", type=str, default = "mva_score")
parser.add_argument("--sm_higgs_unc", help = "value of unc on sm higgs processes", type=float, default = 0.1)

parser.add_argument("--nCores", help = "number of cores to use", type=int, default = 12)
parser.add_argument("--nPoints", help = "number of different cuts to try", type=int, default = 100)
parser.add_argument("--nBins", help = "number of bins", type=int, default = 2)

parser.add_argument("--resonant_bkgs", help = "processes to consider for resonant background (csv list)", type=str, default = "sm_higgs")
parser.add_argument("--signal", help = "process to consider for signal", default = "ttH")

parser.add_argument("--limit", help = "calculate limit instead of significance", action = "store_true")

args = parser.parse_args()

process_dict = { 
    "ttH" : [0],
    "ggH" : [14],
    "FCNC_Hut" : [22,24],
    "FCNC_Hct" : [23,25], 
    "tH" : [11,12],
    "sm_higgs" : [0,11,12,14,15,16],
}

mvas = args.mvas.split(",")


def calculate_bins_significance(idx, scanConfig, scanner, cuts, results):
    tree = scanner.getTree()

    nBins = len(cuts)

    print "[BINNING_SCRIPT_INFO] Testing cut combo %d" % idx

    for i in range(nBins):
        # MVA score cut
        cut = cuts[i]

        # Make signal model
        signals = args.signal.split(",") + args.resonant_bkgs.split(",")
        for signal in signals:
            sigModelConfig = { "var" : "mass", "weightVar" : "weight", "plotpath" : scanConfig["plotpath"], "modelpath" : scanConfig["modelpath"], "filename" : scanConfig["filename"]}
            sigModelConfig["savename"] = "CMS-HGG_sigfit_mva_" + signal + "_hgg_" + scanConfig["tag"] + "_" + str(i) + "_" + str(idx)
            sigModelConfig["tag"] = "hggpdfsmrel_" + signal + "_hgg_" + scanConfig["tag"] + "_" + str(i) + "_" + str(idx)
            processSelection = "("
            for j in range(len(process_dict[signal])):
                processSelection += "process_id == " + str(process_dict[signal][j])
                if j == (len(process_dict[signal]) - 1):
                    processSelection += ")"
                else:
                    processSelection += " || "
            sigModelConfig["selection"] = scanConfig["selection"] + " && " + cut + " && " + processSelection
            if "FCNC" not in args.signal.split(",")[0]: # this is the ttH analysis, we use M127 for optimization
                signal_sample_selection = " && signal_mass_category == 127"
            else: # this is FCNC, use M125 since we don't have other mass points for FCNC
                signal_sample_selection = " && ((process_id >= 22 && process_id <= 25) || (signal_mass_label == 0 && signal_mass_category == 125))" # accept all FCNC events, only M125 Madgraph (not Powheg) for all other SM Higgs # FIXME
            sigModelConfig["selection"] += signal_sample_selection
            print "\n[BINNING_SCRIPT_INFO] Making signal model with selection %s" % sigModelConfig["selection"]
            print "\n[BINNING_SCRIPT_INFO] Saving as %s" % "CMS-HGG_sigfit_mva_" + signal + "_hgg_" + scanConfig["tag"] + "_" + str(i) + "_" + str(idx)
            print "\n\n"
            model = makeModel(sigModelConfig)
            model.getTree(tree)
            model.makeSignalModel("wsig_13TeV", {"replaceNorm":False, "norm_in":-1, "fixParameters":True})

        # Make background model
        bkgModelConfig = { "var" : "mass", "weightVar" : "weight", "plotpath" : scanConfig["plotpath"], "modelpath" : scanConfig["modelpath"], "filename" : scanConfig["filename"] }
        bkgModelConfig["selection"] = scanConfig["selection"] + " && " + cut + " && (process_id == 1 || process_id == 2 || process_id == 3 || process_id == 5 || process_id == 6 || process_id == 7 || process_id == 9 || process_id == 13 || process_id == 18 || process_id == 19 || process_id == 20 || process_id == 21 || process_id == 26)"
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

    card = makeCards(scanConfig["modelpath"], "CMS-HGG_mva_13TeV_datacard_" + str(idx) + ".txt", {"sm_higgs_unc" : args.sm_higgs_unc})
    tagList = [(scanConfig["tag"] + "_%d" % x) for x in range(nBins)] 
    print "tagList is", tagList, "\n\n\n\n"
    card.WriteCard(sigList, bkgList, tagList, "_" + str(idx))

    if args.limit:
        combineOption = "AsymptoticLimits -m 125 "
    else:
        combineOption = "Significance --significance "
    combineConfig = { "combineOption" : combineOption, "combineOutName" : "sig_" + str(idx), "cardName" : "CMS-HGG_mva_13TeV_datacard_" + str(idx) + ".txt", "outtxtName" : "sig_" + str(idx) + ".txt", "grepContent" : "\"Significance\"" }

    significance, sig_up1sigma, sig_down1sigma = scanner.runCombine(combineConfig)
    print "[BINNING_SCRIPT_INFO] %s for cut combo %d is %.6f" % ("Significance" if not args.limit else "Limit", idx, significance)

    #for cut in cuts:
    #    cut = float(cut)
    result = { "mva_cuts" : cuts, ("limit" if args.limit else "significance") : significance , "datacard" : scanConfig["modelpath"] + combineConfig["cardName"] , "up1sigma" : sig_up1sigma, "down1sigma" : sig_down1sigma }

    results[idx] = result
    print results
    return

def next_largest(array, value):
    larger = [x for x in array if x > value]
    return min(larger)

def enumerate_cuts(mva, combo):
    cuts = []
    for cut in combo:
        cut_string = "(%s > %.6f" % (mva, cut)
        if numpy.array([cut < x for x in combo]).any(): # check if it is not the largest value in the combo (need an upper cut too)
            cut_string += " && %s < %.6f" % (mva, next_largest(combo, cut))
        cut_string += ")"
        cuts.append(cut_string)
    return cuts

def calculate_cut_combos(nBins, nPoints, scanner, scanConfig, signal):
    processSelection = " && ("
    for j in range(len(process_dict[signal])):
        processSelection += "process_id == " + str(process_dict[signal][j])
        if j == (len(process_dict[signal]) - 1):
            processSelection += ")"
        else:
            processSelection += " || "
    

    mva_cuts = {}

    for mva in scanConfig["mvaName"]:
        mvaScores = scanner.quantiles_to_mva_score(nPoints, mva, processSelection)
        cut_combos_unformatted = list(itertools.combinations(mvaScores, nBins)) 
        cut_combos = []

        for combo in cut_combos_unformatted:
            combo_formatted = [x[0] for x in combo]
            combo_formatted.reverse()
            cut_combos.append(combo_formatted)

        mva_cuts[mva] = cut_combos

    
    cut_combos = []
    if len(scanConfig["mvaName"]) == 1: # 1d
        cut_combos_0 = mva_cuts[scanConfig["mvaName"][0]]
        mva0 = scanConfig["mvaName"][0]
        for combo0 in cut_combos_0:
            cuts = enumerate_cuts(mva0, combo0)
            cut_combos.append(cuts)

    else: # 2d
        cut_combos_0 = mva_cuts[scanConfig["mvaName"][0]]
        cut_combos_1 = mva_cuts[scanConfig["mvaName"][1]]
        mva0 = scanConfig["mvaName"][0]
        mva1 = scanConfig["mvaName"][1]
        for combo0 in cut_combos_0:
            for combo1 in cut_combos_1:
                cuts0 = enumerate_cuts(mva0, combo0)
                cuts1 = enumerate_cuts(mva1, combo1)
                cuts_both = list(itertools.product(cuts0, cuts1))
                cuts_full = []
                for combo_both in cuts_both:
                    cut_string = combo_both[0] + " && " + combo_both[1]
                    cuts_full.append(cut_string)
                cut_combos.append(cuts_full)

    print "[derive_binning.py] args.nBins:     %d" % args.nBins
    print "[derive_binning.py] args.nPoints:   %d" % args.nPoints
    print "[derive_binning.py] args.mvas:      %s" % args.mvas

    print "[derive_binning.py] n cut combos:   %d" % len(cut_combos)
    print "[derive_binning.py] n bins/combo:   %d" % len(cut_combos[0])
    print "[derive_binning.py] these cuts look like:"
    for cut in cut_combos[0]:
        print cut

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

doLow = False
lowCut = 0.0
highCut = 1.0
date = "20200106"
postFix = "fcnc_test"
tag = "TTHHadronicTag"
# setup scan
scanConfig= {\
"tag" : args.channel,
"filename" : args.file,
"selection" : "mass > 100 && mass < 180",
"sigName":"ttH_hgg",
"modelpath" : "/home/users/sjmay/ttH/Binning/models/" + args.tag + "/",
"plotpath" : "/home/users/sjmay/public_html/ttH/Binning/" + args.tag + "/",
"combineEnv" : "/home/users/sjmay/ttH/Binning/CMSSW_10_2_13/src",
"var":"mass",
"weightVar":"weight",
"mvaName":mvas,
}

testScan = scanClass(scanConfig)
testScan.cleanDir()

calculate_cut_combos(args.nBins, args.nPoints, testScan, scanConfig, args.signal)
