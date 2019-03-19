# Script to get "putarget" from data in order to implement pu reweighting in flashgg. Example usage:
# 2016: python calculate_pileup_distribution.py --campaign "ReMiniAOD-03Feb2017-2_5_Y" --year "2016" --tag "test" # output: 2.655e-06,3.886e-05,9.626e-05,0.0001362,0.0002348,0.0003614,0.0006951,0.006669,0.0267,0.03918,0.04415,0.04991,0.05799,0.06841,0.08072,0.09534,0.1064,0.1074,0.09827,0.08132,0.05984,0.03843,0.0212,0.009982,0.004036,0.001429,0.0004543,0.0001348,4.194e-05,1.877e-05,1.456e-05,1.468e-05,1.542e-05,1.597e-05,1.618e-05,1.603e-05,1.56e-05,1.493e-05,1.411e-05,1.324e-05,1.239e-05,1.163e-05,1.098e-05,1.048e-05,1.011e-05,9.864e-06,9.71e-06,9.62e-06,9.566e-06,9.522e-06,9.465e-06,9.372e-06,9.24e-06,9.062e-06,8.835e-06,8.561e-06,8.241e-06,7.88e-06,7.485e-06,7.06e-06,6.615e-06,6.155e-06,5.688e-06,5.221e-06,4.759e-06,4.308e-06,3.874e-06,3.459e-06,3.068e-06,2.703e-06,2.365e-06,2.055e-06,1.773e-06,1.52e-06,1.294e-06,1.094e-06,9.19e-07,7.665e-07,6.35e-07,5.225e-07,4.27e-07,3.466e-07,2.794e-07,2.237e-07,1.779e-07,1.405e-07,1.102e-07,8.59e-08,6.647e-08,5.109e-08,3.9e-08,2.957e-08,2.227e-08,1.666e-08,1.237e-08,9.128e-09,6.689e-09,4.868e-09,3.519e-09
# 2017: python calculate_pileup_distribution.py --campaign "RunIIFall17-3_1_0" --year "2017" --tag "test" # output: 6.246e-06,2.63e-05,4.92e-05,9.085e-05,9.855e-05,0.0001426,0.0001557,0.0001656,0.0002269,0.0005395,0.001076,0.002034,0.00322,0.004616,0.006528,0.009202,0.01283,0.01707,0.02125,0.0251,0.02847,0.03118,0.03325,0.03486,0.03625,0.03757,0.03869,0.03937,0.03946,0.03892,0.03782,0.03627,0.03435,0.03211,0.02967,0.0272,0.02483,0.02264,0.0207,0.01907,0.01784,0.01709,0.01686,0.0171,0.01771,0.01849,0.01916,0.01945,0.01911,0.01804,0.01627,0.01399,0.01147,0.008977,0.006728,0.004849,0.003375,0.002281,0.001504,0.0009715,0.0006179,0.0003883,0.000242,0.0001501,9.294e-05,5.768e-05,3.599e-05,2.263e-05,1.438e-05,9.234e-06,5.996e-06,3.933e-06,2.602e-06,1.731e-06,1.157e-06,7.744e-07,5.184e-07,3.466e-07,2.311e-07,1.535e-07,1.015e-07,6.677e-08,4.366e-08,2.836e-08,1.829e-08,1.171e-08,7.438e-09,4.686e-09,2.927e-09,1.812e-09,1.111e-09,6.754e-10,4.066e-10,2.424e-10,1.431e-10,8.363e-11,4.839e-11,2.771e-11,1.571e-11

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
	"2016" : "/home/users/sjmay/ttH/BabyMaker/Cert_271036-273730_13TeV_PromptReco_Collisions16_JSON.txt",
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
nPileupBins = 100

print("pileupCalc.py -i %s_%s_cert.json --inputLumiJSON %s --calcMode true --minBiasXsec %d --maxPileupBin %d --numPileupBins %d %s_%s_cert.root" % (data_name, args.tag, "pileup_latest.txt", minBiasXsec, nPileupBins, nPileupBins, data_name, args.tag))
os.system("pileupCalc.py -i %s_%s_cert.json --inputLumiJSON %s --calcMode true --minBiasXsec %d --maxPileupBin %d --numPileupBins %d %s_%s_cert.root" % (data_name, args.tag, "pileup_latest.txt", minBiasXsec, nPileupBins, nPileupBins, data_name, args.tag))

# Convert to text format
print("python ../../../pu2string.py %s_%s_cert.root" % (data_name, args.tag))
os.system("python ../../../pu2string.py %s_%s_cert.root" % (data_name, args.tag))
