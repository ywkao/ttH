import os, sys
import h5py
import ROOT
import numpy
import root_numpy
import math

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input root file", type=str)
parser.add_argument("--signal", help = "which process to consider as signal", type=str, default="ttH")
parser.add_argument("--backgrounds", help = "which processes to consider as bkgs", type=str, default="all")
parser.add_argument("--boosted_objects", help = "use objects that are boosted to higgs p4", action="store_true")
parser.add_argument("--do_top_tag", help = "use top tagger BDT in training", action="store_true")
parser.add_argument("--invert", help = "invert test/train split", action="store_true")
parser.add_argument("--train_frac", help = "what fraction of data to use for training", type = float, default = 0.5)
parser.add_argument("--tag", help = "name to add to hdf5 file name", type=str, default="")
parser.add_argument("--z_score", help = "preprocess features to express them as z-score (subtract mean, divide by std.)", action="store_true")
args = parser.parse_args()

baby_file = args.input.replace(".root", "") + ".root"
output_file = args.input.replace(".root", "").replace("../Loopers/MVABaby_","") + "_dnn_features_" + args.tag + ".hdf5"

f = ROOT.TFile(baby_file)
tree = f.Get("t")

# load tree to array
#feature_names = ["objects_", "objects_boosted_", "lead_eta_", "sublead_eta_", "lead_phi_", "sublead_phi_", "leadptoM_", "subleadptoM_", "maxIDMVA_", "minIDMVA_", "met_", "met_phi_", "leadPSV_", "subleadPSV_", "dipho_rapidity_", "dipho_pt_over_mass_", "dipho_delta_R"]
feature_names = ["objects_", "objects_boosted_", "lead_eta_", "sublead_eta_", "lead_phi_", "sublead_phi_", "leadptoM_", "subleadptoM_", "maxIDMVA_", "minIDMVA_", "log_met_", "met_phi_", "leadPSV_", "subleadPSV_", "dipho_rapidity_", "dipho_pt_over_mass_", "dipho_delta_R", "max1_btag_", "max2_btag_", "njets_"]
branches = numpy.concatenate((feature_names, ["evt_weight_", "label_", "multi_label_", "process_id_", "mass_", "lead_sigmaEtoE_", "sublead_sigmaEtoE_", "top_tag_score_", "tth_ttPP_mva_", "tth_std_mva_", "tth_dipho_mva_"]))

rand_branch = "rand_"
data_label = 2
train_frac = args.train_frac

if args.backgrounds == "all":
  selection = ""
else:
  process_dict = {"ttH" : 0, "ttGG" : 5, "dipho" : 2}
  selection = "&& ("
  procs = args.backgrounds.split(",") + args.signal.split(",")
  for i in range(len(procs)):
    selection += "process_id_ == %d" % (process_dict[procs[i]])
    if i != len(procs) - 1:
      selection += " || "

  selection += ")"


print selection

features = root_numpy.tree2array(tree, branches = branches, selection = '((label_ == 0) || (label_ == 1 && signal_mass_label_ != 0)) && %s %s %.6f %s' % (rand_branch, ">" if args.invert else "<", train_frac, selection))
features_validation = root_numpy.tree2array(tree, branches = branches, selection = '((label_ == 0) || (label_ == 1 && signal_mass_label_ == 1)) && %s %s %.6f %s' % (rand_branch, "<" if args.invert else ">", train_frac, selection))
#features = root_numpy.tree2array(tree, branches = branches, selection = 'label_ != %d && %s < %.6f %s' % (data_label, rand_branch, train_frac, selection))
#features_validation = root_numpy.tree2array(tree, branches = branches, selection = 'label_ != %d && %s > %.6f %s' % (data_label, rand_branch, train_frac, selection))
features_data = root_numpy.tree2array(tree, branches = branches, selection = 'label_ == %d' % (data_label))


if args.boosted_objects:
  object_features = features["objects_boosted_"]
  object_features_validation = features_validation["objects_boosted_"]
  object_features_data = features_data["objects_boosted_"]

else:
  object_features = features["objects_"]
  object_features_validation = features_validation["objects_"]
  object_features_data = features_data["objects_"]

