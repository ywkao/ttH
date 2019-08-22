import bdt_helper
import siamese_model
import siamese_utils
import tensorflow as tf
import tensorflow.keras as keras
import numpy
import time

plot_dict = {
        0 : { "color" : "black" , "label" : "ttH"},
        7 : { "color" : "red", "label" : r"V + $\gamma$"},
        2 : { "color" : "blue", "label" : r"$\gamma\gamma$ + jets"},
        3 : { "color" : "cyan", "label" : r"$\gamma$ + jets"},
        5 : { "color" : "green", "label" : r"$t\bar{t} + \gamma\gamma$"},
        6 : { "color" : "lime", "label" : r"$t\bar{t} + \gamma$ + jets"},
        9 : { "color" : "gold", "label" : r"$t\bar{t}$ + jets"}
}

probs_yield = {
    0 : -1,
    7 : 0.45,
    2 : 0.14,
    3 : 0.19,
    5 : 0.03,
    6 : 0.09,
    9 : 0.04
}

probs = {
    0 : -1,
    7 : 0.3,
    2 : 0.2,
    3 : 0.1,
    5 : 0.7,
    6 : 0.5,
    9 : 0.2 
}

psum = sum([prob for prob in probs.values() if prob != -1])
for value in probs.keys():
    if probs[value] != -1:
        probs[value] *= 1./psum
print("Normalized probabilities: ", probs)

