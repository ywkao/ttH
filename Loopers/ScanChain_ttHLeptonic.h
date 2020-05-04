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
#include "scale1fb/scale1fb_2016_RunII.h"
#include "scale1fb/scale1fb_2017_RunII.h"
#include "scale1fb/scale1fb_2018_RunII.h"
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
const double lumi_2018 = 59.76;
const double lumi_all = lumi_2016 + lumi_2017 + lumi_2018;

const double min_photon_ID_presel_cut = -0.7;

const vector<double> mva_thresh_2017 = { 0.3, 0.7 };

bool pass_2017_mva_presel() {
  if (mass() < 100)		  return false;
  if (n_jets() < 1)               return false;
  if (nb_medium() < 1)            return false;
  if (leadIDMVA() < -0.2)         return false;
  if (subleadIDMVA() < -0.2)      return false;
  return true;
}

bool passes_btag_rescale_selection() {
    if (n_jets() < 1)   return false;
    return true;
}

bool passes_selection(TString tag, float minIDMVA_, float maxIDMVA_, int n_lep_medium, int n_lep_tight, float mva_value = -1, float m_gl_lead = 999, float m_gl_sublead = 999) {

  if (tag == "ttHLeptonic_RunII_ttZ_CR") {
    if (abs(mass() - m_Z) > 10.)         return false;
    if (n_jets() < 1)                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    return true;
  }  

  else if (tag == "ttHLeptonic_RunII_ttZ_Tight_CR") {
    if (abs(mass() - m_Z) > 10.)         return false;
    if (n_jets() < 3)                   return false;
    if (nb_medium() < 2)                return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    return true;
  } 

  // common to all selections
  if (!(leadPassEVeto() && subleadPassEVeto()))       return false; // always require e veto
  if (leadIDMVA() < -0.9 || subleadIDMVA() < -0.9)    return false; // don't use photon ID below -0.9

  if (tag == "ttHLeptonic_RunII_MVA_Presel") {
    if (mass() < 100 || mass() > 180)                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)				                return false;
    return true;
  }

  else if (tag == "ttHLeptonic_RunII_MVA_Presel_v2") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (lead_ptoM() < 0.25 || sublead_ptoM() < 0.25)    return false;
    return true;
  }

  else if (tag == "ttHLeptonic_RunII_MVA_Presel_v3") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (lead_ptoM() < 0.25 || sublead_ptoM() < 0.25)    return false;
    if (m_gl_lead < 12 || m_gl_sublead < 12)            return false;
    return true;
  }


  else if (tag == "FCNC_Leptonic_Hut_RunII_SR_Inclusive") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (mva_value < 0.72113216)                         return false;
    return true;
  }

  else if (tag == "FCNC_Leptonic_Hct_RunII_SR_Inclusive") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (mva_value < 0.5791701)                          return false;
    return true;
  }
   
  else if (tag == "ttHLeptonic_RunII_SR_Tight") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (tthMVA() < 0.9625)                        return false;
    return true;
  }

  else if (tag == "ttHLeptonic_RunII_SR_Inclusive") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (tthMVA() < 0.8997816)                        return false;
    return true;
  } 

  else if (tag == "ttHLeptonic_RunII_SR_3") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (tthMVA_RunII() < 0.8997816 || tthMVA_RunII() > 0.95635754)      return false;
    return true;
  } 

  else if (tag == "ttHLeptonic_RunII_SR_2") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (tthMVA_RunII() < 0.95635754 || tthMVA_RunII() > 0.9725133)      return false;
    return true;
  } 

  else if (tag == "ttHLeptonic_RunII_SR_1") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (tthMVA_RunII() < 0.9725133 || tthMVA_RunII() > 0.9870608)      return false;
    return true;
  } 

  else if (tag == "ttHLeptonic_RunII_SR_0") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    if (tthMVA_RunII() < 0.9870608)                     return false; 
    return true;
  } 
 

  else if (tag == "ttHLeptonic_RunII_MVA_Presel_CutPtoM_light") {
    if (mass() < 100)                                   return false;
    if (lead_ptoM() < 0.2 || sublead_ptoM() < 0.2)      return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    return true;
  }

  else if (tag == "ttHLeptonic_RunII_MVA_Presel_CutPtoM") {
    if (mass() < 100)                                   return false;
    if (lead_ptoM() < 0.33 || sublead_ptoM() < 0.25)	return false;
    if (n_jets() < 1)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    return true;
  }

  else if (tag == "ttHLeptonic_RunII_MVA_Presel_CutPtoM_NJets") {
    if (mass() < 100)                                   return false;
    if (lead_ptoM() < 0.33 || sublead_ptoM() < 0.25)    return false;
    if (n_jets() < 2)                                   return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (n_lep_medium < 1)                               return false;
    return true;
  }

  else if (tag == "ttHLeptonic_RunII_MVA_Presel_lowGammaAndLeptonIDSideband") {
    if (mass() < 100)                                   return false;
    if (n_jets() < 1)                                   return false;
    if (maxIDMVA_ < min_photon_ID_presel_cut)           return false;
    if (minIDMVA_ > min_photon_ID_presel_cut)           return false;
    if (n_lep_medium >= 1)				return false;
    return true;
  }

  else if (tag == "ttHLeptonic_RunII_ttbar_region") {
    if (mass() < 100)                                   return false;
    if (n_lep_tight < 1)				return false;
    if (nb_medium() < 1)				return false;
    if (leadPixelSeed() || subleadPixelSeed())      	return false;
    return true;
  }

  else if (tag == "ttHLeptonic_2017_TagsInclusive") {
    if (mass() < 100 || mass() > 180)                   return false;
    if (lead_ptoM() < 0.33 || sublead_ptoM() < 0.25)    return false;
    if (n_jets() < 1)                                   return false;
    if (nb_medium() < 1)                                return false;
    if (minIDMVA_ < -0.2)           			return false;
    if (n_lep_medium < 1)                               return false;
    if (tthMVA() < 0.3)                                 return false;
    return true;
  }

  else if (tag == "ttHLeptonic_2017_Tag0") {
    if (mass() < 100 || mass() > 180)                   return false;
    if (lead_ptoM() < 0.33 || sublead_ptoM() < 0.25)    return false;
    if (n_jets() < 1)                                   return false;
    if (nb_medium() < 1)				return false;
    if (minIDMVA_ < -0.2)                               return false;
    if (n_lep_medium < 1)                               return false;
    if (tthMVA() < 0.6)                                 return false;
    return true;
  }

  else if (tag == "ttHLeptonic_2017_Tag1") {
    if (mass() < 100 || mass() > 180)                   return false;
    if (lead_ptoM() < 0.33 || sublead_ptoM() < 0.25)    return false;
    if (n_jets() < 1)                                   return false;
    if (nb_medium() < 1)				return false;
    if (minIDMVA_ < -0.2)                               return false;
    if (n_lep_medium < 1)                               return false;
    if (tthMVA() > 0.6 || tthMVA() < 0.3)               return false;
    return true;
  }


  else
    cout << "Did not recognize tag name" << endl;
  return false;

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

