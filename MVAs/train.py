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

import ROOT
import root_numpy

import utils
import tmva_utils
import ks_test
import significance_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--channel", help = "e.g. Hadronic or Leptonic", type=str)
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--ext", help = "extension, e.g. '1'", type=str)
parser.add_argument("--tag", help = "tag to identify this training", type=str)
parser.add_argument("-m", "--multi", help = "run a multiclassifier based BDT", action="store_true")
parser.add_argument("-r", "--res", help = "weight signal events by relative mass resolution", action = "store_true")
parser.add_argument("-s", "--sideband", help = "use data sideband for training", action = "store_true")
parser.add_argument("--njets_cut", help = "apply manual cut on njets >= 4", action = "store_true")
parser.add_argument("--optimization_vars", help = "csv list of additional variables to perform N-d Z_A optimization scan in (along with BDT score)", type=str)
args = parser.parse_args()


### Read features ###
f = h5py.File(args.input.replace(".hdf5", "") + ".hdf5", "r")

feature_names = utils.load_array(f, 'feature_names')
training_feature_names = utils.load_array(f, 'training_feature_names')

print training_feature_names

global_features = utils.load_array(f, 'global')
label = utils.load_array(f, 'label')
multi_label = utils.load_array(f, 'multi_label')
weights = utils.load_array(f, 'weights')
mass = utils.load_array(f, 'mass')
njets = utils.load_array(f, 'njets')
lead_sigmaEtoE = utils.load_array(f, 'lead_sigmaEtoE')
sublead_sigmaEtoE = utils.load_array(f, 'sublead_sigmaEtoE')
signal_mass_label = utils.load_array(f, 'signal_mass_label')
tth_2017_reference_mva = utils.load_array(f, 'tth_2017_reference_mva')
evt = utils.load_array(f, 'evt')
run = utils.load_array(f, 'run')
lumi = utils.load_array(f, 'lumi')
process_id = utils.load_array(f, 'process_id')
year = utils.load_array(f, 'year')

if args.sideband:
  global_features = utils.load_array(f, 'global_data_sideband')
  label = utils.load_array(f, 'label_data_sideband')
  multi_label = utils.load_array(f, 'multi_label_data_sideband')
  weights = utils.load_array(f, 'weights_data_sideband')
  mass = utils.load_array(f, 'mass_data_sideband')
  #lead_sigmaEtoE = utils.load_array(f, 'lead_sigmaEtoE_data_sideband')
  #sublead_sigmaEtoE = utils.load_array(f, 'sublead_sigmaEtoE_data_sideband')

global_features_validation = utils.load_array(f, 'global_validation')
label_validation = utils.load_array(f, 'label_validation')
multi_label_validation = utils.load_array(f, 'multi_label_validation')
weights_validation = utils.load_array(f, 'weights_validation')
mass_validation = utils.load_array(f, 'mass_validation')
njets_validation = utils.load_array(f, 'njets_validation')
signal_mass_label_validation = utils.load_array(f, 'signal_mass_label_validation')
tth_2017_reference_mva_validation = utils.load_array(f, 'tth_2017_reference_mva_validation')
evt_validation = utils.load_array(f, 'evt_validation')
run_validation = utils.load_array(f, 'run_validation')
lumi_validation = utils.load_array(f, 'lumi_validation')
process_id_validation = utils.load_array(f, 'process_id_validation')
year_validation = utils.load_array(f, 'year_validation')

global_features_data = utils.load_array(f, 'global_data')
label_data = utils.load_array(f, 'label_data')
multi_label_data = utils.load_array(f, 'multi_label_data')
weights_data = utils.load_array(f, 'weights_data')
mass_data = utils.load_array(f, 'mass_data')
njets_data = utils.load_array(f, 'njets_data')
signal_mass_label_data = utils.load_array(f, 'signal_mass_label_data')
tth_2017_reference_mva_data = utils.load_array(f, 'tth_2017_reference_mva_data')
evt_data = utils.load_array(f, 'evt_data')
run_data = utils.load_array(f, 'run_data')
lumi_data = utils.load_array(f, 'lumi_data')
process_id_data = utils.load_array(f, 'process_id_data')
year_data = utils.load_array(f, 'year_data')

global_features_final_fit = utils.load_array(f, 'global_final_fit')
label_final_fit = utils.load_array(f, 'label_final_fit')
multi_label_final_fit = utils.load_array(f, 'multi_label_final_fit')
weights_final_fit = utils.load_array(f, 'weights_final_fit')
mass_final_fit = utils.load_array(f, 'mass_final_fit')
njets_final_fit = utils.load_array(f, 'njets_final_fit')
signal_mass_label_final_fit = utils.load_array(f, 'signal_mass_label_final_fit')
tth_2017_reference_mva_final_fit = utils.load_array(f, 'tth_2017_reference_mva_final_fit')
evt_final_fit = utils.load_array(f, 'evt_final_fit')
run_final_fit = utils.load_array(f, 'run_final_fit')
lumi_final_fit = utils.load_array(f, 'lumi_final_fit')
process_id_final_fit = utils.load_array(f, 'process_id_final_fit')
year_final_fit = utils.load_array(f, 'year_final_fit')

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

