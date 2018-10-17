import sys, os
import time
import itertools
import numpy
import glob

sys.path.append("~/Utilities")

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("tag", help = "job tag e.g. 'v7'", type=str)
parser.add_argument("year", help = "2016 or 2017", type=str)
parser.add_argument("--data_only", action="store_true")
parser.add_argument("--no_signal", action="store_true")
args = parser.parse_args()

hadoop_name = "smay" if os.environ.get("USER") == "sjmay" else os.environ.get("USER")

dirs = glob.glob(("/hadoop/cms/store/user/%s/ttH/*ttH_Babies_" % hadoop_name) + args.tag + "_" + args.year)
print "Adding ntuples from the following directories:"
for dir in dirs:
  print dir

destination = (os.popen("pwd").read()).rstrip() + "/../Loopers/merged_babies/"

if not os.path.isdir(destination):
  os.system("mkdir %s" % destination)

# Clean up old files
old_files = glob.glob(destination + "/*.root")
if len(old_files) > 0:
  os.system("rm %s/*.root" % destination)

nPar = 10
for dir in dirs:
  if not "TTGG" in dir:
    continue 

  if args.data_only:
    if "DoubleEG" not in dir:
      continue
  if args.no_signal:
    if "ttHJetToGG" in dir or "ttHToGG" in dir:
      continue
  files = glob.glob(dir+"/merged_ntuple*.root")
  name = dir.split("/")[-1]
  if not os.path.isdir(destination + name):
    os.system("mkdir %s" % destination + name)
  print("addHistos %s %s %d %d" % (destination + name + "/merged_ntuple", dir + "/merged_ntuple", len(files), nPar))
  os.system("addHistos %s %s %d %d" % (destination + name + "/merged_ntuple", dir + "/merged_ntuple", len(files), nPar))
  
  # Delete intermediate files
  os.system("rm %s" % destination + name + "/merged_ntuple_*.root")
