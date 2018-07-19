import os, sys
import h5py
import ROOT
import numpy
import root_numpy

baby_file = "../Loopers/MVABaby_ttHHadronic.root"
output_file = "ttHHadronic_features.hdf5"

f = ROOT.TFile(baby_file)
tree = f.Get("t")

# load tree to array
feature_names = (root_numpy.tree2array(tree, branches = ["mva_branches"], start=0, stop=1))[0][0]
feature_names = list(feature_names) 

branches = numpy.concatenate((feature_names, ["evt_weight_", "label_", "process_id_"]))

# grab features
features = root_numpy.tree2array(tree, branches = branches, selection = 'label_ != 2') # 0 = signal, 1 = bkg, 2 = data

# organize features
global_features = [] 
for feature in feature_names:
  global_features.append(features[feature])
global_features = numpy.asarray(global_features)
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
