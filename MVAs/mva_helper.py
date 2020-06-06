import os, sys
import numpy
import h5py
import xgboost
import pandas
import copy
import root_numpy

import numpy.lib.recfunctions as rfn

import utils
import tmva_utils

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

### Data loading util functions ###

default_branches = ["evt_weight_", "process_id_", "mass_", "evt_", "run_", "lumi_", "year_", "tth_runII_mva_", "signal_mass_category_", "rand_", "signal_mass_label_"]

def load_data(self):
    self.file = h5py.File(self.input, "r")
    print("[MVA_HELPER] INPUT FILE: %s" % self.input)
    print("[MVA_HELPER] INPUT FILE KEYS: ", self.file.keys())

    self.data = {}
    self.data["feature_names"] = utils.load_array(self.file, "feature_names")
    for set in ["train", "test", "data"]:
        self.data[set] = {}
        for aux in default_branches:
            self.data[set][aux]   = utils.load_array(self.file, "%s_%s" % (aux, set))
        self.data[set]["label"]   = utils.load_array(self.file, "%s_%s" % ("label", set))
        self.data[set]["global"]  = utils.load_array(self.file, "%s_%s" % ("global", set))
        self.data[set]["objects"] = utils.load_array(self.file, "%s_%s" % ("objects", set))

        self.data[set]["n_events_raw"] = len(self.data[set]["label"])
        for check in default_branches + ["global"]:
            if not len(self.data[set][check]) == self.data[set]["n_events_raw"]:
                print("[MVA_HELPER] WARNING -- %s set: entry %s does not have the same number of events as the label (%d vs. %d)" % (set, check, len(self.data[set][check]), self.data[set]["n_events_raw"]))
        self.data[set]["n_events"]     = numpy.sum(self.data[set]["evt_weight_"])

        print("[MVA_HELPER] LOADING DATA -- %s set: %d raw events with a normalization of %.3f" % (set, self.data[set]["n_events_raw"], self.data[set]["n_events"]))

    self.data["train"]["train_id"] = numpy.zeros(len(self.data["train"]["label"]))
    self.data["test"]["train_id"] = numpy.ones(len(self.data["test"]["label"]))
    self.data["data"]["train_id"] = numpy.ones(len(self.data["data"]["label"]))

def scale_pos_weights(self):
    for set in ["train"]:
        self.n_pos = len(self.data[set]["evt_weight_"][self.data[set]["label"] == 1])
        self.n_neg = len(self.data[set]["evt_weight_"][self.data[set]["label"] == 0])

        self.sum_pos_weight = numpy.sum(self.data[set]["evt_weight_"][self.data[set]["label"] == 1])
        self.sum_neg_weight = numpy.sum(self.data[set]["evt_weight_"][self.data[set]["label"] == 0])

        print("[MVA_HELPER] LOADING DATA -- %s set: %d positive entries with normalization of %.3f" % (set, self.n_pos, self.sum_pos_weight))
        print("[MVA_HELPER] LOADING DATA -- %s set: %d negative entries with normalization of %.3f" % (set, self.n_neg, self.sum_neg_weight))


    if hasattr(self, "config"):
        self.config["scale_pos_weight"] = self.sum_neg_weight / self.sum_pos_weight

def load_data_xgb(self):
    load_data(self)
    scale_pos_weights(self)

    for set in ["train", "test", "data"]:
        self.data[set]["X"] = pandas.DataFrame(data = self.data[set]["global"], columns = self.data["feature_names"])
        self.data[set]["DMatrix"] = xgboost.DMatrix(self.data[set]["X"], self.data[set]["label"], weight = self.data[set]["evt_weight_"])


# MVA Manager Class

class MVA_Manager():
    def __init__(self, **kwargs):
        self.input = kwargs.get('input')
        self.output = kwargs.get('output')

        self.mvas = {}
        self.mva_helpers = {}
        self.data = {}

        load_data_xgb(self)

    def add_mva_helper(self, mva_helper, name):
        self.mva_helpers[name] = { 
            "mva_helper" : mva_helper,
        }

        self.mva_helpers[name]["prediction"] = mva_helper.predict(self.data)       
 
    def append_arrays(self, name):
        return [y for x in [self.data["train"][name], self.data["test"][name], self.data["data"][name]] for y in x]

    def create_structured_array(self, array, name):
        return numpy.array(array, dtype = [(name, numpy.float32)])

    def write_tree(self):
        array = []

        all_branches = default_branches + ["label", "train_id"] 
        for aux in all_branches:
            print aux
            a = self.append_arrays(aux)
            if aux == "label" or aux == "train_id":
                tree_name = aux
            elif "evt_weight" in aux:
                tree_name = "weight"
            else:
                tree_name = aux[:-1]
            a_ = self.create_structured_array(a, tree_name)
            array.append(a_)

        for mva in self.mva_helpers.keys():
            a = [y for x in [self.mva_helpers[mva]["prediction"]["train"], self.mva_helpers[mva]["prediction"]["test"], self.mva_helpers[mva]["prediction"]["data"]] for y in x]
            a_ = self.create_structured_array(a, mva)
            array.append(a_)

        merged_array = rfn.merge_arrays(array, flatten = True, usemask = False)

        self.output_root = self.output + ".root"
        os.system("rm %s" % self.output_root)
        root_numpy.array2root(merged_array, self.output_root, treename="t")
 

