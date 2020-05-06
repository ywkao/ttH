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

bdt_lep_hut = "../MVAs/Leptonic_v4.11_14Jan2020_hut__bdt.xml"
bdt_lep_hct = "../MVAs/Leptonic_v4.11_14Jan2020_hct__bdt.xml"
bdt_had_hut = "../MVAs/Hadronic_v4.11_14Jan2020_impute_hut__bdt.xml"
bdt_had_hct = "../MVAs/Hadronic_v4.11_14Jan2020_impute_hct__bdt.xml"

os.chdir("../")

do_looping = False
if do_looping:
    parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none"  --bdt "%s"' % (args.baby_version, args.tag + "_hut_BDT", bdt_lep_hut))
    parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute"  --bdt "%s"' % (args.baby_version, args.tag + "_impute_hut_BDT", bdt_had_hut))
    parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none"  --bdt "%s"' % (args.baby_version, args.tag + "_hct_BDT", bdt_lep_hct))
    parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute"  --bdt "%s"' % (args.baby_version, args.tag + "_impute_hct_BDT", bdt_had_hct))

    os.chdir("Plots")
    parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std" --plot_labels "FCNC Leptonic|Loose MVA Presel." --signals "TT_FCNC_hut|ST_FCNC_hut" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG"' % ("ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_hut_BDT_FCNC")))
    parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std" --plot_labels "FCNC Leptonic|Loose MVA Presel." --signals "TT_FCNC_hct|ST_FCNC_hct" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG"' % ("ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_hct_BDT_FCNC")))

    parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std" --plot_labels "FCNC Hadronic|Loose MVA Presel." --signals "TT_FCNC_hut|ST_FCNC_hut" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG"' % ("ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_impute_hut_BDT_FCNC")))
    parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std" --plot_labels "FCNC Hadronic|Loose MVA Presel." --signals "TT_FCNC_hct|ST_FCNC_hct" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG"' % ("ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_impute_hct_BDT_FCNC")))

    os.chdir("../")

do_babies = False
if do_babies:
    parallel_utils.run('python looper_wrapper.py --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --babymaker --fcnc' % (args.baby_version, args.tag))
    parallel_utils.run('python looper_wrapper.py --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --babymaker --fcnc' % (args.baby_version, args.tag + "_impute"))

