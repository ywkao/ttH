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
#include "covMatrix.C"
#include "ctag_reshaping/src/ctag_reshaping.cpp"

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

bool passes_btag_rescale_selection() {
    if (n_jets() < 3)   return false;
    return true;
}

bool passes_selection(TString tag, float minIDMVA_, float maxIDMVA_, float mva_value = -1) {

  if (tag == "ttHHadronic_RunII_ttZ_CR") {
    if (abs(mass() - m_Z) > 10.)         return false;
    if (n_jets() < 3)                   return false;
    if (nb_loose() < 1)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    return true;
  }

  else if (tag == "ttHHadronic_RunII_ttZ_Tight_CR") {
    if (abs(mass() - m_Z) > 10.)         return false;
    if (n_jets() < 5)                   return false;
    if (nb_tight() < 2)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    return true;
  } 

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
 
  else if (tag == "FCNC_Hadronic_Hut_RunII_SR_Inclusive") {
    if (mass() < 100)                   return false;
    if (n_jets() < 2)                   return false;
    if (nb_loose() < 1)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    if (mva_value < 0.9457)             return false;
    return true;
  }

  else if (tag == "FCNC_Hadronic_Hct_RunII_SR_Inclusive") {
    if (mass() < 100)                   return false;
    if (n_jets() < 2)                   return false;
    if (nb_loose() < 1)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    if (mva_value < 0.9458)             return false;
    return true;
  }

  else if (tag == "ttHHadronic_RunII_MVA_Presel") {
    if (mass() < 100 || mass() > 180) 	return false;
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
    if (tthMVA_RunII() < 0.986025)        return false;
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

  else if (tag == "ttHHadronic_RunII_SR_3") {
    if (mass() < 100 || mass() > 180)   return false;
    if (n_jets() < 3)                   return false;
    if (nb_loose() < 1)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    if (tthMVA_RunII() < 0.986025 || tthMVA_RunII() > 0.9948537)        return false;
    return true;
  }

  else if (tag == "ttHHadronic_RunII_SR_2") {
    if (mass() < 100 || mass() > 180)   return false;
    if (n_jets() < 3)                   return false;
    if (nb_loose() < 1)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    if (tthMVA_RunII() < 0.9948537 || tthMVA_RunII() > 0.9983046)        return false;
    return true;
  } 

  else if (tag == "ttHHadronic_RunII_SR_1") {
    if (mass() < 100 || mass() > 180)   return false;
    if (n_jets() < 3)                   return false;
    if (nb_loose() < 1)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    if (tthMVA_RunII() < 0.9983046 || tthMVA_RunII() > 0.9990729)        return false;
    return true;
  }

  else if (tag == "ttHHadronic_RunII_SR_0") {
    if (mass() < 100 || mass() > 180)   return false;
    if (n_jets() < 3)                   return false;
    if (nb_loose() < 1)                 return false;
    if (minIDMVA_ < min_photon_ID_presel_cut)       return false;
    if (tthMVA_RunII() < 0.9990729)        return false;
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

double calculate_CvsL(double c_disciminant, double udsg_discriminant)
{
    double CvsL = c_disciminant / (c_disciminant + udsg_discriminant);
    return CvsL;
}

double calculate_CvsB(double c_disciminant, double b_discriminant, double bb_discriminant)
{
    double CvsB = c_disciminant / (c_disciminant + b_discriminant + bb_discriminant);
    return CvsB;
}

//#chi-2 related
//--------------------------------------------------//
//get_bjet_indices{{{
double pfDeepCSVJetTags_loose  = 0.1522;
vector<int> get_bjet_indices(vector<TLorentzVector> Jets, vector<double> btag_scores)
{
    vector<int> indices;
    //# consider all b-jet candidates
    //for(size_t i=0; i!=Jets.size(); ++i)
    //{
    //    double btag_score = btag_scores[i];
    //    if(btag_score >= pfDeepCSVJetTags_loose) indices.push_back(i);
    //}

    //# consider the jet with the highest b-tag score only
    int index_bjet = std::max_element(btag_scores.begin(), btag_scores.end()) - btag_scores.begin();
    indices = {index_bjet};

    return indices;
}
//}}}
//chi-2 with 3x3 covariance matrix{{{
std::vector<int> get_bjjq_indices_chi2_3x3(std::vector<TLorentzVector> Jets, int index_bjet, TLorentzVector diphoton, double &chi2_min)
{
    // 1. pick up 3 jets
    // 2. q-jj three combinations 
    int index_tqh_qjet;
    std::vector<int> index_wjets(2);
    TLorentzVector wjets[2], tqh_qjet;
    TLorentzVector w_boson, sm_top, fcnc_top; 
    TLorentzVector bjet = Jets[index_bjet];
    std::size_t num_jets = Jets.size();
    for(std::size_t i=0; i!=num_jets; ++i){
        if(i==index_bjet) continue;//bypass bjet
        for(std::size_t j=i+1; j!=num_jets; ++j){
            if(j==index_bjet) continue;//bypass bjet
            for(std::size_t k=j+1; k!=num_jets; ++k){
                if(k==index_bjet) continue;//bypass bjet
                //--- combinations{{{
                TLorentzVector jets_chosen[3];
                jets_chosen[0] = Jets[i];
                jets_chosen[1] = Jets[j];
                jets_chosen[2] = Jets[k];

                TLorentzVector tqh_q_chosen[3];
                tqh_q_chosen[0] = jets_chosen[0];
                tqh_q_chosen[1] = jets_chosen[1];
                tqh_q_chosen[2] = jets_chosen[2];

                TLorentzVector w_candidate[3];
                w_candidate[0] = jets_chosen[1] + jets_chosen[2];
                w_candidate[1] = jets_chosen[0] + jets_chosen[2];
                w_candidate[2] = jets_chosen[0] + jets_chosen[1];

                TLorentzVector top_candidate[3];
                TLorentzVector fcnc_top_candidate[3];
                //}}}
                //--- calculation{{{
                std::vector<double> chi2;
                double w_mass[3], t_mass[3], fcnc_top_mass[3];
                for(int x = 0; x<3; ++x){
                    w_mass[x] =  w_candidate[x].M();

                    top_candidate[x] = w_candidate[x] + bjet;
                    t_mass[x] =  top_candidate[x].M();

                    fcnc_top_candidate[x] = tqh_q_chosen[x] + diphoton;
                    fcnc_top_mass[x] = fcnc_top_candidate[x].M();

                    chi2.push_back( Chi2_calculator_improved(w_mass[x], t_mass[x], fcnc_top_mass[x]) );
                }
                //}}}
                //--- sorting{{{
                int smallest_chi2_index = std::min_element(chi2.begin(),chi2.end()) - chi2.begin();
                double smallest_chi2 = *std::min_element(chi2.begin(),chi2.end());

                if(smallest_chi2 < chi2_min){
                    if(smallest_chi2_index == 0){
                        tqh_qjet = tqh_q_chosen[0];
                        wjets[0] = jets_chosen[1];
                        wjets[1] = jets_chosen[2];
                        index_tqh_qjet = i;
                        index_wjets[0] = j;
                        index_wjets[1] = k;
                    } else if(smallest_chi2_index == 1){
                        tqh_qjet = tqh_q_chosen[1];
                        wjets[0] = jets_chosen[0];
                        wjets[1] = jets_chosen[2];
                        index_tqh_qjet = j;
                        index_wjets[0] = i;
                        index_wjets[1] = k;
                    } else{
                        tqh_qjet = tqh_q_chosen[2];
                        wjets[0] = jets_chosen[0];
                        wjets[1] = jets_chosen[1];
                        index_tqh_qjet = k;
                        index_wjets[0] = i;
                        index_wjets[1] = j;
                    }

                    chi2_min = smallest_chi2;
                }
                //}}}
            }
        }
    }//end of looping jets
    w_boson  = wjets[0] + wjets[1];
    sm_top   = w_boson + bjet;
    fcnc_top = tqh_qjet + diphoton;

    std::vector<int> indices = {index_bjet, index_wjets[0], index_wjets[1], index_tqh_qjet};
    return indices;
}


std::vector<int> get_bjjq_indices_min_chi2_3x3(std::vector<TLorentzVector> Jets, std::vector<int> indices_bjet, TLorentzVector diphoton)
{
    vector<double> vec_chi2;
    vector<vector<int>> vec_indices_jet;
    for(std::size_t i=0; i!=indices_bjet.size(); ++i){
        double chi2_min = 99999;
        std::vector<int> index_jet_chi2 = get_bjjq_indices_chi2_3x3(Jets, indices_bjet[i], diphoton, chi2_min); // indices of b j j q
        vec_chi2.push_back(chi2_min);
        vec_indices_jet.push_back(index_jet_chi2);
        //printf("[check-imp] [%d] (b, j, j, q) = ", i);
        //printf("(%d, ", index_jet_chi2[0]);
        //printf("%d, " , index_jet_chi2[1]);
        //printf("%d, " , index_jet_chi2[2]);
        //printf("%d), ", index_jet_chi2[3]);
        //printf("chi2 = %7.3f\n", chi2_min);
    }

    int min_index =  std::min_element(vec_chi2.begin(), vec_chi2.end()) - vec_chi2.begin();
    double min    = *std::min_element(vec_chi2.begin(), vec_chi2.end());
    //printf("[curiosity] begin, end, diff = %d, %d, %d\n", vec_chi2.begin(), vec_chi2.end(), vec_chi2.end() - vec_chi2.begin());
    //printf("[curiosity] *begin, *end     = %f, %f\n", *vec_chi2.begin(), *(vec_chi2.end()-1));
    //printf("[check-imp] min: [%d] (b, j, j, q) = ", min_index);
    //printf("(%d, ", vec_indices_jet[min_index][0]);
    //printf("%d, " , vec_indices_jet[min_index][1]);
    //printf("%d, " , vec_indices_jet[min_index][2]);
    //printf("%d), ", vec_indices_jet[min_index][3]);
    //printf("chi2 = %7.3f\n", min);
    return vec_indices_jet[min_index];
}


//}}}
// chi-2 with 2x2 covariance matrix{{{
int get_q_index_min_chi2(std::vector<TLorentzVector> Jets, std::vector<int> indices_bjj, TLorentzVector diphoton)
{
    std::vector<int> indices;
    std::vector<double> top_fcnh_chi2;
    for(std::size_t i=0; i!=Jets.size(); ++i){
        if(i==indices_bjj[0] || i==indices_bjj[1] || i==indices_bjj[2]) continue; //skip the selected jets for bjj
        TLorentzVector top_fcnh_tmp = diphoton + Jets[i];
        double chi2 = (top_fcnh_tmp.M() - top_quark_mass) * (top_fcnh_tmp.M() - top_quark_mass);
        indices.push_back(i);
        top_fcnh_chi2.push_back(chi2);
        //printf("[check-ywk] q = ");
        //printf("%d, " , i);
        //printf("chi2 = %7.3f\n", chi2);
    }

    int min_index =  std::min_element(top_fcnh_chi2.begin(), top_fcnh_chi2.end()) - top_fcnh_chi2.begin();
    double min    = *std::min_element(top_fcnh_chi2.begin(), top_fcnh_chi2.end());
    //printf("[check-ywk] min: q = ");
    //printf("%d, " , indices[min_index]);
    //printf("chi2 = %7.3f\n", top_fcnh_chi2[min_index]);

    return indices[min_index];
}


std::vector<int> get_indices_chi2(std::vector<TLorentzVector> Jets, int index_bjet, double &chi2_min, bool is_chi2_modified)
{
    std::vector<int> indices_selected_jet(3, -999);
    indices_selected_jet[0] = index_bjet;
    TLorentzVector bjet = Jets[index_bjet];

    std::size_t num_jets = Jets.size();
    for(std::size_t i=0; i<num_jets; ++i){
        if(i==index_bjet) continue;//bypass bjet
        for(std::size_t j=i+1; j<num_jets; ++j){
            if(j==index_bjet) continue;//bypass bjet
            TLorentzVector w_candidate = Jets[i] + Jets[j];
            double w_mass = w_candidate.M();
            TLorentzVector top_candidate = w_candidate + bjet;
            double t_mass = top_candidate.M();
            double chi2 = !is_chi2_modified ? Chi2_calculator_simple(w_mass, t_mass) : Chi2_calculator_modified(w_mass, t_mass);
            if(chi2 < chi2_min){
                indices_selected_jet[1] = i;
                indices_selected_jet[2] = j;
                chi2_min = chi2;
            }
        }
    }//end of looping jets

    return indices_selected_jet; //indices of b j j
}


vector<int> get_bjj_indices_min_chi2(std::vector<TLorentzVector> Jets, std::vector<int> indices_bjet, bool is_chi2_modified)
{
    vector<double> vec_chi2;
    vector<vector<int>> vec_indices_jet;
    for(std::size_t i=0; i!=indices_bjet.size(); ++i){
        double chi2_min = 99999;
        std::vector<int> index_jet_chi2 = get_indices_chi2(Jets, indices_bjet[i], chi2_min, is_chi2_modified); // indices of b j j
        vec_chi2.push_back(chi2_min);
        vec_indices_jet.push_back(index_jet_chi2);
    }

    //printf("[check-ywk] ScanChain_ttHHadronic.h::get_bjj_indices_min_chi2::indices_bjet.size() = %d\n", indices_bjet.size());
    //printf("[check-ywk] ScanChain_ttHHadronic.h::get_bjj_indices_min_chi2::vec_chi2.size()     = %d\n", vec_chi2.size());

    int min_index =  std::min_element(vec_chi2.begin(), vec_chi2.end()) - vec_chi2.begin();
    double min    = *std::min_element(vec_chi2.begin(), vec_chi2.end());
    //printf("[curiosity] begin, end, diff = %d, %d, %d\n", vec_chi2.begin(), vec_chi2.end(), vec_chi2.end() - vec_chi2.begin());
    //printf("[curiosity] *begin, *end     = %f, %f\n", *vec_chi2.begin(), *(vec_chi2.end()-1));
    //printf("[check-ywk] min: [%d] (b, j, j) = ", min_index);
    //printf("(%d, ", vec_indices_jet[min_index][0]);
    //printf("%d, " , vec_indices_jet[min_index][1]);
    //printf("%d), ", vec_indices_jet[min_index][2]);
    //printf("chi2 = %7.3f\n\n", min);
    return vec_indices_jet[min_index];
}


vector<int> get_bjjq_indices_min_chi2(std::vector<TLorentzVector> Jets, std::vector<int> indices_bjet, TLorentzVector diphoton, bool is_chi2_modified)
{
    std::vector<int> indices = get_bjj_indices_min_chi2(Jets, indices_bjet, is_chi2_modified);
    int index_q = get_q_index_min_chi2(Jets, indices, diphoton);
    indices.push_back(index_q);
    return indices;
}
//}}}
//--------------------------------------------------//

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
    double btag_score = jet1_bdiscriminant() + jet1_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet2_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet2_pt(), jet2_eta(), jet2_phi(), jet2_energy());
    vJets.push_back(jet);
    double btag_score = jet2_bdiscriminant() + jet2_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet3_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet3_pt(), jet3_eta(), jet3_phi(), jet3_energy());
    vJets.push_back(jet);
    double btag_score = jet3_bdiscriminant() + jet3_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet4_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet4_pt(), jet4_eta(), jet4_phi(), jet4_energy());
    vJets.push_back(jet);
    double btag_score = jet4_bdiscriminant() + jet4_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet5_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet5_pt(), jet5_eta(), jet5_phi(), jet5_energy());
    vJets.push_back(jet);
    double btag_score = jet5_bdiscriminant() + jet5_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet6_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet6_pt(), jet6_eta(), jet6_phi(), jet6_energy());
    vJets.push_back(jet);
    double btag_score = jet6_bdiscriminant() + jet6_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet7_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet7_pt(), jet7_eta(), jet7_phi(), jet7_energy());
    vJets.push_back(jet);
    double btag_score = jet7_bdiscriminant() + jet7_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet8_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet8_pt(), jet8_eta(), jet8_phi(), jet8_energy());
    vJets.push_back(jet);
    double btag_score = jet8_bdiscriminant() + jet8_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet9_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet9_pt(), jet9_eta(), jet9_phi(), jet9_energy());
    vJets.push_back(jet);
    double btag_score = jet9_bdiscriminant() + jet9_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet10_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet10_pt(), jet10_eta(), jet10_phi(), jet10_energy());
    vJets.push_back(jet);
    double btag_score = jet10_bdiscriminant() + jet10_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet11_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet11_pt(), jet11_eta(), jet11_phi(), jet11_energy());
    vJets.push_back(jet);
    double btag_score = jet11_bdiscriminant() + jet11_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet12_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet12_pt(), jet12_eta(), jet12_phi(), jet12_energy());
    vJets.push_back(jet);
    double btag_score = jet12_bdiscriminant() + jet12_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet13_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet13_pt(), jet13_eta(), jet13_phi(), jet13_energy());
    vJets.push_back(jet);
    double btag_score = jet13_bdiscriminant() + jet13_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet14_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet14_pt(), jet14_eta(), jet14_phi(), jet14_energy());
    vJets.push_back(jet);
    double btag_score = jet14_bdiscriminant() + jet14_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  if (jet15_pt() > 0) {
    TLorentzVector jet;
    jet.SetPtEtaPhiE(jet15_pt(), jet15_eta(), jet15_phi(), jet15_energy());
    vJets.push_back(jet);
    double btag_score = jet15_bdiscriminant() + jet15_bbdiscriminant();
    btag_scores.push_back(btag_score);
  }
  return vJets;
}

