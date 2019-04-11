#include "MakeMVABabies_ttHLeptonic.h"
#include "ScanChain_ttHLeptonic.h"

void BabyMaker::ScanChain(TChain* chain, TString tag, TString year, TString ext, TString bkg_options, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Make baby ntuple
  MakeBabyNtuple( Form("%s.root", ("MVABaby_ttHLeptonic_" + ext).Data()));

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if (nEvents >= 0) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  int nLowID = 0;

  TF1* photon_fakeID_shape = get_photon_ID_shape("fake");
  TF1* photon_fakeID_shape_runII = get_photon_ID_shape("fake_runII");

  // ttH MVA Business
  unique_ptr<TMVA::Reader> tth_qcdX_mva;
  unique_ptr<TMVA::Reader> tth_ttX_mva;
  unique_ptr<TMVA::Reader> tth_ttPP_mva;
  unique_ptr<TMVA::Reader> tth_dipho_mva;
  unique_ptr<TMVA::Reader> tth_std_mva;

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

    tth_ttPP_mva->AddVariable("lep_pt_", &lep_pt_);
    tth_ttPP_mva->AddVariable("lep_eta_", &lep_eta_);

    tth_ttPP_mva->AddVariable("n_lep_loose_", &n_lep_loose_);
    tth_ttPP_mva->AddVariable("n_lep_medium_", &n_lep_medium_);
    tth_ttPP_mva->AddVariable("n_lep_tight_", &n_lep_tight_);

    tth_ttPP_mva->AddVariable("muon1_mini_iso_", &muon1_mini_iso_);
    tth_ttPP_mva->AddVariable("muon2_mini_iso_", &muon2_mini_iso_);

    tth_ttPP_mva->BookMVA("BDT", "../MVAs/Leptonic_5Apr2019_ttPP__bdt.xml");
  }



  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TString currentFileTitle = currentFile->GetTitle();
    cout << currentFileTitle << endl;
    TFile file(currentFileTitle);
    TTree *tree = (TTree*)file.Get("tthLeptonicTagDumper/trees/tth_13TeV_all");
    if (fast) TTreeCache::SetLearnEntries(10);
    if (fast) tree->SetCacheSize(128*1024*1024);
    cms3.Init(tree);

    // Skip Pythia GJets
    if (currentFileTitle.Contains("GJet-Pt")) {
      cout << "Skipping Pythia GJets sample: " << currentFileTitle << endl;
    }

    // Initialize map of evt_run_lumi -> rand
    //RandomMap* rand_map = new RandomMap("Utils/random_map_Leptonic_" + ext + ".txt");

    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma"); 
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG") || currentFileTitle.Contains("THQ") || currentFileTitle.Contains("THW") || currentFileTitle.Contains("VBF") || currentFileTitle.Contains("GluGluHToGG") || currentFileTitle.Contains("VHToGG"); 

    TString mYear = currentFileTitle.Contains("2016") ? "2016" : (currentFileTitle.Contains("2017") ? "2017" : (currentFileTitle.Contains("2018") ? "2018" : "2018"));

    if (tag == "ttHLeptonic_ttPP") {
      if (!isSignal && !isData) {
	if (!(currentFileTitle.Contains("TTGG"))) {
	  cout << "Skipping " << currentFileTitle << endl;
	  continue;
	}
      }
    }
    if (tag == "ttHLeptonic_dipho") {
      if (!isSignal && !isData) {
        if (!(currentFileTitle.Contains("DiPhotonJets"))) {
	  cout << "Skipping " << currentFileTitle << endl;
          continue;
	}
      }
    }

    if (is_wrong_tt_jets_sample(currentFileTitle, "Leptonic"))                        continue;

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
      ttHLeptonic::progress( nEventsTotal, nEventsChain );

      InitBabyNtuple();

      // Check golden json
      if (isData) {
        if (!pass_json(mYear, cms3.run(), cms3.lumi()))         continue;
      }

      // Blinded region
      if (!isSignal && blind && mass() > 120 && mass() < 130)	continue;

      evt_weight_ = 1.;

      maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();

      n_lep_loose_ = nElecLoose() + nMuonLoose();
      n_lep_medium_ = nElecMedium() + nMuonMedium();
      n_lep_tight_ = nElecTight() + nMuonTight();

      // Decide what type of sample this is
      int genPhotonId = categorize_photons(leadGenMatch(), subleadGenMatch());
      process_id_ = categorize_process(currentFileTitle, genPhotonId);
      if (process_id_ == 17)
        process_id_ = 3; // use Madgraph GJets instead of Pythia

      // Impute, if applicable
      if (bkg_options.Contains("impute")) {
        if (isData)
          impute_photon_and_lepton_id(min_photon_ID_presel_cut, maxIDMVA_, photon_fakeID_shape_runII, minIDMVA_, n_lep_medium_, n_lep_tight_, evt_weight_, process_id_);
      } 

      // Scale bkg weight
      evt_weight_ *= scale_bkg(currentFileTitle, bkg_options, process_id_, "Leptonic");

      // Skipping events/samples
      if (is_low_stats_process(currentFileTitle))       continue;
      if (has_std_overlaps(currentFileTitle, lead_Prompt(), sublead_Prompt(), genPhotonId))     continue;

      if (!passes_selection(tag, minIDMVA_, maxIDMVA_, n_lep_medium_, n_lep_tight_)) continue;

      /*
      // Selection
      if (tag == "ttHLeptonicLoose") {
        if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        //if (nb_loose() < 1)             continue;
	if (nElecMedium() + nMuonMedium() < 1)	continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadIDMVA() < -0.9 || subleadIDMVA() < -0.9) continue;
	if (n_lep_medium_ < 1)	continue;
	if (minIDMVA_ < -0.7)	continue;
      }

      else if (tag == "ttHLeptonicLoose_impute2") {
	if (process_id_ == 3 || process_id_ == 4 || process_id_ == 17) continue; // skip gjets and QCD (QCD is skipped by default anyway)
	if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (leadIDMVA() < -0.9 || subleadIDMVA() < -0.9) continue;
	if (maxIDMVA_ < -0.7)	continue;
	if (isData) {
	  if (minIDMVA_ < -0.7 && n_lep_medium_ < 1) {
	    minIDMVA_ = impute_from_fakePDF(-0.7, maxIDMVA_, cms3.event(), photon_fakeID_shape, evt_weight_);
	    n_lep_tight_  = impute_leps_from_fakePDF();
	    process_id_ = 18;
	    evt_weight_ *= 1.8657; // from fits of gg+jets, imputed QCD/gjets sample AFTER fits of ttX in tt region
	  }
	  else {
	    if (minIDMVA_ < -0.7)         continue;
            if (n_lep_medium_ < 1)        continue;
	  }
	}
	else {
	  if (minIDMVA_ < -0.7)		continue;
	  if (n_lep_medium_ < 1)	continue;
	
	  if (process_id_ == 2) {
	    evt_weight_ *= 1.223; // from fits of gg+jets, imputed QCD/gjets sample AFTER fits of ttX in tt region
	  }

	  // Scale ttX samples
	  bool scale_ttX = true;
	  std::map<int, double> ttX_scale = {
	    {5, 0.7107},
	    {6, 1.7715},
	    {9, 2.0501}
	  };

	  if (scale_ttX && (process_id_ == 5 || process_id_ == 6 || process_id_ == 9)) { 
	    evt_weight_ *= ttX_scale[process_id_];
	  }
	}
      }

      else if (tag == "ttHLeptonicLoose_scaleMC") {
	if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (leadIDMVA() < -0.9 || subleadIDMVA() < -0.9) continue;
	if (minIDMVA_ < -0.7)	continue;
	if (n_lep_medium_ < 1)	continue;
	bool scale_nleps = true;
	std::map<int, double> lep_scale = {
	  {0, 2.3525},
	  {1, 1.25451},
	  {2, 0.791874}
	};
	if (scale_nleps && !isData && !isSignal)
	  evt_weight_ *= lep_scale[n_lep_tight_];
      } 

      else if (tag == "ttHLeptonic_ttPP" || tag == "ttHLeptonic_dipho") {
	if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        if (nElecMedium() + nMuonMedium() < 1)  continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadIDMVA() < -0.9 || subleadIDMVA() < -0.9) continue;
      }

      else if (tag == "ttHLeptonic_data_sideband_0b") {
	if (mass() < 100)                continue;
	if (n_jets() < 2)               continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHLeptonic_data_sideband_phoID") {
	if (mass() < 100)                continue;
	if (n_jets() < 2)                continue;
	if (nb_loose() < 1)             continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHLeptonic_2017_MVA_presel") {
        if (mass() < 100)               continue;
        if (n_jets() < 1)               continue;
        if (nb_medium() < 1)            continue;
        if (leadIDMVA() < -0.2)         continue;
        if (subleadIDMVA() < -0.2)      continue;
      }

      else if (tag == "ttHLeptonicMedium") {
        if (mass() < 100)                               continue;
        if (n_jets() < 2)                               continue;
        if (nb_medium() < 1)                            continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (leadPixelSeed() || subleadPixelSeed())      continue;
        double lep_pt, lep_eta;
        if (get_lep_pt(lep_eta) < 15)                   continue;
      }
      else if (tag == "ttHLeptonicTight") {
        if (mass() < 100)                               continue;
        if (n_jets() < 2)                               continue;
        if (nb_tight() < 1)                             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (leadPixelSeed() || subleadPixelSeed())      continue;
        double lep_pt, lep_eta;
        if (get_lep_pt(lep_eta) < 25)                   continue;
        if (MetPt() < 50)                             continue;
      }
      else if (tag == "ttHLeptonicLoose_tightPhoIDMVA") {
        if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (leadIDMVA() < -0.2)         continue;
        if (subleadIDMVA() < -0.2)         continue;
      }

      else if (tag == "ttHLeptonicPresel_v2") {
        if (mass() < 100)                               continue;
        if (n_jets() < 2)                               continue;
        if (nb_medium() < 1)                            continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (leadPixelSeed() || subleadPixelSeed())      continue;
        double lep_pt, lep_eta;
        if (get_lep_pt(lep_eta) < 20)                   continue;
        //if (MetPt() < 50)                             continue;
      }

      else {
        cout << "Did not recognize tag name" << endl;
      }
      */ 



      // Make p4 for physics objects
      vector<TLorentzVector> jets;
      vector<double> btag_scores;
      vector<std::pair<int, double>> btag_scores_sorted;
      TLorentzVector lead_photon;
      TLorentzVector sublead_photon;
      vector<TLorentzVector> electrons;
      vector<TLorentzVector> muons;
      vector<TLorentzVector> leps;
      jets = make_jets(btag_scores, year);
      btag_scores_sorted = sortVector(btag_scores);
      lead_photon = make_lead_photon();
      sublead_photon = make_sublead_photon();
      electrons = make_els();
      muons = make_mus();
      leps = make_leps(electrons, muons);
      TLorentzVector diphoton = lead_photon + sublead_photon;
      vector<TLorentzVector> objects;
      for (int i = 0; i < jets.size(); i++)
	objects.push_back(jets[i]);
      for (int i = 0; i < leps.size(); i++)
        objects.push_back(leps[i]);


      // Fill histograms //
      if (!no_weights && !isData) {
	if (year == "2016")
          evt_weight_ *= scale1fb_2016(currentFileTitle) * lumi_2016 * weight();
        else if (year == "2017")
          evt_weight_ *= scale1fb_2017(currentFileTitle) * lumi_2017 * weight();
      }

      bool pu_weight = true;
      if (pu_weight) {
        evt_weight_ *= puweight();
      }

      if (isnan(evt_weight_) || isinf(evt_weight_) || evt_weight_ == 0) {
        continue; //some pu weights are nan/inf and this causes problems for histos 
      }


      label_ = (isData && process_id_ != 18) ? 2 : (process_id_ == 0 ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data sidebands
      multi_label_ = multiclassifier_label(currentFileTitle, genPhotonId);
      signal_mass_label_ = categorize_signal_sample(currentFileTitle);

      tth_2017_reference_mva_ = year == "2017" ? tthMVA() : -999;

      if (tag == "ttHLeptonic_data_sideband_0b") {
	if (nb_medium() == 0)
	  data_sideband_label_ = 1;
	else
	  data_sideband_label_ = 0;
      }
      else if (tag == "ttHLeptonic_data_sideband_phoID") {
	if (minIDMVA_ < -0.2)
          data_sideband_label_ = 1;
        else
          data_sideband_label_ = 0;
      }


      // Variable definitions
      lep_pt_ = leps[0].Pt();
      lep_eta_ = leps[0].Eta();

      //n_lep_loose_ = nElecLoose() + nMuonLoose();
      //n_lep_medium_ = nElecMedium() + nMuonMedium();
      //n_lep_tight_ = nElecTight() + nMuonTight();

      muon1_mini_iso_ = muon1_pt() > 0 ? muonLeadIso() / muon1_pt() : -999;
      muon2_mini_iso_ = muon2_pt() > 0 ? muonSubleadIso() / muon2_pt() : -999;
     
      top_tag_score_ = topTag_score();

      //maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      //minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
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
      leadIDMVA_ = leadIDMVA();
      subleadIDMVA_ = subleadIDMVA();
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
      super_rand_ = cms3.rand(); //rand_map->retrieve_rand(cms3.event(), cms3.run(), cms3.lumi());
      mass_ = mass();

      lead_sigmaEtoE_ = lead_sigmaEoE();
      sublead_sigmaEtoE_ = sublead_sigmaEoE(); 

      if (do_tth_ttPP_mva)
	tth_ttPP_mva_ = convert_tmva_to_prob(tth_ttPP_mva->EvaluateMVA( "BDT" ));

      vector<vector<float>> jet_objects = make_jet_objects(year, diphoton, false);
      vector<vector<float>> lep_objects = make_lep_objects(electrons, muons, diphoton, false);
      vector<vector<float>> all_objects = make_all_objects(lep_objects, jet_objects);

      jets_ = sort_objects(jet_objects);
      leptons_ = sort_objects(lep_objects);
      objects_ = sort_objects(all_objects);

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
