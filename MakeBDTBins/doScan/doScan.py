from scanClass import scanClass
from makeModels import makeModel
from cardMaker import makeCards

import sys
import subprocess
# when debug, add exit block by block
# only four blocks to check, not to bad

# setup scan
scanConfig= {\
"tag":"FCNCHCTHadronicTag",
"selection":"process_id == 23 && mass > 100 && mass < 180",
"sigName":"FCNC_hut",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/doScan/models/test_20190716/",
"plotpath":"/home/users/hmei/public_html/2019/20190716_test/",
"combineEnv":"/home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/"
}

testScan = scanClass(scanConfig)
testScan.getTree()
testScan.cleanDir()
mvaScores = testScan.quantiles_to_mva_score(50, "mva_score_0")

print mvaScores

# make signal model
sigModelConfig = {}
sigModelConfig["var"] = "mass"
sigModelConfig["weightVar"] = "weight"
sigModelConfig["selection"] = scanConfig["selection"] + " && mva_score_0 > 0.5"
sigModelConfig["plotpath"] = scanConfig["plotpath"]
sigModelConfig["modelpath"] = scanConfig["modelpath"]
sigModelConfig["savename"] = "CMS-HGG_sigfit_mva_TT_FCNC_hct_" + scanConfig["tag"]
sigModelConfig["tag"] = "hggpdfsmrel_TT_FCNC_hct_" + scanConfig["tag"]

testSigModel = makeModel(sigModelConfig)
testSigModel.getTree(testScan.tree)
sigConfig = {"replaceNorm":False, "norm_in":"-1", "fixParameters":True}
testSigModel.makeSignalModel("wsig_13TeV", sigConfig)

# make background model
bkgModelConfig = {}
bkgModelConfig["var"] = "mass"
bkgModelConfig["weightVar"] = "weight"
bkgModelConfig["selection"] = "sample_id == 0 && process_id != 0 && mva_score_0 > 0.5 && ((mass > 100 && mass < 120) || ( mass > 130 && mass < 180))"
bkgModelConfig["plotpath"] = scanConfig["plotpath"]
bkgModelConfig["modelpath"] = scanConfig["modelpath"]
bkgModelConfig["savename"] = "CMS-HGG_bkg_" + scanConfig["tag"]
bkgModelConfig["tag"] = "CMS_hgg_bkgshape_" + scanConfig["tag"]

testBkgModel = makeModel(bkgModelConfig)
testBkgModel.getTree(testScan.tree)
datasetTag = scanConfig["tag"]
testBkgModel.makeBackgroundModel("wbkg_13TeV", datasetTag)


# make datacard
sigList = ["TT_FCNC_hct"]
bkgList = ["bkg_mass"]
card = makeCards(scanConfig["modelpath"], "CMS-HGG_mva_13TeV_datacard.txt")
# tag here is the place to get two bdt bins
# postfix is identify ith scan
# the other options in original doScan.py are for sigName, can specify in list
#card.WriteCard(sigList, bkgList, args.tags, args.postFix)
card.WriteCard(sigList, bkgList, ["FCNCHCTHadronicTag"], "")

# run combine
combineConfig = {}
combineConfig["combineOption"] = "Asymptotic -m 125 "
combineConfig["combineOutName"] = "limit"
combineConfig["cardName"] = "CMS-HGG_mva_13TeV_datacard.txt"
combineConfig["outtxtName"] = "limit.txt"
combineConfig["grepContent"] = "\"Expected 50\""

limit = testScan.runCombine(combineConfig)
#print limit
#import os
#os.system("echo $CMSSWENV")
#os.system("cat /home/users/hmei/ttH/MakeBDTBins/doScan/models/test_20190716/combineCmd_limit.sh")
##os.system("ls;source /home/users/hmei/ttH/MakeBDTBins/doScan/models/test_20190716/combineCmd_limit.sh")
#sys.exit()
#subprocess.call('chmod -R 755 ' + scanConfig["plotpath"], shell=True)
