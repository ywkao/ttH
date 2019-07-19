import os, sys
import h5py
import ROOT
import numpy
import root_numpy
import math
import json

import utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input root file", type=str)
parser.add_argument("--channel", help = "Hadronic or Leptonic", type=str, default="Hadronic")
parser.add_argument("--signal", help = "which process to consider as signal", type=str, default="ttH")
parser.add_argument("--backgrounds", help = "which processes to consider as bkgs", type=str, default="all")
parser.add_argument("--boosted_objects", help = "use objects that are boosted to higgs p4", action="store_true")
parser.add_argument("--do_top_tag", help = "use top tagger BDT in training", action="store_true")
parser.add_argument("--invert", help = "invert test/train split", action="store_true")
parser.add_argument("--train_frac", help = "what fraction of data to use for training", type = float, default = 0.5)
parser.add_argument("--tag", help = "name to add to hdf5 file name", type=str, default="")
parser.add_argument("--z_score", help = "preprocess features to express them as z-score (subtract mean, divide by std.)", action="store_true")
parser.add_argument("--fcnc", help = "use fcnc as signal, sm as bkg", action="store_true")
parser.add_argument("--ggH_treatment", help = "how to deal with large ggH weights in fcnc vs SM higgs training", type=str, default="none")
parser.add_argument("--add_mass_constraints", help = "add mjjj and mggj as global features", action = "store_true")
parser.add_argument("--add_ext_mass_constraints", help = "add full extended set of mass constraints", action = "store_true")
args = parser.parse_args()

baby_file = args.input.replace(".root", "") + ".root"
output_file = args.input.replace(".root", "").replace("../Loopers/MVABaby_","") + "_dnn_features_" + args.tag + ".hdf5"
print(output_file)

f = ROOT.TFile(baby_file)
tree = f.Get("t")


# load tree to array
#feature_names = ["objects_", "objects_boosted_", "lead_eta_", "sublead_eta_", "lead_phi_", "sublead_phi_", "leadptoM_", "subleadptoM_", "maxIDMVA_", "minIDMVA_", "met_", "met_phi_", "leadPSV_", "subleadPSV_", "dipho_rapidity_", "dipho_pt_over_mass_", "dipho_delta_R"]
feature_names = ["objects_", "lead_eta_", "sublead_eta_", "lead_phi_", "sublead_phi_", "leadptoM_", "subleadptoM_", "maxIDMVA_", "minIDMVA_", "log_met_", "met_phi_", "leadPSV_", "subleadPSV_", "dipho_rapidity_", "dipho_pt_over_mass_", "dipho_delta_R", "max1_btag_", "max2_btag_", "njets_"]
if args.channel == "Hadronic":
  feature_names += ["objects_boosted_"]
elif args.channel == "Leptonic":
  feature_names += ["n_lep_tight_", "leptons_", "jets_"]

if args.do_top_tag:
  top_tag_features = ["top_tag_score_"]
  #top_tag_features = ["top_tag_score_", "top_tag_mass_", "top_tag_pt_", "top_tag_eta_", "top_tag_phi_"]
else:
  top_tag_features = []

feature_names += top_tag_features 

if args.add_mass_constraints:
    feature_names += ["m_ggj_", "m_jjj_"]

if args.add_ext_mass_constraints:
    top_cand_names = ["top_candidates_" + str(i) + "_" for i in range(1,13)]
    feature_names += top_cand_names

branches = numpy.concatenate((feature_names, ["evt_weight_", "label_", "multi_label_", "process_id_", "mass_", "lead_sigmaEtoE_", "sublead_sigmaEtoE_", "top_tag_score_", "tth_ttPP_mva_", "tth_std_mva_", "tth_dipho_mva_", "evt_", "run_", "lumi_"]))
branches = numpy.unique(branches)


rand_branch = "rand_"
data_label = 2
train_frac = args.train_frac

if args.backgrounds == "all":
  selection = ""
else:
  process_dict = {"ttH" : 0, "ttGG" : 5, "dipho" : 2, "tH" : 11, "ttG" : 6, "ttJets" : 9, "GJets_QCD" : 18 if args.channel == "Hadronic" else 3, "VGamma" : 7, "FCNC_hut" : 22, "FCNC_hct" : 23, "ggH" : 14}
  selection = "&& ("
  procs = args.backgrounds.split(",") + args.signal.split(",")
  for i in range(len(procs)):
    selection += "((process_id_ == %d" % (process_dict[procs[i]])
    
    if procs[i] == "ttGG":
      selection += " || process_id_ == 6 || process_id_ == 9) && abs(evt_weight_) < 0.01)"
    elif procs[i] == "tH":
      selection += " || process_id_ == 12))"
    elif procs[i] == "FCNC_hut":
      selection += " || process_id_ == 24))"
    elif procs[i] == "FCNC_hct":
      selection += " || process_id_ == 25))"
    else:
      selection += "))"
    if i != len(procs) - 1:
      selection += " || "

  selection += ")"

