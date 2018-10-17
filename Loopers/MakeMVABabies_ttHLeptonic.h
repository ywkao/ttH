#ifndef MakeMVABabies_ttHLeptonic_h 
#define MakeMVABabies_ttHLeptonic_h

#include <string>
#include <vector>

#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH2.h"
#include "TString.h"
#include "Math/LorentzVector.h"
#include "Math/GenVector/LorentzVector.h"

using namespace std;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

class BabyMaker {
  public:
    BabyMaker() {};
    ~BabyMaker() {
      if (BabyFile_) delete BabyFile_;
      if (BabyTree_) delete BabyTree_;
    }
    void ScanChain(TChain* chain, TString tag, TString ext, bool blind, bool fast, int nEvents, string skimFilePrefix);
    void MakeBabyNtuple(const char *);
    void InitBabyNtuple();
    void FillBabyNtuple();
    void CloseBabyNtuple();
    

  private:
    TFile *BabyFile_;
    TTree *BabyTree_;

    // Variable names
    vector<string> mva_branches = {"dipho_rapidity_", "jet3_eta_", "jet2_eta_", "jet1_eta_", "jet3_pt_", "jet2_pt_", "jet1_pt_", "helic", "ht_", "mt_", "dR_higgs_lep", "dipho_deltaR", "sublead_eta_", "lead_eta_", "met_", "lep_eta_", "subleadptoM_", "leadptoM_", "pt_higgs", "max2_btag_", "max1_btag_", "lep_pt_", "minIDMVA_", "maxIDMVA_", "subleadPSV_", "leadPSV_", "nb_loose_", "njets_",  };
    
    int 	label_;
    int 	multi_label_;
    double	evt_weight_;
    int 	process_id_;
    double	rand_;
    double 	super_rand_;
    double	mass_;

    // Variable declarations
    double           dipho_rapidity_;
    double           jet3_eta_;
    double           jet2_eta_;
    double           jet1_eta_;
    double           jet3_pt_;
    double           jet2_pt_;
    double           jet1_pt_;
    double           helic;
    double           ht_;
    double           mt_;
    double           dR_higgs_lep;
    double           dipho_deltaR;
    double           sublead_eta_;
    double           lead_eta_;
    double           met_;
    double           lep_eta_;
    double           subleadptoM_;
    double           leadptoM_;
    double           pt_higgs;
    double           max2_btag_;
    double           max1_btag_;
    double           lep_pt_;
    double           minIDMVA_;
    double           maxIDMVA_;
    int           subleadPSV_;
    int           leadPSV_;
    int           nb_loose_;
    int           njets_;


 


};

inline
void BabyMaker::MakeBabyNtuple(const char *BabyFilename){
  BabyFile_ = new TFile(Form("%s", BabyFilename), "RECREATE");
  BabyFile_->cd();
  BabyTree_ = new TTree("t", "A Baby Ntuple");

  BabyTree_->Branch("mva_branches"	, &mva_branches );

  BabyTree_->Branch("evt_weight_"     	, &evt_weight_  );
  BabyTree_->Branch("label_"     	, &label_       );
  BabyTree_->Branch("multi_label_"            , &multi_label_       );
  BabyTree_->Branch("process_id_"     	, &process_id_  );
  BabyTree_->Branch("rand_"            , &rand_       );
  BabyTree_->Branch("super_rand_"            , &super_rand_       );
  BabyTree_->Branch("mass_"		, &mass_	);

  // Variable branches
  BabyTree_->Branch("dipho_rapidity_" ,&dipho_rapidity_);
  BabyTree_->Branch("jet3_eta_" ,&jet3_eta_);
  BabyTree_->Branch("jet2_eta_" ,&jet2_eta_);
  BabyTree_->Branch("jet1_eta_" ,&jet1_eta_);
  BabyTree_->Branch("jet3_pt_" ,&jet3_pt_);
  BabyTree_->Branch("jet2_pt_" ,&jet2_pt_);
  BabyTree_->Branch("jet1_pt_" ,&jet1_pt_);
  BabyTree_->Branch("helic" ,&helic);
  BabyTree_->Branch("ht_" ,&ht_);
  BabyTree_->Branch("mt_" ,&mt_);
  BabyTree_->Branch("dR_higgs_lep" ,&dR_higgs_lep);
  BabyTree_->Branch("dipho_deltaR" ,&dipho_deltaR);
  BabyTree_->Branch("sublead_eta_" ,&sublead_eta_);
  BabyTree_->Branch("lead_eta_" ,&lead_eta_);
  BabyTree_->Branch("met_" ,&met_);
  BabyTree_->Branch("lep_eta_" ,&lep_eta_);
  BabyTree_->Branch("subleadptoM_" ,&subleadptoM_);
  BabyTree_->Branch("leadptoM_" ,&leadptoM_);
  BabyTree_->Branch("pt_higgs" ,&pt_higgs);
  BabyTree_->Branch("max2_btag_" ,&max2_btag_);
  BabyTree_->Branch("max1_btag_" ,&max1_btag_);
  BabyTree_->Branch("lep_pt_" ,&lep_pt_);
  BabyTree_->Branch("minIDMVA_" ,&minIDMVA_);
  BabyTree_->Branch("maxIDMVA_" ,&maxIDMVA_);
  BabyTree_->Branch("subleadPSV_" ,&subleadPSV_);
  BabyTree_->Branch("leadPSV_" ,&leadPSV_);
  BabyTree_->Branch("nb_loose_" ,&nb_loose_);
  BabyTree_->Branch("njets_" ,&njets_);
  //BabyTree_->Branch("nbjets_"		, &nbjets_	);

 
  


  return;
}

inline
void BabyMaker::InitBabyNtuple () {
  return;
}

inline
void BabyMaker::FillBabyNtuple(){
  BabyTree_->Fill();
  return;
}

inline
void BabyMaker::CloseBabyNtuple(){
  BabyFile_->cd();
  BabyTree_->Write();
  BabyFile_->Close();
  return;
}

#endif
