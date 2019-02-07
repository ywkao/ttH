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

import dnn_model
import significance_utils
import utils
import ks_test

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--tag", help = "save name for weights file", type=str)
parser.add_argument("--no_global", help = "don't use global features", action="store_true")
parser.add_argument("--no_lstm", help = "don't use object features (lstm)", action="store_true")
parser.add_argument("--evaluate", help = "give weights files to make learning curve and pick best one", type=str)
parser.add_argument("--load", help = "give weights file to use as starting point", type=str)
args = parser.parse_args()

def extract_epoch(weights_name):
  value = weights_name[-7:-5]
  return int(value)

def compare(item_1, item_2):
  return extract_epoch(item_1) - extract_epoch(item_2)

def organize(input_list):
  output_list = sorted(input_list, cmp = compare)
  return output_list

f = h5py.File(args.input, "r")
if args.evaluate:
  weights_list = glob.glob(args.evaluate)
  weights_list = organize(weights_list)
  print "Evaluating the following sets of weight files: "
  for file in weights_list:
    print file

object_features, object_features_validation, object_features_data = f['object'], f['object_validation'], f['object_data']
global_features, global_features_validation, global_features_data = f['global'], f['global_validation'], f['global_data']
label, label_validation, label_data = f['label'], f['label_validation'], f['label_data']
weights, weights_validation, weights_data = f['weights'], f['weights_validation'], f['weights_data']
mass, mass_validation, mass_data = f['mass'], f['mass_validation'], f['mass_data']
top_tag_score, top_tag_score_validation, top_tag_score_data = f['top_tag_score'], f['top_tag_score_validation'], f['top_tag_score_data']
tth_ttPP_mva, tth_ttPP_mva_validation, tth_ttPP_mva_data = f['tth_ttPP_mva'], f['tth_ttPP_mva_validation'], f['tth_ttPP_mva_data']
tth_dipho_mva, tth_dipho_mva_validation, tth_dipho_mva_data = f['tth_dipho_mva'], f['tth_dipho_mva_validation'], f['tth_dipho_mva_data']
tth_std_mva, tth_std_mva_validation, tth_std_mva_data = f['tth_std_mva'], f['tth_std_mva_validation'], f['tth_std_mva_data']

max_objects = len(object_features[0])
n_features = len(object_features[0][0])
n_global_features = len(global_features[0])

print(max_objects, n_features, len(object_features), n_global_features)

model = dnn_model.baseline_v1(max_objects, n_features, n_global_features, args.no_global, args.no_lstm, 200)

nEpochs = 10
nBatch = 10000

weights_file = "dnn_weights/" + args.tag + "_weights_{epoch:02d}.hdf5"
checkpoint = keras.callbacks.ModelCheckpoint(weights_file) # save after every epoch 
callbacks_list = [checkpoint]
#callbacks_list = []

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

if not args.evaluate:
  if args.load:
    print "Loading weights: %s" % args.load
    model.load_weights(args.load)
  model.fit([global_features, object_features], label, epochs = nEpochs, batch_size = nBatch, sample_weight = numpy.asarray(weights), callbacks = callbacks_list)

else:
  auc_train = []
  auc_test = []
  epochs = []
  for i in range(len(weights_list)):
    model.load_weights(weights_list[i])

    pred = model.predict([global_features, object_features], batch_size = nBatch)
    pred_test = model.predict([global_features_validation, object_features_validation], batch_size = nBatch)
    pred_data = model.predict([global_features_data, object_features_data], batch_size = nBatch)

    fpr_train, tpr_train, thresh_train = metrics.roc_curve(label, pred, pos_label = 1, sample_weight = weights_train)
    fpr_test,  tpr_test,  threst_test  = metrics.roc_curve(label_validation, pred_test, pos_label = 1, sample_weight = weights_validation)

    epochs.append(i+1)
    auc_train.append(metrics.auc(fpr_train, tpr_train, reorder = True))
    auc_test.append(metrics.auc(fpr_test, tpr_test, reorder = True))

    print "Epoch %d: train AUC = %.5f, test AUC = %.5f" % (i+1, metrics.auc(fpr_train, tpr_train, reorder = True), metrics.auc(fpr_test, tpr_test, reorder = True))

  fig = plt.figure()
  ax = fig.add_subplot(111)
  ax.yaxis.set_ticks_position('both')
  ax.grid(True)
  plt.plot(epochs, auc_train, color = 'darkred', label='Training Set')
  plt.plot(epochs,  auc_test, color = 'darkorange', label = 'Testing Set')


  best_weights_idx = numpy.argmax(numpy.asarray(auc_test))
  model.load_weights(weights_list[best_weights_idx])

pred = model.predict([global_features, object_features], batch_size = nBatch)
pred_test = model.predict([global_features_validation, object_features_validation], batch_size = nBatch)
pred_data = model.predict([global_features_data, object_features_data], batch_size = nBatch)

