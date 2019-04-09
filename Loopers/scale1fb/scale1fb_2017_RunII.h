double scale1fb_2017(TString currentFileTitle) {
  std::map<TString, double> m = {
        {"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0131740441},
        {"THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5", 0.0000000357},
        {"WW_TuneCP5_13TeV-pythia8", 0.0081126235},
        {"TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8", 0.0005012288},
        {"DiPhotonJetsBox_M40_80-Sherpa", 0.1103250733},
        {"VBFHToGG_M125_13TeV_amcatnlo_pythia8", 0.0000011127},
        {"DiPhotonJets_MGG-80toInf_13TeV_amcatnloFXFX_pythia8", 0.0001207668},
        {"TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8", 0.0000671414},
        {"GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8", 0.0000005151},
        {"GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8", 3.3460809924},
        {"ttHJetToGG_M65_13TeV_amcatnloFXFX_madspin_pythia8", 0.0005043159},
        {"GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8", 0.1179531432},
        {"WZ_TuneCP5_13TeV-pythia8", 0.0058086407},
        {"THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5", 0.0000000893},
        {"GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8", 0.8668177815},
        {"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0110810550},
        {"ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000037138},
        {"ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8", 0.0000650475},
        {"ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000037046},
        {"ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000037379},
        {"ttHJetToGG_M115_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000024396},
        {"ttHJetToGG_M110_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000022707},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CP5Down", 0.0689043693},
        {"DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8", 0.0000015174},
        {"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8", 0.0006708954},
        {"ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000017449},
        {"ttHJetToGG_M90_13TeV_amcatnloFXFX_madspin_pythia8", 0.0008231405},
        {"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8", 5.7410435024},
        {"tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8", 0.0000766003},
        {"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8", 0.0752378014},
        {"VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000012495},
        {"TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8", 0.0000228745},
        {"ZZ_TuneCP5_13TeV-pythia8", 0.0053584474},
        {"TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8", 0.0000382420},
        {"TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8", 0.0000026226},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000028126},
        {"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 1.6912594607},
        {"GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8", 0.1595262779},
        {"TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8", 0.0001344319},
        {"ttHToGG_M125_13TeV_powheg_pythia8", 0.0000022835},
        {"ttHJetToGG_M95_13TeV_amcatnloFXFX_madspin_pythia8", 0.0012666571},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_PSWeights", 0.0009929994},
        {"WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0645042945},
        {"ttHJetToGG_M80_13TeV_amcatnloFXFX_madspin_pythia8", 0.0006643635},
        {"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000037555},
        {"TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8", 0.0000601096},
        {"GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0555372610},
        {"ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8", 0.0000667924},
        {"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa", 0.0038391582},
        {"ttHJetToGG_M75_13TeV_amcatnloFXFX_madspin_pythia8", 0.0006827760},
        {"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000036485},
        {"ttHJetToGG_M105_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000019049},
        {"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 5.7268841667},
        {"ttHJetToGG_M70_13TeV_amcatnloFXFX_madspin_pythia8", 0.0006850542},
        {"ttHJetToGG_M60_13TeV_amcatnloFXFX_madspin_pythia8", 0.0004923791},
        {"ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000037432},
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