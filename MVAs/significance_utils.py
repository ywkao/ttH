import numpy
import ROOT, root_numpy
import math
from scipy.optimize import curve_fit

def Z_A(s, b):
  if s < 0:
    s = 0
  if b < 0:
    b = 0
  return math.sqrt(2 * ( ((s+b) * math.log(1 + (s/b) )) - s) )

def quantiles_to_mva_score(n_quantiles, mva_scores): # return mva_scores corresponding to each quantile in n_quantiles
  sorted_mva = numpy.flip(numpy.sort(mva_scores), 0)
  quantiles = []
  mva = []
  for i in range(n_quantiles):
    idx = int((float(i+1) / float(n_quantiles)) * len(sorted_mva)) - 1
    quantiles.append(float(i+1) / float(n_quantiles))
    mva.append(sorted_mva[idx])
  return quantiles, mva

def all_ones(array):
  for item in array:
    if item != 1:
      return False
  return True

def gaus(x, a, b, c):
  return a * numpy.exp(-0.5 * (( (x - b) / c ) ** 2))

def constant_estimate(weights, sigma_eff):
  return numpy.sum(weights) * ((2 * 1.645 * sigma_eff) / (180. - 100.))

def calc_sigma_eff(signal_data, weights): 
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

  # fit
  popt, pcov = curve_fit(gaus, x_fit, y_fit, sigma = y_err, p0 = [1, 125, 2])

  del h
  return popt[1], abs(popt[2])

def events_passing_cut(events, cut):
  events_pass = {"mass" : [], "weights" : [], "mva_score" : []}
  for i in range(len(events["mass"])):
    if events["mva_score"][i] > cut:
      events_pass["mass"].append(events["mass"][i])
      events_pass["weights"].append(events["weights"][i])
      events_pass["mva_score"].append(events["mva_score"][i])
  return events_pass

def za_scores(n_quantiles, signal_events, background_events):
  testing_frac = 0.5 # this assumes that we always use half the mc for testing and half for training
  za = []
  # Calculate cuts corresponding to each quantile
  quantiles, mva_cut = quantiles_to_mva_score(n_quantiles, signal_events["mva_score"])

  # For each cut:
  for cut_value in mva_cut:
    # Get events passing cut
    signal_events_passing_cut = events_passing_cut(signal_events, cut_value)
    bkg_events_passing_cut = events_passing_cut(background_events, cut_value)

    # Calculate sigma eff
    mean_eff, sigma_eff = calc_sigma_eff(signal_events_passing_cut["mass"], signal_events_passing_cut["weights"])
    
    # Calculate s
    signal_events_mass_window = []
    for i in range(len(signal_events_passing_cut["mass"])):
      if signal_events_passing_cut["mass"][i] > mean_eff - (1.645 * sigma_eff) and signal_events_passing_cut["mass"][i] < mean_eff + (1.645 * sigma_eff):
        signal_events_mass_window.append(signal_events_passing_cut["weights"][i])
    signal_events_mass_window = numpy.asarray(signal_events_mass_window) 
    s = ( 1. / testing_frac) * numpy.sum(signal_events_mass_window)

    # Calculate b
    bkg_events_mass_window = []
    for i in range(len(bkg_events_passing_cut["mass"])):
      if bkg_events_passing_cut["mass"][i] > 100 and bkg_events_passing_cut["mass"][i] < 180:
        bkg_events_mass_window.append(bkg_events_passing_cut["weights"][i])
    bkg_events_mass_window = numpy.asarray(bkg_events_mass_window)
    b = ( 1. / testing_frac) * constant_estimate(bkg_events_mass_window, sigma_eff)

    # Calculate Z_A
    z_mc = Z_A(s, b)
    print s, b, z_mc
    za.append(z_mc)

  return za
