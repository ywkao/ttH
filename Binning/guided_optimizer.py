import os, sys
import multiprocessing
from json import JSONEncoder
import json

import tensorflow
import keras
import xgboost
import pandas
from sklearn.model_selection import train_test_split

import numpy

import ROOT
import root_numpy

from scanClass import scanClass
from makeModels import makeModel
from cardMaker import makeCards

process_dict = {
    "ttH" : [0],
    "ggH" : [14],
    "FCNC_Hut" : [22,24],
    "FCNC_Hct" : [23,25],
    "tH" : [11,12],
    "sm_higgs" : [0,11,12,14,15,16],
    "bkg" : [1, 2, 3, 5, 6, 7, 9, 13, 18, 19, 20, 21, 26]
}


class NumpyArrayEncoder(JSONEncoder):
    def default(self, obj):
        if isinstance(obj, numpy.ndarray):
            return obj.tolist()
        return JSONEncoder.default(self, obj)

class Guided_Optimizer():
    def __init__(self, **kwargs):
        self.input  =   kwargs.get('input')
        self.tag    =   kwargs.get('tag', '')
        self.channel = "FCNC_Hadronic" if "Hadronic" in self.input else "FCNC_Leptonic"

        self.mvas   =   kwargs.get('mvas', { "1d" : ["mva_score"], "2d" : ["mva_smhiggs_score", "mva_nonres_score"] }) 
        self.n_bins =   kwargs.get('n_bins', [1, 2, 3, 4]) 
        self.strategies = kwargs.get('strategies', ['random', 'guided'])

        self.nCores = kwargs.get('nCores', 12)

        self.verbose = kwargs.get('verbose', True)

        self.combineOption = kwargs.get('combineOption', 'AsymptoticLimits -m 125 ')
        self.sm_higgs_unc  = kwargs.get('sm_higgs_unc', 0.4)

        self.coupling = kwargs.get('coupling')
        self.signal =   kwargs.get('signal', ['FCNC_%s' % self.coupling])
        self.resonant_bkgs = kwargs.get('resonant_bkgs', ['sm_higgs'])

        self.points_per_epoch = kwargs.get('points_per_epoch', 200)
        self.initial_points = kwargs.get('initial_points', 48)
        self.n_epochs = kwargs.get('n_epochs', -1) # how many epochs of optimization to run (-1 means run until convergence)

        self.mva = kwargs.get('mva', 'bdt')
        self.dnn_config = kwargs.get('dnn_config', {
                                "dropout" : 0.0,
                                "n_nodes" : 128,
                                "n_layers" : 5,
                                "batch_norm" : False,
                                "activation" : "relu",
                                "learning_rate" : 0.00001,
                                "loss" : "mean_absolute_error",
                                "batch_size" : 1000,
                            })

        self.bdt_config = kwargs.get('bdt_config', {
                                'max_depth': 10,
                                'eta': 0.2,
                                'objective': 'reg:linear',
                                'subsample': 1.0,
                                'colsample_bytree': 1.0,
                                'nthread' : 12, 
                            })

        self.scanConfig = kwargs.get('scanConfig', { "tag" : self.channel,
                                                     "filename" : self.input,
                                                     "selection" : "",
                                                     "sigName" : self.signal[0] + "_hgg",
                                                     "var" : "mass",
                                                     "weightVar" : "weight",
                                                     "modelpath" : "/home/users/sjmay/ttH/Binning/models/" + self.tag,
                                                     "plotpath" : "/home/users/sjmay/public_html/ttH/Binning/" + self.tag,
                                                     "combineEnv" : "/home/users/sjmay/ttH/Binning/CMSSW_10_2_13/src",
                                                    })

        # Setup
        f = ROOT.TFile(self.input)
        self.tree = f.Get("t")
    
        self.n_points = self.points_per_epoch


    def optimize(self): 
        self.results = {}
        for dim, mvas in self.mvas.items():
            self.results[dim] = {}
            for n_bin in self.n_bins:
                self.results[dim][n_bin] = {}
                for strategy in self.strategies:
                    self.find_optimal_binning(dim, mvas, n_bin, strategy)

        # save results
        outfile = "optimization_results/guided_optimizer_results_%s_%s_%s.json" % (self.coupling, self.channel, self.tag)
        with open(outfile, "w") as f_out:
            json.dump(self.results, f_out, cls = NumpyArrayEncoder, sort_keys = True, indent = 4)


    def find_optimal_binning(self, dim, mvas, n_bin, strategy): # find optimal binning for len(mvas)-D optimization with n_bin SRs
        if self.verbose:
            print("[GUIDED OPTIMIZER] Finding optimal binning for %s optimization in mva scores: %s with n_bin = %d, and %s optimiziation strategy" % (dim, mvas, n_bin, strategy))
        
        self.iteration_ctr = 0
        self.results[dim][n_bin][strategy] = {
            "X" : [], # points actually tried
            "y" : [], # limits for these points
            "exp_lim" : [], # dict with idx, exp_lim (+/-1sigma), selection
            "eff" : [], # acceptance rate vs. epoch (1 for random sampling)
            "sample_mean" : [], # mean value of limit for sampled points vs. epoch
            "sample_std" : [], # std ""
            "sample_best" : [], # best limit for sampled points vs. epoch
            "accuracy" : [], # dnn accuracy vs. epcoh
        }
        
        self.n_bad_epochs = 0

        initial_results = self.initialize(mvas, n_bin)
        self.update_results(dim, n_bin, strategy, initial_results)

        self.converged = False
        while not self.converged:
            if strategy == "guided":
                accuracy = self.train_mva(self.results[dim][n_bin][strategy]["X"], self.results[dim][n_bin][strategy]["y"])
            elif strategy == "random":
                accuracy = 0

            results = self.sample(mvas, n_bin, strategy)
            results["accuracy"] = accuracy
            self.update_results(dim, n_bin, strategy, results) 

            self.check_convergence(self.results[dim][n_bin][strategy])
           
            self.reset_mva(mvas, n_bin) 

    def reset_mva(self, mvas, n_bin):
        if self.mva == "dnn":
            self.initialize_dnn(n_bin * len(mvas)) # reset dnn (so we start out with a fresh training)
        if self.mva == "bdt":
            self.initialize_bdt(n_bin * len(mvas))

    def check_convergence(self, results): # if we go N epochs without improving by X%, we are converged
        N = 50 # number of early stopping rounds
        X = 0.01

        if self.n_epochs > 0 and self.iteration_ctr >= self.n_epochs: # max limit on n_epochs
            self.converged = True
            return

        if len(results["sample_best"]) <= 1: # need at least 2 optimization epochs to start asking about convergence
            self.converged = False
            return

        best_limit = min(results["sample_best"][:-1]) # find best limit from all previous epochs (not this one)

        if (results["sample_best"][-1] * (1 + X)) <= best_limit:
            self.converged = False # not converged bc we improved by at least X%
        else:
            self.n_bad_epochs += 1
            if self.n_bad_epochs >= N:
                self.converged = True
            else:
                self.converged = False

    def update_results(self, dim, n_bin, strategy, results):
        for field in ["X", "y", "exp_lim"]:
            if len(self.results[dim][n_bin][strategy][field]) == 0:
                self.results[dim][n_bin][strategy][field] = numpy.array(results[field])
            else:
                self.results[dim][n_bin][strategy][field] = numpy.concatenate([self.results[dim][n_bin][strategy][field], numpy.array(results[field])])
            #self.results[dim][n_bin][strategy][field] += results[field]
            #self.results[dim][n_bin][strategy][field] = numpy.concatenateresults[field]
        
        for field in ["eff", "sample_mean", "sample_std", "sample_best", "accuracy"]:
            self.results[dim][n_bin][strategy][field].append(results[field])

        self.current_best_lim = min(self.results[dim][n_bin][strategy]["sample_best"])

        self.iteration_ctr += 1

        if self.verbose:
            print("[GUIDED OPTIMIZER] Finished optimization epoch %d, for %s optimization with %d bins and %s optimization strategy" % (self.iteration_ctr, dim, n_bin, strategy))
            print("[GUIDED OPTIMIZER] Summary of results so far:")
            print("[GUIDED OPTIMIZER] Mean value of sampled points vs. epoch: ", self.results[dim][n_bin][strategy]["sample_mean"])
            print("[GUIDED OPTIMIZER] Best value of sampled points vs. epoch: ", self.results[dim][n_bin][strategy]["sample_best"])
            print("[GUIDED OPTIMIZER] Efficiency of proposed points vs. epoch: ", self.results[dim][n_bin][strategy]["eff"])
            print("[GUIDED OPTIMIZER] DNN Accuracy vs. epoch: ", self.results[dim][n_bin][strategy]["accuracy"])



    def initialize(self, mvas, n_bin): # randomly sample initial_points points to get initial training/testing set
        # Set up scanClass
        self.scanConfig["modelpath"] = self.scanConfig["modelpath"] + "_%dd_%dbin/" % (len(mvas), n_bin)
        self.scanConfig["plotpath"] = self.scanConfig["plotpath"] + "_%dd_%dbin/" % (len(mvas), n_bin)
        self.scanner = scanClass(self.scanConfig)
        self.scanner.cleanDir()

        # Calculate quantiles <-> mva scores
        if self.verbose:
            print("[GUIDED OPTIMIZER] Calculating quantiles to mva score function")
    
        self.quantiles = {}
        for mva in mvas:
            scores, quantiles = self.scanner.quantiles_to_mva_score(5000, mva, self.base_selection() + "&&" + self.process_selection(self.signal[0]))
            self.quantiles[mva] = { "scores" : scores, "quantiles" : quantiles }

        # Set up DNN
        self.initialize_mva(n_bin * len(mvas))

        # sample initial points and calculate limits
        X, acc = self.generate_cut_combos(self.initial_points, mvas, n_bin, mode = 'random')
        exp_limits = self.calculate_expected_limits(X, mvas, n_bin)
        #y = exp_limits[:]["exp_lim"][0]
        y = []
        X_ = []
        for lim in exp_limits:
            X_.append(lim["x"])
            y.append(lim["exp_lim"][0])

        results = {
            "X" : X_,
            "y" : y,
            "exp_lim" : exp_limits,
            "eff" : 1,
            "sample_mean" : numpy.mean(y),
            "sample_std"  : numpy.std(y),
            "sample_best" : min(y),
            "accuracy" : 0,
        }

        return results

    def initialize_mva(self, n_cuts):
        if self.mva == "dnn":
            self.initialize_dnn(n_cuts)
        elif self.mva == "bdt":
            self.initialize_bdt(n_cuts)

    def initialize_bdt(self, n_cuts):
        self.bdt = self.bdt_regressor(n_cuts, self.bdt_config)

    def initialize_dnn(self, n_cuts):
        self.model = self.mlp(n_cuts, self.dnn_config)

    def bdt_regressor(self, n_cuts, config):
        return xgboost.XGBRegressor()

    def mlp(self, n_cuts, config):
        dropout = config["dropout"]
        n_nodes = config["n_nodes"]
        n_layers = config["n_layers"]
        batch_norm = config["batch_norm"]
        learning_rate = config["learning_rate"]
        activation = config["activation"]
        loss = config["loss"]

        input_global = keras.layers.Input(shape=(n_cuts,), name = 'input_global')
        dense = input_global
        for i in range(n_layers):
            dense = keras.layers.Dense(n_nodes, activation = activation, kernel_initializer = 'lecun_uniform', name = 'dense_%d' % i)(dense)
            if dropout > 0:
                dense = keras.layers.Dropout(rate = dropout, name = 'dense_dropout_%d' % i)(dense)
            if batch_norm:
                dense = keras.layers.BatchNormalization(name = 'dense_batchnorm_%d' % i)(dense)

        output = keras.layers.Dense(1, kernel_initializer = 'lecun_uniform', name = 'output')(dense)


        model = keras.models.Model(inputs = [input_global], outputs = [output])
        optimizer = keras.optimizers.Adam(lr = learning_rate)
        model.compile(optimizer = 'adam', loss = loss, metrics = ['mae'])
    
        if self.verbose:
            print("[GUIDED OPTIMIZER] DNN Model Summary:", model.summary())
            print("[GUIDED OPTIMIZER] DNN Config:", config)

        return model

    def train_mva(self, X, y):
        if self.mva == "dnn":
            return self.train_dnn(X, y)
        elif self.mva == "bdt":
            return self.train_bdt(X, y)

    def train_bdt(self, X, y):
        X_train, X_test, y_train, y_test = train_test_split(X, y, train_size = 0.8)

        #X_train = X_train.reshape(-1,1)        
        #X_test = X_test.reshape(-1,1)        

        X_train = pandas.DataFrame(data = X_train)#, columns = [str(i) for i in range(len(X[0]))])
        X_test = pandas.DataFrame(data = X_test)#, columns = [str(i) for i in range(len(X[0]))])

        eval_list = [(X_test, y_test)]
        self.bdt.fit(X_train, y_train, early_stopping_rounds=5, eval_metric='mae', eval_set = eval_list)

        pred_test = self.bdt.predict(X_test)

        percent_error = (pred_test - y_test) / y_test
        percent_error_mean = numpy.mean(percent_error)
        percent_error_std  = numpy.std(percent_error)

        self.percent_error = numpy.sqrt(numpy.mean(percent_error**2)) # just use rms 

        if self.verbose:
            print("[GUIDED OPTIMIZER] Finished training BDT with error %.3f +/- %.3f" % (percent_error_mean, percent_error_std))

        return percent_error_std 
        
            


    def train_dnn(self, X, y): # train dnn with early stopping
        X_train, X_test, y_train, y_test = train_test_split(X, y, train_size = 0.8)

        train_more = True
        best_val_loss = 999
        ticker = 0
        while train_more:
            results = self.model.fit([X_train], y_train, batch_size = self.dnn_config["batch_size"], validation_data = (X_test, y_test), epochs = 1)
            val_loss = results.history['val_loss'][0]
            if val_loss < best_val_loss:
                best_val_loss = val_loss
                ticker = 0
            else:
                ticker += 1
            if ticker >= 15:
                train_more = False

        pred_test = self.model.predict([X_test], batch_size = 10**5)[:,0]
        percent_error = (pred_test - y_test) / y_test

        percent_error_mean = numpy.mean(percent_error)
        percent_error_std  = numpy.std(percent_error)

        self.percent_error = numpy.sqrt(numpy.mean(percent_error**2)) # just use rms 

        if self.verbose:
            print("[GUIDED OPTIMIZER] Finished training DNN with error %.3f +/- %.3f" % (percent_error_mean, percent_error_std)) 
        
        return percent_error_std


    def sample(self, mvas, n_bin, strategy):
        X, eff = self.generate_cut_combos(self.n_points, mvas, n_bin, strategy)

        exp_limits = self.calculate_expected_limits(X, mvas, n_bin)

        #y = exp_limits[:]["exp_lim"][0]
        X_ = []
        y = []
        for lim in exp_limits:
            X_.append(lim["x"])
            y.append(lim["exp_lim"][0])

        sample_mean = numpy.mean(y)
        sample_std  = numpy.std(y)
        sample_best = min(y)

        results = {
            "X" : X_,
            "y" : y, 
            "exp_lim" : exp_limits,
            "eff" : eff,
            "sample_mean" : sample_mean, 
            "sample_std" : sample_std,
            "sample_best" : sample_best
         }

        return results


    def generate_cut_combos(self, N_combos, mvas, n_bin, mode):
        if mode == 'random':
            X = self.generate_random_cut_combos(N_combos, mvas, n_bin)
            return X, 1.0

        elif mode == 'guided':
            X = []

            n_total = 0
            while len(X) < N_combos:
                n_total += N_combos
                X += self.subsample(self.generate_random_cut_combos(N_combos, mvas, n_bin))          
                
                if self.verbose:
                    print("[GUIDED OPTIMIZER] Subsampling: %d accepted points with %d total points tried (%.3f acceptance rate)" % (len(X), n_total, float(len(X))/float(n_total)))


            eff = float(len(X))/float(n_total)

            return X, eff

        else:
            print("[GUIDED_OPTIMIZER] sample mode: %s is not supported!" % mode)
            sys.exit(1)

    def reasonable_effs(self, effs):
        if effs[0] < 0.1:
            return False # don't accept SRs with signal eff less than 5%
        if effs[-1] > 0.8: # don't accept SRs with the lowest signal eff defined by a cut that has more than 80% efficiency on signal
            return False
        if len(effs) > 1:
            for i in range(len(effs)-1):
                if (effs[i+1] - effs[i]) < 0.05:
                    return False
        return True

    def generate_random_cut_combos(self, N_combos, mvas, n_bin):
        if self.verbose:
            print("[GUIDED_OPTIMIZER] Calculating random cut combos for %d bins with mvas" % n_bin, mvas)
        
        X = []
        for i in range(N_combos):
            cuts_list = []
            effs_list = []
            for mva in mvas:
                reasonable = False
                while not reasonable:
                    effs = list(numpy.sort(numpy.random.rand(n_bin)))
                    reasonable = self.reasonable_effs(effs)
                if len(effs_list) > 0:
                    effs_list += effs
                else:
                    effs_list = effs

                cuts = self.convert_eff_to_cut(mva, effs)
                if len(cuts_list) > 0:
                    cuts_list += cuts
                else:
                    cuts_list = cuts

            if self.verbose:
                if i < 10:
                    print("[GUIDED_OPTIMIZER] the %d-th cut combo is " % i, cuts_list, " corresponding to effs of ", effs_list)
            X.append(cuts_list)
        return X 

    def convert_eff_to_cut(self, mva, effs):
        cuts = []
        for eff in effs:
            cuts.append(self.convert_single_eff_to_cut(mva, eff))
        return cuts

    def find_nearest(self, array, value):
        val = numpy.ones_like(array)*value
        idx = (numpy.abs(array-val)).argmin()
        return array[idx], idx

    def convert_single_eff_to_cut(self, mva, eff):
        value, idx = self.find_nearest(self.quantiles[mva]["quantiles"], eff)
        return self.quantiles[mva]["scores"][idx][0]

    def subsample(self, X):
        pred = self.predict_limits(X)
        prob = self.calculate_probs(pred)
   
        if self.verbose:
            print("[GUIDED OPTIMIZER] Here are the first few points, along with their predictions and accept probs")
            for i in range(3):
                print("[GUIDED OPTIMIZER] Point: ", X[i], " prediction: %.3f, probability to accept point: %.3f" % (pred[i], prob[i]))

        accept_idx = numpy.nonzero(prob > numpy.random.rand(len(X)))[0]
        print accept_idx

        X = numpy.array(X)

        return list(X[accept_idx])

    def predict_limits(self, X):
        if self.mva == "dnn":
            return self.model.predict([X], batch_size = 10**5)[:,0]
        elif self.mva == "bdt":
            X_frame = pandas.DataFrame(data = X)#, columns = [str(i) for i in range(len(X[0]))])
            return self.bdt.predict(X_frame)

    def calculate_probs(self, pred):
        pred_normalized = pred * (1./self.current_best_lim)

        #prob = (1./pred_normalized) * numpy.exp( -(pred_normalized - 1) / (self.percent_error))
        prob = numpy.exp( -(pred_normalized - 1) / (self.percent_error))
        #max_prob = max(prob)
        #prob *= 1./max_prob # scale highest probability up to 1

        for i in range(len(prob)):
            if prob[i] > 1.:
                prob[i] = 1.
        return prob

    def calculate_expected_limits(self, points, mvas, n_bin):
        exp_limits = []

        manager = multiprocessing.Manager()
        temp_results = manager.dict() # create this dict that multiprocess childs can talk to

        running_procs = []

        selections = []
        for point in points:
            selections.append(self.get_selection(point, mvas, n_bin))

        # submit jobs
        for i in range(len(selections)):
            if self.verbose:
                print("[GUIDED OPTIMIZER] On selection %d" % i)
                if i < 3:
                    print("[GUIDED OPTIMIZER] Point ", points[i], " was converted to selection string: %s" % selections[i])

            running_procs.append(multiprocessing.Process(target = self.calculate_expected_limit, args = (selections[i], i + (len(selections) * self.iteration_ctr), points[i], temp_results)))
            running_procs[-1].start()

            while True:
                for i in range(len(running_procs)):
                    if not running_procs[i].is_alive():
                        running_procs.pop(i)
                        break
                if len(running_procs) < self.nCores: # if we have less than nCores jobs running, break infinite loop and add another
                    break
                else:
                    os.system("sleep 5s")

        # somewhat hacky (or perhaps elegant?) snippet to wait for last nCores jobs to finish running before we move on
        while len(running_procs) > 0:
            for i in range(len(running_procs)):
                try:
                    if not running_procs[i].is_alive():
                        running_procs.pop(i)
                except:
                    continue

        # extract results from temp_results
        for idx, result in temp_results.items():
            exp_limits.append(result)

        return exp_limits

    def get_selection(self, point, mvas, n_bin):
        assert len(point) == ( len(mvas) * n_bin )

        selection = []
        for i in range(n_bin):
            sel = ""
            for j in range(len(mvas)):
                sel += "%s >= %.6f && " % (mvas[j], point[(j*n_bin) + i])
            sel = sel[:-3] # remove trailing "&& "
            if i > 0:
                sel += " && "
                for j in range(len(mvas)):
                    sel += "%s < %.6f && " % (mvas[j], point[(j*n_bin) + (i - 1)])
            sel = sel[:-3] # remove trailing "&& "
            selection.append(sel)

        return selection

    def process_selection(self, process):
        selection = " ("
        for i in range(len(process_dict[process])):
            selection += "process_id == " + str(process_dict[process][i])
            if i == (len(process_dict[process]) - 1):
                selection += ") "
            else:
                selection += " || "
        return selection

    def base_selection(self):
        return "(mass > 100 && mass < 180 && train_id == 1) "

    def calculate_expected_limit(self, selection, idx, m_point, temp_results):
        yields = {}
        for i in range(len(selection)):
            #sr_too_small = False # require at least 15 expected events in [100,180] GeV region from nonres bkg MC
            
            bin = "Bin_%d" % i
            yields[bin] = {}
            for process in self.signal + self.resonant_bkgs:
                signalModelConfig = {
                    "var" : "mass", "weightVar" : "weight",
                    "plotpath" : self.scanConfig["plotpath"],
                    "modelpath" : self.scanConfig["modelpath"],
                    "filename" : self.input,
                    "savename" : "CMS-HGG_sigfit_mva_" + process + "_hgg_" + self.channel + "_" + str(i) + "_" + str(idx),
                    "tag" : "hggpdfsmrel_" + process + "_hgg_" + self.channel + "_" + str(i) + "_" + str(idx),
                    "selection" : self.base_selection() + "&&" + self.process_selection(process) + " && " + selection[i],
                }
                model = makeModel(signalModelConfig)
                model.getTree(self.scanner.getTree())
                sig_yield = model.makeSignalModel("wsig_13TeV",
                        { "replaceNorm" : False, "norm_in" : -1, "fixParameters" : True },
                )
                yields[bin][process] = sig_yield                

            bkgModelConfig = {
                "var" : "mass", "weightVar" : "weight",
                "plotpath" : self.scanConfig["plotpath"],
                "modelpath" : self.scanConfig["modelpath"],
                "filename" : self.input,
                "savename" : "CMS-HGG_bkg_" + self.channel + "_" + str(i) + "_" + str(idx),
                "tag" : "CMS_hgg_bkgshape_" + self.channel + "_" + str(i) + "_" + str(idx),
                "selection" : self.base_selection() + "&&" + self.process_selection("bkg") + " && " + selection[i],
            }

            model = makeModel(bkgModelConfig)
            model.getTree(self.scanner.getTree())
            bkg_yield, bkg_yield_raw = model.makeBackgroundModel("wbkg_13TeV", self.channel + "_" + str(i) + "_" + str(idx))

            yields[bin]["bkg"] = bkg_yield


        datacard = makeCards(self.scanConfig["modelpath"], "CMS-HGG_mva_13TeV_datacard_" + str(idx) + ".txt",
                { "sm_higgs_unc" : self.sm_higgs_unc },
        )
        tagList = [self.channel + "_" + str(x) for x in range(len(selection))]
        sigList = [self.signal[0] + "_hgg"]
        bkgList = ["bkg_mass"]
        for bkg in self.resonant_bkgs:
            bkgList.append(bkg + "_hgg")

        datacard.WriteCard(sigList, bkgList, tagList, "_" + str(idx))

        combineConfig = {
                "combineOption" : self.combineOption,
                "combineOutName" : "sig_" + str(idx),
                "cardName" : "CMS-HGG_mva_13TeV_datacard_" + str(idx) + ".txt",
                "outtxtName" : "sig_" + str(idx) + ".txt",
        }

        exp_lim, exp_lim_up1sigma, exp_lim_down1sigma = self.scanner.runCombine(combineConfig)

        result = {
           "idx" : idx,
           "x" : [float(x) for x in m_point],
           "exp_lim" : [exp_lim, exp_lim_up1sigma, exp_lim_down1sigma],
           "selection" : selection,
           "yields" : yields
        }

        temp_results[",".join(selection) + str(idx)] = result

        if self.verbose:
            print("[GUIDED OPTIMIZER]", result)
            #print("[GUIDED OPTIMIZER] Finished testing %d-th combo in epoch" % idx)
            #print("[GUIDED OPTIMIZER] With %d bins, here is the info for each bin:" % len(selection)) 
            #for i in range(len(selection)):
            #    print("[GUIDED OPTIMIZER] Bin %d: signal = %.2f, sm_higgs = %.2f, background = %.2f" % (i, result["yields"]["Bin_%d" % i][self.signal[0]], result["yields"]["Bin_%d" % i][self.resonant_bkgs[0]], result["yields"]["Bin_%d" % i]["bkg"]))
            #print("[GUIDED OPTIMIZER] Giving an expected limit of %.2f" % result["exp_lim"][0])

        return
