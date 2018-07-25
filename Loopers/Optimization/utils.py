import ROOT
import numpy
import math
from scipy.optimize import curve_fit

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def exp(x, a, b):
  return a * numpy.exp(-b*x) 

def fit_exp(data, idx):
  h = ROOT.TH1D("h_data", "", 80, 100, 180)
  for data_point in data:
    h.Fill(data_point)

  x_fit = numpy.append(numpy.linspace(100, 120, 20), numpy.linspace(130, 180, 50))
  y_fit = []
  for i in range(h.GetNbinsX()):
    if h.GetBinCenter(i) > 120 and h.GetBinCenter(i) < 130:
      continue
    y_fit.append(h.GetBinContent(i+1))
  popt, pcov = curve_fit(exp, x_fit, y_fit, p0 = [10, 0.01])

  mass_low = 122
  mass_high = 128
  bkg_pred = 0
  for mass in range(mass_low, mass_high):
    bkg_pred += exp(mass, *popt)

  fig = plt.figure()
  x = numpy.linspace(100, 180, 80)
  plt.plot(x, exp(x, *popt), 'r-', label='fit')
  plt.errorbar(x_fit, y_fit, yerr = numpy.sqrt(y_fit), label = 'data', fmt='o')  
  plt.legend(loc='upper right')
  plt.savefig("bkg_fits/bkg_fit_%d.pdf" % idx)
  plt.close()

  del h
  return bkg_pred
