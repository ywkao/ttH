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
parser.add_argument("--data_only", action="store_true")
args = parser.parse_args()

job_tag = "ttH_Babies_" + args.tag
exec_path = "condor_exe.sh"
tar_path = "package.tar.gz"
hadoop_path = "ttH"

os.system("rm -rf tasks/*" + args.tag)
os.system("rm package.tar.gz")
os.system("tar -czf package.tar.gz --exclude='.git' --exclude='my*.root' --exclude='*.tar*' --exclude='merged_ntuple*.root' CMSSW_8_0_28")

base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2"
#base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v3_jetPt20"
with open("versions.txt", "a") as fout:
  os.chdir("CMSSW_8_0_28/src/flashgg/")
  commit = os.popen("git log -n 1 --pretty=format:'%H'").read()
  os.chdir("../../..")
  fout.write("Date: %s \n" % datetime.datetime.now())
  fout.write("Submitting ttH Babies version %s using skims in %s and commit %s of tth_dev branch of flashgg\n" % (args.tag, base_path, commit))
  fout.write("\n")

subdir_map = { 	"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1",
		"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1",
		"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1",
		"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-BS2016_BSandPUSummer16_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1",
		"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1"
}
default_subdir = "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1"


#samples = glob.glob("/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/*")
samples = glob.glob(base_path + "/*")
dslocs = []
for sample in samples:
  name = sample.split("/")[-1]
  if name in subdir_map.keys():
    subdir = subdir_map[name]
  else:
    subdir = default_subdir
  if "DoubleEG" in name:
    nFilesPerOutput = 25
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
                cmssw_version = "CMSSW_9_2_1", # doesn't do anything
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
    os.system("python delete_bad_files.py %s" % args.tag)
    print "Sleeping 300 seconds ..."
    time.sleep(300)
