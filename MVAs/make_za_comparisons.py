import numpy

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--inputs", help = "csv list of which .npz files to consider", type=str)
parser.add_argument("--labels", help = "csv list of labels for each .npz file", type=str)
parser.add_argument("--ylim", help = "csv list of ylim", type=str)
parser.add_argument("--tag", help = "tag to denote with", type=str)
args = parser.parse_args()

print "------------------------------------------------------------"
print "Hello World! (from MVAs/make_za_comparisons.py)"
print "------------------------------------------------------------"
print "args.inputs               : %s" % args.inputs
print "args.labels               : %s" % args.labels
print "args.ylim                 : %s" % args.ylim
print "args.tag                 : %s" % args.tag
print "------------------------------------------------------------"

inputs = (args.inputs.replace(" ","")).split(",")
labels = args.labels.split(",")

colors = ["black", "red", "blue", "green", "orange"]

files = []
for input in inputs:
  print input
  files.append(numpy.load(input))

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

fig = plt.figure()
ax1 = fig.add_subplot(111)

for i in range(len(files)):
  print("MC")
  print(("Max ZA", numpy.max(files[i]["za"]), " +/- ", files[i]["za_unc"][numpy.argmax(files[i]["za"])]))
  print(("50 sig eff ZA", files[i]["za"][len(files[i]["za"])/2], " +/- ", files[i]["za_unc"][len(files[i]["za"])/2]))
  ax1.plot(files[i]["signal"], files[i]["za"], label = labels[i], color = colors[i])
  ax1.fill_between(files[i]["signal"], files[i]["za"] - files[i]["za_unc"], files[i]["za"] + files[i]["za_unc"], color = colors[i], alpha = 0.25)
  plt.xlabel('# Signal Events')
  plt.gca().set_xlim(left=2)
  ax1.set_ylabel('Significance (Z_A)')
  ax1.legend(loc='lower left')
  

if args.ylim:
  ylim = args.ylim.split(",")
  plt.ylim([float(ylim[0]), float(ylim[1])])

plt.savefig('za_comparison_mc_%s.pdf' % (args.tag))

fig = plt.figure()
ax1 = fig.add_subplot(111)

for i in range(len(files)):
  print("Data")
  print((numpy.max(files[i]["za_data"]), " +/- ", files[i]["za_unc_data"][numpy.argmax(files[i]["za_data"])]))
  print files[i]["za_data"]
  print((files[i]["za_data"][len(files[i]["za_data"])/2], " +/- ", files[i]["za_unc_data"][len(files[i]["za_data"])/2]))
  ax1.plot(files[i]["signal_data"], files[i]["za_data"], label = labels[i], color = colors[i])
  ax1.fill_between(files[i]["signal_data"], files[i]["za_data"] - files[i]["za_unc_data"], files[i]["za_data"] + files[i]["za_unc_data"], color = colors[i], alpha = 0.25)
  plt.xlabel('# Signal Events')
  plt.gca().set_xlim(left=2.5)
  ax1.set_ylabel('Significance (Z_A)')
  ax1.legend(loc='lower left')

if args.ylim:
  ylim = args.ylim.split(",")
  plt.ylim([float(ylim[0]), float(ylim[1])])

plt.savefig('za_comparison_data_%s.pdf' % (args.tag))
