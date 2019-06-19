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

import dnn_helper
import significance_utils
import utils
import ks_test

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--tag", help = "save name for weights file", type=str)
parser.add_argument("--channel", help = "Hadronic or Leptonic", type=str, default="Hadronic")
parser.add_argument("--no_global", help = "don't use global features", action="store_true")
parser.add_argument("--no_lstm", help = "don't use object features (lstm)", action="store_true")
parser.add_argument("--load", help = "give weights file to use as starting point", type=str)
parser.add_argument("--no_bootstrap", help = "don't use bootstrapping to estimate unc. in AUC (to save time during hyperparameter opt)", action="store_true")
args = parser.parse_args()

import train_dnn_core

train_dnn_core.train(args, {})
