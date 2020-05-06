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
from mva_helper import *

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--channel", help = "e.g. Hadronic or Leptonic", type=str)
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--tag", help = "tag to identify", type=str)
parser.add_argument("--mvas", help = "csv list of mva weight files", type=str)
parser.add_argument("--names", help = "name for each mva", type=str)
args = parser.parse_args()

print "[make_optimization_tree.py]: args.input               : %s" % args.input
print "[make_optimization_tree.py]: args.channel             : %s" % args.channel
print "[make_optimization_tree.py]: args.tag                 : %s" % args.tag
print "[make_optimization_tree.py]: args.mvas                : %s" % args.mvas
print "[make_optimization_tree.py]: args.names               : %s" % args.names

mva_files = args.mvas.split(",")
names = args.names.split(",")

mvas = {}

manager = MVA_Manager(
        input = args.input,
        output = args.channel + "_" + args.tag,
)

for file, name in zip(mva_files, names):
    bdt = BDT_Helper(
            input = args.input,
            output = args.channel + "_" + args.tag + "_" + name,
    )
    bdt.load_weights(file)
    bdt.predict()
    bdt.evaluate_performance()
    manager.add_mva_helper(bdt, name)

manager.write_tree()
