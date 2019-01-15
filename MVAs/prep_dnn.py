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
output_file = args.input.replace(".root", "").replace("../Loopers/MVABaby_","") + "_dnn_features.hdf5"

f = ROOT.TFile(baby_file)
tree = f.Get("t")

# load tree to array
feature_names = ["objects_", "lead_eta_", "sublead_eta_", "lead_phi_", "sublead_phi_", "leadptoM_", "subleadptoM_", "leadIDMVA_", "subleadIDMVA_"]
branches = numpy.concatenate((feature_names, ["evt_weight_", "label_", "multi_label_", "process_id_", "mass_", "lead_sigmaEtoE_", "sublead_sigmaEtoE_"]))

rand_branch = "rand_"
data_label = 2
train_frac = 0.5

features = root_numpy.tree2array(tree, branches = branches, selection = 'label_ != %d && %s < %.6f' % (data_label, rand_branch, train_frac))
features_validation = root_numpy.tree2array(tree, branches = branches, selection = 'label_ != %d && %s > %.6f' % (data_label, rand_branch, train_frac))
features_data = root_numpy.tree2array(tree, branches = branches, selection = 'label_ == %d' % (data_label))

#features
object_features = features["objects_"]
object_features_validation = features_validation["objects_"]
object_features_data = features_data["objects_"]

global_features = numpy.transpose(numpy.array([features['lead_eta_'], features['sublead_eta_'], features['lead_phi_'], features['sublead_phi_'], features['leadptoM_'], features['subleadptoM_'], features['leadIDMVA_'], features['subleadIDMVA_']]))
global_features_validation = numpy.transpose(numpy.array([features_validation['lead_eta_'], features_validation['sublead_eta_'], features_validation['lead_phi_'], features_validation['sublead_phi_'], features_validation['leadptoM_'], features_validation['subleadptoM_'], features_validation['leadIDMVA_'], features_validation['subleadIDMVA_']]))
global_features_data = numpy.transpose(numpy.array([features_data['lead_eta_'], features_data['sublead_eta_'], features_data['lead_phi_'], features_data['sublead_phi_'], features_data['leadptoM_'], features_data['subleadptoM_'], features_data['leadIDMVA_'], features_data['subleadIDMVA_']]))

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

def pad_array(array):
  max_objects = 18
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
object_features_signal = root_numpy.tree2array(tree, branches = branches, selection = 'label_ == 1')["objects_"]
object_features_signal = pad_array(object_features_signal)

n_objects = 18
n_features = 10

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

label_validation = features_validation["label_"]
multi_label_validation = features_validation["multi_label_"]
weights_validation = features_validation["evt_weight_"]
mass_validation = features_validation["mass_"]

label_data = features_data["label_"]
multi_label_data = features_data["multi_label_"]
weights_data = features_data["evt_weight_"]
mass_data = features_data["mass_"]

# reorganize features
#object_features = numpy.transpose(object_features)
#object_features_validation = numpy.transpose(object_features_validation)
#object_features_data = numpy.transpose(object_features_data)

f_out = h5py.File(output_file, "w")

dset_feature_names = f_out.create_dataset("feature_names", data=feature_names)

dset_object = f_out.create_dataset("object", data=object_features)
dset_global = f_out.create_dataset("global", data=global_features)
dset_label = f_out.create_dataset("label", data=label)
dset_multi_label = f_out.create_dataset("multi_label", data=multi_label)
dset_weights = f_out.create_dataset("weights", data=weights)
dset_mass = f_out.create_dataset("mass", data=mass)
dset_lead_sigmaEtoE = f_out.create_dataset("lead_sigmaEtoE", data=lead_sigmaEtoE)
dset_sublead_sigmaEtoE = f_out.create_dataset("sublead_sigmaEtoE", data=sublead_sigmaEtoE)

dset_object_validation = f_out.create_dataset("object_validation", data=object_features_validation)
dset_global_validation = f_out.create_dataset("global_validation", data=global_features_validation)
dset_label_validation = f_out.create_dataset("label_validation", data=label_validation)
dset_multi_label_validation = f_out.create_dataset("multi_label_validation", data=multi_label_validation)
dset_weights_validation = f_out.create_dataset("weights_validation", data=weights_validation)
dset_mass_validation = f_out.create_dataset("mass_validation", data=mass_validation)

dset_object_data = f_out.create_dataset("object_data", data=object_features_data)
dset_global_data = f_out.create_dataset("global_data", data=global_features_data)
dset_label_data = f_out.create_dataset("label_data", data=label_data)
dset_multi_label_data = f_out.create_dataset("multi_label_data", data=multi_label_data)
dset_weights_data = f_out.create_dataset("weights_data", data=weights_data)
dset_mass_data = f_out.create_dataset("mass_data", data=mass_data)

f_out.close()
