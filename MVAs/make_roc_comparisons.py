import numpy
import utils
from sklearn import metrics

import utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--inputs", help = "csv list of which .npz files to consider", type=str)
parser.add_argument("--labels", help = "csv list of labels for each .npz file", type=str)
parser.add_argument("--log", help = "log axis for roc curve", action="store_true")
parser.add_argument("--modified_tH_curve", help = "make tH stolen vs. ttH kept curve", action="store_true")
args = parser.parse_args()

inputs = (args.inputs.replace(" ","")).split(",")
labels = args.labels.split(",")

colors = ["black", "red", "blue", "green", "orange"]

files = []
for input in inputs:
  files.append(numpy.load(input))

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

fig = plt.figure()
ax1 = fig.add_subplot(111)

#signal_effs = [0.68, 0.69, 0.7, 0.71]
signal_effs = [0.25, 0.5, 0.9, 0.99]
#signal_effs = [0.2, 0.45, 0.9, 0.99]
#signal_effs = [0.95, 0.97, 0.98, 0.99]

effs = {}

for i in range(len(files)):
  if not "dnn_roc" in inputs[i]:
    fpr = files[i]["fpr_test"]
    tpr = files[i]["tpr_test"]
  else:
    fpr = files[i]["fpr_validation"]
    tpr = files[i]["tpr_validation"]

  tprs = []
  fprs = []
  for eff in signal_effs:
    sig_eff, idx = utils.find_nearest(tpr, eff)
    tprs.append(tpr[idx])
    fprs.append(fpr[idx])

  auc = metrics.auc(fpr, tpr, reorder=True)
  effs[labels[i]] = { "fpr" : fprs, "tpr" : tprs, "auc" : auc }

  ax1.plot(fpr, tpr, label = labels[i] + " [AUC = %.3f]" % (auc), color = colors[i])
  if "tpr_unc_test" in files[i].files:
      tpr_unc = files[i]["tpr_unc_test"]
      ax1.fill_between(fpr, tpr - 1*(tpr_unc/2.), tpr + 1*(tpr_unc/2.), color = colors[i], alpha = 0.25)


plt.ylim(0,1)
plt.xlim(0,1)

if args.log:
    plt.xlim(0.005, 1)
    if effs[labels[0]]["auc"] > 0.95:
        plt.xlim(0.0005, 1)
    ax1.set_xscale("log")

plt.xlabel('False Positive Rate (Background Efficiency)')
ax1.set_ylabel('True Positive Rate (Signal Efficiency)')
ax1.legend(loc='lower right')
ax1.yaxis.set_ticks_position('both')
ax1.grid(True)
plt.savefig('auc_comparison.pdf')

