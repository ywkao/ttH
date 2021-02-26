import sys, os
sys.path.append("../")

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--input_file", help = "root file with histograms to plot", type=str)
parser.add_argument("--backgrounds", help = "list of processes to plot as bkgs", type=str, default="DiPhoton|GammaJets|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV")
parser.add_argument("--signals", help = "list of processes to plot as signals", type=str, default="ttH|TT_FCNC_hct|TT_FCNC_hut|ST_FCNC_hct|ST_FCNC_hut")
parser.add_argument("--plot_type", help = "which type of plot to make (e.g. std, std_linear, etc.)", type=str, default="std")
parser.add_argument("--plot_labels", help = "labels to put on plots", type=str)
args = parser.parse_args()

import parallel_utils

def rename(file, tag):
    command = 'mv %s.pdf %s_%s.pdf' % (file, file, tag)
    print "[check] " + command
    os.system(command)

def batch_rename(tag):
    stem_file = args.input_file.split('/')[1].split('.root')[0] + args.plot_type
    print "[check] " + stem_file
    rename(stem_file, tag)


#if "impute" in args.input_file:
#  args.backgrounds = args.backgrounds.replace("GammaJets", "QCD_GammaJets_imputed")

parallel_utils.run('./makePlots "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))

#parallel_utils.run('./makePlots_NNscore_st_v2 "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("NNscore_st_v2")
#parallel_utils.run('./makePlots_NNscore_tt_v2 "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("NNscore_tt_v2")
#parallel_utils.run('./makePlots_NNscore_st_v4 "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("NNscore_st_v4")
#parallel_utils.run('./makePlots_NNscore_tt_v4 "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("NNscore_tt_v4")
#
#parallel_utils.run('./makePlots_maxbsum "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("maxbsum")
#parallel_utils.run('./makePlots_2ndmaxbsum "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("2ndmaxbsum")
#parallel_utils.run('./makePlots_maxbtag "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("maxbtag")
#parallel_utils.run('./makePlots_2ndmaxbtag "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("2ndmaxbtag")
#parallel_utils.run('./makePlots_maxctag "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("maxctag")
#parallel_utils.run('./makePlots_2ndmaxctag "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("2ndmaxctag")
#parallel_utils.run('./makePlots_maxcvsl "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("maxcvsl")
#parallel_utils.run('./makePlots_2ndmaxcvsl "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("2ndmaxcvsl")
#parallel_utils.run('./makePlots_maxcvsb "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("maxcvsb")
#parallel_utils.run('./makePlots_2ndmaxcvsb "%s" "%s" "%s" "%s" "%s"' % (args.plot_type, args.input_file, args.plot_labels, args.backgrounds, args.signals))
#batch_rename("2ndmaxcvsb")

#full_bkgs = "DiPhoton|QCD_GammaJets_imputed|GammaJets|QCD|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV|ggH|VBF|VH|THQ|THW"
#parallel_utils.run('./makeTables "%s" "%s" "%s" > tables_%s.txt' % (args.input_file, args.signals, full_bkgs, args.input_file.replace(".root", "").replace("../", "")))