X_train = pandas.DataFrame(data=x_train, columns = training_feature_names)
X_test = pandas.DataFrame(data=x_test, columns = training_feature_names)
X_data = pandas.DataFrame(data=global_features_data, columns = training_feature_names)
X_final_fit = pandas.DataFrame(data=global_features_final_fit, columns = training_feature_names)

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
d_final_fit = xgboost.DMatrix(X_final_fit)

# Define BDT parameters
param = { 
    	'max_depth': 4,
	'eta': 0.2,
	'objective': 'binary:logistic',
	'scale_pos_weight': sum_neg_weights / sum_pos_weights,
	'subsample': 1.0,
	'colsample_bytree': 1.0,
	'nthread' : 12,
	'min_child_weight' : 1,
	}

if args.multi:
  param["num_class"] = 5
  param["objective"] = "multi:softprob"
  param["scale_pos_weight"] = 1

print param

n_round = 300 if args.channel == "Hadronic" else 150
if "FCNC" in args.input:
  n_round = 50
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
pred_final_fit = bdt.predict(d_final_fit, output_margin=args.multi)

print pred_test.shape

if args.multi:
  pred_train = pred_train[:,0] 
  pred_test = pred_test[:,0] 
  pred_data = pred_data[:,0]
  pred_final_fit = pred_final_fit[:,0]

print pred_test.shape

# analysis
# ks test
d_sig, p_value_sig, d_bkg, p_value_bkg = ks_test.ks_test(pred_train, pred_test, y_train, y_test)
print "Results of ks-test (d-score) for signal: %.10f and background: %.10f" % (d_sig, d_bkg)
print "Results of ks-test (p-value) for signal: %.10f and background: %.10f" % (p_value_sig, p_value_bkg)

# roc curves
fpr_train, tpr_train, thresh_train = metrics.roc_curve(y_train, pred_train, pos_label = 1, sample_weight = weights_train)
fpr_test, tpr_test, thresh_test = metrics.roc_curve(y_test, pred_test, pos_label = 1, sample_weight = weights_test)

auc_train = metrics.auc(fpr_train, tpr_train, reorder = True)
auc_test  = metrics.auc(fpr_test , tpr_test , reorder = True)

auc, unc = utils.auc_and_unc(y_test, pred_test, weights_test, 100)

print "Training AUC: %.3f" % auc_train
print "Testing  AUC: %.3f" % auc_test

print "Testing  AUC: %.3f +/- %.4f" % (auc, unc)
numpy.savez("bdt_roc_%s.npz" % (args.channel + "_" + args.tag), y_train = y_train, y_test = y_test, pred_train = pred_train, pred_test = pred_test, fpr_train = fpr_train, fpr_test = fpr_test, tpr_train = tpr_train, tpr_test = tpr_test)

# Write output to TTree
tree_train_id = numpy.concatenate((numpy.zeros(len(pred_train)), numpy.ones(len(pred_test)), numpy.ones(len(pred_data)), numpy.ones(len(pred_final_fit))))
tree_sample_id = numpy.concatenate((label, label_validation, label_data, numpy.ones(len(pred_final_fit))))
tree_mass = numpy.concatenate((mass, mass_validation, mass_data, mass_final_fit))
tree_weight = numpy.concatenate((weights, weights_validation, weights_data, weights_final_fit))
tree_bdt_score = numpy.concatenate((pred_train, pred_test, pred_data, pred_final_fit))
tree_signal_mass_label = numpy.concatenate((signal_mass_label, signal_mass_label_validation, signal_mass_label_data, numpy.zeros(len(pred_final_fit))))
tree_tth_2017_reference_mva = numpy.concatenate((tth_2017_reference_mva, tth_2017_reference_mva_validation, tth_2017_reference_mva_data, tth_2017_reference_mva_final_fit))
tree_evt = numpy.concatenate((evt, evt_validation, evt_data, evt_final_fit))
tree_run = numpy.concatenate((run, run_validation, run_data, run_final_fit))
tree_lumi = numpy.concatenate((lumi, lumi_validation, lumi_data, lumi_final_fit))
tree_process_id = numpy.concatenate((process_id, process_id_validation, process_id_data, process_id_final_fit))
tree_year = numpy.concatenate((year, year_validation, year_data, year_final_fit))
tree_global_features = numpy.concatenate((global_features, global_features_validation, global_features_data, global_features_final_fit))

