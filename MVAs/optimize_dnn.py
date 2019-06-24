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
parser.add_argument("--random", help = "do random exploration instead of bayesian exploration", action="store_true")
parser.add_argument("--no_bootstrap", help = "don't use bootstrapping to estimate unc. in AUC (to save time during hyperparameter opt)", action="store_true")
parser.add_argument("--n_points", help = "how many points to probe", type=str, default="200")
args = parser.parse_args()


def auc(n_nodes_dense_1, n_nodes_dense_2, n_dense_1, n_dense_2, n_nodes_lstm, n_lstm, maxnorm, dropout_rate, learning_rate, start_batch, batch_momentum):
    global idx, log, full_results

    config = {}
    config["n_nodes_dense_1"] = int(n_nodes_dense_1)
    config["n_nodes_dense_2"] = int(n_nodes_dense_2)
    config["n_dense_1"] = int(n_dense_1)
    config["n_dense_2"] = int(n_dense_2)
    config["n_nodes_lstm"] = int(n_nodes_lstm)
    config["n_lstm"] = int(n_lstm)
    config["maxnorm"] = 10**(maxnorm)
    config["dropout_rate"] = dropout_rate
    config["learning_rate"] = 10**(learning_rate)
    config["start_batch"] = int(2**(start_batch))
    config["batch_norm"] = True
    config["layer_norm"] = True
    config["batch_momentum"] = batch_momentum

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
    "maxnorm" : (-1, 2), # 10**(maxnorm)
    "dropout_rate" : (0.0, 0.5), 
    "learning_rate" : (-6, -1), # 10**(learning_rate)
    "start_batch" : (7, 13), # 2**(start_batch)
    "batch_momentum" : (0.0, 0.9999)
}

pbounds_light = {
    "n_nodes_dense_1" : (300, 300), 
    "n_nodes_dense_2" : (200, 200), 
    "n_dense_1" : (1,1), 
    "n_dense_2" : (4,4), 
    "n_nodes_lstm" : (100, 100), 
    "n_lstm" : (3,3), 
    "maxnorm" : (0.5, 0.5), # 10**(maxnorm)
    "dropout_rate" : (0.0, 0.5), 
    "learning_rate" : (-6, -2), # 10**(learning_rate)
    "start_batch" : (10, 10), # 2**(start_batch)
    "batch_momentum" : (0.99, 0.99)
}


starting_point = {
    "n_nodes_dense_1" : 300, 
    "n_nodes_dense_2" : 200, 
    "n_dense_1" : 1, 
    "n_dense_2" : 4, 
    "n_nodes_lstm" : 100, 
    "n_lstm" : 3, 
    "maxnorm" : 0.5, 
    "dropout_rate" : 0.25, 
    "learning_rate" : -3,
    "start_batch" : 10,
    "batch_momentum" : 0.99
}

optimizer = BayesianOptimization(
    f=auc,
    pbounds=pbounds_light,
    verbose=2, # verbose = 1 prints only when a maximum is observed, verbose = 0 is silent
    random_state=1,
)

official_log = "log_%s_%s.json" % (args.channel, args.tag)

if os.path.isfile(official_log):
    load_logs(optimizer, logs=[official_log])
    with open(log) as f_in:
        full_results = json.load(f_in)
        idx += len(full_results.keys())
        print "Loaded %d previous results from log file" % len(full_results.keys())

logger = JSONLogger(path=official_log)
optimizer.subscribe(Events.OPTMIZATION_STEP, logger)

optimizer.probe(params = starting_point, lazy = True)

optimizer.maximize(
        init_points = int(args.n_points) if args.random else 0,
        n_iter = 0 if args.random else int(args.n_points),
        acq = "ei",
        xi = 0.001,
)

for i, res in enumerate(optimizer.res):
    print("Iteration {}: \n\t{}".format(i, res))
