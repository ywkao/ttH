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
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--tag", help = "tag to identify this training", type=str)
args = parser.parse_args()

# Read features
f = h5py.File(args.input.replace(".hdf5", "") + ".hdf5", "r")

feature_names = f['feature_names']
features = f['features']
label = f['label']

feature_names = numpy.asarray(feature_names)
features = numpy.asarray(features)
label = numpy.asarray(label)

x_train, x_test, y_train, y_test = train_test_split(features, label, test_size = 0.5)

X_train = pandas.DataFrame(data=x_train, columns = feature_names)
X_test = pandas.DataFrame(data=x_test, columns = feature_names)

#sum_neg_weights = utils.sum_of_weights(weights_train, label, 0)
#sum_pos_weights = utils.sum_of_weights(weights_train, label, 1)

#print sum_pos_weights, sum_neg_weights

d_train = xgboost.DMatrix(X_train, label = y_train) 
d_test = xgboost.DMatrix(X_test, label = y_test)

# Define BDT parameters
param = { 
    	'max_depth': 4,
	'eta': 0.2,
	'objective': 'binary:logistic',
	'scale_pos_weight': 1, # might want to change later 
	'subsample': 1.0,
	'colsample_bytree': 1.0,
	'nthread' : 8,
	'min_child_weight' : 1,
	}

print param

n_round = 75
evallist = [(d_train, 'train'), (d_test, 'test')]
progress = {}

# train
bdt = xgboost.train(param, d_train, n_round, evallist, evals_result = progress)	

bdt.save_model("GJetReweight_" + args.tag + '_bdt.xgb')
model = bdt.get_dump()

input_variables = []
for name in feature_names:
  input_variables.append((name, 'F'))
tmva_utils.convert_model(model, input_variables = input_variables, output_xml = "GJetReweight_" + args.tag + '_bdt.xml')

# predict
pred_train = bdt.predict(d_train, output_margin=False)
pred_test = bdt.predict(d_test, output_margin=False)

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
plt.savefig('feature_importance_GJetReweight.pdf')

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
plt.savefig('roc_GJetReweight.pdf', bbox_inches='tight')
