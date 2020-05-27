import os, sys
import json
import numpy

import guided_optimizer

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to distinguish this optimization", type=str, default = "test")
parser.add_argument("--channel", help = "leptonic or hadronic", type=str, default = "Hadronic")
parser.add_argument("--file", help = "path to final fit tree", type=str)
parser.add_argument("--coupling", help = "coupling (Hut or Hct)", type=str)
parser.add_argument("--mvas", help = "list of mva branches to perform Nd optimization with", type=str, default = "mva_score")
parser.add_argument("--sm_higgs_unc", help = "value of unc on sm higgs processes", type=float, default = 0.1)
parser.add_argument("--nCores", help = "number of cores to use", type=int, default = 24)
args = parser.parse_args()


optimizer = guided_optimizer.Guided_Optimizer(
                input = args.file,
                tag = args.tag,
                channel = args.channel,
                coupling = args.coupling,
                nCores = args.nCores,

                sm_higgs_unc = args.sm_higgs_unc,

                n_bins = [1,2,3,4],
                #mvas = { "1d" : ["mva_score"] },
                mvas = { "2d" : ["mva_smhiggs_score", "mva_nonres_score"] },
                #mvas = { "1d" : ["mva_score"], "2d" : ["mva_smhiggs_score", "mva_nonres_score"] },
                #strategies = ['guided'],
                strategies = ['guided', 'random'],
               
                initial_points = 240,
                points_per_epoch = 240,
                n_epochs = 5,
                verbose = True
)

optimizer.optimize()

#for dim, mvas in optimizer.mvas.items():
#    for bin in optimizer.n_bins:
#        bin = str(bin)
#        x = numpy.arange(len(results[dim][bin]["guided"]["sample_mean"]))
#        y1 = results[dim][bin]["guided"]["sample_mean"]
#        y1_unc = results[dim][bin]["guided"]["sample_std"]
#        y2 = results[dim][bin]["guided"]["sample_best"]
#
#        print dim, mvas, bin, x, y1, y1_unc, y2

