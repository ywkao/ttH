import os, sys
import ROOT, root_numpy
import numpy
import math

from sklearn import metrics

import utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("file", help = "input root file", type=str)
parser.add_argument("-r", "--randomize", help = "use a random test/train split", action="store_true")
parser.add_argument("-i", "--invert", help = "invert the test/train split", action="store_true")
#parser.add_argument("file2", help = "second input root file (for comparing two different bdts)", type=str, required=False)
#parser.add_argument("file3", help = "third input root file (for comparing three different bdts)", type=str, required=False)
args = parser.parse_args()

name = "Hadronic" if "Hadronic" in args.file else "Leptonic"

def Z_A(s, b):
  if s < 0:
    s = 0
  if b < 0:
    b = 0
  return math.sqrt(2 * ( ((s+b) * math.log(1 + (s/b) )) - s) ) 

def unc_ZA(s, b, ds, db):
  Zs = math.log(1 + (s/b)) / Z_A(s,b)
  Zb = (math.log(1 + (s/b)) - (s/b)) / Z_A(s,b)
  return ((Zs*ds)**2 + (Zb*db)**2)**(0.5)

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

train_frac = 0.5
rand_branch = "super_rand_" if args.randomize else "rand_"
if args.invert:
  selection_signal = "label_ == 1 && %s <= %.10f" % (rand_branch, train_frac)
  selection_bkg = "label_ == 0 && %s <= %.10f" % (rand_branch, train_frac)
else:
  selection_signal = "label_ == 1 && %s > %.10f" % (rand_branch, train_frac)
  selection_bkg = "label_ == 0 && %s > %.10f" % (rand_branch, train_frac)
selection_data = "label_ == 2 && mass_ >= 100 && mass_ <= 180"
selection_sideband = "mass_ >= 100 && mass_ <= 180"


signal_mva_scores = root_numpy.tree2array(tree, branches = ["mva_score_"], selection = selection_signal)
signal_ref_mva_scores = root_numpy.tree2array(tree, branches = ["reference_mva_"], selection = selection_signal)

n_quantiles = 100
quantiles, mva_cut = quantiles_to_mva_score(n_quantiles, signal_mva_scores)
quantiles_ref, mva_cut_ref = quantiles_to_mva_score(n_quantiles, signal_ref_mva_scores)

quants_mc = []
quants_data = []
quants_mc_ref = []
quants_data_ref = []

n_sig_mc = []
n_sig_data = []
n_sig_mc_ref = []
n_sig_data_ref = []

n_bkg_mc = []
n_bkg_data = []
n_bkg_mc_ref = []
n_bkg_data_ref = []

sig_mc = []
sig_data = []
sig_unc_mc = []
sig_unc_data = []
sig_mc_ref = []
sig_data_ref = []
sig_unc_mc_ref = []
sig_unc_data_ref = []


# for each signal efficiency between 0-100%, calculate Z_A

do_simple_estimate = True # estimate background as a constant rather than an expontential fit

# helper function to calculate Z_A
def calc_significance(selection_base, quants_mc, n_sig_mc, n_bkg_mc, sig_mc, sig_unc_mc, quants_data, n_sig_data, n_bkg_data, sig_data, sig_unc_data, name):
  sig_mass = root_numpy.tree2array(tree, branches = "mass_", selection = selection_signal + " && " + selection_base)
  sig_weights = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_signal + " && " + selection_base)

  # calculate effective width
  mean_eff, sigma_eff = utils.calc_sigma_eff(sig_mass, sig_weights, i, name)
  if sigma_eff < 0:
    sigma_eff = abs(sigma_eff)
  selection_mass = "mass_ >= %.10f && mass_ <= %.10f" % (mean_eff - (1.645 * sigma_eff), mean_eff + (1.645 * sigma_eff))

  # calculate s
  sig_events = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_signal + " && " + selection_base + " && " + selection_mass)
  s = (1 / (1 - train_frac)) * numpy.sum(sig_events)
  s_unc = (1 / (1 - train_frac)) * math.sqrt(numpy.sum(sig_events**2))

  # calculate b from mc
  selection_bkg_sidebands = selection_bkg + " && " + selection_base + " && " + selection_sideband
  #selection_bkg_sidebands = selection_bkg + " && " + selection_base + " && " + selection_mass
  bkg_weights = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_bkg_sidebands) 
  bkg_events = root_numpy.tree2array(tree, branches = "mass_", selection = selection_bkg_sidebands) 

  if do_simple_estimate:
    b_mc, unc_b_mc = utils.constant_estimate(bkg_events, bkg_weights, mean_eff, sigma_eff, 0)
    b_mc *= (1 / (1 - train_frac)) # account for train/test split
    unc_b_mc *= (1 / (1 - train_frac)) # account for train/test split

  else:
    try:
      b_mc = (1 / (1 - train_frac)) * utils.fit_exp(bkg_events, bkg_weights, mean_eff, sigma_eff, i, name)
    except:
      print ("error in fit, continuing")
      return

  z_mc = Z_A(s, b_mc)
  unc_z_mc = unc_ZA(s, b_mc, s_unc, unc_b_mc)
  quants_mc.append(quantiles[i])
  n_sig_mc.append(s)
  sig_mc.append(z_mc)
  sig_unc_mc.append(unc_z_mc)
  n_bkg_mc.append(b_mc)

  # calculate b from fit to data sidebands
  data_events = root_numpy.tree2array(tree, branches = "mass_", selection = selection_data + " && " + selection_base)
  if len(data_events) < 4:
    #b_data = -1 
    return # fit doesn't seem to work with less than 5 events

  elif do_simple_estimate:
    b_data, unc_b_data = utils.constant_estimate(data_events, numpy.ones(len(data_events)), mean_eff, sigma_eff, 0)

  else:
    try: 
      b_data = utils.fit_exp(data_events, numpy.ones(len(data_events)), mean_eff, sigma_eff, i, name)
    except:
      print ("error in fit, continuing")
      return

  z_data = Z_A(s, b_data)
  unc_z_data = unc_ZA(s, b_data, s_unc, unc_b_data)

  print i, s, s_unc, b_mc, unc_b_mc, b_data, unc_b_data
  print i, z_mc, unc_z_mc, z_data, unc_z_data
  print (unc_z_mc * 100) / z_mc, (unc_z_data * 100) / z_data
  #print i, s, b_mc, b_data, z_mc, z_data

  quants_data.append(quantiles[i])
  n_sig_data.append(s)
  sig_data.append(z_data)
  sig_unc_data.append(unc_z_data)
  n_bkg_data.append(b_data)


