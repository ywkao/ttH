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
parser.add_argument("tag", help = "job tag e.g. 'v7'", type=str)
parser.add_argument("year", help = "which year to run on e.g. '2016'", type=str)
parser.add_argument("--data_only", action="store_true")
parser.add_argument("--do_all", help = "run on every single skim (not just important ones)", action="store_true")
parser.add_argument("--soft_rerun", help = "don't remake tarball", action="store_true")
args = parser.parse_args()

job_tag = "ttH_Babies_" + args.tag + "_" + args.year
exec_path = "condor_exe.sh"
tar_path = "package.tar.gz"
hadoop_path = "ttH"

if args.year == "2016":
  cmssw_ver = "CMSSW_8_0_28"
  base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v3_jetPt20"
elif args.year == "2017":
  cmssw_ver = "CMSSW_9_4_6"
  base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2017_skim_v1"

if not args.soft_rerun:
  os.system("rm -rf tasks/*" + args.tag + "_" + args.year)
  os.system("rm package.tar.gz")

  #os.system("tar -czf package.tar.gz --exclude='.git' --exclude='my*.root' --exclude='*.tar*' --exclude='merged_ntuple*.root' --exclude='*.cc' --exclude='*.h' --exclude-vcs %s" % cmssw_ver)

  os.system("XZ_OPT=-9 tar -Jc --exclude='.git' --exclude='my*.root' --exclude='*.tar*' --exclude='merged_ntuple*.root' --exclude='*.cc' --exclude='*.h' --exclude-vcs --exclude='*.C' -f package.tar.gz %s" % cmssw_ver) 

  with open("versions.txt", "a") as fout:
    os.chdir("%s/src/flashgg/" % cmssw_ver)
    commit = os.popen("git log -n 1 --pretty=format:'%H'").read()
    branch = "tth_dev" + ((os.popen("git branch").read()).strip("\n")).split("tth_dev")[-1]
    os.chdir("../../..")
    fout.write("Date: %s \n" % datetime.datetime.now())
    fout.write("Submitting ttH Babies version %s using skims in %s and commit %s of %s branch of flashgg\n" % (args.tag, base_path, commit, branch))
    fout.write("\n")

subdir_map = { 	"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1",
		"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1",
		"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1",
		"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-BS2016_BSandPUSummer16_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1",
		"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1"
}
default_subdir = "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1"

important_samples = ["TTJets", "TTGJets", "TTGG", "QCD", "GJet_Pt-", "DiPhoton", "DY", "WG", "ZG", "WJets", "ttHJetToGG_M125", "DoubleEG"]
def important_sample(name):
  for sample in important_samples:
    if sample in name:
      return True
  return False
 
samples = glob.glob(base_path + "/*")
dslocs = []
for sample in samples:
  name = sample.split("/")[-1]
  if args.data_only and not "DoubleEG" in name:
    continue
  if not args.do_all and not important_sample(name):
    print "Skipping %s" % name
    continue
  if name in subdir_map.keys():
    subdir = subdir_map[name]
  else:
    subdir = default_subdir
  if "DoubleEG" in name:
    nFilesPerOutput = 25
    dslocs.append(["/" + name + "/", base_path + "/" + name + "/*", nFilesPerOutput])
  elif not args.year == "2016": # in 2016 there are some samples we want to exclude. For 2017, use everything that is there (usually there is only one subdirectory, but e.g. for DY, there is a nominal sample and an ext1 sample and we want to use both)
    if "ttH" in name:
      nFilesPerOutput = 1
    elif "DiPhoton" in name or "TTGG" in name:
      nFilesPerOutput = 5
    else:
      nFilesPerOutput = 100
    dslocs.append(["/" + name + "/", base_path + "/" + name + "/*", nFilesPerOutput])
  elif not args.data_only:
    if "ttH" in name or "DiPhoton" in name or "TTGG" in name:
      nFilesPerOutput = 1
    else:
      nFilesPerOutput = 25
    dslocs.append(["/" + name + "/", base_path + "/" + name + "/" + subdir + "/", nFilesPerOutput])

total_summary = {}
while True:
    allcomplete = True
    for ds,loc,fpo in dslocs:
        sample = DirectorySample( dataset=ds, location=loc )
        #files = [f.name for f in sample.get_files()]
        task = CondorTask(
                sample = sample,
                open_dataset = False,
                flush = True,
                files_per_output = fpo,
                output_name = "merged_ntuple.root",
                tag = job_tag,
                cmssw_version = cmssw_ver, # doesn't do anything
                executable = exec_path,
                tarfile = tar_path,
                condor_submit_params = {"sites" : "T2_US_UCSD"},
                special_dir = hadoop_path
                )
        task.process()
	if not task.complete():
          allcomplete = False
        # save some information for the dashboard
        total_summary[ds] = task.get_task_summary()
    # parse the total summary and write out the dashboard
    #StatsParser(data=total_summary, webdir="~/public_html/dump/ttH_BabyMaker/").do()
    os.system("chmod -R 755 ~/public_html/dump/ttH_BabyMaker")
    if allcomplete:
        print ""
        print "Job={} finished".format(job_tag)
        print ""
        break
    #os.system("python delete_bad_files.py %s" % args.tag)
    print "Sleeping 300 seconds ..."
    time.sleep(300)