do_mva_prep = False
if do_mva_prep:
    os.chdir("../MVAs/")
    command_list = []

    bdt_training_features_base = ["helicity_angle_", "dipho_pt_over_mass_", "met_", "dipho_rapidity_", "dipho_cosphi_", "subleadPSV_", "leadPSV_", "jet3_btag_", "jet3_eta_", "jet3_pt_", "jet2_btag_", "jet2_eta_", "jet2_pt_", "jet1_btag_", "jet1_eta_", "jet1_pt_", "sublead_eta_", "lead_eta_", "subleadptoM_", "leadptoM_", "ht_", "njets_", "dipho_delta_R", "max1_btag_", "max2_btag_", "minIDMVA_", "maxIDMVA_"]

    bdt_training_features_hadronic = bdt_training_features_base + ["jet4_btag_", "jet4_eta_", "jet4_pt_", "m_ggj_", "m_jjj_"]
    bdt_training_features_leptonic = bdt_training_features_base + ["lep_pt_", "lep_eta_", "n_lep_tight_"]

    top_tagger_bdt = ["top_tag_score_"]
    top_chi2_hadronic = ["chi2_tbw_mass_", "chi2_tbw_pt_", "chi2_tbw_eta_", "chi2_tbw_deltaR_dipho_", "chi2_qjet_pt_", "chi2_qjet_eta_", "chi2_qjet_btag_", "chi2_qjet_deltaR_dipho_", "chi2_tqh_ptOverM_", "chi2_tqh_eta_", "chi2_tqh_deltaR_tbw_", "chi2_tqh_deltaR_dipho_", "chi2_3x3_tbw_mass_", "chi2_3x3_tbw_pt_", "chi2_3x3_tbw_eta_", "chi2_3x3_tbw_deltaR_dipho_", "chi2_3x3_qjet_pt_", "chi2_3x3_qjet_eta_", "chi2_3x3_qjet_btag_", "chi2_3x3_qjet_deltaR_dipho_", "chi2_3x3_tqh_ptOverM_", "chi2_3x3_tqh_eta_", "chi2_3x3_tqh_deltaR_tbw_", "chi2_3x3_tqh_deltaR_dipho_"]
    top_chi2_leptonic = ["chi2_neutrino_pz_", "chi2_tbw_mass_", "chi2_tbw_pt_", "chi2_tbw_eta_", "chi2_tbw_deltaR_dipho_", "chi2_qjet_pt_", "chi2_qjet_eta_", "chi2_qjet_btag_", "chi2_qjet_deltaR_dipho_", "chi2_tqh_ptOverM_", "chi2_tqh_eta_", "chi2_tqh_deltaR_tbw_", "chi2_tqh_deltaR_dipho_"]

    bdt_training_features_leptonic_toptagger = bdt_training_features_leptonic + top_chi2_leptonic
    bdt_training_features_hadronic_toptagger = bdt_training_features_hadronic + top_tagger_bdt + top_chi2_hadronic

    non_resonant_bkg = "dy,dipho,ttgg,ttg,vgamma,tt,tgamma,ttz,vv,tv,ttw"
    non_resonant_bkg_hadronic = non_resonant_bkg + ",qcd_gjets"
    non_resonant_bkg_leptonic = non_resonant_bkg + ",gjets"
    sm_higgs = "tth,thq,thw,ggh,vbf,vh"

    bdt_training_features_hadronic = ",".join(bdt_training_features_hadronic)
    bdt_training_features_leptonic = ",".join(bdt_training_features_leptonic)
    bdt_training_features_hadronic_toptagger = ",".join(bdt_training_features_hadronic_toptagger)
    bdt_training_features_leptonic_toptagger = ",".join(bdt_training_features_leptonic_toptagger)

    # Leptonic BDT Baseline
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --tag "Hut" --signal "tt_fcnc_hut" --bkg "%s" --features "%s" --tag "baseline_hut"' % (args.tag, non_resonant_bkg_leptonic + "," + sm_higgs, bdt_training_features_leptonic))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --tag "Hct" --signal "tt_fcnc_hct" --bkg "%s" --features "%s" --tag "baseline_hct"' % (args.tag, non_resonant_bkg_leptonic + "," + sm_higgs, bdt_training_features_leptonic))

    # Hadronic BDT Baseline
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_impute_FCNC.root" --channel "Hadronic" --tag "Hut" --signal "tt_fcnc_hut" --bkg "%s" --features "%s" --tag "baseline_hut"' % (args.tag, non_resonant_bkg_hadronic + "," + sm_higgs, bdt_training_features_hadronic))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_impute_FCNC.root" --channel "Hadronic" --tag "Hct" --signal "tt_fcnc_hct" --bkg "%s" --features "%s" --tag "baseline_hct"' % (args.tag, non_resonant_bkg_hadronic + "," + sm_higgs, bdt_training_features_hadronic))
    
    # Leptonic BDT Top Tagger
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --tag "Hut" --signal "tt_fcnc_hut" --bkg "%s" --features "%s" --tag "addTopTagger_hut"' % (args.tag, non_resonant_bkg_leptonic + "," + sm_higgs, bdt_training_features_leptonic_toptagger))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --tag "Hct" --signal "tt_fcnc_hct" --bkg "%s" --features "%s" --tag "addTopTagger_hct"' % (args.tag, non_resonant_bkg_leptonic + "," + sm_higgs, bdt_training_features_leptonic_toptagger))

    # Hadronic BDT Top Tagger
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_impute_FCNC.root" --channel "Hadronic" --tag "Hut" --signal "tt_fcnc_hut" --bkg "%s" --features "%s" --tag "addTopTagger_hut"' % (args.tag, non_resonant_bkg_hadronic + "," + sm_higgs, bdt_training_features_hadronic_toptagger))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_impute_FCNC.root" --channel "Hadronic" --tag "Hct" --signal "tt_fcnc_hct" --bkg "%s" --features "%s" --tag "addTopTagger_hct"' % (args.tag, non_resonant_bkg_hadronic + "," + sm_higgs, bdt_training_features_hadronic_toptagger))

    # Leptonic Non-Resonant BDT
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --tag "Hut" --signal "tt_fcnc_hut" --bkg "%s" --features "%s" --tag "nonres_hut"' % (args.tag, non_resonant_bkg_leptonic, bdt_training_features_leptonic_toptagger))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --tag "Hct" --signal "tt_fcnc_hct" --bkg "%s" --features "%s" --tag "nonres_hct"' % (args.tag, non_resonant_bkg_leptonic, bdt_training_features_leptonic_toptagger))

    # Hadronic Non-Resonant BDT
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_impute_FCNC.root" --channel "Hadronic" --tag "Hut" --signal "tt_fcnc_hut" --bkg "%s" --features "%s" --tag "nonres_hut"' % (args.tag, non_resonant_bkg_hadronic, bdt_training_features_hadronic_toptagger))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_impute_FCNC.root" --channel "Hadronic" --tag "Hct" --signal "tt_fcnc_hct" --bkg "%s" --features "%s" --tag "nonres_hct"' % (args.tag, non_resonant_bkg_hadronic, bdt_training_features_hadronic_toptagger)) 

    # Leptonic SM Higgs BDT
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --tag "Hut" --signal "tt_fcnc_hut" --bkg "%s" --features "%s" --tag "smhiggs_hut"' % (args.tag, sm_higgs, bdt_training_features_leptonic_toptagger))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --tag "Hct" --signal "tt_fcnc_hct" --bkg "%s" --features "%s" --tag "smhiggs_hct"' % (args.tag, sm_higgs, bdt_training_features_leptonic_toptagger))

    # Hadronic SM Higgs BDT
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_impute_FCNC.root" --channel "Hadronic" --tag "Hut" --signal "tt_fcnc_hut" --bkg "%s" --features "%s" --tag "smhiggs_hut"' % (args.tag, sm_higgs, bdt_training_features_hadronic_toptagger))
    command_list.append('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_impute_FCNC.root" --channel "Hadronic" --tag "Hct" --signal "tt_fcnc_hct" --bkg "%s" --features "%s" --tag "smhiggs_hct"' % (args.tag, sm_higgs, bdt_training_features_hadronic_toptagger))

    parallel_utils.submit_jobs(command_list, 4)