### Now calculate significances ###
# Calculate for our BDT
print "Significance estimates for our BDT: s, b_mc, b_data, z_mc, z_data"
for i in range(len(quantiles)):
  selection_base = "mva_score_ >= %.10f" % mva_cut[i][0]
  calc_significance(selection_base, quants_mc, n_sig_mc, n_bkg_mc, sig_mc, sig_unc_mc, quants_data, n_sig_data, n_bkg_data, sig_data, sig_unc_data, name)

do_reference_bdt = True

if do_reference_bdt:
  print "Significance estimates for reference BDT: s, b_mc, b_data, z_mc, z_data"
  # Calculate for reference BDT
  for i in range(len(quantiles_ref)):
    selection_base = "reference_mva_ >= %.10f && pass_ref_presel_ == 1" % mva_cut_ref[i][0]
    calc_significance(selection_base, quants_mc_ref, n_sig_mc_ref, n_bkg_mc_ref, sig_mc_ref, sig_unc_mc_ref, quants_data_ref, n_sig_data_ref, n_bkg_data_ref, sig_data_ref, sig_unc_data_ref, name + "_ref")

numpy.savez('ZA_curves/%s' % args.file.replace(".root", ""), za_mc = sig_mc, za_data = sig_data, n_sig_mc = n_sig_mc, n_sig_data = n_sig_data, n_bkg_mc = n_bkg_mc, n_bkg_data = n_bkg_data, za_unc_mc = sig_unc_mc, za_unc_data = sig_unc_data)
  
### Make diagnostic plots ###
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.plot(n_sig_mc, sig_mc, label='MC', color = 'blue')
ax1.fill_between(n_sig_mc, numpy.asarray(sig_mc) - numpy.asarray(sig_unc_mc), numpy.asarray(sig_mc) + numpy.asarray(sig_unc_mc), color = 'blue', alpha = 0.25)
ax1.plot(n_sig_data, sig_data, label='Data', color = 'black')
ax1.fill_between(n_sig_data, numpy.asarray(sig_data) - numpy.asarray(sig_unc_data), numpy.asarray(sig_data) + numpy.asarray(sig_unc_data), color = 'black', alpha = 0.25)
if do_reference_bdt:
  ax1.plot(n_sig_mc_ref, sig_mc_ref, label='2017 ttH BDT (MC)', color = 'blue', linestyle = '--', dashes = (5,2))
  ax1.plot(n_sig_data_ref, sig_data_ref, label='2017 ttH BDT (data)', color = 'black', linestyle = '--', dashes = (5,2))
plt.xlabel('# Signal Events')
ax1.set_ylabel('Significance (Z_A)')
ax1.tick_params('y', colors = 'green')
plt.ylim([0.0, 3.0])
l, r = plt.xlim()
plt.xlim([1.0, r])

#if not do_reference_bdt:
#  ax2 = ax1.twinx()
#  ax2.plot(n_sig_mc, n_bkg_mc, color = 'red', linestyle = '--', dashes = (5,2))
#  ax2.plot(n_sig_data, n_bkg_data, color = 'red')
#  ax2.set_ylabel('# Bkg Events')
#  ax2.tick_params('y', colors = 'red')
#  ax2.set_yscale("log", nonposy='clip')
#  ax2.set_ylim([10**(-2), 10**3])

ax1.legend(loc='upper right')
plt.savefig('optimization_%s_.pdf' % args.file.replace(".root", ""))
