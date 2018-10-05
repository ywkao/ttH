import os, sys
import numpy
import glob

def find_nearest(array,value):
    val = numpy.ones_like(array)*value
    idx = (numpy.abs(array-val)).argmin()
    return array[idx], idx

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("file", help = "input npz file (enter indices as wildcard)", type=str)
args = parser.parse_args()

unc_data = 0.096
unc_mc = 0.091

files = glob.glob(args.file)

max_za_data = []
max_za_mc = []
for file in files:
  data = numpy.load(file)
  value, start_idx = find_nearest(data["n_sig_mc"], 2.5)
  za_data = data["za_data"] # for data, we already require at least 4 data events in sideband to trust Z_A estimate
  za_mc = data["za_mc"][start_idx:] # for mc, require at least 2.5 signal events to trust Z_A estimate

  max_za_data.append(numpy.max(za_data))
  max_za_mc.append(numpy.max(za_mc))

print max_za_mc
print max_za_data

print "[data] Mean of max Z_A: %.3f +/- %.3f " % (numpy.mean(max_za_data), unc_data / (float(len(files)) ** 0.5))
print "[mc] Mean of max Z_A: %.3f +/- %.3f " % (numpy.mean(max_za_mc), unc_mc / (float(len(files)) ** 0.5)) 

print "[data] Std. Dev of max Z_A: %.3f " % numpy.std(max_za_data)
print "[mc] Std. Dev of max Z_A: %.3f " % numpy.std(max_za_mc)

#for za in max_za_mc:
#  print za

#print "[data] Mean/std. dev of max Z_A: %.3f, %.3f" % (numpy.mean(max_za_data), numpy.std(max_za_data))
#print "[mc] Mean/std. dev of max Z_A: %.3f, %.3f" % (numpy.mean(max_za_mc), numpy.std(max_za_mc)) 

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

vars_of_interest = {
        "max_za_data" : [max_za_data, "Max. Z_A (data)"],
        "max_za_mc" : [max_za_mc, "Max. Z_A (mc)"],
}

fig = plt.figure()
bins = numpy.linspace(1, 3, 40)
for var, info in vars_of_interest.iteritems():
  dist_to_plot = info[0]
  plt.hist(dist_to_plot, label = info[1], bins = bins)
  plt.ylabel("# of MVA Trainings")
  plt.xlabel(info[1])
  plt.xlim([1.0, 3.0])
  #plt.ylim([0.0, 30.0])
  plt.legend(loc = 'upper right')
  plt.text(2.5, 25, "Mean       = %.3f" % numpy.mean(dist_to_plot))
  plt.text(2.5, 20,  "Std. Dev. = %.3f" % numpy.std(dist_to_plot))
  print args.file.replace(".npz", "") + var +'.pdf'
  plt.savefig(args.file.replace(".npz", "") + var +'.pdf')
  plt.clf()