if args.ggH_treatment == "dont_train":
    selection_train = selection.replace(" || ((process_id_ == 14))", "") # don't train with ggH because it has high weights
else:
    selection_train = selection
print(selection_train)

if args.fcnc:
    features = root_numpy.tree2array(tree, branches = branches, selection = '((label_ == 0) || (label_ == 1)) && %s %s %.6f %s' % (rand_branch, ">" if args.invert else "<", train_frac, selection_train))
    features_validation = root_numpy.tree2array(tree, branches = branches, selection = '((label_ == 0 && !(process_id_ == 0 && signal_mass_label_ != 0)) || (label_ == 1)) && %s %s %.6f %s' % (rand_branch, ">" if args.invert else "<", train_frac, selection))
    features_final_fit = root_numpy.tree2array(tree, branches = branches, selection = '((label_ == 0 && signal_mass_label_ == 0 && rand_ < 0.01))') # dummy selection

    for i in range(len(features["process_id_"])):
        if features["process_id_"][i] == 0: # scale ttH by 1/7 bc we use 7 mass points in training
            features["evt_weight_"][i] *= 1.0/7.0 
        if args.ggH_treatment == "scale":
            if features["process_id_"][i] == 14: # scale ggH by 50 to see if it helps
                features["evt_weight_"][i] *= 1.0/50.0
        #if args.ggH_treatment == "oversample":
        #    oversample_factor = 50.
        #    oversample_array = numpy.empty()
        #    if features["process_id_"][i] == 14:
        #        features["evt_weight_"][i] *= 
        #        for j in range(oversample_factor):

else:
    features = root_numpy.tree2array(tree, branches = branches, selection = '((label_ == 0) || (label_ == 1 && signal_mass_label_ != 0)) && %s %s %.6f %s' % (rand_branch, ">" if args.invert else "<", train_frac, selection))
    features_validation = root_numpy.tree2array(tree, branches = branches, selection = '((label_ == 0) || (label_ == 1 && signal_mass_label_ == 1)) && %s %s %.6f %s' % (rand_branch, "<" if args.invert else ">", train_frac, selection))
    features_final_fit = root_numpy.tree2array(tree, branches = branches, selection = '((label_ == 1 && signal_mass_label_ == 0))')

features_data = root_numpy.tree2array(tree, branches = branches, selection = 'label_ == %d' % (data_label))

if args.boosted_objects:
  object_features = features["objects_boosted_"]
  object_features_validation = features_validation["objects_boosted_"]
  object_features_data = features_data["objects_boosted_"]
  object_features_final_fit = features_final_fit["objects_boosted_"]

else:
  object_features = features["objects_"]
  object_features_validation = features_validation["objects_"]
  object_features_data = features_data["objects_"]
  object_features_final_fit = features_final_fit["objects_"]

if args.channel == "Leptonic": # also add sequences of leptons and jets separately
  jet_features = features["jets_"]
  jet_features_validation = features_validation["jets_"]
  jet_features_data = features_data["jets_"]
  jet_features_final_fit = features_final_fit["jets_"]

  lepton_features = features["leptons_"]
  lepton_features_validation = features_validation["leptons_"]
  lepton_features_data = features_data["leptons_"]
  lepton_features_final_fit = features_final_fit["leptons_"]


def preprocess_sigmoid_v2(array, mean, std):
  alpha = (array - mean)*(1./std)
  return (1 - numpy.exp(-alpha))/(1 + numpy.exp(-alpha))

def preprocess_sigmoid(array):
  array_trimmed = array[array != pad_value]
  mean = numpy.mean(array_trimmed)
  std = numpy.std(array_trimmed)
  if std == 0:
    std = 1
  alpha = (array - mean)*(1./std)
  return (1 - numpy.exp(-alpha))/(1 + numpy.exp(-alpha))


for name in feature_names:
    if "leptons_" == name or "jets_" == name or "objects_" in name:
        feature_names.remove(name)

print("Here are the ordered global features:", feature_names)

if args.z_score:
    preprocess_dict = {}
    for feature in feature_names:
        if ("objects_" not in feature and "leptons_" != feature and "jets_" != feature): 
            mean, stddev = utils.get_mean_and_std(features[feature])
            preprocess_dict[feature] = { "mean" : float(mean), "std_dev" : float(stddev)}
 
