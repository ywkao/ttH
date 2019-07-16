import ROOT
import filenameDict as filenameDict
import processIDMap as processIDMap
import root_numpy
import subprocess

gSystem.AddIncludePath("-I$CMSSW_BASE/src/ ")
#gSystem.Load("$CMSSW_BASE/lib/slc6_amd64_gcc481/libHiggsAnalysisCombinedLimit.so")
gSystem.Load("$CMSSW_BASE/lib/slc6_amd64_gcc630/libHiggsAnalysisCombinedLimit.so")
gSystem.AddIncludePath("-I$ROOFITSYS/include")
gSystem.AddIncludePath("-Iinclude/")
RooMsgService.instance().getStream(1).removeTopic(ROOT.RooFit.DataHandling)
RooMsgService.instance().getStream(1).removeTopic(ROOT.RooFit.ObjectHandling)

class scanClass():

    def __init__(self, config):

        self.debug = False

        self.tag = config["tag"]
        self.selection= config["selection"]
        self.sigName = config["sigName"]
        self.modelpath = config["modelpath"]
        self.plotpath = config["plotpath"]
        self.combineEnv = config["combinEnv"]
        #/home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/

        self.treename = "t"


    def getTree(self):

        self.filename = filenameDict.namedict[self.tag]
        self.file = ROOT.TFile.Open(self.filename)
        self.tree = self.file.Get(self.treename)


    def cleanDir(self):

        pathCmd =  "mkdir -p " + self.modelpath + ";"
        pathCmd += "rm " + self.modelpath+ "*;"
        pathCmd += "mkdir -p " + self.plotpath + ";"
        pathCmd += "rm " + self.plotpath+ "*;"
        pathCmd += "cp ~/public_html/tmpFile/index.php " + self.plotpath


    def quantiles_to_mva_score(self, n_quantiles, selection, mvaName):
        # for given selection, return mva_scores corresponding to each quantile in n_quantiles

        # get a numpy array from tree
        mva_scores = (root_numpy.tree2array(self.tree, branches = [mvaName], selection = selection))

        sorted_mva = numpy.flip(numpy.sort(mva_scores), 0)
        quantiles = []
        mva = []
        for i in range(n_quantiles):
            idx = int((float(i+1) / float(n_quantiles)) * len(sorted_mva)) - 1
            quantiles.append(float(i+1) / float(n_quantiles))
            mva.append(sorted_mva[idx])
        #print mva
        return mva


    def runCombine(self, config):
        # perform finalfit with combine

        combineOption = config["combineOption"]
        #ProfileLikelihood --significance -m 125 -t -1 --expectSignal=1
        #Asymptotic -m 125 -t -1 --expectSignal=1
        combineOutName = config["combineOutname"]
        cardName = config["cardName"]
        outtxtName = config["outtxtName"]
        grepContent = config["grepContent"]

        cmdCombine = "cd " + self.combineEnv + "; eval `scramv1 runtime -sh`; cd -;"
        cmdCombine += "cd " + self.modelPath + ";"

        cmdCombine += "combine -M " + combineOption + " -n " + combineOutName + " " + cardName " > " outtxtName + ";"

        #if "ttH" in sig:
        #    cmdCombine += "combine -M ProfileLikelihood -n sig_" + str(index) + " --significance CMS-HGG_mva_13TeV_datacard_" + str(index) + ".txt -t -1 --expectSignal=1 > sig_" + str(index) + ".txt;"
        #    cmdCombine += "sig=`grep Significance sig_" + str(index) + ".txt`; "
        #    cmdCombine += "echo " + str(index) + " ${sig} " + str(n_bkg0) + " " + str(n_bkg1) + " >> scan2Bin.txt; cd -;"
        #if "FCNC" in sig:
        #    cmdCombine += "combine -M Asymptotic -m 125 CMS-HGG_mva_13TeV_datacard_" + str(index) +  ".txt -t -1 --expectSignal=1 -n limit_" + str(index) + " > limit_" + str(index) + ".txt;"
        #    cmdCombine += "limit=`grep \"Expected 50\" limit_" + str(index) +".txt | awk -F \"<\" '{print $2}'`; "
        #    cmdCombine += "echo " + str(index) + " ${limit} " + str(n_bkg0) + " " + str(n_bkg1) + " >> scan2Bin.txt; cd -;"

        #print cmdCombine
        subprocess.call(cmdCombine, shell=True)

        hosts_process = subprocess.Popen(['grep', grepContent, outtxtName], stdout= subprocess.PIPE)
        hosts_out, hosts_err = hosts_process.communicate()

        output = ( hosts_out.strip("\n").split() )[-1]

        return float(output)
