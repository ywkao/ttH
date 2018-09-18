import ROOT
import numpy
import math
from scipy.optimize import curve_fit
from scipy.integrate import quad


import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def all_ones(array):
  for item in array:
    if item != 1:
      return False
  return True

def exp(x, a, b):
  return a * numpy.exp(-b*x) 

def gaus(x, a, b, c):
  return a * numpy.exp(-0.5 * (( (x - b) / c ) ** 2))

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