fpr_train, tpr_train, thresh_train = metrics.roc_curve(label, pred, pos_label = 1, sample_weight = weights_train)
fpr_test,  tpr_test,  threst_test  = metrics.roc_curve(label_validation, pred_test, pos_label = 1, sample_weight = weights_validation)

fpr_tts, tpr_tts, thresh_tts = metrics.roc_curve(label, top_tag_score, pos_label = 1, sample_weight = weights_train)
fpr_ttPP, tpr_ttPP, thresh_ttPP = metrics.roc_curve(label_validation, tth_ttPP_mva_validation, pos_label = 1, sample_weight = weights_validation)
fpr_dipho, tpr_dipho, thresh_dipho = metrics.roc_curve(label_validation, tth_dipho_mva_validation, pos_label = 1, sample_weight = weights_validation)
fpr_std, tpr_std, thresh_std = metrics.roc_curve(label_validation, tth_std_mva_validation, pos_label = 1, sample_weight = weights_validation)

auc_train, auc_train_unc = utils.auc_and_unc(label, pred, weights_train, 100)
auc_test,  auc_test_unc  = utils.auc_and_unc(label_validation, pred_test, weights_validation, 100)
auc_tts,   auc_tts_unc   = utils.auc_and_unc(label, top_tag_score, weights_train, 100)
auc_ttPP,  auc_ttPP_unc  = utils.auc_and_unc(label_validation, tth_ttPP_mva_validation, weights_validation, 100)
auc_dipho,  auc_dipho_unc  = utils.auc_and_unc(label_validation, tth_dipho_mva_validation, weights_validation, 100)
auc_std_bdt, auc_std_bdt_unc = utils.auc_and_unc(label_validation, tth_std_mva_validation, weights_validation, 100)

print("Training AUC: %.5f +/- %.5f") % (auc_train, auc_train_unc) 
print("Testing AUC: %.5f +/- %.5f") % (auc_test, auc_test_unc)
print("Top Tag AUC: %.5f +/- %.5f") % (auc_tts, auc_tts_unc)
print("TTPP BDT AUC: %.5f +/- %.5f") % (auc_ttPP, auc_ttPP_unc)
print("Std. BDT AUC: %.5f +/- %.5f") % (auc_std_bdt, auc_std_bdt_unc)

numpy.savez("dnn_rocs_%s.npz" % (args.tag), fpr_train = fpr_train, tpr_train = tpr_train, fpr_test = fpr_test, tpr_test = tpr_test, fpr_tts = fpr_tts, tpr_tts = tpr_tts, fpr_ttPP = fpr_ttPP, tpr_ttPP = tpr_ttPP, fpr_std = fpr_std, tpr_std = tpr_std, fpr_dipho = fpr_dipho, tpr_dipho = tpr_dipho)
y_test = label_validation

fig = plt.figure()
ax = fig.add_subplot(111)
ax.yaxis.set_ticks_position('both')
ax.grid(True)
plt.plot(fpr_train, tpr_train, color='darkred', lw=2, label='DNN (Train) [AUC = %.3f]' % (metrics.auc(fpr_train, tpr_train, reorder = True)))
#plt.plot(fpr_bdt, tpr_bdt, color='blue', lw=2, label='BDT')
plt.plot(fpr_test, tpr_test, color = 'darkorange', lw=2, label='DNN (Test) [AUC = %.3f]' % (metrics.auc(fpr_test , tpr_test, reorder = True)))
plt.plot(fpr_tts,  tpr_tts,  color = 'blue', lw=2, label='Top tag score [AUC = %.3f]' % (metrics.auc(fpr_tts,  tpr_tts, reorder = True)))
#plt.plot(fpr_ttPP, tpr_ttPP, color = 'green', lw=2, label='ttPP BDT')
plt.grid()
plt.ylim([0.0, 1.0])
plt.xlim([0.0, 1.0])
plt.xlabel('False Positive Rate (bkg. eff.)')
plt.ylabel('True Positive Rate (sig. eff.)')
plt.legend(loc='lower right')
plt.savefig('plot_auc_dnn.pdf')


bkg_dnn = ks_test.logical_vector(pred_test, y_test, 0)
bkg_ttPP = ks_test.logical_vector(tth_ttPP_mva_validation, y_test, 0)
bkg_dipho = ks_test.logical_vector(tth_dipho_mva_validation, y_test, 0)
bkg_mass = ks_test.logical_vector(mass_validation, y_test, 0)

fig = plt.figure()
ax = fig.add_subplot(111)
ax.scatter(bkg_mass, bkg_dnn, label = 'Bkg Events', edgecolors = 'none', marker=".", c = 'blue')
plt.ylim([0.0, 1.0])
plt.xlim([100, 150])
plt.xlabel('m_gg [GeV]')
plt.ylabel('DNN Score')
plt.legend(loc='upper right')
plt.savefig('dnn_score_vs_mass.pdf')

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
  
