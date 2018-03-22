// -*- C++ -*-
// This is a header file generated with the command:
// makeCMS3ClassFiles("/hadoop/cms/store/user/smay/ttH/ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8__ttH_Babies_v1/merged_ntuple_1.root", "tthHadronicTagDumper/trees/tth_13TeV_all", "ttHHadronic", "tas", "cms3")

#ifndef ttHHadronic_H
#define ttHHadronic_H

#include "Math/LorentzVector.h"
#include "Math/Point3D.h"
#include "TMath.h"
#include "TBranch.h"
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "TBits.h"
#include <vector> 
#include <unistd.h> 
typedef ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > LorentzVector;

using namespace std; 
class ttHHadronic {
 private: 
 protected: 
  unsigned int index;
  int      candidate_id_;
  TBranch *candidate_id_branch;
  bool     candidate_id_isLoaded;
  float    weight_;
  TBranch *weight_branch;
  bool     weight_isLoaded;
  float    dipho_sumpt_;
  TBranch *dipho_sumpt_branch;
  bool     dipho_sumpt_isLoaded;
  float    dipho_cosphi_;
  TBranch *dipho_cosphi_branch;
  bool     dipho_cosphi_isLoaded;
  float    mass_;
  TBranch *mass_branch;
  bool     mass_isLoaded;
  float    leadPt_;
  TBranch *leadPt_branch;
  bool     leadPt_isLoaded;
  float    leadEt_;
  TBranch *leadEt_branch;
  bool     leadEt_isLoaded;
  float    leadEta_;
  TBranch *leadEta_branch;
  bool     leadEta_isLoaded;
  float    leadPhi_;
  TBranch *leadPhi_branch;
  bool     leadPhi_isLoaded;
  float    lead_sieie_;
  TBranch *lead_sieie_branch;
  bool     lead_sieie_isLoaded;
  float    lead_hoe_;
  TBranch *lead_hoe_branch;
  bool     lead_hoe_isLoaded;
  float    lead_sigmaEoE_;
  TBranch *lead_sigmaEoE_branch;
  bool     lead_sigmaEoE_isLoaded;
  float    lead_ptoM_;
  TBranch *lead_ptoM_branch;
  bool     lead_ptoM_isLoaded;
  float    leadR9_;
  TBranch *leadR9_branch;
  bool     leadR9_isLoaded;
  float    subleadPt_;
  TBranch *subleadPt_branch;
  bool     subleadPt_isLoaded;
  float    subleadEt_;
  TBranch *subleadEt_branch;
  bool     subleadEt_isLoaded;
  float    subleadEta_;
  TBranch *subleadEta_branch;
  bool     subleadEta_isLoaded;
  float    subleadPhi_;
  TBranch *subleadPhi_branch;
  bool     subleadPhi_isLoaded;
  float    sublead_sieie_;
  TBranch *sublead_sieie_branch;
  bool     sublead_sieie_isLoaded;
  float    sublead_hoe_;
  TBranch *sublead_hoe_branch;
  bool     sublead_hoe_isLoaded;
  float    sublead_sigmaEoE_;
  TBranch *sublead_sigmaEoE_branch;
  bool     sublead_sigmaEoE_isLoaded;
  float    sublead_ptoM_;
  TBranch *sublead_ptoM_branch;
  bool     sublead_ptoM_isLoaded;
  float    subleadR9_;
  TBranch *subleadR9_branch;
  bool     subleadR9_isLoaded;
  float    leadIDMVA_;
  TBranch *leadIDMVA_branch;
  bool     leadIDMVA_isLoaded;
  float    subleadIDMVA_;
  TBranch *subleadIDMVA_branch;
  bool     subleadIDMVA_isLoaded;
  float    dipho_rapidity_;
  TBranch *dipho_rapidity_branch;
  bool     dipho_rapidity_isLoaded;
  float    n_bjets_;
  TBranch *n_bjets_branch;
  bool     n_bjets_isLoaded;
  float    n_jets_;
  TBranch *n_jets_branch;
  bool     n_jets_isLoaded;
  float    bjet1_pt_;
  TBranch *bjet1_pt_branch;
  bool     bjet1_pt_isLoaded;
  float    bjet2_pt_;
  TBranch *bjet2_pt_branch;
  bool     bjet2_pt_isLoaded;
  float    jet1_pt_;
  TBranch *jet1_pt_branch;
  bool     jet1_pt_isLoaded;
  float    jet2_pt_;
  TBranch *jet2_pt_branch;
  bool     jet2_pt_isLoaded;
  float    jet3_pt_;
  TBranch *jet3_pt_branch;
  bool     jet3_pt_isLoaded;
  float    jet4_pt_;
  TBranch *jet4_pt_branch;
  bool     jet4_pt_isLoaded;
  float    bjet1_csv_;
  TBranch *bjet1_csv_branch;
  bool     bjet1_csv_isLoaded;
  float    bjet2_csv_;
  TBranch *bjet2_csv_branch;
  bool     bjet2_csv_isLoaded;
  float    tthMVA_;
  TBranch *tthMVA_branch;
  bool     tthMVA_isLoaded;
  float    rho_;
  TBranch *rho_branch;
  bool     rho_isLoaded;
  int      nvtx_;
  TBranch *nvtx_branch;
  bool     nvtx_isLoaded;
  unsigned long long event_;
  TBranch *event_branch;
  bool     event_isLoaded;
  unsigned int lumi_;
  TBranch *lumi_branch;
  bool     lumi_isLoaded;
  int      processIndex_;
  TBranch *processIndex_branch;
  bool     processIndex_isLoaded;
  unsigned int run_;
  TBranch *run_branch;
  bool     run_isLoaded;
  float    npu_;
  TBranch *npu_branch;
  bool     npu_isLoaded;
  float    puweight_;
  TBranch *puweight_branch;
  bool     puweight_isLoaded;
public: 
void Init(TTree *tree);
void GetEntry(unsigned int idx); 
void LoadAllBranches(); 
  const int &candidate_id();
  const float &weight();
  const float &dipho_sumpt();
  const float &dipho_cosphi();
  const float &mass();
  const float &leadPt();
  const float &leadEt();
  const float &leadEta();
  const float &leadPhi();
  const float &lead_sieie();
  const float &lead_hoe();
  const float &lead_sigmaEoE();
  const float &lead_ptoM();
  const float &leadR9();
  const float &subleadPt();
  const float &subleadEt();
  const float &subleadEta();
  const float &subleadPhi();
  const float &sublead_sieie();
  const float &sublead_hoe();
  const float &sublead_sigmaEoE();
  const float &sublead_ptoM();
  const float &subleadR9();
  const float &leadIDMVA();
  const float &subleadIDMVA();
  const float &dipho_rapidity();
  const float &n_bjets();
  const float &n_jets();
  const float &bjet1_pt();
  const float &bjet2_pt();
  const float &jet1_pt();
  const float &jet2_pt();
  const float &jet3_pt();
  const float &jet4_pt();
  const float &bjet1_csv();
  const float &bjet2_csv();
  const float &tthMVA();
  const float &rho();
  const int &nvtx();
  const unsigned long long &event();
  const unsigned int &lumi();
  const int &processIndex();
  const unsigned int &run();
  const float &npu();
  const float &puweight();

