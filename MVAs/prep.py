import os, sys
import h5py
import ROOT
import numpy
import root_numpy

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("input", help = "input root file", type=str)
parser.add_argument("-r", "--randomize", help = "use a random test/train split", action="store_true")
parser.add_argument("-i", "--invert", help = "invert the test/train split", action="store_true")
args = parser.parse_args()

baby_file = args.input.replace(".root", "") + ".root"
output_file = args.input.replace(".root", "").replace("../Loopers/MVABaby_","") + "_features.hdf5"

f = ROOT.TFile(baby_file)
tree = f.Get("t")

# load tree to array
feature_names = (root_numpy.tree2array(tree, branches = ["mva_branches"], start=0, stop=1))[0][0]
feature_names = list(feature_names) 

print feature_names

branches = numpy.concatenate((feature_names, ["evt_weight_", "label_", "process_id_", "mass_"]))

# grab features
train_frac = 0.5
rand_branch = "super_rand_" if args.randomize else "rand_"
features = root_numpy.tree2array(tree, branches = branches, selection = 'label_ != 2 && %s < %.6f' % (rand_branch, train_frac)) # 0 = signal, 1 = bkg, 2 = data
features_validation = root_numpy.tree2array(tree, branches = branches, selection = 'label_ != 2 && %s > %.6f' % (rand_branch, train_frac))

if args.invert: # swap test and train
  features_temp = features
  features = features_validation
  features_validation = features_temp

# organize features
global_features = []
global_features_validation = []
for feature in feature_names:
  global_features.append(features[feature])
  global_features_validation.append(features_validation[feature])
global_features = numpy.asarray(global_features)
global_features_validation = numpy.asarray(global_features_validation)

label = features["label_"]
weights = features["evt_weight_"]
mass = features["mass_"]

label_validation = features_validation["label_"]
weights_validation = features_validation["evt_weight_"]
mass_validation = features_validation["mass_"]

# reorganize features
global_features = numpy.transpose(global_features)
global_features_validation = numpy.transpose(global_features_validation)

f_out = h5py.File(output_file, "w")
dset_feature_names = f_out.create_dataset("feature_names", data=feature_names)
dset_global = f_out.create_dataset("global", data=global_features)
dset_label = f_out.create_dataset("label", data=label)
dset_weights = f_out.create_dataset("weights", data=weights)
dset_mass = f_out.create_dataset("mass", data=mass)
dset_global_validation = f_out.create_dataset("global_validation", data=global_features_validation)
dset_label_validation = f_out.create_dataset("label_validation", data=label_validation)
dset_weights_validation = f_out.create_dataset("weights_validation", data=weights_validation)
dset_mass_validation = f_out.create_dataset("mass_validation", data=mass_validation)

f_out.close()
