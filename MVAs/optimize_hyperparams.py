import sys
import json
import math
import copy

from bayes_opt import BayesianOptimization

sys.path.append("../Loopers")
import parallel_utils
import train_v2

def estimate_max_za(n_round, max_depth, eta, subsample, colsample_bytree, min_child_weight, gamma, reg_alpha, reg_lambda):
  global idx, config, log
 
  print "%d-th iteration of hyperparameter scan" % idx

  n_round = int(n_round)
  max_depth = int(max_depth)

  config["n_round"] = n_round
  config["max_depth"] = max_depth
  config["eta"] = eta
  config["subsample"] = subsample
  config["colsample_bytree"] = colsample_bytree
  config["min_child_weight"] = min_child_weight
  config["gamma"] = gamma
  config["reg_alpha"] = reg_alpha
  config["reg_lambda"] = reg_lambda

  config_1, config_2 = copy.deepcopy(config), copy.deepcopy(config)
  config_1["invert_test_and_train"] = True
  config_2["invert_test_and_train"] = False


  max_za_mc_1, max_za_mc_unc_1, max_za_data_1, max_za_data_unc_1, auc_train_1, auc_test_1 = train_v2.train_bdt(config_1)
  max_za_mc_2, max_za_mc_unc_2, max_za_data_2, max_za_data_unc_2, auc_train_2, auc_test_2 = train_v2.train_bdt(config_2)

  max_za_mc_comb = (max_za_mc_1 + max_za_mc_2) / 2
  max_za_mc_unc_comb = math.sqrt((max_za_mc_unc_1**2) + (max_za_mc_unc_2**2)) / 2

  auc_train = (auc_train_1 + auc_train_2) / 2.
  auc_test  = (auc_test_1 + auc_test_2) / 2.

  if idx == 0:
    scan_results = {}

  else:
    with open(log, "r") as f_in:
      scan_results = json.load(f_in)

  scan_results[idx] = {
	"config" : config,
	"max_za_mc_1" : max_za_mc_1,
	"max_za_mc_unc_1" : max_za_mc_unc_1,
	"max_za_data_1" : max_za_data_1,
        "max_za_data_unc_1" : max_za_data_unc_1,
	"max_za_mc_2" : max_za_mc_2,
        "max_za_mc_unc_2" : max_za_mc_unc_2,
        "max_za_data_2" : max_za_data_2,
        "max_za_data_unc_2" : max_za_data_unc_2,
	"max_za_mc_comb" : max_za_mc_comb,
	"max_za_mc_unc_comb" : max_za_mc_unc_comb,
	"auc_train" : auc_train, 
	"auc_test" : auc_test 
    }

  with open(log, "w") as f_out:
    json.dump(scan_results, f_out, indent=4, sort_keys=True)

  idx += 1
  return auc_test
  #return max_za_mc_comb

config = {
        "input_file" : "", 

	"input_file_1" : "ttHLeptonic_1617_ttHLeptonicLoose_features.hdf5",
	"input_file_2" : "ttHLeptonic_1617_ttHLeptonicLoose_features_invert.hdf5",
        "tag" : "testin",

        "sideband" : False,
        "optimization_vars" : [],
        "multiclassifier" : False,
}

idx = 0
log = "bayes_hyperparam_scan.json"

pbounds = {
	"n_round" 		: (1, 1000),
	"max_depth" 		: (2, 15),
	"eta" 			: (0.001, 1.0),
	"subsample" 		: (0.5, 1.0),
	"colsample_bytree" 	: (0.1, 1.0),
	"min_child_weight" 	: (0, 20),
	"gamma" 		: (0, 10),
	"reg_alpha"		: (0, 10),
	"reg_lambda"		: (0, 10),
}

starting_point = {
	"n_round" 		: 75,
	"max_depth" 		: 4,
	"eta" 			: 0.2,
	"subsample" 		: 1.0,
	"colsample_bytree" 	: 1.0,
	"min_child_weight" 	: 1,
	"gamma" 		: 0,
	"reg_lambda" 		: 1,
	"reg_alpha"		: 0,
}

optimizer = BayesianOptimization(
    f=estimate_max_za,
    pbounds=pbounds,
    verbose=2, # verbose = 1 prints only when a maximum is observed, verbose = 0 is silent
    random_state=1,
)

optimizer.probe(params = starting_point, lazy = True)

optimizer.maximize(
	init_points = 20,
	n_iter = 1000,
)

for i, res in enumerate(optimizer.res):
    print("Iteration {}: \n\t{}".format(i, res))
