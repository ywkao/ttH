import os, sys
import tensorflow as tf
sys.path.append("../")
import utils
print("GPU with lowest memory %s" % str(utils.pick_gpu_lowest_memory()))
os.environ["CUDA_VISIBLE_DEVICES"] = str(utils.pick_gpu_lowest_memory())
print((tf.__version__))
config = tf.ConfigProto(log_device_placement=True)
config.gpu_options.allow_growth = True
session = tf.Session(config=config)
print(session)

import h5py
import numpy

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--tag", help = "save name for weights file", type=str)
args = parser.parse_args()

f = h5py.File(args.input, "r")

object_features, object_features_validation, object_features_data = f['object'], f['object_validation'], f['object_data']
global_features, global_features_validation, global_features_data = f['global'], f['global_validation'], f['global_data']
label, label_validation, label_data = f['label'], f['label_validation'], f['label_data']
process_id, process_id_validation, process_id_data = f['process_id'], f['process_id_validation'], f['process_id_data']
weights, weights_validation, weights_data = f['weights'], f['weights_validation'], f['weights_data']
mass, mass_validation, mass_data = f['mass'], f['mass_validation'], f['mass_data']

import siamese_helper


config = {"n_nodes_dense_1" : 300, "n_nodes_dense_2" : 200, "n_dense_1" : 2, "n_dense_2" : 3, "n_nodes_lstm" : 100, "n_lstm" : 2, "maxnorm" : 10, "dropout_rate" : 0.2, "learning_rate" : 0.0005, "start_batch" : 10000, "batch_norm" : True, "batch_momentum" : 0.99, "layer_norm" : False, "n_manifold" : 2}

metadata = { "config" : config, "input" : args.input }

features_train = { "objects" : numpy.asarray(object_features), "global" : numpy.asarray(global_features), "label" : numpy.asarray(label), "process_id" : numpy.asarray(process_id), "weights" : numpy.asarray(weights) }
features_validation= { "objects" : numpy.asarray(object_features_validation), "global" : numpy.asarray(global_features_validation), "label" : numpy.asarray(label_validation), "process_id" : numpy.asarray(process_id_validation), "weights" : numpy.asarray(weights_validation) }
siamese_trainer = siamese_helper.Siamese_Helper(features_train = features_train, features_validation = features_validation, tag = args.tag, channel = "Hadronic" if "Hadronic" in args.input else "Leptonic", metadata = metadata)

for i in range(1000):
    siamese_trainer.pick_pairs()
    siamese_trainer.set_pairs()
    siamese_trainer.train()
    siamese_trainer.predict()
#siamese_trainer.predict()
#siamese_trainer.subsample_contributing_pairs()
#siamese_trainer.train()
#siamese_trainer.predict()
#siamese_trainer.subsample_contributing_pairs()
#siamese_trainer.train()
siamese_trainer.evaluate()
siamese_trainer.do_diagnostics()