do_mvas = True
if do_mvas:
    os.chdir("../MVAs/")
    
    command_list = []
    # Leptonic Baseline BDT
    #command_list.append('python train_bdt.py --input "ttHLeptonic_%s_FCNC_features_baseline_hut.hdf5" --channel "Leptonic" --tag "baseline_hut_%s"' % (args.tag, args.tag))
    #command_list.append('python train_bdt.py --input "ttHLeptonic_%s_FCNC_features_baseline_hct.hdf5" --channel "Leptonic" --tag "baseline_hct_%s"' % (args.tag, args.tag))

    # Hadronic Baseline BDT
    command_list.append('python train_bdt.py --input "ttHHadronic_%s_impute_FCNC_features_baseline_hut.hdf5" --channel "Hadronic" --tag "baseline_hut_%s"' % (args.tag, args.tag))
    command_list.append('python train_bdt.py --input "ttHHadronic_%s_impute_FCNC_features_baseline_hct.hdf5" --channel "Hadronic" --tag "baseline_hct_%s"' % (args.tag, args.tag))

    # Leptonic AddTopTagger BDT
    #command_list.append('python train_bdt.py --input "ttHLeptonic_%s_FCNC_features_addTopTagger_hut.hdf5" --channel "Leptonic" --tag "addTopTagger_hut_%s"' % (args.tag, args.tag))
    #command_list.append('python train_bdt.py --input "ttHLeptonic_%s_FCNC_features_addTopTagger_hct.hdf5" --channel "Leptonic" --tag "addTopTagger_hct_%s"' % (args.tag, args.tag))

    # Hadronic AddTopTagger BDT
    command_list.append('python train_bdt.py --input "ttHHadronic_%s_impute_FCNC_features_addTopTagger_hut.hdf5" --channel "Hadronic" --tag "addTopTagger_hut_%s"' % (args.tag, args.tag))
    command_list.append('python train_bdt.py --input "ttHHadronic_%s_impute_FCNC_features_addTopTagger_hct.hdf5" --channel "Hadronic" --tag "addTopTagger_hct_%s"' % (args.tag, args.tag))

    # Leptonic Non-Resonant BDT 
    #command_list.append('python train_bdt.py --input "ttHLeptonic_%s_FCNC_features_nonres_hut.hdf5" --channel "Leptonic" --tag "nonres_hut_%s"' % (args.tag, args.tag))
    #command_list.append('python train_bdt.py --input "ttHLeptonic_%s_FCNC_features_nonres_hct.hdf5" --channel "Leptonic" --tag "nonres_hct_%s"' % (args.tag, args.tag))

    # Hadronic Non-Resonant BDT 
    #command_list.append('python train_bdt.py --input "ttHHadronic_%s_impute_FCNC_features_nonres_hut.hdf5" --channel "Hadronic" --tag "nonres_hut_%s"' % (args.tag, args.tag))
    #command_list.append('python train_bdt.py --input "ttHHadronic_%s_impute_FCNC_features_nonres_hct.hdf5" --channel "Hadronic" --tag "nonres_hct_%s"' % (args.tag, args.tag))

    # Leptonic SM Higgs BDT 
    #command_list.append('python train_bdt.py --input "ttHLeptonic_%s_FCNC_features_smhiggs_hut.hdf5" --channel "Leptonic" --tag "smhiggs_hut_%s"' % (args.tag, args.tag))
    #command_list.append('python train_bdt.py --input "ttHLeptonic_%s_FCNC_features_smhiggs_hct.hdf5" --channel "Leptonic" --tag "smhiggs_hct_%s"' % (args.tag, args.tag))

    # Hadronic SM Higgs BDT 
    #command_list.append('python train_bdt.py --input "ttHHadronic_%s_impute_FCNC_features_smhiggs_hut.hdf5" --channel "Hadronic" --tag "smhiggs_hut_%s"' % (args.tag, args.tag))
    #command_list.append('python train_bdt.py --input "ttHHadronic_%s_impute_FCNC_features_smhiggs_hct.hdf5" --channel "Hadronic" --tag "smhiggs_hct_%s"' % (args.tag, args.tag))

    parallel_utils.submit_jobs(command_list, 4)

