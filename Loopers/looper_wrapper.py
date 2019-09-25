import sys, os
import glob

sys.path.append("~/Utilities")

import parallel_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--channel", help = "Hadronic or Leptonic", type=str)
parser.add_argument("--babymaker", help = "run mva_babymaker instead of looper", action="store_true")
parser.add_argument("--baby_version", help = "version to match babies", type=str)
parser.add_argument("--tag", help = "tag to identify these histograms", type=str)
parser.add_argument("--selection", help = "selection for tagger", type=str)
parser.add_argument("--bdt", help = "bdt to evaluate", type=str, default="none")
parser.add_argument("--bkg_options", help = "how to treat bkg", type=str, default="none")
parser.add_argument("--years", help = "which years to run for", type=str, default="2016,2017,2018")
parser.add_argument("--fcnc", help = "run babymaker with fcnc as signal, ttH as bkg", action="store_true")
args = parser.parse_args()

if args.fcnc:
  args.tag += "_FCNC"

babies_2016 = [
    # Data
    "DoubleEG_Run2016B-17Jul2018_ver2-v1",
    "DoubleEG_Run2016C-17Jul2018-v1",
    "DoubleEG_Run2016D-17Jul2018-v1",
    "DoubleEG_Run2016E-17Jul2018-v1",
    "DoubleEG_Run2016F-17Jul2018-v1",
    "DoubleEG_Run2016G-17Jul2018-v1",
    "DoubleEG_Run2016H-17Jul2018-v1",

    # ttH
    "ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_v2_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    #"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_v2_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "ttHToGG_M125_13TeV_powheg_pythia8_v2_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",

    # Other Higgs production modes
    "GluGluHToGG_M-120_13TeV_powheg_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "GluGluHToGG_M-130_13TeV_powheg_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "GluGluHToGG_M120_13TeV_amcatnloFXFX_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "GluGluHToGG_M123_13TeV_amcatnloFXFX_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v2",
    "GluGluHToGG_M126_13TeV_amcatnloFXFX_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "GluGluHToGG_M127_13TeV_amcatnloFXFX_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "GluGluHToGG_M130_13TeV_amcatnloFXFX_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",

    "VBFHToGG_M-120_13TeV_powheg_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "VBFHToGG_M-130_13TeV_powheg_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "VBFHToGG_M120_13TeV_amcatnlo_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "VBFHToGG_M123_13TeV_amcatnlo_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "VBFHToGG_M124_13TeV_amcatnlo_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "VBFHToGG_M125_13TeV_amcatnlo_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2",
    "VBFHToGG_M125_13TeV_amcatnlo_pythia8_v2_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "VBFHToGG_M126_13TeV_amcatnlo_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "VBFHToGG_M127_13TeV_amcatnlo_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "VBFHToGG_M130_13TeV_amcatnlo_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",

    "VHToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "VHToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "VHToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "VHToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "VHToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",

    "bbHToGG_M-125_4FS_ybyt_13TeV_amcatnlo_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "bbHToGG_M-120_4FS_yb2_13TeV_amcatnlo_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "bbHToGG_M-125_4FS_yb2_13TeV_amcatnlo_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "bbHToGG_M-130_4FS_yb2_13TeV_amcatnlo_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "bbHToGG_M-130_4FS_ybyt_TuneCP5_13TeV-amcatnlo-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",

    "THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCUETP8M1_v2_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    #"THW_ctcvcp_HToGG_13TeV-madgraph-pythia8_TuneCUETP8M1_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "THW_ctcvcp_HToGG_M120_13TeV-madgraph-pythia8_TuneCUETP8M1_v2_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCUETP8M1_v2_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "THW_ctcvcp_HToGG_M130_13TeV-madgraph-pythia8_TuneCUETP8M1_v2_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "THQ_ctcvcp_HToGG_M120_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M123_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M124_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3",
    "THQ_ctcvcp_HToGG_M126_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "THQ_ctcvcp_HToGG_M127_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M130_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M123_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M124_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M126_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M127_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",

    # Background MC
    # QCD + X
    "QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "DiPhotonJetsBox_M40_80-Sherpa_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",

    # tt + X
    "TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",

    # V + X
    "DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1",
    "WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1",
    "WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1",

    # Rare
    "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1",
    "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1",
    "ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1",
    "tZq_ll_4f_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1",
    "TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2",
    "TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext2-v1",
    "TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v2",
    "WW_TuneCUETP8M1_13TeV-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "WZ_TuneCUETP8M1_13TeV-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "ZZ_TuneCUETP8M1_13TeV-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",

]