  static void progress(int nEventsTotal, int nEventsChain);
};

#ifndef __CINT__
extern ttHHadronic cms3;
#endif

namespace tas {
  const int &candidate_id();
  const float &weight();
  const float &dipho_sumpt();
  const float &dipho_cosphi();
  const float &mass();
  const float &leadPt();
  const float &leadEt();
  const float &leadEta();
  const float &leadPhi();
  const float &lead_sieie();
  const float &lead_hoe();
  const float &lead_sigmaEoE();
  const float &lead_ptoM();
  const float &leadR9();
  const float &subleadPt();
  const float &subleadEt();
  const float &subleadEta();
  const float &subleadPhi();
  const float &sublead_sieie();
  const float &sublead_hoe();
  const float &sublead_sigmaEoE();
  const float &sublead_ptoM();
  const float &subleadR9();
  const float &leadIDMVA();
  const float &subleadIDMVA();
  const float &dipho_rapidity();
  const float &n_bjets();
  const float &n_jets();
  const float &bjet1_pt();
  const float &bjet2_pt();
  const float &jet1_pt();
  const float &jet2_pt();
  const float &jet3_pt();
  const float &jet4_pt();
  const float &bjet1_csv();
  const float &bjet2_csv();
  const float &tthMVA();
  const float &rho();
  const int &nvtx();
  const unsigned long long &event();
  const unsigned int &lumi();
  const int &processIndex();
  const unsigned int &run();
  const float &npu();
  const float &puweight();
}
#endif