do_trees = True
if do_trees:
    os.chdir("../MVAs/")
    
    # Leptonic merge
    parallel_utils.run('python make_optimization_tree.py --input "ttHLeptonic_%s_FCNC_features_addTopTagger_hut.hdf5" --channel "Leptonic" --tag "hut_merge2d_%s" --mvas "Leptonic_addTopTagger_hut_%s_bdt.xgb,Leptonic_nonres_hut_%s_bdt.xgb,Leptonic_smhiggs_hut_%s_bdt.xgb" --names "mva_score,mva_nonres_score,mva_smhiggs_score"' % (args.tag, args.tag, args.tag, args.tag, args.tag))
    parallel_utils.run('python make_optimization_tree.py --input "ttHLeptonic_%s_FCNC_features_addTopTagger_hct.hdf5" --channel "Leptonic" --tag "hct_merge2d_%s" --mvas "Leptonic_addTopTagger_hct_%s_bdt.xgb,Leptonic_nonres_hct_%s_bdt.xgb,Leptonic_smhiggs_hct_%s_bdt.xgb" --names "mva_score,mva_nonres_score,mva_smhiggs_score"' % (args.tag, args.tag, args.tag, args.tag, args.tag))
    
    # Hadronic merge
    parallel_utils.run('python make_optimization_tree.py --input "ttHHadronic_%s_impute_FCNC_features_addTopTagger_hut.hdf5" --channel "Hadronic" --tag "hut_merge2d_%s" --mvas "Hadronic_addTopTagger_hut_%s_bdt.xgb,Hadronic_nonres_hut_%s_bdt.xgb,Hadronic_smhiggs_hut_%s_bdt.xgb" --names "mva_score,mva_nonres_score,mva_smhiggs_score"' % (args.tag, args.tag, args.tag, args.tag, args.tag))
    parallel_utils.run('python make_optimization_tree.py --input "ttHHadronic_%s_impute_FCNC_features_addTopTagger_hct.hdf5" --channel "Hadronic" --tag "hct_merge2d_%s" --mvas "Hadronic_addTopTagger_hct_%s_bdt.xgb,Hadronic_nonres_hct_%s_bdt.xgb,Hadronic_smhiggs_hct_%s_bdt.xgb" --names "mva_nonres,mva_smhiggs"' % (args.tag, args.tag, args.tag, args.tag, args.tag))
