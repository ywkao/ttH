from scanClass import scanClass
from makeModels import makeModel
from cardMaker import makeCards
import multiprocessing

import sys
import subprocess

#for i in range(0):#len(cuts_bin0)):
#for i in range(len(cuts_bin0)):
def doAllFits_oneCore(index1, index2, scanConfig, lowCut, highCut):
    for index in range(index1, index2):
        print index
        doAllFits(index, scanConfig, lowCut, highCut)

def doAllFits(i, scanConfig, lowCut, highCut):
    testScan.getTree()
    mvaScores = testScan.quantiles_to_mva_score(100, scanConfig["mvaName"])
    mvaScores.insert(0,[highCut,])
    print mvaScores

    cuts_bin0 = []
    cuts_bin1 = []

# this is to accomodate low end cut
    scanConfig["selection"] = "global_features[7] > 0.33 && global_features[8] > 0.25 && signal_mass_category == 127 && mass > 100 && mass < 180"
#scanConfig["selection"] = "global_features[7] > 0.33 && global_features[8] > 0.25 && train_id == 1 && signal_mass_category == 127 && mass > 100 && mass < 180"

# setup cuts to try
    for j in range(1, len(mvaScores)):
        if not doLow:
            cuts_bin0.append("mva_score > " + str(mvaScores[j][0]) + "&& mva_score < " + str(highCut))
            cuts_bin1.append("mva_score < " + str(mvaScores[j][0]) + "&& mva_score > " + str(lowCut))
        else:
            cuts_bin0.append("mva_score < 1 && mva_score > " + str(highCut))
            cuts_bin1.append("mva_score > " + str(mvaScores[j][0]) + "&& mva_score < " + str(highCut))
        subprocess.call("echo mvaScore " + str(mvaScores[j][0]) + " > " + scanConfig["modelpath"] + "/mvaScore_"+str(j-1)+".txt" , shell=True)

