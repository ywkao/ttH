import os, sys
import ROOT, root_numpy
import numpy
import math

from sklearn import metrics

import utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("file", help = "input root file", type=str)
args = parser.parse_args()

def Z_A(s, b):
  if s < 0:
    s = 0
  if b < 0:
    b = 0
  return math.sqrt(2 * ( ((s+b) * math.log(1 + (s/b) )) - s) ) 

def find_nearest(array,value):
    val = numpy.ones_like(array)*value
    idx = (numpy.abs(array-val)).argmin()
    return array[idx], idx

def quantiles_to_mva_score(n_quantiles, mva_scores): # return mva_scores corresponding to each quantile in n_quantiles
  sorted_mva = numpy.flip(numpy.sort(mva_scores), 0)
  quantiles = []
  mva = []
  for i in range(n_quantiles):
    idx = int((float(i+1) / float(n_quantiles)) * len(sorted_mva)) - 1
    quantiles.append(float(i+1) / float(n_quantiles))
    mva.append(sorted_mva[idx])
  return quantiles, mva


f = ROOT.TFile(args.file)
tree = f.Get("t")

signal_mva_scores = root_numpy.tree2array(tree, branches = ["mva_score_"], selection = 'label_ == 1')

n_quantiles = 100
quantiles, mva_cut = quantiles_to_mva_score(n_quantiles, signal_mva_scores)

quants = []
sig_mc = []
sig_data = []

# for each signal efficiency between 0-100%, calculate Z_A
selection_signal = "label_ == 1"
selection_bkg = "label_ == 0"
selection_data = "label_ == 2"

for i in range(len(quantiles)):
  selection_base = "mva_score_ >= %.6f" % mva_cut[i][0]

  sig_mass = root_numpy.tree2array(tree, branches = "mass_", selection = selection_signal + " && " + selection_base) 
  sig_weights = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_signal + " && " + selection_base) 

  # calculate effective width
  mean_eff, sigma_eff = utils.calc_sigma_eff(sig_mass, sig_weights, i)
  selection_mass = "mass_ >= %.6f && mass_ <= %.6f" % (mean_eff - (1.675 * sigma_eff), mean_eff + (1.675 * sigma_eff))

  # calculate s
  sig_events = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_signal + " && " + selection_base + " && " + selection_mass)
  s = numpy.sum(sig_events)

  # calculate b from mc
  bkg_events = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_bkg + " && " + selection_base + " && " + selection_mass)
  b_mc = numpy.sum(bkg_events)

  # calculate b from fit to data sidebands
  data_events = root_numpy.tree2array(tree, branches = "mass_", selection = selection_data + " && " + selection_base)
  if len(data_events) < 5:
    continue # fit doesn't seem to work with less than 5 events
  b_data = utils.fit_exp(data_events, mean_eff, sigma_eff, i)

  z_mc = Z_A(s*0.9, b_mc)
  z_data = Z_A(s*0.9, b_data)

  print s, b_mc, b_data, z_mc, z_data

  quants.append(quantiles[i])
  sig_mc.append(z_mc)
  sig_data.append(z_data)

### Make diagnostic plots ###
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

fig = plt.figure()
plt.plot(quants, sig_mc, label='Estimate bkg from MC')
plt.plot(quants, sig_data, label='Estimate bkg from data')
plt.xlabel('Signal Efficiency')
plt.ylabel('Significance (Z_A)')
plt.ylim([0.0, 3.0])
plt.legend(loc='upper right')
plt.savefig('optimization.pdf')
