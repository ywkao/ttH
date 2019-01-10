import os, sys
import numpy
import h5py
import xgboost
import pandas
import json
import math

from sklearn.cross_validation import train_test_split
from sklearn.utils import shuffle
from sklearn import metrics

import utils
import tmva_utils
import ks_test
import significance_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("channel", help = "e.g. Hadronic or Leptonic", type=str)
parser.add_argument("input", help = "input hdf5 file", type=str)
parser.add_argument("ext", help = "extension, e.g. '1'", type=str)
parser.add_argument("tag", help = "tag to identify this training", type=str)
parser.add_argument("-m", "--multi", help = "run a multiclassifier based BDT", action="store_true")
parser.add_argument("-d", "--data", help = "check ZA on data also", action="store_true")
parser.add_argument("-r", "--res", help = "weight signal events by relative mass resolution", action = "store_true")
parser.add_argument("-s", "--sideband", help = "use data sideband for training", action = "store_true")
args = parser.parse_args()

# Read features
f = h5py.File(args.input.replace(".hdf5", "") + ".hdf5", "r")

feature_names = f['feature_names']

global_features = f['global']
label = f['label']
multi_label = f['multi_label']
weights = f['weights']
mass = f['mass']
lead_sigmaEtoE = f['lead_sigmaEtoE']
sublead_sigmaEtoE = f['sublead_sigmaEtoE']

if args.sideband:
  global_features = f['global_data_sideband_mc']
  label = f['label_data_sideband_mc']
  multi_label = f['multi_label_data_sideband_mc']
  weights = f['weights_data_sideband_mc']
  mass = f['mass_data_sideband_mc']

global_features_validation = f['global_validation']
label_validation = f['label_validation']
multi_label_validation = f['multi_label_validation']
weights_validation = f['weights_validation']
mass_validation = f['mass_validation']

global_features_data = f['global_data']
label_data = f['label_data']
multi_label_data = f['multi_label_data']
weights_data = f['weights_data']
mass_data = f['mass_data']

global_features = numpy.asarray(global_features)
label = numpy.asarray(label)
multi_label = numpy.asarray(multi_label)
weights = numpy.asarray(weights)
mass = numpy.asarray(mass)
lead_sigmaEtoE = numpy.asarray(lead_sigmaEtoE)
sublead_sigmaEtoE = numpy.asarray(sublead_sigmaEtoE)

global_features_validation = numpy.asarray(global_features_validation)
label_validation = numpy.asarray(label_validation)
multi_label_validation = numpy.asarray(multi_label_validation)
weights_validation = numpy.asarray(weights_validation)
mass_validation = numpy.asarray(mass_validation)

global_features_data = numpy.asarray(global_features_data)
label_data = numpy.asarray(label_data)
multi_label_data = numpy.asarray(multi_label_data)
weights_data = numpy.asarray(weights_data)
mass_data = numpy.asarray(mass_data)

feature_names = numpy.asarray(feature_names)

train_frac = 1.0 # use this fraction of data for training, use 1-train_frac for testing
nTrain = int(len(label)*train_frac)

print global_features.shape
print label.shape
print weights.shape

print global_features_validation.shape
print label_validation.shape
print weights_validation.shape

print global_features_data.shape
print label_data.shape
print weights_data.shape


x_train, y_train, y_train_multi, weights_train = global_features, label, multi_label, weights
x_test, y_test, y_test_multi, weights_test  = global_features_validation, label_validation, multi_label_validation, weights_validation

X_train = pandas.DataFrame(data=x_train, columns = feature_names)
X_test = pandas.DataFrame(data=x_test, columns = feature_names)
X_data = pandas.DataFrame(data=global_features_data, columns = feature_names)

if args.multi:
  Y_train = y_train_multi 
  Y_test = y_test_multi
else:
  Y_train = y_train
  Y_test = y_test

#unique, count =  numpy.unique(multi_label,return_counts=True)
#weights_train = numpy.multiply(weights_train, numpy.where(multi_label == 0, 1/(count[0]/float(sum(count))), 1) )
#weights_train = numpy.multiply(weights_train, numpy.where(multi_label == 1, 1/(count[1]/float(sum(count))), 1) )
#weights_train = numpy.multiply(weights_train, numpy.where(multi_label == 2, 1/(count[2]/float(sum(count))), 1) )
#weights_train = numpy.multiply(weights_train, numpy.where(multi_label == 3, 1/(count[3]/float(sum(count))), 1) )
#weights_train = numpy.multiply(weights_train, numpy.where(multi_label == 4, 1/(count[4]/float(sum(count))), 1) )

sum_neg_weights = utils.sum_of_weights(weights_train, label, 0)
sum_pos_weights = utils.sum_of_weights(weights_train, label, 1)

print sum_pos_weights, sum_neg_weights

if args.multi:
  for i in range(len(weights_train)):
    if label[i] == 1:
      weights_train[i] *= (sum_neg_weights / sum_pos_weights)

