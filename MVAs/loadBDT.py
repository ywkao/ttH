import os, sys
import h5py
import ROOT
import numpy
import root_numpy
import pandas
import xgboost
from sklearn.cross_validation import train_test_split
from sklearn.utils import shuffle
from sklearn import metrics

import utils
import tmva_utils

import math
import argparse

import pickle

parser = argparse.ArgumentParser()
parser.add_argument("channel", help = "e.g. Hadronic or Leptonic", type=str)
parser.add_argument("inputfile", help = "", type=str)
parser.add_argument("-r", "--randomize", help = "use a random test/train split", action="store_true")
parser.add_argument("-l", "--load", help = "load saved model", action="store_true")

args = parser.parse_args()

baby_file = "../Loopers/MVABaby_ttH" + args.channel + ".root"
#output_file = "ttH" + args.channel + "_features.hdf5"
f = ROOT.TFile(args.inputfile, 'UPDATE')
tree = f.Get("t")

# load tree to array
feature_names = (root_numpy.tree2array(tree, branches = ["mva_branches"], start=0, stop=1))[0][0]
feature_names = list(feature_names) 
print feature_names
branches = numpy.concatenate((feature_names, ["evt_weight_", "label_", "process_id_", "mass_"]))

train_frac = 0.5
rand_branch = "super_rand_" if args.randomize else "rand_"
features = root_numpy.tree2array(tree, branches = branches, selection = 'label_ != 2 && %s < %.6f' % (rand_branch, train_frac)) 
features_validation = root_numpy.tree2array(tree, branches = branches, selection = 'label_ != 2 && %s > %.6f' % (rand_branch, train_frac))

# organize features
global_features = []
global_features_validation = []
for feature in feature_names:
  global_features.append(features[feature])
  global_features_validation.append(features_validation[feature])
global_features = numpy.asarray(global_features)
global_features_validation = numpy.asarray(global_features_validation)
global_features = numpy.transpose(global_features)
global_features_validation = numpy.transpose(global_features_validation)

label = features["label_"]
weights = features["evt_weight_"]

label_validation = features_validation["label_"]
weights_validation = features_validation["evt_weight_"]
mass_validation = features_validation["mass_"]
processID_validation = features_validation["process_id_"]

print global_features.shape
print label.shape
print weights.shape

print global_features_validation.shape
print label_validation.shape
print weights_validation.shape

#################################################

x_train, y_train, weights_train = global_features, label, weights
x_test, y_test, weights_test  = global_features_validation, label_validation, weights_validation

X_train = pandas.DataFrame(data=x_train, columns = feature_names)
X_test = pandas.DataFrame(data=x_test, columns = feature_names)

'''
unique, count =  numpy.unique(label,return_counts=True)
weight1 = utils.sum_of_weights(weights, label, 1)
weight2 = utils.sum_of_weights(weights, label, 0)
#weight3 = utils.sum_of_weights(weights, label, 2)
#weight4 = utils.sum_of_weights(weights, label, 3)
sumWeight = weight1 + weight2# + weight3 + weight4

weights_train1 = numpy.multiply(weights_train, numpy.where(label == 1, weight2/weight1, 1) )
#weights_train1 = numpy.multiply(weights_train1, numpy.where(label == 0, sumWeight/weight2, 1) )
#weights_train1 = numpy.multiply(weights_train1, numpy.where(label == 2, sumWeight/weight3, 1) )
#weights_train1 = numpy.multiply(weights_train1, numpy.where(label == 3, sumWeight/weight4, 1) )
'''

#d_train = xgboost.DMatrix(x_train, label = y_train, weight = weights_train1)
d_test = xgboost.DMatrix(X_test, label = y_test)

#print sum_pos_weights, sum_neg_weights

# Define BDT parameters
param = { 
    	'max_depth': 8,
	'eta': 0.2,
	'objective': 'binary:logistic',
#	'objective': 'multi:softprob',#'multi:softmax',
#        'num_class': 4,
#	'scale_pos_weight': sum_neg_weights / sum_pos_weights,
	'subsample': 1.0,
	'colsample_bytree': 1.0,
	'nthread' : 8,
      	}

n_round = 100
#evallist = [(d_train, 'train'), (d_test, 'test')]
progress = {}

