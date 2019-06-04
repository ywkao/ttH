import sys, os
sys.path.append("../")

import itertools

import parallel_utils
import workflow_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to denote with", type=str)
parser.add_argument("--baby_version", help = "which version of babies to use", type=str)
args = parser.parse_args()

bdt = "none"

os.chdir("../")

do_looping = False
if do_looping:
  # Loopers
  parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017"' % (args.baby_version, args.tag))
  parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel_veryLoose" --bkg_options "impute" --years "2017"' % (args.baby_version, args.tag + "_impute"))

  # Data/MC Plots
  os.chdir("Plots")
  parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std_2017" --plot_labels "FCNC Leptonic|Loose MVA Presel." --signals "ttH|TT_FCNC_hut|TT_FCNC_hct|ST_FCNC_hut|ST_FCNC_hct" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG"' % ("ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" % args.tag))
  parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std_2017" --plot_labels "FCNC Hadronic|Loose MVA Presel." --signals "ttH|TT_FCNC_hut|TT_FCNC_hct|ST_FCNC_hut|ST_FCNC_hct" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG"' % ("ttHHadronic_RunII_MVA_Presel_veryLoose_%s_histogramsRunII.root" % (args.tag + "_impute")))

  os.chdir("../")

  # MVA BabyMaker
  parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017" --babymaker --fcnc' % (args.baby_version, args.tag))
  parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel_veryLoose" --bkg_options "impute" --years "2017" --babymaker --fcnc' % (args.baby_version, args.tag + "_impute"))

do_mvas = True
if do_mvas:
  os.chdir("../MVAs/")
  # MVA Prep
  parallel_utils.run('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --fcnc' % (args.tag))
  parallel_utils.run('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_FCNC.root" --channel "Hadronic" --fcnc' % (args.tag + "_impute")) 

  # MVA Training
  parallel_utils.run('python train.py --input "ttHLeptonic_%s_FCNC_features.hdf5" --channel "Leptonic" --tag "%s" --ext ""' % (args.tag, args.tag))
  parallel_utils.run('python train.py --input "ttHHadronic_%s_FCNC_features.hdf5" --channel "Hadronic" --tag "%s" --ext ""' % (args.tag + "_impute", args.tag + "_impute")) 
