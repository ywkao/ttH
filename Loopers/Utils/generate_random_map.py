import os, sys
import ROOT, root_numpy
import glob
import random

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("channel", help = "Hadronic or Leptonic", type=str)
parser.add_argument("ext", help = "extension, e.g. '1'", type=str)
args = parser.parse_args()

baby_version_2016 = "v3.16"
baby_version_2017 = "v1.2"

files_2016 = glob.glob("../merged_babies/*ttH_Babies_%s_2016/*.root" % baby_version_2016)
files_2017 = glob.glob("../merged_babies/*ttH_Babies_%s_2017/*.root" % baby_version_2017)
files = files_2016 + files_2017

with open("random_map_%s_%s.txt" % (args.channel, args.ext), "w") as fout:

  for file in files:
    f = ROOT.TFile(file)
    tree = f.Get("tth%sTagDumper/trees/tth_13TeV_all" % args.channel)

    info = root_numpy.tree2array(tree, branches = ["event", "run", "lumi"])
    evt, run, lumi = info["event"], info["run"], info["lumi"]

    for i in range(tree.GetEntries()):
      key = str(evt[i]) + "_" + str(run[i]) + "_" + str(lumi[i])
      rand = random.random()
      fout.write('%s %.10f\n' % (key, rand))
