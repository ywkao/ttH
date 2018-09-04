import sys, os

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("channel", help = "Hadronic or Leptonic", type=str)
parser.add_argument("selection", help = "e.g. ttHLeptonicLoose", type=str)
parser.add_argument("year", help = "e.g. 2016 or 2017", type=str)
parser.add_argument("tag", help = "tag to identify this training of the BDT", type=str)
parser.add_argument("-r", "--randomize", help = "use a random test/train split", action="store_true")
args = parser.parse_args()

build_success = os.system("make")
if build_success != 0:
  print "Errors building, will not run rest of BDT workflow"
  os.system("exit(1)")

if args.randomize:
  os.chdir("Utils")
  os.system("python generate_random_map.py %s" % args.channel)
  os.chdir("../")

os.system("./ttH%sMVABabyMaker %s %s" % (args.channel, args.selection, args.year))
os.chdir("../MVAs")
os.system("python prep.py %s" % args.channel)
if args.randomize:
  os.system("python prep.py %s --randomize" % args.channel)
else:
  os.system("python prep.py %s" % args.channel)
os.system("python train.py %s %s" % (args.channel, args.tag))
os.chdir("../Loopers")
os.system("./ttH%sLooper %s %s %s" % (args.channel, args.selection, args.year, args.channel + "_" + args.tag + '_bdt.xml'))

os.chdir("Optimization")
if args.randomize:
  os.system("python estimate_significance.py MVAOptimizationBaby_%s_%s_bdt.root --randomize" % (args.channel, args.tag))
else:
  os.system("python estimate_significance.py MVAOptimizationBaby_%s_%s_bdt.root" % (args.channel, args.tag))
