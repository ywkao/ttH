import os, sys
import h5py
import ROOT
import numpy
import root_numpy

import dnn_helper

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input root file", type=str)
parser.add_argument("--channel", help = "Hadronic or Leptonic", type=str, default = "Hadronic")
parser.add_argument("--tag", help = "name to append to filename", type=str, default = "")

parser.add_argument("--dnn_models", help = "csv list of dnn models to add as inputs", type=str, default = "")
parser.add_argument("--do_top_tag", help = "add top tag score", action = "store_true")

parser.add_argument("-r", "--randomize", help = "use a random test/train split", action="store_true")
parser.add_argument("-i", "--invert", help = "invert the test/train split", action="store_true")
parser.add_argument("-s", "--sideband", help = "use data sideband for training", action = "store_true")

parser.add_argument("--train_frac", help = "what fraction of data to use for training", type = float, default = 0.5)
parser.add_argument("--handicap", help = "use only 1/3 of training data (to get a feel for how much stats matter)", action = "store_true")
parser.add_argument("--sideband_name", help = "which data sideband to use", type=str, default="none")
parser.add_argument("--old_vars", help = "remove top tagger, delta r of photons, and helicity", action="store_true")
parser.add_argument("--no_psv", help = "remove pixel seed veto", action = "store_true")
parser.add_argument("--dipho_only", help = "use only diphoton as bkg", action = "store_true")
parser.add_argument("--ttGG_only", help = "use only ttGG as bkg", action = "store_true")
parser.add_argument("--no_lepton_id", help = "don't use lepton ID vars", action = "store_true")
parser.add_argument("--one_mass_point", help = "only use one signal mass point in training", action = "store_true")

args = parser.parse_args()

baby_file = args.input.replace(".root", "") + ".root"
output_file = args.input.replace(".root", "").replace("../Loopers/MVABaby_","") + "_features" + args.tag + "%s%s%s%s.hdf5" % ("_invert" if args.invert else "", "_remove_new_vars" if args.old_vars else "", "_noPSV" if args.no_psv else "", "noLepID" if args.no_lepton_id else "")

print output_file

f = ROOT.TFile(baby_file)
tree = f.Get("t")

# load tree to array
#feature_names = (root_numpy.tree2array(tree, branches = ["mva_branches"], start=0, stop=1))[0][0]
#feature_names = list(feature_names) 

feature_names = ["maxIDMVA_", "minIDMVA_", "max2_btag_", "max1_btag_", "dipho_delta_R", "njets_", "ht_", "leadptoM_", "subleadptoM_", "lead_eta_", "sublead_eta_", "jet1_pt_", "jet1_eta_", "jet1_btag_", "jet2_pt_", "jet2_eta_", "jet2_btag_", "jet3_pt_", "jet3_eta_", "jet3_btag_", "jet4_pt_", "jet4_eta_", "jet4_btag_", "leadPSV_", "subleadPSV_", "dipho_cosphi_", "dipho_rapidity_", "met_", "dipho_pt_over_mass_", "helicity_angle_"] 

if args.do_top_tag:
  feature_names += ["top_tag_score_"]

to_remove = []
if args.channel == "Leptonic":
  feature_names += ["lep_pt_", "lep_eta_", "n_lep_tight_"]
  #feature_names += ["lep_pt_", "lep_eta_", "n_lep_loose_", "n_lep_medium_", "n_lep_tight_", "muon1_mini_iso_", "muon2_mini_iso_"]
  if args.no_lepton_id:
    to_remove += ["n_lep_tight_"]
    #to_remove += ["n_lep_loose_", "n_lep_medium_", "n_lep_tight_", "muon1_mini_iso_", "muon2_mini_iso_"] 
  to_remove += ["jet4_pt_", "jet4_eta_", "jet4_btag_"]

if args.sideband: # remove b-tagging features
  if args.sideband_name == "0b":
    to_remove += ["max2_btag_", "max1_btag_", "jet1_btag_", "jet2_btag_", "jet3_btag_", "jet4_btag_", "top_tag_score_"]
  elif args.sideband_name == "phoID":
    to_remove += ["leadIDMVA_", "subleadIDMVA_", "maxIDMVA_", "minIDMVA_"]

if args.old_vars:
  to_remove += ["top_tag_score_", "dipho_delta_R", "helicity_angle_"]

if args.no_psv:
  to_remove += ["leadPSV_", "subleadPSV_"]

