from ROOT import *
import numpy
import root_numpy
from subprocess import call
import multiprocessing
import filenameDict as nameDict


def quantiles_to_mva_score(n_quantiles, mva_scores): # return mva_scores corresponding to each quantile in n_quantiles
  sorted_mva = numpy.flip(numpy.sort(mva_scores), 0)
  quantiles = []
  mva = []
  for i in range(n_quantiles):
    idx = int((float(i+1) / float(n_quantiles)) * len(sorted_mva)) - 1
    quantiles.append(float(i+1) / float(n_quantiles))
    mva.append(sorted_mva[idx])
#  return quantiles, mva
  print mva
  return mva


def get_mva_cuts(t, n_quantiles, low, high, process_id):

    #selection_sig = "sample_id == 1 && signal_mass_label == 0"
    selection_sig = "process_id == " + str(process_id) + " && signal_mass_label == 0"
    if low < high:
        selection_sig += " && mva_score > " + str(low) + " && mva_score < " + str(high)
        
    mva_scores = (root_numpy.tree2array(t, branches = ["mva_score"], selection = selection_sig)) 
    #print len(mva_scores), mva_scores
    mva_score_n = quantiles_to_mva_score(n_quantiles, mva_scores)
    return mva_score_n


def doScan_oneCore(lowIndex, highIndex, mva_score_n, lowCut, highCut, tag, modelPath, savepath, sig, fixBin2, checkOneCut=999):
  # the scan is done here
  # if fix second bin, then do scan like [a,b][c,d] -> d = 1, b=c=middleCut, then move a from 0 to b

  for index in range(lowIndex, highIndex, 1):
       
    tag1 = tag + "_0_" + str(index)
    tag2 = tag + "_1_" + str(index)

    if checkOneCut < len(mva_score_n) and index != checkOneCut:
