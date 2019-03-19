import os
import tensorflow as tf
print(tf.__version__)
config = tf.ConfigProto(log_device_placement=True)
config.gpu_options.allow_growth = True
session = tf.Session(config=config)

print(session)

import keras
import numpy
import sys
from sklearn import metrics
import h5py
import glob

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import dnn_helper
import significance_utils
import utils
import ks_test

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--tag", help = "save name for weights file", type=str)
parser.add_argument("--no_global", help = "don't use global features", action="store_true")
parser.add_argument("--no_lstm", help = "don't use object features (lstm)", action="store_true")
parser.add_argument("--load", help = "give weights file to use as starting point", type=str)
args = parser.parse_args()

f = h5py.File(args.input, "r")

object_features, object_features_validation, object_features_data = f['object'], f['object_validation'], f['object_data']
global_features, global_features_validation, global_features_data = f['global'], f['global_validation'], f['global_data']
label, label_validation, label_data = f['label'], f['label_validation'], f['label_data']
weights, weights_validation, weights_data = f['weights'], f['weights_validation'], f['weights_data']
mass, mass_validation, mass_data = f['mass'], f['mass_validation'], f['mass_data']
top_tag_score, top_tag_score_validation, top_tag_score_data = f['top_tag_score'], f['top_tag_score_validation'], f['top_tag_score_data']
tth_ttPP_mva, tth_ttPP_mva_validation, tth_ttPP_mva_data = f['tth_ttPP_mva'], f['tth_ttPP_mva_validation'], f['tth_ttPP_mva_data']
tth_dipho_mva, tth_dipho_mva_validation, tth_dipho_mva_data = f['tth_dipho_mva'], f['tth_dipho_mva_validation'], f['tth_dipho_mva_data']
tth_std_mva, tth_std_mva_validation, tth_std_mva_data = f['tth_std_mva'], f['tth_std_mva_validation'], f['tth_std_mva_data']

# Features
dnn_features_train = dnn_helper.DNN_Features(name = 'train', global_features = global_features, objects = object_features, label = label, weights = weights, references = { "Top Tag Score" : top_tag_score, "ttPP BDT" : tth_ttPP_mva, "Dipho BDT" : tth_dipho_mva, "Baseline BDT" : tth_std_mva})
dnn_features_validation = dnn_helper.DNN_Features(name = 'validation', global_features = global_features_validation, objects = object_features_validation, label = label_validation, weights = weights_validation, references = { "Top Tag Score" : top_tag_score_validation, "ttPP BDT" : tth_ttPP_mva_validation, "Dipho BDT" : tth_dipho_mva_validation, "Baseline BDT" : tth_std_mva_validation})
dnn_features_data = dnn_helper.DNN_Features(name = 'data', global_features = global_features_data, objects = object_features_data, label = label_data, weights = weights_data, references = { "Top Tag Score" : top_tag_score_data, "ttPP BDT" : tth_ttPP_mva_data, "Dipho BDT" : tth_dipho_mva_data, "Baseline BDT" : tth_std_mva_data})

# DNN Helper
dnn = dnn_helper.DNN_Helper(features_train = dnn_features_train, features_validation = dnn_features_validation, features_data = dnn_features_data, tag = args.tag)

# Train
dnn.train_with_early_stopping()

# Diagnostics
dnn.do_diagnostics()

