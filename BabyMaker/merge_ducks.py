import sys, os
import time
import itertools
import numpy
import glob

sys.path.append("~/Utilities")

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("tag", help = "job tag e.g. 'v7'", type=str)
args = parser.parse_args()

dirs = glob.glob("/hadoop/cms/store/user/smay/ttH/*ttH_Babies_" + args.tag)
destination = "/home/users/sjmay/tth/ttH/Loopers/merged_babies/"

nPar = 10
for dir in dirs:
  files = glob.glob(dir+"/merged_ntuple*.root")
  name = dir.split("/")[-1]
  print(name)
  if not os.path.isdir(destination + name):
    os.system("mkdir %s" % destination + name)
  print("addHistos %s %s %d %d" % (destination + name + "/merged_ntuple", dir + "/merged_ntuple", len(files), nPar))
  os.system("addHistos %s %s %d %d" % (destination + name + "/merged_ntuple", dir + "/merged_ntuple", len(files), nPar))