babies_2017 = [
    # Data
    "DoubleEG_Run2017B-31Mar2018-v1",
    "DoubleEG_Run2017C-31Mar2018-v1",
    "DoubleEG_Run2017D-31Mar2018-v1",
    "DoubleEG_Run2017E-31Mar2018-v1",
    "DoubleEG_Run2017F-31Mar2018-v1",

    # ttH
    "ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",

    # Other Higgs production modes
    "GluGluHToGG_M-120_13TeV_powheg_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GluGluHToGG_M-125_13TeV_powheg_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GluGluHToGG_M-130_13TeV_powheg_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GluGluHToGG_M120_13TeV_amcatnloFXFX_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GluGluHToGG_M123_13TeV_amcatnloFXFX_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GluGluHToGG_M124_13TeV_amcatnloFXFX_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "GluGluHToGG_M126_13TeV_amcatnloFXFX_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GluGluHToGG_M127_13TeV_amcatnloFXFX_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GluGluHToGG_M130_13TeV_amcatnloFXFX_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",

    "VBFHToGG_M-120_13TeV_powheg_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VBFHToGG_M-125_13TeV_powheg_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VBFHToGG_M-130_13TeV_powheg_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VBFHToGG_M120_13TeV_amcatnlo_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VBFHToGG_M123_13TeV_amcatnlo_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VBFHToGG_M124_13TeV_amcatnlo_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VBFHToGG_M125_13TeV_amcatnlo_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VBFHToGG_M126_13TeV_amcatnlo_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VBFHToGG_M127_13TeV_amcatnlo_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VBFHToGG_M130_13TeV_amcatnlo_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",

    "VHToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VHToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VHToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VHToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "VHToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",

    "bbHToGG_M-120_4FS_yb2_13TeV_amcatnlo_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "bbHToGG_M-125_4FS_yb2_13TeV_amcatnlo_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "bbHToGG_M-125_4FS_ybyt_TuneCP5_13TeV-amcatnlo-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "bbHToGG_M-130_4FS_yb2_13TeV_amcatnlo_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "bbHToGG_M-130_4FS_ybyt_TuneCP5_13TeV-amcatnlo-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",

    "THQ_ctcvcp_HToGG_M120_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M123_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M124_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3",
    "THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M126_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "THQ_ctcvcp_HToGG_M127_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M130_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M120_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M123_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M124_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M126_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M127_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M130_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",

    # Background MC
    # QCD + X
    "QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    #"GJets_HT-200To400_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "DiPhotonJetsBox_M40_80-Sherpa_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",

    # tt + X
    "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1",
    "TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1",

    # V + X
    "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1",    "WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3",
    "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3",

    # Rare
    "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "WW_TuneCP5_13TeV-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "WZ_TuneCP5_13TeV-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "ZZ_TuneCP5_13TeV-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v2",
    "tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-madgraph-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2",

]

babies_2018 = [
    # Data
    "EGamma_Run2018A-17Sep2018-v2",
    "EGamma_Run2018B-17Sep2018-v1",
    "EGamma_Run2018C-17Sep2018-v1",
    "EGamma_Run2018D-22Jan2019-v2",

    # ttH
    "ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "ttHJetToGG_M124_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",

    # Other Higgs production modes
    "GluGluHToGG_M-120_13TeV_powheg_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GluGluHToGG_M-125_13TeV_powheg_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GluGluHToGG_M-130_13TeV_powheg_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GluGluHToGG_M120_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GluGluHToGG_M123_13TeV_amcatnloFXFX_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GluGluHToGG_M124_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GluGluHToGG_M125_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GluGluHToGG_M126_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GluGluHToGG_M127_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "GluGluHToGG_M130_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",

    "VBFHToGG_M-120_TuneCP5_13TeV-powheg-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VBFHToGG_M-125_13TeV_powheg_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VBFHToGG_M120_13TeV_amcatnlo_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VBFHToGG_M123_13TeV_amcatnlo_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VBFHToGG_M124_13TeV_amcatnlo_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VBFHToGG_M125_13TeV_amcatnlo_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VBFHToGG_M126_13TeV_amcatnlo_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VBFHToGG_M127_13TeV_amcatnlo_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VBFHToGG_M130_13TeV_amcatnlo_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",

    "VHToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VHToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VHToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VHToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VHToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "VHToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",

    "bbHToGG_M-120_4FS_yb2_TuneCP5-13TeV-amcatnlo-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "bbHToGG_M-125_4FS_yb2_TuneCP5-13TeV-amcatnlo-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "bbHToGG_M-125_4FS_ybyt_TuneCP5_13TeV-amcatnlo-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "bbHToGG_M-130_4FS_yb2_TuneCP5_13TeV-amcatnlo-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "bbHToGG_M-130_4FS_ybyt_TuneCP5_13TeV-amcatnlo-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",

    "THQ_ctcvcp_HToGG_M120_TuneCP5_13TeV-madgraph-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "THQ_ctcvcp_HToGG_M130_TuneCP5_13TeV-madgraph-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    #"THQ_ctcvcp_HToGG_M120_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M123_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M124_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v3",
    "THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THQ_ctcvcp_HToGG_M126_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "THQ_ctcvcp_HToGG_M127_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    #"THQ_ctcvcp_HToGG_M130_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M120_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M123_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M124_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M126_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M127_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "THW_ctcvcp_HToGG_M130_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",

    # Background MC
    # QCD + X
    "QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    "GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2",
    "GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    "GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2",
    #"GJets_HT-40To100_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    #"GJets_HT-100To200_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    #"GJets_HT-200To400_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    #"GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1",
    #"GJets_HT-600ToInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "DiPhotonJetsBox_M40_80-Sherpa_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",

    # tt + X
    "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2",
    #"TTJets_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1",
    "TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2",

    # V + X
    "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v1",
    "WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1",
    "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2",

    # Rare
    "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1",
    "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1",
    "TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2",
    "tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-madgraph-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2",
    "WW_TuneCP5_13TeV-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2",
    "WZ_TuneCP5_13TeV-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v3",
    "ZZ_TuneCP5_13TeV-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2",
    "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2",
    "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8_RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2",
]

