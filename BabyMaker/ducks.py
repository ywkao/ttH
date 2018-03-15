import sys, os
import time
import itertools
import numpy

from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask
from metis.StatsParser import StatsParser

job_tag = "ttH_Babies_v0"
exec_path = "condor_exe.sh"
tar_path = "package.tar.gz"
hadoop_path = "ttH"

os.system("rm -rf tasks")
os.system("rm package.tar.gz")
os.system("tar -czf package.tar.gz --exclude='.git' --exclude='my*.root' --exclude='*.tar*' CMSSW_8_0_28")

dslocs = [
    ["/ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8/", "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8/*", 1] ,
    ["/DoubleEG", "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/DoubleEG/*", 25] ,
]

total_summary = {}
while True:
    allcomplete = True
    for ds,loc,fpo in dslocs:
        sample = DirectorySample( dataset=ds, location=loc )
        files = [f.name for f in sample.get_files()]
        print(files)
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
    StatsParser(data=total_summary, webdir="~/public_html/dump/ttH_BabyMaker/").do()
    os.system("chmod -R 755 ~/public_html/dump/ttH_BabyMaker")
    if allcomplete:
        print ""
        print "Job={} finished".format(job_tag)
        print ""
        break
    print "Sleeping 300 seconds ..."
    time.sleep(300)