global_features = utils.create_array(features, feature_names, preprocess_dict, args.z_score)
global_features_validation = utils.create_array(features_validation, feature_names, preprocess_dict, args.z_score)
global_features_data = utils.create_array(features_data, feature_names, preprocess_dict, args.z_score)
global_features_final_fit = utils.create_array(features_final_fit, feature_names, preprocess_dict, args.z_score)

object_features = utils.pad_array(object_features)
object_features_validation = utils.pad_array(object_features_validation)
object_features_data = utils.pad_array(object_features_data)
object_features_final_fit = utils.pad_array(object_features_final_fit)

if args.z_score:
    n_object_features = len(object_features[0][0])
    for i in range(n_object_features):
        mean, stddev = utils.get_mean_and_std(object_features[:,:,i])
        preprocess_dict["objects_" + str(i)] = { "mean" : mean, "std_dev" : stddev }

    with open("preprocess_scheme_%s_%s.json" % (args.channel, args.tag), "w") as f_out:
        json.dump(preprocess_dict, f_out, indent=4, sort_keys=True)

    for object_set in [object_features, object_features_validation, object_features_data, object_features_final_fit]:
        object_set = utils.preprocess_array(object_set, preprocess_dict)
        #for i in range(n_object_features):
        #    object_set[:,:,i] = preprocess(object_set[:,:,i], preprocess_dict["objects_" + str(i)]["mean"], preprocess_dict["objects_" + str(i)]["std_dev"])

label = features["label_"]
multi_label = features["multi_label_"]
process_id = features["process_id_"]
weights = features["evt_weight_"]
mass = features["mass_"]
lead_sigmaEtoE = features["lead_sigmaEtoE_"]
sublead_sigmaEtoE = features["sublead_sigmaEtoE_"]
top_tag_score = features["top_tag_score_"]
tth_ttPP_mva = features["tth_ttPP_mva_"]
tth_dipho_mva = features["tth_dipho_mva_"]
tth_std_mva = features["tth_std_mva_"]
evt = features["evt_"]
run = features["run_"]
lumi = features["lumi_"]

label_validation = features_validation["label_"]
multi_label_validation = features_validation["multi_label_"]
process_id_validation = features_validation["process_id_"]
weights_validation = features_validation["evt_weight_"]
mass_validation = features_validation["mass_"]
top_tag_score_validation = features_validation["top_tag_score_"]
tth_ttPP_mva_validation = features_validation["tth_ttPP_mva_"]
tth_dipho_mva_validation = features_validation["tth_dipho_mva_"]
tth_std_mva_validation = features_validation["tth_std_mva_"]
evt_validation = features_validation["evt_"]
run_validation = features_validation["run_"]
lumi_validation = features_validation["lumi_"]

label_data = features_data["label_"]
multi_label_data = features_data["multi_label_"]
process_id_data = features_data["process_id_"]
weights_data = features_data["evt_weight_"]
mass_data = features_data["mass_"]
top_tag_score_data = features_data["top_tag_score_"]
tth_ttPP_mva_data = features_data["tth_ttPP_mva_"]
tth_dipho_mva_data = features_data["tth_dipho_mva_"]
tth_std_mva_data = features_data["tth_std_mva_"]
evt_data = features_data["evt_"]
run_data = features_data["run_"]
lumi_data = features_data["lumi_"]

label_final_fit = features_final_fit["label_"]
multi_label_final_fit = features_final_fit["multi_label_"]
process_id_final_fit = features_final_fit["process_id_"]
weights_final_fit = features_final_fit["evt_weight_"]
mass_final_fit = features_final_fit["mass_"]
top_tag_score_final_fit = features_final_fit["top_tag_score_"]
tth_ttPP_mva_final_fit = features_final_fit["tth_ttPP_mva_"]
tth_dipho_mva_final_fit = features_final_fit["tth_dipho_mva_"]
tth_std_mva_final_fit = features_final_fit["tth_std_mva_"]
evt_final_fit = features_final_fit["evt_"]
run_final_fit = features_final_fit["run_"]
lumi_final_fit = features_final_fit["lumi_"]

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
dset_process_id = f_out.create_dataset("process_id", data=process_id)
dset_weights = f_out.create_dataset("weights", data=weights)
dset_mass = f_out.create_dataset("mass", data=mass)
dset_tth_ttPP_mva = f_out.create_dataset("tth_ttPP_mva", data=tth_ttPP_mva)
dset_tth_dipho_mva = f_out.create_dataset("tth_dipho_mva", data=tth_dipho_mva)
dset_tth_std_mva = f_out.create_dataset("tth_std_mva", data=tth_std_mva)
dset_evt = f_out.create_dataset("evt", data=evt)
dset_run = f_out.create_dataset("run", data=run)
dset_lumi = f_out.create_dataset("lumi", data=lumi)
dset_top_tag_score = f_out.create_dataset("top_tag_score", data=top_tag_score)
dset_lead_sigmaEtoE = f_out.create_dataset("lead_sigmaEtoE", data=lead_sigmaEtoE)
dset_sublead_sigmaEtoE = f_out.create_dataset("sublead_sigmaEtoE", data=sublead_sigmaEtoE)