if args.res:
  for i in range(len(weights_train)):
    if label[i] == 1:
      print weights_train[i], 1/math.sqrt(lead_sigmaEtoE[i] ** 2 + sublead_sigmaEtoE[i] ** 2)
      weights_train[i] *= 1/math.sqrt(lead_sigmaEtoE[i] ** 2 + sublead_sigmaEtoE[i] ** 2) 
      print weights_train[i]

sum_neg_weights = utils.sum_of_weights(weights_train, label, 0)
sum_pos_weights = utils.sum_of_weights(weights_train, label, 1)

print sum_pos_weights, sum_neg_weights

d_train = xgboost.DMatrix(X_train, label = Y_train, weight = weights_train)
d_test = xgboost.DMatrix(X_test, label = Y_test)
d_data = xgboost.DMatrix(X_data)

# Define BDT parameters
param = { 
    	'max_depth': 4,
	'eta': 0.2,
	'objective': 'binary:logistic',
	'scale_pos_weight': sum_neg_weights / sum_pos_weights,
	'subsample': 1.0,
	'colsample_bytree': 1.0,
	'nthread' : 8,
	'min_child_weight' : 1,
	}

if args.multi:
  param["num_class"] = 5
  param["objective"] = "multi:softprob"
  param["scale_pos_weight"] = 1

print param

n_round = 75
evallist = [(d_train, 'train'), (d_test, 'test')]
progress = {}

# or, read hyperparameters from dictionary
if "hyperparameter_grid_search" in args.tag:
  hyperparams = {}
  with open("hyperparameter_points.json") as f_in:
    all_hyperparams = json.load(f_in)
  hyperparameter_choice = args.tag.strip("hyperparameter_grid_search").strip("_")
  hyperparams = all_hyperparams[hyperparameter_choice]
  new_param = hyperparams["params"]
  for key, value in param.iteritems():
    if key in new_param:
      param[key] = new_param[key]
  n_round = hyperparams["n_rounds"]

print param, n_round

# train
bdt = xgboost.train(param, d_train, n_round, evallist, evals_result = progress)	

bdt.save_model(args.channel + "_" + args.tag + "_" + args.ext + "_bdt.xgb")
model = bdt.get_dump()

input_variables = []
for name in feature_names:
  input_variables.append((name, 'F'))
tmva_utils.convert_model(model, input_variables = input_variables, output_xml = args.channel + "_" + args.tag + "_" + args.ext + '_bdt.xml')

# predict
pred_train = bdt.predict(d_train, output_margin=args.multi)
pred_test = bdt.predict(d_test, output_margin=args.multi)
pred_data = bdt.predict(d_data, output_margin=args.multi)


print pred_test.shape

if args.multi:
  pred_train = pred_train[:,0] 
  pred_test = pred_test[:,0] 
  pred_data = pred_data[:,0]

print pred_test.shape

# analysis
# ks test
d_sig, p_value_sig, d_bkg, p_value_bkg = ks_test.ks_test(pred_train, pred_test, y_train, y_test)
print "Results of ks-test (d-score) for signal: %.10f and background: %.10f" % (d_sig, d_bkg)
print "Results of ks-test (p-value) for signal: %.10f and background: %.10f" % (p_value_sig, p_value_bkg)

# roc curves
fpr_train, tpr_train, thresh_train = metrics.roc_curve(y_train, pred_train)
fpr_test, tpr_test, thresh_test = metrics.roc_curve(y_test, pred_test)

auc_train = metrics.auc(fpr_train, tpr_train)
auc_test = metrics.auc(fpr_test, tpr_test)

print("Training AUC: %.4f" % auc_train)
print("Testing  AUC: %.4f" % auc_test)

### Make diagnostic plots ###
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# variable importance #
fig = plt.figure()
xgboost.plot_importance(bdt)
plt.tight_layout()
plt.savefig('feature_importance_' + args.channel + '.pdf')

# make ROC curve #
fig = plt.figure()
ax = fig.add_subplot(111)
ax.yaxis.set_ticks_position('both')
ax.grid(True)
plt.grid(color='black', linestyle='--', linewidth = 0.1, which = 'both')

plt.plot(fpr_train, tpr_train, color='red', label='Training Set', lw = 3)
plt.plot(fpr_test, tpr_test, color='green', label='Testing Set', lw = 3)

plt.xscale('log')

plt.xlim([0.005, 1.0])
plt.ylim([0.3, 1.05])
plt.xlabel('False Positive Rate (background efficiency)')
plt.ylabel('True Positive Rate (signal efficiency)')
plt.legend(loc='lower right')
plt.savefig('roc' + args.channel + '.pdf', bbox_inches='tight')

