import os, sys
import json

sys.path.append("../Loopers/")
import parallel_utils

import argparse
parser = argparse.ArgumentParser()

parser.add_argument("--Hut_Leptonic", help = "final fit tree for Hut Leptonic", type=str)
parser.add_argument("--Hct_Leptonic", help = "final fit tree for Hct Leptonic", type=str)
parser.add_argument("--Hut_Hadronic", help = "final fit tree for Hut Hadronic", type=str)
parser.add_argument("--Hct_Hadronic", help = "final fit tree for Hct Hadronic", type=str)
parser.add_argument("--tag", help = "tag for this optimization", type=str)

parser.add_argument("--nCores", help = "number of cores to use", type=int, default = 12)
parser.add_argument("--nPoints", help = "number of different cuts to try", type=int, default = 100)
parser.add_argument("--nBins", help = "number of bins", type=int, default = 2)

args = parser.parse_args()

job_dict = {
        "Hut_Leptonic" : { "file" : args.Hut_Leptonic, "tag" : "Hut_Leptonic_%s" % args.tag, "channel" : "FCNCLeptonicTag", "resonant_bkgs" : "ttH,tH", "signal" : "FCNC_Hut" },
        "Hct_Leptonic" : { "file" : args.Hct_Leptonic, "tag" : "Hct_Leptonic_%s" % args.tag, "channel" : "FCNCLeptonicTag", "resonant_bkgs" : "ttH,tH", "signal" : "FCNC_Hct" },
        "Hut_Hadronic" : { "file" : args.Hut_Hadronic, "tag" : "Hut_Hadronic_%s" % args.tag, "channel" : "FCNCHadronicTag", "resonant_bkgs" : "ttH,tH,ggH", "signal" : "FCNC_Hut" },
        "Hct_Hadronic" : { "file" : args.Hct_Hadronic, "tag" : "Hct_Hadronic_%s" % args.tag, "channel" : "FCNCHadronicTag", "resonant_bkgs" : "ttH,tH,ggH", "signal" : "FCNC_Hct" }
}

# Run optimization for each channel and each coupling
for job in job_dict.keys():
    parallel_utils.run('python derive_binning.py --tag "%s" --channel "%s" --file "%s" --nCores %d --nPoints %d --nBins %d --resonant_bkgs "%s" --signal "%s" --limit' % (job_dict[job]["tag"], job_dict[job]["channel"], job_dict[job]["file"], args.nCores, args.nPoints, args.nBins, job_dict[job]["resonant_bkgs"], job_dict[job]["signal"]))

# For each coupling, find best binning for each channel and combine cards to get expected limit
for coupling in ["Hut", "Hct"]:
    for channel in ["Hadronic", "Leptonic"]:
        with open("results_%s.json" % job_dict[coupling + "_" + channel]["tag"], "r") as f_in:
            results = json.load(f_in)
            best_combination = None
            for combination in results.keys():
                if not best_combination or results[combination]["limit"] < best_combination["limit"]:
                    best_combination = results[combination].copy()
            job_dict[coupling + "_" + channel]["best_combination"] = best_combination

#assumed_br = 0.1441
assumed_br = 1. # already factored in upstream in loopers (also have yields divided by 100)

for coupling in ["Hut", "Hct"]:
    combined_card = "CMS-HGG_mva_13TeV_datacard_FCNC_" + coupling + "_" + args.tag + ".txt"
    parallel_utils.run('combineCards.py %s %s > %s' % (job_dict[coupling + "_Leptonic"]["best_combination"]["datacard"], job_dict[coupling + "_Hadronic"]["best_combination"]["datacard"], combined_card))

    limit_txtfile = "limit_%s.txt" % (coupling + "_" + args.tag)
    parallel_utils.run('combine -M AsymptoticLimits -m 125 %s -t -1 > %s' % (combined_card, limit_txtfile))
    limit = os.popen('grep "Expected 50.0" %s | awk "{print $2}"' % limit_txtfile).read().split(" ")[-1]
    
    print "Limit for %s: BR < %.6f at 95 percent CL" % (coupling, float(limit)*assumed_br)


with open("binning_scan_results_%s.json" % args.tag, "w") as f_out:
    json.dump(job_dict, f_out, indent=4, sort_keys=True)
