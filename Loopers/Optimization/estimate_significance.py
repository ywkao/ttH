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

name = "Hadronic" if "Hadronic" in args.file else "Leptonic"

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


### Startin here ###
f = ROOT.TFile(args.file)
tree = f.Get("t")

signal_mva_scores = root_numpy.tree2array(tree, branches = ["mva_score_"], selection = 'label_ == 1')
signal_ref_mva_scores = root_numpy.tree2array(tree, branches = ["reference_mva_"], selection = 'label_ == 1')

n_quantiles = 100
quantiles, mva_cut = quantiles_to_mva_score(n_quantiles, signal_mva_scores)
quantiles_ref, mva_cut_ref = quantiles_to_mva_score(n_quantiles, signal_ref_mva_scores)

quants_mc = []
quants_data = []
quants_mc_ref = []
quants_data_ref = []
sig_mc = []
sig_data = []
sig_mc_ref = []
sig_data_ref = []

# for each signal efficiency between 0-100%, calculate Z_A
selection_signal = "label_ == 1"
selection_bkg = "label_ == 0"
selection_data = "label_ == 2 && mass_ >= 100 && mass_ <= 180"
selection_sideband = "mass_ >= 100 && mass_ <= 180"

# helper function to calculate Z_A
def calc_significance(selection_base, quants_mc, sig_mc, quants_data, sig_data, name):
  sig_mass = root_numpy.tree2array(tree, branches = "mass_", selection = selection_signal + " && " + selection_base)
  sig_weights = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_signal + " && " + selection_base)

  # calculate effective width
  mean_eff, sigma_eff = utils.calc_sigma_eff(sig_mass, sig_weights, i, name)
  selection_mass = "mass_ >= %.6f && mass_ <= %.6f" % (mean_eff - (1.645 * sigma_eff), mean_eff + (1.645 * sigma_eff))

  # calculate s
  sig_events = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_signal + " && " + selection_base + " && " + selection_mass)
  s = numpy.sum(sig_events)

  # calculate b from mc
  selection_bkg_sidebands = selection_bkg + " && " + selection_base + " && " + selection_sideband
  bkg_weights = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_bkg_sidebands) 
  bkg_events = root_numpy.tree2array(tree, branches = "mass_", selection = selection_bkg_sidebands) 

  try:
    b_mc = utils.fit_exp(bkg_events, bkg_weights, mean_eff, sigma_eff, i, name)
  except:
    print ("error in fit, continuing")
    return

  z_mc = Z_A(s, b_mc)
  quants_mc.append(quantiles[i])
  sig_mc.append(z_mc)

  # calculate b from fit to data sidebands
  data_events = root_numpy.tree2array(tree, branches = "mass_", selection = selection_data + " && " + selection_base)
  if len(data_events) < 4:
    return # fit doesn't seem to work with less than 5 events
  b_data = utils.fit_exp(data_events, numpy.ones(len(data_events)), mean_eff, sigma_eff, i, name)

  z_mc = Z_A(s, b_mc)
  z_data = Z_A(s, b_data)

  print s, b_mc, b_data, z_mc, z_data

  quants_data.append(quantiles[i])
  sig_data.append(z_data)


### Now calculate significances ###
# Calculate for reference BDT
for i in range(len(quantiles_ref)):
  selection_base = "reference_mva_ >= %.6f && pass_ref_presel_ == 1" % mva_cut_ref[i][0]
  calc_significance(selection_base, quants_mc_ref, sig_mc_ref, quants_data_ref, sig_data_ref, name + "_ref") 

# Calculate for our BDT
for i in range(len(quantiles)):
  selection_base = "mva_score_ >= %.6f" % mva_cut[i][0]
  calc_significance(selection_base, quants_mc, sig_mc, quants_data, sig_data, name)

  
### Make diagnostic plots ###
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

fig = plt.figure()
plt.plot(quants_mc, sig_mc, label='Estimate bkg from MC', color = 'blue')
plt.plot(quants_data, sig_data, label='Estimate bkg from data', color = 'green')
plt.plot(quants_mc_ref, sig_mc_ref, label='2016 ttH BDT (MC)', color = 'blue', linestyle = '--')
plt.plot(quants_data_ref, sig_data_ref, label='2016 ttH BDT (data)', color = 'green', linestyle = '--')
plt.xlabel('Signal Efficiency')
plt.ylabel('Significance (Z_A)')
plt.ylim([0.0, 3.0])
plt.legend(loc='upper right')
plt.savefig('optimization.pdf')