tree_train_id = tree_train_id.astype(numpy.int64)
tree_sample_id = tree_sample_id.astype(numpy.int64)
tree_mass = tree_mass.astype(numpy.float64)
tree_weight = tree_weight.astype(numpy.float64)
tree_bdt_score = tree_bdt_score.astype(numpy.float64)
tree_signal_mass_label = tree_signal_mass_label.astype(numpy.int64)
tree_tth_2017_reference_mva = tree_tth_2017_reference_mva.astype(numpy.float64)
tree_evt = tree_evt.astype(numpy.uint64)
tree_run = tree_run.astype(numpy.uint64)
tree_lumi = tree_lumi.astype(numpy.uint64)
tree_process_id = tree_process_id.astype(numpy.int64)
tree_year = tree_year.astype(numpy.int64)
tree_global_features = tree_global_features.astype(numpy.float64)

dict = {"train_id" : tree_train_id, "sample_id" : tree_sample_id, "mass" : tree_mass, "weight" : tree_weight, "mva_score" : tree_bdt_score, "signal_mass_label" : tree_signal_mass_label, "tth_2017_reference_mva" : tree_tth_2017_reference_mva, "process_id" : tree_process_id, "year" : tree_year, "event" : tree_evt, "lumi" : tree_lumi, "run" : tree_run, "global_features" : tree_global_features}
utils.numpy_to_tree(dict, "ttH%s_%s_FinalFitTree.root" % (args.channel, args.tag))

#out_array = numpy.core.records.fromarrays([tree_train_id, tree_sample_id, tree_mass, tree_weight, tree_bdt_score, tree_signal_mass_label, tree_tth_2017_reference_mva], names = 'train_id,sample_id,mass,weight,mva_score,signal_mass_label,tth_2017_reference_mva')
#out_array = numpy.core.records.fromarrays([tree_train_id, tree_sample_id, tree_mass, tree_weight, tree_bdt_score, tree_signal_mass_label], dtype = [('train_id','<u8'), ('sample_id','<u8'), ('mass','<f8'),('weight','<f8'), ('mva_score','<f8'),('signal_mass_label','<u8')])
#root_numpy.array2root(out_array, "ttH%s_%s_FinalFitTree.root" % (args.channel, args.tag), treename = 't')


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
  n_quantiles = 50
  signal_mva_scores = {"bdt_score" : ks_test.logical_vector(pred_test, y_test, 1)}
  bkg_mva_scores = {"bdt_score" : ks_test.logical_vector(pred_test, y_test, 0)}
  data_mva_scores = {"bdt_score" : pred_data}

  signal_mass = ks_test.logical_vector(mass_validation, y_test, 1)
  bkg_mass = ks_test.logical_vector(mass_validation, y_test, 0)

  signal_njets = ks_test.logical_vector(njets_validation, y_test, 1)
  bkg_njets = ks_test.logical_vector(njets_validation, y_test, 0)

  signal_weights = ks_test.logical_vector(weights_validation, y_test, 1)
  bkg_weights = ks_test.logical_vector(weights_validation, y_test, 0)

  optimization_vars = args.optimization_vars.split(",") if args.optimization_vars else []
  for var in optimization_vars: 
    signal_mva_scores[var] = ks_test.logical_vector(utils.load_array(f, var + '_validation'), y_test, 1)
    bkg_mva_scores[var]    = ks_test.logical_vector(utils.load_array(f, var + '_validation'), y_test, 0)
    data_mva_scores[var]   = utils.load_array(f, var + '_data')

  signal_events = { "mass" : signal_mass, "weights" : signal_weights, "mva_score" : signal_mva_scores, "njets" : signal_njets } 
  bkg_events = { "mass" : bkg_mass, "weights" : bkg_weights, "mva_score" : bkg_mva_scores , "njets" : bkg_njets} 
  data_events = { "mass" : mass_data, "weights" : weights_data, "mva_score" : data_mva_scores , "njets" : njets_data} 

  za, za_unc, s, b, sigma_eff = significance_utils.za_scores(n_quantiles, signal_events, bkg_events, False, args.njets_cut)
  za_data, za_unc_data, s_data, b_data, sigma_eff_data = significance_utils.za_scores(n_quantiles, signal_events, data_events, True, args.njets_cut)
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
  ax1.plot(s, za, label='MC', color = 'red')
  ax1.fill_between(s, numpy.asarray(za) - numpy.asarray(za_unc), numpy.asarray(za) + numpy.asarray(za_unc), color = 'red', alpha = 0.25)
  ax1.plot(s_data, za_data, label='Data', color = 'black')
  ax1.fill_between(s_data, numpy.asarray(za_data) - numpy.asarray(za_unc_data), numpy.asarray(za_data) + numpy.asarray(za_unc_data), color = 'black', alpha = 0.25)

  plt.xlabel('# Signal Events')
  ax1.set_ylabel('Significance (Z_A)')

  plt.ylim([0.0, 3.0])
  l, r = plt.xlim()
  plt.xlim([1.0, r])

  ax1.legend(loc='upper right')
  plt.savefig('za_curve.pdf')