# add 1 bin
    if not doLow:
        cuts_bin0.append("mva_score > " + str(lowCut) + "&& mva_score < " + str(highCut))
        cuts_bin1.append("mva_score > " + str(lowCut) + "&& mva_score < " + str(highCut))
    else:
        cuts_bin0.append("mva_score < 1 && mva_score > " + str(highCut))
        cuts_bin1.append("mva_score < 1 && mva_score > " + str(highCut))

    # make signal model
    sigModelConfig = {}
    sigModelConfig["var"] = "mass"
    sigModelConfig["weightVar"] = "weight"

    # tth bin 0
    sigModelConfig["selection"] = scanConfig["selection"] + " &&" + cuts_bin0[i] + " && process_id == 0"
    #print sigModelConfig["selection"]
    sigModelConfig["plotpath"] = scanConfig["plotpath"]
    sigModelConfig["modelpath"] = scanConfig["modelpath"]
    sigModelConfig["savename"] = "CMS-HGG_sigfit_mva_ttH_hgg_" + scanConfig["tag"] + "_0_" + str(i)
    sigModelConfig["tag"] = "hggpdfsmrel_ttH_hgg_" + scanConfig["tag"] + "_0_" + str(i)
    testSigModel = makeModel(sigModelConfig)
    testSigModel.getTree(testScan.tree)
    sigConfig = {"replaceNorm":False, "norm_in":-1, "fixParameters":True}
    testSigModel.makeSignalModel("wsig_13TeV", sigConfig)

    # ggh bin 0
    sigModelConfig["selection"] = scanConfig["selection"] + " &&" + cuts_bin0[i] + " && process_id == 14"
    sigModelConfig["savename"] = "CMS-HGG_sigfit_mva_ggH_hgg_" + scanConfig["tag"] + "_0_" + str(i)
    sigModelConfig["tag"] = "hggpdfsmrel_ggH_hgg_" + scanConfig["tag"] + "_0_" + str(i)
    testSigModel = makeModel(sigModelConfig)
    testSigModel.getTree(testScan.tree)
    sigConfig = {"replaceNorm":False, "norm_in":-1, "fixParameters":True}
    testSigModel.makeSignalModel("wsig_13TeV", sigConfig)
    # if norm is small
    norm_hgg = testSigModel.norm
    print norm_hgg
    sigModelConfig["selection"] = scanConfig["selection"] + " &&" + cuts_bin0[i] + " && process_id == 0"
    testSigModel = makeModel(sigModelConfig)
    testSigModel.getTree(testScan.tree)
    sigConfig = {"replaceNorm":True, "norm_in":norm_hgg, "fixParameters":True}
    testSigModel.makeSignalModel("wsig_13TeV", sigConfig)

    # tth bin 1
    sigModelConfig["selection"] = scanConfig["selection"] + " &&" + cuts_bin1[i] + " && process_id == 0"
    #print sigModelConfig["selection"]
    sigModelConfig["savename"] = "CMS-HGG_sigfit_mva_ttH_hgg_" + scanConfig["tag"] + "_1_" + str(i)
    sigModelConfig["tag"] = "hggpdfsmrel_ttH_hgg_" + scanConfig["tag"] + "_1_" + str(i)
    testSigModel = makeModel(sigModelConfig)
    testSigModel.getTree(testScan.tree)
    sigConfig = {"replaceNorm":False, "norm_in":-1, "fixParameters":True}
    testSigModel.makeSignalModel("wsig_13TeV", sigConfig)

    # ggh bin 1
    sigModelConfig["selection"] = scanConfig["selection"] + " &&" + cuts_bin1[i] + " && process_id == 14"
    sigModelConfig["savename"] = "CMS-HGG_sigfit_mva_ggH_hgg_" + scanConfig["tag"] + "_1_" + str(i)
    sigModelConfig["tag"] = "hggpdfsmrel_ggH_hgg_" + scanConfig["tag"] + "_1_" + str(i)
    testSigModel = makeModel(sigModelConfig)
    testSigModel.getTree(testScan.tree)
    sigConfig = {"replaceNorm":False, "norm_in":-1, "fixParameters":True}
    testSigModel.makeSignalModel("wsig_13TeV", sigConfig)
    # if norm is small
    norm_hgg = testSigModel.norm
    sigModelConfig["selection"] = scanConfig["selection"] + " &&" + cuts_bin1[i] + " && process_id == 0"
    testSigModel = makeModel(sigModelConfig)
    testSigModel.getTree(testScan.tree)
    sigConfig = {"replaceNorm":True, "norm_in":norm_hgg, "fixParameters":True}
    testSigModel.makeSignalModel("wsig_13TeV", sigConfig)

    bkgModelConfig = {}
    bkgModelConfig["var"] = "mass"
    bkgModelConfig["weightVar"] = "weight"
    bkgModelConfig["selection"] = "global_features[7] > 0.33 && global_features[8] > 0.25 && sample_id == 0 && process_id <= 10 && ((mass > 100 && mass < 120) || ( mass > 130 && mass < 180)) && " + cuts_bin0[i]
    #bkgModelConfig["selection"] = "global_features[7] > 0.33 && global_features[8] > 0.25 && train_id == 1 &&sample_id == 0 && process_id <= 10 && ((mass > 100 && mass < 120) || ( mass > 130 && mass < 180)) && " + cuts_bin0[i]
    bkgModelConfig["plotpath"] = scanConfig["plotpath"]
    bkgModelConfig["modelpath"] = scanConfig["modelpath"]
    bkgModelConfig["savename"] = "CMS-HGG_bkg_" + scanConfig["tag"] + "_0_"  + str(i)
    bkgModelConfig["tag"] = "CMS_hgg_bkgshape_" + scanConfig["tag"] + "_0_" + str(i)

    testBkgModel = makeModel(bkgModelConfig)
    testBkgModel.getTree(testScan.tree)
    datasetTag = scanConfig["tag"] + "_0_" + str(i)
    testBkgModel.makeBackgroundModel("wbkg_13TeV", datasetTag)

    bkgModelConfig["selection"] =  "global_features[7] > 0.33 && global_features[8] > 0.25 && sample_id == 0 && process_id <= 10 && ((mass > 100 && mass < 120) || ( mass > 130 && mass < 180)) && " + cuts_bin1[i]
    #bkgModelConfig["selection"] =  "global_features[7] > 0.33 && global_features[8] > 0.25 && train_id == 1 &&sample_id == 0 && process_id <= 10 && ((mass > 100 && mass < 120) || ( mass > 130 && mass < 180)) && " + cuts_bin1[i]
    bkgModelConfig["savename"] = "CMS-HGG_bkg_" + scanConfig["tag"] + "_1_"  + str(i)
    bkgModelConfig["tag"] = "CMS_hgg_bkgshape_" + scanConfig["tag"] + "_1_" + str(i)

    testBkgModel = makeModel(bkgModelConfig)
    testBkgModel.getTree(testScan.tree)
    datasetTag = scanConfig["tag"] + "_1_" + str(i)
    testBkgModel.makeBackgroundModel("wbkg_13TeV", datasetTag)

    # make datacard
    sigList = ["ttH_hgg"]
    bkgList = ["ggH_hgg", "bkg_mass"]
    for i in range(len(cuts_bin0)):
        card = makeCards(scanConfig["modelpath"], "CMS-HGG_mva_13TeV_datacard_"+str(i)+".txt")
        if i != len(cuts_bin0) - 1:
            card.WriteCard(sigList, bkgList, [tag + "_0", tag + "_1"], "_" + str(i))
        else:
            card.WriteCard(sigList, bkgList, [tag + "_0"], "_" + str(i))

        # run combine
        combineConfig = {}
        combineConfig["combineOption"] = "ProfileLikelihood --significance "
        combineConfig["combineOutName"] = "sig_" + str(i)
        combineConfig["cardName"] = "CMS-HGG_mva_13TeV_datacard_" + str(i) + ".txt"
        combineConfig["outtxtName"] = "sig_" + str(i) + ".txt"
        combineConfig["grepContent"] = "\"Significance\""

        limit = testScan.runCombine(combineConfig)

