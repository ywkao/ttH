import os, sys
import ROOT
import numpy

def combine_hists(hist1, hist2, name):
  target_hist = ROOT.TH1D(name, "", hist1.GetNbinsX() + hist2.GetNbinsX(), 0, 1)
  for i in range(hist1.GetNbinsX()):
    target_hist.SetBinContent(i+1, hist1.GetBinContent(i+1))
  for i in range(hist2.GetNbinsX()):
    target_hist.SetBinContent(hist1.GetNbinsX() + i + 1, hist2.GetBinContent(i+1))
  for i in range(hist1.GetNbinsX() + hist2.GetNbinsX()):
    if target_hist.GetBinContent(i+1) < 0:
      target_hist.SetBinContent(i+1, 0)

  return target_hist


latex_dict = { "DiPhoton" : "$ \\gamma \\gamma $ + jets", "GammaJets" : "$ \\gamma $ + jets", "QCD" : "QCD"}

pp_template = "DiPhoton"
fp_template = "GammaJets"
ff_template = "QCD"

hist_names = ["hPhotonMinIDMVA", "hPhotonMaxIDMVA"]
templates_dict = { "data" : [["Data"], ""], 
		"pp" : [["DiPhoton"], "GenPhoton_2"], 
		"fp" : [["GammaJets"], "GenPhoton_1"], 
		"ff" : [["QCD"], "GenPhoton_0"], 
		"bkg" : [["TTGG", "DY", "TTGJets", "VG", "TTJets"], ""] 
}

hists = {}

f = ROOT.TFile("../ttHHadronicLoose_histograms.root")

hists_unweighted = {} 
hists_weighted = {}
hists_weights = {}

# Grab our histos
for template, info in templates_dict.iteritems():
  hists_unweighted[template] = combine_hists(f.Get(hist_names[0] + "_entries_" + info[0][0] + info[1]), f.Get(hist_names[1] + "_entries_" + info[0][0] + info[1]), template + "unweighted")
  hists_weighted[template] = combine_hists(f.Get(hist_names[0] + "_" + info[0][0] + info[1]), f.Get(hist_names[1] + "_" + info[0][0] + info[1]), template + "weighted")
  for i in range(len(info[0]) - 1):
    hists_unweighted[template].Add(combine_hists(f.Get(hist_names[0] + "_entries_" + info[0][i+1] + info[1]), f.Get(hist_names[1] + "_entries_" + info[0][i+1] + info[1]), template + "unweighted" + str(i)))
    hists_weighted[template].Add(combine_hists(f.Get(hist_names[0] + "_" + info[0][i+1] + info[1]), f.Get(hist_names[1] + "_" + info[0][i+1] + info[1]), template + "weighted" + str(i)))

  hists_weights[template] = hists_weighted[template].Clone(template + "weights")
  hists_weights[template].Divide(hists_unweighted[template])
  for i in range(hists_weights[template].GetNbinsX()):
    if hists_weights[template].GetBinContent(i+1) <= 0:
       hists_weights[template].SetBinContent(i+1, 0.000000001)
  hists[template] = [ hists_unweighted[template], hists_weighted[template], hists_weights[template] ]


hist_unweighted_list = [hists["ff"][0], hists["fp"][0], hists["pp"][0], hists["bkg"][0]]
hist_weighted_list = [hists["ff"][1], hists["fp"][1], hists["pp"][1], hists["bkg"][1]]
hist_weights_list = [hists["ff"][2], hists["fp"][2], hists["pp"][2], hists["bkg"][2]]

h_data =  hists["data"][1]
initial_fracs = []
for hist in hist_weighted_list:
  initial_fracs.append(hist.Integral() / h_data.Integral())

# Fit
mc = ROOT.TObjArray(4)
mc.Add(hists["ff"][0])
mc.Add(hists["fp"][0])
mc.Add(hists["pp"][0])
mc.Add(hists["bkg"][0])

fit = ROOT.TFractionFitter(h_data, mc, "Q")
for i in range(4):
  fit.SetWeight(i, hist_weights_list[i]) # set bin-by-bin weights for raw MC counts 
  if i == 3: # other bkgs, fix
    fit.Constrain(i, initial_fracs[i]-0.00000001, initial_fracs[i]+0.00000001)
  else:
    fit.Constrain(i, 0.0, 1.0)

fit.Fit()

fracs = []
frac_errs = []
scales = []
for i in range(4):
  f = ROOT.Double()
  ferr = ROOT.Double()
  fit.GetResult(i, f, ferr)
  fracs.append(f)
  frac_errs.append(ferr)
  scales.append(f/initial_fracs[i])

# Print fit results
print "Initial fractions", initial_fracs
print "Fitted fractions", fracs
print "Errors", frac_errs
print "Scales" , scales

# Make pretty table
print "\\begin{center} \\Fontvi \\begin{tabular}{|l|| r| r| r|} \\hline"
print "Template & Initial Fraction & Fitted Fraction & Scale \\\\ \\hline"
print "%s & %.2f & %.2f $ \\pm $ %.2f & %.2f \\\\" % (latex_dict[ff_template] + " (fake/fake)", initial_fracs[0], fracs[0], frac_errs[0], scales[0])
print "%s & %.2f & %.2f $ \\pm $ %.2f & %.2f \\\\" % (latex_dict[fp_template] + " (fake/prompt)", initial_fracs[1], fracs[1], frac_errs[1], scales[1])
print "%s & %.2f & %.2f $ \\pm $ %.2f & %.2f \\\\ \\hline" % (latex_dict[pp_template] + " (prompt/prompt)", initial_fracs[2], fracs[2], frac_errs[2], scales[2])
print "\\end{tabular} \\end{center}"

