#include "MakeMVABabies_ttHHadronic.h"
#include "ScanChain_ttHHadronic.h"

void BabyMaker::ScanChain(TChain* chain, TString tag, TString ext, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

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

  int nLowID = 0;

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

  gjet_mva->BookMVA("BDT", "../MVAs/GJetReweight_binary_crossEntropy_bdt.xml");

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
    RandomMap* rand_map = new RandomMap("Utils/random_map_Hadronic_" + ext + ".txt");

    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG"); 
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG");
    TString year = currentFileTitle.Contains("2017") ? "2017" : "2016";

    // Loop over Events in current file
    if (nEventsTotal >= nEventsChain) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();

    if (tag == "GJet_Reweight_Preselection") {
      if (!(currentFileTitle.Contains("GJet_Pt") || currentFileTitle.Contains("GJets_HT"))) { 
	cout << "Skipping " << currentFileTitle << endl;
	continue;
      }
      else {
	cout << "Looping over " << currentFileTitle << endl;
      }
    }

    if (tag == "ttHHadronic_DNN_presel") {
      if (!(currentFileTitle.Contains("ttHJet") || currentFileTitle.Contains("TTGG") || currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma"))) {
	cout << "Skipping " << currentFileTitle << endl;
        continue;
      }
      else {
        cout << "Looping over " << currentFileTitle << endl;
      }
    }

    for (unsigned int event = 0; event < nEventsTree; ++event) {


      // Get Event Content
      if (nEventsTotal >= nEventsChain) continue;
      if (fast) tree->LoadTree(event);
      cms3.GetEntry(event);
      ++nEventsTotal;

      // Progress
      ttHHadronic::progress( nEventsTotal, nEventsChain );

      InitBabyNtuple();

      // Decide what type of sample this is
      int genPhotonId = categorize_photons(leadGenMatch(), subleadGenMatch());
      process_id_ = categorize_process(currentFileTitle, genPhotonId);
      if (is_low_stats_process(currentFileTitle))       continue;
      if (process_id_ == 17)				continue; // skip MadGraph G+Jets

      // Blinded region
      if (isData && blind && mass() > 120 && mass() < 130)	continue;

      // Selection
      if (has_ttX_overlap(currentFileTitle, lead_Prompt(), sublead_Prompt()))           continue;
      if (has_simple_qcd_overlap(currentFileTitle, genPhotonId))			continue;

      if (tag == "ttHHadronicLoose") {
        if (mass() < 100)                continue;
	if (n_jets() < 3)		continue;
	if (nb_loose() < 1)		continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHHadronic_data_sideband_0b") {
	if (mass() < 100)                continue;
	if (n_jets() < 2)               continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHHadronic_data_sideband_tt_enriched") {
	if (mass() < 100)                continue;
	if (n_jets() < 3)		 continue;
	if (nb_medium() < 1)		 continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue; 
      }

      else if (tag == "ttHHadronic_DNN_presel") {
	if (mass() < 100)                continue;
	if (n_jets() < 3)               continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHHadronic") {
	if (mass() < 100)                continue;
        if (n_jets() < 3)       continue;
        if (tthMVA() < 0.75)    continue;
        if (diphoMVARes() < 0.4)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
	if (nb_loose() < 1)	continue;
      }
      else if (tag == "GJet_Reweight_Preselection") {
	if (mass() < 100)	continue;
	if (n_jets() < 2)	continue;
      }

      else {
        cout << "Did not recognize tag name" << endl;
      }
 
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

      // Fill histograms //
      evt_weight_ = 1.;
      if (!isData) {
	if (year == "2016")
          evt_weight_ = scale1fb_2016(currentFileTitle) * lumi_2016 * weight();
        else if (year == "2017")
          evt_weight_ = scale1fb_2017(currentFileTitle) * lumi_2017 * weight();
      }

      bool pu_weight = true;
      if (pu_weight) {
        evt_weight_ *= puweight();
      }

      TString scale_qcd = "binned_NJets";
      bool do_scale_qcd = true;
      if (do_scale_qcd) {
        evt_weight_ *= qcdX_factor(currentFileTitle, scale_qcd, n_jets());
      }

      if (isnan(evt_weight_) || isinf(evt_weight_) || evt_weight_ == 0) {
        continue; //some pu weights are nan/inf and this causes problems for histos 
      }


      // Skip blinded region for MC after filling mass histogram
      bool isSignal = process_id_ == 0;

      label_ = isData ? 2 : (isSignal ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data
      multi_label_ = multiclassifier_label(currentFileTitle, genPhotonId);

      if (tag == "ttHHadronic_data_sideband_0b") {
	if (nb_tight() == 0)
	  data_sideband_label_ = 1;
	else if (nb_tight() > 0)
	  data_sideband_label_ = 0; 
      }

      else if (tag == "ttHHadronic_data_sideband_tt_enriched") {
	if (!(leadIDMVA() > -0.2 && subleadIDMVA() > -0.2))
	  data_sideband_label_ = 1;
	else
	  data_sideband_label_ = 0;
      }

      // Variable definitions
      maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      max2_btag_ = btag_scores_sorted[1].second;
      max1_btag_ = btag_scores_sorted[0].second;
      dipho_delta_R = lead_photon.DeltaR(sublead_photon);
      ht_ = get_ht(jets);
      njets_ = n_jets();
      //nbjets_ = nb_medium();
      jet1_pt_  = jet1_pt() > 0 ? jet1_pt() : -999;
      jet1_eta_ =  jet1_pt() > 0 ? jet1_eta() : -999;
      jet1_btag_ =  jet1_pt() > 0 ? jet1_bdiscriminant() : -999;
      jet2_pt_  = jet2_pt() > 0 ? jet2_pt() : -999;
      jet2_eta_ =  jet2_pt() > 0 ? jet2_eta() : -999;
      jet2_btag_ =  jet2_pt() > 0 ? jet2_bdiscriminant() : -999;
      jet3_pt_  = jet3_pt() > 0 ? jet3_pt() : -999;
      jet3_eta_ =  jet3_pt() > 0 ? jet3_eta() : -999;
      jet3_btag_ =  jet3_pt() > 0 ? jet3_bdiscriminant() : -999;
      jet4_pt_  = jet4_pt() > 0 ? jet4_pt() : -999;
      jet4_eta_ =  jet4_pt() > 0 ? jet4_eta() : -999;
      jet4_btag_ =  jet4_pt() > 0 ? jet4_bdiscriminant() : -999;     
      jet5_pt_  = jet5_pt() > 0 ? jet5_pt() : -999;
      jet5_eta_ =  jet5_pt() > 0 ? jet5_eta() : -999;
      jet5_btag_ =  jet5_pt() > 0 ? jet5_bdiscriminant() : -999;
      jet6_pt_  = jet6_pt() > 0 ? jet6_pt() : -999;
      jet6_eta_ =  jet6_pt() > 0 ? jet6_eta() : -999;
      jet6_btag_ =  jet6_pt() > 0 ? jet6_bdiscriminant() : -999;

      lead_pT_ = leadPt();
      sublead_pT_ = subleadPt();
      leadptoM_ = lead_ptoM();
      subleadptoM_ = sublead_ptoM();
      leadIDMVA_ = leadIDMVA(); 
      subleadIDMVA_ = subleadIDMVA();
      lead_eta_ = leadEta();
      sublead_eta_ = subleadEta();

      leadPSV_ = leadPixelSeed();
      subleadPSV_ = subleadPixelSeed();

      dipho_cosphi_ = dipho_cosphi();
      dipho_rapidity_ = dipho_rapidity();
      dipho_pt_ = diphoton.Pt();
      met_ = MetPt();

      rand_ = cms3.rand();
      super_rand_ = rand_map->retrieve_rand(cms3.event(), cms3.run(), cms3.lumi());
      mass_ = mass();
      lead_sigmaEtoE_ = lead_sigmaEoE();
      sublead_sigmaEtoE_ = sublead_sigmaEoE();
  
      // Gamma + jets reweighting
      double gjet_mva_value = -999;
      gjet_mva_value = convert_tmva_to_prob(gjet_mva->EvaluateMVA( "BDT" ));
      if (process_id_ == 3) {
	double prob = gjet_mva_value;
	double prob_ratio = prob / ( 1 - prob);
	evt_weight_ *= prob_ratio;
	evt_weight_ *= gjet_normalization;
      } 

      // DNN Business
      vector<vector<float>> unordered_objects;
    
      vector<vector<float>> jet_objects = make_jet_objects(year); 
      for (int i = 0; i < jet_objects.size(); i++)
        unordered_objects.push_back(jet_objects[i]);


      TLorentzVector lead_photon_modified, sublead_photon_modified;
      lead_photon_modified.SetPtEtaPhiE(lead_ptoM(), leadEta(), leadPhi(), -999);
      sublead_photon_modified.SetPtEtaPhiE(sublead_ptoM(), subleadEta(), subleadPhi(), -999);
      unordered_objects.push_back(make_object(lead_photon_modified,    {-999, -999, -999, -999},    leadIDMVA(), -999));
      unordered_objects.push_back(make_object(sublead_photon_modified, {-999, -999, -999, -999}, subleadIDMVA(), -999));

      TLorentzVector Met;
      Met.SetPtEtaPhiE(MetPt(), 0.0, MetPhi(), MetPt());
      unordered_objects.push_back(make_object(Met,	      	       {-999, -999, -999, -999},           -999,  1.0));

      objects_ = sort_objects(unordered_objects);

      FillBabyNtuple();

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