doLow =True
lowCut =0
highCut =0.9908256
date = "20191009"
postFix = "preapproval_had_test9"
#tag = "TTHLeptonicTag"
tag = "TTHHadronicTag"
# setup scan
scanConfig= {\
"tag":tag,
"selection":"global_features[7] > 0.33 && global_features[8] > 0.25 && signal_mass_category == 127 && mass > 100 && mass < 180 && mva_score < " + str(highCut)+ "&& mva_score > " + str(lowCut),
#"selection":"global_features[7] > 0.33 && global_features[8] > 0.25 && signal_mass_category == 127 && mass > 100 && mass < 180 && mva_score < " + str(highCut)+ "&& mva_score > " + str(lowCut),
#"selection":"global_features[7] > 0.33 && global_features[8] > 0.25 && train_id == 1 && signal_mass_category == 127 && mass > 100 && mass < 180 && mva_score < " + str(highCut)+ "&& mva_score > " + str(lowCut),
"sigName":"ttH_hgg",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/doScan/models/ttH_"+postFix+"_"+date+"/",
"plotpath":"/home/users/hmei/public_html/2019/"+date+"_ttH_"+postFix+"/",
"combineEnv":"/home/users/hmei/CMSSW_7_4_7/src/",
"var":"mass",
"weightVar":"weight",
"mvaName":"mva_score"
#"mvaName":"mva_score"
}

testScan = scanClass(scanConfig)
testScan.cleanDir()


useNCores = 20
nJobsPerCore =5
jobs = []
for i in range(useNCores):
    lowIndex = nJobsPerCore*i
    highIndex = nJobsPerCore*(i+1)
    p = multiprocessing.Process(target=doAllFits_oneCore, args=(lowIndex, highIndex, scanConfig, lowCut, highCut))
    jobs.append(p)
    p.start()

doAllFits(100, scanConfig, lowCut, highCut)
subprocess.call('chmod -R 755 ' + scanConfig["plotpath"], shell=True)


#print limit
#import os
#os.system("echo $CMSSWENV")
#os.system("cat /home/users/hmei/ttH/MakeBDTBins/doScan/models/ttHHad_preapproval_20190926/combineCmd_sig.sh")
#os.system("ls;source /home/users/hmei/ttH/MakeBDTBins/doScan/models/ttHHad_preapproval_20190926/combineCmd_sig.sh")
