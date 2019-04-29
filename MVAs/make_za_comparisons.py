import numpy

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--inputs", help = "csv list of which .npz files to consider", type=str)
parser.add_argument("--labels", help = "csv list of labels for each .npz file", type=str)
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

for i in range(len(files)):
  print "MC"
  print "Max ZA", numpy.max(files[i]["za"]), " +/- ", files[i]["za_unc"][numpy.argmax(files[i]["za"])]
  print "50 sig eff ZA", files[i]["za"][len(files[i]["za"])/2], " +/- ", files[i]["za_unc"][len(files[i]["za"])/2]
  ax1.plot(files[i]["signal"], files[i]["za"], label = labels[i], color = colors[i])
  ax1.fill_between(files[i]["signal"], files[i]["za"] - files[i]["za_unc"], files[i]["za"] + files[i]["za_unc"], color = colors[i], alpha = 0.25)
  plt.xlabel('# Signal Events')
  ax1.set_ylabel('Significance (Z_A)')
  ax1.legend(loc='lower left')

plt.savefig('za_comparison_mc.pdf')

fig = plt.figure()
ax1 = fig.add_subplot(111)

for i in range(len(files)):
  print "Data"
  print numpy.max(files[i]["za_data"]), " +/- ", files[i]["za_unc_data"][numpy.argmax(files[i]["za_data"])]
  print files[i]["za_data"][len(files[i]["za_data"])/2], " +/- ", files[i]["za_unc_data"][len(files[i]["za_data"])/2]
  ax1.plot(files[i]["signal_data"], files[i]["za_data"], label = labels[i], color = colors[i])
  ax1.fill_between(files[i]["signal_data"], files[i]["za_data"] - files[i]["za_unc_data"], files[i]["za_data"] + files[i]["za_unc_data"], color = colors[i], alpha = 0.25)
  plt.xlabel('# Signal Events')
  ax1.set_ylabel('Significance (Z_A)')
  ax1.legend(loc='lower left')

plt.savefig('za_comparison_data.pdf') 


