#include "TLorentzVector.h"
#include "ttH_process.h"

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
    
    v[i]->add_histogram("hAbsCosHelicity", 25, 0, 1);

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
    v[i]->add_histogram("hPhotonLeadSigmaEOverE", 25, 0, 1);
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
    v[i]->add_histogram("hPhotonSubleadSigmaEOverE", 25, 0, 1);
    v[i]->add_histogram("hPhotonSubleadPtGen", 25, 0, 350);

    v[i]->add_histogram("hPhotonPtRatio", 50, 0.0, 2.0);
    v[i]->add_histogram("hPhotonDeltaRGen", 50, 0, 0.15);

    v[i]->add_histogram("htthMVA", 50, -1, 1);
    v[i]->add_histogram("hMaxBTag", 50, 0, 1);
    v[i]->add_histogram("hSecondMaxBTag", 50, 0, 1);  

    v[i]->add_histogram("hPhotonMaxIDMVA", 10, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA", 10, -1, 1);
    v[i]->add_histogram("hPhotonMaxIDMVA_entries", 10, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_entries", 10, -1, 1);
    v[i]->add_histogram("hPhotonMinIDMVA_coarse", 5, -1, 1);
    v[i]->add_histogram("hPhotonMaxIDMVA_coarse", 5, -1, 1);
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
    return 5; // split into hadronic/semileptonic/dileptonic
  else if (currentFileTitle.Contains("TTGJets"))
    return 6; // split into hadronic/semileptonic/dileptonic 
  else if (currentFileTitle.Contains("WG") || currentFileTitle.Contains("ZG"))
    return 7;
  else if (currentFileTitle.Contains("WJets"))
    return 8;
  else if (currentFileTitle.Contains("TTJets"))
    return 9; // split into hadronic/semileptonic/dileptonic
  else if (currentFileTitle.Contains("DoubleEG"))
    return 10;
  else {
    cout << "File does not fit into one of the background categories." << endl;
    return -1;
  }
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
};
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
			"TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8"
			// No V + gamma samples for 2017 :(
};


void add_samples(TChain* ch, TString year) {
  TString tag = year == "2017" ? "v1.1" : "v3.12";

  TString location = "/home/users/sjmay/ttH/Loopers/merged_babies";

  vector<TString> vSamples = year == "2017" ? vSamples_2017 : vSamples_2016;

  for (int i = 0; i < vSamples.size(); i++)
    ch->Add(location + "/" + vSamples[i] + "__ttH_Babies_" + tag + "_" + year + "/merged_ntuple.root");

}
