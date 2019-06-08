import os, sys
import json

from bayes_opt import BayesianOptimization
from bayes_opt.observer import JSONLogger
from bayes_opt.event import Events
from bayes_opt.util import load_logs

sys.path.append("../Loopers")
import parallel_utils
import train_dnn_core

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input", help = "input hdf5 file", type=str)
parser.add_argument("--tag", help = "save name for weights file", type=str)
parser.add_argument("--channel", help = "Hadronic or Leptonic", type=str, default="Hadronic")
parser.add_argument("--no_global", help = "don't use global features", action="store_true")
parser.add_argument("--no_lstm", help = "don't use object features (lstm)", action="store_true")
parser.add_argument("--load", help = "give weights file to use as starting point", type=str)
args = parser.parse_args()


def auc(n_nodes_dense_1, n_nodes_dense_2, n_dense_1, n_dense_2, n_nodes_lstm, n_lstm, maxnorm, dropout_rate, learning_rate, start_batch):
    global idx, log, full_results

    config = {}
    config["n_nodes_dense_1"] = int(n_nodes_dense_1)
    config["n_nodes_dense_2"] = int(n_nodes_dense_2)
    config["n_dense_1"] = int(n_dense_1)
    config["n_dense_2"] = int(n_dense_2)
    config["n_nodes_lstm"] = int(n_nodes_lstm)
    config["n_lstm"] = int(n_lstm)
    config["maxnorm"] = maxnorm
    config["dropout_rate"] = dropout_rate
    config["learning_rate"] = learning_rate
    config["start_batch"] = int(start_batch)

    """
    # Check if a json already exists and if this entry has already been calculated
    found_results = False
    with open(log, "r") as f_in:
        results = json.load(f_in)
        for entry in results.iterkeys():
            if results[entry]["config"] == config:
                print "Found previous results from json file"
                full_results[idx] = entry
                found_results = True
                target = entry["results"]["auc_test"][-1]
    """
    #if not found_results:
    trained_dnn = train_dnn_core.train(args, config)
    full_results[idx] = {"config" : config, "results" : {"auc_train" : trained_dnn.auc["train"], "auc_train_unc" : trained_dnn.auc_unc["train"], "auc_test" : trained_dnn.auc["validation"], "auc_test_unc" : trained_dnn.auc_unc["validation"]}}
    target = trained_dnn.auc["validation"][-1]

    with open(log, "w") as f_out:
        json.dump(full_results, f_out, indent=4, sort_keys=True)

    idx += 1
    return target 

idx = 0
log = "bayes_dnn_hyperparam_scan_%s_%s.json" % (args.channel, args.tag)
full_results = {}

pbounds = {
    "n_nodes_dense_1" : (100, 500), 
    "n_nodes_dense_2" : (25, 200), 
    "n_dense_1" : (1,3), 
    "n_dense_2" : (1,4), 
    "n_nodes_lstm" : (25, 150), 
    "n_lstm" : (1,5), 
    "maxnorm" : (0.1, 100), 
    "dropout_rate" : (0.0, 0.5), 
    "learning_rate" : (0.00001, 0.01),
    "start_batch" : (256, 20000)
}

starting_point = {
    "n_nodes_dense_1" : 300, 
    "n_nodes_dense_2" : 200, 
    "n_dense_1" : 1, 
    "n_dense_2" : 4, 
    "n_nodes_lstm" : 100, 
    "n_lstm" : 3, 
    "maxnorm" : 3, 
    "dropout_rate" : 0.25, 
    "learning_rate" : 0.001,
    "start_batch" : 1024
}

optimizer = BayesianOptimization(
    f=auc,
    pbounds=pbounds,
    verbose=2, # verbose = 1 prints only when a maximum is observed, verbose = 0 is silent
    random_state=1,
)

official_log = "log_%s_%s.json" % (args.channel, args.tag)
logger = JSONLogger(path=official_log)
optimizer.subscribe(Events.OPTMIZATION_STEP, logger)

load_logs(optimizer, logs=[official_log])

optimizer.probe(params = starting_point, lazy = True)

optimizer.maximize(
        init_points = 1,
        n_iter = 1000,
        acq = "ei",
        xi = 0.001,
)

for i, res in enumerate(optimizer.res):
    print("Iteration {}: \n\t{}".format(i, res))