# train
#bdt = xgboost.train(param, d_train, n_round, evallist, evals_result = progress)	

'''
print "train1"
print "X_train.shape", X_train
print "y_train.shape", y_train
print "weights_train.shape", weights_train
print "X_test.shape", X_test
print "y_test.shape", y_test
'''

#bdt.save_model(args.channel + "_bdt.xgb")
#pickle.dump(bdt, open(args.channel + "_bdt.dat",'wb'))

# load model from file
loaded_model1 = pickle.load(open("Hadronic_ttH_ttgg_bdt.dat", "rb"))
loaded_model2 = pickle.load(open("Hadronic_ttH_ttg_bdt.dat", "rb"))
loaded_model3 = pickle.load(open("Hadronic_ttH_tt_bdt.dat", "rb"))
loaded_model4 = pickle.load(open("Hadronic_ttH_qcd_bdt.dat", "rb"))

y_pred1 = loaded_model1.predict(d_test)
y_pred2 = loaded_model2.predict(d_test)
y_pred3 = loaded_model3.predict(d_test)
y_pred4 = loaded_model4.predict(d_test)

print y_pred1
print y_pred2
print y_pred3
print y_pred4

#sys.exit()

#test = bdt.predict(d_test)#, output_margin=True)


mvaScore = y_pred1 + y_pred2 + y_pred3 + y_pred4
mvaScore1 = y_pred1 #test[:,0] #/test[:,4]
mvaScore2 = y_pred2 #test[:,1]#/test[:,4]
mvaScore3 = y_pred3 #test[:,2]#/test[:,4]
mvaScore4 = y_pred4 # test[:,3]#/test[:,4]

#print test
#sys.exit()

mass2 = numpy.array([mass_validation], dtype = [('mass', numpy.float64)])
weight2 = numpy.array([weights_validation], dtype = [('weight', numpy.float64)])
label2 = numpy.array([label_validation], dtype = [('label', numpy.float64)])
mva1 = numpy.array([mvaScore], dtype = [('mva_score1', numpy.float64)])
mva2 = numpy.array([mvaScore2], dtype = [('mva_score2', numpy.float64)])
mva3 = numpy.array([mvaScore3], dtype = [('mva_score3', numpy.float64)])
mva4 = numpy.array([mvaScore4], dtype = [('mva_score4', numpy.float64)])
mva = numpy.array([mvaScore], dtype = [('mva_score', numpy.float64)])
processID = numpy.array([processID_validation], dtype = [('processID', numpy.int32)])

hfile = ROOT.TFile("Event2_4bdt.root","RECREATE");

tree_test = root_numpy.array2tree(numpy.transpose(mass2))

root_numpy.array2tree(numpy.transpose(weight2), tree=tree_test)
root_numpy.array2tree(numpy.transpose(label2), tree=tree_test)
root_numpy.array2tree(numpy.transpose(mva1), tree=tree_test)
root_numpy.array2tree(numpy.transpose(mva2), tree=tree_test)
root_numpy.array2tree(numpy.transpose(mva3), tree=tree_test)
root_numpy.array2tree(numpy.transpose(mva4), tree=tree_test)
root_numpy.array2tree(numpy.transpose(mva), tree=tree_test)
root_numpy.array2tree(numpy.transpose(processID), tree=tree_test)

tree_test.Print()

hfile.Write()
f.Close()

#############################################

signal_mva_scores = root_numpy.tree2array(tree_test, branches = ["mva_score1"], selection = "label == 1")
n_quantiles = 100
quantiles, mva_cut = utils.quantiles_to_mva_score(n_quantiles, signal_mva_scores)

print "Significance estimate sfor our BDT: s, b_mc, b_data, z_mc, z_data"
nGrid = 100
nGrid2 = 10
#step1 = (maxmva - minmva)/float(nGrid)
for i in range(nGrid):
#  for j in range(nGrid2):
#  selection_base = "mva_score_ >= %.10f" % mva_cut[i][0]
#    selection_base = "mva_score >= %.10f && mva_score4 < %.10f" % ( mva_cut[i][0], 0.00001  + (j+1)*0.0001)
#    print mva_cut[i][0]
    selection_base = "mva_score1 > %.10f" % (mva_cut[i][0])
    utils.calc_significance(tree_test, selection_base, i, "Hadronic")
