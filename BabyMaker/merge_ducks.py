import sys, os
import time
import itertools
import numpy
import glob

sys.path.append("~/Utilities")
sys.path.append("../Loopers/")

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "job tag e.g. 'v7'", type=str)
parser.add_argument("--year", help = "2016,2017,RunII", type=str)
parser.add_argument("--data_only", action="store_true")
parser.add_argument("--no_signal", action="store_true")
args = parser.parse_args()

import parallel_utils

hadoop_name = "smay" if os.environ.get("USER") == "sjmay" else os.environ.get("USER")

if args.year == "RunII":
  dirs = glob.glob(("/hadoop/cms/store/user/%s/ttH/*ttH_Babies_" % hadoop_name) + args.year + args.tag)
else:
  dirs = glob.glob(("/hadoop/cms/store/user/%s/ttH/*ttH_Babies_" % hadoop_name) + args.tag + "_" + args.year)
print("Adding ntuples from the following directories:")
for dir in dirs:
  print(dir)

destination = (os.popen("pwd").read()).rstrip() + "/../Loopers/merged_babies/"

if not os.path.isdir(destination):
  os.system("mkdir %s" % destination)

# Clean up old files
old_files = glob.glob(destination + "/*.root")
if len(old_files) > 0:
  os.system("rm %s/*.root" % destination)

nPar = 4
command_list = []

for dir in dirs:
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
  
  command = "addHistos %s %s %d %d" % (destination + name + "/merged_ntuple", dir + "/merged_ntuple", len(files), nPar)
  print(command)
  command_list.append(command)

  #print("addHistos %s %s %d %d" % (destination + name + "/merged_ntuple", dir + "/merged_ntuple", len(files), nPar))
  #os.system("addHistos %s %s %d %d" % (destination + name + "/merged_ntuple", dir + "/merged_ntuple", len(files), nPar))

parallel_utils.submit_jobs(command_list, 8, False)

#for dir in dirs:  
  # Delete intermediate files
  #name = dir.split("/")[-1]
  #os.system("rm %s" % destination + name + "/merged_ntuple_*.root")