if args.modified_tH_curve:
    tH_yield = 1.75 
    tH_total = 2.93 
    ttH_yield = 25.44
    plt.clf()
    fig = plt.figure()
    ax1 = fig.add_subplot(111)
    for i in range(len(files)):
        if "bdt_roc" in inputs[i]:
            fpr = files[i]["fpr_test"]
            tpr = files[i]["tpr_test"]
        elif "dnn_roc" in inputs[i]:
            fpr = files[i]["fpr_validation"]
            tpr = files[i]["tpr_validation"]
        ax1.plot(tpr*ttH_yield, fpr*tH_yield, label = labels[i], color = colors[i])

    plt.xlabel('ttH Events in Leptonic SRs')
    plt.ylabel('Upper Bound on "stolen" tH events')
    ax1.yaxis.set_ticks_position('both')
    ax1.legend(loc='upper left')
    ax1.grid(True)
    plt.savefig('ttH_vs_tH_yield_comparison.pdf')

    plt.clf()
    fig = plt.figure()
    ax1 = fig.add_subplot(111)
    ttH_effs = [1.00, 0.99, 0.95, 0.80]
    tH_effs = []
    tH_yields = []
    tH_effs_bdt = []
    tH_yields_bdt = []
    for i in range(len(files)):
        if "bdt_roc" in inputs[i]:
            fpr = files[i]["fpr_test"]
            tpr = files[i]["tpr_test"]
        elif "dnn_roc" in inputs[i]:
            fpr = files[i]["fpr_validation"]
            tpr = files[i]["tpr_validation"]
        ax1.plot(tpr, fpr * (tH_yield / tH_total), label = labels[i], color = colors[i])
        for eff in ttH_effs: 
            value, idx = utils.find_nearest(tpr, eff)
            if "bdt_roc" in inputs[i]:
                tH_effs_bdt.append(fpr[idx] * (tH_yield / tH_total))
                tH_yields_bdt.append(fpr[idx] * tH_yield)
            else:
                tH_effs.append(fpr[idx] * (tH_yield / tH_total))
                tH_yields.append(fpr[idx] * tH_yield)

        #value1, idx1 = utils.find_nearest(tpr, 0.99)
        #value2, idx2 = utils.find_nearest(tpr, 0.95)
        #value3, idx3 = utils.find_nearest(tpr, 0.80)
        print(("BDT" if "bdt_roc" in inputs[i] else "DNN"))
        #print tpr[idx1], fpr[idx1]
        #print "If we want to maintain %.3f%% of original ttH SR events, we steal at most %.3f%% of tH events" % (100*value1, 100*fpr[idx1])
        #print "If we want to maintain %.3f%% of original ttH SR events, we steal at most %.3f%% of tH events" % (100*value2, 100*fpr[idx2])
        #print "If we want to maintain %.3f%% of original ttH SR events, we steal at most %.3f%% of tH events" % (100*value3, 100*fpr[idx3])


    plt.xlabel('Fraction of "saved" ttH Events in Leptonic SRs')
    plt.ylabel('Upper Bound on fraction "stolen" tH events')
    plt.xlim(0.5, 1.0)
    ax1.yaxis.set_ticks_position('both')
    ax1.legend(loc='upper left')
    ax1.grid(True)
    plt.savefig('ttH_vs_tH_eff_comparison.pdf')
    print("\\begin{center} \\Fontvi")
    print("\\begin{tabular}{|c|c|c|c|c|}")
    print("\\multicolumn{5}{c}{Comparison} \\\\ \\hline \\hline")
    print("\\multirow{2}{*}{$\\mathcal F$ ttH SR Preserved (\\%)} & \\multicolumn{2}{c|}{BDT} & \\multicolumn{2}{c|}{DNN} \\\\ \\cline{2-5}")
    print("& U.B. on stolen tH (\\%) & U.B. on stolen tH (yield)  & U.B. on stolen tH (\\%) & U.B. on stolen tH (yield) \\\\ \\hline")
    for i in range(len(ttH_effs)):
        print(("%.0f & %.1f & %.3f & %.1f & %.3f \\\\ \\hline" % (ttH_effs[i] * 100, tH_effs_bdt[i] * 100, tH_yields_bdt[i], tH_effs[i] * 100, tH_yields[i])))
    print("\\end{tabular} \\end{center}")




# Table
print("\\begin{center} \\Fontvi")
print("\\begin{tabular}{|c||c|c|c|c|c|}")
print("\\multicolumn{6}{c}{Performance Metrics} \\\\ \\hline \\hline")
print("\\multirow{3}{*}{Method} & \\multicolumn{5}{c|}{Metric (\\%)} \\\\ \\cline{2-6}")
print(" & \\multirow{2}{*}{AUC} & \\multicolumn{4}{c|}{Background efficiency at fixed signal efficiency} \\\\ \\cline{3-6}") 
print(" & & $\\epsilon_{\\text{bkg}} (\\epsilon_{\\text{sig}} = 25\\%)$ & $\\epsilon_{\\text{bkg}} (\\epsilon_{\\text{sig}} = 50\\%)$ & $\\epsilon_{\\text{bkg}} (\\epsilon_{\\text{sig}} = 90\\%)$ & $\\epsilon_{\\text{bkg}} (\\epsilon_{\\text{sig}} = 99\\%)$ \\\\ \\hline")
for method in list(effs.keys()):
  print(("%s & %.2f & %.2f & %.2f & %.2f & %.2f \\\\ \\hline" % (method, effs[method]["auc"] * 100, effs[method]["fpr"][0] * 100, effs[method]["fpr"][1] * 100, effs[method]["fpr"][2] * 100, effs[method]["fpr"][3] * 100)))

print("\\end{tabular} \\end{center}")
