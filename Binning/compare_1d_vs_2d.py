import os, sys
import numpy

sys.path.append("../Loopers/")
import parallel_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input final fit tree", type=str)
parser.add_argument("--nUnc", help = "number of uncertainties to sample", type=float, default=5)
parser.add_argument("--channel", help = "Hadronic or Leptonic", type=str)
parser.add_argument("--coupling", help = "Hut or Hct", type=str)
parser.add_argument("--plotOnly", help = "dont rederive binning", action="store_true")
parser.add_argument("--nPoints", help = "number of points to scan", type=str, default="10")
args = parser.parse_args()

tree = args.input 
sm_higgs_unc = numpy.linspace(0.01, 0.5, args.nUnc)

command_list = []

for unc in sm_higgs_unc:
    unc_str = str(int(unc*100))
    command_base = 'python derive_binning_Nd.py --channel "FCNC%s" --file "%s" --mvas "MVAS" --nPoints "NPOINTS" --nBins 1 --signal "FCNC_%s" --limit --tag "FCNC_%s_%s_simple_DIMd_unc%s_11May2020" --sm_higgs_unc %.6f' % (args.channel, tree, args.coupling, args.channel, args.coupling, unc_str, unc)
    
    command_1d = command_base.replace("MVAS", "mva_score").replace("NPOINTS", args.nPoints).replace("DIM", "1")
    command_2d = command_base.replace("MVAS", "mva_nonres_score,mva_smhiggs_score").replace("NPOINTS", args.nPoints).replace("DIM", "2")

    command_list.append(command_1d)
    command_list.append(command_2d)

if not args.plotOnly:
    parallel_utils.submit_jobs(command_list, 1)

import glob
import json

best_limit_1d = []
best_limit_2d = []

best_limit_1d_up = []
best_limit_2d_up = []

best_limit_1d_down = []
best_limit_2d_down = []

def get_best_limit(file):
    with open(file, "r") as f_in:
        result = json.load(f_in)
        best_lim = 999
        best_lim_up = 999
        best_lim_down = 999

        full_info = {}

        for combo, info in result.items():
            if info["limit"] < best_lim:
                if info["limit"] == 1.: # weird issue with fit...
                    continue
                best_lim = info["limit"]
                best_lim_up = info["up1sigma"]
                best_lim_down = info["down1sigma"]
                full_info = { combo : info }

    return best_lim, best_lim_up, best_lim_down, full_info

for unc in sm_higgs_unc:
    unc_str = str(int(unc*100))
    results_1d = "results_FCNC_%s_%s_simple_1d_unc%s_11May2020.json" % (args.channel, args.coupling, unc_str)
    results_2d = "results_FCNC_%s_%s_simple_2d_unc%s_11May2020.json" % (args.channel, args.coupling, unc_str)

    lim1d, lim1d_up, lim1d_down, info_1d = get_best_limit(results_1d)
    lim2d, lim2d_up, lim2d_down, info_2d = get_best_limit(results_2d)

    print "Best limit 1d", results_1d, lim1d, info_1d
    print "Best limit 2d", results_2d, lim2d, info_2d


    best_limit_1d.append(lim1d)
    best_limit_1d_up.append(lim1d_up)
    best_limit_1d_down.append(lim1d_down)

    best_limit_2d.append(lim2d)
    best_limit_2d_up.append(lim2d_up)
    best_limit_2d_down.append(lim2d_down)

best_limit = min(best_limit_1d)

best_limit_1d = numpy.array(best_limit_1d)
best_limit_1d_up = numpy.array(best_limit_1d_up)
best_limit_1d_down = numpy.array(best_limit_1d_down)
best_limit_2d = numpy.array(best_limit_2d)
best_limit_2d_up = numpy.array(best_limit_2d_up)
best_limit_2d_down = numpy.array(best_limit_2d_down)

best_limit_1d *= 1./best_limit
best_limit_1d_up *= 1./best_limit
best_limit_1d_down *= 1./best_limit
best_limit_2d *= 1./best_limit
best_limit_2d_up *= 1./best_limit
best_limit_2d_down *= 1./best_limit

unc_1d = [ best_limit_1d - best_limit_1d_down, best_limit_1d_up - best_limit_1d ]
unc_2d = [ best_limit_2d - best_limit_2d_down, best_limit_2d_up - best_limit_2d ]


#if args.channel == "Hadronic" and args.coupling == "Hut":
#    sm_higgs_unc = sm_higgs_unc[:-1]
     

print "1d", best_limit_1d
print "2d", best_limit_2d

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.plot(sm_higgs_unc, best_limit_1d, label = "1d optimization", color = "blue")
ax1.plot(sm_higgs_unc, best_limit_2d, label = "2d optimization", color = "red")

ax1.plot(sm_higgs_unc, best_limit_1d_up, label = r'$\pm 1\sigma$', color = "blue", linestyle = "dashed")
ax1.plot(sm_higgs_unc, best_limit_1d_down, color = "blue", linestyle = "dashed")
ax1.plot(sm_higgs_unc, best_limit_2d_up, color = "red", linestyle = "dashed")
ax1.plot(sm_higgs_unc, best_limit_2d_down, color = "red", linestyle = "dashed")


#ax1.fill_between(sm_higgs_unc, best_limit_1d_down, best_limit_1d_up, color = "blue", alpha = 0.25)
#ax1.fill_between(sm_higgs_unc, best_limit_2d_down, best_limit_2d_up, color = "red", alpha = 0.25)


#ax1.errorbar(sm_higgs_unc, best_limit_1d, yerr = unc_1d, label = "1d optimization", color = "blue", ls = 'none', fmt = 'o', ms = 7, capsize = 5, capthick = 1)
#ax1.errorbar(sm_higgs_unc, best_limit_2d, yerr = unc_2d, label = "2d optimization", color = "red", ls = 'none', fmt = 'o', ms = 7, capsize = 5, capthick = 1)
ax1.grid(True)

plt.legend(loc = 'upper left')
plt.xlabel('SM Higgs XS Uncertainty')
plt.ylabel('Expected Limit (Normalized)') 
plt.savefig('1d_vs_2d_optimization.pdf')
