// -*- C++ -*-
// Usage:
// > root -b -q doAll.C

#include <iostream>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"

// ttHHadronic
#include "ttHHadronic.cc"
#include "ttHLooper.h"
#include "scale1fb/scale1fb_2016.h"
#include "scale1fb/scale1fb_2017.h"
#include "scale1fb/scale1fb_2016_RunII.h"
#include "scale1fb/scale1fb_2017_RunII.h"
#include "scale1fb/scale1fb_2018_RunII.h"
#include "MakeMVAOptimizationBabies.h"
#include "Utils/RandomMap.h"
#include "GJetReweight/ProbMap.h"

// tmva
#include "TMVA/Reader.h" 

using namespace std;
using namespace tas;

const double lumi_2016 = 35.9;
const double lumi_2017 = 41.5;
const double lumi_2018 = 59.76;
const double lumi_all = lumi_2016 + lumi_2017 + lumi_2018;

const double min_photon_ID_presel_cut = -0.7;

const vector<double> mva_thresh_2017 = { 0.38, 0.48, 0.56 };

bool pass_2016_mva_presel() {
  if (mass() < 100)             	return false;
  if (n_jets() < 3)       		return false;
  if (diphoMVARes() < 0.4)        	return false;
  if (leadIDMVA() < -0.9)         	return false;
  if (subleadIDMVA() < -0.9)         	return false;
  if (nb_loose() < 1)     		return false;
  return true;
}

bool pass_2017_mva_presel() {
  if (mass() < 100)                     return false;
  if (leadIDMVA() < -0.2)               return false;
  if (subleadIDMVA() < -0.2)            return false;
  if (n_jets() < 2)                     return false;
  return true;
}

bool passes_selection(TString tag, float minIDMVA_, float maxIDMVA_) {

  // common to all selections
  if (!(leadPassEVeto() && subleadPassEVeto()))       return false; // always require e veto
  if (leadIDMVA() < -0.9 || subleadIDMVA() < -0.9)    return false; // don't use photon ID below -0.9

  if (tag == "ttHHadronic_RunII_MVA_Presel_veryLoose") {
    if (mass() < 100) 					return false;
    if (n_jets() < 2)					return false;
    if (nb_loose() < 1)					return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)		return false;
    return true;
  }
  
  if (tag == "ttHHadronic_RunII_MVA_Presel") {
    if (mass() < 100) 					return false;
    if (n_jets() < 3)					return false;
    if (nb_loose() < 1)					return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)		return false;
    return true;
  }

  else if (tag == "ttHHadronic_RunII_SR_Inclusive") {
    if (mass() < 100)                   return false;
    if (n_jets() < 3)                   return false;
    if (nb_loose() < 1)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    if (tthMVA_RunII() < 0.9675)        return false;
    return true;
  }    

  else if (tag == "ttHHadronic_RunII_SR_Tight") {
    if (mass() < 100)                   return false;
    if (n_jets() < 3)                   return false;
    if (nb_loose() < 1)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    if (tthMVA_RunII() < 0.9971)        return false;
    return true;
  }

  else if (tag == "ttHHadronic_RunII_MVA_Presel_CutPtoM_light") {
    if (mass() < 100)                                   return false;
    if (lead_ptoM() < 0.2 || sublead_ptoM() < 0.2)      return false;
    if (n_jets() < 3)                                   return false;
    if (nb_loose() < 1)                                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    return true;
  }

  else if (tag == "ttHHadronic_RunII_MVA_Presel_CutPtoM") {
    if (mass() < 100)                                   return false;
    if (lead_ptoM() < 0.333 || sublead_ptoM() < 0.25)	return false;
    if (n_jets() < 3)                                   return false;
    if (nb_loose() < 1)                                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    return true;
  }

  else if (tag == "ttHHadronic_RunII_MVA_Presel_lowGammaIDSideband") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 3)                                   return false;
    if (nb_loose() < 1)                                 return false;
    if (maxIDMVA_ < min_photon_ID_presel_cut)		return false;
    if (minIDMVA_ > min_photon_ID_presel_cut)		return false;
    return true; 
  }

  else if (tag == "ttHHadronic_RunII_DiPhotonFits_Presel") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 2)                                   return false;
    return true;
  }

  else if (tag == "ttHHadronic_2017_Presel") {
    if (mass() < 100 || mass() > 180)             	return false;
    if (lead_ptoM() < 0.333 || sublead_ptoM() < 0.25)   return false;
    if (n_jets() < 2)                                   return false;
    if (minIDMVA_ < -0.2)				return false;
    return true;
  }

  else if (tag == "ttHHadronic_2017_TagsInclusive") {
    if (mass() < 100 || mass() > 180)                   return false;
    if (lead_ptoM() < 0.333 || sublead_ptoM() < 0.25)   return false;
    if (n_jets() < 2)                                   return false;
    if (minIDMVA_ < -0.2)                               return false;
    if (tthMVA() < 0.38)             			return false;   
    return true;
  }

  else if (tag == "ttHHadronic_2017_Tag0") {
    if (mass() < 100 || mass() > 180)                   return false;
    if (lead_ptoM() < 0.333 || sublead_ptoM() < 0.25)   return false;
    if (n_jets() < 2)                                   return false;
    if (minIDMVA_ < -0.2)                               return false;
    if (tthMVA() < 0.56)				return false;
    return true;
  }

  else if (tag == "ttHHadronic_2017_Tag1") {
    if (mass() < 100 || mass() > 180)                   return false;
    if (lead_ptoM() < 0.333 || sublead_ptoM() < 0.25)   return false;
    if (n_jets() < 2)                                   return false;
    if (minIDMVA_ < -0.2)                               return false;
    if (tthMVA() > 0.56 || tthMVA() < 0.48)             return false;
    return true;
  }

  else if (tag == "ttHHadronic_2017_Tag2") {
    if (mass() < 100 || mass() > 180)                   return false;
    if (lead_ptoM() < 0.333 || sublead_ptoM() < 0.25)   return false;
    if (n_jets() < 2)                                   return false;
    if (minIDMVA_ < -0.2)                               return false;
    if (tthMVA() > 0.48 || tthMVA() < 0.38)             return false;
    return true;
  }

  else 
    cout << "Did not recognize tag name" << endl;
  return false;

}