def full_path(baby):
  full_path_baby = "/home/users/sjmay/ttH/Loopers/merged_babies/" + baby + "_ttH_Babies_RunII" + args.baby_version + "/merged_ntuple.root"
  return [full_path_baby]

def little_babies(baby):
  babies = glob.glob("/home/users/sjmay/ttH/Loopers/merged_babies/" + baby + "_ttH_Babies_RunII" + args.baby_version + "/merged_ntuple_*.root")
  return babies

command_list = []
idx = 0

# MVA BabyMaker
if args.babymaker:
  if "2016" in args.years:
    for baby in babies_2016:
      for little_baby in little_babies(baby):
        command_list.append('./ttH%sMVABabyMaker "%s" "RunII" "%s" "%s" "%s" "%s" "%s" "%s"' % (args.channel, args.selection, args.tag, args.bkg_options, little_baby, "2016", "_" + str(idx), "FCNC" if args.fcnc else ""))
        idx += 1
  if "2017" in args.years:
    for baby in babies_2017:
      for little_baby in little_babies(baby):
        command_list.append('./ttH%sMVABabyMaker "%s" "RunII" "%s" "%s" "%s" "%s" "%s" "%s"' % (args.channel, args.selection, args.tag, args.bkg_options, little_baby, "2017", "_" + str(idx), "FCNC" if args.fcnc else ""))
        idx += 1
  if "2018" in args.years:
    for baby in babies_2018:
      for little_baby in little_babies(baby):
        command_list.append('./ttH%sMVABabyMaker "%s" "RunII" "%s" "%s" "%s" "%s" "%s" "%s"' % (args.channel, args.selection, args.tag, args.bkg_options, little_baby, "2018", "_" + str(idx), "FCNC" if args.fcnc else ""))
        idx += 1

# Loopers
else:
  if "2016" in args.years:
    for baby in babies_2016:
      for little_baby in little_babies(baby):
        command_list.append('./ttH%sLooper "%s" "RunII" "%s" "%s" "%s" "%s" "%s" "%s"' % (args.channel, args.selection, args.tag, args.bdt, args.bkg_options, little_baby, "2016", "_" + str(idx))) 
        idx += 1
  if "2017" in args.years:
    for baby in babies_2017:
      for little_baby in little_babies(baby):
        command_list.append('./ttH%sLooper "%s" "RunII" "%s" "%s" "%s" "%s" "%s" "%s"' % (args.channel, args.selection, args.tag, args.bdt, args.bkg_options, little_baby, "2017", "_" + str(idx)))    
        idx += 1
  if "2018" in args.years:
    for baby in babies_2018:
      for little_baby in little_babies(baby):
        command_list.append('./ttH%sLooper "%s" "RunII" "%s" "%s" "%s" "%s" "%s" "%s"' % (args.channel, args.selection, args.tag, args.bdt, args.bkg_options, little_baby, "2018", "_" + str(idx)))    
        idx += 1

nPar = 24
parallel_utils.submit_jobs(command_list, nPar)

if args.babymaker:
  histos = glob.glob("MVABaby_ttH%s_%s_*.root" % (args.channel, args.tag))
else:
  histos = glob.glob("%s_%s_histogramsRunII_*.root" % (args.selection, args.tag))
good_histos = []
for hist in histos:
  size = os.stat(hist).st_size * (1./(1024))
  if size >= 1.:
    good_histos.append(hist)
    print("good hist: %s, size (kb): %d" % (hist, os.stat(hist).st_size * (1./(1024))))
  else:
    print("bad  hist: %s, size (kb): %d" % (hist, os.stat(hist).st_size * (1./(1024))))

target = " "
for hist in good_histos:
  target += "%s " % hist

if args.babymaker:
  master = "MVABaby_ttH%s_%s.root" % (args.channel, args.tag)
else:
  master = "%s_%s_histogramsRunII.root" % (args.selection, args.tag)
os.system('/usr/bin/ionice -c2 -n7 hadd -f -k -j 2 %s %s' % (master, target))


# Cleanup
for hist in histos:
  os.system("rm %s" % hist)