def preprocess_sigmoid_v2(array, mean, std):
  alpha = (array - mean)*(1./std)
  return (1 - numpy.exp(-alpha))/(1 + numpy.exp(-alpha))

def preprocess_sigmoid(array):
  array_trimmed = array[array != -999]
  mean = numpy.mean(array_trimmed)
  std = numpy.std(array_trimmed)
  if std == 0:
    std = 1
  alpha = (array - mean)*(1./std)
  return (1 - numpy.exp(-alpha))/(1 + numpy.exp(-alpha))

def create_array(features_list, names):
  print [feat for feat in names if ("objects_" not in feat)]
  arr = [features_list[feat] for feat in names if ("objects_" not in feat)]
  if args.z_score:
    for feat in arr:
      feat = preprocess(feat)
  return numpy.transpose(numpy.array(arr))

if args.do_top_tag:
  feature_names += ["top_tag_score_"]
global_features = create_array(features, feature_names)
global_features_validation = create_array(features_validation, feature_names)
global_features_data = create_array(features_data, feature_names)

def get_mean_and_std(array):
  array_trimmed = array[array != -999]
  if (len(array_trimmed) <= 1):
    return 0, 1
  mean = numpy.mean(array_trimmed)
  std = numpy.std(array_trimmed)
  if std == 0:
    std = 1
  return mean, std

def preprocess(array, mean, std):
  array[array != -999] += -mean
  array[array != -999] *= 1./std
  return array

n_objects = 8
def pad_array(array):
  max_objects = n_objects
  nData = len(array)
  nFeatures = len(array[0][0])
  y = numpy.ones((nData, max_objects, nFeatures))
  y *= -999
  for i in range(nData):
    for j in range(min(max_objects, len(array[i]))):
      for k in range(nFeatures):
        y[i][j][k] = array[i][j][k]

  return y


object_features = pad_array(object_features)
object_features_validation = pad_array(object_features_validation)
object_features_data = pad_array(object_features_data)

# use set of all signal events to get mean and std dev for preprocessing purposes
#object_features_signal = root_numpy.tree2array(tree, branches = branches, selection = 'label_ == 1')["objects_"]
#object_features_signal = pad_array(object_features_signal)

#for i in range(n_objects):
#  for j in range(n_features):
#    for k in range(5):
#      print object_features[k][i][j]

#for i in range(n_objects):
#  for j in range(n_features):
#    mean, std = get_mean_and_std(object_features_signal[:,i,j])
#    print "Mean, std dev of object %d, feature %d: %.5f, %.5f" % (i, j, mean, std)
#    object_features[:,i,j] = preprocess(object_features[:,i,j], mean, std)
#    object_features_validation[:,i,j] = preprocess(object_features_validation[:,i,j], mean, std)
#    object_features_data[:,i,j] = preprocess(object_features_data[:,i,j], mean, std)

#for i in range(n_objects):
#  for j in range(n_features):
#    for k in range(5):
#      print object_features[k][i][j]

label = features["label_"]
multi_label = features["multi_label_"]
weights = features["evt_weight_"]
mass = features["mass_"]
lead_sigmaEtoE = features["lead_sigmaEtoE_"]
sublead_sigmaEtoE = features["sublead_sigmaEtoE_"]
top_tag_score = features["top_tag_score_"]
tth_ttPP_mva = features["tth_ttPP_mva_"]
tth_dipho_mva = features["tth_dipho_mva_"]
tth_std_mva = features["tth_std_mva_"]

label_validation = features_validation["label_"]
multi_label_validation = features_validation["multi_label_"]
weights_validation = features_validation["evt_weight_"]
mass_validation = features_validation["mass_"]
top_tag_score_validation = features_validation["top_tag_score_"]
tth_ttPP_mva_validation = features_validation["tth_ttPP_mva_"]
tth_dipho_mva_validation = features_validation["tth_dipho_mva_"]
tth_std_mva_validation = features_validation["tth_std_mva_"]

