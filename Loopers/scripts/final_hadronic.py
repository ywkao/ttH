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

#plot_types = ["std", "std_linear", "std_shape", "std_shape_sig_vs_data", "std_2016", "std_2017", "std_2018"]
plot_types = ["std"]

derive_fake_shape = False
if derive_fake_shape:
    # Derive photon fake ID shape
    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_DiPhotonFits_Presel" --bkg_options "none" --bdt "none"' % (args.baby_version, args.tag))
    os.chdir("Plots")
    for plot_type in plot_types: 
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|GammaJets|QCD|TTGG|TTGJets|TTJets" --signals "ttH" --plot_type "%s" --plot_labels "ttH Hadronic|DiPhoton Fits Presel.|Pre-Fit"' % (args.tag, plot_type))

    os.chdir("../PhotonID_Sideband/")
    parallel_utils.run('python derive_shape_fake.py --input "../ttHHadronic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" --hist_name "hFakePhotonIDMVA"' % (args.tag))

    os.chdir("../")
    dummy_input = raw_input("You probably want to update ttHLooper.h with results of deriving fake shape. Press any key to continue")


do_diphoton_fits = False
if do_diphoton_fits:
    # Run imputing with no fit
    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute_no_scale" --bdt "none"' % (args.baby_version, args.tag + "_impute_no_scale"))
    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "none" --bdt "none"' % (args.baby_version, args.tag + "_no_scale"))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Hadronic|Loose MVA Presel.|Pre-Fit"' % (args.tag + "_impute_no_scale", plot_type))
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|GammaJets|QCD|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Hadronic|Loose MVA Presel.|Pre-Fit"' % (args.tag + "_no_scale", plot_type))

    # Do fit
    os.chdir("../tt_template_fit/")
    parallel_utils.run('python do_fits_impute.py --input_file "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" --jet_bin "2+"' % (args.tag + "_impute_no_scale"))
    parallel_utils.run('python do_fits_qcd.py --input_file "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" --jet_bin "2+"' % (args.tag + "_no_scale"))

    os.chdir("../")
    dummy_input = raw_input("You probably want to update ttHLooper.h with results of DiPhoton Fits. Press any key to continue")


# Run imputing with fit
do_imputing = False
if do_imputing:
    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --bdt "none"' % (args.baby_version, args.tag + "_impute"))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Hadronic|Loose MVA Presel."' % (args.tag + "_impute", plot_type))

    # Run QCD + X with fit
    os.chdir("../")
    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "scale_diphoton" --bdt "none"' % (args.baby_version, args.tag + "_scale_diphoton"))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|GammaJets|QCD|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Hadronic|Loose MVA Presel."' % (args.tag + "_scale_diphoton", plot_type))
    os.chdir("../")

do_ttZ = False
if do_ttZ:
    #parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_ttZ_CR" --bkg_options "impute" --bdt "none"' % (args.baby_version, args.tag + "_impute"))
    os.chdir("Plots")
    #for plot_type in plot_types:
    #    parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_ttZ_CR_%s_histogramsRunII.root" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Hadronic|t#bar{t}Z CR|N_{jets} #geq 3|N_{b-jets} (loose) #geq 1"' % (args.tag + "_impute", plot_type))
    os.chdir("../")
    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_ttZ_Tight_CR" --bkg_options "impute" --bdt "none" --do_systematics' % (args.baby_version, args.tag + "_impute"))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_ttZ_Tight_CR_%s_histogramsRunII.root" --backgrounds "DY|Other2|TTZ" --signals "ttH" --plot_type "%s" --plot_labels "Hadronic Channel|t#bar{t}Z Control Region|N_{jets} #geq 5|N_{b-jets} (tight) #geq 2"' % (args.tag + "_impute", plot_type))


do_mvas = False
if do_mvas:
    # Make MVA Babies
    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --bdt "none" --babymaker' % (args.baby_version, args.tag + "_RunII_MVA_Presel_impute"))
    #parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "scale_diphoton" --bdt "none" --babymaker' % (args.baby_version, args.tag + "_RunII_MVA_Presel_scale_diphoton"))

    os.chdir("../MVAs/")
    #os.system("source setup.sh")
    # Prep and train: scale_diphoton vs. imputing
    command_list = []
    #command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_impute.root" --channel "Hadronic" --tag ""' % (args.tag))
    #command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_scale_diphoton.root" --channel "Hadronic" --tag ""' % (args.tag))
    parallel_utils.submit_jobs(command_list, 2)

    #parallel_utils.run('python train.py --input "ttHHadronic_%s_RunII_MVA_Presel_impute_features.hdf5" --channel "Hadronic" --tag "_%s_RunII_MVA_Presel_impute" --ext ""' % (args.tag, args.tag))
    #parallel_utils.run('python train.py --input "ttHHadronic_%s_RunII_MVA_Presel_scale_diphoton_features.hdf5" --channel "Hadronic" --tag "_%s_RunII_MVA_Presel_scale_diphoton" --ext ""' % (args.tag, args.tag))
    #parallel_utils.run('python make_za_comparisons.py --inputs "za_Hadronic___%s_RunII_MVA_Presel_scale_diphoton.npz,za_Hadronic___%s_RunII_MVA_Presel_impute.npz" --labels "MC Description,Data-driven Description"' % (args.tag, args.tag))

