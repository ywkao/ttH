double scale1fb_2018(TString currentFileTitle) {
  std::map<TString, double> m = {
        {"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5", 0.0000000000},
        {"WW_TuneCP5_13TeV-pythia8", 0.0000000000},
        {"TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8", 0.0000000000},
        {"DiPhotonJetsBox_M40_80-Sherpa", 0.0000000000},
        {"VBFHToGG_M125_13TeV_amcatnlo_pythia8", 0.0000000000},
        {"DiPhotonJets_MGG-80toInf_13TeV_amcatnloFXFX_pythia8", 0.0000000000},
        {"TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8", 0.0000000000},
        {"GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8", 0.0000000000},
        {"GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"ttHJetToGG_M65_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"WZ_TuneCP5_13TeV-pythia8", 0.0000000000},
        {"THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5", 0.0000000000},
        {"GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8", 0.0000000000},
        {"ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M115_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M110_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CP5Down", 0.0000000000},
        {"DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8", 0.0000000000},
        {"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M90_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8", 0.0000000000},
        {"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8", 0.0000000000},
        {"ZZ_TuneCP5_13TeV-pythia8", 0.0000000000},
        {"TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8", 0.0000000000},
        {"TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8", 0.0000000000},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8", 0.0000000000},
        {"ttHToGG_M125_13TeV_powheg_pythia8", 0.0000000000},
        {"ttHJetToGG_M95_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_PSWeights", 0.0000000000},
        {"WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"ttHJetToGG_M80_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8", 0.0000000000},
        {"GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8", 0.0000000000},
        {"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa", 0.0000000000},
        {"ttHJetToGG_M75_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M105_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"ttHJetToGG_M70_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M60_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  };
  TObjArray *tx = currentFileTitle.Tokenize("/");
  TString key = ((TObjString *)(tx->At(tx->GetEntries()-2)))->String();
  std::map<TString, double>::iterator it;
  for (it = m.begin(); it != m.end(); it++) {
    if (key.Contains(it->first) && it->second > 0) {
      return it->second;
    }
  }
  cout << "Did not match " << key << " to a sample in the scale1fb map!" << endl;
}