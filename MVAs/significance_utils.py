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

def constant_estimate(data, weights, mean_eff, sigma_eff, smear):
  if smear == 0:
    return numpy.sum(weights) * ((2 * 1.645 * sigma_eff) / (180. - 100.))
  else:
    h = ROOT.TH1D("h_sig", "", 1, 100, 180)
    for i in range(len(data)):
      h.Fill(data[i], weights[i])
    mc_yield = h.GetBinContent(1)
    mc_unc = h.GetBinError(1)
    if smear == -1:
      mc_yield += -mc_unc
    elif smear == 1:
      mc_yield += mc_unc
    return mc_yield * ((2 * 1.645 * sigma_eff) / (180. - 100.))

def calc_sigma_eff(signal_data, weights) 
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
  return popt[1], popt[2]

def events_passing_cut(events, cut):
  events_pass = {"mass" : [], "weight" : [], "mva_score" : []}
  for i in range(len(events["mass"])):
    if events["mva_score"][i] > cut:
      events_pass["mass"].append(events["mass"][i])
      events_pass["weight"].append(events["weight"][i])
      events_pass["mva_score"].append(events["mva_score"][i])
  return events_pass


def za_scores(n_quantiles, signal_events, background_events):
  za = []
  # Calculate cuts corresponding to each quantile
  quantiles, mva_cut = quantiles_to_mva_score(n_quantiles, signal_events["mva_score"])
  
  # For each cut:
  for cut_value in mva_cut:
    # Get events passing cut
    signal_events_pass = events_pass(signal_events, mva_cut)
    bkg_events_pass = events_pass(background_events, mva_cut)

    # Calculate sigma eff
    mean_eff, sigma_eff = calc_sigma_eff(signal_events_pass["mass"], signal_events_pass["weights"])
    
    # Calculate s
    signal_events_mass_window = []
    for i in range(len(signal_events_pass["mass"])):
      if signal_events_pass["mass"][i] > mean_eff - (1.645 * sigma_eff) and signal_events_pass["mass"][i] < mean_eff + (1.645 * sigma_eff):
        signal_events_mass_window.append(signal_events_pass["weight"][i])
    signal_events_mass_window = numpy.asarray(signal_events_mass_window) 
    s = 2.0 * numpy.sum(signal_events_mass_window)

    # Calculate b
    bkg_events_mass_window = []
    for i in range(len(bkg_events_pass["mass"])):
      if bkg_events_pass["mass"][i] > 100 and bkg_events_pass["mass"] < 180:
        bkg_events_mass_window.append(bkg_events_pass["weight"][i])
    bkg_events_mass_window = numpy.asarray(bkg_events_mass_window)
    b = constant_estimate(bkg_events_mass_window, sigma_eff)

    # Calculate Z_A
    z_mc = Z_A(s, b)
    za.append(z_mc)

  return za
