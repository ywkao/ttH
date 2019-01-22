import numpy
import random

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
