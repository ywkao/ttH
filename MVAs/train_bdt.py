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
parser.add_argument("--config", help = ".json with bdt hyperparameters", type=str)
parser.add_argument("--tag", help = "tag to identify this set of weights", type=str)
args = parser.parse_args()

print "[train_bdt.py]: args.input               : %s" % args.input
print "[train_bdt.py]: args.channel             : %s" % args.channel
print "[train_bdt.py]: args.tag                 : %s" % args.tag

bdt = BDT_Helper(
        input = args.input,
        output = args.channel + "_" + args.tag
)

bdt.train()
bdt.predict()
bdt.evaluate_performance()
bdt.save_model()
#bdt.write_tree()
