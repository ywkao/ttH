vector<TH1D*> generate_1Dhist_vector(TString name, int length, int nBins, float xLow, float xHigh) {
  vector<TH1D*> hVec(length);
  for (unsigned int i = 0; i < hVec.size(); i++) {
    hVec[i] = new TH1D(name + "_" + to_string(i), "", nBins, xLow, xHigh);
    hVec[i]->Sumw2();
  }
  return hVec;
}

int categorize_process(TString currentFileTitle) {
  if (currentFileTitle.Contains("ttHJet"))
    return 0;
  else if (currentFileTitle.Contains("DY"))
    return 1;
  else if (currentFileTitle.Contains("DiPhoton"))
    return 2;
  else if (currentFileTitle.Contains("GJet_")) 
    return 3;
  //else if (currentFileTitle.Contains("GGJet") && !currentFileTitle.Contains("TTG"))
  //  return 4;
  else if (currentFileTitle.Contains("QCD"))
    return 4;
  else if (currentFileTitle.Contains("TTGG"))
    return 5;
  else if (currentFileTitle.Contains("TTGJets"))
    return 6;
  else if (currentFileTitle.Contains("WG") || currentFileTitle.Contains("ZG"))
    return 7;
  else {
    cout << "File does not fit into one of the background categories." << endl;
    return -1;
  }
}

double sgn(double x) {
  if (x < 0)
    return -1;
  else if (x > 0)
    return 1;
  else 
    return 0;
}

void add_samples(TChain* ch) {
  ch->Add("/hadoop/cms/store/user/smay/ttH/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2__ttH_Babies_v3/merged_ntuple_*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8__ttH_Babies_v3/merged_ntuple*.root");
  ch->Add("/hadoop/cms/store/user/smay/ttH/ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8__ttH_Babies_v3/merged_ntuple*.root");
}
