import numpy
import random

def find_nearest(array,value):
    val = numpy.ones_like(array)*value
    idx = (numpy.abs(array-val)).argmin()
    return array[idx], idx

def random_sample(array, N): # randomly sample N events from array
  random_array = array
  random.shuffle(random_array)
  return random_array[:N]
   

max_za_data = []
max_za_mc = []
mean_za_data = []
mean_za_mc = []
Nmax_za_data = []
Nmax_za_mc = []

max_za_5trainings_data = []
max_za_5trainings_mc = []
max_za_25trainings_data = []
max_za_25trainings_mc = []


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


N_avg = 25
N_avg_list = numpy.linspace(1, 100, 100).astype(int)
print N_avg_list

fractional_uncertainty_data = []
fractional_uncertainty_mc = []

for i in range(n_trainings):
  avg_max_za_data = numpy.mean(random_sample(max_za_data, 5))
  avg_max_za_mc = numpy.mean(random_sample(max_za_mc, 5))
  max_za_5trainings_data.append(avg_max_za_data)
  max_za_5trainings_mc.append(avg_max_za_mc)
  avg_max_za_data = numpy.mean(random_sample(max_za_data, 25))
  avg_max_za_mc = numpy.mean(random_sample(max_za_mc, 25))
  max_za_25trainings_data.append(avg_max_za_data)
  max_za_25trainings_mc.append(avg_max_za_mc)


for n_avg in N_avg_list:
  avg_max_za_data = []
  avg_max_za_mc = []
  for i in range(n_trainings):
    avg_max_za_data.append(numpy.mean(random_sample(max_za_data, n_avg)))
    avg_max_za_mc.append(numpy.mean(random_sample(max_za_mc, n_avg)))
  fractional_uncertainty_data.append(numpy.std(avg_max_za_data) / numpy.mean(avg_max_za_data))
  fractional_uncertainty_mc.append(numpy.std(avg_max_za_mc) / numpy.mean(avg_max_za_mc))



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


vars_of_interest = { 
	"max_za_data" : [max_za_data, "Max. Z_A (data)"], 
	"max_za_mc" : [max_za_mc, "Max. Z_A (mc)"],
	"Nmax_za_data" : [Nmax_za_data, "Mean of %d highest Z_A (data)" % N],
        "Nmax_za_mc" : [Nmax_za_mc, "Mean of %d highest Z_A (mc)" % N],
	"mean_za_data" : [mean_za_data, "Mean Z_A (data)"],
        "mean_za_mc" : [mean_za_mc, "Mean Z_A (mc)"],
	"max_za_5trainings_data" : [max_za_5trainings_data, "Max. Z_A (data, mean of %d trainings)" % 5],
	"max_za_5trainings_mc" : [max_za_5trainings_mc, "Max. Z_A (mc, mean of %d trainings)" % 5],
	"max_za_25trainings_data" : [max_za_25trainings_data, "Max. Z_A (data, mean of %d trainings)" % 25],
        "max_za_25trainings_mc" : [max_za_25trainings_mc, "Max. Z_A (mc, mean of %d trainings)" % 25],
}

fig = plt.figure()
bins = numpy.linspace(2, 4, 40)
for var, info in vars_of_interest.iteritems():
  dist_to_plot = info[0]
  plt.hist(dist_to_plot, bins = bins, label = info[1])
  plt.ylabel("# of MVA Trainings")
  plt.xlabel(info[1])
  plt.legend(loc = 'upper right')
  plt.text(3.5, 100, "Mean       = %.2f" % numpy.mean(dist_to_plot))
  plt.text(3.5, 80,  "Std. Dev. = %.2f" % numpy.std(dist_to_plot))
  plt.savefig(var+'.pdf')
  plt.clf()


plt.plot(N_avg_list, fractional_uncertainty_data, 'r', label='data')
plt.plot(N_avg_list, fractional_uncertainty_mc, 'b', label='mc')
plt.ylabel(r'$\sigma(\langle$ Max $Z_A \rangle_{N})$ / $\mu(\langle$ Max $Z_A \rangle_{N})$')
plt.xlabel("N")
plt.legend(loc = 'upper right')
plt.savefig('max_za_avgN.pdf')

#plt.hist(max_za_data, bins = numpy.linspace(2, 4, 40))
#plt.savefig('max_za_data.pdf')
#plt.clf()
#plt.hist(mean_za_data, bins = numpy.linspace(2, 4, 40))
#plt.savefig('hist.pdf')