do_dnns = False
if do_dnns:
    os.chdir("../MVAs/")
    #os.system("source setup.sh")
    
    command_list = []
    command_list.append('python prep_dnn.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_impute.root" --channel "Hadronic" --signal "ttH" --background "dipho" --tag "dipho" --z_score' % (args.tag))
    command_list.append('python prep_dnn.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_impute.root" --channel "Hadronic" --signal "ttH" --background "ttGG" --tag "ttGG" --z_score' % (args.tag))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_impute.root" --channel "Hadronic" --tag "dipho" --dipho_only' % (args.tag))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_impute.root" --channel "Hadronic" --tag "ttGG" --ttGG_only' % (args.tag))
    parallel_utils.submit_jobs(command_list, 4)

    parallel_utils.run('python train.py --input "ttHHadronic_%s_RunII_MVA_Presel_impute_featuresdipho.hdf5" --channel "Hadronic" --tag "_%s_RunII_MVA_Presel_impute_ttH_vs_dipho" --ext ""' % (args.tag, args.tag))
    parallel_utils.run('python train.py --input "ttHHadronic_%s_RunII_MVA_Presel_impute_featuresttGG.hdf5" --channel "Hadronic" --tag "_%s_RunII_MVA_Presel_impute_ttH_vs_ttGG" --ext ""' % (args.tag, args.tag))

    parallel_utils.run('python train_dnn.py --input "ttHHadronic_%s_RunII_MVA_Presel_impute_dnn_features_dipho.hdf5" --tag "ttHHadronic_ttH_vs_dipho_%s" --channel "Hadronic" --preprocess_scheme "preprocess_scheme_Hadronic_dipho.json"' % (args.tag, args.tag), True)
    parallel_utils.run('python train_dnn.py --input "ttHHadronic_%s_RunII_MVA_Presel_impute_dnn_features_ttGG.hdf5" --tag "ttHHadronic_ttH_vs_ttGG_%s" --channel "Hadronic" --preprocess_scheme "preprocess_scheme_Hadronic_ttGG.json"' % (args.tag, args.tag), True)

    parallel_utils.run('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_impute.root" --channel "Hadronic" --tag "addDNNs" --dnn_models "dnn_weights/metadata_Hadronic_ttHHadronic_ttH_vs_dipho_%s.json,dnn_weights/metadata_Hadronic_ttHHadronic_ttH_vs_ttGG_%s.json"' % (args.tag, args.tag, args.tag))
    parallel_utils.run('python train.py --input "ttHHadronic_%s_RunII_MVA_Presel_impute_featuresaddDNNs.hdf5" --channel "Hadronic" --tag "_%s_RunII_MVA_Presel_impute_addDNNs" --ext ""' % (args.tag, args.tag))

    parallel_utils.run('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_impute.root" --channel "Hadronic" --tag "addDNNs_addTopTag" --do_top_tag --dnn_models "dnn_weights/metadata_Hadronic_ttHHadronic_ttH_vs_dipho_%s.json,dnn_weights/metadata_Hadronic_ttHHadronic_ttH_vs_ttGG_%s.json"' % (args.tag, args.tag, args.tag))
    parallel_utils.run('python train.py --input "ttHHadronic_%s_RunII_MVA_Presel_impute_featuresaddDNNs_addTopTag.hdf5" --channel "Hadronic" --tag "_%s_RunII_MVA_Presel_impute_addDNNs_addTopTag" --ext ""' % (args.tag, args.tag))

do_srs = False
if do_srs:
    os.chdir("../Loopers")
    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_SR_Inclusive" --bkg_options "impute" --bdt "none"' % (args.baby_version, args.tag + "_impute"))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_SR_Inclusive_%s_histogramsRunII.root" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Hadronic|Signal Regions"' % (args.tag + "_impute", plot_type))
    os.chdir("../")
    #for sr in [0, 1, 2, 3]:
    #    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_SR_%s" --bkg_options "impute" --bdt "none"' % (args.baby_version, args.tag + "_impute", str(sr)))
    #    os.chdir("Plots")
    #    for plot_type in plot_types:
    #        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_SR_%s_%s_histogramsRunII.root" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Hadronic|Signal Region %s"' % (str(sr), args.tag + "_impute", plot_type, str(sr)))
    #    os.chdir("../")

do_syst = False
if do_syst:
    os.chdir("../Loopers/")
    #parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --bdt "none" --do_systematics' % (args.baby_version, args.tag + "_impute"))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|DY|VG|TGamma|TTV|VV|tV" --signals "ttH" --plot_type "%s" --plot_labels "ttH Hadronic|Loose MVA Presel."' % (args.tag + "_impute", plot_type))

do_l1_prefire = False
if do_l1_prefire:
    os.chdir("../Loopers")
    for sr in [0, 1, 2, 3]:
        parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_SR_%s" --bkg_options "impute" --bdt "none" --l1_prefire "undo"' % (args.baby_version, args.tag + "_impute_undoL1Prefire", str(sr)))
        parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_SR_%s" --bkg_options "impute" --bdt "none" --l1_prefire "up"' % (args.baby_version, args.tag + "_impute_L1PrefireUp", str(sr)))
        parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_SR_%s" --bkg_options "impute" --bdt "none" --l1_prefire "down"' % (args.baby_version, args.tag + "_impute_L1PrefireDown", str(sr)))
        parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_SR_%s" --bkg_options "impute" --bdt "none" --l1_prefire ""' % (args.baby_version, args.tag + "_impute", str(sr)))
        

do_paper_plots = True
if do_paper_plots:
    os.chdir("../Loopers/")
    #parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --bdt "none" --do_systematics' % (args.baby_version, args.tag + "_impute"))
    os.chdir("Plots")
    for plot_type in plot_types:
        parallel_utils.run('python plot_wrapper.py --input_file "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG|Other" --signals "ttH" --plot_type "%s" --plot_labels ""' % (args.tag + "_impute", plot_type))

 
