import ROOT
import numpy
import math
from scipy.optimize import curve_fit
from scipy.integrate import quad
import root_numpy


import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import numpy
import random

def unc_ZA(s, b, ds, db):
  Zs = math.log(1 + (s/b)) / Z_A(s,b)
  Zb = (math.log(1 + (s/b)) - (s/b)) / Z_A(s,b)
  return ((Zs*ds)**2 + (Zb*db)**2)**(0.5)

def calc_significance2(tree, selection_base, index, mva_cut, name):

  do_simple_estimate = True
  train_frac = 0.5

#  selection_signal = "label == 0"
#  selection_bkg = "label >= 1 && label < 4"
  selection_signal = "label_ == 1 && process_id_ == 0"
  selection_bkg = "label_ == 0 && process_id_ != 10"
  selection_sideband = "mass_ >= 100 && mass_ <= 180 && process_id_ != 10"
  selection_data = "process_id_ == 10 && mass_ >= 100 && mass_ <= 180"                                                                                                                                              

  sig_mass = root_numpy.tree2array(tree, branches = "mass_", selection = selection_signal + " && " + selection_base)
  sig_weights = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_signal + " && " + selection_base)

  # calculate effective width
  mean_eff, sigma_eff = calc_sigma_eff(sig_mass, sig_weights, index, name)
  if sigma_eff < 0:
    sigma_eff = abs(sigma_eff)
  selection_mass = "mass_ >= %.10f && mass_ <= %.10f" % (mean_eff - (1.645 * sigma_eff), mean_eff + (1.645 * sigma_eff))

  # calculate s
  sig_events = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_signal + " && " + selection_base + " && " + selection_mass)
  s = (1 / (1 - train_frac)) * numpy.sum(sig_events)
  s_unc = (1 / (1 - train_frac)) * math.sqrt(numpy.sum(sig_events**2))

#  print s,s_unc
  # calculate b from mc
  selection_bkg_sidebands = selection_bkg + " && " + selection_base + " && " + selection_sideband
  #selection_bkg_sidebands = selection_bkg + " && " + selection_base + " && " + selection_mass
  bkg_weights = root_numpy.tree2array(tree, branches = "evt_weight_", selection = selection_bkg_sidebands) 
  bkg_events = root_numpy.tree2array(tree, branches = "mass_", selection = selection_bkg_sidebands) 

  if do_simple_estimate:
    b_mc, unc_b_mc = constant_estimate2(bkg_events, bkg_weights, mean_eff, sigma_eff, 0)

#    print b_mc, unc_b_mc
    b_mc *= (1 / (1 - train_frac)) # account for train/test split
    unc_b_mc *= (1 / (1 - train_frac)) # account for train/test split

  else:
    try:
      b_mc = (1 / (1 - train_frac)) * fit_exp(bkg_events, bkg_weights, mean_eff, sigma_eff, i, name)
    except:
      print ("error in fit, continuing")
      return

  z_mc = Z_A(s, b_mc)
  unc_z_mc = unc_ZA(s, b_mc, s_unc, unc_b_mc)

  # calculate b from fit to data sidebands
  
  data_events = root_numpy.tree2array(tree, branches = "mass_", selection = selection_data + " && " + selection_base)
  #print selection_data + " && " + selection_base
  #print "data_events: ", data_events
  if len(data_events) < 4:
    #b_data = -1 
    return 0, 0, 0, 0, 0, 0, 0# fit doesn't seem to work with less than 5 events

  elif do_simple_estimate:
    b_data, unc_b_data = constant_estimate2(data_events, numpy.ones(len(data_events)), mean_eff, sigma_eff, 0)

  else:
    try: 
      b_data = utils.fit_exp(data_events, numpy.ones(len(data_events)), mean_eff, sigma_eff, i, name)
    except:
      print ("error in fit, continuing")
      return

  b_data = b_data/(1-train_frac)
  unc_b_data = unc_b_data/(1-train_frac)

  z_data = Z_A(s, b_data)
  unc_z_data = unc_ZA(s, b_data, s_unc, unc_b_data)

  #print s,b_mc,b_data
  #print s, s_unc, b_mc, unc_b_mc, b_data, unc_b_data
  #print z_mc, unc_z_mc, z_data, unc_z_data
  #print (unc_z_mc * 100) / z_mc, (unc_z_data * 100) / z_data
  
  #print index, s, z_mc, unc_z_mc
  #print  s, b_mc, z_mc, unc_z_mc, b_data, z_data, unc_z_data

  return s, b_mc, z_mc, unc_z_mc, b_data, z_data, unc_z_data



