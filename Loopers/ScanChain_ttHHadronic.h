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
#include "MakeMVAOptimizationBabies.h"
#include "Utils/RandomMap.h"
#include "GJetReweight/ProbMap.h"

// tmva
#include "TMVA/Reader.h" 

using namespace std;
using namespace tas;

const double lumi_2016 = 35.9;
const double lumi_2017 = 41.5;
const double lumi_2018 = 45.966;
const double lumi_all = lumi_2016 + lumi_2017;


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

vector<TLorentzVector> make_jets(vector<double> &btag_scores, TString year) {
  vector<TLorentzVector> vJets;
  if (jet1_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet1_pt(), jet1_eta(), jet1_phi(), jet1_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet1_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet1_bdiscriminant());
  }
  if (jet2_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet2_pt(), jet2_eta(), jet2_phi(), jet2_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet2_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet2_bdiscriminant());
  }
  if (jet3_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet3_pt(), jet3_eta(), jet3_phi(), jet3_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet3_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet3_bdiscriminant());
  }
  if (jet4_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet4_pt(), jet4_eta(), jet4_phi(), jet4_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet4_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet4_bdiscriminant());
  }
  if (jet5_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet5_pt(), jet5_eta(), jet5_phi(), jet5_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet5_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet5_bdiscriminant());
  }
  if (jet6_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet6_pt(), jet6_eta(), jet6_phi(), jet6_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet6_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet6_bdiscriminant());
  }
  if (jet7_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet7_pt(), jet7_eta(), jet7_phi(), jet7_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet7_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet7_bdiscriminant());
  }
  if (jet8_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet8_pt(), jet8_eta(), jet8_phi(), jet8_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet8_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet8_bdiscriminant());
  }
  if (jet9_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet9_pt(), jet9_eta(), jet9_phi(), jet9_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet9_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet9_bdiscriminant());
  }
  if (jet10_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet10_pt(), jet10_eta(), jet10_phi(), jet10_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet10_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet10_bdiscriminant());
  }
  if (jet11_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet11_pt(), jet11_eta(), jet11_phi(), jet11_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet11_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet11_bdiscriminant());
  }
  if (jet12_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet12_pt(), jet12_eta(), jet12_phi(), jet12_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet12_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet12_bdiscriminant());
  }
  if (jet13_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet13_pt(), jet13_eta(), jet13_phi(), jet13_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet13_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet13_bdiscriminant());
  }
  if (jet14_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet14_pt(), jet14_eta(), jet14_phi(), jet14_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet14_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet14_bdiscriminant());
  }
  if (jet15_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet15_pt(), jet15_eta(), jet15_phi(), jet15_energy());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet15_b_DeepCSV_discriminant());
    else
      btag_scores.push_back(jet15_bdiscriminant());
  }
  return vJets;
}