vector<vector<double>> make_jets_hadronFlavour_and_discriminants() {
  vector<vector<double>> vJets_info;

  if (jet1_pt() > 0) {
    vector<double> jet_info = {jet1_hadronFlavour(), jet1_udsgdiscriminant(), jet1_cdiscriminant(), jet1_bdiscriminant(), jet1_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet2_pt() > 0) {
    vector<double> jet_info = {jet2_hadronFlavour(), jet2_udsgdiscriminant(), jet2_cdiscriminant(), jet2_bdiscriminant(), jet2_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet3_pt() > 0) {
    vector<double> jet_info = {jet3_hadronFlavour(), jet3_udsgdiscriminant(), jet3_cdiscriminant(), jet3_bdiscriminant(), jet3_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet4_pt() > 0) {
    vector<double> jet_info = {jet4_hadronFlavour(), jet4_udsgdiscriminant(), jet4_cdiscriminant(), jet4_bdiscriminant(), jet4_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet5_pt() > 0) {
    vector<double> jet_info = {jet5_hadronFlavour(), jet5_udsgdiscriminant(), jet5_cdiscriminant(), jet5_bdiscriminant(), jet5_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet6_pt() > 0) {
    vector<double> jet_info = {jet6_hadronFlavour(), jet6_udsgdiscriminant(), jet6_cdiscriminant(), jet6_bdiscriminant(), jet6_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet7_pt() > 0) {
    vector<double> jet_info = {jet7_hadronFlavour(), jet7_udsgdiscriminant(), jet7_cdiscriminant(), jet7_bdiscriminant(), jet7_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet8_pt() > 0) {
    vector<double> jet_info = {jet8_hadronFlavour(), jet8_udsgdiscriminant(), jet8_cdiscriminant(), jet8_bdiscriminant(), jet8_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet9_pt() > 0) {
    vector<double> jet_info = {jet9_hadronFlavour(), jet9_udsgdiscriminant(), jet9_cdiscriminant(), jet9_bdiscriminant(), jet9_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet10_pt() > 0) {
    vector<double> jet_info = {jet10_hadronFlavour(), jet10_udsgdiscriminant(), jet10_cdiscriminant(), jet10_bdiscriminant(), jet10_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet11_pt() > 0) {
    vector<double> jet_info = {jet11_hadronFlavour(), jet11_udsgdiscriminant(), jet11_cdiscriminant(), jet11_bdiscriminant(), jet11_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet12_pt() > 0) {
    vector<double> jet_info = {jet12_hadronFlavour(), jet12_udsgdiscriminant(), jet12_cdiscriminant(), jet12_bdiscriminant(), jet12_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet13_pt() > 0) {
    vector<double> jet_info = {jet13_hadronFlavour(), jet13_udsgdiscriminant(), jet13_cdiscriminant(), jet13_bdiscriminant(), jet13_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet14_pt() > 0) {
    vector<double> jet_info = {jet14_hadronFlavour(), jet14_udsgdiscriminant(), jet14_cdiscriminant(), jet14_bdiscriminant(), jet14_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  if (jet15_pt() > 0) {
    vector<double> jet_info = {jet15_hadronFlavour(), jet15_udsgdiscriminant(), jet15_cdiscriminant(), jet15_bdiscriminant(), jet15_bbdiscriminant()};
    vJets_info.push_back(jet_info);
  }
  return vJets_info;
}

double get_ctag_reshaping_weight(TString mYear, retrieve_scale_factor &sf)
{
    //std::string root_file_deepJet = "/wk_cms2/ykao/CMSSW_9_4_10/src/ttH/Loopers/ctag_reshaping/sfs_rootfiles/DeepJet_ctagSF_MiniAOD94X_2017_pTincl.root";
    //retrieve_scale_factor sf(root_file_deepJet);

    double weight_JetCTagWeight = 1.;
    vector<vector<double>> jets_info = make_jets_hadronFlavour_and_discriminants();
    for(std::size_t i=0; i!=jets_info.size(); ++i)
    {
        double hadronFlavour_ = jets_info[i][0];
        double udsgtag_ = jets_info[i][1];
        double ctag_ = jets_info[i][2];
        double btag_ = jets_info[i][3];
        double bbtag_ = jets_info[i][4];
        double cvsl_ = calculate_CvsL(ctag_, udsgtag_);
        double cvsb_ = calculate_CvsB(ctag_, btag_, bbtag_);
    
        TString type_flavour = (hadronFlavour_ == 5.) ? "b" : ((hadronFlavour_ == 4.) ? "c" : "l");
        //TString name = (mYear == "2016") ? "SF" + type_flavour + "_hist" + "_central" : "SF" + type_flavour + "_hist";
        TString name = "SF" + type_flavour + "_hist";

        double scale_factor_ = sf.get_scale_factor(name, cvsl_, cvsb_);
        weight_JetCTagWeight *= scale_factor_;
        //std::cout << "hadronFlavour = " << hadronFlavour_ << ", ";
        //std::cout << "string = " << name << ", ";
        //std::cout << "scale_factor_ = " << scale_factor_ << std::endl;
    }
    return weight_JetCTagWeight;
}

void retrieve_max_discriminants(float &max_bsum, float &second_max_bsum, float &max_btag, float &second_max_btag, float &max_ctag, float &second_max_ctag, float &max_cvsl, float &second_max_cvsl, float &max_cvsb, float &second_max_cvsb)
{
    vector<double> bsum_scores; // prob_b + prob_bb
    vector<double> btag_scores;
    vector<double> ctag_scores;
    vector<double> cvsl_scores;
    vector<double> cvsb_scores;

    vector<vector<double>> jets_info = make_jets_hadronFlavour_and_discriminants();
    for(std::size_t i=0; i!=jets_info.size(); ++i)
    {
        double hadronFlavour_ = jets_info[i][0];
        double udsgtag_ = jets_info[i][1];
        double ctag_ = jets_info[i][2];
        double btag_ = jets_info[i][3];
        double bbtag_ = jets_info[i][4];
        double cvsl_ = calculate_CvsL(ctag_, udsgtag_);
        double cvsb_ = calculate_CvsB(ctag_, btag_, bbtag_);
        double bsum_ = btag_ + bbtag_;

        bsum_scores.push_back(bsum_);
        btag_scores.push_back(btag_);
        ctag_scores.push_back(ctag_);
        cvsl_scores.push_back(cvsl_);
        cvsb_scores.push_back(cvsb_);
    }

    vector<std::pair<int, double>> bsum_scores_sorted = sortVector(bsum_scores);
    vector<std::pair<int, double>> btag_scores_sorted = sortVector(btag_scores);
    vector<std::pair<int, double>> ctag_scores_sorted = sortVector(ctag_scores);
    vector<std::pair<int, double>> cvsl_scores_sorted = sortVector(cvsl_scores);
    vector<std::pair<int, double>> cvsb_scores_sorted = sortVector(cvsb_scores);

    max_bsum        = bsum_scores_sorted[0].second;
    second_max_bsum = bsum_scores_sorted[1].second;
    max_btag        = btag_scores_sorted[0].second;
    second_max_btag = btag_scores_sorted[1].second;
    max_ctag        = ctag_scores_sorted[0].second;
    second_max_ctag = ctag_scores_sorted[1].second;
    max_cvsl        = cvsl_scores_sorted[0].second;
    second_max_cvsl = cvsl_scores_sorted[1].second;
    max_cvsb        = cvsb_scores_sorted[0].second;
    second_max_cvsb = cvsb_scores_sorted[1].second;
}

TMatrixD get_cov_matrix_2x2(TString json_file)
{
    TMatrixD matrix(2,2);

    // give up jsoncpp related...
    //Json::Value root;
    //Json::Reader reader;
    //std::ifstream ifs(json_file.Data());
    //if(!reader.parse(ifs, root)) printf("fail to parse\n");
    //else{
    //    matrix(0,0) = root["covMatrix_00"].asDouble();
    //    matrix(0,1) = root["covMatrix_01"].asDouble();
    //    matrix(1,0) = root["covMatrix_10"].asDouble();
    //    matrix(1,1) = root["covMatrix_11"].asDouble();
    //}

    matrix(0,0) = 305.14;
    matrix(0,1) = 282.18;
    matrix(1,0) = 282.18;
    matrix(1,1) = 572.63;

    return matrix;
}

double chi2_calculator_2x2(double w_mass, double t_mass, TString json_file)
{
    // load value
    TVectorD vec_mean_values(2);

    //Json::Value root;
    //Json::Reader reader;
    //std::ifstream ifs(json_file.Data());

    //if(!reader.parse(ifs, root)) printf("fail to parse\n");
    //else{
    //    vec_mean_values(0) = root["mass_reco_w"].asDouble();
    //    vec_mean_values(1) = root["mass_reco_top"].asDouble();
    //}

    vec_mean_values(0) = 85.70;
    vec_mean_values(1) = 174.81;

    // evaluation
    TVectorD vec_mass(2);
    vec_mass(0) = w_mass - vec_mean_values(0);
    vec_mass(1) = t_mass - vec_mean_values(1);

    TMatrixD matrix = get_cov_matrix_2x2(json_file);

    double chi2_value = matrix.Invert()*vec_mass*vec_mass;

    return chi2_value;
}

bool get_the_best_bjj_candidate(std::vector<int> &indices_bjj, std::vector<TLorentzVector> jets, TLorentzVector diphoton, std::vector<double> btag_scores, double &min_chi2_value, TString json_file)
{
    std::size_t num_jets = jets.size();
    for(std::size_t i = 0; i < num_jets; ++i ){ // b-jet
        if (btag_scores[i] < pfDeepCSVJetTags_loose) continue;
        for(std::size_t j = 0; j < num_jets-1; ++j ){ // w-jet1
            if(j==i) continue;
            for(std::size_t k = j+1; k < num_jets; ++k ){ // w-jet2
                if(k==i) continue;

                TLorentzVector bjet = jets[i];
                TLorentzVector w_candidate = jets[j] + jets[k];
                TLorentzVector top_candidate = w_candidate + bjet;
                TLorentzVector tprime_candidate = top_candidate + diphoton;
                double w_mass = w_candidate.M();
                double t_mass = top_candidate.M();
                double tprime_mass = tprime_candidate.M();
                //double chi2 = chi2_calculator_3x3(w_mass, t_mass, tprime_mass, json_file);
                double chi2 = chi2_calculator_2x2(w_mass, t_mass, json_file);
                if(chi2 < min_chi2_value){
                    indices_bjj[0] = i;
                    indices_bjj[1] = j;
                    indices_bjj[2] = k;
                    min_chi2_value = chi2;
                }
            }
        }
    }//end of looping jets

    bool has_resonable_reco = indices_bjj[0] >= 0 && indices_bjj[1] >= 0 && indices_bjj[2] >= 0;
    return has_resonable_reco;
}
