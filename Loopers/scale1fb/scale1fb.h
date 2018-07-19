double scale1fb(TString currentFileTitle) {
  std::map<TString, double> m = {
  	{"ttHJetToGG_M90_13TeV_amcatnloFXFX_madspin_pythia8", 0.0062983529},
  	{"ttHJetToGG_M70_13TeV_amcatnloFXFX_madspin_pythia8", 0.0216201652},
  	{"WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0001638318},
  	{"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 6.9351820053},
  	{"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 1.2276990222},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_DownPS", 0.0000078353},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_UpPS", 0.0000055700},
  	{"ttHJetToGG_M126_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000088312},
  	{"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 5.4962897621},
  	{"ttHJetToGG_M110_13TeV_amcatnloFXFX_madspin_pythia8", 0.0085824956},
  	{"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000331104},
  	{"TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8", 0.0024204069},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000137263},
  	{"TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0655818540},
  	{"ttHJetToGG_M80_13TeV_amcatnloFXFX_madspin_pythia8", 0.0064522921},
  	{"ttHJetToGG_M127_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000125240},
  	{"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa", 0.0014586505},
  	{"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.1511516950},
  	{"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 0.0862785919},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Up", 0.0000108632},
  	{"TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8", 0.0000223279},
  	{"WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8", 0.0000336636},
  	{"ttHJetToGG_M120_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000223752},
  	{"ttHToGG_M125_13TeV_powheg_pythia8_v2", 0.0000015549},
  	{"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_CUETP8M1Down", 0.0000116084},
  	{"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.0119876314},
  	{"ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8", 0.0077291129},
  	{"ttHJetToGG_M123_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000124664},
  	{"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8_v2", 0.0000180178},
  	{"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 0.0076964789},
  	{"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 0.0684293426},
  	{"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 0.0117677651},
  	{"ttHJetToGG_M124_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000126108},
  	{"ttHJetToGG_M130_13TeV_amcatnloFXFX_madspin_pythia8", 0.0000665420},
  };
  TObjArray *tx = currentFileTitle.Tokenize("/");
  TString key = ((TObjString *)(tx->At(tx->GetEntries()-2)))->String();
  TString tag = "v3.7";
  TString to_replace = "__ttH_Babies_" + tag;
  TString replace_with = "";
  key = key.ReplaceAll(to_replace, replace_with);
  return m.find(key)->second;
}
