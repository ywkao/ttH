import os, sys
import numpy
import h5py
import xgboost
import pandas

from sklearn.cross_validation import train_test_split
from sklearn.utils import shuffle
from sklearn import metrics

import utils
import tmva_utils

import pickle

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("channel", help = "e.g. Hadronic or Leptonic", type=str)
parser.add_argument("tag", help = "tag to identify this training", type=str)
parser.add_argument("ext", help = "extension, e.g. '1'", type=str)
parser.add_argument("load", help = "loadandtest", type=bool, default=False)

args = parser.parse_args()

load_and_test = args.load

# Read features
print "ttH" + args.channel + "_features.hdf5"
f = h5py.File("ttH" + args.channel + "_features.hdf5")

feature_names = f['feature_names']

global_features = f['global']
label = f['label']
weights = f['weights']

global_features_validation = f['global_validation']
label_validation = f['label_validation']
weights_validation = f['weights_validation']

global_features = numpy.asarray(global_features)
label = numpy.asarray(label)
weights = numpy.asarray(weights)

global_features_validation = numpy.asarray(global_features_validation)
label_validation = numpy.asarray(label_validation)
weights_validation = numpy.asarray(weights_validation)

feature_names = numpy.asarray(feature_names)

train_frac = 1.0 # use this fraction of data for training, use 1-train_frac for testing
nTrain = int(len(label)*train_frac)

print global_features.shape
print label.shape
print weights.shape

print global_features_validation.shape
print label_validation.shape
print weights_validation.shape

x_train, y_train, weights_train = global_features, label, weights
x_test, y_test, weights_test  = global_features_validation, label_validation, weights_validation

#x_train, x_test, y_train, y_test, weights_train, weights_test = train_test_split(global_features, label, weights, test_size = 1 - train_frac)

X_train = pandas.DataFrame(data=x_train, columns = feature_names)
X_test = pandas.DataFrame(data=x_test, columns = feature_names)
#d_train = xgboost.DMatrix(X_train, label = y_train)
unique, count =  numpy.unique(label,return_counts=True)
weights_train = numpy.multiply(weights_train, numpy.where(label == 0, 1/(count[0]/float(sum(count))), 1) )
weights_train = numpy.multiply(weights_train, numpy.where(label == 1, 1/(count[1]/float(sum(count))), 1) )
weights_train = numpy.multiply(weights_train, numpy.where(label == 2, 1/(count[2]/float(sum(count))), 1) )
weights_train = numpy.multiply(weights_train, numpy.where(label == 3, 1/(count[3]/float(sum(count))), 1) )

d_train = xgboost.DMatrix(X_train, label = y_train, weight = weights_train)
d_test = xgboost.DMatrix(X_test, label = y_test)

unique, count =  numpy.unique(label,return_counts=True)
print count[0]/float(sum(count))
print count[1]/float(sum(count))
print count[2]/float(sum(count))
print count[3]/float(sum(count))
# need to feed this scales to weight

sum_neg_weights = utils.sum_of_weights(weights, label, 0)
sum_pos_weights = utils.sum_of_weights(weights, label, 1)

print sum_pos_weights, sum_neg_weights

# Define BDT parameters
param = { 
    	'max_depth': 8,
	'eta': 0.2,
#	'objective': 'binary:logistic',
	'objective': 'multi:softmax',
        'num_class': 4,
#	'scale_pos_weight': sum_neg_weights / sum_pos_weights,
	'subsample': 1.0,
	'colsample_bytree': 1.0,
	'nthread' : 8,
        
	}

n_round = 10
evallist = [(d_train, 'train'), (d_test, 'test')]
progress = {}

'''
if load_and_test:
  model = pickle.load(open(args.channel + args.tag + "_bdt.dat", "rb"))
  y_pred = model.predict(d_test)
  # evaluate predictions
  accuracy = metrics.accuracy_score(y_test, y_pred)
  print "accuracy: ", accuracy
  sys.exit()
'''
# train
bdt = xgboost.train(param, d_train, n_round, evallist, evals_result = progress)	

bdt.save_model(args.channel + args.tag + "_bdt.xgb")

pickle.dump(bdt, open(args.channel + args.tag + "_bdt.dat",'wb'))

test = bdt.predict(d_test, output_margin=True)

#numpy.set_printoptions(threshold=numpy.nan)
#print numpy.unique(test)
#print numpy.unique(y_test)
#print metrics.accuracy_score(test, y_test)
print test
print d_test.num_row(), test.shape
print d_test.feature_names

model = bdt.get_dump()

input_variables = []
for name in feature_names:
  input_variables.append((name, 'F'))
tmva_utils.convert_model(model, input_variables = input_variables, output_xml = args.channel + "_" + args.tag + '_' + args.ext + '_bdt.xml')

# predict
pred_train = bdt.predict(d_train)
pred_test = bdt.predict(d_test)

# analysis
fpr_train, tpr_train, thresh_train = metrics.roc_curve(y_train, pred_train, pos_label = 0)
fpr_test, tpr_test, thresh_test = metrics.roc_curve(y_test, pred_test, pos_label = 0)
#fpr_train, tpr_train, thresh_train = metrics.roc_curve(y_train, pred_train, sample_weight = weights_train)
#fpr_test, tpr_test, thresh_test = metrics.roc_curve(y_test, pred_test, sample_weight = weights_test)

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
