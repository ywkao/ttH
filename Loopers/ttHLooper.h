#include "TLorentzVector.h"
#include "ttH_process.h"
#include "GoldenJSON/goodrun.cc"

vector<Process*> generate_processes(TFile* f) {
  vector<Process*> v;
  v.push_back(new Process(f, "ttH"));
  v.push_back(new Process(f, "DY"));
  v.push_back(new Process(f, "DiPhoton"));
  v.push_back(new Process(f, "GammaJets"));
  v.push_back(new Process(f, "QCD"));
  v.push_back(new Process(f, "TTGG"));
  v.push_back(new Process(f, "TTGJets"));
  v.push_back(new Process(f, "VG"));
  v.push_back(new Process(f, "WJets"));
  v.push_back(new Process(f, "TTJets"));
  v.push_back(new Process(f, "Data"));
  v.push_back(new Process(f, "THQ"));
  v.push_back(new Process(f, "THW"));
  v.push_back(new Process(f, "TGamma"));
  v.push_back(new Process(f, "ggH"));
  v.push_back(new Process(f, "VBF"));
  v.push_back(new Process(f, "VH"));
  v.push_back(new Process(f, "GammaJets_Madgraph"));
  //v.push_back(new Process

  return v;
}

void add_variables(vector<Process*> v, TString tag) {
  bool hadronic = tag.Contains("Hadronic");
  bool leptonic = tag.Contains("Leptonic");
  if (!hadronic && !leptonic)
    cout << "Tag name not matched to either hadronic/leptonic" << endl;

  for (int i = 0; i < v.size(); i++) {
    v[i]->add_histogram("hMass", 50, 0, 250);
    v[i]->add_histogram("hMassAN", 80, 100, 180);
    v[i]->add_histogram("hRapidity", 25, -3, 3);
    v[i]->add_histogram("hDiphotonSumPt", 25, 0, 1000);
    v[i]->add_histogram("hDiphotonCosPhi", 25, -1, 1);
    v[i]->add_histogram("hNVtx", 25, 0, 75);
    v[i]->add_histogram("hMetPt", 25, 0, 200);
    v[i]->add_histogram("hHT", 50, 0, 5000);
    v[i]->add_histogram("hMT", 50, 0, 500);
    v[i]->add_histogram("hDiphotonMassResolution", 250, 0, 0.1);
    v[i]->add_histogram("hDiphotonMassResolutionLowMVA", 25, 0, 0.1);
    v[i]->add_histogram("hDiphotonMassResolutionMedMVA", 25, 0, 0.1);
    v[i]->add_histogram("hDiphotonMassResolutionHighMVA", 25, 0, 0.1);

    v[i]->add_histogram("hGJet_BDT", 100, 0, 1);

    v[i]->add_histogram("hNJets", 16, -0.5, 15.5);
    v[i]->add_histogram("hNbLoose", 16, -0.5, 15.5);
    v[i]->add_histogram("hNbMedium", 16, -0.5, 15.5);
    v[i]->add_histogram("hNbTight", 16, -0.5, 15.5);
    v[i]->add_histogram("hJet1pT", 25, 0, 500);
    v[i]->add_histogram("hJet1Eta", 25, -3, 3);
    v[i]->add_histogram("hJet2pT", 25, 0, 500);
    v[i]->add_histogram("hJet2Eta", 25, -3, 3);
    v[i]->add_histogram("hJet3pT", 25, 0, 500);
    v[i]->add_histogram("hJet3Eta", 25, -3, 3);
    v[i]->add_histogram("hJet4pT", 25, 0, 500);
    v[i]->add_histogram("hJet4Eta", 25, -3, 3);
    v[i]->add_histogram("hJet5pT", 25, 0, 500);
    v[i]->add_histogram("hJet5Eta", 25, -3, 3);
    v[i]->add_histogram("hJet6pT", 25, 0, 500);
    v[i]->add_histogram("hJet6Eta", 25, -3, 3);

    v[i]->add_histogram("hbJet1pT", 25, 0, 500);
    //v[i]->add_histogram("hbJet1Eta", 25, -3, 3);
    v[i]->add_histogram("hbJet2pT", 25, 0, 500);
    //v[i]->add_histogram("hbJet2Eta", 50, -3, 3);    

    v[i]->add_histogram("hPtHiggs", 25, 0, 400);
    v[i]->add_histogram("hPhotonDeltaR", 25, 0, 6); 

    v[i]->add_histogram("hMinDrDiphoJet", 25, 0, 6);
    v[i]->add_histogram("hDijetClosestWMass", 25, 0, 50);
    v[i]->add_histogram("hDijetMass", 50, 0, 500);
    v[i]->add_histogram("hDeltaRDiphoW", 25, 0, 6);
    v[i]->add_histogram("hDeltaRDiphoLep", 25, 0, 6);
    v[i]->add_histogram("hDeltaRDiphoTop", 25, 0, 6);
    v[i]->add_histogram("hTopPt", 25, 0, 400);
    v[i]->add_histogram("hTopMass", 25, 0, 400);
    v[i]->add_histogram("hTopEta", 25, -3, 3);

    v[i]->add_histogram("hLeadMinDr", 25, 0, 6);
    v[i]->add_histogram("hSubleadMinDr", 25, 0, 6);
    
    v[i]->add_histogram("hAbsCosHelicity", 10, 0, 1);

    // Leading photon
    v[i]->add_histogram("hPhotonLeadPt", 25, 0, 350);
    v[i]->add_histogram("hPhotonLeadEt", 25, 0, 350);
    v[i]->add_histogram("hPhotonLeadEta", 25, -3, 3);
    v[i]->add_histogram("hPhotonLeadPhi", 25, -3.142, 3.142);
    v[i]->add_histogram("hPhotonLeadSigmaIEtaIEta", 50, 0, 0.05);
    v[i]->add_histogram("hPhotonLeadHOverE", 25, 0, 0.1);
    v[i]->add_histogram("hPhotonLeadR9", 25, 0, 1);
    v[i]->add_histogram("hPhotonLeadIDMVA", 20, -1, 1);
    v[i]->add_histogram("hPhotonLeadPToM", 25, 0, 5);
    v[i]->add_histogram("hPhotonLeadSigmaEOverE", 100, 0, 1);
    v[i]->add_histogram("hPhotonLeadPtGen", 25, 0, 350);

    // Subleading photon
    v[i]->add_histogram("hPhotonSubleadPt", 25, 0, 350);
    v[i]->add_histogram("hPhotonSubleadEt", 25, 0, 350);
    v[i]->add_histogram("hPhotonSubleadEta", 25, -3, 3);
    v[i]->add_histogram("hPhotonSubleadPhi", 25, -3.142, 3.142);
    v[i]->add_histogram("hPhotonSubleadSigmaIEtaIEta", 50, 0, 0.05);
    v[i]->add_histogram("hPhotonSubleadHOverE", 25, 0, 0.1);
    v[i]->add_histogram("hPhotonSubleadR9", 25, 0, 1);
    v[i]->add_histogram("hPhotonSubleadIDMVA", 20, -1, 1);
    v[i]->add_histogram("hPhotonSubleadPToM", 25, 0, 5);
    v[i]->add_histogram("hPhotonSubleadSigmaEOverE", 100, 0, 1);
    v[i]->add_histogram("hPhotonSubleadPtGen", 25, 0, 350);

    v[i]->add_histogram("hPhotonPtRatio", 50, 0.0, 2.0);
    v[i]->add_histogram("hPhotonDeltaRGen", 50, 0, 0.15);

    v[i]->add_histogram("htthMVA", 50, -1, 1);
    v[i]->add_histogram("hMaxBTag", 50, 0, 1);
    v[i]->add_histogram("hSecondMaxBTag", 50, 0, 1);  

    v[i]->add_histogram("hPhotonMaxIDMVA", 10, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA", 10, -1, 1);
    v[i]->add_histogram("hPhotonMaxIDMVA_fine", 30, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_fine", 30, -1, 1);
    v[i]->add_histogram("hPhotonMaxIDMVA_entries", 10, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_entries", 10, -1, 1);
    v[i]->add_histogram("hPhotonMaxIDMVA_fine_entries", 30, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_fine_entries", 30, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_coarse", 5, -1, 1);
    v[i]->add_histogram("hPhotonMaxIDMVA_coarse", 5, -1, 1);
    v[i]->add_2D_histogram("hPhotonMaxIDMVA_NJets", 30, -1, 1, 16, -0.5, 15.5); 
    v[i]->add_2D_histogram("hPhotonMinIDMVA_NJets", 30, -1, 1, 16, -0.5, 15.5);
    v[i]->add_2D_histogram("hPhotonMaxIDMVA_NJets_entries", 30, -1, 1, 16, -0.5, 15.5);
    v[i]->add_2D_histogram("hPhotonMinIDMVA_NJets_entries", 30, -1, 1, 16, -0.5, 15.5); 

    v[i]->add_histogram("hPhotonMaxIDMVA_NJets2", 30, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_NJets2", 30, -1, 1);
    v[i]->add_histogram("hPhotonMaxIDMVA_NJets3", 30, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_NJets3", 30, -1, 1);
    v[i]->add_histogram("hPhotonMaxIDMVA_NJets4+", 30, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_NJets4+", 30, -1, 1);


    v[i]->add_histogram("hDiphoMVA", 25, -1, 1);

    v[i]->add_histogram("hPhotonIDMVA_prompt", 5, -1, 1);
    v[i]->add_histogram("hPhotonIDMVA_elec", 5, -1, 1);
    v[i]->add_histogram("hPhotonIDMVA_fake", 5, -1, 1);

    v[i]->add_histogram("hPhotonPt_prompt", 8, 0, 200);
    v[i]->add_histogram("hPhotonPt_elec", 8, 0, 200);
    v[i]->add_histogram("hPhotonPt_fake", 8, 0, 200);

    v[i]->add_histogram("hPhotonEta_prompt", 10, -3, 3);
    v[i]->add_histogram("hPhotonEta_elec", 10, -3, 3);
    v[i]->add_histogram("hPhotonEta_fake", 10, -3, 3);

    v[i]->add_histogram("hHadronicMVA", 25, -1.0, 1.0);
    v[i]->add_histogram("hLeptonicMVA", 25, -1.0, 1.0);

    v[i]->add_histogram("hLeptonPt", 25, 0, 200);
    v[i]->add_histogram("hLeptonEta", 25, -3, 3);

    v[i]->add_histogram("hPixelSeed", 2, -0.5, 1.5);
    v[i]->add_histogram("hPixelSeedEB", 2, -0.5, 1.5);
    v[i]->add_histogram("hPixelSeedEE", 2, -0.5, 1.5);


    v[i]->add_histogram("hPhotonMinIDMVA_passPSV", 5, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_failPSV", 5, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_passEVeto", 5, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_failEVeto", 5, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_passBothVeto", 5, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_failBothVeto", 5, -1, 1);

    v[i]->add_histogram("hPhotonIDMVA_passPSV", 5, -1, 1);
    v[i]->add_histogram("hPhotonIDMVA_failPSV", 5, -1, 1);
    v[i]->add_histogram("hPhotonIDMVA_passEVeto", 5, -1, 1);
    v[i]->add_histogram("hPhotonIDMVA_failEVeto", 5, -1, 1);
    v[i]->add_histogram("hPhotonIDMVA_passBothVeto", 5, -1, 1);
    v[i]->add_histogram("hPhotonIDMVA_failBothVeto", 5, -1, 1);

    v[i]->add_histogram("hPhotonPt_passPSV", 8, 0, 200);
    v[i]->add_histogram("hPhotonPt_failPSV", 8, 0, 200);
    v[i]->add_histogram("hPhotonPt_passEVeto", 8, 0, 200);
    v[i]->add_histogram("hPhotonPt_failEVeto", 8, 0, 200);
    v[i]->add_histogram("hPhotonPt_passBothVeto", 8, 0, 200);
    v[i]->add_histogram("hPhotonPt_failBothVeto", 8, 0, 200);

    v[i]->add_histogram("hPhotonEta_passPSV", 10, -3, 3);
    v[i]->add_histogram("hPhotonEta_failPSV", 10, -3, 3);
    v[i]->add_histogram("hPhotonEta_passEVeto", 10, -3, 3);
    v[i]->add_histogram("hPhotonEta_failEVeto", 10, -3, 3);
    v[i]->add_histogram("hPhotonEta_passBothVeto", 10, -3, 3);
    v[i]->add_histogram("hPhotonEta_failBothVeto", 10, -3, 3); 

    v[i]->add_histogram("hTopTagger_score", 20, -1, 1);
    v[i]->add_histogram("hTopTagger_topMass", 40, 0, 400);
    v[i]->add_histogram("hTopTagger_WMass", 20, 0, 200);

  }
}

vector<TH1D*> generate_1Dhist_vector(TString name, int length, int nBins, float xLow, float xHigh) {
  vector<TH1D*> hVec(length);
  for (unsigned int i = 0; i < hVec.size(); i++) {
    hVec[i] = new TH1D(name + "_" + to_string(i), "", nBins, xLow, xHigh);
    hVec[i]->Sumw2();
  }
  return hVec;
}

int categorize_process(TString currentFileTitle, int genPhotonId) {
  if (currentFileTitle.Contains("ttHJet"))
    return 0;
  else if (currentFileTitle.Contains("DY"))
    return 1;
  else if (currentFileTitle.Contains("DiPhoton"))
    return 2;
  else if (currentFileTitle.Contains("GJet_Pt"))
    return 3;
  //else if (currentFileTitle.Contains("GGJet") && !currentFileTitle.Contains("TTG"))
  //  return 4;
  else if (currentFileTitle.Contains("QCD"))
    return 4;

  else if (currentFileTitle.Contains("TTGG") || currentFileTitle.Contains("TTGJets") || (currentFileTitle.Contains("TTJets")) || currentFileTitle.Contains("TTTo2L2Nu") || currentFileTitle.Contains("TTToSemiLeptonic")) {
    if (genPhotonId == 2) // pp
      return 5;
    if (genPhotonId == 1) // pf
      return 6;
    if (genPhotonId == 0) // ff
      return 9;  
  }

  //else if (currentFileTitle.Contains("TTGG"))
  //  return 5; // split into hadronic/semileptonic/dileptonic
  //else if (currentFileTitle.Contains("TTGJets"))
  //  return 6; // split into hadronic/semileptonic/dileptonic 
  else if (currentFileTitle.Contains("WG") || currentFileTitle.Contains("ZG"))
    return 7;
  else if (currentFileTitle.Contains("WJets"))
    return 8;
  //else if (currentFileTitle.Contains("TTJets"))
  //  return 9; // split into hadronic/semileptonic/dileptonic
  else if (currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma"))
    return 10;
  else if (currentFileTitle.Contains("THQ"))
    return 11;
  else if (currentFileTitle.Contains("THW"))
    return 12;
  else if (currentFileTitle.Contains("TGJets"))
    return 13;
  else if (currentFileTitle.Contains("GluGluHToGG"))
    return 14;
  else if (currentFileTitle.Contains("VBF"))
    return 15;
  else if (currentFileTitle.Contains("VHToGG"))
    return 16;
  else if (currentFileTitle.Contains("GJets_HT"))
    return 17;
  else {
    cout << "File does not fit into one of the background categories." << endl;
    return -1;
  }
}

int multiclassifier_label(TString currentFileTitle, int genPhotonId) {
  if (currentFileTitle.Contains("ttHJet"))
    return 0;
  else if (currentFileTitle.Contains("TTGG") || currentFileTitle.Contains("TTGJets") || currentFileTitle.Contains("TTJets") || currentFileTitle.Contains("TTTo2L2Nu") || currentFileTitle.Contains("TTToSemiLeptonic")) {
    if (genPhotonId == 2) // pp
      return 1;
    else if (genPhotonId == 1) // pf
      return 2;
    else if (genPhotonId == 0) // ff
      return 3;
  }
  else if (currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma")) {
    return -1;
  }
  else
    return 4;
}

bool has_ttX_overlap(TString currentFileTitle, int lead_prompt, int sublead_prompt) {
  if (!(currentFileTitle.Contains("TTJets") || currentFileTitle.Contains("TTGJets"))) {
    return false;
  }
  else if (lead_prompt != 0 && sublead_prompt != 0)
    return false;
  return true;
}

bool has_simple_qcd_overlap(TString currentFileTitle, int genPhotonId) {
  if (!(currentFileTitle.Contains("QCD") || currentFileTitle.Contains("GJet_Pt") || currentFileTitle.Contains("GJets_HT"))) {
    return false;
  }
  else if (currentFileTitle.Contains("GJet_Pt") || currentFileTitle.Contains("GJets_HT")) {
    if (genPhotonId == 2) // PP
      return true;
  }
  else if (currentFileTitle.Contains("QCD")) {
    if (genPhotonId == 1 || genPhotonId == 2) // FP, PP
      return true;
  }
  return false;
}

const double gjet_normalization = 2.67547; // scale factor to make Pythia yield match MadGraph yield
const double qcd_factor = 2.6302765969200204; 
const double gjets_factor = 1.9817470714424696; 
const double diphoton_factor = 1.1645911726186993; 
const vector<double> qcd_factor_Njets = {2.2940045704500824, 1.7784105604582408, 1.7659539814519811}; 
const vector<double> gjets_factor_Njets = {1.945909958008661, 2.865798441193098, 3.6723528776911034}; 
const vector<double> diphoton_factor_Njets = {1.195047195435529, 0.9420013195895437, 0.8631697452232668}; 

double qcdX_njets_bin(int n_jets) {
  if (n_jets == 2)
    return 0;
  else if (n_jets == 3)
    return 1;
  else if (n_jets >= 4) 
    return 2;
  else
    return 0;
}

double qcdX_factor(TString currentFileTitle, TString qcd_scale, int n_jets) {
  if (qcd_scale == "inclusive_NJets") {
    if (currentFileTitle.Contains("GJet_Pt")) 
      return gjets_factor; 
    else if (currentFileTitle.Contains("GJets_HT"))
      return 1.8847607139999762;
    else if (currentFileTitle.Contains("QCD"))
      return qcd_factor; 
    else if (currentFileTitle.Contains("DiPhotonJetsBox"))
      return diphoton_factor; 
    else
      return 1.0;
  }
  else if (qcd_scale == "binned_NJets") {
    int jet_idx = qcdX_njets_bin(n_jets);
    if (currentFileTitle.Contains("GJet_Pt"))
      return gjets_factor_Njets[jet_idx];
    else if (currentFileTitle.Contains("GJets_HT"))
      return 1.8847607139999762;
    else if (currentFileTitle.Contains("QCD"))
      return qcd_factor_Njets[jet_idx];
    else if (currentFileTitle.Contains("DiPhotonJetsBox"))
      return diphoton_factor_Njets[jet_idx];  
    else
      return 1.0; 
  }
}

bool is_wrong_tt_jets_sample(TString currentFileTitle, TString channel) {
  if (!(currentFileTitle.Contains("TTTo") || currentFileTitle.Contains("TTJets")))
    return false;
  if (currentFileTitle.Contains("TTJets_Tune")) { // use these for Hadronic
    if (channel == "Hadronic")
      return false;
    else if (channel == "Leptonic")
      return true;
  }
  else if (currentFileTitle.Contains("SingleLept") || currentFileTitle.Contains("DiLept") || currentFileTitle.Contains("TTTo2L2Nu") || currentFileTitle.Contains("TTToSemiLeptonic")) { // use these for Leptonic
    if (channel == "Hadronic")
      return true;
    else if (channel == "Leptonic")
      return false;  
  }
} 

bool is_low_stats_process(TString currentFileTitle) {
  //if (currentFileTitle.Contains("QCD") || currentFileTitle.Contains("DY") || currentFileTitle.Contains("TTJets") || currentFileTitle.Contains("TTTo2L2Nu") || currentFileTitle.Contains("TTToSemiLeptonic"))
  if (currentFileTitle.Contains("QCD"))
    return true;
  return false;
}

int categorize_photons(int leadGenMatch, int subleadGenMatch) {
  if (leadGenMatch != 1 && subleadGenMatch != 1)
    return 0; // fake-fake
  else if (!(leadGenMatch == 1 && subleadGenMatch == 1) && (leadGenMatch == 1 || subleadGenMatch == 1))
    return 1; // prompt-fake
  else if (leadGenMatch == 1 && subleadGenMatch == 1)
    return 2; // prompt-prompt
  else {
    cout << "Shouldn't be here" << endl;
    return -999;
  }
}

int categorize_photons_detail(int leadPhotonType, int subleadPhotonType) {
  if (leadPhotonType == 3 && subleadPhotonType == 3)
    return 0; //fake-fake
  else if ((leadPhotonType == 2 && subleadPhotonType == 3) || (leadPhotonType == 3 && subleadPhotonType == 2))
    return 1; //fake-elec
  else if (leadPhotonType == 2 && subleadPhotonType == 2)
    return 2; //elec-elec
  else if ((leadPhotonType == 3 && subleadPhotonType == 1) || (leadPhotonType == 1 && subleadPhotonType == 3))
    return 3; //fake-prompt
  else if ((leadPhotonType == 2 && subleadPhotonType == 1) || (leadPhotonType == 1 && subleadPhotonType == 2))
    return 4; //elec-prompt
  else if (leadPhotonType == 1 && subleadPhotonType == 1)
    return 5; //prompt-prompt
}

int categorize_leptons(int nGoodEls, int nGoodMus) {
  int nLeps = nGoodEls + nGoodMus;
  if (nLeps == 0)
    return 0;
  if (nLeps == 1) {
    if (nGoodEls == 1)
      return 1;
    if (nGoodMus == 1)
      return 2;
  }
  if (nLeps == 2) {
    if (nGoodEls == 1 || nGoodMus == 1)
      return 3;
    if (nGoodEls >= 2)
      return 4;
    if (nGoodMus >= 2)
      return 5;
  }
}

const double barrel_eta = 1.479;
int categorize_photon_locations(double eta_lead, double eta_sublead) {
  if (abs(eta_lead) < barrel_eta && abs(eta_sublead) < barrel_eta)
    return 0;
  else if (abs(eta_lead) < barrel_eta || abs(eta_sublead) < barrel_eta)
    return 1;
  else if (abs(eta_lead) > barrel_eta && abs(eta_sublead) > barrel_eta)
    return 2;
  else {
    cout << "Shouldn't be here" << endl;
    return -999;
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

bool sortByValue(const std::pair<int,double>& pair1, const std::pair<int,double>& pair2 ) {
  return pair1.second > pair2.second;
}

vector<std::pair<int, double>> sortVector(const vector<double> v) {
  vector<std::pair<int, double>> v2;
  for (int i = 0; i < v.size(); i++)
    v2.push_back(std::pair<int, double>(i, v[i]));
  std::sort(v2.begin(), v2.end(), sortByValue);
  return v2;
}

const double m_top = 172.44;
TLorentzVector get_hadronic_top(const vector<TLorentzVector> jets, const vector<std::pair<int, double>> btag_scores_sorted) {
  int idx_btag_1 = btag_scores_sorted[0].first;
  int idx_btag_2 = btag_scores_sorted[1].first;
  TLorentzVector b_candidate_1 = jets[idx_btag_1];
  TLorentzVector b_candidate_2 = jets[idx_btag_2];
  TLorentzVector top_candidate;
  double min_diff = 9999;
  for (int i = 0; i < jets.size(); i++) {
    if (i == idx_btag_1 || i == idx_btag_2)
      continue;
    for (int j = i + 1; j < jets.size(); j++) {
      if (j == idx_btag_1 || j == idx_btag_2)
	continue;
      TLorentzVector top_candidate_1 = b_candidate_1 + jets[i] + jets[j];
      TLorentzVector top_candidate_2 = b_candidate_2 + jets[i] + jets[j];
      if (abs(top_candidate_1.M() - m_top) < min_diff) {
	top_candidate = top_candidate_1;
	min_diff = abs(top_candidate_1.M() - m_top);
      }
      if (abs(top_candidate_2.M() - m_top) < min_diff) {
        top_candidate = top_candidate_2;
        min_diff = abs(top_candidate_2.M() - m_top);
      }
    }
  }
  return top_candidate;
}

const double mW = 80;
double closest_mW(const vector<TLorentzVector> jets, const TLorentzVector diphoton, double &deltaR) {
  double min_diff = 999;
  int jet1_idx(-1), jet2_idx(-1);
  for (int i = 0; i < jets.size(); i++) {
    for (int j = i + 1; j < jets.size(); j++) {
      TLorentzVector dijet = jets[i] + jets[j];
      double diff = abs(dijet.M() - mW);
      if (diff < min_diff) {
	min_diff = diff;
	jet1_idx = i;
	jet2_idx = j;
      }
    }
  }
  deltaR = jets[jet1_idx].DeltaR(jets[jet2_idx]);
  return min_diff;
}

double deltaR_Higgs_W(const vector<TLorentzVector> jets, const TLorentzVector diphoton) {
  double min_diff = 999;
  int jet1_idx(-1), jet2_idx(-1);
  for (int i = 0; i < jets.size(); i++) {
    for (int j = i + 1; j < jets.size(); j++) {
      TLorentzVector dijet = jets[i] + jets[j];
      double diff = abs(dijet.M() - mW);
      if (diff < min_diff) {
        min_diff = diff;
        jet1_idx = i;
        jet2_idx = j;
      }
    }
  }
  return jets[jet1_idx].DeltaR(jets[jet2_idx]);
}

double get_ht(vector<TLorentzVector> jets) {
  double ht = 0;
  for (int i = 0; i < jets.size(); i++) {
    ht += jets[i].Pt();
  }
  return ht;
}

double min_dr(const TLorentzVector target, const vector<TLorentzVector> objects) {
  double min = 999;
  for (int i = 0; i < objects.size(); i++) {
    double dr = target.DeltaR(objects[i]);
    min = dr < min ? dr : min;
  }
  return min;
}

double helicity(const TLorentzVector particle_1, const TLorentzVector particle_2) {
  TLorentzVector p1 = particle_1;
  TLorentzVector parent = particle_1 + particle_2;
  
  TVector3 boost_to_parent = -(parent.BoostVector());
  p1.Boost(boost_to_parent);

  TVector3 v1 = p1.Vect();
  TVector3 vParent = parent.Vect();

  double cos_theta_1 = (v1.Dot(vParent)) / (v1.Mag() * vParent.Mag());

  return abs(cos_theta_1);  
}

double convert_tmva_to_prob(double score) {
  // Undo TMVA transformation
  double raw_score = -0.5 * log( 0.5 * (( 2 / (score + 1)) - 1));

  // Apply logistic (sigmoid) transformation
  double prob = 1 / (1 + exp(-raw_score));
  return prob;
}

/*
const vector<TString> vSamples_2016 = {"DoubleEG", 
			"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2", 
			"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8", 
			"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa", 
			"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 
			"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 
			"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 
			"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 
			"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8", 
			"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8", 
			"TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8",
			"TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8",		
			"WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8",
			"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8",
			"TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8"				
}; */

const vector<TString> vSamples_2016 = {
			// Data
			"DoubleEG_Run2016B-03Feb2017_ver1-v1_MINIAOD_2016_topTag_overlapRemoval",
			"DoubleEG_Run2016B-03Feb2017_ver2-v2_MINIAOD_2016_topTag_overlapRemoval",
			"DoubleEG_Run2016C-03Feb2017-v1_MINIAOD_2016_topTag_overlapRemoval",
			"DoubleEG_Run2016D-03Feb2017-v1_MINIAOD_2016_topTag_overlapRemoval",
			"DoubleEG_Run2016E-03Feb2017-v1_MINIAOD_2016_topTag_overlapRemoval",
			"DoubleEG_Run2016F-03Feb2017-v1_MINIAOD_2016_topTag_overlapRemoval",
			"DoubleEG_Run2016G-03Feb2017-v1_MINIAOD_2016_topTag_overlapRemoval",
			"DoubleEG_Run2016H-03Feb2017_ver2-v1_MINIAOD_2016_topTag_overlapRemoval",
			"DoubleEG_Run2016H-03Feb2017_ver3-v1_MINIAOD_2016_topTag_overlapRemoval",
			// ttH
			"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			// DY
			"DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			// gamma gamma + jets
			"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"DiPhotonJetsBox_M40_80-Sherpa_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			// gamma + jets
			"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			// gamma + jets madgraph sample
			"GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",

			// jets
			"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			// tt + X
			"TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"TTJets_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"TTJets_SingleLeptFromT_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"TTJets_SingleLeptFromTbar_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"TTJets_DiLept_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",	
			// V + gamma
			"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"ZGTo2LG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"WGToLNuG_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext3-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			// t + gamma
			"TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			// other signal modes
			"THQ_HToGG_13TeV-madgraph-pythia8_TuneCUETP8M1_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"THW_HToGG_13TeV-madgraph-pythia8_TuneCUETP8M1_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"VBFHToGG_M125_13TeV_amcatnlo_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1_MINIAODSIM_2016_topTag_overlapRemoval",
			"VBFHToGG_M125_13TeV_amcatnlo_pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1_MINIAODSIM_2016_topTag_overlapRemoval",
};

const vector<TString> vSamples_2017 = {
			// Data
			/*
			"DoubleEG",
			"DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
			"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa",
			"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
			"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8",
			"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
			"GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8",
			"GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8",
			"GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8",
			"GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8",
			"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
			"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8",
			"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
			"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8",
			"TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
			"TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",
			"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8"
			*/

			
			"DoubleEG_Run2017B-31Mar2018-v1_MINIAOD_forHualin_2017",
			"DoubleEG_Run2017C-31Mar2018-v1_MINIAOD_forHualin_2017",
			"DoubleEG_Run2017D-31Mar2018-v1_MINIAOD_forHualin_2017",
			"DoubleEG_Run2017E-31Mar2018-v1_MINIAOD_forHualin_2017",
			"DoubleEG_Run2017F-31Mar2018-v1_MINIAOD_forHualin_2017",
			// DY
			"DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAOD-94X_mc2017_realistic_v10_ext1-v1_MINIAODSIM_forHualin_2017",
			"DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAOD-94X_mc2017_realistic_v10-v1_MINIAODSIM_forHualin_2017",
			// gamma gamma + jets
			"DiPhotonJetsBox_M40_80-Sherpa_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2_MINIAODSIM_forHualin_2017",
			"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2_MINIAODSIM_forHualin_2017",
			// gamma + jets
			"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2_MINIAODSIM_forHualin_2017",
			// gamma + jets MadGraph samples
			"GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2_MINIAODSIM_forHualin_2017",
			"GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			// jets
			"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			// TT + X
			"TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1_MINIAODSIM_forHualin_2017",
			"TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2_MINIAODSIM_forHualin_2017",
			// V + gamma
			"WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			// t + gamma
			"TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2_MINIAODSIM_forHualin_2017",
			// ttH
			"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			// other signal modes
			"THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
			"VBFHToGG_M125_13TeV_amcatnlo_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017",
};

/*
const vector<TString> vSamples_2017 = {"DoubleEG",
			"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8",
			"DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
			"DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa",
			"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
			"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8",
			"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
			"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
			"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8",
			"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
			"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8",
			"TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
			"TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8",
			
			// No V + gamma samples for 2017 :(
};
*/

const vector<TString> vSamples_2018 = {"EGamma",
                        "ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8",
                        "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
                        "DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa",
                        "GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
                        "GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8",
                        "GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
                        "QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
                        "QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8",
                        "QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8",
                        "TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8",
                        "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
                        "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8"
                        // No V + gamma samples for 2017 :(
};


const char* json_2018 = "GoldenJSON/Cert_314472-322633_13TeV_PromptReco_Collisions18_JSON_snt.txt";
const char* json_2017 = "GoldenJSON/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_snt.txt";
const char* json_2016 = "GoldenJSON/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt";

void set_json(TString year) {
  const char* json_file;
  json_file = year == "2018" ? json_2018 : ( year == "2017" ? json_2017 : ( year == "2016" ? json_2016 : ""));
  set_goodrun_file(json_file);
}

bool pass_json(TString year, unsigned int run, unsigned int lumi_block) {
  if (year == "2018" || year == "2017" || year == "2016") { 
    return goodrun(run, lumi_block);
  }
}

/*const vector<TString> vSamples_2018 = {
			"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIISummer18MiniAOD-101X_upgrade2018_realistic_v7-v1_MINIAODSIM_test",
			"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8_RunIISummer18MiniAOD-101X_upgrade2018_realistic_v7-v1_MINIAODSIM_test",
			"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIISummer18MiniAOD-101X_upgrade2018_realistic_v7-v1_MINIAODSIM_test"
};*/

void add_samples(TChain* ch, TString year) {
  //TString tag = year == "2018" ? "v102.1" : (year == "2017" ? "v1.2" : "v3.16");
  TString tag = year == "2016" ? "v80.2" : "v94.4";

  TString location = "/home/users/sjmay/ttH/Loopers/merged_babies";

  vector<TString> vSamples = year == "2018" ? vSamples_2018 : (year == "2017" ? vSamples_2017 : vSamples_2016);

  for (int i = 0; i < vSamples.size(); i++) {
    TString tag_temp = vSamples[i].Contains("EGamma") ? "v102.1" : tag;
    TString year_temp = year == "2018" ? (vSamples[i].Contains("EGamma") ? "2018" : "2017") : year;
    ch->Add(location + "/" + vSamples[i] + "__ttH_Babies_" + tag_temp + "_" + year_temp + "/merged_ntuple.root");
  }

}
