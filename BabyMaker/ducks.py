import sys, os
import time
import itertools
import numpy

from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser

job_tag = "ttH_Babies_v3"
exec_path = "condor_exe.sh"
tar_path = "package.tar.gz"
hadoop_path = "ttH"

#os.system("rm -rf tasks")
#os.system("rm package.tar.gz")
#os.system("./setup.sh")
#os.system("tar -czf package.tar.gz --exclude='.git' --exclude='my*.root' --exclude='*.tar*' --exclude='merged_ntuple*.root' CMSSW_8_0_28")

base_path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2"
dslocs = [
    ["/ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M110_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M110_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_v2/", base_path + "/ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_v2/*", 1] ,
    ["/ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Down/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Down/*", 1] ,
    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Up/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Up/*", 1] ,
    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_DownPS/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_DownPS/*", 1] ,
    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_UpPS/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_UpPS/*", 1] ,
    ["/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2/", base_path + "/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2/*", 1] ,
    ["/ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_v2/", base_path + "/ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_v2/*", 1] ,
    ["/ttHJetToGG_M70_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M70_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M80_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M80_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHJetToGG_M90_13TeV_amcatnloFXFX_madspin_pythia8/", base_path + "/ttHJetToGG_M90_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/ttHToGG_M125_13TeV_powheg_pythia8_v2/", base_path + "/ttHToGG_M125_13TeV_powheg_pythia8_v2/*", 1] ,
    ["/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/", base_path + "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/*", 1] ,
    ["/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/", base_path + "/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/*", 1] ,
    ["/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
    ["/GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
    ["/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/*", 1] , 
    ["/QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
    ["/QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
    ["/QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/", base_path + "/QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/*", 1] ,
    ["/TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/", base_path + "/TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/*", 1] ,
    ["/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/", base_path + "/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/*", 1] ,
    ["/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/", base_path + "/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/*", 1] ,
    ["/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/", base_path + "/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*", 1] ,
    ["/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/", base_path + "/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/*", 1] ,
    ["/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/", base_path + "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*", 1],
    ["/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/", base_path + "/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/*", 1],
    ["/DoubleEG", base_path + "/DoubleEG/*", 25] ,
]

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