//#quadratic equation related
//--------------------------------------------------//
const double w_boson_mass = 80.379;
const double top_quark_mass = 173.;
//get_bjet_indices{{{
double pfDeepCSVJetTags_loose  = 0.1522;
vector<int> get_bjet_indices(vector<TLorentzVector> Jets, vector<double> btag_scores)
{
    vector<int> indices;
    for(size_t i=0; i!=Jets.size(); ++i)
    {
        double btag_score = btag_scores[i];
        if(btag_score >= pfDeepCSVJetTags_loose) indices.push_back(i);
    }
    return indices;
}
//}}}
//double evaluate_neutrino_pz(TLorentzVector lepton, vector<double> met_info){{{
double evaluate_neutrino_pz(TLorentzVector lepton, vector<double> met_info)
{
    float met_pt = met_info[0];
    float met_px = met_info[1];
    float met_py = met_info[2];

    float lepton_px = lepton.Px();
    float lepton_py = lepton.Py();
    float lepton_pz = lepton.Pz();
    float lepton_energy = lepton.E();
    float coefficient_factor = ( w_boson_mass*w_boson_mass + 2*lepton_px*met_px + 2*lepton_py*met_py ) / (2.*lepton_energy);
    float coefficient_A = 1. - (lepton_pz*lepton_pz)/(lepton_energy*lepton_energy);
    float coefficient_B = 2.*coefficient_factor*lepton_pz/lepton_energy;
    float coefficient_C = met_pt*met_pt - coefficient_factor*coefficient_factor;
    float coefficient_D = coefficient_B*coefficient_B - 4.*coefficient_A*coefficient_C;
    
    float met_pz_solution_1 = 0.0;
    float met_pz_solution_2 = 0.0;

    if(coefficient_D < 0){
        met_pz_solution_1 = coefficient_B / (2.*coefficient_A);
        met_pz_solution_2 = coefficient_B / (2.*coefficient_A);
    } else{
        met_pz_solution_1 = (coefficient_B + TMath::Sqrt(coefficient_D))/(2.*coefficient_A);
        met_pz_solution_2 = (coefficient_B - TMath::Sqrt(coefficient_D))/(2.*coefficient_A);
    }
    //ordering
    float larger_pz  = (abs(met_pz_solution_1) > abs(met_pz_solution_2) ) ? met_pz_solution_1 : met_pz_solution_2;
    float smaller_pz = (abs(met_pz_solution_1) < abs(met_pz_solution_2) ) ? met_pz_solution_1 : met_pz_solution_2;
    met_pz_solution_1 = larger_pz;
    met_pz_solution_2 = smaller_pz;

    return met_pz_solution_2;
}
//}}}
//TLorentzVector derive_reco_neutrino(TLorentzVector lepton, vector<double> met_info){{{
TLorentzVector derive_reco_neutrino(TLorentzVector lepton, vector<double> met_info)
{
    double neutrino_pz = evaluate_neutrino_pz(lepton, met_info);
    double met_pt = met_info[0];
    double met_px = met_info[1];
    double met_py = met_info[2];
    double neutrino_energy = TMath::Sqrt(met_pt*met_pt + neutrino_pz*neutrino_pz);

    TLorentzVector reco_neutrino;
    reco_neutrino.SetPxPyPzE( met_px, met_py, neutrino_pz, neutrino_energy );
    return reco_neutrino;
}
//}}}
//TLorentzVector derive_reco_wboson(TLorentzVector lepton, TLorentzVector reco_neutrino){{{
TLorentzVector derive_reco_wboson(TLorentzVector lepton, TLorentzVector reco_neutrino)
{
    TLorentzVector reco_wboson = reco_neutrino + lepton;
    return reco_wboson;
}
//}}}
//TLorentzVector derive_reco_tbw(TLorentzVector reco_wboson, TLorentzVector bjet){{{
TLorentzVector derive_reco_tbw(TLorentzVector reco_wboson, TLorentzVector bjet)
{
    TLorentzVector reco_tbw = reco_wboson + bjet;
    return reco_tbw;
}
//}}}
//int get_q_index_min_chi2(std::vector<TLorentzVector> Jets, int index_bjet, TLorentzVector diphoton){{{
int get_q_index_min_chi2(std::vector<TLorentzVector> Jets, int index_bjet, TLorentzVector diphoton)
{
    std::vector<int> indices;
    std::vector<double> top_fcnh_chi2;
    for(std::size_t i=0; i!=Jets.size(); ++i){
        if(i==index_bjet) continue; //skip the selected jets for bjet
        TLorentzVector top_fcnh_tmp = diphoton + Jets[i];
        double chi2 = (top_fcnh_tmp.M() - top_quark_mass) * (top_fcnh_tmp.M() - top_quark_mass);
        indices.push_back(i);
        top_fcnh_chi2.push_back(chi2);
        //printf("[check-ywk] q = ");
        //printf("%d, " , i);
        //printf("chi2 = %7.3f\n", chi2);
    }

    int min_index =  std::min_element(top_fcnh_chi2.begin(), top_fcnh_chi2.end()) - top_fcnh_chi2.begin();
    //double min    = *std::min_element(top_fcnh_chi2.begin(), top_fcnh_chi2.end());
    //printf("[check-ywk] min: q = ");
    //printf("%d, " , indices[min_index]);
    //printf("chi2 = %7.3f\n", top_fcnh_chi2[min_index]);

    int result = Jets.size() > 1 ? indices[min_index] : -1;
    return result;
}
//}}}
//--------------------------------------------------//

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