if args.channel == "Hadronic":
  branches = numpy.concatenate((feature_names, ["evt_weight_", "label_", "multi_label_", "process_id_", "mass_", "lead_sigmaEtoE_", "sublead_sigmaEtoE_", "tth_ttX_mva_", "tth_qcdX_mva_", "tth_ttPP_mva_", "objects_", "lead_phi_", "sublead_phi_", "log_met_", "met_phi_"]))
elif args.channel == "Leptonic":
  branches = numpy.concatenate((feature_names, ["evt_weight_", "label_", "multi_label_", "process_id_", "mass_", "lead_sigmaEtoE_", "sublead_sigmaEtoE_", "objects_", "lead_phi_", "sublead_phi_", "log_met_", "met_phi_"]))

# grab features
train_frac = args.train_frac
rand_branch = "super_rand_" if args.randomize else "rand_"

data_label = 2
selection_train      = '((label_ == 0%s%s) || (label_ == 1 && signal_mass_label_ != 0)) && %s %s %.6f %s' % (" && process_id_ == 2" if args.dipho_only else "", " && (process_id_ == 5)" if args.ttGG_only else "", rand_branch, ">" if args.invert else "<", train_frac, "&& data_sideband_label_ == 0" if args.sideband else "")

if args.one_mass_point:
  selection_train      = '((label_ == 0%s%s) || (label_ == 1 && signal_mass_label_ == 1)) && %s %s %.6f %s' % (" && process_id_ == 2" if args.dipho_only else "", " && (process_id_ == 5)" if args.ttGG_only else "", rand_branch, ">" if args.invert else "<", train_frac, "&& data_sideband_label_ == 0" if args.sideband else "")
selection_validation = '((label_ == 0%s%s) || (label_ == 1 && signal_mass_label_ == 1)) && %s %s %.6f %s' % (" && process_id_ == 2" if args.dipho_only else "", " && (process_id_ == 5)" if args.ttGG_only else "", rand_branch, "<" if args.invert else ">", train_frac, "&& data_sideband_label_ == 0" if args.sideband else "")

print "Selection for training events: %s" % selection_train
print "Selection for validation events: %s" % selection_validation

features = root_numpy.tree2array(tree, branches = branches, selection = selection_train) 
features_validation = root_numpy.tree2array(tree, branches = branches, selection = selection_validation) 

if args.sideband:
  handicap = ""
  if args.handicap:
    handicap = " && %s < 0.5" % (rand_branch)
  features_data_sideband = root_numpy.tree2array(tree, branches = branches, selection = '(data_sideband_label_ == 1 && label_ == 2 %s) || (label_ == 1 && %s < %.6f && data_sideband_label_ == 0)' % (handicap, rand_branch, train_frac)) 
  features_data_sideband_mc = root_numpy.tree2array(tree, branches = branches, selection = '(data_sideband_label_ == 1 && label_ == 0 %s) || (label_ == 1 && %s < %.6f && data_sideband_label_ == 0)' % (handicap, rand_branch, train_frac))

features_data = root_numpy.tree2array(tree, branches = branches, selection = 'label_ == %d %s' % (data_label, "&& data_sideband_label_ == 0" if args.sideband else ""))


# Calculate DNN scores
dnn_models = args.dnn_models.split(",")
do_dnn = len(args.dnn_models) > 0

dnn_predictions = []

if do_dnn:
  print "Calculating dnn scores"
  dnn_features = ["lead_eta_", "sublead_eta_", "lead_phi_", "sublead_phi_", "leadptoM_", "subleadptoM_", "maxIDMVA_", "minIDMVA_", "log_met_", "met_phi_", "leadPSV_", "subleadPSV_", "dipho_rapidity_", "dipho_pt_over_mass_", "dipho_delta_R", "max1_btag_", "max2_btag_", "njets_"]
  print len(dnn_features)
  if args.channel == "Leptonic":
    dnn_features += ["n_lep_tight_"]
  i = 0
  for model in dnn_models:
    dnn_features_data = dnn_helper.DNN_Features(name = 'data', global_features = [features_data[feat] for feat in dnn_features], objects = features_data["objects_"])
    dnn_features_train = dnn_helper.DNN_Features(name = 'train', global_features = [features[feat] for feat in dnn_features], objects = features["objects_"])
    dnn_features_validation = dnn_helper.DNN_Features(name = 'validation', global_features = [features_validation[feat] for feat in dnn_features], objects = features_validation["objects_"])
    dnn = dnn_helper.DNN_Helper(features_validation = dnn_features_validation, features_train = dnn_features_train, features_data = dnn_features_data, weights_file = model)
    #dnn.predict()
    #dnn_predictions.append([dnn.predictions["train"], dnn.predictions["validation"], dnn.predictions["data"]])
    dnn_predictions.append(dnn.predict())
    feature_names.append("dnn_score_%d" % i)
    i += 1 