void calculate_masses(TLorentzVector diphoton, vector<TLorentzVector> jets, float &m1, float &m2) {
  if (jets.size() < 4) {
    m1 = 0.;
    m2 = 0.;
    return;
  }
  float min_mass_diff = 999999;
  for (int i = 0; i < 4; i++) { 
    TLorentzVector t1 = diphoton + jets[i];
    TLorentzVector t2;
    for (int j = 0; j < 4; j++) {
      if (j == i) continue;
      t2 += jets[j];
    }
    float mass_diff = abs(t1.M() - m_top) + abs(t2.M() - m_top);
    if (mass_diff < min_mass_diff) {
      min_mass_diff = mass_diff;
      m1 = t1.M();
      m2 = t2.M();
    }
  }
  return; 
}

vector<float> make_object(TLorentzVector p4, vector<float> b_disc, const TLorentzVector diphoton, bool boost) { 
// 0: pT, 1: eta, 2: phi, 3: E, 4: b disc, 5: bb disc, 6: c disc, 7: udsg disc,
  vector<float> object;
  TLorentzVector p4_mod = p4;

  if (boost) {
    TVector3 boost_to_higgs = -(diphoton.BoostVector());
    p4_mod.Boost(boost_to_higgs);
  }  

  object.push_back(log(p4_mod.Pt()));
  object.push_back(p4_mod.Eta());
  object.push_back(p4_mod.Phi());
  object.push_back(log(p4_mod.E()));
  object.push_back(b_disc[0]);
  object.push_back(b_disc[1]);
  object.push_back(b_disc[2]);
  object.push_back(b_disc[3]);
  //object.push_back(photon_idmva);
  //object.push_back(met_bool); 

  return object;
}

