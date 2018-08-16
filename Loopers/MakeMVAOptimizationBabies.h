#ifndef MakeMVABabies_ttHHadronic_h 
#define MakeMVABabies_ttHHadronic_h

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

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

class OptimizationBabyMaker {
  public:
    OptimizationBabyMaker() {};
    ~OptimizationBabyMaker() {
      if (BabyFile_) delete BabyFile_;
      if (BabyTree_) delete BabyTree_;
    }
    void MakeBabyNtuple(const char *);
    void InitBabyNtuple();
    void FillBabyNtuple(int label, double evt_weight, int process_id, double rand, double mass, double mva_score, double reference_mva, bool pass_ref_presel);
    void CloseBabyNtuple();

        

  private:
    TFile *BabyFile_;
    TTree *BabyTree_;

    int 	label_;
    double	evt_weight_;
    int 	process_id_;
    double 	rand_;

    double	mass_;
    double 	mva_score_;
    double	reference_mva_;
    bool 	pass_ref_presel_;
};

inline
void OptimizationBabyMaker::MakeBabyNtuple(const char *BabyFilename){
  BabyFile_ = new TFile(Form("%s", BabyFilename), "RECREATE");
  BabyFile_->cd();
  BabyTree_ = new TTree("t", "A Baby Ntuple");

  BabyTree_->Branch("evt_weight_"     	, &evt_weight_  );
  BabyTree_->Branch("label_"     	, &label_       );
  BabyTree_->Branch("process_id_"     	, &process_id_  );
  BabyTree_->Branch("rand_"            	, &rand_       	);

  BabyTree_->Branch("mass_"       	, &mass_  );
  BabyTree_->Branch("mva_score_"       	, &mva_score_  );
  BabyTree_->Branch("reference_mva_"	, &reference_mva_ );
  BabyTree_->Branch("pass_ref_presel_"	, &pass_ref_presel_);
  return;
}

inline
void OptimizationBabyMaker::InitBabyNtuple () {
  return;
}

inline
void OptimizationBabyMaker::FillBabyNtuple(int label, double evt_weight, int process_id, double rand, double mass, double mva_score, double reference_mva, bool pass_ref_presel){
  label_ = label;
  evt_weight_ = evt_weight;
  process_id_ = process_id;
  rand_ = rand;

  mass_ = mass;
  mva_score_ = mva_score;
  reference_mva_ = reference_mva;
  pass_ref_presel_ = pass_ref_presel;

  BabyTree_->Fill();
  return;
}

inline
void OptimizationBabyMaker::CloseBabyNtuple(){
  BabyFile_->cd();
  BabyTree_->Write();
  BabyFile_->Close();
  return;
}

#endif
