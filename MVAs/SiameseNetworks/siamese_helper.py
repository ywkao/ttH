import bdt_helper
import siamese_model
import siamese_utils
import tensorflow as tf
import tensorflow.keras as keras
import numpy

class Siamese_Helper:
    def __init__(self, **kwargs):
        self.kwargs = kwargs

        self.metadata = kwargs.get('metadata')
        self.config = self.metadata["config"]
        self.batch_size = 5000 

        self.features_train = kwargs.get('features_train', [])
        self.features_validation = kwargs.get('features_validation', [])

        #for set in [self.features_train, self.features_validation]:
        #    for key, item in set.items():
        #        item = numpy.asarray(item)

        self.tag = kwargs.get('tag', '')
        self.channel = kwargs.get('channel', '')

        self.metadata["weights"] = self.channel + "_" + self.tag + "_weights.hdf5"

        self.n_pairs = kwargs.get('n_pairs', 10000000)

        # DNN Models
        self.n_objects = len(self.features_train["objects"][0])
        self.n_object_features = len(self.features_train["objects"][0][0])
        self.n_global_features = len(self.features_train["global"][0])

        self.model, self.shared_network = siamese_model.siamese(self.n_objects, self.n_object_features, self.n_global_features, self.config)
        self.predictor = siamese_model.base_network(self.n_objects, self.n_object_features, self.n_global_features, self.config)

    def create_pairs(self):
        pairs, self.siamese_labels_train = siamese_utils.create_pairs(self.features_train["process_id"], self.n_pairs)
        self.siamese_features_train = [self.features_train["global"][pairs[:,0]], self.features_train["objects"][pairs[:,0]], self.features_train["global"][pairs[:,1]], self.features_train["objects"][pairs[:,1]]]

    def train(self):
        self.model.fit(self.siamese_features_train, self.siamese_labels_train, epochs = 1, batch_size = self.config["start_batch"])
        self.shared_network.save_weights(self.metadata["weights"])

    def evaluate(self):
        self.predictor.load_weights(self.metadata["weights"])
        self.prediction_train = self.predictor.predict([self.features_train["global"], self.features_train["objects"]])
        self.prediction_validation = self.predictor.predict([self.features_validation["global"], self.features_validation["objects"]])

        self.bdt = bdt_helper.BDT_Helper(x_train = self.prediction_train, x_validation = self.prediction_validation, y_train = self.features_train["label"], y_validation = self.features_validation["label"], weights_train = self.features_train["weights"], weights_validation = self.features_validation["weights"], metadata = self.metadata)
        self.bdt.train()
        self.bdt.evaluate()

