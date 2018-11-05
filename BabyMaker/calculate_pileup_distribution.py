import sys, os

import argparse
parser = argparse.ArgumentParser()

parser.add_argument("--campaign", help = "which flashgg campaign to consider (e.g. RunIIFall17-3_1_0)", type=str)
parser.add_argument("--year", help = "2016, 2017, or 2018", type=str)
parser.add_argument("--tag", help = "give a name to this pu weighting", type=str)

args = parser.parse_args()

# Move to proper flashgg area
os.chdir("%s/src/flashgg" % os.popen("echo $CMSSW_BASE").read().strip("\n"))

# Get lumi json from catalog and get overlap with cert JSON
data_name = "EGamma" if args.year == "2018" else "DoubleEG"

print("fggManageSamples.py -C %s getlumi /%s\* output=%s_%s_proc.json" % (args.campaign, data_name, data_name, args.tag))
os.system("fggManageSamples.py -C %s getlumi /%s\* output=%s_%s_proc.json" % (args.campaign, data_name, data_name, args.tag))

json_file = { 
	"2016" : "/home/users/sjmay/ttH/BabyMaker/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt",
	"2017" : "/home/users/sjmay/ttH/BabyMaker/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt",
	"2018" : "/home/users/sjmay/ttH/BabyMaker/Cert_314472-324420_13TeV_PromptReco_Collisions18_JSON.txt",
}

print("compareJSON.py --and %s %s_%s_proc.json %s_%s_cert.json" % (json_file[args.year], data_name, args.tag, data_name, args.tag))
os.system("compareJSON.py --and %s %s_%s_proc.json %s_%s_cert.json" % (json_file[args.year], data_name, args.tag, data_name, args.tag))

# Compute pileup distribution
if not os.path.isfile("pileup_latest.txt"):
  print("scp smay@lxplus.cern.ch:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions" + args.year[2:] + "/13TeV/PileUp/pileup_latest.txt .")
  os.system("scp smay@lxplus.cern.ch:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions" + args.year[2:] + "/13TeV/PileUp/pileup_latest.txt .")

minBiasXsec = 69000
nPileupBins = 99

print("pileupCalc.py -i %s_%s_cert.json --inputLumiJSON %s --calcMode true --minBiasXsec %d --maxPileupBin %d --numPileupBins %d %s_%s_cert.root" % (data_name, args.tag, "pileup_latest.txt", minBiasXsec, nPileupBins, nPileupBins, data_name, args.tag))
os.system("pileupCalc.py -i %s_%s_cert.json --inputLumiJSON %s --calcMode true --minBiasXsec %d --maxPileupBin %d --numPileupBins %d %s_%s_cert.root" % (data_name, args.tag, "pileup_latest.txt", minBiasXsec, nPileupBins, nPileupBins, data_name, args.tag))

# Convert to text format
print("python ../../../pu2string.py %s_%s_cert.root" % (data_name, args.tag))
os.system("python ../../../pu2string.py %s_%s_cert.root" % (data_name, args.tag))
