import ROOT
import root_numpy
import numpy
from sklearn import metrics
import keras

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

import utils
import dnn_model


class DNN_Features:
  def __init__(self, **kwargs):
    self.name = kwargs.get('name', 'validation')
    if not kwargs.get('no_prep'):
      self.global_features = numpy.transpose(numpy.array(kwargs.get('global_features')))
      self.objects = utils.pad_array(kwargs.get('objects'))
    else:
      self.global_features = numpy.array(kwargs.get('global_features'))
      self.objects = numpy.array(kwargs.get('objects'))

    if 'leptons' in kwargs:
      if not kwargs.get('no_prep'):
        self.leptons = utils.pad_array(kwargs.get('leptons', []))
      else:
	self.leptons = numpy.array(kwargs.get('leptons'))
      self.features = [self.global_features, self.objects, self.leptons]
      self.channel = "Leptonic"
    else:
      self.features = [self.global_features, self.objects]
      self.channel = "Hadronic"
 
    self.label = numpy.array(kwargs.get('label', []))
    self.weights = numpy.array(kwargs.get('weights', []))

    self.references = kwargs.get('references', {}) # dictionary of reference BDT/DNN values for comparisons

class DNN_Helper:
  def __init__(self, **kwargs):
    self.kwargs = kwargs

    self.features_validation = kwargs.get('features_validation', [])
    self.features_train = kwargs.get('features_train', [])
    self.features_data = kwargs.get('features_data', [])
    self.features_final_fit = kwargs.get('features_final_fit', [])   
    self.evt_data = kwargs.get('evt_data', [])
    self.run_data = kwargs.get('run_data', [])
    self.lumi_data = kwargs.get('lumi_data', [])
 
    self.tag = kwargs.get('tag', '')

    self.batch_size = kwargs.get('batch_size', 10000)
    self.batch_size_train = kwargs.get('batch_size_train', 512)

    self.channel = self.features_validation.channel 

    self.weights_file = kwargs.get('weights_file', '')
    self.tag = kwargs.get('tag', '')

    if 'tag' in kwargs:
      self.save_name = "dnn_weights/" + self.tag + "_weights_{epoch:02d}.hdf5"
      self.callbacks = [keras.callbacks.ModelCheckpoint(self.save_name)]
    else:
      self.callbacks = []

    self.n_objects = len(self.features_validation.objects[0])
    self.n_object_features = len(self.features_validation.objects[0][0])
    self.n_global_features = len(self.features_validation.global_features[0])

    if self.channel == "Hadronic":
      self.model = dnn_model.baseline_v1(self.n_objects, self.n_object_features, self.n_global_features, False, False, 100)

    elif self.channel == "Leptonic":
      self.n_leptons = len(self.features_validation.leptons[0])
      self.n_lepton_features = len(self.features_validation.leptons[0][0])
      self.model = dnn_model.baseline_leptonic_v1(self.n_objects, self.n_object_features, self.n_leptons, self.n_lepton_features, self.n_global_features, False, False, 100)

    if self.weights_file:
      self.model.load_weights(self.weights_file)

    self.n_epochs = 0
    self.predictions = { "train" : [], "validation" : [], "data" : [], "final_fit" : []} # store predictions for most recent epoch
    self.auc = { "train" : [], "validation" : [] } # store auc and unc for all epochs
    self.auc_unc = { "train" : [], "validation" : [] }
    self.tpr = { "train" : [], "validation" : [] } # store tpr, fpr for all epochs
    self.fpr = { "train" : [], "validation" : [] } 

    self.prepped = False

  def predict(self):
    self.predictions["train"] = self.model.predict(self.features_train.features, self.batch_size).flatten()
    self.predictions["validation"] = self.model.predict(self.features_validation.features, self.batch_size).flatten()
    self.predictions["data"] = self.model.predict(self.features_data.features, self.batch_size).flatten()
    if len(self.features_final_fit) == 0:
      self.predictions["final_fit"] = []
    else:
      self.predictions["final_fit"] = self.model.predict(self.features_final_fit.features, self.batch_size).flatten()
    return [self.predictions["train"], self.predictions["validation"], self.predictions["data"], self.predictions["final_fit"]]

  def train(self, n_epochs, n_batch):
    if not self.prepped:
      sum_neg_weights = utils.sum_of_weights_v2(self.features_train.weights, self.features_train.label, 0)
      sum_pos_weights = utils.sum_of_weights_v2(self.features_train.weights, self.features_train.label, 1)
      print("Sum of weights before scaling: ", sum_pos_weights, sum_neg_weights)

      self.features_train.weights[numpy.where(self.features_train.label == 1)] *= sum_neg_weights / sum_pos_weights 
      self.prepped = True

      sum_neg_weights = utils.sum_of_weights_v2(self.features_train.weights, self.features_train.label, 0)
      sum_pos_weights = utils.sum_of_weights_v2(self.features_train.weights, self.features_train.label, 1)
      print("Sum of weights before scaling: ", sum_pos_weights, sum_neg_weights)

    for i in range(n_epochs):
      self.model.fit(self.features_train.features, self.features_train.label, epochs = 1, batch_size = self.batch_size_train, sample_weight = self.features_train.weights, callbacks = self.callbacks)
      self.n_epochs += 1
      self.predict()

      fpr_train, tpr_train, thresh_train = metrics.roc_curve(self.features_train.label, self.predictions["train"], pos_label = 1, sample_weight = self.features_train.weights)
      fpr_validation, tpr_validation, thresh_validation = metrics.roc_curve(self.features_validation.label, self.predictions["validation"], pos_label = 1, sample_weight = self.features_validation.weights)

      auc, auc_unc = utils.auc_and_unc(self.features_validation.label, self.predictions["validation"], self.features_validation.weights, 100)
      auc_train, auc_unc_train = utils.auc_and_unc(self.features_train.label, self.predictions["train"], self.features_train.weights, 100)

      print("Test AUC: %.4f +/- %.4f" % (auc, auc_unc))
      print("Train  AUC: %.4f +/- %.4f" % (auc_train, auc_unc_train))

      self.tpr["validation"].append(tpr_validation)
      self.tpr["train"].append(tpr_train)
      self.fpr["validation"].append(fpr_validation)
      self.fpr["train"].append(fpr_train)

      self.auc["validation"].append(auc)
      self.auc_unc["validation"].append(auc_unc)
      self.auc["train"].append(auc_train)
      self.auc_unc["train"].append(auc_unc_train)

      self.model.save_weights("dnn_weights/" + self.tag + "_weights_%d.hdf5" % i)
      with open("dnn_weights/" + self.tag + "_model_architecture_%d.json" % i, "w") as f_out:
	f_out.write(self.model.to_json())

    return auc_train, auc
    
  def train_with_early_stopping(self):
    best_auc = 0.5
    keep_training = True

    epochs = 1
    bad_epochs = 0
    while keep_training:
      auc_train, auc = self.train(epochs, self.batch_size_train)
      if auc > best_auc:
	best_auc = auc
	bad_epochs = 0
      else:
	if self.batch_size_train * 4 < 100000:
	  self.batch_size_train *= 4
	  bad_epochs = 0
	else:
	  bad_epochs += 1
	if bad_epochs >= 2:
          keep_training = False

    self.model.save_weights("dnn_weights/" + self.tag + "_weights.hdf5")
    with open("dnn_weights/" + self.tag + "_model_architecture.json", "w") as f_out:
      f_out.write(self.model.to_json())

    return

  def initialize_plot(self):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.yaxis.set_ticks_position('both')
    ax.grid(True)

  def make_learning_curve(self):
    x = numpy.linspace(1, self.n_epochs, num = self.n_epochs)
    numpy.savez("dnn_learning_curve_%s.npz" % self.tag, x = x, auc = self.auc["validation"], auc_unc = self.auc_unc["validation"], auc_train = self.auc["train"], auc_train_unc = self.auc_unc["train"])

    self.initialize_plot()
    plt.errorbar(x, self.auc["validation"], yerr = self.auc_unc["validation"], label = 'Validation Set', color = 'green')
    plt.errorbar(x, self.auc["train"], yerr = self.auc_unc["train"], label = "Training Set", color = 'red')
    plt.xlabel("Epoch #")
    plt.ylabel("AUC")
    plt.legend(loc = 'lower right')
    plt.savefig('learning_curve_%s.pdf' % self.tag)
    plt.clf()

  def make_comparison(self, reference):
    fpr_ref, tpr_ref, thresh_ref = metrics.roc_curve(self.features_validation.label, self.features_validation.references[reference], pos_label = 1, sample_weight = self.features_validation.weights)

    numpy.savez("dnn_rocs_%s_%s.npz" % (reference.replace(" ", "_"), self.tag), fpr_validation = self.fpr["validation"][-1], tpr_validation = self.tpr["validation"][-1], fpr_train = self.fpr["train"][-1], tpr_train = self.tpr["train"][-1], fpr_ref = fpr_ref, tpr_ref = tpr_ref) 

    self.initialize_plot()
    plt.plot(self.fpr["validation"][-1], self.tpr["validation"][-1], color = 'darkred', lw = 2, label = 'DNN [AUC = %.3f]' % self.auc["validation"][-1])
    plt.plot(fpr_ref, tpr_ref, color = 'blue', lw = 2, label = '%s [AUC = %.3f]' % (reference, metrics.auc(fpr_ref, tpr_ref, reorder = True)))
    plt.xlabel("False Positive Rate (bkg. eff.)")
    plt.ylabel("True Positive Rate (sig. eff.)")
    plt.legend(loc = 'lower right')
    plt.savefig('dnn_roc_%s_%s.pdf' % (reference.replace(" ", "_"), self.tag))

  def do_diagnostics(self):
    numpy.savez("dnn_scores_%s_.npz" % self.tag, scores_train = self.predictions["train"], scores_validation = self.predictions["validation"], scores_data = self.predictions["data"], scores_final_fit = self.predictions["final_fit"], evt_data = self.evt_data, run_data = self.run_data, lumi_data = self.lumi_data)
    self.make_learning_curve()
    for ref in self.features_validation.references.iterkeys():
      self.make_comparison(ref)

