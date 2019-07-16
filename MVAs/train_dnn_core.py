import dnn_helper
import os
os.environ["CUDA_VISIBLE_DEVICES"]="0"
import tensorflow as tf
#print((tf.__version__))
#if tf.__version__ == "1.14.0":
#    print("Disabling new tf behaviors, because Sam wrote this DNN in tf 1.3.0")
#    import tensorflow.compat.v1 as tf
#    tf.disable_v2_behavior()
config = tf.ConfigProto(log_device_placement=True)
config.gpu_options.allow_growth = True
session = tf.Session(config=config)

print(session)

import numpy
import sys
from sklearn import metrics
import h5py
import glob

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import utils
import ks_test

def train(args, config):
  f = h5py.File(args.input, "r")
  if len(list(config.keys())) == 0:
    config = {"n_nodes_dense_1" : 300, "n_nodes_dense_2" : 200, "n_dense_1" : 1, "n_dense_2" : 4, "n_nodes_lstm" : 100, "n_lstm" : 3, "maxnorm" : 3, "dropout_rate" : 0.25, "learning_rate" : 0.001, "start_batch" : 512, "batch_norm" : True, "batch_momentum" : 0.99, "layer_norm" : False} 


  object_features, object_features_validation, object_features_data = f['object'], f['object_validation'], f['object_data']
  #if args.channel == "Leptonic":
  #  jet_features, jet_features_validation, jet_features_data = f['jet'], f['jet_validation'], f['jet_data']
  #  lepton_features, lepton_features_validation, lepton_features_data = f['lepton'], f['lepton_validation'], f['lepton_data']

  global_features, global_features_validation, global_features_data = f['global'], f['global_validation'], f['global_data']
  label, label_validation, label_data = f['label'], f['label_validation'], f['label_data']
  weights, weights_validation, weights_data = f['weights'], f['weights_validation'], f['weights_data']
  if args.absolute_weights:
    weights = numpy.absolute(numpy.array(weights))
  mass, mass_validation, mass_data = f['mass'], f['mass_validation'], f['mass_data']
  top_tag_score, top_tag_score_validation, top_tag_score_data = f['top_tag_score'], f['top_tag_score_validation'], f['top_tag_score_data']
  tth_ttPP_mva, tth_ttPP_mva_validation, tth_ttPP_mva_data = f['tth_ttPP_mva'], f['tth_ttPP_mva_validation'], f['tth_ttPP_mva_data']
  tth_dipho_mva, tth_dipho_mva_validation, tth_dipho_mva_data = f['tth_dipho_mva'], f['tth_dipho_mva_validation'], f['tth_dipho_mva_data']
  tth_std_mva, tth_std_mva_validation, tth_std_mva_data = f['tth_std_mva'], f['tth_std_mva_validation'], f['tth_std_mva_data']
  evt_data = f['evt_data']
  run_data = f['run_data']
  lumi_data = f['lumi_data']


  # Features
  leptonic_single_lstm = True


  if args.channel == "Hadronic" or leptonic_single_lstm:
    dnn_features_train = dnn_helper.DNN_Features(name = 'train', global_features = global_features, objects = object_features, label = label, weights = weights, references = { "Top Tag Score" : top_tag_score, "ttPP BDT" : tth_ttPP_mva, "Dipho BDT" : tth_dipho_mva, "Baseline BDT" : tth_std_mva}, no_prep = True)
    dnn_features_validation = dnn_helper.DNN_Features(name = 'validation', global_features = global_features_validation, objects = object_features_validation, label = label_validation, weights = weights_validation, references = { "Top Tag Score" : top_tag_score_validation, "ttPP BDT" : tth_ttPP_mva_validation, "Dipho BDT" : tth_dipho_mva_validation, "Baseline BDT" : tth_std_mva_validation}, no_prep = True)
    dnn_features_data = dnn_helper.DNN_Features(name = 'data', global_features = global_features_data, objects = object_features_data, label = label_data, weights = weights_data, references = { "Top Tag Score" : top_tag_score_data, "ttPP BDT" : tth_ttPP_mva_data, "Dipho BDT" : tth_dipho_mva_data, "Baseline BDT" : tth_std_mva_data}, no_prep = True)

  else:
    dnn_features_train = dnn_helper.DNN_Features(name = 'train', global_features = global_features, objects = jet_features, leptons = lepton_features, label = label, weights = weights, references = { "Top Tag Score" : top_tag_score, "ttPP BDT" : tth_ttPP_mva, "Dipho BDT" : tth_dipho_mva, "Baseline BDT" : tth_std_mva}, no_prep = True)
    dnn_features_validation = dnn_helper.DNN_Features(name = 'validation', global_features = global_features_validation, objects = jet_features_validation, leptons = lepton_features_validation, label = label_validation, weights = weights_validation, references = { "Top Tag Score" : top_tag_score_validation, "ttPP BDT" : tth_ttPP_mva_validation, "Dipho BDT" : tth_dipho_mva_validation, "Baseline BDT" : tth_std_mva_validation}, no_prep = True)
    dnn_features_data = dnn_helper.DNN_Features(name = 'data', global_features = global_features_data, objects = jet_features_data, leptons = lepton_features_data, label = label_data, weights = weights_data, references = { "Top Tag Score" : top_tag_score_data, "ttPP BDT" : tth_ttPP_mva_data, "Dipho BDT" : tth_dipho_mva_data, "Baseline BDT" : tth_std_mva_data}, no_prep = True)

  # DNN Helper
  metadata = { "input" : args.input, "config" : config, "preprocess_scheme" : args.preprocess_scheme if args.preprocess_scheme else "none"}
  dnn = dnn_helper.DNN_Helper(features_train = dnn_features_train, features_validation = dnn_features_validation, features_data = dnn_features_data, tag = args.channel + "_" + args.tag, evt_data = evt_data, run_data = run_data, lumi_data = lumi_data, mass_data = mass_data, metadata = metadata)
  if args.no_bootstrap:
    dnn.n_bootstrap = 0

  if args.load:
    dnn.weights_file = args.load
    dnn.debug()

  else:
    # Train
    dnn.train_with_early_stopping()

    # Diagnostics
    dnn.do_diagnostics()
  return dnn
