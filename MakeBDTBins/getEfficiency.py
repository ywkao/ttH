from ROOT import *
import root_numpy
import numpy

def quantiles_to_mva_score(n_quantiles, mva_scores): # return mva_scores corresponding to each quantile in n_quantiles
  sorted_mva = numpy.flip(numpy.sort(mva_scores), 0)
  quantiles = []
  mva = []
  for i in range(n_quantiles):
    idx = int((float(i+1) / float(n_quantiles)) * len(sorted_mva)) - 1
    quantiles.append(float(i+1) / float(n_quantiles))
    mva.append(sorted_mva[idx])
#  return quantiles, mva
#  print mva
  return mva

def get_mva_cuts2(t, n_quantiles, low, high, process_cut):

    selection_sig = process_cut + " && signal_mass_label == 0 "
    if low < high:
        selection_sig += " && mva_score > " + str(low) + " && mva_score < " + str(high)
        #selection_sig += " && tth_2017_reference_mva > " + str(low) + " && tth_2017_reference_mva < " + str(high)

    mva_scores = (root_numpy.tree2array(t, branches = ["mva_score"], selection = selection_sig))
    #mva_scores = (root_numpy.tree2array(t, branches = ["tth_2017_reference_mva"], selection = selection_sig))
    #print len(mva_scores), mva_scores
    mva_score_n = quantiles_to_mva_score(n_quantiles, mva_scores)
    return mva_score_n


# get a set of cuts on mva scores, in each interval the number of signals are the same to make scan more efficient
def get_mva_cuts(t, n_quantiles, low, high, process_id):

    selection_sig = "process_id == " + str(process_id) + " && signal_mass_label == 0"
    if low < high:
        #selection_sig += " && mva_score > " + str(low) + " && mva_score < " + str(high)
        selection_sig += " && tth_2017_reference_mva > " + str(low) + " && tth_2017_reference_mva < " + str(high)

    #mva_scores = (root_numpy.tree2array(t, branches = ["mva_score"], selection = selection_sig))
    mva_scores = (root_numpy.tree2array(t, branches = ["tth_2017_reference_mva"], selection = selection_sig))
    #print len(mva_scores), mva_scores
    mva_score_n = quantiles_to_mva_score(n_quantiles, mva_scores)
    return mva_score_n


def getEfficiency(filename,cut1,cut2):

    f_ = TFile.Open(filename)
    t_ = f_.Get("t")

    h1 = TH1F("h1","",80,100,180)
    t_.Project(h1.GetName(), "mass", "weight*(mass>100 && mass < 180 && mva_score > " + str(cut1) + " && sample_id == 1 && signal_mass_label == 0)")

    h2 = TH1F("h2","",80,100,180)
    t_.Project(h2.GetName(), "mass", "weight*(mass>100 && mass < 180 && mva_score > " + str(cut2) + " && sample_id == 1 && signal_mass_label == 0)")

    eff = h2.Integral()/h1.Integral()
    print "efficiency: ", eff, h2.Integral(), h1.Integral()

    return eff

#filename1 = "/home/users/hmei/ttH/MVAs/ttHHadronic_withPtOMggCut_FinalFitTree.root"
#filename2 = "/home/users/hmei/ttH/MVAs/ttHHadronic_noPtOMggCut_FinalFitTree.root"
#getEfficiency(filename1, 0, 0.9587)
#getEfficiency(filename2, 0, 0.98)

filename_fcncHad = "/home/users/sjmay/ttH/MVAs/ttHHadronic_v1.7_4Jul2019_scaleByXs_impute_hut_FinalFitTree.root"
f_fcncHad = TFile.Open(filename_fcncHad)
tree_fcncHad = f_fcncHad.Get("t")

cuts_had = get_mva_cuts(tree_fcncHad, nQuantile, -1, 1, 0)

#filename_ttHHad = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.7_19Jun2019_RunII_MVA_Presel_impute_addDNNs_addTopTag_addYear_FinalFitTree.root"
#f_ttHHad = TFile.Open(filename_ttHHad)
#tree_ttHHad = f_ttHHad.Get("t")
#
#filename_ttHLep = "/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.7_19Jun2019_RunII_MVA_Presel_addDNN_addYear_FinalFitTree.root"
#f_ttHLep = TFile.Open(filename_ttHLep)
#tree_ttHLep = f_ttHLep.Get("t")
#
#hadCuts = [0.9675, 0.9937, 0.9971, 0.9991]
#lepCuts = [0.8435, 0.9346, 0.9625, 0.9890]
#
#nQuantile = 100000
## get cuts from 2017 PAS BDT
#cuts_had = get_mva_cuts(tree_ttHHad, nQuantile, -1, 1, 0)
#cuts_lep = get_mva_cuts(tree_ttHLep, nQuantile, -1, 1, 0)
#
#for cut in lepCuts:
#    # get efficiency from our Run2 BDT
#    eff = getEfficiency(filename_ttHLep, 0, cut)
#    # get cuts using 2017 PAS BDT
#    print cut, cuts_lep[int((eff)*nQuantile)]
#
#for cut in hadCuts:
#    eff = getEfficiency(filename_ttHHad, 0, cut)
#    print cut, cuts_had[int((eff)*nQuantile)]

#f = TFile.Open(filename1)
#t = f.Get("t")
#cuts = get_mva_cuts(t, 10000, 0, 1, 0)
#print 0.9955, 0.9955*10000, cuts[1-int(0.9955*10000)]

