import sys, os

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("channel", help = "Hadronic or Leptonic", type=str)
parser.add_argument("selection", help = "e.g. ttHLeptonicLoose", type=str)
parser.add_argument("year", help = "e.g. 2016 or 2017", type=str)
parser.add_argument("ext", help = "extension, e.g. '1'", type=str)
parser.add_argument("tag", help = "tag to identify this training of the BDT", type=str)
parser.add_argument("-r", "--randomize", help = "use a random test/train split", action="store_true")
parser.add_argument("-i", "--invert", help = "invert the test/train split", action="store_true")
args = parser.parse_args()

build_success = os.system("make")
if build_success != 0:
  print "Errors building, will not run rest of BDT workflow"
  os.system("exit(1)")

if args.randomize:
  os.chdir("Utils")
  random_map = "random_map_%s_%s.txt" % (args.channel, args.ext)
  if not os.path.exists(random_map): # only generate random map if it doesn't already exist 
    os.system("python generate_random_map.py %s %s" % (args.channel, args.ext))
  os.chdir("../")

os.system("./ttH%sMVABabyMaker %s %s %s" % (args.channel, args.selection, args.year, args.ext))
os.chdir("../MVAs")

mva_baby = "../Loopers/MVABaby_ttH%s_%s.root" % (args.channel, args.ext)
if args.randomize:
  os.system("python prep.py %s --randomize" % (mva_baby))
elif args.invert:
  os.system("python prep.py %s --invert" % (mva_baby))
else:
  os.system("python prep.py %s" % (mva_baby))

hdf5_file = "ttH%s_%s_features.hdf5" % (args.channel, args.ext)
os.system("python train.py %s %s %s %s" % (args.channel, hdf5_file, args.ext, args.tag))
os.chdir("../Loopers")
os.system("./ttH%sLooper %s %s %s %s" % (args.channel, args.selection, args.year, args.ext, args.channel + "_" + args.tag + "_" + args.ext + '_bdt.xml'))

os.chdir("Optimization")
if args.randomize:
  os.system("python estimate_significance.py MVAOptimizationBaby_%s_%s_%s_%s_bdt.root --randomize" % (args.ext, args.channel, args.tag, args.ext))
else:
  os.system("python estimate_significance.py MVAOptimizationBaby_%s_%s_%s_%s_bdt.root" % (args.ext, args.channel, args.tag, args.ext))

# Cleanup
os.chdir("../")
os.system("rm MVABaby_ttH%s_%s.root" % (args.channel, args.ext))
os.system("rm ../MVAs/ttH%s_%s_features.hdf5" % (args.channel, args.ext))
#os.system("rm Utils/random_map_%s_%s.txt" % (args.channel, args.ext))  # save random maps so that we can train multiple BDTs with the same test/train splits
