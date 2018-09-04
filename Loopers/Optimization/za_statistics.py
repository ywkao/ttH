import numpy

def find_nearest(array,value):
    val = numpy.ones_like(array)*value
    idx = (numpy.abs(array-val)).argmin()
    return array[idx], idx

max_za_data = []
max_za_mc = []
mean_za_data = []
mean_za_mc = []
Nmax_za_data = []
Nmax_za_mc = []

N = 5 

n_trainings = 1000
for i in range(n_trainings):
  file = numpy.load("ZA_curves/MVAOptimizationBaby_Leptonic_rand_baseline_%d_bdt.npz" % i)
  value, start_idx = find_nearest(file["n_sig_mc"], 2.5)
  za_data = file["za_data"] # for data, we already require at least 4 data events in sideband to trust Z_A estimate
  za_mc = file["za_mc"][start_idx:] # for mc, require at least 2.5 signal events to trust Z_A estimate
  za_data.sort()
  za_mc.sort()

  max_za_data.append(numpy.max(za_data))
  max_za_mc.append(numpy.max(za_mc))
  mean_za_data.append(numpy.mean(za_data))
  mean_za_mc.append(numpy.mean(za_mc))
  Nmax_za_data.append(numpy.mean(za_data[-N:]))
  Nmax_za_mc.append(numpy.mean(za_mc[-N:]))

print "Data:"
print(numpy.std(max_za_data) / numpy.mean(max_za_data))
print(numpy.std(Nmax_za_data) / numpy.mean(Nmax_za_data))
print(numpy.std(mean_za_data) / numpy.mean(mean_za_data))

print "MC: "
print(numpy.std(max_za_mc) / numpy.mean(max_za_mc))
print(numpy.std(Nmax_za_mc) / numpy.mean(Nmax_za_mc))
print(numpy.std(mean_za_mc) / numpy.mean(mean_za_mc))

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

fig = plt.figure
plt.hist(mean_za_data, bins = numpy.linspace(2, 4, 40))
plt.savefig('hist.pdf')
