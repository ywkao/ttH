double scale1fb(TString currentFileTitle) {
  std::map<TString, double> m = {
  	{"ttHJetToGG_M90_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"ttHJetToGG_M110_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"ttHJetToGG_M70_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0000000000},
  	{"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 6.9351866211},
  	{"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 1.2276989540},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_DownPS", 0.0000000000},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_UpPS", 0.0000000000},
  	{"ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 5.4962961556},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Up", 0.0000000000},
  	{"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8", 0.0007840422},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000000000},
  	{"ttHJetToGG_M80_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa", 0.0014570762},
  	{"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.0968610292},
  	{"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 0.0862786567},
  	{"TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8", 0.0000121568},
  	{"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000000000},
  	{"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000000000},
  	{"ttHToGG_M125_13TeV_powheg_pythia8_v2", 0.0000015879},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Down", 0.0000000000},
  	{"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.0082005571},
  	{"ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 0.0000000000},
  	{"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.2079863155},
  	{"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 0.0117581899},
  	{"ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  	{"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000000000},
  };
  TObjArray *tx = currentFileTitle.Tokenize("/");
  TString key = ((TObjString *)(tx->At(tx->GetEntries()-2)))->String();
  TString to_replace = "__ttH_Babies_v1";
  TString replace_with = "";
  key = key.ReplaceAll(to_replace, replace_with);
  return m.find(key)->second;
}