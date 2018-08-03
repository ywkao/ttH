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

class BabyMaker {
  public:
    BabyMaker() {};
    ~BabyMaker() {
      if (BabyFile_) delete BabyFile_;
      if (BabyTree_) delete BabyTree_;
    }
    void ScanChain(TChain* chain, TString tag, bool blind, bool fast, int nEvents, string skimFilePrefix);
    void MakeBabyNtuple(const char *);
    void InitBabyNtuple();
    void FillBabyNtuple();
    void CloseBabyNtuple();
    

  private:
    TFile *BabyFile_;
    TTree *BabyTree_;

    vector<string> mva_branches = {"njets_", "ht_", "leadptoM_", "subleadptoM_", "leadIDMVA_", "subleadIDMVA_", "lead_eta_", "sublead_eta_", "jet1_pt_", "jet1_eta_", "jet1_btag_", "jet2_pt_", "jet2_eta_", "jet2_btag_", "jet3_pt_", "jet3_eta_", "jet3_btag_", "jet4_pt_", "jet4_eta_", "jet4_btag_", "jet5_pt_", "jet5_eta_", "jet5_btag_", "jet6_pt_", "jet6_eta_", "jet6_btag_", "max1_btag_", "max2_btag_", "leadPSV_", "subleadPSV_", "dipho_cosphi_", "dipho_rapidity_", "met_"};
    

    int 	label_;
    double	evt_weight_;
    int 	process_id_;
    int		use_for_train_;
    
    int		njets_;
    //int		nbjets_;
    double	ht_;

    double	jet1_pt_;
    double      jet1_eta_;
    double 	jet1_btag_;
    double      jet2_pt_;
    double      jet2_eta_;
    double      jet2_btag_;
    double      jet3_pt_;
    double      jet3_eta_;
    double      jet3_btag_;
    double      jet4_pt_;
    double      jet4_eta_;
    double      jet4_btag_;
    double      jet5_pt_;
    double      jet5_eta_;
    double      jet5_btag_;
    double      jet6_pt_;
    double      jet6_eta_;
    double      jet6_btag_;

    double	max1_btag_;
    double	max2_btag_;

    double	leadptoM_;
    double	subleadptoM_;
    double	leadIDMVA_;
    double 	subleadIDMVA_;
    double      lead_eta_;
    double      sublead_eta_;
 
    double      leadPSV_;
    double      subleadPSV_;

    double	dipho_cosphi_;
    double 	dipho_rapidity_;
    double      met_;    
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

  BabyTree_->Branch("njets_"     	, &njets_       );
  //BabyTree_->Branch("nbjets_"		, &nbjets_	);
  BabyTree_->Branch("ht_"    		, &ht_       	);

  BabyTree_->Branch("jet1_pt_"            , &jet1_pt_   );
  BabyTree_->Branch("jet1_eta_"            , &jet1_eta_ );
  BabyTree_->Branch("jet1_btag_"            , &jet1_btag_ );
  BabyTree_->Branch("jet2_pt_"            , &jet2_pt_   );
  BabyTree_->Branch("jet2_eta_"            , &jet2_eta_ );
  BabyTree_->Branch("jet2_btag_"            , &jet2_btag_ );
  BabyTree_->Branch("jet3_pt_"            , &jet3_pt_   );
  BabyTree_->Branch("jet3_eta_"            , &jet3_eta_ );
  BabyTree_->Branch("jet3_btag_"            , &jet3_btag_ );
  BabyTree_->Branch("jet4_pt_"            , &jet4_pt_   );
  BabyTree_->Branch("jet4_eta_"            , &jet4_eta_ );
  BabyTree_->Branch("jet4_btag_"            , &jet4_btag_ );
  BabyTree_->Branch("jet5_pt_"            , &jet5_pt_   );
  BabyTree_->Branch("jet5_eta_"            , &jet5_eta_ );
  BabyTree_->Branch("jet5_btag_"            , &jet5_btag_ );
  BabyTree_->Branch("jet6_pt_"            , &jet6_pt_   );
  BabyTree_->Branch("jet6_eta_"            , &jet6_eta_ );
  BabyTree_->Branch("jet6_btag_"            , &jet6_btag_ );
 
  BabyTree_->Branch("max1_btag_"            , &max1_btag_   );
  BabyTree_->Branch("max2_btag_"            , &max2_btag_   );

  BabyTree_->Branch("leadptoM_"    	, &leadptoM_      );
  BabyTree_->Branch("subleadptoM_"    	, &subleadptoM_   );
  BabyTree_->Branch("leadIDMVA_"    	, &leadIDMVA_    );
  BabyTree_->Branch("subleadIDMVA_"    	, &subleadIDMVA_    );
  BabyTree_->Branch("lead_eta_"        , &lead_eta_    );
  BabyTree_->Branch("sublead_eta_"     , &sublead_eta_    ); 
  BabyTree_->Branch("leadPSV_"           , &leadPSV_      );
  BabyTree_->Branch("subleadPSV_"        , &subleadPSV_   );
  
  BabyTree_->Branch("dipho_cosphi_"           , &dipho_cosphi_      );
  BabyTree_->Branch("dipho_rapidity_"           , &dipho_rapidity_      );
  BabyTree_->Branch("met_"           , &met_      );

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