vector<float> calculate_lepton_charges() {
  vector<float> vCharges;

  if (ele1_pt() > muon1_pt())
    vCharges.push_back(ele1_charge());
  else 
    vCharges.push_back(muon1_charge());

  if ((ele1_pt() > 0 && ele2_pt() > 0) || (muon1_pt() > 0 && muon2_pt() > 0) || (ele1_pt() > 0 && muon1_pt() > 0)) { // have at least 2 leps
    if (ele2_pt() > muon1_pt())
      vCharges.push_back(ele2_charge());
    else if (muon2_pt() > ele1_pt())
      vCharges.push_back(muon2_charge());
    else
      vCharges.push_back(ele1_pt() > muon1_pt() ? muon1_charge() : ele1_charge());
  }
  else
    vCharges.push_back(0);

  return vCharges;

}

vector<float> calculate_forward_jets(vector<vector<float>> jets, float max1_btag) {
    float forward_jet_pt = -1;
    float forward_jet_eta = 0.0;

    for (unsigned int i = 0; i < jets.size(); i++) {
      if (jets[i][4] >= max1_btag && jets.size() > 1) // forward jet can't be the b jet (but this only makes sense if theres more than 1 jet)
        continue;
      if (abs(jets[i][1]) > forward_jet_eta) {
        forward_jet_pt = jets[i][0];
        forward_jet_eta = abs(jets[i][1]);
      }
    }
    vector<float> forward_jet = {forward_jet_pt, forward_jet_eta};
    return forward_jet;
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

vector<float> make_lepton(TLorentzVector p4, float id, const TLorentzVector diphoton, bool boost) {
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
  object.push_back(id);

  return object;
}

vector<vector<float>> make_all_objects(vector<vector<float>> lep_objects, vector<vector<float>> jet_objects) {
// 0: pT, 1: eta, 2: phi, 3: E, 4: b disc, 5: bb disc, 6: c disc, 7: udsg disc, 8: lep flag (-2 = jet, 0 = mu, 1 = el
  vector<vector<float>> all_objects;
  for (int i = 0; i < lep_objects.size(); i++) {
    vector<float> obj = {lep_objects[i][0], lep_objects[i][1], lep_objects[i][2], lep_objects[i][3], -2, -2, -2, -2, lep_objects[i][4]};
    all_objects.push_back(obj);
  }
  for (int i = 0; i < jet_objects.size(); i++) {
    vector<float> obj = jet_objects[i];
    obj.push_back(-2);
    all_objects.push_back(obj);
  }
  return all_objects;
}

vector<vector<float>> make_lep_objects(vector<TLorentzVector> els, vector<TLorentzVector> mus, TLorentzVector diphoton, bool boost) {
  vector<vector<float>> lep_objects;
  for (int i = 0; i < mus.size(); i++) 
    lep_objects.push_back(make_lepton(mus[i], 0, diphoton, boost));
  for (int i = 0; i < els.size(); i++) 
    lep_objects.push_back(make_lepton(els[i], 1, diphoton, boost));
  return lep_objects;
}

vector<vector<float>> make_jet_objects(TString year, TLorentzVector diphoton, bool boost) {
  vector<vector<float>> jet_objects;

  if (jet_pt1() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt1(), jet_eta1(), jet_phi1(), jet_energy1());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant1());
    else
      jet_discs.push_back(jet_bdiscriminant1());
    jet_discs.push_back(jet_bbdiscriminant1());
    jet_discs.push_back(jet_cdiscriminant1());
    jet_discs.push_back(jet_udsgdiscriminant1());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt2() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt2(), jet_eta2(), jet_phi2(), jet_energy2());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant2());
    else
      jet_discs.push_back(jet_bdiscriminant2());
    jet_discs.push_back(jet_bbdiscriminant2());
    jet_discs.push_back(jet_cdiscriminant2());
    jet_discs.push_back(jet_udsgdiscriminant2());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt3() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt3(), jet_eta3(), jet_phi3(), jet_energy3());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant3());
    else
      jet_discs.push_back(jet_bdiscriminant3());
    jet_discs.push_back(jet_bbdiscriminant3());
    jet_discs.push_back(jet_cdiscriminant3());
    jet_discs.push_back(jet_udsgdiscriminant3());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt4() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt4(), jet_eta4(), jet_phi4(), jet_energy4());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant4());
    else
      jet_discs.push_back(jet_bdiscriminant4());
    jet_discs.push_back(jet_bbdiscriminant4());
    jet_discs.push_back(jet_cdiscriminant4());
    jet_discs.push_back(jet_udsgdiscriminant4());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt5() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt5(), jet_eta5(), jet_phi5(), jet_energy5());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant5());
    else
      jet_discs.push_back(jet_bdiscriminant5());
    jet_discs.push_back(jet_bbdiscriminant5());
    jet_discs.push_back(jet_cdiscriminant5());
    jet_discs.push_back(jet_udsgdiscriminant5());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt6() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt6(), jet_eta6(), jet_phi6(), jet_energy6());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant6());
    else
      jet_discs.push_back(jet_bdiscriminant6());
    jet_discs.push_back(jet_bbdiscriminant6());
    jet_discs.push_back(jet_cdiscriminant6());
    jet_discs.push_back(jet_udsgdiscriminant6());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt7() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt7(), jet_eta7(), jet_phi7(), jet_energy7());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant7());
    else
      jet_discs.push_back(jet_bdiscriminant7());
    jet_discs.push_back(jet_bbdiscriminant7());
    jet_discs.push_back(jet_cdiscriminant7());
    jet_discs.push_back(jet_udsgdiscriminant7());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt8() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt8(), jet_eta8(), jet_phi8(), jet_energy8());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant8());
    else
      jet_discs.push_back(jet_bdiscriminant8());
    jet_discs.push_back(jet_bbdiscriminant8());
    jet_discs.push_back(jet_cdiscriminant8());
    jet_discs.push_back(jet_udsgdiscriminant8());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt9() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt9(), jet_eta9(), jet_phi9(), jet_energy9());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant9());
    else
      jet_discs.push_back(jet_bdiscriminant9());
    jet_discs.push_back(jet_bbdiscriminant9());
    jet_discs.push_back(jet_cdiscriminant9());
    jet_discs.push_back(jet_udsgdiscriminant9());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt10() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt10(), jet_eta10(), jet_phi10(), jet_energy10());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant10());
    else
      jet_discs.push_back(jet_bdiscriminant10());
    jet_discs.push_back(jet_bbdiscriminant10());
    jet_discs.push_back(jet_cdiscriminant10());
    jet_discs.push_back(jet_udsgdiscriminant10());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt11() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt11(), jet_eta11(), jet_phi11(), jet_energy11());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant11());
    else
      jet_discs.push_back(jet_bdiscriminant11());
    jet_discs.push_back(jet_bbdiscriminant11());
    jet_discs.push_back(jet_cdiscriminant11());
    jet_discs.push_back(jet_udsgdiscriminant11());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt12() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt12(), jet_eta12(), jet_phi12(), jet_energy12());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant12());
    else
      jet_discs.push_back(jet_bdiscriminant12());
    jet_discs.push_back(jet_bbdiscriminant12());
    jet_discs.push_back(jet_cdiscriminant12());
    jet_discs.push_back(jet_udsgdiscriminant12());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt13() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt13(), jet_eta13(), jet_phi13(), jet_energy13());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant13());
    else
      jet_discs.push_back(jet_bdiscriminant13());
    jet_discs.push_back(jet_bbdiscriminant13());
    jet_discs.push_back(jet_cdiscriminant13());
    jet_discs.push_back(jet_udsgdiscriminant13());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt14() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt14(), jet_eta14(), jet_phi14(), jet_energy14());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant14());
    else
      jet_discs.push_back(jet_bdiscriminant14());
    jet_discs.push_back(jet_bbdiscriminant14());
    jet_discs.push_back(jet_cdiscriminant14());
    jet_discs.push_back(jet_udsgdiscriminant14());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }
  if (jet_pt15() > 0) {
    TLorentzVector jet_p4;
    jet_p4.SetPtEtaPhiE(jet_pt15(), jet_eta15(), jet_phi15(), jet_energy15());

    vector<float> jet_discs;
    if (year == "2016")
      jet_discs.push_back(jet_bdiscriminant15());
    else
      jet_discs.push_back(jet_bdiscriminant15());
    jet_discs.push_back(jet_bbdiscriminant15());
    jet_discs.push_back(jet_cdiscriminant15());
    jet_discs.push_back(jet_udsgdiscriminant15());

    jet_objects.push_back(make_object(jet_p4, jet_discs,  diphoton, boost));
  }

  return jet_objects;
}

