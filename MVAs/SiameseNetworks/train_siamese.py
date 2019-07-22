import os, sys
import tensorflow as tf
sys.path.append("../")
import utils
print("GPU with lowest memory %s" % str(utils.pick_gpu_lowest_memory()))
os.environ["CUDA_VISIBLE_DEVICES"] = str(utils.pick_gpu_lowest_memory())
print((tf.__version__))
config = tf.ConfigProto(log_device_placement=True)
config.gpu_options.allow_growth = True
session = tf.Session(config=config)

print(session)

import numpy
import sys
from sklearn import metrics
import h5py
import glob

import siamese_utils
import siamese_model

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--tag", help = "save name for weights file", type=str)
args = parser.parse_args()

f = h5py.File(args.input, "r")

object_features, object_features_validation, object_features_data = f['object'], f['object_validation'], f['object_data']
global_features, global_features_validation, global_features_data = f['global'], f['global_validation'], f['global_data']
label, label_validation, label_data = f['label'], f['label_validation'], f['label_data']
process_id, process_id_validation, process_id_data = f['process_id'], f['process_id_validation'], f['process_id_data']
weights, weights_validation, weights_data = f['weights'], f['weights_validation'], f['weights_data']
mass, mass_validation, mass_data = f['mass'], f['mass_validation'], f['mass_data']

global_features = numpy.asarray(global_features)
object_features = numpy.asarray(object_features)
label = numpy.asarray(label)
global_features_validation = numpy.asarray(global_features_validation)
object_features_validation = numpy.asarray(object_features_validation)
label_validation = numpy.asarray(label_validation)

process_id_validation = numpy.array(process_id_validation)
cap = 10000

print(len(process_id_validation))

p1 = numpy.random.choice(numpy.where(process_id_validation == 0)[0], cap, replace=False)
p2 = numpy.random.choice(numpy.where(process_id_validation == 7)[0], cap, replace=False)

p = numpy.concatenate((p1, p2))

n_pairs = 10000000
pairs, labels = siamese_utils.create_pairs(process_id, n_pairs)
pairs_val, labels_val = siamese_utils.create_pairs(process_id_validation, n_pairs)

print(numpy.mean(labels))

config = {"n_nodes_dense_1" : 300, "n_nodes_dense_2" : 200, "n_dense_1" : 2, "n_dense_2" : 2, "n_nodes_lstm" : 150, "n_lstm" : 1, "maxnorm" : 10, "dropout_rate" : 0.2, "learning_rate" : 0.0001, "start_batch" : 2048, "batch_norm" : True, "batch_momentum" : 0.99, "layer_norm" : False, "n_manifold" : 2}
model, shared_network = siamese_model.siamese(len(object_features[0]), len(object_features[0][0]), len(global_features[0]), config)

n_epoch = 1
model.fit([global_features[pairs[:,0]], object_features[pairs[:,0]], global_features[pairs[:,1]], object_features[pairs[:,1]]], labels, epochs = n_epoch, batch_size = config["start_batch"], validation_data = ([global_features_validation[pairs_val[:,0]], object_features_validation[pairs_val[:,0]], global_features_validation[pairs_val[:,1]], object_features_validation[pairs_val[:,1]]], labels_val))
shared_network.save_weights("siamese_weights.hdf5")

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

predictor = siamese_model.base_network(len(object_features[0]), len(object_features[0][0]), len(global_features[0]), config)
predictor.load_weights("siamese_weights.hdf5")
cap = 5000

#p = numpy.random.permutation(len(label_validation))[:cap]

#object_features_validation = object_features_validation[p]
#global_features_validation = global_features_validation[p]
#label_validation = label_validation[p]
prediction = predictor.predict([global_features_validation, object_features_validation])

print(prediction)
x, y = prediction[:,0], prediction[:,1]

plot_dict = {
        0 : { "color" : "blue" , "label" : "ttH", "x" : x[p1], "y" : y[p1] },
        7 : { "color" : "red", "label" : r"V + $\gamma$", "x" : x[p2], "y" : y[p2] }
}

fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)

for pid in numpy.unique(process_id_validation):
    ax.scatter(plot_dict[pid]["x"], plot_dict[pid]["y"], c = plot_dict[pid]["color"], label = plot_dict[pid]["label"], s=3, edgecolors='none')

plt.legend()
plt.savefig('siamese_scatter.pdf')
