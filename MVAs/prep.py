import os, sys
import h5py
import ROOT
import numpy
import root_numpy
import json

import utils
import prep_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input root file", type=str)
parser.add_argument("--channel", help = "Hadronic or Leptonic", type=str, default = "Hadronic")
parser.add_argument("--tag", help = "name to append to filename", type=str, default = "")

parser.add_argument("--signal", help = "csv list of processes to consider as signal", type=str)

parser.add_argument("--bkg", help = "csv list of processes to consider as bkg", type=str)
parser.add_argument("--features", help = "csv list of features to use in training", type=str)

parser.add_argument("--objects", help = "create dset of pt ordered objects", action="store_true")
parser.add_argument("--z_score", help = "preprocess features with z-score transformation", action="store_true")

args = parser.parse_args()

print "[prep.py]: args.input               : %s" % args.input
print "[prep.py]: args.channel             : %s" % args.channel
print "[prep.py]: args.tag                 : %s" % args.tag
print "[prep.py]: args.signal              : %s" % args.signal
print "[prep.py]: args.bkg                 : %s" % args.bkg
print "[prep.py]: args.features            : %s" % args.features
print "[prep.py]: args.objects             : %s" % args.objects


output_file = args.input.replace(".root", "").replace("../Loopers/MVABaby_","") + "_features" + "_" + args.tag + ".hdf5"
z_score_json = "preprocess_scheme_%s_%s.json" % (args.channel, args.tag)

print "[prep.py]: output file              : %s" % output_file
print "[prep.py]: output z-score json      : %s" % z_score_json

# Load features from TTree
f = ROOT.TFile(args.input)
tree = f.Get("t")

training_features = args.features.split(",")
default_branches = ["evt_weight_", "process_id_", "mass_", "evt_", "run_", "lumi_", "year_", "tth_runII_mva_", "signal_mass_category_", "rand_", "signal_mass_label_"]

branches = training_features + default_branches
if args.z_score:
    branches.append("objects_")

signal = args.signal.split(",")
bkg = args.bkg.split(",")

selection_train = prep_utils.selection(signal, bkg, 0)
selection_test  = prep_utils.selection(signal, bkg, 1)
selection_data  = prep_utils.selection(signal, bkg, 2)

features_train = root_numpy.tree2array(tree, branches = branches, selection = selection_train)
features_test = root_numpy.tree2array(tree, branches = branches, selection = selection_test)
features_data = root_numpy.tree2array(tree, branches = branches, selection = selection_data)

features = {
    "train" : features_train,
    "test"  : features_test,
    "data"  : features_data,
}

preprocess_dict = {}
if args.z_score:
    for feat in training_features:
        mean, std = utils.get_mean_and_std(features_train[feat])
        preprocess_dict[feat] = {
            "mean"      : float(mean),
            "std_dev"   : float(std)
        }

    with open(z_score_json, "w") as f_out:
        json.dump(preprocess_dict, f_out, indent=4, sort_keys=True)

f_out = h5py.File(output_file, "w")
f_out.create_dataset("feature_names", data = training_features)

for set in features.keys():
    global_features, label = prep_utils.create_features_and_label(features[set], training_features, signal, bkg, preprocess_dict, args.z_score)

    f_out.create_dataset("global_%s" % set, data = global_features)
    f_out.create_dataset("label_%s" % set, data = label)

    for misc in default_branches: 
        array = features[set][misc]
        f_out.create_dataset("%s_%s" % (misc, set), data = array)

    if args.objects:
        objects = utils.pad_array(features[set]["objects_"])
        f_out.create_dataset("objects_%s" % set, data = objects)

f_out.close()