print dnn_predictions

#if do_dnn:
#  i = 0
#  dnn_features = ["lead_eta_", "sublead_eta_", "lead_phi_", "sublead_phi_", "leadptoM_", "subleadptoM_", "maxIDMVA_", "minIDMVA_", "log_met_", "met_phi_", "leadPSV_", "subleadPSV_", "dipho_rapidity_", "dipho_pt_over_mass_", "dipho_delta_R", "max1_btag_", "max2_btag_", "njets_", "top_tag_score_"]
#  for model in dnn_models:
#    dnn_predictions.append([])
#    for features_set in [features, features_validation, features_data]:
#      dnn = dnn_helper.DNN_Helper(objects = features_set["objects_"], global_features = [features_set[feat] for feat in dnn_features], weights_file = model)
#      prediction = dnn.predict()
#      dnn_predictions[i].append(prediction)
#    feature_names.append("dnn_score_%d" % i)
#    i += 1


training_feature_names = [feature for feature in feature_names if feature not in to_remove]

print training_feature_names

# organize features
global_features = []
global_features_validation = []
global_features_data = []
global_features_data_sideband = []
global_features_data_sideband_mc = []
for feature in training_feature_names:
  if "dnn_score" in feature:
    continue
  global_features.append(features[feature])
  global_features_validation.append(features_validation[feature])
  global_features_data.append(features_data[feature])
  if args.sideband:
    global_features_data_sideband.append(features_data_sideband[feature])
    global_features_data_sideband_mc.append(features_data_sideband_mc[feature])

if do_dnn:
  for i in range(len(dnn_predictions)):
    global_features.append(dnn_predictions[i][0])
    global_features_validation.append(dnn_predictions[i][1])
    global_features_data.append(dnn_predictions[i][2])


global_features = numpy.asarray(global_features)
global_features_validation = numpy.asarray(global_features_validation)
global_features_data = numpy.asarray(global_features_data)

if args.sideband:
  global_features_data_sideband = numpy.asarray(global_features_data_sideband)
  global_features_data_sideband_mc = numpy.asarray(global_features_data_sideband_mc)

if args.channel == "Hadronic":
  mva_names = ["max1_btag_", "max2_btag_", "maxIDMVA_", "minIDMVA_", "tth_ttX_mva_", "tth_qcdX_mva_", "tth_ttPP_mva_"]
elif args.channel == "Leptonic":
  mva_names = ["max1_btag_", "max2_btag_", "maxIDMVA_", "minIDMVA_"]

label = features["label_"]
multi_label = features["multi_label_"]
weights = features["evt_weight_"]
mass = features["mass_"]
njets = features["njets_"]
lead_sigmaEtoE = features["lead_sigmaEtoE_"]
sublead_sigmaEtoE = features["sublead_sigmaEtoE_"]
mvas = {}
for name in mva_names:
  mvas[name] = numpy.asarray(features[name])

label_validation = features_validation["label_"]
multi_label_validation = features_validation["multi_label_"]
weights_validation = features_validation["evt_weight_"]
mass_validation = features_validation["mass_"]
njets_validation = features_validation["njets_"]
mvas_validation = {}
for name in mva_names:
  mvas_validation[name] = numpy.asarray(features_validation[name])

label_data = features_data["label_"]
multi_label_data = features_data["multi_label_"]
weights_data = features_data["evt_weight_"]
mass_data = features_data["mass_"]
njets_data = features_data["njets_"]
mvas_data = {}
for name in mva_names:
  mvas_data[name] = numpy.asarray(features_data[name])

if args.sideband:
  label_data_sideband = features_data_sideband["label_"]
  multi_label_data_sideband = features_data_sideband["multi_label_"]
  weights_data_sideband = features_data_sideband["evt_weight_"]
  mass_data_sideband = features_data_sideband["mass_"]
  mvas_data_sideband = {}
  for name in mva_names:
    mvas_data_sideband[name] = numpy.asarray(features_data_sideband[name])

  label_data_sideband_mc = features_data_sideband_mc["label_"]
  multi_label_data_sideband_mc = features_data_sideband_mc["multi_label_"]
  weights_data_sideband_mc = features_data_sideband_mc["evt_weight_"]
  mass_data_sideband_mc = features_data_sideband_mc["mass_"]
  mvas_data_sideband_mc = {}
  for name in mva_names:
    mvas_data_sideband_mc[name] = numpy.asarray(features_data_sideband_mc[name])

  label_data_sideband[label_data_sideband == 2] = 0 # reassign label so that it plays nice with xgboost