label_data = features_data["label_"]
multi_label_data = features_data["multi_label_"]
weights_data = features_data["evt_weight_"]
mass_data = features_data["mass_"]
top_tag_score_data = features_data["top_tag_score_"]
tth_ttPP_mva_data = features_data["tth_ttPP_mva_"]
tth_dipho_mva_data = features_data["tth_dipho_mva_"]
tth_std_mva_data = features_data["tth_std_mva_"]

# reorganize features
#object_features = numpy.transpose(object_features)
#object_features_validation = numpy.transpose(object_features_validation)
#object_features_data = numpy.transpose(object_features_data)

# relabel labels
if args.backgrounds != "all":
  for sig in args.signal.split(","):
    label[features["process_id_"] == process_dict[sig]] = 1
    label_validation[features_validation["process_id_"] == process_dict[sig]] = 1

  for bkg in args.backgrounds.split(","):
    label[features["process_id_"] == process_dict[bkg]] = 0
    label_validation[features_validation["process_id_"] == process_dict[bkg]] = 0

f_out = h5py.File(output_file, "w")

dset_feature_names = f_out.create_dataset("feature_names", data=feature_names)

dset_object = f_out.create_dataset("object", data=object_features)
dset_global = f_out.create_dataset("global", data=global_features)
dset_label = f_out.create_dataset("label", data=label)
dset_multi_label = f_out.create_dataset("multi_label", data=multi_label)
dset_weights = f_out.create_dataset("weights", data=weights)
dset_mass = f_out.create_dataset("mass", data=mass)
dset_tth_ttPP_mva = f_out.create_dataset("tth_ttPP_mva", data=tth_ttPP_mva)
dset_tth_dipho_mva = f_out.create_dataset("tth_dipho_mva", data=tth_dipho_mva)
dset_tth_std_mva = f_out.create_dataset("tth_std_mva", data=tth_std_mva)
dset_top_tag_score = f_out.create_dataset("top_tag_score", data=top_tag_score)
dset_lead_sigmaEtoE = f_out.create_dataset("lead_sigmaEtoE", data=lead_sigmaEtoE)
dset_sublead_sigmaEtoE = f_out.create_dataset("sublead_sigmaEtoE", data=sublead_sigmaEtoE)

dset_object_validation = f_out.create_dataset("object_validation", data=object_features_validation)
dset_global_validation = f_out.create_dataset("global_validation", data=global_features_validation)
dset_label_validation = f_out.create_dataset("label_validation", data=label_validation)
dset_multi_label_validation = f_out.create_dataset("multi_label_validation", data=multi_label_validation)
dset_weights_validation = f_out.create_dataset("weights_validation", data=weights_validation)
dset_top_tag_score_validation = f_out.create_dataset("top_tag_score_validation", data=top_tag_score_validation)
dset_mass_validation = f_out.create_dataset("mass_validation", data=mass_validation)
dset_tth_ttPP_mva_validation = f_out.create_dataset("tth_ttPP_mva_validation", data=tth_ttPP_mva_validation)
dset_tth_dipho_mva_validation = f_out.create_dataset("tth_dipho_mva_validation", data=tth_dipho_mva_validation)
dset_tth_std_mva_validation = f_out.create_dataset("tth_std_mva_validation", data=tth_std_mva_validation)

dset_object_data = f_out.create_dataset("object_data", data=object_features_data)
dset_global_data = f_out.create_dataset("global_data", data=global_features_data)
dset_label_data = f_out.create_dataset("label_data", data=label_data)
dset_multi_label_data = f_out.create_dataset("multi_label_data", data=multi_label_data)
dset_weights_data = f_out.create_dataset("weights_data", data=weights_data)
dset_top_tag_score_data = f_out.create_dataset("top_tag_score_data", data=top_tag_score_data)
dset_mass_data = f_out.create_dataset("mass_data", data=mass_data)
dset_tth_ttPP_mva_data = f_out.create_dataset("tth_ttPP_mva_data", data=tth_ttPP_mva_data)
dset_tth_dipho_mva_data = f_out.create_dataset("tth_dipho_mva_data", data=tth_dipho_mva_data)
dset_tth_std_mva_data = f_out.create_dataset("tth_std_mva_data", data=tth_std_mva_data)

f_out.close()
