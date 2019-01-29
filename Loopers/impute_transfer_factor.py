import ROOT
import numpy

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input_fail", help = "file with histograms for events failing minIDMVA cut", type=str)
parser.add_argument("--input_pass", help = "file with histograms for events passing minIDMVA cut", type=str)
args = parser.parse_args()

f_fail = ROOT.TFile(args.input_fail)
f_pass = ROOT.TFile(args.input_pass)

def qcd_gjets_yield(f):
  qcd_hist = f.Get("hNJets_QCD")
  qcd_yield = qcd_hist.Integral()
  gjets_hist = f.Get("hNJets_GammaJets")
  gjets_yield = gjets_hist.Integral()
  print qcd_yield, gjets_yield
  return qcd_yield + gjets_yield

yield_fail = qcd_gjets_yield(f_fail)
yield_pass = qcd_gjets_yield(f_pass)

print "Transfer factor is: %.3f" % (yield_pass / yield_fail)