# helper function to calculate Z_A
def calc_significance(tree_test, selection_base, idx, idy, name):

  do_simple_estimate = True

#  selection_signal = "label == 0"
#  selection_bkg = "label >= 1 && label < 4"
  selection_signal = "label_ == 1"
  selection_bkg = "label_ == 0"
  selection_sideband = "mass_ >= 100 && mass_ <= 180"

  sig_mass = root_numpy.tree2array(tree_test, branches = "mass_", selection = selection_signal + " && " + selection_base)
  sig_weights = root_numpy.tree2array(tree_test, branches = "evt_weight_", selection = selection_signal + " && " + selection_base)

  # calculate effective width
  mean_eff, sigma_eff = calc_sigma_eff(sig_mass, sig_weights, idx, name)
#  print mean_eff, sigma_eff
  selection_mass = "mass_ >= %.10f && mass_ <= %.10f" % (mean_eff - (1.645 * sigma_eff), mean_eff + (1.645 * sigma_eff))

  # calculate s
  sig_events = root_numpy.tree2array(tree_test, branches = "evt_weight_", selection = selection_signal + " && " + selection_base + " && " + selection_mass)
  s =  numpy.sum(sig_events)

  # calculate b from mc
  selection_bkg_sidebands = selection_bkg + " && " + selection_base + " && " + selection_sideband
  #selection_bkg_sidebands = selection_bkg + " && " + selection_base + " && " + selection_mass
  bkg_weights = root_numpy.tree2array(tree_test, branches = "evt_weight_", selection = selection_bkg_sidebands) 
  bkg_events = root_numpy.tree2array(tree_test, branches = "mass_", selection = selection_bkg_sidebands) 

  #train_frac = 0.5
  #b_mc = (1 / (1 - train_frac)) * numpy.sum(bkg_weights)

  if do_simple_estimate:
    b_mc = constant_estimate(bkg_events, bkg_weights, mean_eff, sigma_eff)
  else:
    try:
      b_mc = fit_exp(bkg_events, bkg_weights, mean_eff, sigma_eff, idx, name)
    except:
      print ("error in fit, continuing")
      return

  s = s*2
  b_mc = b_mc*2

  z_mc = Z_A(s, b_mc)

#  quants_mc.append(quantiles[i])
#  n_sig_mc.append(s)
#  sig_mc.append(z_mc)
#  n_bkg_mc.append(b_mc)

#  print idx, idy, s, b_mc, z_mc
  print selection_base, s, b_mc, z_mc, mean_eff, sigma_eff
  return s, b_mc, z_mc

def Z_A(s, b):
  if s < 0:
    s = 0
  if b < 0:
    b = 0
#  print s,b  
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
  print quantiles, mva
  return quantiles, mva


def sum_of_weights(weights, label, label_index):
  sum = 0
  for i in range(len(weights)):
    if label[i] == label_index:
      sum += weights[i]
  return sum

def shuffle(x, y, weights):
  rand = numpy.random.permutation(len(y))
  x = numpy.asarray(x)
  x = x[rand]
  y = numpy.asarray(y)
  y = y[rand]
  weights = numpy.asarray(weights)
  weights = weights[rand]
  return x, y, weights

def all_ones(array):
  for item in array:
    if item != 1:
      return False
  return True

def exp(x, a, b):
  return a * numpy.exp(-b*x) 

def gaus(x, a, b, c):
  return a * numpy.exp(-0.5 * (( (x - b) / c ) ** 2))

def constant_estimate2(data, weights, mean_eff, sigma_eff, smear):
  if numpy.sum(weights) < 0:
    return 999, 999
  est = numpy.sum(weights) * ((2 * 1.645 * sigma_eff) / (180. - 100.))
  unc = math.sqrt(numpy.sum(weights**2)) * ((2 * 1.645 * sigma_eff) / (180. - 100.))
  return est, unc

def constant_estimate(data, weights, mean_eff, sigma_eff):
  return numpy.sum(weights) * ((2 * 1.645 * sigma_eff) / (180. - 100.))

