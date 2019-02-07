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
    vector<string> mva_branches = {"maxIDMVA_", "minIDMVA_", "max2_btag_", "max1_btag_", "dipho_delta_R", "njets_", "ht_", "leadptoM_", "subleadptoM_", "leadIDMVA_", "subleadIDMVA_", "lead_eta_", "sublead_eta_", "jet1_pt_", "jet1_eta_", "jet1_btag_", "jet2_pt_", "jet2_eta_", "jet2_btag_", "jet3_pt_", "jet3_eta_", "jet3_btag_", "jet4_pt_", "jet4_eta_", "jet4_btag_", "jet5_pt_", "jet5_eta_", "jet5_btag_", "jet6_pt_", "jet6_eta_", "jet6_btag_", "leadPSV_", "subleadPSV_", "dipho_cosphi_", "dipho_rapidity_", "met_"};
    

    int 	label_;
    int         multi_label_;
    int		data_sideband_label_;
    int		signal_mass_label_;
    float	evt_weight_;
    int 	process_id_;
    float 	rand_;
    float       super_rand_;
    float       mass_;
    float       lead_sigmaEtoE_;
    float       sublead_sigmaEtoE_;

    float	tth_2017_reference_mva_;
    float	tth_qcdX_mva_;
    float	tth_ttX_mva_;
    float	tth_ttPP_mva_;
    float	tth_dipho_mva_;
    float	tth_std_mva_;

    // DNN Business
    vector<vector<float>>  objects_;
    vector<vector<float>>  objects_boosted_;

    // Variable declarations
    float       maxIDMVA_;
    float       minIDMVA_;
    float       max2_btag_;
    float       max1_btag_;
    float       dipho_delta_R;
    float	njets_;
    int		nbjets_;
    float	ht_;

    float       top_tag_score_;

    float	jet1_pt_;
    float       jet1_eta_;
    float 	jet1_btag_;
    float       jet2_pt_;
    float       jet2_eta_;
    float       jet2_btag_;
    float       jet3_pt_;
    float       jet3_eta_;
    float       jet3_btag_;
    float       jet4_pt_;
    float       jet4_eta_;
    float       jet4_btag_;
    float       jet5_pt_;
    float       jet5_eta_;
    float       jet5_btag_;
    float       jet6_pt_;
    float       jet6_eta_;
    float       jet6_btag_;

    float       lead_pT_;
    float       sublead_pT_;
    float	leadptoM_;
    float	subleadptoM_;
    float	leadIDMVA_;
    float 	subleadIDMVA_;
    float       lead_eta_;
    float       sublead_eta_;
    float	lead_phi_;
    float	sublead_phi_;
 
    float       leadPSV_;
    float       subleadPSV_;

    float	dipho_cosphi_;
    float 	dipho_rapidity_;
    float       dipho_pt_;
    float       met_;
    float	met_phi_; 
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
  BabyTree_->Branch("signal_mass_label_"            , &signal_mass_label_       );
  BabyTree_->Branch("data_sideband_label_"            , &data_sideband_label_       );
  BabyTree_->Branch("process_id_"     	, &process_id_  );
  BabyTree_->Branch("rand_"            , &rand_       );
  BabyTree_->Branch("super_rand_"            , &super_rand_       );
  BabyTree_->Branch("mass_"             , &mass_        );
 
  BabyTree_->Branch("tth_2017_reference_mva_", 	&tth_2017_reference_mva_); 
  BabyTree_->Branch("tth_qcdX_mva_"             , &tth_qcdX_mva_        );
  BabyTree_->Branch("tth_ttX_mva_"             , &tth_ttX_mva_        );
  BabyTree_->Branch("tth_ttPP_mva_"             , &tth_ttPP_mva_        );
  BabyTree_->Branch("tth_dipho_mva_"             , &tth_dipho_mva_        );
  BabyTree_->Branch("tth_std_mva_"		, &tth_std_mva_		);

  BabyTree_->Branch("lead_sigmaEtoE_"             , &lead_sigmaEtoE_        );
  BabyTree_->Branch("sublead_sigmaEtoE_"             , &sublead_sigmaEtoE_        );

  BabyTree_->Branch("objects_"             , &objects_        );
  BabyTree_->Branch("objects_boosted_"             , &objects_boosted_        );

  // Variable branches
  BabyTree_->Branch("maxIDMVA_" ,&maxIDMVA_);
  BabyTree_->Branch("minIDMVA_" ,&minIDMVA_);
  BabyTree_->Branch("max2_btag_" ,&max2_btag_);
  BabyTree_->Branch("max1_btag_" ,&max1_btag_);
  BabyTree_->Branch("dipho_delta_R" ,&dipho_delta_R);
  BabyTree_->Branch("njets_"     	, &njets_       );
  BabyTree_->Branch("nbjets_"		, &nbjets_	);
  BabyTree_->Branch("ht_"    		, &ht_       	);

  BabyTree_->Branch("top_tag_score_"               , &top_tag_score_          );

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
 
  BabyTree_->Branch("lead_pT_"         , &lead_pT_      );
  BabyTree_->Branch("sublead_pT_"      , &sublead_pT_   );
  BabyTree_->Branch("leadptoM_"    	, &leadptoM_      );
  BabyTree_->Branch("subleadptoM_"    	, &subleadptoM_   );
  BabyTree_->Branch("leadIDMVA_"    	, &leadIDMVA_    );
  BabyTree_->Branch("subleadIDMVA_"    	, &subleadIDMVA_    );
  BabyTree_->Branch("lead_eta_"        , &lead_eta_    );
  BabyTree_->Branch("sublead_eta_"     , &sublead_eta_    ); 
  BabyTree_->Branch("leadPSV_"           , &leadPSV_      );
  BabyTree_->Branch("subleadPSV_"        , &subleadPSV_   );
  BabyTree_->Branch("lead_phi_"        , &lead_phi_    );
  BabyTree_->Branch("sublead_phi_"     , &sublead_phi_    ); 
 
  BabyTree_->Branch("dipho_cosphi_"           , &dipho_cosphi_      );
  BabyTree_->Branch("dipho_rapidity_"           , &dipho_rapidity_      );
  BabyTree_->Branch("dipho_pt_"           , &dipho_pt_      );
  BabyTree_->Branch("met_"           , &met_      );
  BabyTree_->Branch("met_phi_"           , &met_phi_      );

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
