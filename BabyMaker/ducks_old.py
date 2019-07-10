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
parser.add_argument("--skip_data", action="store_true")
parser.add_argument("--data_only", action="store_true")
parser.add_argument("--central_microAOD", action="store_true")
parser.add_argument("--updated_microAOD", action="store_true")
parser.add_argument("--old_data", action="store_true")
parser.add_argument("--do_all", help = "run on every single skim (not just important ones)", action="store_true")
parser.add_argument("--soft_rerun", help = "don't remake tarball", action="store_true")
parser.add_argument("--tH_only", help = "only jobs for THW, THQ", action="store_true")
args = parser.parse_args()

job_tag = "ttH_Babies_" + args.tag + "_" + args.year
exec_path = "condor_exe_old.sh"
tar_path = "package.tar.gz"
hadoop_path = "ttH"

old_data_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2017_skim_v1"

if args.year == "2016":
  cmssw_ver = "CMSSW_8_0_28"
  #base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v3_jetPt20"
  base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/2016_topTag_overlapRemoval"
elif args.year == "2017":
  cmssw_ver = "CMSSW_9_4_6"
  #base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2017_skim_v1"
  #base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/RunIIFall17-3_2_0_skim_v1" # new version of microAOD with required gen info for tt+X overlap removal
  base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/forHualin_2017" # microAOD with tt+X overlap and top tagger info
  if args.central_microAOD:
    base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2017_skim_v1"
  if args.updated_microAOD:
    base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/2017_updated/"
elif args.year == "2018":
  cmssw_ver = "CMSSW_10_2_1"
  base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/RunIIFall18-4_0_0/"

if not args.soft_rerun:
  os.system("rm -rf tasks/*" + args.tag + "_" + args.year)
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

subdir_map = { 	"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1",
		"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1",
		"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1",
		"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-BS2016_BSandPUSummer16_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1",
		"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1",
		"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8" : "RunIIFall17-3_1_0-3_1_0-v1-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
		#"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8" : "RunIIFall17-3_2_0-3_2_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
}
default_subdir = "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1"

important_samples = ["TTJets", "TTGJets", "TTGG", "QCD", "GJet_Pt-", "DiPhoton", "DY", "WG", "ZG", "WJets", "ttHJetToGG_M1", "DoubleEG", "EGamma", "TTTo2L2Nu", "TTToSemiLeptonic", "THQ", "THW", "TGJets", "VHToGG_M125", "VBFHToGG_M125", "GluGluHToGG_M125", "GJets_HT", "WW", "WZ", "ZZ", "TTZ", "TTW", "tZq", "tW"]
def important_sample(name):
  if args.tH_only:
    if "THQ" in name or "THW" in name:
      return True
    else:
      return False
  if args.year == "2018":
    #if "101X" not in name:
    #  return False
    return True # need to FIXME later
  for sample in important_samples:
    if sample in name:
      return True

  return False
 
samples = glob.glob(base_path + "/*")
if args.old_data:
  samples = glob.glob(old_data_path + "/*")
dslocs = []
for sample in samples:
  name = sample.split("/")[-1]
  if args.data_only and not ("DoubleEG" in name or "EGamma" in name):
    continue
  if not args.do_all and not important_sample(name):
    print("Skipping %s" % name)
    continue
  if name in list(subdir_map.keys()):
    subdir = subdir_map[name]
  else:
    subdir = default_subdir

  if "DoubleEG" in name or "EGamma" in name:
    if args.skip_data:
      continue
    nFilesPerOutput = 25
    if not args.old_data:
      dslocs.append(["/" + name + "/", base_path + "/" + name + "/", nFilesPerOutput])
    else:
      dslocs.append(["/" + name + "/", old_data_path + "/" + name + "/*", nFilesPerOutput])
  if "ttH" in name or "TTGG" in name:
    nFilesPerOutput = 1
  elif "DiPhoton" in name: 
      nFilesPerOutput = 5
  else:
      nFilesPerOutput = 100

  if args.year == "2016" and "topTag_overlapRemoval" not in base_path:
    dslocs.append(["/" + name + "/", base_path + "/" + name + "/" + subdir + "/", nFilesPerOutput])
  elif args.year == "2017":
    if "forHualin" in base_path:
      dslocs.append(["/" + name + "/", base_path + "/" + name + "/", nFilesPerOutput])
    elif "TTGG" in name:
      dslocs.append(["/" + name + "/", base_path + "/" + name + "/" + subdir + "/", nFilesPerOutput])
    else:
      dslocs.append(["/" + name + "/", base_path + "/" + name + "/*", nFilesPerOutput])    
  else:
    print("here")
    dslocs.append(["/" + name + "/", base_path + "/" + name + "/", nFilesPerOutput]) 


print(dslocs)
#time.sleep(300)


total_summary = {}
while True:
    allcomplete = True
    for ds,loc,fpo in dslocs:
	print(loc)
        sample = DirectorySample( dataset=ds, location=loc )
        files = [f.name for f in sample.get_files()]
        sample.set_files([f.name for f in sample.get_files() if "/cms/store/user/bemarsh/flashgg/MicroAOD/forHualin_2017/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2_MINIAODSIM_forHualin_2017/test_skim_630.root" not in f.name])
        #print [f.name for f in sample.get_files()]
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
        print("")
        print("Job={} finished".format(job_tag))
        print("")
        break
    #os.system("python delete_bad_files.py %s" % args.tag)
    print("Sleeping 300 seconds ...")
    time.sleep(300)
