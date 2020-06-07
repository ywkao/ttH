import sys, os
sys.path.append("../")

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input_file", help = "root file with histograms to plot", type=str)
parser.add_argument("--backgrounds", help = "list of processes to plot as bkgs", type=str, default="DiPhoton|GammaJets|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV")
parser.add_argument("--signals", help = "list of processes to plot as signals", type=str, default="ttH|TT_FCNC_hct|TT_FCNC_hut|ST_FCNC_hct|ST_FCNC_hut")
parser.add_argument("--plot_type", help = "which type of plot to make (e.g. std, std_linear, etc.)", type=str, default="std")
parser.add_argument("--plot_labels", help = "labels to put on plots", type=str)
args = parser.parse_args()

import parallel_utils

#if "impute" in args.input_file:
#  args.backgrounds = args.backgrounds.replace("GammaJets", "QCD_GammaJets_imputed")

parallel_utils.run('./makePlots "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))

#full_bkgs = "DiPhoton|QCD_GammaJets_imputed|GammaJets|QCD|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV|ggH|VBF|VH|THQ|THW"
full_bkgs = "DiPhoton|QCD_GammaJets_imputed|GammaJets|QCD|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTZ|TTW|VV|tV|ggH|VBF|VH|THQ|THW|ttH"
parallel_utils.run('./makeTables "%s" "%s" "%s" > tables_%s.txt' % (args.input_file, args.signals, full_bkgs, args.input_file.replace(".root", "").replace("../", "")))
