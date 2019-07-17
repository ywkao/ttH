from scanClass import scanClass
from makeModels import makeModel
from cardMaker import makeCards

# setup scan
scanConfig= {\
"tag":"FCNCHCTHadronicTag",
"selection":"process_id == 23 && mass > 100 && mass < 180",
"sigName":"FCNC_hut",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/doScan/models/test_20190716/",
"plotpath":"/home/users/hmei/public_html/2019/20190716_test/",
"combineEnv","/home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/"
}

testScan = scanClass(scanConfig)
testScan.getTree()
testScan.cleanDir()
mvaScores = testScan.quantiles_to_mva_score(100, "mva_score_0")


# both in sig and bkg model need signame for tag and savename: ttH/TT_FCNC/ggH...
# double check cuts in orignal file
# make signal model
sigModelConfig = {}
sigModelConfig["var"] = "mass"
sigModelConfig["weightVar"] = "weight"
sigModelConfig["selection"] += scanConfig["selection"] + "mva_score_0 > 0.95"
sigModelConfig["plotpath"] = scanConfig["plotpath"]
sigModelConfig["modelpath"] = scanConfig["modelpath"]
sigModelConfig["savename"] = "CMS-HGG_sigfit_mva_" + scanConfig["tag"]
sigModelConfig["tag"] = "hggpdfsmrel_" + scanConfig["tag"]

testSigModel = makeModel(sigModelConfig)
testSigModel.getTree(testScan.tree)
sigConfig = {"replaceNorm":False, "norm_in":"-1", "fixParameters":True}
testSigModel.makeSignalModel("wsig_13TeV", sigConfig)


# make background model
# need to change bkg cut here, below is for signal
bkgModelConfig = {}
bkgModelConfig["var"] = "mass"
bkgModelConfig["weightVar"] = "weight"
bkgModelConfig["selection"] += scanConfig["selection"] + "mva_score_0 > 0.95"
bkgModelConfig["plotpath"] = scanConfig["plotpath"]
bkgModelConfig["modelpath"] = scanConfig["modelpath"]
bkgModelConfig["savename"] = "CMS-HGG_sigfit_mva_" + scanConfig["tag"]
bkgModelConfig["tag"] = "hggpdfsmrel_" + scanConfig["tag"]

testBkgModel = makeModel(bkgModelConfig)
testBkgModel.getTree(testScan.tree)
testBkgModel.makeBackgroundModel("bkg_13TeV")


# make datacard
card = makeCards(scanConfig["modelpath"], "CMS-HGG_mva_13TeV_datacard.txt")
# tag here is the place to get two bdt bins
# postfix is identify ith scan
# the other options in original doScan.py are for sigName, can specify in list
card.WriteCard(sigList, bkgList, args.tags, args.postFix)

# run combine
combineConfig = {}
combineConfig["combineOption"] = "Asymptotic -m 125 -t -1 --expectSignal=1"
combineConfig["cardName"] = ""
combineConfig["outtxtName"] = "limit.txt"
combineConfig["grepContent"] = "\"Expected 50\""

testScan.runCombine(combineConfig)
