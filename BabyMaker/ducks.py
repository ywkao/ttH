import sys, os
import time
import itertools
import numpy
import glob
import datetime

from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to identify this set of babies", type=str)
parser.add_argument("--data_only", help = "run on data only", action="store_true")
parser.add_argument("--soft_rerun", help = "don't remake tarball", action="store_true")
args = parser.parse_args()

job_tag = "ttH_Babies_RunII" + args.tag
exec_path = "condor_exe.sh"
tar_path = "package.tar.gz"
hadoop_path = "ttH"

cmssw_ver = "CMSSW_10_2_9"
base_path = "/hadoop/cms/store/user/smay/ttH/MicroAOD/RunII"

if not args.soft_rerun:
  os.system("rm -rf tasks/*" + args.tag + "*")
  os.system("rm package.tar.gz")

  os.system("XZ_OPT=-3 tar -Jc --exclude='.git' --exclude='my*.root' --exclude='*.tar*' --exclude='merged_ntuple*.root' -f package.tar.gz %s" % cmssw_ver) 

  with open("versions.txt", "a") as fout:
    os.chdir("%s/src/flashgg/" % cmssw_ver)
    commit = os.popen("git log -n 1 --pretty=format:'%H'").read()
    branch = "tth_dev" + ((os.popen("git branch").read()).strip("\n")).split("tth_dev")[-1]
    os.chdir("../../..")
    fout.write("Date: %s \n" % datetime.datetime.now())
    fout.write("Submitting ttH Babies version %s using skims in %s and commit %s of %s branch of flashgg\n" % (args.tag, base_path, commit, branch))
    fout.write("\n")

conds_dict = {
        "2016" : "MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json",
        "2017" : "MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json",
        "2018" : "MetaData/data/MetaConditions/Era2018_RR-17Sep2018_v1.json"
}

samples = glob.glob(base_path + "/*")
datasets = {}

for sample in samples:
  name = sample.split("/")[-1]
  datasets[name] = { "input_loc" : sample }
  datasets[name]["isData"] = True if ("EGamma" in name or "DoubleEG" in name) else False
  if datasets[name]["isData"]:
    datasets[name]["fpo"] = 25
  elif "ttH" in name:
    datasets[name]["fpo"] = 1
  elif "DiPhoton" in name:
    datasets[name]["fpo"] = 5
  else:
    datasets[name]["fpo"] = 100

  if datasets[name]["isData"]:
    if "Run2016" in sample:
      datasets[name]["meta_conditions"] = conds_dict["2016"]
    elif "Run2017" in sample:
      datasets[name]["meta_conditions"] = conds_dict["2017"]
    elif "Run2018" in sample:
      datasets[name]["meta_conditions"] = conds_dict["2018"]
    else:
      print "ERROR: could not match data sample to a year"
  else:
    if "RunIISummer16" in sample:
      datasets[name]["meta_conditions"] = conds_dict["2016"]
    elif "RunIIFall17" in sample:
      datasets[name]["meta_conditions"] = conds_dict["2017"]
    elif "RunIIAutumn18" in sample:
      datasets[name]["meta_conditions"] = conds_dict["2018"]
    else:
      print "ERROR: could not match MC sample to a year"



#print datasets

total_summary = {}
while True:
    allcomplete = True
    for dataset, info in datasets.iteritems():
      if args.data_only and not info["isData"]:
        continue
      print "Submitting jobs for: ", dataset
      #time.sleep(15)
      sample = DirectorySample( dataset = dataset, location = info["input_loc"])
      files = [f.name for f in sample.get_files()]
      task = CondorTask(
	sample = sample,
	open_dataset = False,
	flush = True,
	files_per_output = info["fpo"],
	output_name = "merged_ntuple.root",
	tag = job_tag,
	cmssw_version = cmssw_ver,
	executable = exec_path,
	tarfile = tar_path,
	condor_submit_params = {"sites" : "T2_US_UCSD"},
        special_dir = hadoop_path,
	arguments = info["meta_conditions"]
        )
      task.process()
      if not task.complete():
        allcomplete = False
      # save some information for the dashboard
      total_summary[dataset] = task.get_task_summary()
      break
    # parse the total summary and write out the dashboard
    #StatsParser(data=total_summary, webdir="~/public_html/dump/ttH_BabyMaker/").do()
    os.system("chmod -R 755 ~/public_html/dump/ttH_BabyMaker")
    if allcomplete:
        print ""
        print "Job={} finished".format(job_tag)
        print ""
        break
    print "Sleeping 300 seconds ..."
    time.sleep(300)


    break

