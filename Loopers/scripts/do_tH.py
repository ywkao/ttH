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

os.chdir("../")

# Hadronic
#parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel_veryLoose" --bkg_options "impute"' % (args.baby_version, args.tag))
#parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel_veryLoose" --bkg_options "impute" --babymaker' % (args.baby_version, args.tag))

# Leptonic
#parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none"' % (args.baby_version, args.tag))
#parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_SR_Inclusive" --bkg_options "none"' % (args.baby_version, args.tag))
#parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_SR_Tight" --bkg_options "none"' % (args.baby_version, args.tag))
#parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_SR_Inclusive" --bkg_options "none" --babymaker --ttH_vs_tH' % (args.baby_version, args.tag))

# Prep
os.chdir("../MVAs/")
command_list = []
#command_list.append('python prep.py --channel "Hadronic" --input "../Loopers/MVABaby_ttHHadronic_%s.root" --ttH_vs_tH' % (args.tag))
#command_list.append('python prep_dnn.py --channel "Hadronic" --input "../Loopers/MVABaby_ttHHadronic_%s.root" --signal "ttH" --backgrounds "tH"' % (args.tag))
#command_list.append('python prep.py --channel "Leptonic" --input "../Loopers/MVABaby_ttHLeptonic_%s.root" --ttH_vs_tH' % (args.tag))
#command_list.append('python prep_dnn.py --channel "Leptonic" --input "../Loopers/MVABaby_ttHLeptonic_%s.root" --signal "ttH" --backgrounds "tH" --z_score --tag "%s"' % (args.tag, args.tag))
parallel_utils.submit_jobs(command_list, 4)

# Train
#parallel_utils.run('python train.py --input "ttHHadronic_%s_features.hdf5" --channel "Hadronic" --tag "ttH_vs_tH_%s" --ext ""' % (args.tag, args.tag))
#parallel_utils.run('python train_dnn.py --input "ttHHadronic_%s_dnn_features_.hdf5" --tag "ttHHadronic_ttH_vs_tH_%s" --channel "Hadronic"' % (args.tag, args.tag))

#parallel_utils.run('python train.py --input "ttHLeptonic_%s_features.hdf5" --channel "Leptonic" --tag "ttH_vs_tH_%s" --ext ""' % (args.tag, args.tag))
#parallel_utils.run('python train_dnn.py --input "ttHLeptonic_%s_dnn_features_%s.hdf5" --tag "ttHLeptonic_ttH_vs_tH_%s" --channel "Leptonic" --preprocess_scheme "preprocess_scheme_%s_%s.json"' % (args.tag, args.tag, args.tag, "Leptonic", args.tag), True)
#parallel_utils.submit_jobs(command_list, 4)

# Zip BDT/DNN score into final fit ntuple
command_list = []
#command_list.append('python prep.py --channel "Leptonic" --input "../Loopers/MVABaby_ttHLeptonic_%s.root" --ttH_vs_tH --dnn_models "dnn_weights/metadata_Leptonic_ttHLeptonic_ttH_vs_tH_%s.json" --dont_train_with_dnn --tag "%s"' % (args.tag, args.tag, args.tag))
parallel_utils.submit_jobs(command_list, 4)
parallel_utils.run('python train.py --input "ttHLeptonic_%s_features%s.hdf5" --channel "Leptonic" --tag "ttH_vs_tH_%s" --ext "" --reference_mva "dnn_weights/metadata_Leptonic_ttHLeptonic_ttH_vs_tH_%s.json" --reference_mva_name "ttH_vs_tH_DNN"' % (args.tag, args.tag, args.tag, args.tag)) 

