import os, sys
import h5py
import ROOT
import numpy
import root_numpy


baby_file = "../Loopers/MVABaby_ttHHadronic.root"
output_file = "ttHHadronic_features.hdf5"
branches_hadronic = ["evt_weight_", "label_", "process_id_", "njets_", "ht_", "leadPt_", "subleadPt_", "maxIDMVA_", "minIDMVA_", "jet1_pt_", "jet1_eta_", "jet2_pt_", "jet2_eta_", "jet3_pt_", "jet3_eta_", "jet4_pt_", "jet4_eta_", "max1_btag_", "max2_btag_", "leadPSV_", "subleadPSV_", "dipho_cosphi_", "dipho_rapidity_", "met_"]

f = ROOT.TFile(baby_file)
tree = f.Get("t")

# load tree to array
features = root_numpy.tree2array(tree, branches = branches_hadronic, selection = 'label_ != 2') # 0 = signal, 1 = bkg, 2 = data

# grab features
feature_names = numpy.array(["njets_", "ht_", "leadPt_", "subleadPt_", "maxIDMVA_", "minIDMVA_", "jet1_pt_", "jet1_eta_", "jet2_pt_", "jet2_eta_", "jet3_pt_", "jet3_eta_", "jet4_pt_", "jet4_eta_", "max1_btag_", "max2_btag_", "leadPSV_", "subleadPSV_", "dipho_cosphi_", "dipho_rapidity_", "met_"])
global_features = [] 
for feature in feature_names:
  global_features.append(features[feature])
global_features = numpy.asarray(global_features)
#global_features = numpy.array([features["njets_"], features["nbjets_"], features["ht_"], features["leadPt_"], features["subleadPt_"], features["maxIDMVA_"], features["minIDMVA_"]])
label = features["label_"]
weights = features["evt_weight_"]

# reorganize features
global_features = numpy.transpose(global_features)

f_out = h5py.File(output_file, "w")
dset_feature_names = f_out.create_dataset("feature_names", data=feature_names)
dset_global = f_out.create_dataset("global", data=global_features)
dset_label = f_out.create_dataset("label", data=label)
dset_weights = f_out.create_dataset("weights", data=weights)

f_out.close()
