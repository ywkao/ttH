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

# get a set of cuts on mva scores, in each interval the number of signals are the same to make scan more efficient
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
      continue

    # this is the default cuts setup, move cut between a low and high bound
    # move cut between low and high [0, x], [x, 1], find optimal cut x
    middleCut = str(mva_score_n[index][0])
    middleCut_ = middleCut
    lowCut_ = lowCut
    highCut_ = highCut

    # this is for the following situation:
    # 1, a middle cut has been determined at x
    # 2, try to find where is the lower bound, meaning find the lower cut that decide which events going to finalfit: start with [0, x], [x, 1], then move cut between 0 and x
    if fixBin2:
      lowCut_ = middleCut
      middleCut_ = highCut
      highCut_ = 1

    # start to make signal and background models according to cut configuration
    # from here until making datacard can be absorbed into some function
    call("python makeSigBkgShape.py -l " + str(lowCut_) + " --hi " + str(middleCut_) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p " + sig, shell=True)
    call("python makeSigBkgShape.py -l " + str(middleCut_) + " --hi " + str(highCut_) + " --tag " + tag2 + " --modelPath " + modelPath + " --savepath " + savepath + " -p " + sig, shell=True)
    call("python makeSigBkgShape.py -l " + str(lowCut_) + " --hi " + str(middleCut_) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p ggH_hgg --skipBkg" , shell=True)
    call("python makeSigBkgShape.py -l " + str(middleCut_) + " --hi " + str(highCut_) + " --tag " + tag2 + " --modelPath " + modelPath + " --savepath " + savepath + " -p ggH_hgg --skipBkg", shell=True)

    if "FCNC" in sig:
      call("python makeSigBkgShape.py -l " + str(lowCut_) + " --hi " + str(middleCut_) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " --skipBkg -p " + sig.replace("TT","ST"), shell=True)
      call("python makeSigBkgShape.py -l " + str(middleCut_) + " --hi " + str(highCut_) + " --tag " + tag2 + " --modelPath " + modelPath + " --savepath " + savepath + " --skipBkg -p " + sig.replace("TT","ST"), shell=True)
      call("python makeSigBkgShape.py -l " + str(lowCut_) + " --hi " + str(middleCut_) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " --skipBkg -p ttH_hgg" , shell=True)
      call("python makeSigBkgShape.py -l " + str(middleCut_) + " --hi " + str(highCut_) + " --tag " + tag2 + " --modelPath " + modelPath + " --savepath " + savepath + " --skipBkg -p ttH_hgg", shell=True)

#
    # make datacard for finalfit
    cmdMakecard = "python cardMaker.py --postFix _" + str(index) + " --savepath models/" + modelPath + " --tags " + tag + "_0 " + tag + "_1"

    if "FCNC" in sig:
        cmdMakecard += " --doFCNC --doMultiSig --FCNCSig " + sig

    print cmdMakecard
    call(cmdMakecard, shell=True)
#    import sys
#    sys.exit()

    f_bkg0 = open("models/" + modelPath + "/CMS-HGG_bkg_" + tag + "_0_" + str(index) + ".txt")
    f_bkg1 = open("models/" + modelPath + "/CMS-HGG_bkg_" + tag + "_1_" + str(index) + ".txt")
    n_bkg0 = ( (f_bkg0.readline()).split('\n') )[0]
    n_bkg1 = ( (f_bkg1.readline()).split('\n') )[0]

    # perform finalfit with combine
    cmdCombine = "cd /home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/; eval `scramv1 runtime -sh`; cd -;"
    cmdCombine += "cd models/" + modelPath + ";"

    if "ttH" in sig:
        cmdCombine += "combine -M ProfileLikelihood -n sig_" + str(index) + " --significance CMS-HGG_mva_13TeV_datacard_" + str(index) + ".txt -t -1 --expectSignal=1 > sig_" + str(index) + ".txt;"
        cmdCombine += "sig=`grep Significance sig_" + str(index) + ".txt`; "
        cmdCombine += "echo " + str(index) + " ${sig} " + str(n_bkg0) + " " + str(n_bkg1) + " >> scan2Bin.txt; cd -;"
    if "FCNC" in sig:
        cmdCombine += "combine -M Asymptotic -m 125 CMS-HGG_mva_13TeV_datacard_" + str(index) +  ".txt -t -1 --expectSignal=1 -n limit_" + str(index) + " > limit_" + str(index) + ".txt;"
        cmdCombine += "limit=`grep \"Expected 50\" limit_" + str(index) +".txt | awk -F \"<\" '{print $2}'`; "
        cmdCombine += "echo " + str(index) + " ${limit} " + str(n_bkg0) + " " + str(n_bkg1) + " >> scan2Bin.txt; cd -;"

    #print cmdCombine
    call(cmdCombine, shell=True)

def doScan_once(lowCut, highCut, tag, modelPath, savepath, sig):

  tag1 = tag + "_1bin"

  # make signal and background models
  call("python makeSigBkgShape.py -l " + str(lowCut) + " --hi " + str(highCut) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p " + sig, shell=True)
  call("python makeSigBkgShape.py -l " + str(lowCut) + " --hi " + str(highCut) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p ggH_hgg --skipBkg" , shell=True)
  if "FCNC" in sig:
    call("python makeSigBkgShape.py -l " + str(lowCut) + " --hi " + str(highCut) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p ttH_hgg --skipBkg" , shell=True)
    call("python makeSigBkgShape.py -l " + str(lowCut) + " --hi " + str(highCut) + " --tag " + tag1 + " --modelPath " + modelPath + " --savepath " + savepath + " -p " + sig.replace("TT","ST") + " --skipBkg" , shell=True)

  # make datacard
  cmdMakecard = "python cardMaker.py --postFix _1bin --savepath models/" + modelPath + " --tags " + tag
  if "FCNC" in sig:
    cmdMakecard += " --doFCNC --doMultiSig --FCNCSig " + sig
  call(cmdMakecard, shell=True)

  # perform finalfit with combine
  cmd1bin = "cd /home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/; eval `scramv1 runtime -sh`; cd -;"
  cmd1bin += "cd models/" + modelPath + ";"
  cmd1bin += "echo 1 bin scan from " + str(lowCut) + " to " + str(highCut) + " > result_scan.txt;"
  if "ttH" in sig:
    cmd1bin +=  "combine -M ProfileLikelihood --significance CMS-HGG_mva_13TeV_datacard_1bin.txt -t -1 --expectSignal=1 >> result_scan.txt;"
  if "FCNC" in sig:
    cmd1bin += "combine -M Asymptotic -m 125 CMS-HGG_mva_13TeV_datacard_1bin.txt -t -1 --expectSignal=1 >> result_scan.txt;"
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
if "FCNCHUTHadronicTag" in tag:
    filename = nameDict.namedict["FCNCHUTHadronicTag"] #hadfilename_fcnc
if "FCNCHUTLeptonicTag" in tag:
    filename = nameDict.namedict["FCNCHUTLeptonicTag"] #lepfilename_fcnc
if "FCNCHCTHadronicTag" in tag:
    filename = nameDict.namedict["FCNCHCTHadronicTag"] #hadfilename_fcnc
if "FCNCHCTLeptonicTag" in tag:
    filename = nameDict.namedict["FCNCHCTLeptonicTag"] #lepfilename_fcnc

processIDMap = {"ttH_hgg":0, "ggH_hgg":14, "VBF_hgg":15, "VH_hgg":16, "THQ_hgg":11, "THW_hgg":12,
                "TT_FCNC_hut":22, "TT_FCNC_hct":23, "ST_FCNC_hut":24, "ST_FCNC_hct":25}

print filename
f = TFile.Open(filename)
t = f.Get("t")

nJobsPerCore = float(n_quantiles)/useNCores
mva_score_n = get_mva_cuts(t, n_quantiles, lowCut, highCut, processIDMap[sigName])

if n_quantiles%useNCores != 0:
    raise Exception('check n_quantiles and useNCores')

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
