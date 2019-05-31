from ROOT import *
import numpy
import root_numpy
from subprocess import call
import multiprocessing

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


def get_mva_cuts(n_quantiles, low, high):

    selection_sig = "sample_id == 1 && signal_mass_label == 0"
    if low < high:
        selection_sig += " && mva_score > " + str(low) + " && mva_score < " + str(high)
        
    mva_scores = (root_numpy.tree2array(t, branches = ["mva_score"], selection = selection_sig)) 
    #print len(mva_scores), mva_scores
    mva_score_n = quantiles_to_mva_score(n_quantiles, mva_scores)
    return mva_score_n


def doScan_oneCore(lowIndex, highIndex, mva_score_n, lowCut, highCut, tag, modelPath):
# the scan is done here
    for index in range(lowIndex, highIndex, 1):
#        print lowCut, mva_score_n[index][0], highCut
#        print "python makeSigShape.py -l " + str(lowCut) + " --hi " + str(mva_score_n[index][0]) + " -b 0 -i " + str(index) + " --tag " + tag + " --modelPath " + modelPath
        call("python makeSigBkgShape.py -l " + str(lowCut) + " --hi " + str(mva_score_n[index][0]) + " -b 0 -i " + str(index) + " --tag " + tag + " --modelPath " + modelPath, shell=True)
        call("python makeSigBkgShape.py -l " + str(mva_score_n[index][0]) + " --hi " + str(highCut) + " -b 1 -i " + str(index) + " --tag " + tag + " --modelPath " + modelPath, shell=True)
    
# had
hadfilename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.6_28May2019_RunII_MVA_Presel_impute_addDNNs_addTopTag_FinalFitTree.root"
# lep
lepfilename = "/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root"
filename = lepfilename

tag = "TTHLeptonicTag"

f = TFile.Open(filename)
t = f.Get("t")

useNCores = 10
n_quantiles = 100
nJobsPerCore = float(n_quantiles)/useNCores 

#mva_score_100 = get_mva_cuts(n_quantiles, 0, 1)
#mva_score_100 = get_mva_cuts(n_quantiles, 0.96945995, 1)
mva_score_100 = get_mva_cuts(n_quantiles, 0,0.96945995)

if n_quantiles%useNCores != 0:
    raise Exception('check n_quantiles and useNCores')


print mva_score_100[40]
import sys
sys.exit()

jobs = []
for i in range(useNCores):
    lowIndex = nJobsPerCore*i
    highIndex = nJobsPerCore*(i+1)  
    p = multiprocessing.Process(target=doScan_oneCore, args=(int(lowIndex), int(highIndex), mva_score_100, 0, 1, tag, "testStep2"))
    jobs.append(p)
    p.start()
