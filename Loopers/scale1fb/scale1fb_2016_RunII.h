double scale1fb_2016(TString currentFileTitle) {
  std::map<TString, double> m = {
        {"TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8", 0.0006896736},
        {"TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8", 0.0000000000},
        {"TTJets_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"DiPhotonJets_MGG-80toInf_13TeV_amcatnloFXFX_pythia8", 0.0000000000},
        {"TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8", 0.0000000000},
        {"TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8", 0.0000378117},
        {"THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5", 0.0000000000},
        {"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000030614},
        {"VBFHToGG_M126_13TeV_amcatnlo_pythia8", 0.0000000000},
        {"VBFHToGG_M125_13TeV_amcatnlo_pythia8", 0.0000000000},
        {"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000059052},
        {"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"WZ_TuneCUETP8M1_13TeV-pythia8", 0.0057084665},
        {"ttHJetToGG_M70_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4", 0.0010080401},
        {"THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCUETP8M1_v2", 0.0005049842},
        {"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa", 0.0030298354},
        {"ttHJetToGG_M75_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000057595},
        {"TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8", 0.0000085683},
        {"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 0.0858695100},
        {"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5", 0.0000000000},
        {"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000058333},
        {"ZZ_TuneCUETP8M1_13TeV-pythia8", 0.0051908910},
        {"tZq_ll_4f_13TeV-amcatnlo-pythia8", 0.0000720878},
        {"ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000039146},
        {"WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8", 0.0000000000},
        {"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 5.4476833126},
        {"ZZ_TuneCP5_13TeV-pythia8", 0.0000000000},
        {"TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8", 0.0000000000},
        {"TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8", 0.0000000000},
        {"ttHJetToGG_M95_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.1110704827},
        {"TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0018991196},
        {"TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8", 0.0000000000},
        {"GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000114581},
        {"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8", 0.0000000000},
        {"DiPhotonJetsBox2BJets_MGG-80toInf_13TeV-Sherpa", 0.0000000000},
        {"VBFHToGG_M125_13TeV_amcatnlo_pythia8_v2", 0.0002558371},
        {"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 0.0116940090},
        {"GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 2.4769092547},
        {"ttHJetToGG_M110_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000025520},
        {"TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0018500121},
        {"WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0663963222},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_DownPS", 0.0000023061},
        {"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.0000344562},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_UpPS", 0.0000016394},
        {"GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8", 0.0000000000},
        {"GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1", 0.0051708516},
        {"GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8", 0.0000000000},
        {"ttHJetToGG_M115_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M90_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"tZq_ll_4f_ckm_NLO_TuneCP5_PSweights_13TeV-amcatnlo-pythia8", 0.0000000000},
        {"ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8", 0.0000000000},
        {"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 0.0087894014},
        {"WW_TuneCP5_13TeV-pythia8", 0.0000000000},
        {"TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8", 0.0000000000},
        {"TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8", 0.0000000000},
        {"DiPhotonJetsBox_M40_80-Sherpa", 0.0825352768},
        {"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 0.0000000000},
        {"ttHToGG_M125_13TeV_powheg_pythia8", 0.0000000000},
        {"ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1", 0.0051561738},
        {"THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCUETP8M1_v2", 0.0000000000},
        {"GluGluHToGG_M-125_13TeV_powheg_pythia8", 0.0000000000},
        {"ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8", 0.0000000000},
        {"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 1.2276976061},
        {"GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"ttHJetToGG_M80_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000039859},
        {"GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8", 0.0000000000},
        {"TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0094506448},
        {"ttHJetToGG_M65_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000043015},
        {"GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0185992245},
        {"ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000029617},
        {"ttHJetToGG_M60_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8", 0.0000000000},
        {"GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.9016092885},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_PSWeights", 0.0000000000},
        {"ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M2T4", 0.0010017270},
        {"WZ_TuneCP5_13TeV-pythia8", 0.0000000000},
        {"TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8", 0.0001143528},
        {"GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0539518228},
        {"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CP5Down", 0.0000000000},
        {"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.0000031758},
        {"DiPhotonJetsBox1BJet_MGG-80toInf_13TeV-Sherpa", 0.0000000000},
        {"tZq_ll_4f_ckm_NLO_TuneCP5_13TeV-madgraph-pythia8", 0.0000000000},
        {"TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8", 0.0000727812},
        {"ttHJetToGG_M105_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
        {"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 6.9258319174},
        {"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.0000184632},
        {"WW_TuneCUETP8M1_13TeV-pythia8", 0.0079187600},
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