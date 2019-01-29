import numpy
import random
from sklearn import metrics

def auc_and_unc(label, pred, sample_weight, n_bootstraps):
  fpr, tpr, thresh = metrics.roc_curve(label, pred, pos_label = 1, sample_weight = sample_weight)
  auc = metrics.auc(fpr, tpr, reorder=True)
  print "Initial AUC: ", auc

  label = numpy.asarray(label)
  pred  = numpy.asarray(pred)
  sample_weight = numpy.asarray(sample_weight)

  n_points = len(label)
  bootstrap_aucs = numpy.empty(n_bootstraps)
  for i in range(n_bootstraps):
    bootstrap_indices = numpy.random.randint(0, n_points, n_points)
    bootstrap_label = label[bootstrap_indices]
    bootstrap_pred = pred[bootstrap_indices]
    bootstrap_weights = sample_weight[bootstrap_indices]
    fpr_b, tpr_b, thresh_b = metrics.roc_curve(bootstrap_label, bootstrap_pred, pos_label = 1, sample_weight = bootstrap_weights)
    bootstrap_auc = metrics.auc(fpr_b, tpr_b, reorder=True)
    bootstrap_aucs[i] = bootstrap_auc

  unc = numpy.std(bootstrap_aucs)
  print bootstrap_aucs
  return auc, unc

def sum_of_weights(weights, label, label_index):
  sum = 0
  for i in range(len(weights)):
    if label[i] == label_index:
      #if weights[i] > 999:
      #  print weights[i]
      sum += weights[i]
  return sum

def sum_of_weights_v2(weights, label, label_index):
  weights = numpy.asarray(weights)
  return numpy.sum(weights[numpy.asarray(label) == label_index])

def shuffle(x, y, weights):
  rand = numpy.random.permutation(len(y))
  x = numpy.asarray(x)
  x = x[rand]
  y = numpy.asarray(y)
  y = y[rand]
  weights = numpy.asarray(weights)
  weights = weights[rand]
  return x, y, weights

def load_array(file, name):
  array = file[name]
  array = numpy.asarray(array)
  return array

def find_nearest(array,value):
    val = numpy.ones_like(array)*value
    idx = (numpy.abs(array-val)).argmin()
    return array[idx], idx
