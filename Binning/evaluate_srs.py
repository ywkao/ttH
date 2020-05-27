import os, sys
import json
import numpy

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input json with results", type=str)
parser.add_argument("--dim", help = "1d, 2d, or both (csv)", type=str)
parser.add_argument("--strategy", help = "random, guided, both", type=str)
parser.add_argument("--bins", help = "1,2,3,4, or some combination", type=str)
args = parser.parse_args()

dims = args.dim.split(",")
strategies = args.strategy.split(",")
bins = args.bins.split(",")

# load data
with open(args.input, "r") as f_in:
    results = json.load(f_in)


import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

#colors = ['cyan', 'azure', 'coral', 'fuschia', 'darkblue', 'darkgreen', 'crimson', 'yellow', 'grey'] 
colors = ['red', 'blue', 'green', 'grey']
linestyles = ["solid", "dotted", "dashed", "dashdot"]

# mean value vs. epoch
fig = plt.figure()
ax = plt.subplot(111)

i = 0
for dim in dims:
    for bin in bins:
        fig = plt.figure()
        ax = plt.subplot(111)
        for strat in strategies:
            mean = numpy.array(results[dim][bin][strat]["sample_mean"])
            std = numpy.array(results[dim][bin][strat]["sample_std"])
            best = numpy.ones_like(mean) * min(results[dim][bin][strat]["sample_best"])
            linestyle = 'solid' if strat == "guided" else 'dotted'

            ax.plot(1+numpy.arange(len(mean)), mean, color = colors[i], linestyle = linestyle, label = 'Mean sampled value (%s sampling)' % (strat))
            ax.fill_between(1+numpy.arange(len(mean)), mean - (0.5*std), mean + (0.5*std), color = colors[i], alpha = 0.25)
            ax.plot(1+numpy.arange(len(mean)), best, color = 'black', linestyle = linestyle, label = 'Best limit (%s sampling)' % strat)
        plt.xlabel('Optimization epoch')
        plt.ylabel('Mean sampled limit')
        plt.legend()
        plt.savefig('sr_opt_meanlim_%s_%sbin.pdf' % (dim, str(bin)))
        plt.close(fig)
        i += 1


# dnn accuracy vs. eff of sampled points

baseline_effs = {
        "1d" : { "1" : 0.692, "2" : 0.423, "3" : 0.210, "4" : 0.093, "5" : 0.034 },
        "2d" : { "1" : 0.479, "2" : 0.179, "3" : 0.044, "4" : 0.0087, "5" : 0.0011 },
        }

fig = plt.figure()
ax1 = plt.subplot(111)

ax1.set_xlabel('Optimization epoch')
ax1.set_ylabel('MVA Accuracy', color = 'tab:red')
ax1.tick_params(axis='y', labelcolor = 'tab:red')
ax1.set_scale('log')

ax2 = ax1.twinx()

ax2.set_ylabel('Eff. of sampled points', color = 'tab:blue')
ax2.tick_params(axis = 'y', labelcolor = 'tab:blue')
ax2.set_scale('log')

i = 0
for dim in dims:
    for bin in bins:
        acc = numpy.array(results[dim][bin]["guided"]["accuracy"])
        for j in range(len(acc)):
            if acc[j] == 0:
                acc[j] = 1.
        eff = numpy.array(results[dim][bin]["guided"]["eff"]) * baseline_effs[dim][bin]
        ax1.plot(1 + numpy.arange(len(acc)), acc, color = 'red', linestyle = linestyles[i], label = '%s-Opt, %d bins' % (dim, int(bin)))
        ax2.plot(1 + numpy.arange(len(acc)), eff, color = 'blue', linestyle = linestyles[i], label = '%s-Opt, %d bins' % (dim, int(bin)))

plt.legend()
plt.savefig('sr_opt_dnn_acc_and_eff.pdf')
plt.close(fig)
