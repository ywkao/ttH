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


print xgboost.__version__ 
# Read features
f = h5py.File("ttHHadronic_features.hdf5")

global_features = f['global']
feature_names = f['feature_names']
label = f['label']
weights = f['weights']

global_features = numpy.asarray(global_features)
label = numpy.asarray(label)
weights = numpy.asarray(weights)
feature_names = numpy.asarray(feature_names)

train_frac = 0.8 # use this fraction of data for training, use 1-train_frac for testing
nTrain = int(len(label)*train_frac)

print global_features.shape
print label.shape
print weights.shape

x_train, x_test, y_train, y_test, weights_train, weights_test = train_test_split(global_features, label, weights, test_size = 1 - train_frac)

X_train = pandas.DataFrame(data=x_train, columns = feature_names)
X_test = pandas.DataFrame(data=x_test, columns = feature_names)
#d_train = xgboost.DMatrix(X_train, label = y_train)
d_train = xgboost.DMatrix(X_train, label = y_train, weight = weights_train)
d_test = xgboost.DMatrix(X_test, label = y_test)

# Define BDT parameters
param = { 
    	'max_depth': 5,
	'eta': 0.2,
	'objective': 'binary:logistic',
	}

n_round = 250
evallist = [(d_train, 'train'), (d_test, 'test')]
progress = {}

# train
#model = xgboost.XGBClassifier()
#model.train(param, d_train, n_round, evallist, evals_result = progress)
#model.fit(x_train, y_train)
bdt = xgboost.train(param, d_train, n_round, evallist, evals_result = progress)	

bdt.save_model("hadronic_bdt.xgb")
model = bdt.get_dump()

with open("model.json", "w") as fhout:
    fhout.write("[\n"+",\n".join(bdt.get_dump(dump_format="json"))+"\n]")

input_variables = []
for name in feature_names:
  input_variables.append((name, 'F'))
tmva_utils.convert_model(model, input_variables = input_variables, output_xml = 'hadronic_bdt.xml')

# predict
pred_train = bdt.predict(d_train)
pred_test = bdt.predict(d_test)

for i in range(100):
  print(pred_test[i])

# analysis
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
plt.savefig('feature_importance.pdf')

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
plt.savefig('plot.pdf', bbox_inches='tight')
