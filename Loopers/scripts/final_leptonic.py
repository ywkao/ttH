import sys, os
sys.path.append("../")

import parallel_utils
import workflow_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to denote with", type=str)
parser.add_argument("--baby_version", help = "which version of babies to use", type=str)
args = parser.parse_args()

bdt = "none"

os.chdir("../")

plot_types = ["std", "std_linear", "std_shape", "std_shape_sig_vs_data"]

do_looping = False
if do_looping:
    parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --bdt "none"' % (args.baby_version, args.tag))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG|DY|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Leptonic|Loose MVA Presel."' % (args.tag, plot_type))
    os.chdir("../")

do_ttZ = False
if do_ttZ:
    #parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_ttZ_CR" --bkg_options "none" --bdt "none"' % (args.baby_version, args.tag))
    os.chdir("Plots")
    #for plot_type in plot_types:
    #    parallel_utils.run('python plot_wrapper.py --input_file "../ttHLeptonic_RunII_ttZ_CR_%s_histogramsRunII.root" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG|DY|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Leptonic|t#bar{t}Z CR|N_{jets} #geq 1"' % (args.tag, plot_type))
    os.chdir("../")
    parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_ttZ_Tight_CR" --bkg_options "none" --bdt "none" --do_systematics' % (args.baby_version, args.tag))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHLeptonic_RunII_ttZ_Tight_CR_%s_histogramsRunII.root" --backgrounds "DY|Other2|TTZ" --signals "ttH" --plot_type "%s" --plot_labels "Leptonic Channel|t#bar{t}Z Control Region|N_{jets} #geq 3|N_{b-jets} (medium) #geq 2"' % (args.tag, plot_type))
 
    os.chdir("../")


do_mvas = False
if do_mvas:
    os.chdir("../Loopers")
    parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --bdt "none" --babymaker' % (args.baby_version, args.tag + "_RunII_MVA_Presel"))

    os.chdir("../MVAs/")
    #parallel_utils.run('python prep.py --channel "Leptonic" --input "../Loopers/MVABaby_ttHLeptonic_%s_RunII_MVA_Presel.root" --tag ""' % (args.tag))
    #parallel_utils.run('python train.py --channel "Leptonic" --input "ttHLeptonic_%s_RunII_MVA_Presel_features.hdf5" --tag "_%s_RunII_MVA_Presel" --ext ""' % (args.tag, args.tag))

do_dnn = False
if do_dnn:
    os.chdir("../MVAs/")
    #os.system("./setup.sh")

    command_list = []
    command_list.append('python prep.py --channel "Leptonic" --input "../Loopers/MVABaby_ttHLeptonic_%s_RunII_MVA_Presel.root" --tag "ttGG" --ttGG_only' % (args.tag))
    command_list.append('python prep_dnn.py --channel "Leptonic" --input "../Loopers/MVABaby_ttHLeptonic_%s_RunII_MVA_Presel.root" --tag "ttGG" --signal "ttH" --background "ttGG" --z_score' % (args.tag))
    parallel_utils.submit_jobs(command_list, 2)

    parallel_utils.run('python train.py --input "ttHLeptonic_%s_RunII_MVA_Presel_featuresttGG.hdf5" --channel "Leptonic" --tag "_%s_RunII_MVA_Presel_ttH_vs_ttGG" --ext ""' % (args.tag, args.tag))
    parallel_utils.run('python train_dnn.py --input "ttHLeptonic_%s_RunII_MVA_Presel_dnn_features_ttGG.hdf5" --tag "ttHLeptonic_ttH_vs_ttGG_%s" --channel "Leptonic" --preprocess_scheme "preprocess_scheme_Leptonic_ttGG.json"' % (args.tag, args.tag), True)

    parallel_utils.run('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_RunII_MVA_Presel.root" --channel "Leptonic" --tag "addDNNs" --dnn_models "dnn_weights/metadata_Leptonic_ttHLeptonic_ttH_vs_ttGG_%s.json"' % (args.tag, args.tag))
    parallel_utils.run('python train.py --input "ttHLeptonic_%s_RunII_MVA_Presel_featuresaddDNNs.hdf5" --channel "Leptonic" --tag "_%s_RunII_MVA_Presel_addDNNs" --ext ""' % (args.tag, args.tag))

do_srs = False
if do_srs:
    os.chdir("../Loopers/")
    parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_SR_Inclusive" --bkg_options "none" --bdt "none"' % (args.baby_version, args.tag))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHLeptonic_RunII_SR_Inclusive_%s_histogramsRunII.root" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Leptonic|Signal Regions"' % (args.tag, plot_type))

    #os.chdir("../")
    #for sr in [0, 1, 2, 3]:
    #    parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_SR_%s" --bkg_options "none" --bdt "none"' % (args.baby_version, args.tag, str(sr)))
    #    os.chdir("Plots")
    #    for plot_type in plot_types:
    #        parallel_utils.run('python plot_wrapper.py --input_file "../ttHLeptonic_RunII_SR_%s_%s_histogramsRunII.root" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Leptonic|Signal Region %s"' % (str(sr), args.tag, plot_type, str(sr)))
    #    os.chdir("../")

do_syst = False
if do_syst:
    os.chdir("../Loopers/")
    parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --bdt "none" --do_systematics' % (args.baby_version, args.tag))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Leptonic|Loose MVA Presel."' % (args.tag, plot_type))

do_l1_prefire = False
if do_l1_prefire:
    os.chdir("../Loopers")
    for sr in [0, 1, 2, 3]:
        parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_SR_%s" --bkg_options "none" --bdt "none" --l1_prefire "undo"' % (args.baby_version, args.tag + "_undoL1Prefire", str(sr)))
        parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_SR_%s" --bkg_options "none" --bdt "none" --l1_prefire "up"' % (args.baby_version, args.tag + "_L1PrefireUp", str(sr)))
        parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_SR_%s" --bkg_options "none" --bdt "none" --l1_prefire "down"' % (args.baby_version, args.tag + "_L1PrefireDown", str(sr)))
        parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_SR_%s" --bkg_options "none" --bdt "none" --l1_prefire ""' % (args.baby_version, args.tag + "", str(sr)))

do_paper_plots = True
if do_paper_plots:
    os.chdir("../Loopers/")
    parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --bdt "none" --do_systematics' % (args.baby_version, args.tag))
    os.chdir("Plots")
    #for plot_type in plot_types:
    #    parallel_utils.run('python plot_wrapper.py --input_file "../ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG|Other" --signals "ttH" --plot_type "%s" --plot_labels ""' % (args.tag, plot_type))