vector<vector<float>> sort_objects(vector<vector<float>> unordered_objects) { // sorts physics objects by pT
  vector<vector<float>> ordered_objects;

  std::vector<std::pair<int, double> > pt_ordering;

  for (int i = 0; i < unordered_objects.size(); i++)
    pt_ordering.push_back(std::pair<int, double>(i, unordered_objects[i][0]));

  // sort
  std::sort(pt_ordering.begin(), pt_ordering.end(), sortByValue);
  for (int i = 0; i < unordered_objects.size(); i++)
    ordered_objects.push_back(unordered_objects.at(pt_ordering[i].first));

  return ordered_objects;
}

/*
vector<float> make_objects(vector<TLorentzVector> jets, vector<TLorentzVector> photons, TLorentzVector met, 
				vector<float> jet_discs, 
) { // makes pT-ordered list of photons, jets + met in the event
// 0: pT, 1: eta, 2: phi, 3: E, 4: b disc, 5: bb disc, 6: c disc, 7: udsg disc, 8: photon ID MVA, 9: MET bool


}
*/


TLorentzVector make_lead_photon() {
  TLorentzVector pho;
  pho.SetPtEtaPhiE(leadPt(), leadEta(), leadPhi(), leadEnergy());
  return pho;
}

TLorentzVector make_sublead_photon() {
  TLorentzVector pho;
  pho.SetPtEtaPhiE(subleadPt(), subleadEta(), subleadPhi(), subleadEnergy());
  return pho;
}

vector<vector<float>> make_jet_objects(TString year, TLorentzVector diphoton, bool boost) {
  vector<vector<float>> jet_objects;

  if (jet1_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet1_pt(), jet1_eta(), jet1_phi(), jet1_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet1_bdiscriminant());
    jet_discs.push_back(jet1_bbdiscriminant());
    jet_discs.push_back(jet1_cdiscriminant());
    jet_discs.push_back(jet1_udsgdiscriminant());   

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet2_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet2_pt(), jet2_eta(), jet2_phi(), jet2_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet2_bdiscriminant());
    jet_discs.push_back(jet2_bbdiscriminant());
    jet_discs.push_back(jet2_cdiscriminant());
    jet_discs.push_back(jet2_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet3_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet3_pt(), jet3_eta(), jet3_phi(), jet3_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet3_bdiscriminant());
    jet_discs.push_back(jet3_bbdiscriminant());
    jet_discs.push_back(jet3_cdiscriminant());
    jet_discs.push_back(jet3_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet4_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet4_pt(), jet4_eta(), jet4_phi(), jet4_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet4_bdiscriminant());
    jet_discs.push_back(jet4_bbdiscriminant());
    jet_discs.push_back(jet4_cdiscriminant());
    jet_discs.push_back(jet4_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet5_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet5_pt(), jet5_eta(), jet5_phi(), jet5_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet5_bdiscriminant());
    jet_discs.push_back(jet5_bbdiscriminant());
    jet_discs.push_back(jet5_cdiscriminant());
    jet_discs.push_back(jet5_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet6_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet6_pt(), jet6_eta(), jet6_phi(), jet6_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet6_bdiscriminant());
    jet_discs.push_back(jet6_bbdiscriminant());
    jet_discs.push_back(jet6_cdiscriminant());
    jet_discs.push_back(jet6_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet7_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet7_pt(), jet7_eta(), jet7_phi(), jet7_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet7_bdiscriminant());
    jet_discs.push_back(jet7_bbdiscriminant());
    jet_discs.push_back(jet7_cdiscriminant());
    jet_discs.push_back(jet7_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet8_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet8_pt(), jet8_eta(), jet8_phi(), jet8_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet8_bdiscriminant());
    jet_discs.push_back(jet8_bbdiscriminant());
    jet_discs.push_back(jet8_cdiscriminant());
    jet_discs.push_back(jet8_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet9_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet9_pt(), jet9_eta(), jet9_phi(), jet9_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet9_bdiscriminant());
    jet_discs.push_back(jet9_bbdiscriminant());
    jet_discs.push_back(jet9_cdiscriminant());
    jet_discs.push_back(jet9_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet10_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet10_pt(), jet10_eta(), jet10_phi(), jet10_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet10_bdiscriminant());
    jet_discs.push_back(jet10_bbdiscriminant());
    jet_discs.push_back(jet10_cdiscriminant());
    jet_discs.push_back(jet10_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet11_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet11_pt(), jet11_eta(), jet11_phi(), jet11_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet11_bdiscriminant());
    jet_discs.push_back(jet11_bbdiscriminant());
    jet_discs.push_back(jet11_cdiscriminant());
    jet_discs.push_back(jet11_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet12_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet12_pt(), jet12_eta(), jet12_phi(), jet12_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet12_bdiscriminant());
    jet_discs.push_back(jet12_bbdiscriminant());
    jet_discs.push_back(jet12_cdiscriminant());
    jet_discs.push_back(jet12_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet13_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet13_pt(), jet13_eta(), jet13_phi(), jet13_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet13_bdiscriminant());
    jet_discs.push_back(jet13_bbdiscriminant());
    jet_discs.push_back(jet13_cdiscriminant());
    jet_discs.push_back(jet13_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet14_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet14_pt(), jet14_eta(), jet14_phi(), jet14_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet14_bdiscriminant());
    jet_discs.push_back(jet14_bbdiscriminant());
    jet_discs.push_back(jet14_cdiscriminant());
    jet_discs.push_back(jet14_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet15_pt() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet15_pt(), jet15_eta(), jet15_phi(), jet15_energy());

    vector<float> jet_discs;
    jet_discs.push_back(jet15_bdiscriminant());
    jet_discs.push_back(jet15_bbdiscriminant());
    jet_discs.push_back(jet15_cdiscriminant());
    jet_discs.push_back(jet15_udsgdiscriminant());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  return jet_objects;
}

