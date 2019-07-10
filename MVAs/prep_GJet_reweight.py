import os, sys
import h5py
import ROOT
import numpy
import root_numpy

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input root file", type=str)
args = parser.parse_args()

baby_file = args.input.replace(".root", "") + ".root"
output_file = args.input.replace(".root", "").replace("../Loopers/MVABaby_","") + "_features.hdf5"

f = ROOT.TFile(baby_file)
tree = f.Get("t")

#feature_names = ["mass_", "njets_", "ht_", "jet1_pt_", "jet1_eta_", "max1_btag_", "max2_btag_", "leadIDMVA_", "subleadIDMVA_", "lead_eta_", "sublead_eta_", "leadptoM_", "subleadptoM_"]

#feature_names = ["jet1_pt_"]
feature_names = ["njets_", "ht_", "jet1_pt_", "jet2_pt_", "jet1_eta_", "jet2_eta_", "max1_btag_", "max2_btag_", "leadptoM_", "subleadptoM_", "lead_eta_", "sublead_eta_", "minIDMVA_", "maxIDMVA_", "lead_pT_", "sublead_pT_", "dipho_pt_", "dipho_rapidity_", "dipho_cosphi_", "leadIDMVA_", "subleadIDMVA_"]

label = root_numpy.tree2array(tree, branches = "process_id_")
evt_weight = root_numpy.tree2array(tree, branches = "evt_weight_")
features = root_numpy.tree2array(tree, branches = feature_names)

label = numpy.transpose(label)
for i in range(len(label)):
  if label[i] == 3: # Pythia sample is bkg
    label[i] = 0
  elif label[i] == 17: # Madgraph sample is signal
    label[i] = 1
  else:
     print("Sample other than GJets, shouldn't be here")




features = numpy.transpose(features)
evt_weight = numpy.transpose(evt_weight)


f_out = h5py.File(output_file, "w")
dset_feature_names = f_out.create_dataset("feature_names", data=feature_names)
dset_evt_weight = f_out.create_dataset("evt_weight", data=evt_weight)
dset_features = f_out.create_dataset("features", data=features)
dset_label = f_out.create_dataset("label", data=label)
f_out.close()
