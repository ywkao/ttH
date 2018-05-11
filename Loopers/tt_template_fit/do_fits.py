import os, sys
import ROOT
import numpy

pp_template = "TTGG"
fp_template = "TTGG"
ff_template = "TTJets"

hist = "hPhotonMinIDMVA_coarse"

f = ROOT.TFile("../ttbar_cr_histograms.root")
h_data = f.Get(hist + "_Data")
h_pp = f.Get(hist + "_" + pp_template + "GenPhoton_2")
h_fp = f.Get(hist + "_" + fp_template + "GenPhoton_1")
h_ff = f.Get(hist + "_" + ff_template + "GenPhoton_0")

h_other = 0
bkgs = ["DiPhoton", "VG"]
#bkgs = ["DY", "DiPhoton", "GammaJets", "QCD", "VG", "WJets"]
for bkg in bkgs:
  hist_name = hist + "_" + bkg
  histogram = f.Get(hist_name)
  if h_other == 0:
    h_other = histogram
  else:
    h_other.Add(histogram)

mc = ROOT.TObjArray(4)
mc.Add(h_pp)
mc.Add(h_fp)
mc.Add(h_ff)
mc.Add(h_other)

n_bins = h_data.GetSize() - 2

hist_list = [h_pp, h_fp, h_ff, h_other]
initial_fracs = []

for entry in hist_list:
  initial_fracs.append(entry.Integral()/h_data.Integral())
  for i in range(n_bins):
    if entry.GetBinContent(i+1) < 0:
      entry.SetBinContent(i+1, 0)

fit = ROOT.TFractionFitter(h_data, mc, "Q")
for i in range(4):
  fit.Constrain(i, 0.0, 1.0)
  #fit.Constrain(i, initial_fracs[i]-0.25, initial_fracs[i]+0.25)

fit.Fit()

fracs = []
frac_errs = []
scales = []
for i in range(4):
  f = ROOT.Double()
  ferr = ROOT.Double()
  fit.GetResult(i, f, ferr)
  print i, f, ferr
  fracs.append(f)
  frac_errs.append(ferr)
  scales.append(f/initial_fracs[i])
  #scales.append(initial_fracs[i]/f)


print "Initial fractions", initial_fracs
print "Fitted fractions", fracs
print "Errors", frac_errs

print "Scales" , scales

del fit
