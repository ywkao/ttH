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
colors = ['red', 'blue', 'green', 'grey', "magenta"]
linestyles = ["solid", "dotted", "dashed", "dashdot", "solid"]

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
            best = numpy.array(results[dim][bin][strat]["sample_best"])
            for k in range(len(best)):
                if k == 0:
                    continue
                for j in range(k):
                    if best[j] < best[k]:
                        best[k] = best[j]
            linestyle = 'solid' if strat == "guided" else 'dotted'

            ax.plot(1+numpy.arange(len(mean)), mean, color = colors[i], linestyle = linestyle, label = 'Mean sampled value (%s sampling)' % (strat))
            ax.fill_between(1+numpy.arange(len(mean)), mean - (0.5*std), mean + (0.5*std), color = colors[i], alpha = 0.25)
            ax.plot(1+numpy.arange(len(mean)), best, color = 'black', linestyle = linestyle, label = 'Best limit (%s sampling)' % strat)
            print dim, bin, strat, best
        plt.xlabel('Optimization epoch')
        plt.ylabel('Mean sampled limit')
        plt.ylim([1.0, 3.0])
        plt.legend()
        plt.savefig('sr_opt_meanlim_%s_%sbin.pdf' % (dim, str(bin)))
        plt.close(fig)
        i += 1
        if i >= 5:
            i = 0


# dnn accuracy vs. eff of sampled points

baseline_effs = {
        "1d" : { "1" : 0.692, "2" : 0.423, "3" : 0.210, "4" : 0.093, "5" : 0.034 },
        "2d" : { "1" : 0.479, "2" : 0.179, "3" : 0.044, "4" : 0.0087, "5" : 0.0011 },
        }

fig = plt.figure()
ax1 = plt.subplot(111)

plt.yscale('log')

ax1.set_xlabel('Optimization epoch')
ax1.set_ylabel('MVA Accuracy', color = 'tab:red')
ax1.tick_params(axis='y', labelcolor = 'tab:red')
#ax1.set_scale('log')

ax2 = ax1.twinx()

ax2.set_ylabel('Eff. of sampled points', color = 'tab:blue')
ax2.tick_params(axis = 'y', labelcolor = 'tab:blue')
#ax2.set_scale('log')

plt.yscale('log')

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

        i += 1
        if i >= 5:
            i = 0

plt.legend()
plt.savefig('sr_opt_dnn_acc_and_eff.pdf')
plt.close(fig)

# limit plots
assumed_br = 0.1441

import ROOT
import tdrStyle
tdrStyle.setTDRStyle()

coupling = "Hut"
ylabel = { "Hut" : "BF (t #rightarrow Hu)", "Hct" : "BF (t #rightarrow Hc)" }
c1 = ROOT.TCanvas("c1", "c1", 800, 800)
nBins = len(dim) * len(bins)

h_exp = ROOT.TH1F("h_exp", "h_exp", nBins, 0, nBins)
h_plus1sigma = ROOT.TH1F("h_plus1sigma", "h_plus1sigma", nBins, 0, nBins)
h_plus2sigma = ROOT.TH1F("h_plus2sigma", "h_plus2sigma", nBins, 0, nBins)
h_minus1sigma = ROOT.TH1F("h_minus1sigma", "h_minus1sigma", nBins, 0, nBins)
h_minus2sigma = ROOT.TH1F("h_minus2sigma", "h_minus2sigma", nBins, 0, nBins)

h_exp.SetLineColor(1)
h_exp.SetFillColor(0)
h_exp.SetLineStyle(2)
h_exp.SetLineWidth(2)

h_plus1sigma.SetLineColor(3)
h_minus1sigma.SetLineColor(3)
h_plus1sigma.SetFillColor(3)
h_minus1sigma.SetFillColor(3)

h_plus2sigma.SetLineColor(5)
h_minus2sigma.SetLineColor(5)
h_plus2sigma.SetFillColor(5)
h_minus2sigma.SetFillColor(5)

idx = 1
strat = "guided"
for dim in dims:
    for bin in bins:
        best = min(results[dim][bin][strat]["sample_best"])
        exp_lim = results[dim][bin][strat]["exp_lim"]
        for entry in exp_lim:
            if entry["exp_lim"][0] == best:
                limits = entry["exp_lim"]
                break
        h_exp.SetBinContent(idx, limits[0])
        h_exp.SetBinError(idx, 0.000000001)
        h_exp.GetXaxis().SetBinLabel(idx, "%s, %d bins" % (dim, int(bin)))

        diff_plus1sigma = abs(limits[1] - limits[0]) / 2.
        diff_minus1sigma = abs(limits[2] - limits[0]) / 2.
        diff_plus2sigma = abs(limits[3] - limits[0]) / 2.
        diff_minus2sigma = abs(limits[4] - limits[0]) / 2.

        h_plus1sigma.SetBinContent(idx, limits[0] + diff_plus1sigma)
        h_plus2sigma.SetBinContent(idx, limits[0] + diff_plus2sigma)
        h_minus1sigma.SetBinContent(idx, limits[0] - diff_minus1sigma)
        h_minus2sigma.SetBinContent(idx, limits[0] - diff_minus2sigma)

        h_plus1sigma.SetBinError(idx, diff_plus1sigma)
        h_plus2sigma.SetBinError(idx, diff_plus2sigma)
        h_minus1sigma.SetBinError(idx, diff_minus1sigma)
        h_minus2sigma.SetBinError(idx, diff_minus2sigma)

        idx += 1

h_exp.Draw("E")
for hist in [h_plus2sigma, h_minus2sigma, h_plus1sigma, h_minus1sigma]:
    hist.SetMarkerColor(0)
    hist.SetMarkerSize(0)
    hist.Draw("E2,SAME")

h_exp.Draw("SAME,E")
h_exp.SetMinimum(0.0)
h_exp.SetMaximum(4.0)


h_exp.GetYaxis().SetTitle(ylabel[coupling])
h_exp.GetYaxis().SetTitleOffset(1.23)
ROOT.gPad.RedrawAxis()

legend = ROOT.TLegend(0.16, 0.77, 0.46, 0.87)
legend.AddEntry(h_exp, "Median Expected", "l")
legend.AddEntry(h_plus1sigma, "#pm 1#sigma", "f")
legend.AddEntry(h_plus2sigma, "#pm 2#sigma", "f")
legend.SetBorderSize(0)
legend.Draw("SAME")

latex = ROOT.TLatex()
latex.SetNDC()
latex.SetTextSize(0.4*c1.GetTopMargin())
latex.SetTextFont(42)
latex.SetTextAlign(11)
latex.SetTextColor(1)
latex.DrawLatex(0.17, 0.88, "95% CL Upper Limits: #bf{" + coupling + "}")

latex.SetTextSize(0.6*c1.GetTopMargin())
latex.DrawLatex(0.12, 0.935, "#bf{CMS}")

latex.SetTextSize(0.6*c1.GetTopMargin())
latex.DrawLatex(0.215, 0.935, "#it{Preliminary}")

latex.DrawLatex(0.67, 0.935, "137 fb^{-1} (13 TeV)")

c1.SaveAs("limits_%s.pdf" % (coupling))







