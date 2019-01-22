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

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import dnn_model
import significance_utils
import utils
import ks_test

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--tag", help = "save name for weights file", type=str)
parser.add_argument("--global_features", help = "use global features also", action="store_true")
args = parser.parse_args()

f = h5py.File(args.input, "r")

object_features, object_features_validation, object_features_data = f['object'], f['object_validation'], f['object_data']
global_features, global_features_validation, global_features_data = f['global'], f['global_validation'], f['global_data']
label, label_validation, label_data = f['label'], f['label_validation'], f['label_data']
weights, weights_validation, weights_data = f['weights'], f['weights_validation'], f['weights_data']
mass, mass_validation, mass_data = f['mass'], f['mass_validation'], f['mass_data']
top_tag_score, top_tag_score_validation, top_tag_score_data = f['top_tag_score'], f['top_tag_score_validation'], f['top_tag_score_data']

max_objects = len(object_features[0])
n_features = len(object_features[0][0])
n_global_features = len(global_features[0])

print(max_objects, n_features, len(object_features), n_global_features)

model = dnn_model.separate_photons(max_objects, n_features, n_global_features, args.global_features)

nEpochs = 3
nBatch = 8192

weights_file = "dnn_weights/" + args.tag + "_weights_{epoch:02d}.hdf5"
checkpoint = keras.callbacks.ModelCheckpoint(weights_file) # save after every epoch 
callbacks_list = [checkpoint]

weights_train = numpy.asarray(weights)
sum_neg_weights = utils.sum_of_weights_v2(weights_train, label, 0)
sum_pos_weights = utils.sum_of_weights_v2(weights_train, label, 1)
print("Sum of weights before scaling: ", sum_pos_weights, sum_neg_weights)

weights_train[numpy.where(numpy.asarray(label) == 1)] *= sum_neg_weights / sum_pos_weights

sum_neg_weights = utils.sum_of_weights_v2(weights_train, label, 0)
sum_pos_weights = utils.sum_of_weights_v2(weights_train, label, 1)
print("Sum of weights after scaling: ", sum_pos_weights, sum_neg_weights)

object_features = numpy.asarray(object_features)
label = numpy.asarray(label)
global_features = numpy.asarray(global_features)

model.fit([global_features, object_features], label, epochs = nEpochs, batch_size = nBatch, sample_weight = numpy.asarray(weights))

pred = model.predict([global_features, object_features], batch_size = nBatch)
pred_test = model.predict([global_features_validation, object_features_validation], batch_size = nBatch)
pred_data = model.predict([global_features_data, object_features_data], batch_size = nBatch)

fpr_train, tpr_train, thresh_train = metrics.roc_curve(label, pred, pos_label = 1, sample_weight = weights_train)
fpr_test,  tpr_test,  threst_test  = metrics.roc_curve(label_validation, pred_test, pos_label = 1, sample_weight = weights_validation)

fpr_tts, tpr_tts, thresh_tts = metrics.roc_curve(label, top_tag_score, pos_label = 1, sample_weight = weights_train)

print("Training AUC: %.5f" % metrics.auc(fpr_train, tpr_train, reorder = True))
print("Testing  AUC: %.5f" % metrics.auc(fpr_test , tpr_test, reorder = True))
print("Top Tag  AUC: %.5f" % metrics.auc(fpr_tts,   tpr_tts, reorder = True))

numpy.savez("dnn_rocs_%s.npz" % (args.tag), fpr_train = fpr_train, tpr_train = tpr_train, fpr_test = fpr_test, tpr_test = tpr_test, fpr_tts = fpr_tts, tpr_tts = tpr_tts)

y_test = label_validation


import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

fig = plt.figure()
ax = fig.add_subplot(111)
ax.yaxis.set_ticks_position('both')
ax.grid(True)
plt.plot(fpr_train, tpr_train, color='darkred', lw=2, label='Train')
#plt.plot(fpr_bdt, tpr_bdt, color='blue', lw=2, label='BDT')
plt.plot(fpr_test, tpr_test, color = 'darkorange', lw=2, label='Test')
plt.plot(fpr_tts,  tpr_tts,  color = 'blue', lw=2, label='Top tag score')
plt.grid()
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate (bkg. eff.)')
plt.ylabel('True Positive Rate (sig. eff.)')
plt.legend(loc='lower right')
plt.savefig('plot_auc_dnn.pdf')


estimate_za = False
if estimate_za:
  n_quantiles = 10
  signal_mva_scores = {"bdt_score" : ks_test.logical_vector(pred_test, y_test, 1)}
  bkg_mva_scores = {"bdt_score" : ks_test.logical_vector(pred_test, y_test, 0)}
  data_mva_scores = {"bdt_score" : pred_data}

  signal_mass = ks_test.logical_vector(mass_validation, y_test, 1)
  bkg_mass = ks_test.logical_vector(mass_validation, y_test, 0)

  signal_weights = ks_test.logical_vector(weights_validation, y_test, 1)
  bkg_weights = ks_test.logical_vector(weights_validation, y_test, 0)

  optimization_vars = []#args.optimization_vars.split(",")
  for var in optimization_vars:
    signal_mva_scores[var] = ks_test.logical_vector(utils.load_array(f, var + '_validation'), y_test, 1)
    bkg_mva_scores[var]    = ks_test.logical_vector(utils.load_array(f, var + '_validation'), y_test, 0)
    data_mva_scores[var]   = utils.load_array(f, var + '_data')

  signal_events = { "mass" : signal_mass, "weights" : signal_weights, "mva_score" : signal_mva_scores }
  #bkg_events = { "mass" : bkg_mass, "weights" : bkg_weights, "mva_score" : bkg_mva_scores }
  data_events = { "mass" : mass_data, "weights" : weights_data, "mva_score" : data_mva_scores }

  #za, za_unc, s, b, sigma_eff = significance_utils.za_scores(n_quantiles, signal_events, bkg_events, False)
  za_data, za_unc_data, s_data, b_data, sigma_eff_data = significance_utils.za_scores(n_quantiles, signal_events, data_events, True)
  za = numpy.asarray(za_data)

  max_za = numpy.max(za)
  max_za_unc = za_unc_data[numpy.argmax(za)]
  print max_za, max_za_unc
  