estimate_za = True
if estimate_za:
  ref_file = "/home/users/sjmay/ttH/Loopers/Optimization/ZA_curves/MVAOptimizationBaby_1_Leptonic_reproduce_1_bdt.npz"
  ref2_file = "/home/users/sjmay/ttH/Loopers/Optimization/ZA_curves/MVAOptimizationBaby_1_Leptonic_baseline_20var_14Oct2018_1_bdt.npz"
  ref_results = numpy.load(ref_file)
  ref2_results = numpy.load(ref2_file)

  s_ref = ref_results["n_sig_mc"]
  za_ref = ref_results["za_mc"]
  za_ref_unc = ref_results["za_unc_mc"]

  s_ref_data = ref_results["n_sig_data"]
  za_ref_data = ref_results["za_data"]
  za_ref_unc_data = ref_results["za_unc_data"]

  s_ref2 = ref2_results["n_sig_mc_ref"]
  za_ref2 = ref2_results["za_mc_ref"]
  za_ref2_unc = ref2_results["za_unc_mc_ref"]

  s_ref2_data = ref2_results["n_sig_data_ref"]
  za_ref2_data = ref2_results["za_data_ref"]
  za_ref2_unc_data = ref2_results["za_unc_data_ref"]

  n_quantiles = 100
  signal_mva_scores = ks_test.logical_vector(pred_test, y_test, 1)
  bkg_mva_scores = ks_test.logical_vector(pred_test, y_test, 0)

  signal_mass = ks_test.logical_vector(mass_validation, y_test, 1)
  bkg_mass = ks_test.logical_vector(mass_validation, y_test, 0)

  signal_weights = ks_test.logical_vector(weights_validation, y_test, 1)
  bkg_weights = ks_test.logical_vector(weights_validation, y_test, 0)

  signal_events = { "mass" : signal_mass, "weights" : signal_weights, "mva_score" : signal_mva_scores }
  bkg_events = { "mass" : bkg_mass, "weights" : bkg_weights, "mva_score" : bkg_mva_scores }
  data_events = { "mass" : mass_data, "weights" : weights_data, "mva_score" : pred_data }  

  za, za_unc, s, b, sigma_eff = significance_utils.za_scores(n_quantiles, signal_events, bkg_events, False)
  za_data, za_unc_data, s_data, b_data, sigma_eff_data = significance_utils.za_scores(n_quantiles, signal_events, data_events, True)
  za = numpy.asarray(za)

  max_za = numpy.max(za)
  max_za_unc = za_unc[numpy.argmax(za)]
  print max_za, max_za_unc

  numpy.savez("za_%s.npz" % (args.channel + "_" + args.ext + "_" + args.tag), za = za, za_unc = za_unc, signal = s, bkg = b, sigma_eff = sigma_eff, za_data = za_data, za_unc_data = za_unc_data, signal_data = s_data, bkg_data = b_data, sigma_eff_data = sigma_eff_data)
  numpy.savez("sigma_eff.npz", sigma_eff = sigma_eff, n_sig = s)

  import matplotlib
  matplotlib.use('Agg')
  import matplotlib.pyplot as plt

  fig = plt.figure()
  ax1 = fig.add_subplot(111)
  #if not args.data:
  ax1.plot(s, za, label='MC', color = 'red')
  ax1.fill_between(s, numpy.asarray(za) - numpy.asarray(za_unc), numpy.asarray(za) + numpy.asarray(za_unc), color = 'red', alpha = 0.25)
    #ax1.plot(s_ref, za_ref, label='Reproduce 2017 BDT (MC)', color = 'blue')
    #ax1.fill_between(s_ref, numpy.asarray(za_ref) - numpy.asarray(za_ref_unc), numpy.asarray(za_ref) + numpy.asarray(za_ref_unc), color = 'blue', alpha = 0.25)
    #ax1.plot(s_ref2, za_ref2, label='2017 ttH BDT (MC)', color = 'black', linestyle = '--', dashes = (5,2))
    #ax1.fill_between(s_ref2, numpy.asarray(za_ref2) - numpy.asarray(za_ref2_unc), numpy.asarray(za_ref2) + numpy.asarray(za_ref2_unc), color = 'black', alpha = 0.25)

  #else:
  ax1.plot(s_data, za_data, label='Data', color = 'black')
  ax1.fill_between(s_data, numpy.asarray(za_data) - numpy.asarray(za_unc_data), numpy.asarray(za_data) + numpy.asarray(za_unc_data), color = 'black', alpha = 0.25)
    #ax1.plot(s_ref_data, za_ref_data, label='Reproduce 2017 BDT (Data)', color = 'blue')
    #ax1.fill_between(s_ref_data, numpy.asarray(za_ref_data) - numpy.asarray(za_ref_unc_data), numpy.asarray(za_ref_data) + numpy.asarray(za_ref_unc_data), color = 'blue', alpha = 0.25)
    #ax1.plot(s_ref2_data, za_ref2_data, label='2017 ttH BDT (Data)', color = 'black', linestyle = '--', dashes = (5,2))
    #ax1.fill_between(s_ref2_data, numpy.asarray(za_ref2_data) - numpy.asarray(za_ref2_unc_data), numpy.asarray(za_ref2_data) + numpy.asarray(za_ref2_unc_data), color = 'black', alpha = 0.25)


  plt.xlabel('# Signal Events')
  ax1.set_ylabel('Significance (Z_A)')

  plt.ylim([0.0, 3.0])
  l, r = plt.xlim()
  plt.xlim([1.0, r])

  ax1.legend(loc='upper right')
  plt.savefig('za_curve.pdf')
