import ROOT
import numpy
import math
from scipy.optimize import curve_fit
from scipy.integrate import quad


import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def exp(x, a, b):
  return a * numpy.exp(-b*x) 

def gaus(x, a, b, c):
  return a * numpy.exp(-0.5 * (( (x - b) / c ) ** 2))

def calc_sigma_eff(signal_data, weights, idx):
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
  popt, pcov = curve_fit(gaus, x_fit, y_fit, p0 = [1, 125, 2])

  # plot
  x_plot = numpy.linspace(120, 130, 100)
  fig = plt.figure()
  plt.plot(x_plot, gaus(x_plot, *popt), 'r-', label = 'fit')
  plt.errorbar(x_fit, y_fit, yerr = y_err, label = 'ttH', fmt = 'o')
  plt.legend(loc = 'upper right')
  plt.savefig("bkg_fits/sig_fit_%d.pdf" % idx)
  plt.xlim([120, 130])
  plt.close()

  del h
  return popt[1], popt[2] 
   

def fit_exp(data, mean_eff, sigma_eff, idx):
  h = ROOT.TH1D("h_data", "", 80, 100, 180)
  for data_point in data:
    h.Fill(data_point)

  x_fit = numpy.append(numpy.linspace(100, 120, 20), numpy.linspace(130, 180, 50))
  y_fit = []
  for i in range(h.GetNbinsX()):
    if h.GetBinCenter(i+1) > 120 and h.GetBinCenter(i+1) < 130:
      continue
    y_fit.append(h.GetBinContent(i+1))
  popt, pcov = curve_fit(exp, x_fit, y_fit, p0 = [10, 0.01])

  bkg_pred = quad(exp, mean_eff - (1.675*sigma_eff), mean_eff + (1.675*sigma_eff), args = (popt[0], popt[1]))[0]

  fig = plt.figure()
  x = numpy.linspace(100, 180, 80)
  plt.plot(x, exp(x, *popt), 'r-', label='fit')
  plt.errorbar(x_fit, y_fit, yerr = numpy.sqrt(y_fit), label = 'data', fmt='o')  
  plt.legend(loc='upper right')
  plt.xlim([100, 180])
  plt.savefig("bkg_fits/bkg_fit_%d.pdf" % idx)
  plt.close()

  del h
  return bkg_pred
