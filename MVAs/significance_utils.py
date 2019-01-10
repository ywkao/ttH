import numpy
import ROOT, root_numpy
import math
from scipy.optimize import curve_fit
import itertools

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

def constant_estimate(weights, sigma_eff, is_data):
  sideband_width = (180. - 130.) + (120. - 100.) if is_data else (180. - 100.)
  est = numpy.sum(weights) * ((2 * 1.645 * sigma_eff) / (180. - 100.))
  unc = math.sqrt(numpy.sum(weights**2)) * ((2 * 1.645 * sigma_eff) / (180. - 100.))
  if est > 0:
    return est, unc
  else:
    return 999999, 999999

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
    pass_cut = True
    for mva_name in events["mva_score"].keys():
      if not events["mva_score"][mva_name][i] > cut[mva_name]:
        pass_cut = False
    if pass_cut:
      events_pass["mass"].append(events["mass"][i])
      events_pass["weights"].append(events["weights"][i])
      events_pass["mva_score"].append(events["mva_score"]["bdt_score"][i])
  return events_pass

def scan_za(cut_combos, signal_events, background_events, is_data):
  testing_frac = 0.5 # this assumes that we always use half the mc for testing and half for training

  za_dict = []
  za = []
  za_unc = []
  n_sig = []
  n_bkg = []
  sigma_eff_ = []

  for cut_tuple in cut_combos:
    cut_list = dict((x,y) for x,y in cut_tuple)
    print cut_list
    # Get events passing cut
    signal_events_passing_cut = events_passing_cut(signal_events, cut_list)
    bkg_events_passing_cut = events_passing_cut(background_events, cut_list)

    # Calculate sigma eff
    mean_eff, sigma_eff = calc_sigma_eff(signal_events_passing_cut["mass"], signal_events_passing_cut["weights"])

    # Calculate s
    signal_events_mass_window = []
    for i in range(len(signal_events_passing_cut["mass"])):
      if signal_events_passing_cut["mass"][i] > mean_eff - (1.645 * sigma_eff) and signal_events_passing_cut["mass"][i] < mean_eff + (1.645 * sigma_eff):
        signal_events_mass_window.append(signal_events_passing_cut["weights"][i])
    signal_events_mass_window = numpy.asarray(signal_events_mass_window)
    s = ( 1. / testing_frac) * numpy.sum(signal_events_mass_window)
    s_unc = ( 1. / testing_frac) * math.sqrt(numpy.sum(signal_events_mass_window**2))

    # Calculate b
    bkg_events_mass_window = []
    for i in range(len(bkg_events_passing_cut["mass"])):
      if bkg_events_passing_cut["mass"][i] > 100 and bkg_events_passing_cut["mass"][i] < 180:
        bkg_events_mass_window.append(bkg_events_passing_cut["weights"][i])
    bkg_events_mass_window = numpy.asarray(bkg_events_mass_window)
    #b = ( 1. / testing_frac) * constant_estimate(bkg_events_mass_window, sigma_eff)
    b, b_unc = constant_estimate(bkg_events_mass_window, sigma_eff, is_data)
    if not is_data:
      b *= ( 1. / testing_frac)
      b_unc *= ( 1. / testing_frac)

    # Calculate Z_A
    z_mc = Z_A(s, b)
    z_mc_unc = unc_ZA(s, b, s_unc, b_unc)
    print "s: %.3f +/- %.3f, b: %.3f +/- %.3f, mean_eff: %.3f, sigma_eff: %.3f" % (s, s_unc, b, b_unc, mean_eff, sigma_eff)
    print "za: %.3f +/- %.3f" % (z_mc, z_mc_unc)
    za_dict.append({"z_mc": z_mc, "cut_list" : cut_list}) 
    za.append(z_mc)
    za_unc.append(z_mc_unc)
    n_sig.append(s)
    n_bkg.append(b)
    sigma_eff_.append(sigma_eff)

  return {"za_dict" : za_dict, "za" : za, "za_unc": za_unc, "n_sig" : n_sig, "n_bkg" : n_bkg, "sigma_eff" : sigma_eff_}


def za_scores(n_quantiles, signal_events, background_events, is_data):
  # Calculate cuts corresponding to each quantile
  mva_cuts = {}
  for mva_name in signal_events["mva_score"].keys():
    quantiles, mva_cut = quantiles_to_mva_score(n_quantiles, signal_events["mva_score"][mva_name])
    mva_cut_tuples = []
    for cut in mva_cut:
      mva_cut_tuples.append((mva_name, cut))
    mva_cuts[mva_name] = { "cuts" : mva_cut, "quantiles" : quantiles , "cut_tuples" : mva_cut_tuples}

  # Make list of all cut combinations (N-d)
  mva_cut_list = []
  for mva_name in mva_cuts.keys():
    mva_cut_list.append(mva_cuts[mva_name]["cut_tuples"])
  cut_combos = list(itertools.product(*mva_cut_list)) # all unique combinations of cuts from each of the N MVAs
  print len(cut_combos)

  # Perform full N-d scan
  full_za_dict = scan_za(cut_combos, signal_events, background_events, is_data) # Full N-d scan

  if len(mva_cuts.keys()) > 1:
    # Now fix all other MVA scores except BDT at the value for which we get max Z_A, and perform 1-d scan on BDT score
    max_idx = numpy.argmax(full_za_dict["za"])
    mva_values_at_max = full_za_dict["za_dict"][max_idx]["cut_list"]
    
    mva_cut_list_1d = [mva_cuts["bdt_score"]["cut_tuples"]]
    for mva_name in mva_cuts.keys():
      if mva_name == "bdt_score":
        continue
      else:
        print "Fixing 1d optimization at %s : %.4f" % (mva_name, mva_values_at_max[mva_name])
        mva_cut_list_1d.append([(mva_name, mva_values_at_max[mva_name])])
    cut_combos_1d = list(itertools.product(*mva_cut_list_1d))
    za_dict_1d = scan_za(cut_combos_1d, signal_events, background_events, is_data)

  return za_dict_1d["za"], za_dict_1d["za_unc"], za_dict_1d["n_sig"], za_dict_1d["n_bkg"], za_dict_1d["sigma_eff"]