def calc_sigma_eff(signal_data, weights, idx, name):
  h = ROOT.TH1D("h_sig", "", 80, 100, 180)
  for i in range(len(signal_data)):
    h.Fill(signal_data[i], weights[i])

  x_fit = numpy.linspace(120.5, 129.5, 10)
  y_fit = []
  y_err = []
  for i in range(h.GetNbinsX()):
    if not (h.GetBinCenter(i+1) > 120 and h.GetBinCenter(i+1) < 130):
      continue
    y_fit.append(h.GetBinContent(i+1))
    y_err.append(h.GetBinError(i+1))
    #y_err.append(h.GetBinError(i+1) if h.GetBinError(i+1) != 0 else 1)

  # fit
  #popt, pcov = curve_fit(gaus, x_fit, y_fit, p0 = [1, 125, 2])
  #print y_fit, y_err
  popt, pcov = curve_fit(gaus, x_fit, y_fit, sigma = y_err, p0 = [1, 125, 2])

  # plot
  x_plot = numpy.linspace(120, 130, 100)
  fig = plt.figure()
  plt.plot(x_plot, gaus(x_plot, *popt), 'r-', label = 'fit')
  plt.errorbar(x_fit, y_fit, yerr = y_err, label = 'ttH', fmt = 'o')
  plt.legend(loc = 'upper right')
  plt.xlabel(r'$m_{\gamma \gamma}$ [GeV]')
  plt.ylabel("Events/1 GeV")
  plt.savefig("bkg_fits/sig_fit_%s_%d.pdf" % (name, idx))
  plt.xlim([120, 130])
  plt.close()

  del h
  return popt[1], popt[2] 
   
def fit_exp(data, weights, mean_eff, sigma_eff, idx, name):
  if all_ones(weights):
    type = "data"
    bin_width = 1.
  else:
    type = "mc"
    bin_width = 10. # use wider bins for MC to combat issues arising from wildly different weights

  #if type == "data":
  #  h = ROOT.TH1D("h_data", "", 80, 100, 180)
  #elif type == "mc":
  #  h = ROOT.TH1D("h_data", "", 16, 100, 180)
  h = ROOT.TH1D("h_data", "", int((180-100)/bin_width), 100, 180)


  for i in range(len(data)):
    h.Fill(data[i], weights[i])

  #if type == "data":
  #  x_fit = numpy.append(numpy.linspace(100.5, 119.5, 20), numpy.linspace(130.5, 179.5, 50))
  #elif type == "mc":
  #  x_fit = numpy.linspace(102.5, 177.5, 16)

  if type == "data":
    x_fit = numpy.append(numpy.linspace(100 + (bin_width*0.5), 120 - (bin_width*0.5), int((120-100)/bin_width)),
			numpy.linspace(130 + (bin_width*0.5), 180 - (bin_width*0.5), int((180-130)/bin_width)))
  elif type == "mc":
    x_fit = numpy.linspace(100 + (bin_width*0.5), 180 - (bin_width*0.5), int((180-100)/bin_width))


  y_fit = []
  y_err = []
  for i in range(h.GetNbinsX()):
    if type == "data" and h.GetBinCenter(i+1) > 120 and h.GetBinCenter(i+1) < 130:
      continue
    y_fit.append(h.GetBinContent(i+1))
    y_err.append(h.GetBinError(i+1))
    #y_err.append(h.GetBinError(i+1) if h.GetBinContent(i+1) != 0 else 1)
  if type == "data":
    #popt, pcov = curve_fit(exp, x_fit, y_fit, sigma = y_err, p0 = [10, 0.01])
    popt, pcov = curve_fit(exp, x_fit, y_fit, p0 = [10, 0.01])
  else:
    popt, pcov = curve_fit(exp, x_fit, y_fit, sigma = y_err, p0 = [10, 0.01])
    #popt, pcov = curve_fit(exp, x_fit, y_fit, p0 = [10, 0.01])

  bkg_pred = (quad(exp, mean_eff - (1.645*sigma_eff), mean_eff + (1.645*sigma_eff), args = (popt[0], popt[1]))[0]) / bin_width

  fig = plt.figure()
  x = numpy.linspace(100, 180, 1000)

  #if type == "data":
  #  x = numpy.linspace(100.5, 179.5, 80)
  #elif type == "mc":
  #  x = numpy.linspace(102.5, 177.5, 16)
  plt.plot(x, exp(x, *popt), 'r-', label='fit')
  plt.errorbar(x_fit, y_fit, yerr = y_err, label = type, fmt='o')  
  plt.legend(loc='upper right')
  plt.xlim([100, 180])
  plt.xlabel(r'$m_{\gamma \gamma}$ [GeV]')
  if type == "data":
    plt.ylabel("Events/1 GeV")
  elif type == "mc":
    plt.ylabel("Events/5 GeV")
  plt.savefig("bkg_fits/bkg_fit_%s_%s_%d.pdf" % (type, name, idx))
  plt.close()

  del h
  return bkg_pred