vector<TLorentzVector> make_jets(vector<double> &btag_scores, TString year) {
  vector<TLorentzVector> vJets;
  if (jet1_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet1_pt(), jet1_eta(), jet1_phi(), jet1_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet1_bdiscriminant());
  }
  if (jet2_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet2_pt(), jet2_eta(), jet2_phi(), jet2_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet2_bdiscriminant());
  }
  if (jet3_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet3_pt(), jet3_eta(), jet3_phi(), jet3_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet3_bdiscriminant());
  }
  if (jet4_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet4_pt(), jet4_eta(), jet4_phi(), jet4_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet4_bdiscriminant());
  }
  if (jet5_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet5_pt(), jet5_eta(), jet5_phi(), jet5_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet5_bdiscriminant());
  }
  if (jet6_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet6_pt(), jet6_eta(), jet6_phi(), jet6_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet6_bdiscriminant());
  }
  if (jet7_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet7_pt(), jet7_eta(), jet7_phi(), jet7_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet7_bdiscriminant());
  }
  if (jet8_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet8_pt(), jet8_eta(), jet8_phi(), jet8_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet8_bdiscriminant());
  }
  if (jet9_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet9_pt(), jet9_eta(), jet9_phi(), jet9_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet9_bdiscriminant());
  }
  if (jet10_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet10_pt(), jet10_eta(), jet10_phi(), jet10_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet10_bdiscriminant());
  }
  if (jet11_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet11_pt(), jet11_eta(), jet11_phi(), jet11_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet11_bdiscriminant());
  }
  if (jet12_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet12_pt(), jet12_eta(), jet12_phi(), jet12_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet12_bdiscriminant());
  }
  if (jet13_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet13_pt(), jet13_eta(), jet13_phi(), jet13_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet13_bdiscriminant());
  }
  if (jet14_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet14_pt(), jet14_eta(), jet14_phi(), jet14_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet14_bdiscriminant());
  }
  if (jet15_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet15_pt(), jet15_eta(), jet15_phi(), jet15_energy());
    vJets.push_back(jet);
    btag_scores.push_back(jet15_bdiscriminant());
  }
  return vJets;
}
