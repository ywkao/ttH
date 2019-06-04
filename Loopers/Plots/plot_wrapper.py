import sys, os

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input_file", help = "root file with histograms to plot", type=str)
parser.add_argument("--backgrounds", help = "list of processes to plot as bkgs", type=str, default="DiPhoton|GammaJets|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV")
parser.add_argument("--signals", help = "list of processes to plot as signals", type=str, default="ttH|FCNC_hct|FCNC_hut")
parser.add_argument("--plot_type", help = "which type of plot to make (e.g. std, std_linear, etc.)", type=str, default="std")
parser.add_argument("--plot_labels", help = "labels to put on plots", type=str)
args = parser.parse_args()

if "impute" in args.input_file:
  args.backgrounds.replace("GammaJets", "QCD_GammaJets_Imputed")

os.system('./makePlots "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
