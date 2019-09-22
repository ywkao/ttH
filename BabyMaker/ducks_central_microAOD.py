import sys, os
import time
import itertools
import numpy
import glob
import datetime

from metis.Sample import Sample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser

from flashgg_utils import *

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to identify this set of babies", type=str)
parser.add_argument("--soft_rerun", help = "don't remake tarball", action="store_true")
parser.add_argument("--update_tarball", help = "remake and update tarball for each task", action = "store_true")
parser.add_argument("--use_xrdcp", help = "copy the tarball with xrdcp", action = "store_true")
args = parser.parse_args()

job_tag = "ttH_Babies_RunII" + args.tag
exec_path = "condor_exe.sh"
tar_path = "package_%s.tar.gz" % job_tag if args.use_xrdcp else "package.tar.gz"
hadoop_path = "ttH"

cmssw_ver = "CMSSW_10_5_0"

if not args.soft_rerun:
  if not args.update_tarball:
    os.system("rm -rf tasks/*" + args.tag + "*")
  os.system("rm package.tar.gz")

  os.system("XZ_OPT='-9e -T24' tar -Jc --exclude='.git' --exclude='my*.root' --exclude='*.tar*' --exclude='merged_ntuple*.root' --exclude='*.out' --exclude='*.err' --exclude='*.log' --exclude *MicroAOD/BatchSubmit/* --exclude *MetaData/data/Era201*/*.json --exclude '*Taggers/data/DNN_models/*' --exclude '*Taggers/data/*dijet*' --exclude '*76X*.root' --exclude '*76X*.xml' --exclude '*80X*.xml' --exclude '*80X*.root' --exclude '*Taggers/data/DNN_models/*' --exclude '*Taggers/data/HHTagger/*' --exclude '*Taggers/data/ttHKiller/*' -f package.tar.gz %s" % cmssw_ver)

  if args.use_xrdcp:
      os.system("cp package.tar.gz /hadoop/cms/store/user/smay/ttH/tarballs/%s" % tar_path)
      os.system("cp condor_exe_xrd.sh condor_exe_%s.sh" % job_tag)
      with open("condor_exe_%s.sh" % job_tag, "r") as f_in:
          lines = f_in.readlines()
      with open("condor_exe_%s.sh" % job_tag, "w") as f_out:
        for i in range(len(lines)):
            if "FIXME" in lines[i]:
                lines[i] = lines[i].replace("FIXME", tar_path)
        for line in lines:
            f_out.write(line)

  os.system("for task in `ls -1 -d tasks/*%s*/`; do echo cp package.tar.gz $task/package.tar.gz; cp package.tar.gz $task/package.tar.gz; done" % args.tag) # make sure we overwrite any previously existing tar files

  if not args.update_tarball:
      with open("versions.txt", "a") as fout:
        os.chdir("%s/src/flashgg/" % cmssw_ver)
        commit = os.popen("git log -n 1 --pretty=format:'%H'").read()
        branch = "tth_dev" + ((os.popen("git branch").read()).strip("\n")).split("tth_dev")[-1]
        os.chdir("../../..")
        fout.write("Date: %s \n" % datetime.datetime.now())
        fout.write("Submitting ttH Babies version %s using central microAOD and commit %s of %s branch of flashgg\n" % (args.tag, commit, branch))
        fout.write("\n")

conds_dict = {
        "2016" : "MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json",
        "2017" : "MetaData/data/MetaConditions/Era2017_RR-31Mar2018_v1.json",
        "2018" : "MetaData/data/MetaConditions/Era2018_RR-17Sep2018_v1.json"
}

catalogs = ["Era2016_RR-17Jul2018_v2", "Era2017_RR-31Mar2018_v2", "Era2018_RR-17Sep2018_v2"]
samples = get_samples_from_catalogs(catalogs)
print "Processing %d samples" % len(samples.keys())
os.system("sleep 5s")
#summarize_samples(samples.copy())

def fpo(sample):
    if "ttHJetToGG" in sample or "ttHToGG" in sample or "THQ" in sample or "THW" in sample:
        return 1
    elif any([x in sample for x in ["VBF", "GluGluHToGG", "VHToGG", "bbH"]]):
        return 3
    elif "DiPhoton" in sample or "TTGG" in sample:
        return 3
    elif any([x in sample for x in ["DoubleEG", "EGamma"]]):
        return 10
    elif any([x in sample for x in ["DYJetsToLL", "QCD", "GJets_HT", "TTJets", "GJet-Pt"]]):
        return 10
    else:
        return 5

class file:
    def __init__(self, **kwargs):
        self.name = kwargs.get('name', None)
    def get_name(self):
        return self.name
    def get_nevents(self):
        return 1

total_summary = {}
first_run = False
while True:
    allcomplete = True
    for sample in samples.keys():
        #if not sample == "ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8":
        #    continue
        #if not "GJets_HT-200To400" in sample: 
        #    continue
        if "SingleElectron" in sample and "ttZ" not in job_tag:
            continue
        for year in samples[sample].keys():
            if year == "xs" or year == "-1":
                continue
            for production in samples[sample][year].keys():
                if production == "scale1fb":
                    continue
                dataset = sample + "_" + production
                files = samples[sample][year][production]["files"]
                files = [file(name=x) for x in files]
                metis_sample = Sample(dataset = dataset, files = files) 
                metis_sample.info["nevts"] = 0
                #print "Task for %s, %s, %s" % (sample, year, production)
                #print "Files: "
                #print metis_sample.get_files()
                print "FPO: %d for sample: %s" % (fpo(sample), sample)
                task = CondorTask(
                        sample = metis_sample,
                        open_dataset = False,
                        flush = True,
                        files_per_output = fpo(sample),
                        output_name = "merged_ntuple.root",
                        tag = job_tag,
                        cmssw_version = cmssw_ver,
                        executable = "condor_exe_%s.sh" % job_tag if args.use_xrdcp else "condor_exe.sh",
                        tarfile = "empty" if args.use_xrdcp else tar_path,
                        condor_submit_params = {"sites" : "T2_US_UCSD"} if args.use_xrdcp else {"sites" : "T2_US_UCSD,T2_US_CALTECH,T2_US_MIT,T2_US_WISCONSIN,T2_US_Nebraska,T2_US_Purdue,T2_US_Vanderbilt,T2_US_Florida"},
                        special_dir = hadoop_path,
                        arguments = conds_dict[year]
                )
                task.process()
                if not task.complete():
                    allcomplete = False
                total_summary[dataset] = task.get_task_summary()
                if first_run:
                    print "Sleeping 30s to space out jobs"
                    os.system("sleep 30s")
    StatsParser(data=total_summary, webdir="~/public_html/dump/ttH_BabyMaker_v2_%s/" % job_tag).do()
    os.system("chmod -R 755 ~/public_html/dump/ttH_BabyMaker_v2_%s" % job_tag)
    if allcomplete:
        print("")
        print("Job={} finished".format(job_tag))
        print("")
        break
    first_run = False
    print("Sleeping 300 seconds ...")
    time.sleep(300)

signal_strings = ["ttHJetToGG", "ttHToGG", "THQ", "THW", "VBF", "GluGluHToGG", "VHToGG", "FCNC", "ttHiggs"]
