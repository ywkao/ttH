import sys, os
import time
import itertools
import numpy
import glob

from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser

job_tag = "ttH_Babies_v7"
exec_path = "condor_exe.sh"
tar_path = "package.tar.gz"
hadoop_path = "ttH"

#os.system("rm -rf tasks")
os.system("rm package.tar.gz")
os.system("tar -czf package.tar.gz --exclude='.git' --exclude='my*.root' --exclude='*.tar*' --exclude='merged_ntuple*.root' CMSSW_8_0_28")

base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2"
subdir_map = { 	"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1",
		"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1",
		"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1",
		"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2" : "RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-BS2016_BSandPUSummer16_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1"
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
  else:
    nFilesPerOutput = 1
    dslocs.append(["/" + name + "/", base_path + "/" + name + "/" + subdir + "/", nFilesPerOutput])


#dslocs = [
#    ["/ttHJetToGG_M1_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M1_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M11_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M11_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M12_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M12_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M12_13TeV_amcatnloFXFX_madspin_pythia8_v2/", base_path + "/ttHJetToGG_M12_13TeV_amcatnloFXFX_madspin_pythia8_v2/*", 1] ,
#    ["/ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Down/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Down/*", 1] ,
#    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Up/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Up/*", 1] ,
#    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_DownPS/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_DownPS/*", 1] ,
#    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_UpPS/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_UpPS/*", 1] ,
#    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2/*", 1] ,
#    ["/ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M13_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M13_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M13_13TeV_amcatnloFXFX_madspin_pythia8_v2/", base_path + "/ttHJetToGG_M13_13TeV_amcatnloFXFX_madspin_pythia8_v2/*", 1] ,
#    ["/ttHJetToGG_M7_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M7_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M8_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M8_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHJetToGG_M9_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M9_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
#    ["/ttHToGG_M125_13TeV_powheg_pythia8_v2/", base_path + "/ttHToGG_M125_13TeV_powheg_pythia8_v2/*", 1] ,
#    ["/DYJetsToLL_M-5_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/", base_path + "/DYJetsToLL_M-5_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/*", 1] ,
#    ["/DiPhotonJetsBox_MGG-8toInf_13TeV-Sherpa/", base_path + "/DiPhotonJetsBox_MGG-8toInf_13TeV-Sherpa/*", 1] ,
#    ["/GJet_Pt-2to4_DoubleEMEnriched_MGG-8toInf_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/GJet_Pt-2to4_DoubleEMEnriched_MGG-8toInf_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
#    ["/GJet_Pt-2toInf_DoubleEMEnriched_MGG-4to8_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/GJet_Pt-2toInf_DoubleEMEnriched_MGG-4to8_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
#    ["/GJet_Pt-4toInf_DoubleEMEnriched_MGG-8toInf_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/GJet_Pt-4toInf_DoubleEMEnriched_MGG-8toInf_TuneCUETP8M1_13TeV_Pythia8/*", 1] , 
#    ["/QCD_Pt-3to4_DoubleEMEnriched_MGG-8toInf_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/QCD_Pt-3to4_DoubleEMEnriched_MGG-8toInf_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
#    ["/QCD_Pt-3toInf_DoubleEMEnriched_MGG-4to8_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/QCD_Pt-3toInf_DoubleEMEnriched_MGG-4to8_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
#    ["/QCD_Pt-4toInf_DoubleEMEnriched_MGG-8toInf_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/QCD_Pt-4toInf_DoubleEMEnriched_MGG-8toInf_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
#    ["/TTGG_Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/", base_path + "/TTGG_Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/*", 1] ,
#    ["/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/", base_path + "/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/*", 1] ,
#    ["/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/", base_path + "/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/*", 1] ,
#    ["/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/", base_path + "/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*", 1] ,
#    ["/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/", base_path + "/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/*", 1] ,
#    ["/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/", base_path + "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*", 1],
#    ["/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/", base_path + "/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*", 1],
#    ["/DoubleEG", base_path + "/DoubleEG/*", 25] ,
#]

#dslocs = [
#	["/DoubleEG", base_path + "/DoubleEG/*", 25] ,
#]

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
        allcomplete = allcomplete and task.complete()
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
    print "Sleeping 300 seconds ..."
    time.sleep(300)
