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

#print(Z_A(1.95*0.9, 1.1))
#print(Z_A(4.74*0.9, 11.69))
#print(Z_A(4.09*0.9, 10.11))

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
for i in range(len(quantiles)):
  selection_signal = "label_ == 1 && mass_ >= 122 && mass_ <= 128 && mva_score_ >= %.6f" % mva_cut[i][0]
  sig_events = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_signal )
  selection_bkg = "label_ == 0 && mass_ >= 122 && mass_ <= 128 && mva_score_ >= %.6f" % mva_cut[i][0]
  bkg_events = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_bkg )
  selection_data = "label_ == 2 && mass_ >= 100 && mass_ <= 180 && mva_score_ >= %.6f" % mva_cut[i][0]
  data_events = root_numpy.tree2array(tree, branches = "mass_", selection = selection_data)


  if (len(sig_events) == 0):
    continue

  s = numpy.sum(sig_events)
  if s < 1:
    continue
  b_mc = numpy.sum(bkg_events)
  b_data = utils.fit_exp(data_events, i)
  z_mc = Z_A(s*0.9, b_mc) # multiply signal yield by 90%, per email from Andrea
  z_data = Z_A(s*0.9, b_data)
  print s, b_mc, b_data, z_mc, z_data
  if z_mc > 0 and z_mc < 100 and z_data > 0 and z_data < 100:
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
plt.ylim([0.0, 2.0])
plt.legend(loc='upper right')
plt.savefig('optimization.pdf')
