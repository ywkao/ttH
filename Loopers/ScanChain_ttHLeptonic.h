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

// ttHLeptonic
#include "ttHLeptonic.cc"
#include "ttHLooper.h"
#include "scale1fb/scale1fb_2016.h"
#include "scale1fb/scale1fb_2017.h"
#include "MakeMVAOptimizationBabies.h"
#include "Utils/RandomMap.h"

// tmva
#include "TMVA/Reader.h"

using namespace std;
using namespace tas;

const bool no_weights = false; // turn this on to train/evaluate MVA without weights
			      // useful for determining how much of uncertainty in Z_A comes from MC samples with poor stats

const double lumi_2016 = 35.9;
const double lumi_2017 = 41.5;
const double lumi_2018 = 45.966; //FIXME: got this from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/PdmV2018Analysis 
				 // need to confirm that this is actually the json used
const double lumi_all = lumi_2016 + lumi_2017;

const vector<double> mva_thresh_2017 = { 0.3, 0.7 };

bool pass_2017_mva_presel() {
  if (mass() < 100)		  return false;
  if (n_jets() < 1)               return false;
  if (nb_medium() < 1)            return false;
  if (leadIDMVA() < -0.2)         return false;
  if (subleadIDMVA() < -0.2)      return false;
  return true;
}

double get_lep_pt(double &lep_eta) {
  if (n_ele() + n_muons() == 1) {
    lep_eta = n_ele() == 1 ? ele1_eta() : muon1_eta();
    return n_ele() == 1 ? ele1_pt() : muon1_pt();
  }
  else {
    double highest_pt = 0;
    if (ele1_pt() > highest_pt) {
      highest_pt = ele1_pt();
      lep_eta = ele1_eta();
    }
    if (ele2_pt() > highest_pt) {
      highest_pt = ele2_pt();
      lep_eta = ele2_eta();
    }
    if (muon1_pt() > highest_pt) {
      highest_pt = muon1_pt();
      lep_eta = muon1_eta();
    }
    if (muon2_pt() > highest_pt) {
      highest_pt = muon2_pt();
      lep_eta = muon2_eta();
    }
    return highest_pt;
  }
}


// have to use these ugly functions to make vectors of 4-vectors bc can only store floats in flashgg dumper
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

// returns leptons ordered by pT
vector<TLorentzVector> make_leps(vector<TLorentzVector> els, vector<TLorentzVector> mus) { 
  vector<TLorentzVector> leps_unordered, leps_ordered;
  std::vector<std::pair<int, double> > pt_ordering;
  for (int i = 0; i < els.size(); i++) {
    leps_unordered.push_back(els[i]);
    pt_ordering.push_back(std::pair<int, double>(i, els[i].Pt()));
  }
  for (int i = 0; i < mus.size(); i++) {
    leps_unordered.push_back(mus[i]);
    pt_ordering.push_back(std::pair<int, double>(els.size()+i, mus[i].Pt()));
  }

  // sort
  std::sort(pt_ordering.begin(), pt_ordering.end(), sortByValue);
  for (int i = 0; i < leps_unordered.size(); i++)
    leps_ordered.push_back(leps_unordered.at(pt_ordering[i].first));

  return leps_ordered;
}

vector<TLorentzVector> make_els() {
  vector<TLorentzVector> vEls;
  if (ele1_pt() > 0) {
    TLorentzVector el;
    el.SetPtEtaPhiE(ele1_pt(), ele1_eta(), ele1_phi(), ele1_energy());
    vEls.push_back(el);
  }
  if (ele2_pt() > 0) {
    TLorentzVector el;
    el.SetPtEtaPhiE(ele2_pt(), ele2_eta(), ele2_phi(), ele2_energy());
    vEls.push_back(el);
  }
  return vEls;
}

vector<TLorentzVector> make_mus() {
  vector<TLorentzVector> vMus;
  if (muon1_pt() > 0) {
    TLorentzVector mu;
    mu.SetPtEtaPhiE(muon1_pt(), muon1_eta(), muon1_phi(), muon1_energy());
    vMus.push_back(mu);
  }
  if (muon2_pt() > 0) {
    TLorentzVector mu;
    mu.SetPtEtaPhiE(muon2_pt(), muon2_eta(), muon2_phi(), muon2_energy());
    vMus.push_back(mu);
  }
  return vMus;
}

vector<TLorentzVector> make_jets(vector<double> &btag_scores, TString year) {
  vector<TLorentzVector> vJets;
  if (jet_pt1() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt1(), jet_eta1(), jet_phi1(), jet_energy1());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant1());
    else
      btag_scores.push_back(jet_bdiscriminant1());
  }
  if (jet_pt2() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt2(), jet_eta2(), jet_phi2(), jet_energy2());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant2());
    else
      btag_scores.push_back(jet_bdiscriminant2());
  }
  if (jet_pt3() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt3(), jet_eta3(), jet_phi3(), jet_energy3());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant3());
    else
      btag_scores.push_back(jet_bdiscriminant3());
  }
  if (jet_pt4() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt4(), jet_eta4(), jet_phi4(), jet_energy4());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant4());
    else
      btag_scores.push_back(jet_bdiscriminant4());
  }
  if (jet_pt5() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt5(), jet_eta5(), jet_phi5(), jet_energy5());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant5());
    else
      btag_scores.push_back(jet_bdiscriminant5());
  }
  if (jet_pt6() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt6(), jet_eta6(), jet_phi6(), jet_energy6());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant6());
    else
      btag_scores.push_back(jet_bdiscriminant6());
  }
  if (jet_pt7() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt7(), jet_eta7(), jet_phi7(), jet_energy7());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant7());
    else
      btag_scores.push_back(jet_bdiscriminant7());
  }
  if (jet_pt8() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt8(), jet_eta8(), jet_phi8(), jet_energy8());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant8());
    else
      btag_scores.push_back(jet_bdiscriminant8());
  }
  if (jet_pt9() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt9(), jet_eta9(), jet_phi9(), jet_energy9());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant9());
    else
      btag_scores.push_back(jet_bdiscriminant9());
  }
  if (jet_pt10() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt10(), jet_eta10(), jet_phi10(), jet_energy10());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant10());
    else
      btag_scores.push_back(jet_bdiscriminant10());
  }
  if (jet_pt11() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt11(), jet_eta11(), jet_phi11(), jet_energy11());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant11());
    else
      btag_scores.push_back(jet_bdiscriminant11());
  }
  if (jet_pt12() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt12(), jet_eta12(), jet_phi12(), jet_energy12());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant12());
    else
      btag_scores.push_back(jet_bdiscriminant12());
  }
  if (jet_pt13() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt13(), jet_eta13(), jet_phi13(), jet_energy13());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant13());
    else
      btag_scores.push_back(jet_bdiscriminant13());
  }
  if (jet_pt14() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt14(), jet_eta14(), jet_phi14(), jet_energy14());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant14());
    else
      btag_scores.push_back(jet_bdiscriminant14());
  }
  if (jet_pt15() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt15(), jet_eta15(), jet_phi15(), jet_energy15());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_b_DeepCSV_discriminant15());
    else
      btag_scores.push_back(jet_bdiscriminant15());
  }
  return vJets;
}

