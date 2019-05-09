#include "MakeMVABabies_ttHHadronic.h"
#include "ScanChain_ttHHadronic.h"

void BabyMaker::ScanChain(TChain* chain, TString tag, TString year, TString ext, TString bkg_options, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Make baby ntuple
  MakeBabyNtuple( Form("%s.root", ("MVABaby_ttHHadronic_" + ext).Data()));

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if (nEvents >= 0) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // Dumb hacky stuff to use 2017 MC samples as placeholders for 2018
  bool already_looped_dipho = false;
  bool already_looped_qcd = false;
  bool already_looped_dy = false;

  TF1* gjet_minID_shape = get_photon_ID_shape("min");
  TF1* gjet_maxID_shape = get_photon_ID_shape("max");
  TF1* photon_fakeID_shape = get_photon_ID_shape("fake");
  TF1* photon_fakeID_shape_runII = get_photon_ID_shape("fake_runII");

  // MVA Businesss
  unique_ptr<TMVA::Reader> gjet_mva;
  gjet_mva.reset(new TMVA::Reader( "!Color:Silent" ));

  gjet_mva->AddVariable("njets_", &njets_);
  gjet_mva->AddVariable("ht_", &ht_);
  gjet_mva->AddVariable("jet1_pt_", &jet1_pt_);
  gjet_mva->AddVariable("jet2_pt_", &jet2_pt_);
  gjet_mva->AddVariable("jet1_eta_", &jet1_eta_);
  gjet_mva->AddVariable("jet2_eta_", &jet2_eta_);
  gjet_mva->AddVariable("max1_btag_", &max1_btag_);
  gjet_mva->AddVariable("max2_btag_", &max2_btag_);
  gjet_mva->AddVariable("leadptoM_", &leadptoM_);
  gjet_mva->AddVariable("subleadptoM_", &subleadptoM_);
  gjet_mva->AddVariable("lead_eta_", &lead_eta_);
  gjet_mva->AddVariable("sublead_eta_", &sublead_eta_);
  gjet_mva->AddVariable("minIDMVA_", &minIDMVA_);
  gjet_mva->AddVariable("maxIDMVA_", &maxIDMVA_);
  gjet_mva->AddVariable("lead_pT_", &lead_pT_);
  gjet_mva->AddVariable("sublead_pT_", &sublead_pT_);
  gjet_mva->AddVariable("dipho_pt_", &dipho_pt_);
  gjet_mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
  gjet_mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
  gjet_mva->AddVariable("leadIDMVA_", &leadIDMVA_);
  gjet_mva->AddVariable("subleadIDMVA_", &subleadIDMVA_);

  gjet_mva->BookMVA("BDT", gjet_bdt_file);


  // ttH MVA Business
  unique_ptr<TMVA::Reader> tth_qcdX_mva;
  unique_ptr<TMVA::Reader> tth_ttX_mva;
  unique_ptr<TMVA::Reader> tth_ttPP_mva;
  unique_ptr<TMVA::Reader> tth_dipho_mva;
  unique_ptr<TMVA::Reader> tth_std_mva;

  bool do_tth_ttX_mva = false;
  if (do_tth_ttX_mva) {
    tth_ttX_mva.reset(new TMVA::Reader( "!Color:Silent" ));

    tth_ttX_mva->AddVariable("maxIDMVA_", &maxIDMVA_);
    tth_ttX_mva->AddVariable("minIDMVA_", &minIDMVA_);
    tth_ttX_mva->AddVariable("max2_btag_", &max2_btag_);
    tth_ttX_mva->AddVariable("max1_btag_", &max1_btag_);
    tth_ttX_mva->AddVariable("dipho_delta_R", &dipho_delta_R);
    tth_ttX_mva->AddVariable("njets_", &njets_);
    //tth_ttX_mva->AddVariable("nbjets_", &nbjets_);
    tth_ttX_mva->AddVariable("ht_", &ht_);
    tth_ttX_mva->AddVariable("leadptoM_", &leadptoM_);
    tth_ttX_mva->AddVariable("subleadptoM_", &subleadptoM_);
    tth_ttX_mva->AddVariable("leadIDMVA_", &leadIDMVA_);
    tth_ttX_mva->AddVariable("subleadIDMVA_", &subleadIDMVA_);
    tth_ttX_mva->AddVariable("lead_eta_", &lead_eta_);
    tth_ttX_mva->AddVariable("sublead_eta_", &sublead_eta_);

    tth_ttX_mva->AddVariable("jet1_pt_", &jet1_pt_);
    tth_ttX_mva->AddVariable("jet1_eta_", &jet1_eta_);
    tth_ttX_mva->AddVariable("jet1_btag_", &jet1_btag_);
    tth_ttX_mva->AddVariable("jet2_pt_", &jet2_pt_);
    tth_ttX_mva->AddVariable("jet2_eta_", &jet2_eta_);
    tth_ttX_mva->AddVariable("jet2_btag_", &jet2_btag_);
    tth_ttX_mva->AddVariable("jet3_pt_", &jet3_pt_);
    tth_ttX_mva->AddVariable("jet3_eta_", &jet3_eta_);
    tth_ttX_mva->AddVariable("jet3_btag_", &jet3_btag_);
    tth_ttX_mva->AddVariable("jet4_pt_", &jet4_pt_);
    tth_ttX_mva->AddVariable("jet4_eta_", &jet4_eta_);
    tth_ttX_mva->AddVariable("jet4_btag_", &jet4_btag_);
    //tth_ttX_mva->AddVariable("jet5_pt_", &jet5_pt_);
    //tth_ttX_mva->AddVariable("jet5_eta_", &jet5_eta_);
    //tth_ttX_mva->AddVariable("jet5_btag_", &jet5_btag_);
    //tth_ttX_mva->AddVariable("jet6_pt_", &jet6_pt_);
    //tth_ttX_mva->AddVariable("jet6_eta_", &jet6_eta_);
    //tth_ttX_mva->AddVariable("jet6_btag_", &jet6_btag_);

    tth_ttX_mva->AddVariable("leadPSV_", &leadPSV_);
    tth_ttX_mva->AddVariable("subleadPSV_", &subleadPSV_);

    tth_ttX_mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
    tth_ttX_mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
    tth_ttX_mva->AddVariable("met_", &met_);

    tth_ttX_mva->AddVariable("top_tag_score_", &top_tag_score_);

    tth_ttX_mva->BookMVA("BDT", "../MVAs/Hadronic_1617_data_sideband_phoID_v2__bdt.xml");
  }

  bool do_tth_qcdX_mva = false;
  if (do_tth_qcdX_mva) {
    tth_qcdX_mva.reset(new TMVA::Reader( "!Color:Silent" ));
    
    tth_qcdX_mva->AddVariable("maxIDMVA_", &maxIDMVA_);
    tth_qcdX_mva->AddVariable("minIDMVA_", &minIDMVA_);
    tth_qcdX_mva->AddVariable("max2_btag_", &max2_btag_);
    tth_qcdX_mva->AddVariable("max1_btag_", &max1_btag_);
    tth_qcdX_mva->AddVariable("dipho_delta_R", &dipho_delta_R);
    tth_qcdX_mva->AddVariable("njets_", &njets_);
    //tth_qcdX_mva->AddVariable("nbjets_", &nbjets_);
    tth_qcdX_mva->AddVariable("ht_", &ht_);
    tth_qcdX_mva->AddVariable("leadptoM_", &leadptoM_);
    tth_qcdX_mva->AddVariable("subleadptoM_", &subleadptoM_);
    tth_qcdX_mva->AddVariable("leadIDMVA_", &leadIDMVA_);
    tth_qcdX_mva->AddVariable("subleadIDMVA_", &subleadIDMVA_);
    tth_qcdX_mva->AddVariable("lead_eta_", &lead_eta_);
    tth_qcdX_mva->AddVariable("sublead_eta_", &sublead_eta_);
    
    tth_qcdX_mva->AddVariable("jet1_pt_", &jet1_pt_);
    tth_qcdX_mva->AddVariable("jet1_eta_", &jet1_eta_);
    tth_qcdX_mva->AddVariable("jet1_btag_", &jet1_btag_);
    tth_qcdX_mva->AddVariable("jet2_pt_", &jet2_pt_);
    tth_qcdX_mva->AddVariable("jet2_eta_", &jet2_eta_);
    tth_qcdX_mva->AddVariable("jet2_btag_", &jet2_btag_);
    tth_qcdX_mva->AddVariable("jet3_pt_", &jet3_pt_);
    tth_qcdX_mva->AddVariable("jet3_eta_", &jet3_eta_);
    tth_qcdX_mva->AddVariable("jet3_btag_", &jet3_btag_);
    tth_qcdX_mva->AddVariable("jet4_pt_", &jet4_pt_);
    tth_qcdX_mva->AddVariable("jet4_eta_", &jet4_eta_);
    tth_qcdX_mva->AddVariable("jet4_btag_", &jet4_btag_);
    //tth_qcdX_mva->AddVariable("jet5_pt_", &jet5_pt_);
    //tth_qcdX_mva->AddVariable("jet5_eta_", &jet5_eta_);
    //tth_qcdX_mva->AddVariable("jet5_btag_", &jet5_btag_);
    //tth_qcdX_mva->AddVariable("jet6_pt_", &jet6_pt_);
    //tth_qcdX_mva->AddVariable("jet6_eta_", &jet6_eta_);
    //tth_qcdX_mva->AddVariable("jet6_btag_", &jet6_btag_);
    
    tth_qcdX_mva->AddVariable("leadPSV_", &leadPSV_);
    tth_qcdX_mva->AddVariable("subleadPSV_", &subleadPSV_);
    
    tth_qcdX_mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
    tth_qcdX_mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
    tth_qcdX_mva->AddVariable("met_", &met_);
    
    tth_qcdX_mva->AddVariable("top_tag_score_", &top_tag_score_);
    
    tth_qcdX_mva->BookMVA("BDT", "../MVAs/Hadronic_1617_data_sideband_0b__bdt.xml");
  }

  bool do_tth_ttPP_mva = true;
  if (do_tth_ttPP_mva) {
    tth_ttPP_mva.reset(new TMVA::Reader( "!Color:Silent" ));
    
    tth_ttPP_mva->AddVariable("maxIDMVA_", &maxIDMVA_);
    tth_ttPP_mva->AddVariable("minIDMVA_", &minIDMVA_);
    tth_ttPP_mva->AddVariable("max2_btag_", &max2_btag_);
    tth_ttPP_mva->AddVariable("max1_btag_", &max1_btag_);
    tth_ttPP_mva->AddVariable("dipho_delta_R", &dipho_delta_R);
    tth_ttPP_mva->AddVariable("njets_", &njets_);
    //tth_ttPP_mva->AddVariable("nbjets_", &nbjets_);
    tth_ttPP_mva->AddVariable("ht_", &ht_);
    tth_ttPP_mva->AddVariable("leadptoM_", &leadptoM_);
    tth_ttPP_mva->AddVariable("subleadptoM_", &subleadptoM_);
    //tth_ttPP_mva->AddVariable("leadIDMVA_", &leadIDMVA_);
    //tth_ttPP_mva->AddVariable("subleadIDMVA_", &subleadIDMVA_);
    tth_ttPP_mva->AddVariable("lead_eta_", &lead_eta_);
    tth_ttPP_mva->AddVariable("sublead_eta_", &sublead_eta_);
    
    tth_ttPP_mva->AddVariable("jet1_pt_", &jet1_pt_);
    tth_ttPP_mva->AddVariable("jet1_eta_", &jet1_eta_);
    tth_ttPP_mva->AddVariable("jet1_btag_", &jet1_btag_);
    tth_ttPP_mva->AddVariable("jet2_pt_", &jet2_pt_);
    tth_ttPP_mva->AddVariable("jet2_eta_", &jet2_eta_);
    tth_ttPP_mva->AddVariable("jet2_btag_", &jet2_btag_);
    tth_ttPP_mva->AddVariable("jet3_pt_", &jet3_pt_);
    tth_ttPP_mva->AddVariable("jet3_eta_", &jet3_eta_);
    tth_ttPP_mva->AddVariable("jet3_btag_", &jet3_btag_);
    tth_ttPP_mva->AddVariable("jet4_pt_", &jet4_pt_);
    tth_ttPP_mva->AddVariable("jet4_eta_", &jet4_eta_);
    tth_ttPP_mva->AddVariable("jet4_btag_", &jet4_btag_);
    //tth_ttPP_mva->AddVariable("jet5_pt_", &jet5_pt_);
    //tth_ttPP_mva->AddVariable("jet5_eta_", &jet5_eta_);
    //tth_ttPP_mva->AddVariable("jet5_btag_", &jet5_btag_);
    //tth_ttPP_mva->AddVariable("jet6_pt_", &jet6_pt_);
    //tth_ttPP_mva->AddVariable("jet6_eta_", &jet6_eta_);
    //tth_ttPP_mva->AddVariable("jet6_btag_", &jet6_btag_);
    
    tth_ttPP_mva->AddVariable("leadPSV_", &leadPSV_);
    tth_ttPP_mva->AddVariable("subleadPSV_", &subleadPSV_);
    
    tth_ttPP_mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
    tth_ttPP_mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
    tth_ttPP_mva->AddVariable("met_", &met_);
    
    tth_ttPP_mva->AddVariable("top_tag_score_", &top_tag_score_);
    tth_ttPP_mva->AddVariable("dipho_pt_over_mass_", &dipho_pt_over_mass_);

    tth_ttPP_mva->AddVariable("helicity_angle_", &helicity_angle_);
    
    tth_ttPP_mva->BookMVA("BDT", "../MVAs/Hadronic_ttGG_26Feb2019__bdt.xml");
  }

  bool do_tth_dipho_mva = true;
  if (do_tth_dipho_mva) {
    tth_dipho_mva.reset(new TMVA::Reader( "!Color:Silent" ));

    tth_dipho_mva->AddVariable("maxIDMVA_", &maxIDMVA_);
    tth_dipho_mva->AddVariable("minIDMVA_", &minIDMVA_);
    tth_dipho_mva->AddVariable("max2_btag_", &max2_btag_);
    tth_dipho_mva->AddVariable("max1_btag_", &max1_btag_);
    tth_dipho_mva->AddVariable("dipho_delta_R", &dipho_delta_R);
    tth_dipho_mva->AddVariable("njets_", &njets_);
    //tth_dipho_mva->AddVariable("nbjets_", &nbjets_);
    tth_dipho_mva->AddVariable("ht_", &ht_);
    tth_dipho_mva->AddVariable("leadptoM_", &leadptoM_);
    tth_dipho_mva->AddVariable("subleadptoM_", &subleadptoM_);
    //tth_dipho_mva->AddVariable("leadIDMVA_", &leadIDMVA_);
    //tth_dipho_mva->AddVariable("subleadIDMVA_", &subleadIDMVA_);
    tth_dipho_mva->AddVariable("lead_eta_", &lead_eta_);
    tth_dipho_mva->AddVariable("sublead_eta_", &sublead_eta_);

    tth_dipho_mva->AddVariable("jet1_pt_", &jet1_pt_);
    tth_dipho_mva->AddVariable("jet1_eta_", &jet1_eta_);
    tth_dipho_mva->AddVariable("jet1_btag_", &jet1_btag_);
    tth_dipho_mva->AddVariable("jet2_pt_", &jet2_pt_);
    tth_dipho_mva->AddVariable("jet2_eta_", &jet2_eta_);
    tth_dipho_mva->AddVariable("jet2_btag_", &jet2_btag_);
    tth_dipho_mva->AddVariable("jet3_pt_", &jet3_pt_);
    tth_dipho_mva->AddVariable("jet3_eta_", &jet3_eta_);
    tth_dipho_mva->AddVariable("jet3_btag_", &jet3_btag_);
    tth_dipho_mva->AddVariable("jet4_pt_", &jet4_pt_);
    tth_dipho_mva->AddVariable("jet4_eta_", &jet4_eta_);
    tth_dipho_mva->AddVariable("jet4_btag_", &jet4_btag_);
    //tth_dipho_mva->AddVariable("jet5_pt_", &jet5_pt_);
    //tth_dipho_mva->AddVariable("jet5_eta_", &jet5_eta_);
    //tth_dipho_mva->AddVariable("jet5_btag_", &jet5_btag_);
    //tth_dipho_mva->AddVariable("jet6_pt_", &jet6_pt_);
    //tth_dipho_mva->AddVariable("jet6_eta_", &jet6_eta_);
    //tth_dipho_mva->AddVariable("jet6_btag_", &jet6_btag_);

    tth_dipho_mva->AddVariable("leadPSV_", &leadPSV_);
    tth_dipho_mva->AddVariable("subleadPSV_", &subleadPSV_);

    tth_dipho_mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
    tth_dipho_mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
    tth_dipho_mva->AddVariable("met_", &met_);

    tth_dipho_mva->AddVariable("top_tag_score_", &top_tag_score_);

    tth_dipho_mva->AddVariable("dipho_pt_over_mass_", &dipho_pt_over_mass_);
    tth_dipho_mva->AddVariable("helicity_angle_", &helicity_angle_);

    tth_dipho_mva->BookMVA("BDT", "../MVAs/Hadronic_ttHHadronicLoose_1617_dipho_26Feb2019__bdt.xml");
  }


  bool do_tth_std_mva = true;
  if (do_tth_std_mva) {
    tth_std_mva.reset(new TMVA::Reader( "!Color:Silent" ));

    tth_std_mva->AddVariable("maxIDMVA_", &maxIDMVA_);
    tth_std_mva->AddVariable("minIDMVA_", &minIDMVA_);
    tth_std_mva->AddVariable("max2_btag_", &max2_btag_);
    tth_std_mva->AddVariable("max1_btag_", &max1_btag_);
    tth_std_mva->AddVariable("dipho_delta_R", &dipho_delta_R);
    tth_std_mva->AddVariable("njets_", &njets_);
    //tth_std_mva->AddVariable("nbjets_", &nbjets_);
    tth_std_mva->AddVariable("ht_", &ht_);
    tth_std_mva->AddVariable("leadptoM_", &leadptoM_);
    tth_std_mva->AddVariable("subleadptoM_", &subleadptoM_);
    //tth_std_mva->AddVariable("leadIDMVA_", &leadIDMVA_);
    //tth_std_mva->AddVariable("subleadIDMVA_", &subleadIDMVA_);
    tth_std_mva->AddVariable("lead_eta_", &lead_eta_);
    tth_std_mva->AddVariable("sublead_eta_", &sublead_eta_);

    tth_std_mva->AddVariable("jet1_pt_", &jet1_pt_);
    tth_std_mva->AddVariable("jet1_eta_", &jet1_eta_);
    tth_std_mva->AddVariable("jet1_btag_", &jet1_btag_);
    tth_std_mva->AddVariable("jet2_pt_", &jet2_pt_);
    tth_std_mva->AddVariable("jet2_eta_", &jet2_eta_);
    tth_std_mva->AddVariable("jet2_btag_", &jet2_btag_);
    tth_std_mva->AddVariable("jet3_pt_", &jet3_pt_);
    tth_std_mva->AddVariable("jet3_eta_", &jet3_eta_);
    tth_std_mva->AddVariable("jet3_btag_", &jet3_btag_);
    tth_std_mva->AddVariable("jet4_pt_", &jet4_pt_);
    tth_std_mva->AddVariable("jet4_eta_", &jet4_eta_);
    tth_std_mva->AddVariable("jet4_btag_", &jet4_btag_);
    //tth_std_mva->AddVariable("jet5_pt_", &jet5_pt_);
    //tth_std_mva->AddVariable("jet5_eta_", &jet5_eta_);
    //tth_std_mva->AddVariable("jet5_btag_", &jet5_btag_);
    //tth_std_mva->AddVariable("jet6_pt_", &jet6_pt_);
    //tth_std_mva->AddVariable("jet6_eta_", &jet6_eta_);
    //tth_std_mva->AddVariable("jet6_btag_", &jet6_btag_);

    tth_std_mva->AddVariable("leadPSV_", &leadPSV_);
    tth_std_mva->AddVariable("subleadPSV_", &subleadPSV_);

    tth_std_mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
    tth_std_mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
    tth_std_mva->AddVariable("met_", &met_);

    tth_std_mva->AddVariable("top_tag_score_", &top_tag_score_);

    tth_std_mva->AddVariable("dipho_pt_over_mass_", &dipho_pt_over_mass_);
    tth_std_mva->AddVariable("helicity_angle_", &helicity_angle_);

    //tth_std_mva->BookMVA("BDT", "../MVAs/Hadronic_1617_ttHHadronicLoose_4Feb2019__bdt.xml");
    tth_std_mva->BookMVA("BDT", "../MVAs/Hadronic_ttHHadronicLoose_impute_21Feb2019__bdt.xml");
  }


  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TString currentFileTitle = currentFile->GetTitle();
    cout << currentFileTitle << endl;
    TFile file(currentFileTitle);
    TTree *tree = (TTree*)file.Get("tthHadronicTagDumper/trees/tth_13TeV_all");
    if (fast) TTreeCache::SetLearnEntries(10);
    if (fast) tree->SetCacheSize(128*1024*1024);
    cms3.Init(tree);

    // Initialize map of evt_run_lumi -> rand
    //RandomMap* rand_map = new RandomMap("Utils/random_map_Hadronic_" + ext + ".txt");

    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma"); 
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG") || currentFileTitle.Contains("THQ") || currentFileTitle.Contains("THW") || currentFileTitle.Contains("VBF") || currentFileTitle.Contains("GluGluHToGG") || currentFileTitle.Contains("VHToGG"); 

    TString mYear = (currentFileTitle.Contains("Run2016") || currentFileTitle.Contains("RunIISummer16")) ? "2016" : ((currentFileTitle.Contains("Run2017") || currentFileTitle.Contains("RunIIFall17")) ? "2017" : ((currentFileTitle.Contains("Run2018") || currentFileTitle.Contains("RunIIAutumn18")) ? "2018" : "no_year"));

    if (is_wrong_tt_jets_sample(currentFileTitle, "Hadronic"))                        continue;
    if (bkg_options.Contains("impute") && (currentFileTitle.Contains("GJets_HT") || currentFileTitle.Contains("QCD"))) {
      cout << "Skipping this sample: " << currentFileTitle << ", since we are imputing." << endl;
      continue;
    }

    // Dumb hacky stuff to use 2017 MC as placeholders for 2018
    if (already_looped_dipho && currentFileTitle.Contains("DiPhotonJetsBox"))
      mYear = "2018";
    if (already_looped_qcd && currentFileTitle.Contains("QCD"))
      mYear = "2018";
    if (already_looped_dy && currentFileTitle.Contains("DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX"))
      mYear = "2018";

    if (currentFileTitle.Contains("DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa_RunIIFall17MiniAODv2"))
      already_looped_dipho = true;
    if (currentFileTitle.Contains("QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM"))
      already_looped_qcd = true;
    if (currentFileTitle.Contains("DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_new_pmx_94X_mc2017_realistic_v14_ext1-v1_MINIAODSIM"))
      already_looped_dy = true;

    cout << "mYear: " << mYear << endl;
    int yearId = mYear == "2016" ? 0 : (mYear == "2017" ? 1 : (mYear == "2018" ? 2 : -1));

    // Set json file
    set_json(mYear);

    // Loop over Events in current file
    if (nEventsTotal >= nEventsChain) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();

  

    for (unsigned int event = 0; event < nEventsTree; ++event) {

      // Get Event Content
      if (nEventsTotal >= nEventsChain) continue;
      if (fast) tree->LoadTree(event);
      cms3.GetEntry(event);
      ++nEventsTotal;

      // Progress
      ttHHadronic::progress( nEventsTotal, nEventsChain );
      InitBabyNtuple();

      // Check golden json
      if (isData) {
        if (!pass_json(mYear, cms3.run(), cms3.lumi()))            continue;
      }

      // Blinded region
      if (isData && blind && mass() > 120 && mass() < 130)      continue;


      // Decide what type of sample this is
      int genPhotonId = categorize_photons(leadGenMatch(), subleadGenMatch());
      process_id_ = categorize_process(currentFileTitle, genPhotonId);

      evt_weight_ = 1.;

      // GJets Reweighting
      double gjet_mva_value = -999;
      gjet_mva_value = convert_tmva_to_prob(gjet_mva->EvaluateMVA( "BDT" ));

      // Skipping events/samples
      if (is_low_stats_process(currentFileTitle))       continue;

      if (year.Contains("RunII") && !isData) {
        double scale1fb = currentFileTitle.Contains("RunIISummer16MiniAOD") ? scale1fb_2016_RunII(currentFileTitle) : ( currentFileTitle.Contains("RunIIFall17MiniAOD") ? scale1fb_2017_RunII(currentFileTitle) : ( currentFileTitle.Contains("RunIIAutumn18MiniAOD") ? scale1fb_2018_RunII(currentFileTitle) : 0 ));
        if (mYear == "2016")
          evt_weight_ *= scale1fb * lumi_2016 * weight();
        else if (mYear == "2017")
          evt_weight_ *= scale1fb * lumi_2017 * weight();
        else if (mYear == "2018")
          evt_weight_ *= scale1fb * lumi_2018 * weight();
      }

      else if (!isData) {
        if (year == "2018") // temporary hack to use 2017 mc with 2018 data
          evt_weight_ *= scale1fb_2017(currentFileTitle) * lumi_2018 * weight();
        else if (mYear == "2016")
          evt_weight_ *= scale1fb_2016(currentFileTitle) * lumi_2016 * weight();
        else if (mYear == "2017")
          evt_weight_ *= scale1fb_2017(currentFileTitle) * lumi_2017 * weight();
        else if (mYear == "2018")
          evt_weight_ *= scale1fb_2017(currentFileTitle) * lumi_2018 * weight();
      }

      bool pu_weight = true;
      if (pu_weight) {
        evt_weight_ *= puweight();
      }

      // Impute, if applicable
      maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      if (bkg_options.Contains("impute")) {
        if (isData)
          impute_photon_id(min_photon_ID_presel_cut, maxIDMVA_, photon_fakeID_shape_runII, minIDMVA_, evt_weight_, process_id_);
      }

      double leadID_ = leadIDMVA() == maxIDMVA_ ? maxIDMVA_ : minIDMVA_;
      double subleadID_ = leadIDMVA() == maxIDMVA_ ? minIDMVA_ : maxIDMVA_;

      // Scale bkg weight
      evt_weight_ *= scale_bkg(currentFileTitle, bkg_options, process_id_, "Hadronic");

      if (has_std_overlaps(currentFileTitle, lead_Prompt(), sublead_Prompt(), genPhotonId))     continue;
      if (!passes_selection(tag, minIDMVA_, maxIDMVA_)) continue;

      if (isnan(evt_weight_) || isinf(evt_weight_) || evt_weight_ == 0) {
        continue; //some pu weights are nan/inf and this causes problems for histos 
      }

      label_ = (isData && process_id_ != 18) ? 2 : (process_id_ == 0 ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data sidebands
      multi_label_ = multiclassifier_label(currentFileTitle, genPhotonId);
      signal_mass_label_ = categorize_signal_sample(currentFileTitle);

      tth_2017_reference_mva_ = tthMVA();


      if (tag == "ttHHadronic_data_sideband_0b") {
	if (nb_medium() == 0)
	  data_sideband_label_ = 1;
	else if (nb_medium() > 0)
	  data_sideband_label_ = 0; 
      }

      else if (tag == "ttHHadronic_data_sideband_phoID" || tag == "ttHHadronic_data_sideband_phoID_v2" || tag == "ttHHadronic_data_sideband_phoID_v3") {
	if (minIDMVA_ < -0.2)
	  data_sideband_label_ = 1;
	else
	  data_sideband_label_ = 0;
      }

      // Variable definitions
      vector<TLorentzVector> jets;
      vector<double> btag_scores;
      vector<std::pair<int, double>> btag_scores_sorted;
      TLorentzVector lead_photon;
      TLorentzVector sublead_photon;
      jets = make_jets(btag_scores, year);
      btag_scores_sorted = sortVector(btag_scores);
      lead_photon = make_lead_photon();
      sublead_photon = make_sublead_photon();
      TLorentzVector diphoton = lead_photon + sublead_photon;

      top_tag_score_ = topTag_score();
      /*
      if (top_tag_score_ > -1) {
	top_tag_mass_ = log(topTag_topMass());
	top_tag_pt_ = log(topTag_topPt());
	top_tag_eta_ = topTag_topEta();
	top_tag_phi_ = topTag_topPhi();
      }
      else {
      */
	top_tag_mass_ = -1;
        top_tag_pt_ = -1;
        top_tag_eta_ = -1;
        top_tag_phi_ = -1;
      //}

      evt_run_lumi_ = to_string(cms3.event()) + "_" + to_string(cms3.run()) + "_" + to_string(cms3.lumi()); 

      max2_btag_ = btag_scores_sorted[1].second;
      max1_btag_ = btag_scores_sorted[0].second;
      dipho_delta_R = lead_photon.DeltaR(sublead_photon);
      ht_ = get_ht(jets);
      njets_ = n_jets();
      nbjets_ = nb_medium();

      jet1_pt_   = njets_ >= 1 ? jets[0].Pt()   : -999;
      jet1_eta_  = njets_ >= 1 ? jets[0].Eta()  : -999;
      jet1_btag_ = njets_ >= 1 ? btag_scores[0] : -999;
      jet2_pt_   = njets_ >= 2 ? jets[1].Pt()   : -999; 
      jet2_eta_  = njets_ >= 2 ? jets[1].Eta()  : -999;
      jet2_btag_ = njets_ >= 2 ? btag_scores[1] : -999;
      jet3_pt_   = njets_ >= 3 ? jets[2].Pt()   : -999;
      jet3_eta_  = njets_ >= 3 ? jets[2].Eta()  : -999;
      jet3_btag_ = njets_ >= 3 ? btag_scores[2] : -999;
      jet4_pt_   = njets_ >= 4 ? jets[3].Pt()   : -999;
      jet4_eta_  = njets_ >= 4 ? jets[3].Eta()  : -999;
      jet4_btag_ = njets_ >= 4 ? btag_scores[3] : -999;
      jet5_pt_   = njets_ >= 5 ? jets[4].Pt()   : -999;
      jet5_eta_  = njets_ >= 5 ? jets[4].Eta()  : -999;
      jet5_btag_ = njets_ >= 5 ? btag_scores[4] : -999;
      jet6_pt_   = njets_ >= 6 ? jets[5].Pt()   : -999;
      jet6_eta_  = njets_ >= 6 ? jets[5].Eta()  : -999;
      jet6_btag_ = njets_ >= 6 ? btag_scores[5] : -999;

      lead_pT_ = leadPt();
      sublead_pT_ = subleadPt();
      leadptoM_ = lead_ptoM();
      subleadptoM_ = sublead_ptoM();
      leadIDMVA_ = leadID_;
      subleadIDMVA_ = subleadID_; 
      lead_eta_ = leadEta();
      sublead_eta_ = subleadEta();
      lead_phi_ = leadPhi();
      sublead_phi_ = subleadPhi();

      leadPSV_ = leadPixelSeed();
      subleadPSV_ = subleadPixelSeed();

      dipho_cosphi_ = dipho_cosphi();
      dipho_rapidity_ = dipho_rapidity();
      dipho_pt_ = diphoton.Pt();
      dipho_pt_over_mass_ = diphoton.Pt() / mass();
      met_ = MetPt();
      log_met_ = log(MetPt());
      met_phi_ = MetPhi();

      helicity_angle_ = helicity(lead_photon, sublead_photon);

      rand_ = cms3.rand();
      super_rand_ = -1; //rand_map->retrieve_rand(cms3.event(), cms3.run(), cms3.lumi());
      mass_ = mass();
      lead_sigmaEtoE_ = lead_sigmaEoE();
      sublead_sigmaEtoE_ = sublead_sigmaEoE();
  
      if (do_tth_ttX_mva)
        tth_ttX_mva_ = convert_tmva_to_prob(tth_ttX_mva->EvaluateMVA( "BDT" ));
      else
	tth_ttX_mva_ = -999;
      if (do_tth_qcdX_mva)
        tth_qcdX_mva_ = convert_tmva_to_prob(tth_qcdX_mva->EvaluateMVA( "BDT" ));
      else
	tth_qcdX_mva_ = -999;
      if (do_tth_ttPP_mva)
        tth_ttPP_mva_ = convert_tmva_to_prob(tth_ttPP_mva->EvaluateMVA( "BDT" ));
      else
	tth_ttPP_mva_ = -999;
      if (do_tth_dipho_mva)
        tth_dipho_mva_ = convert_tmva_to_prob(tth_dipho_mva->EvaluateMVA( "BDT" ));
      else
        tth_dipho_mva_ = -999;
      if (do_tth_std_mva)
        tth_std_mva_ = convert_tmva_to_prob(tth_std_mva->EvaluateMVA( "BDT" ));
      else
        tth_std_mva_ = -999;


      // DNN Business
      vector<vector<float>> unordered_objects;
    
      vector<vector<float>> jet_objects = make_jet_objects(year, diphoton, false); // don't boost jet p4 to higgs ref. frame
      vector<vector<float>> jet_objects_boosted = make_jet_objects(year, diphoton, true); // yes boost

      objects_ = sort_objects(jet_objects);
      objects_boosted_ = sort_objects(jet_objects_boosted);

      FillBabyNtuple();

      /*
      if (deriving_gjet_weights) {
	if (process_id_ == 17) {
	  process_id_ == 3;
	  FillBabyNtuple();
        }
      }
      */

    }
  
    // Clean Up
    delete tree;
    file.Close();
  }
  if (nEventsChain != nEventsTotal) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }

  CloseBabyNtuple();
 
  // return
  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time: " << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time: " << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return;
}
