import os, sys
import numpy
import h5py
import xgboost
import pandas

sys.path.append("../")
import ks_test
import utils

default_config = {
        'max_depth': 4,
        'eta': 0.2,
        'objective': 'binary:logistic',
        'scale_pos_weight': 1, 
        'subsample': 1.0,
        'colsample_bytree': 1.0,
        'nthread' : 12,
        'min_child_weight' : 1 
}

class BDT_Helper:
    def __init__(self, **kwargs):
        self.kwargs = kwargs
        
        self.n_features = len(kwargs.get('x_train')[0])
        self.feature_names = ["feature_%d" % i for i in range(self.n_features)]
        self.X_train = pandas.DataFrame(data=kwargs.get('x_train'), columns = self.feature_names)
        self.X_validation = pandas.DataFrame(data=kwargs.get('x_validation'), columns = self.feature_names)

        self.Y_train = kwargs.get('y_train')
        self.Y_validation = kwargs.get('y_validation')

        self.weights_train = kwargs.get('weights_train')
        self.weights_validation = kwargs.get('weights_validation')

        self.config = kwargs.get('config', default_config)
        self.n_round = kwargs.get('n_round', 100)

        self.metadata = kwargs.get('metadata')

    def train(self):
        self.d_train = xgboost.DMatrix(self.X_train, label = self.Y_train, weight = self.weights_train)
        self.d_validation  = xgboost.DMatrix(self.X_validation, label = self.Y_validation)

        evallist = [(self.d_train, 'train'), (self.d_validation, 'test')]
        progress = {}

        sum_neg_weights = utils.sum_of_weights(self.weights_train, self.Y_train, 0)
        sum_pos_weights = utils.sum_of_weights(self.weights_train, self.Y_train, 1)

        print((sum_pos_weights, sum_neg_weights))

        self.config["scale_pos_weight"] = sum_neg_weights / sum_pos_weights
        self.config["min_child_weight"] = min(1, (sum_neg_weights) / 100000.)

        self.bdt = xgboost.train(self.config, self.d_train, self.n_round, evallist, evals_result = progress)
        self.bdt.save_model(self.metadata["weights"].replace(".hdf5",".xgb"))

    def evaluate(self):
        self.pred_train = self.bdt.predict(self.d_train)
        self.pred_validation = self.bdt.predict(self.d_validation)

        d_sig, p_value_sig, d_bkg, p_value_bkg = ks_test.ks_test(self.pred_train, self.pred_validation, self.Y_train, self.Y_validation)
        print(("Results of ks-test (d-score) for signal: %.10f and background: %.10f" % (d_sig, d_bkg)))
        print(("Results of ks-test (p-value) for signal: %.10f and background: %.10f" % (p_value_sig, p_value_bkg)))

        self.auc_train, self.unc_train, self.fpr_train, self.tpr_train, self.thresh_train = utils.auc_and_unc(self.Y_train, self.pred_train, self.weights_train, 50)
        self.auc_validation, self.unc_validation, self.fpr_validation, self.tpr_validation, self.thresh_validation = utils.auc_and_unc(self.Y_validation, self.pred_validation, self.weights_validation, 50)

        print(("Training AUC: %.3f +/- %.4f" % (self.auc_train, self.unc_train)))
        print(("Testing  AUC: %.3f +/- %.4f" % (self.auc_validation, self.unc_validation)))
