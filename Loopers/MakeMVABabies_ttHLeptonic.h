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
    void ScanChain(TChain* chain, TString tag, TString year, TString ext, TString bkg_options, TString mYear, TString idx, bool fcnc, bool blind, bool fast, int nEvents, string skimFilePrefix);
    void MakeBabyNtuple(const char *);
    void InitBabyNtuple();
    void FillBabyNtuple();
    void CloseBabyNtuple();
    

  private:
    TFile *BabyFile_;
    TTree *BabyTree_;

    // Variable names
    vector<string> mva_branches = {"lep_pt_", "minIDMVA_", "maxIDMVA_", "subleadPSV_", "leadPSV_", "nb_loose_", "njets_",  };

    int 	year_;
    unsigned long long          evt_;
    unsigned long long          run_;
    unsigned long long          lumi_;
    int 	label_;
    int 	multi_label_;
    int         data_sideband_label_;
    int 	signal_mass_label_;
    int     signal_mass_category_;
    float	evt_weight_;
    int 	process_id_;
    int         genPhotonId_;
    float	rand_;
    float 	super_rand_;
    float	mass_;
    float 	lead_sigmaEtoE_;
    float 	sublead_sigmaEtoE_;

    float       tth_runII_mva_;
    float       tth_2017_reference_mva_;
    float       tth_qcdX_mva_;
    float       tth_ttX_mva_;
    float       tth_ttPP_mva_;
    float       tth_std_mva_;
    float       tth_dipho_mva_;

    // DNN Business
    vector<vector<float>>  jets_;
    vector<vector<float>>  objects_;
    vector<vector<float>>  leptons_;

    // Variable declarations
    float       maxIDMVA_;
    float       minIDMVA_;
    float       max2_btag_;
    float       max1_btag_;
    float       dipho_delta_R;
    float       njets_;
    int         nbjets_;
    float       ht_;

    float	top_tag_score_;

    float       lep_pt_;
    float 	    lep_eta_;

    float       ele1_pt_;
    float       ele1_eta_;
    float       ele1_phi_;
    float       ele1_energy_;
    float       n_ele_;

    float       muon1_pt_;
    float       muon1_eta_;
    float       muon1_phi_;
    float       muon1_energy_;
    float       n_muons_;

    float       lep1_charge_;
    float       lep2_charge_;

    float       forward_jet_pt_;
    float       forward_jet_eta_;

    float	    n_lep_loose_;
    float       n_lep_medium_;
    float       n_lep_tight_;

    float	    muon1_mini_iso_;
    float	    muon2_mini_iso_;

    float      	jet1_pt_;
    float	    jet1_eta_;
    float       jet1_btag_;
    float      	jet2_pt_;
    float      	jet2_eta_;
    float      	jet2_btag_;
    float      	jet3_pt_;
    float      	jet3_eta_;
    float      	jet3_btag_;
    float      	jet4_pt_;
    float      	jet4_eta_;
    float      	jet4_btag_;
    float      	jet5_pt_;
    float      	jet5_eta_;
    float      	jet5_btag_;
    float      	jet6_pt_;
    float      	jet6_eta_;
    float      	jet6_btag_;

    float       jet1_phi_;
    float       jet1_energy_;
    float       jet2_phi_;
    float       jet2_energy_;
    float       jet3_phi_;
    float       jet3_energy_;
    float       jet4_phi_;
    float       jet4_energy_;
    float       jet5_phi_;
    float       jet5_energy_;
    float       jet6_phi_;
    float       jet6_energy_; 

    float      	lead_pT_;
    float      	sublead_pT_;
    float       leadptoM_;
    float       subleadptoM_;
    float       leadIDMVA_;
    float       subleadIDMVA_;
    float      	lead_eta_;
    float      	sublead_eta_;
    float       lead_phi_;
    float       sublead_phi_;

    float       leadPSV_;
    float       subleadPSV_;

    float       dipho_cosphi_;
    float       dipho_rapidity_;
    float       dipho_pt_;
    float       dipho_pt_over_mass_;
    float       met_;
    float	    log_met_;
    float       met_phi_; 

    float	helicity_angle_;

    //#quadratic equation related
    float chi2_neutrino_pz_;
    float chi2_tbw_mass_;
    float chi2_tbw_pt_;
    float chi2_tbw_eta_;
    float chi2_tbw_deltaR_dipho_;
    float chi2_qjet_pt_;
    float chi2_qjet_eta_;
    float chi2_qjet_btag_;
    float chi2_qjet_deltaR_dipho_;
    float chi2_tqh_ptOverM_;
    float chi2_tqh_eta_;
    float chi2_tqh_deltaR_tbw_;
    float chi2_tqh_deltaR_dipho_;

};