#      print index, checkOneCut
      continue
    
    middleCut = str(mva_score_n[index][0])
    middleCut_ = middleCut
    lowCut_ = lowCut
    highCut_ = highCut

    if fixBin2:
      lowCut_ = middleCut
      middleCut_ = highCut
      highCut_ = 1

    #if index%10 == 0: continue
    #print index
    #print lowCut_, middleCut_, highCut_
 #   import sys
 #   sys.exit()
      
    call("python makeSigBkgShape.py -l " + str(lowCut_) + " --hi " + str(middleCut_) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p " + sig, shell=True)
    call("python makeSigBkgShape.py -l " + str(middleCut_) + " --hi " + str(highCut_) + " --tag " + tag2 + " --modelPath " + modelPath + " --savepath " + savepath + " -p " + sig, shell=True)
#    call("python makeSigBkgShape.py -l " + str(lowCut_) + " --hi " + str(middleCut_) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p ggH_hgg" , shell=True)
#    call("python makeSigBkgShape.py -l " + str(middleCut_) + " --hi " + str(highCut_) + " --tag " + tag2 + " --modelPath " + modelPath + " --savepath " + savepath + " -p ggH_hgg", shell=True)
    if "FCNC" in sig:
      call("python makeSigBkgShape.py -l " + str(lowCut_) + " --hi " + str(middleCut_) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p ttH_hgg" , shell=True)
      call("python makeSigBkgShape.py -l " + str(middleCut_) + " --hi " + str(highCut_) + " --tag " + tag2 + " --modelPath " + modelPath + " --savepath " + savepath + " -p ttH_hgg", shell=True)
    
    cmdMakecard = "python cardMaker.py --postFix _" + str(index) + " --savepath models/" + modelPath + " --tags " + tag + "_0 " + tag + "_1" 
    
    if "FCNC" in sig:
      cmdMakecard += " --doFCNC --FCNCSig " + sig 
    call(cmdMakecard, shell=True)

    cmdCombine = "cd /home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/; eval `scramv1 runtime -sh`; cd -;"
    cmdCombine += "cd models/" + modelPath + ";"
    cmdCombine += "combine -M ProfileLikelihood --significance CMS-HGG_mva_13TeV_datacard_" + str(index) + ".txt -t -1 --expectSignal=1 > sig_" + str(index) + ".txt;"
    cmdCombine += "sig=`grep Significance sig_" + str(index) + ".txt`; "
    cmdCombine += "echo " + str(index) + " ${sig} >> scan2Bin.txt; cd -;"
    print cmdCombine
    call(cmdCombine, shell=True)

def doScan_once(lowCut, highCut, tag, modelPath, savepath, sig):

  tag1 = tag + "_1bin"
  
  call("python makeSigBkgShape.py -l " + str(lowCut) + " --hi " + str(highCut) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p " + sig, shell=True)
  call("python makeSigBkgShape.py -l " + str(lowCut) + " --hi " + str(highCut) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p ggH_hgg" , shell=True)
  if "FCNC" in sig:
    call("python makeSigBkgShape.py -l " + str(lowCut) + " --hi " + str(highCut) + " --tag " + tag + " --modelPath " + modelPath + " --savepath " + savepath + " -p ttH_hgg" , shell=True)
    
  cmdMakecard = "python cardMaker.py --postFix _1bin --savepath models/" + modelPath + " --tags " + tag  
  if "FCNC" in sig:
    cmdMakecard += " --doFCNC " 
  call(cmdMakecard, shell=True)

  cmd1bin = "cd /home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/; eval `scramv1 runtime -sh`; cd -;"
  cmd1bin += "cd models/" + modelPath + ";"
  cmd1bin +=  "combine -M ProfileLikelihood --significance CMS-HGG_mva_13TeV_datacard_1bin.txt -t -1 --expectSignal=1 > sig_1bin.txt;"
  call(cmd1bin, shell=True)

import argparse
def ParseOption():

    parser = argparse.ArgumentParser(description='submit all')
    parser.add_argument("--sigName", dest='sigName', type=str)
    parser.add_argument("--plotpath", dest='plotpath', type=str)
    parser.add_argument("--modelPath", dest='modelPath', type=str)
    parser.add_argument('--tag', dest='tag', type=str)
    parser.add_argument('--lowCut', dest='lowCut', type=float)
    parser.add_argument('--highCut', dest='highCut', type=float)
    parser.add_argument('--useNCores', dest='useNCores', default = 10, type=int)
    parser.add_argument('--n_quantiles', dest='n_quantiles', default = 100, type=int)
    parser.add_argument("--fixBin2", dest="fixBin2", default = False, action="store_true")
    parser.add_argument("--noScan", dest="noScan", default = False, action="store_true")
    parser.add_argument("--scoreIndex", dest="scoreIndex", type=int)
    parser.add_argument("--checkOneCut", dest="checkOneCut", default = 999, type=int)
 
    args = parser.parse_args()
    return args

args=ParseOption()

##tag = "FCNCHadronicTag"
#tag = "FCNCLeptonicTag"
##sigName = "ttH_hgg"
#sigName = "TT_FCNC_hut"
#modelPath = "TT_FCNC_hut_scan_lep"
#plotPath = "/home/users/hmei/public_html/2019/20190605_testScan_TT_FCNC_hut_lep/"
#lowCut = 0
#highCut = 1
#useNCores = 10
#n_quantiles = 100

sigName = args.sigName
plotPath = args.plotpath
modelPath = args.modelPath
tag = args.tag
lowCut = args.lowCut
highCut = args.highCut
useNCores = args.useNCores
n_quantiles = args.n_quantiles
fixBin2 = args.fixBin2

print "tag: ", tag

filename = ""
if "TTHHadronicTag" in tag:
    filename = nameDict.namedict["TTHHadronicTag"] #hadfilename_tth
if "TTHLeptonicTag" in tag:
    filename = nameDict.namedict["TTHLeptonicTag"] #lepfilename_tth
if "FCNCHadronicTag" in tag:
    filename = nameDict.namedict["FCNCHadronicTag"] #hadfilename_fcnc
if "FCNCLeptonicTag" in tag:
    filename = nameDict.namedict["FCNCLeptonicTag"] #lepfilename_fcnc

processIDMap = {"ttH_hgg":0, "ggH_hgg":14, "VBF_hgg":15, "VH_hgg":16, 
                "TT_FCNC_hut":22, "TT_FCNC_hct":23, "ST_FCNC_hut":24, "ST_FCNC_hct":25}

print filename
f = TFile.Open(filename)
t = f.Get("t")

nJobsPerCore = float(n_quantiles)/useNCores 
mva_score_n = get_mva_cuts(t, n_quantiles, lowCut, highCut, processIDMap[sigName])

if n_quantiles%useNCores != 0:
    raise Exception('check n_quantiles and useNCores')

#print mva_score_n[50]
#import sys
#sys.exit()

if not args.noScan:

  jobs = []
  for i in range(useNCores):
    lowIndex = nJobsPerCore*i
    highIndex = nJobsPerCore*(i+1)  
    p = multiprocessing.Process(target=doScan_oneCore, args=(int(lowIndex), int(highIndex), mva_score_n, lowCut, highCut, tag, modelPath, plotPath, sigName, fixBin2, args.checkOneCut))
    jobs.append(p)
    p.start()

  if fixBin2:
    lowCut = highCut
    highCut = 1

  print lowCut, highCut
  if args.checkOneCut > len(mva_score_n):
    doScan_once(lowCut, highCut, tag, modelPath, plotPath, sigName)

else:
  print "bestCut at: ", mva_score_n[args.scoreIndex]
