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
    vector<string> mva_branches = {"mt_", "lep_pt_", "minIDMVA_", "maxIDMVA_", "subleadPSV_", "leadPSV_", "nb_loose_", "njets_",  };
    
    int 	label_;
    double	evt_weight_;
    int 	process_id_;
    double	rand_;
    double 	super_rand_;
    double	mass_;

    // Variable declarations
    double           mt_;
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
  BabyTree_->Branch("process_id_"     	, &process_id_  );
  BabyTree_->Branch("rand_"            , &rand_       );
  BabyTree_->Branch("super_rand_"            , &super_rand_       );
  BabyTree_->Branch("mass_"		, &mass_	);

  // Variable branches
  BabyTree_->Branch("mt_" ,&mt_);
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