# MVA Helper virtual class

class MVA_Helper(object):
    def __init__(self, **kwargs):
        self.input = kwargs.get('input')
        self.output = kwargs.get('output', 'default')

        self.config = kwargs.get('config', {})
        self.prediction = {}
        self.friends = {}
        self.mvas = {}

    def make_plots(self):
        for type in ["train", "test"]:
            fig = plt.figure()
            ax1 = fig.add_subplot(111)
            ax1.yaxis.set_ticks_position('both')
            ax1.grid(True)
            ax1.plot(self.performance[type]["fpr"], 
                     self.performance[type]["tpr"], 
                     color = "red", 
                     label = "BDT AUC: %.2f +/- %.2f" % (self.performance[type]["auc"], self.performance[type]["auc_unc"]))
            ax1.fill_between(self.performance[type]["fpr"],
                             self.performance[type]["tpr"] - (self.performance[type]["tpr_unc"]/2.),
                             self.performance[type]["tpr"] + (self.performance[type]["tpr_unc"]/2.),
                             color = "red",
                             alpha = 0.25, label = r'$\pm 1\sigma')
            
            plt.xlim([-0.05,1.05])
            plt.ylim([-0.05,1.05])
            plt.xlabel("False Positive Rate")
            plt.ylabel("True Positive Rate")
            plt.legend(loc = "lower right")
            plt.savefig("roc_comparison_%s_%s.pdf" % (self.output, type))
            plt.clf()

    def save_performance(self):
        numpy.savez("roc_%s.npz" % (self.output), 
            fpr_train = self.performance["train"]["fpr"],
            tpr_train = self.performance["train"]["tpr"],
            tpr_unc_train = self.performance["train"]["tpr_unc"],
            y_train = self.data["train"]["label"],
            pred_train = self.prediction["train"],
            fpr_test = self.performance["test"]["fpr"],
            tpr_test = self.performance["test"]["tpr"],
            tpr_unc_test = self.performance["test"]["tpr_unc"],
            y_test = self.data["test"]["label"],
            pred_test = self.prediction["test"],
        ) 

    def evaluate_performance(self):
        self.performance = {}
        for set in ["train", "test"]:
            auc, unc, fpr, tpr, tpr_unc = utils.performance_metrics(self.data[set]["label"], self.prediction[set], self.data[set]["evt_weight_"], 10)
            #auc, unc, fpr, tpr, thresh = utils.auc_and_unc(self.data[set]["label"], self.prediction[set], self.data[set]["evt_weight_"], 10)
            self.performance[set] = {
                "auc" : auc,
                "auc_unc" : unc,
                "fpr" : fpr,
                "tpr" : tpr,
                "tpr_unc" : tpr_unc,
            }
            print("[MVA_HELPER] ASSESSING PERFORMANCE -- %s set: AUC = %.3f +/- %.3f" % (set, self.performance[set]["auc"], self.performance[set]["auc_unc"]))
        self.make_plots()
        self.save_performance()

    def load_weights(self, weight_file):
        self.weight_file = weight_file


xgb_default_param = {
    'max_depth': 4,
    'eta': 0.2,
    'objective': 'binary:logistic',
    'subsample': 1.0,
    'colsample_bytree': 1.0,
    'nthread' : 12,
}

# BDT Helper derived class

class BDT_Helper(MVA_Helper):
    def __init__(self, **kwargs):
        super(BDT_Helper, self).__init__(**kwargs)
        
        self.config = kwargs.get('config', xgb_default_param.copy())
        self.n_round = kwargs.get('n_round', 100)
        
        load_data_xgb(self)

    def load_weights(self, weight_file):
        self.bdt = xgboost.Booster()
        self.bdt.load_model(weight_file)

    def train(self):
        eval_list = [(self.data["train"]["DMatrix"], "train"), (self.data["test"]["DMatrix"], "test")]
        progress = {}

        self.bdt = xgboost.train(self.config, self.data["train"]["DMatrix"], self.n_round, eval_list, evals_result = progress)

    def predict(self, data = None):
        self.prediction = {}
        
        if not data:
            data = self.data

        for set in ["train", "test", "data"]:
            self.prediction[set] = self.bdt.predict(data[set]["DMatrix"])

        return self.prediction
        

    def save_model(self):
        self.bdt.save_model(self.output + "_bdt.xgb")
        
        input_variables = []
        for name in self.data["feature_names"]:
            input_variables.append((name, 'F'))
        tmva_utils.convert_model(self.bdt.get_dump(), input_variables = input_variables, output_xml = self.output + "_bdt.xml")