inline
void BabyMaker::MakeBabyNtuple(const char *BabyFilename){
  BabyFile_ = new TFile(Form("%s", BabyFilename), "RECREATE");
  BabyFile_->cd();
  BabyTree_ = new TTree("t", "A Baby Ntuple");

  BabyTree_->Branch("mva_branches"	, &mva_branches );

  BabyTree_->Branch("year_"       , &year_  );
  BabyTree_->Branch("evt_"       , &evt_  );
  BabyTree_->Branch("run_"       , &run_  );
  BabyTree_->Branch("lumi_"       , &lumi_  );
  BabyTree_->Branch("evt_weight_"     	, &evt_weight_  );
  BabyTree_->Branch("label_"     	, &label_       );
  BabyTree_->Branch("multi_label_"            , &multi_label_       );
  BabyTree_->Branch("data_sideband_label_"            , &data_sideband_label_       );
  BabyTree_->Branch("signal_mass_label_"            , &signal_mass_label_       );
  BabyTree_->Branch("signal_mass_category_"            , &signal_mass_category_       );
  BabyTree_->Branch("genPhotonId_"     	, &genPhotonId_  );
  BabyTree_->Branch("process_id_"     	, &process_id_  );
  BabyTree_->Branch("rand_"            , &rand_       );
  BabyTree_->Branch("super_rand_"            , &super_rand_       );
  BabyTree_->Branch("mass_"		, &mass_	);

  BabyTree_->Branch("tth_runII_mva_", &tth_runII_mva_);
  BabyTree_->Branch("tth_2017_reference_mva_",  &tth_2017_reference_mva_);
  BabyTree_->Branch("tth_qcdX_mva_"             , &tth_qcdX_mva_        );
  BabyTree_->Branch("tth_ttX_mva_"             , &tth_ttX_mva_        );
  BabyTree_->Branch("tth_ttPP_mva_"             , &tth_ttPP_mva_        );
  BabyTree_->Branch("tth_dipho_mva_"             , &tth_dipho_mva_        );
  BabyTree_->Branch("tth_std_mva_"             , &tth_std_mva_        );

  BabyTree_->Branch("lead_sigmaEtoE_"             , &lead_sigmaEtoE_        );
  BabyTree_->Branch("sublead_sigmaEtoE_"             , &sublead_sigmaEtoE_        );

  BabyTree_->Branch("objects_"             , &objects_        );
  BabyTree_->Branch("jets_"             , &jets_        );
  BabyTree_->Branch("leptons_"             , &leptons_        );

  // Variable branches
  BabyTree_->Branch("maxIDMVA_" ,&maxIDMVA_);
  BabyTree_->Branch("minIDMVA_" ,&minIDMVA_);
  BabyTree_->Branch("max2_btag_" ,&max2_btag_);
  BabyTree_->Branch("max1_btag_" ,&max1_btag_);
  BabyTree_->Branch("dipho_delta_R" ,&dipho_delta_R);
  BabyTree_->Branch("njets_"            , &njets_       );
  BabyTree_->Branch("nbjets_"         , &nbjets_      );
  BabyTree_->Branch("ht_"               , &ht_          );

  BabyTree_->Branch("lep_pt_"               , &lep_pt_          );
  BabyTree_->Branch("lep_eta_"               , &lep_eta_          );

  BabyTree_->Branch("lep1_charge_"               , &lep1_charge_          );
  BabyTree_->Branch("lep2_charge_"               , &lep2_charge_          );
 
  BabyTree_->Branch("forward_jet_pt_"               , &forward_jet_pt_          );
  BabyTree_->Branch("forward_jet_eta_"               , &forward_jet_eta_          );

  BabyTree_->Branch("ele1_pt_"               , &ele1_pt_        );
  BabyTree_->Branch("ele1_eta_"               , &ele1_eta_        );
  BabyTree_->Branch("ele1_phi_"               , &ele1_phi_        );
  BabyTree_->Branch("ele1_energy_"               , &ele1_energy_        );
  BabyTree_->Branch("n_ele_"                    , &n_ele_       );

  BabyTree_->Branch("muon1_pt_"               , &muon1_pt_        );
  BabyTree_->Branch("muon1_eta_"               , &muon1_eta_        );
  BabyTree_->Branch("muon1_phi_"               , &muon1_phi_        );
  BabyTree_->Branch("muon1_energy_"               , &muon1_energy_        );
  BabyTree_->Branch("n_muons_"                    , &n_muons_       );

  BabyTree_->Branch("n_lep_loose_"               , &n_lep_loose_          );
  BabyTree_->Branch("n_lep_medium_"               , &n_lep_medium_          );
  BabyTree_->Branch("n_lep_tight_"               , &n_lep_tight_          );

  BabyTree_->Branch("muon1_mini_iso_"               , &muon1_mini_iso_          );
  BabyTree_->Branch("muon2_mini_iso_"               , &muon2_mini_iso_          );

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

  BabyTree_->Branch("jet1_phi_"             , &jet1_phi_ );
  BabyTree_->Branch("jet1_energy_"         , &jet1_energy_ );
  BabyTree_->Branch("jet2_phi_"             , &jet2_phi_ );
  BabyTree_->Branch("jet2_energy_"         , &jet2_energy_ );
  BabyTree_->Branch("jet3_phi_"             , &jet3_phi_ );
  BabyTree_->Branch("jet3_energy_"         , &jet3_energy_ );
  BabyTree_->Branch("jet4_phi_"             , &jet4_phi_ );
  BabyTree_->Branch("jet4_energy_"         , &jet4_energy_ );
  BabyTree_->Branch("jet5_phi_"             , &jet5_phi_ );
  BabyTree_->Branch("jet5_energy_"         , &jet5_energy_ );
  BabyTree_->Branch("jet6_phi_"             , &jet6_phi_ );
  BabyTree_->Branch("jet6_energy_"         , &jet6_energy_ ); 

  BabyTree_->Branch("lead_pT_"         , &lead_pT_      );
  BabyTree_->Branch("sublead_pT_"      , &sublead_pT_   );
  BabyTree_->Branch("leadptoM_"         , &leadptoM_      );
  BabyTree_->Branch("subleadptoM_"      , &subleadptoM_   );
  BabyTree_->Branch("leadIDMVA_"        , &leadIDMVA_    );
  BabyTree_->Branch("subleadIDMVA_"     , &subleadIDMVA_    );
  BabyTree_->Branch("lead_eta_"        , &lead_eta_    );
  BabyTree_->Branch("sublead_eta_"     , &sublead_eta_    );
  BabyTree_->Branch("leadPSV_"           , &leadPSV_      );
  BabyTree_->Branch("subleadPSV_"        , &subleadPSV_   );
  BabyTree_->Branch("lead_phi_"        , &lead_phi_    );
  BabyTree_->Branch("sublead_phi_"     , &sublead_phi_    );

  BabyTree_->Branch("dipho_cosphi_"           , &dipho_cosphi_      );
  BabyTree_->Branch("dipho_rapidity_"           , &dipho_rapidity_      );
  BabyTree_->Branch("dipho_pt_"           , &dipho_pt_      );
  BabyTree_->Branch("dipho_pt_over_mass_"           , &dipho_pt_over_mass_      );
  BabyTree_->Branch("met_"           , &met_      );
  BabyTree_->Branch("log_met_"           , &log_met_      );
  BabyTree_->Branch("met_phi_"           , &met_phi_      );

  BabyTree_->Branch("helicity_angle_"           , &helicity_angle_      );
  //#quadratic equation related
  BabyTree_->Branch("chi2_neutrino_pz_", &chi2_neutrino_pz_);
  BabyTree_->Branch("chi2_tbw_mass_", &chi2_tbw_mass_);
  BabyTree_->Branch("chi2_tbw_pt_", &chi2_tbw_pt_);
  BabyTree_->Branch("chi2_tbw_eta_", &chi2_tbw_eta_);
  BabyTree_->Branch("chi2_tbw_deltaR_dipho_", &chi2_tbw_deltaR_dipho_);
  BabyTree_->Branch("chi2_qjet_pt_", &chi2_qjet_pt_);
  BabyTree_->Branch("chi2_qjet_eta_", &chi2_qjet_eta_);
  BabyTree_->Branch("chi2_qjet_btag_", &chi2_qjet_btag_);
  BabyTree_->Branch("chi2_qjet_deltaR_dipho_", &chi2_qjet_deltaR_dipho_);
  BabyTree_->Branch("chi2_tqh_ptOverM_", &chi2_tqh_ptOverM_);
  BabyTree_->Branch("chi2_tqh_eta_", &chi2_tqh_eta_);
  BabyTree_->Branch("chi2_tqh_deltaR_tbw_", &chi2_tqh_deltaR_tbw_);
  BabyTree_->Branch("chi2_tqh_deltaR_dipho_", &chi2_tqh_deltaR_dipho_);
  


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