# reorganize features
global_features = numpy.transpose(global_features)
global_features_validation = numpy.transpose(global_features_validation)
global_features_data = numpy.transpose(global_features_data)

if args.sideband:
  global_features_data_sideband = numpy.transpose(global_features_data_sideband)
  global_features_data_sideband_mc = numpy.transpose(global_features_data_sideband_mc)

f_out = h5py.File(output_file, "w")

dset_feature_names = f_out.create_dataset("feature_names", data=feature_names)
dset_training_feature_names = f_out.create_dataset("training_feature_names", data=training_feature_names)

dset_global = f_out.create_dataset("global", data=global_features)
dset_label = f_out.create_dataset("label", data=label)
dset_multi_label = f_out.create_dataset("multi_label", data=multi_label)
dset_weights = f_out.create_dataset("weights", data=weights)
dset_mass = f_out.create_dataset("mass", data=mass)
dset_njets = f_out.create_dataset("njets", data=njets)
dset_lead_sigmaEtoE = f_out.create_dataset("lead_sigmaEtoE", data=lead_sigmaEtoE)
dset_sublead_sigmaEtoE = f_out.create_dataset("sublead_sigmaEtoE", data=sublead_sigmaEtoE)
for name in mva_names:
  dset_mva = f_out.create_dataset(name, data=mvas[name])

dset_global_validation = f_out.create_dataset("global_validation", data=global_features_validation)
dset_label_validation = f_out.create_dataset("label_validation", data=label_validation)
dset_multi_label_validation = f_out.create_dataset("multi_label_validation", data=multi_label_validation)
dset_weights_validation = f_out.create_dataset("weights_validation", data=weights_validation)
dset_mass_validation = f_out.create_dataset("mass_validation", data=mass_validation)
dset_njets_validation = f_out.create_dataset("njets_validation", data=njets_validation)
for name in mva_names:
  dset_mva = f_out.create_dataset(name+"_validation", data=mvas_validation[name])


dset_global_data = f_out.create_dataset("global_data", data=global_features_data)
dset_label_data = f_out.create_dataset("label_data", data=label_data)
dset_multi_label_data = f_out.create_dataset("multi_label_data", data=multi_label_data)
dset_weights_data = f_out.create_dataset("weights_data", data=weights_data)
dset_mass_data = f_out.create_dataset("mass_data", data=mass_data)
dset_njets_data = f_out.create_dataset("njets_data", data=njets_data)
for name in mva_names:
  dset_mva = f_out.create_dataset(name+"_data", data=mvas_data[name])


if args.sideband:
  dset_global_data_sideband = f_out.create_dataset("global_data_sideband", data=global_features_data_sideband)
  dset_label_data_sideband = f_out.create_dataset("label_data_sideband", data=label_data_sideband)
  dset_multi_label_data_sideband = f_out.create_dataset("multi_label_data_sideband", data=multi_label_data_sideband)
  dset_weights_data_sideband = f_out.create_dataset("weights_data_sideband", data=weights_data_sideband)
  dset_mass_data_sideband = f_out.create_dataset("mass_data_sideband", data=mass_data_sideband)
  for name in mva_names:
    dset_mva = f_out.create_dataset(name+"_data_sideband", data=mvas_data_sideband[name])

  dset_global_data_sideband_mc = f_out.create_dataset("global_data_sideband_mc", data=global_features_data_sideband_mc)
  dset_label_data_sideband_mc = f_out.create_dataset("label_data_sideband_mc", data=label_data_sideband_mc)
  dset_multi_label_data_sideband_mc = f_out.create_dataset("multi_label_data_sideband_mc", data=multi_label_data_sideband_mc)
  dset_weights_data_sideband_mc = f_out.create_dataset("weights_data_sideband_mc", data=weights_data_sideband_mc)
  dset_mass_data_sideband_mc = f_out.create_dataset("mass_data_sideband_mc", data=mass_data_sideband_mc)
  for name in mva_names:
    dset_mva = f_out.create_dataset(name+"_data_sideband_mc", data=mvas_data_sideband_mc[name])

f_out.close()