dset_object_validation = f_out.create_dataset("object_validation", data=object_features_validation)
dset_global_validation = f_out.create_dataset("global_validation", data=global_features_validation)
dset_label_validation = f_out.create_dataset("label_validation", data=label_validation)
dset_multi_label_validation = f_out.create_dataset("multi_label_validation", data=multi_label_validation)
dset_process_id_validation = f_out.create_dataset("process_id_validation", data=process_id_validation)
dset_weights_validation = f_out.create_dataset("weights_validation", data=weights_validation)
dset_top_tag_score_validation = f_out.create_dataset("top_tag_score_validation", data=top_tag_score_validation)
dset_mass_validation = f_out.create_dataset("mass_validation", data=mass_validation)
dset_tth_ttPP_mva_validation = f_out.create_dataset("tth_ttPP_mva_validation", data=tth_ttPP_mva_validation)
dset_tth_dipho_mva_validation = f_out.create_dataset("tth_dipho_mva_validation", data=tth_dipho_mva_validation)
dset_tth_std_mva_validation = f_out.create_dataset("tth_std_mva_validation", data=tth_std_mva_validation)
dset_evt_validation = f_out.create_dataset("evt_validation", data=evt_validation)
dset_run_validation = f_out.create_dataset("run_validation", data=run_validation)
dset_lumi_validation = f_out.create_dataset("lumi_validation", data=lumi_validation)

dset_object_data = f_out.create_dataset("object_data", data=object_features_data)
dset_global_data = f_out.create_dataset("global_data", data=global_features_data)
dset_label_data = f_out.create_dataset("label_data", data=label_data)
dset_multi_label_data = f_out.create_dataset("multi_label_data", data=multi_label_data)
dset_process_id_data = f_out.create_dataset("process_id_data", data=process_id_data)
dset_weights_data = f_out.create_dataset("weights_data", data=weights_data)
dset_top_tag_score_data = f_out.create_dataset("top_tag_score_data", data=top_tag_score_data)
dset_mass_data = f_out.create_dataset("mass_data", data=mass_data)
dset_tth_ttPP_mva_data = f_out.create_dataset("tth_ttPP_mva_data", data=tth_ttPP_mva_data)
dset_tth_dipho_mva_data = f_out.create_dataset("tth_dipho_mva_data", data=tth_dipho_mva_data)
dset_tth_std_mva_data = f_out.create_dataset("tth_std_mva_data", data=tth_std_mva_data)
dset_evt_data = f_out.create_dataset("evt_data", data=evt_data)
dset_run_data = f_out.create_dataset("run_data", data=run_data)
dset_lumi_data = f_out.create_dataset("lumi_data", data=lumi_data)

dset_object_final_fit = f_out.create_dataset("object_final_fit", data=object_features_final_fit)
dset_global_final_fit = f_out.create_dataset("global_final_fit", data=global_features_final_fit)
dset_label_final_fit = f_out.create_dataset("label_final_fit", data=label_final_fit)
dset_multi_label_final_fit = f_out.create_dataset("multi_label_final_fit", data=multi_label_final_fit)
dset_process_id_final_fit = f_out.create_dataset("process_id_final_fit", data=process_id_final_fit)
dset_weights_final_fit = f_out.create_dataset("weights_final_fit", data=weights_final_fit)
dset_top_tag_score_final_fit = f_out.create_dataset("top_tag_score_final_fit", data=top_tag_score_final_fit)
dset_mass_final_fit = f_out.create_dataset("mass_final_fit", data=mass_final_fit)
dset_tth_ttPP_mva_final_fit = f_out.create_dataset("tth_ttPP_mva_final_fit", data=tth_ttPP_mva_final_fit)
dset_tth_dipho_mva_final_fit = f_out.create_dataset("tth_dipho_mva_final_fit", data=tth_dipho_mva_final_fit)
dset_tth_std_mva_final_fit = f_out.create_dataset("tth_std_mva_final_fit", data=tth_std_mva_final_fit)
dset_evt_final_fit = f_out.create_dataset("evt_final_fit", data=evt_final_fit)
dset_run_final_fit = f_out.create_dataset("run_final_fit", data=run_final_fit)
dset_lumi_final_fit = f_out.create_dataset("lumi_final_fit", data=lumi_final_fit)

f_out.close()
