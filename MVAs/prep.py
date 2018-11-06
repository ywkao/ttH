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

from xgboost import XGBClassifier
from sklearn.multiclass import OneVsRestClassifier

import math
import argparse

import argparse
def ParseOption():

    parser = argparse.ArgumentParser(description='submit all')
    parser.add_argument('-f', dest='filename', type=str, help='filename')
    parser.add_argument('-o', dest='outname', type=str, help='output name')

    args = parser.parse_args()
    return args


def prec_xgb(n_trees, max_depth, X_train, y_train, X_test, y_test, weight_train, learning_rate=0.3):
    """
    ExtraTrees
    """
    import xgboost as xgb
    X_train = X_train.reshape((X_train.shape[0], -1))
    X_test = X_test.reshape((X_test.shape[0], -1))
    
#    LOGGER.info('start predict: n_trees={},X_train.shape={},y_train.shape={},X_test.shape={},y_test.shape={}'.format(
    print 'start predict: n_trees={},X_train.shape={},y_train.shape={},X_test.shape={},y_test.shape={}'.format(n_trees, X_train.shape, y_train.shape, X_test.shape, y_test.shape)
    clf = xgb.XGBClassifier(n_estimators=n_trees, max_depth=max_depth, objective='multi:softprob',
            seed=0, silent=True, nthread=-1, learning_rate=learning_rate)
    eval_set = [(X_test, y_test)]
    clf.fit(X_train, y_train, eval_set=eval_set, eval_metric="merror", sample_weight = weight_train)
    y_pred = clf.predict(X_test, output_margin=True)
    y_pred_prob = clf.predict_proba(X_test)

    prec = float(numpy.sum(y_pred == y_test)) / len(y_test)
#    LOGGER.info('prec_xgb_{}={:.6f}%'.format(n_trees, prec*100.0))
    print 'prec_xgb_{}={:.6f}%'.format(n_trees, prec*100.0)
    return clf, y_pred, y_pred_prob


def GetDatasets(inputfile, extraBranches, mySelection):
    train_frac = 0.5
    f = ROOT.TFile(inputfile, 'UPDATE')
    #f = ROOT.TFile("/home/users/hmei/ttH/Loopers/MVABaby_ttHHadronic_multiClass_promptness_toptag.root", 'UPDATE')
    tree = f.Get("t")

    # load tree to array
    feature_names = (root_numpy.tree2array(tree, branches = ["mva_branches"], start=0, stop=1))[0][0]
    feature_names = list(feature_names) 
#    feature_names = feature_names[1:len(feature_names)]
    print feature_names
    allBranches = numpy.concatenate( (feature_names, extraBranches) )
    features_raw = root_numpy.tree2array(tree, branches = allBranches, selection = mySelection) 
    # print features_raw
    features = []
    for feature in feature_names:
        features.append(features_raw[feature])

    #features = numpy.asarray(features)
    features = numpy.transpose(features)
    labels = root_numpy.tree2array(tree, branches = "label_", selection = mySelection)
    weights = root_numpy.tree2array(tree, branches = "evt_weight_", selection = mySelection) 

    extra_arrays = {}
    for extraBranch in extraBranches:
        type_ = features_raw[extraBranch].dtype
        extra_arrays[extraBranch] = numpy.array( features_raw[extraBranch], dtype = [(extraBranch, type_)] )

    return features, labels, weights, extra_arrays


def Train1(d_train, d_test, doBinary, n_round, sum_neg_weights = 1, sum_pos_weights = 1):
    # Define BDT parameters
    param1 = { 
    	    'max_depth': 8,
            'eta': 0.3,
            'objective': 'multi:softprob',#'multi:softmax',
            'num_class': 5,
            'subsample': 1.0,
            'colsample_bytree': 1.0,
            'nthread' : 8,
            }
    
    param2 = { 
    	    'max_depth': 4,
            'eta': 0.3,
            'objective': 'binary:logistic',
#            'scale_pos_weight': sum_neg_weights / sum_pos_weights,
            'subsample': 1,
            'colsample_bytree': 1.0,
            'nthread' : 8,
            }

    #n_round = 100
    evallist = [(d_train, 'train'), (d_test, 'test')]
    progress = {}

    #clf, y_pred, y_pred_prob = prec_xgb(100, 8, x_train, y_train, x_test, y_test, weights_train1)
    #print y_pred, y_pred_prob
    param = param1
    if doBinary:
        param = param2
    bdt = xgboost.train(param, d_train, n_round, evallist, evals_result = progress)	

    d_pred_train = bdt.predict(d_train)#, output_margin=True)
    d_pred_test = bdt.predict(d_test)#, output_margin=True)

    return d_pred_train, d_pred_test

args=ParseOption()

nround1 = 100
nround2 = 100

features_train, labels_train, weights_train, extraArrays_train = GetDatasets(args.filename, ["evt_weight_", "process_id_", "mass_"], "label_ < 5 && rand_ < 0.5")

#features_test, labels_test, weights_test, extraArrays_test = GetDatasets(args.filename, ["evt_weight_", "process_id_", "mass_"], "label_ < 5 && rand_ > 0.5")

features_test, labels_test, weights_test, extraArrays_test = GetDatasets(args.filename, ["evt_weight_", "process_id_", "mass_"], "rand_ > 0.5 && (label_ < 5 || (label_ == 5 && process_id_ == 10) ) ")
labels_test[labels_test == 5] = 0

d_train = xgboost.DMatrix(features_train, label = labels_train, weight = weights_train)
d_test = xgboost.DMatrix(features_test, label = labels_test)

# first training of a multi-class BDT
d_pred_train, d_pred_test = Train1(d_train, d_test, False, nround1)

# modify label
labels_train[labels_train > 0] = -1
labels_train[labels_train == 0] = 1
labels_train[labels_train == -1] = 0

labels_test[labels_test > 0] = -1
labels_test[labels_test == 0] = 1
labels_test[labels_test == -1] = 0

# change training weight for binary output BDT
sum_neg_weights = utils.sum_of_weights(weights_train, labels_train, 0)
sum_pos_weights = utils.sum_of_weights(weights_train, labels_train, 1)

d_train2 = xgboost.DMatrix(d_pred_train, label = labels_train, weight = weights_train)
d_test2 = xgboost.DMatrix(d_pred_test, label = labels_test)

# second training of a binary BDT
d_pred_train2, d_pred_test2 = Train1(d_train2, d_test2, True, nround2, sum_neg_weights, sum_pos_weights)

# save information into a root tree
arrays_forTree = []
for key in extraArrays_test:
    arrays_forTree.append( extraArrays_test[key] )
arrays_forTree.append( numpy.array( d_pred_test2, dtype = [("mva_score_", numpy.float64)] ) ) 
arrays_forTree.append( numpy.array( d_pred_test[:,0], dtype = [("mva_score1_", numpy.float64)] ) ) 
arrays_forTree.append( numpy.array( labels_test, dtype = [("label_", numpy.float64)] ) ) 
#arrays_forTree.append( numpy.array( extraArrays_test["process_id_"], dtype = [("process_id__", numpy.float64)] ) ) 

hfile = ROOT.TFile('rootfiles/' + args.outname + ".root","RECREATE");
tree_out = root_numpy.array2tree(numpy.transpose(arrays_forTree[0]))
for i in range(1, len(arrays_forTree)):
    root_numpy.array2tree(numpy.transpose(arrays_forTree[i]), tree=tree_out)

tree_out.Print()
hfile.Write()
hfile.Close()

