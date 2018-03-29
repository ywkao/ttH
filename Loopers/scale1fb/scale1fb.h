double scale1fb(TString currentFileTitle) {
  std::map<TString, double> m = {
  	{"ttHJetToGG_M90_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"ttHJetToGG_M110_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"ttHJetToGG_M70_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0684951371},
  	{"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 6.9031330138},
  	{"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 1.3493027853},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_DownPS", 0.0000000000},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_UpPS", 0.0000016780},
  	{"ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000029804},
  	{"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 5.2462001400},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Up", 0.0000000000},
  	{"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000069021},
  	{"TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8", 0.0007589956},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000030312},
  	{"ttHJetToGG_M80_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000043716},
  	{"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa", 0.0014501706},
  	{"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.0968610292},
  	{"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 0.0893229572},
  	{"TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8", 0.0000119391},
  	{"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000059033},
  	{"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000069021},
  	{"ttHToGG_M125_13TeV_powheg_pythia8_v2", 0.0000014981},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Down", 0.0000000000},
  	{"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.0082005571},
  	{"ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000045466},
  	{"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 0.0088806737},
  	{"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.1990247806},
  	{"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 0.0119282995},
  	{"ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000040413},
  	{"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000059033},
  };
  TObjArray *tx = currentFileTitle.Tokenize("/");
  TString key = ((TObjString *)(tx->At(tx->GetEntries()-2)))->String();
  cout << key << endl;
  TString to_replace = "__ttH_Babies_v1";
  TString replace_with = "";
  key = key.ReplaceAll(to_replace, replace_with);
  cout << key << endl;
  return m.find(key)->second;
}