class Siamese_Helper:
    def __init__(self, **kwargs):
        self.kwargs = kwargs

        self.metadata = kwargs.get('metadata')
        self.config = self.metadata["config"]
        self.batch_size = 5000 

        self.features_train = kwargs.get('features_train', [])
        self.features_validation = kwargs.get('features_validation', [])

        self.tag = kwargs.get('tag', '')
        self.channel = kwargs.get('channel', '')

        self.metadata["weights"] = self.channel + "_" + self.tag + "_weights.hdf5"

        self.n_pairs = kwargs.get('n_pairs', 50000)

        # DNN Models
        self.n_objects = len(self.features_train["objects"][0])
        self.n_object_features = len(self.features_train["objects"][0][0])
        self.n_global_features = len(self.features_train["global"][0])

        self.model, self.shared_network = siamese_model.siamese(self.n_objects, self.n_object_features, self.n_global_features, self.config)
        self.predictor = siamese_model.base_network(self.n_objects, self.n_object_features, self.n_global_features, self.config)

        # Sampling strategy
        self.sampling_strategy = kwargs.get('sampling_strategy', 'random')

        # Bookkeeping for pair creation
        self.values = numpy.unique(self.features_train["process_id"])
        self.index_dict = {}
        for value in self.values:
            self.index_dict[value] = numpy.where(self.features_train["process_id"] == value)[0]


    def pick_pairs(self):
        pairs = []
        labels = []

        # Get similar pairs
        start_time = time.time()
        for value in self.index_dict.keys():
            if probs[value] == -1: # signal
                n_pair = int(self.n_pairs/4)
            else:
                n_pair = int((self.n_pairs/4)*probs[value])
            print("Process id: %d, number of similar pairs: %d" % (value, n_pair))
            for i in range(n_pair):
                pair = numpy.random.randint(0, len(self.index_dict[value]), 2)
                #if hasattr(self, 'prediction_train'): 
                #    distance = numpy.linalg.norm((self.prediction_train[self.index_dict[value][pair[0]]] - self.prediction_train[self.index_dict[value][pair[1]]]))
                #    if distance < 1:
                #        continue

                pairs.append([self.index_dict[value][pair[0]], self.index_dict[value][pair[1]]])
                labels.append(1)
        elapsed_time = time.time() - start_time
        print("Time to make %d similar pairs %.2f" % (int(self.n_pairs/2), elapsed_time))

        # Get dissimilar pairs
        start_time = time.time()
        for value in self.values: 
            if probs[value] == -1: # signal
                n_pair = int(self.n_pairs/4)
                trimmed_array = self.values[self.values != value]
            else:
                n_pair = int((self.n_pairs/4)*probs[value])
                trimmed_array = self.values[self.values != value]
                trimmed_array = trimmed_array[trimmed_array != 0]
            new_probs = siamese_utils.scale_probabilities([probs[val] for val in trimmed_array])
            print("Value %d, sampling other values with probability:" % value, new_probs)
            print("Process id: %d, number of dissimilar pairs: %d" % (value, n_pair))
            for i in range(n_pair):
                idx1 = numpy.random.randint(0, len(self.index_dict[value]))
                value2 = numpy.random.choice(trimmed_array, 1, p = new_probs)[0]
                idx2 = numpy.random.randint(0, len(self.index_dict[value2]))
                pairs.append([self.index_dict[value][idx1], self.index_dict[value2][idx2]])
                labels.append(0)
        elapsed_time = time.time() - start_time
        print("Time to make %d dissimilar pairs %.2f" % (int(self.n_pairs/2), elapsed_time)) 
        self.pairs = numpy.asarray(pairs)
        self.siamese_labels_train = numpy.asarray(labels)

        return

    def create_pairs(self):
        #pairs, self.siamese_labels_train = siamese_utils.create_pairs_by_strategy(self, probs)
        #pairs, self.siamese_labels_train = siamese_utils.create_pairs_by_strategy(self.sampling_strategy, self.features_train["process_id"], self.n_pairs, probs)
        self.pairs, self.siamese_labels_train = siamese_utils.create_pairs_with_probability(self.features_train["process_id"], self.n_pairs, probs)
        return

    def set_pairs(self):
        self.siamese_features_train = [self.features_train["global"][self.pairs[:,0]], self.features_train["objects"][self.pairs[:,0]], self.features_train["global"][self.pairs[:,1]], self.features_train["objects"][self.pairs[:,1]]]

        return

    def compute_distances(self):
        self.distances = numpy.array([numpy.linalg.norm((self.prediction_train[self.pairs[i,0]] - self.prediction_train[self.pairs[i,1]])) for i in range(len(self.pairs))])

        return

    def subsample_contributing_pairs(self):
        self.compute_distances()
        # Select only pairs that contribute to the loss
        positive = numpy.where((self.siamese_labels_train == 1) & (self.distances > 1))[0]
        negative = numpy.where((self.siamese_labels_train == 0) & (self.distances < 1))[0]
        print("Keeping %d positive and %d negative pairs" % (len(positive), len(negative)))

        keep = numpy.concatenate((positive, negative))
        self.pairs = self.pairs[keep]
        self.siamese_labels_train = self.siamese_labels_train[keep]

        self.set_pairs()

        return

    def train(self):
        self.model.fit(self.siamese_features_train, self.siamese_labels_train, epochs = 1, batch_size = self.config["start_batch"])
        self.shared_network.save_weights(self.metadata["weights"])

        return

    def predict(self):
        self.predictor.load_weights(self.metadata["weights"])
        self.prediction_train = self.predictor.predict([self.features_train["global"], self.features_train["objects"]], batch_size = self.batch_size)
        self.prediction_validation = self.predictor.predict([self.features_validation["global"], self.features_validation["objects"]], batch_size = self.batch_size)

        return

    def evaluate(self):
        self.bdt = bdt_helper.BDT_Helper(x_train = self.prediction_train, x_validation = self.prediction_validation, y_train = self.features_train["label"], y_validation = self.features_validation["label"], weights_train = self.features_train["weights"], weights_validation = self.features_validation["weights"], metadata = self.metadata)
        self.bdt.train()
        self.bdt.evaluate()

        return

    def do_diagnostics(self):
        import matplotlib
        matplotlib.use('Agg')
        import matplotlib.pyplot as plt

        cap = 2000

        fig = plt.figure()
        ax = fig.add_subplot(1, 1, 1)

        x, y = self.prediction_validation[:,0], self.prediction_validation[:,1]
        for pid in numpy.unique(self.features_validation["process_id"]):
            p = numpy.random.choice(numpy.where(self.features_validation["process_id"] == pid)[0], cap, replace=True)
            ax.scatter(x[p], y[p], c = plot_dict[pid]["color"], label = plot_dict[pid]["label"], s=3, edgecolors='none')

        plt.legend(markerscale=3)
        plt.savefig('siamese_scatter_%s_%s.pdf' % (self.channel, self.tag))
        plt.clf()

        return