vector<TLorentzVector> make_jets(vector<double> &btag_scores, TString year) {
  vector<TLorentzVector> vJets;
  if (jet_pt1() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt1(), jet_eta1(), jet_phi1(), jet_energy1());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant1());
    else
      btag_scores.push_back(jet_bdiscriminant1());
  }
  if (jet_pt2() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt2(), jet_eta2(), jet_phi2(), jet_energy2());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant2());
    else
      btag_scores.push_back(jet_bdiscriminant2());
  }
  if (jet_pt3() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt3(), jet_eta3(), jet_phi3(), jet_energy3());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant3());
    else
      btag_scores.push_back(jet_bdiscriminant3());
  }
  if (jet_pt4() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt4(), jet_eta4(), jet_phi4(), jet_energy4());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant4());
    else
      btag_scores.push_back(jet_bdiscriminant4());
  }
  if (jet_pt5() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt5(), jet_eta5(), jet_phi5(), jet_energy5());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant5());
    else
      btag_scores.push_back(jet_bdiscriminant5());
  }
  if (jet_pt6() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt6(), jet_eta6(), jet_phi6(), jet_energy6());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant6());
    else
      btag_scores.push_back(jet_bdiscriminant6());
  }
  if (jet_pt7() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt7(), jet_eta7(), jet_phi7(), jet_energy7());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant7());
    else
      btag_scores.push_back(jet_bdiscriminant7());
  }
  if (jet_pt8() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt8(), jet_eta8(), jet_phi8(), jet_energy8());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant8());
    else
      btag_scores.push_back(jet_bdiscriminant8());
  }
  if (jet_pt9() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt9(), jet_eta9(), jet_phi9(), jet_energy9());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant9());
    else
      btag_scores.push_back(jet_bdiscriminant9());
  }
  if (jet_pt10() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt10(), jet_eta10(), jet_phi10(), jet_energy10());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant10());
    else
      btag_scores.push_back(jet_bdiscriminant10());
  }
  if (jet_pt11() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt11(), jet_eta11(), jet_phi11(), jet_energy11());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant11());
    else
      btag_scores.push_back(jet_bdiscriminant11());
  }
  if (jet_pt12() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt12(), jet_eta12(), jet_phi12(), jet_energy12());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant12());
    else
      btag_scores.push_back(jet_bdiscriminant12());
  }
  if (jet_pt13() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt13(), jet_eta13(), jet_phi13(), jet_energy13());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant13());
    else
      btag_scores.push_back(jet_bdiscriminant13());
  }
  if (jet_pt14() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt14(), jet_eta14(), jet_phi14(), jet_energy14());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant14());
    else
      btag_scores.push_back(jet_bdiscriminant14());
  }
  if (jet_pt15() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet_pt15(), jet_eta15(), jet_phi15(), jet_energy15());
    vJets.push_back(jet);
    if (year == "2016")
      btag_scores.push_back(jet_bdiscriminant15());
    else
      btag_scores.push_back(jet_bdiscriminant15());
  }
  return vJets;
}

