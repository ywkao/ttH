#include "ScanChain_ttHLeptonic.h"

int ScanChain(TChain* chain, TString tag, TString year, TString ext, TString xml_file, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag + "_" + ext + "_histograms" + year + ".root", "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  bool evaluate_mva = xml_file != "none";

  // Make MVA Optimization Baby
  OptimizationBabyMaker* baby = new OptimizationBabyMaker();
  TString xml_file_noExt = xml_file;
  xml_file_noExt.ReplaceAll(".xml", "");
  TString optimization_baby_name = "Optimization/MVAOptimizationBaby_" + ext + "_" + xml_file_noExt + "_" + tag;
  baby->MakeBabyNtuple( Form("%s.root", optimization_baby_name.Data()));

  // Create "process" objects
  vector<Process*> vProcess = generate_processes(f1);
  add_variables(vProcess, tag); // defined in ttHLooper 

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if (nEvents >= 0) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // Initialize map of evt_run_lumi -> rand
  RandomMap* rand_map = new RandomMap("Utils/random_map_Leptonic_" + ext + ".txt");

  // MVA Business
  unique_ptr<TMVA::Reader> mva;

  // Declare BDT vars
  float lep_pt_;
  float lep_eta_;

  float top_tag_score_;

  float maxIDMVA_;
  float minIDMVA_;
  float max2_btag_;
  float max1_btag_;
  float dipho_delta_R;
  float njets_;
  float nbjets_;
  float ht_;
  float lead_pT_;
  float sublead_pT_;
  float leadptoM_;
  float subleadptoM_;
  float leadIDMVA_;
  float subleadIDMVA_;
  float lead_eta_;
  float sublead_eta_;

  float jet1_pt_;
  float jet1_eta_;
  float jet1_btag_;
  float jet2_pt_;
  float jet2_eta_;
  float jet2_btag_;
  float jet3_pt_;
  float jet3_eta_;
  float jet3_btag_;
  float jet4_pt_;
  float jet4_eta_;
  float jet4_btag_;
  float jet5_pt_;
  float jet5_eta_;
  float jet5_btag_;
  float jet6_pt_;
  float jet6_eta_;
  float jet6_btag_;

  float leadPSV_;
  float subleadPSV_;

  float dipho_cosphi_;
  float dipho_rapidity_;
  float dipho_pt_;
  float met_;

  


  if (evaluate_mva) {
    mva.reset(new TMVA::Reader( "!Color:Silent" ));
    mva->AddVariable("maxIDMVA_", &maxIDMVA_);
    mva->AddVariable("minIDMVA_", &minIDMVA_);
    mva->AddVariable("max2_btag_", &max2_btag_);
    mva->AddVariable("max1_btag_", &max1_btag_);
    mva->AddVariable("dipho_delta_R", &dipho_delta_R);
    mva->AddVariable("njets_", &njets_);
    //mva->AddVariable("nbjets_", &nbjets_);
    mva->AddVariable("ht_", &ht_);
    mva->AddVariable("leadptoM_", &leadptoM_);
    mva->AddVariable("subleadptoM_", &subleadptoM_);
    mva->AddVariable("leadIDMVA_", &leadIDMVA_);
    mva->AddVariable("subleadIDMVA_", &subleadIDMVA_); 
    mva->AddVariable("lead_eta_", &lead_eta_);
    mva->AddVariable("sublead_eta_", &sublead_eta_);

    mva->AddVariable("jet1_pt_", &jet1_pt_);
    mva->AddVariable("jet1_eta_", &jet1_eta_);
    mva->AddVariable("jet1_btag_", &jet1_btag_);
    mva->AddVariable("jet2_pt_", &jet2_pt_);
    mva->AddVariable("jet2_eta_", &jet2_eta_);
    mva->AddVariable("jet2_btag_", &jet2_btag_);
    mva->AddVariable("jet3_pt_", &jet3_pt_);
    mva->AddVariable("jet3_eta_", &jet3_eta_);
    mva->AddVariable("jet3_btag_", &jet3_btag_);
    mva->AddVariable("jet4_pt_", &jet4_pt_);
    mva->AddVariable("jet4_eta_", &jet4_eta_);
    mva->AddVariable("jet4_btag_", &jet4_btag_);
    //mva->AddVariable("jet5_pt_", &jet5_pt_);
    //mva->AddVariable("jet5_eta_", &jet5_eta_);
    //mva->AddVariable("jet5_btag_", &jet5_btag_);
    //mva->AddVariable("jet6_pt_", &jet6_pt_);
    //mva->AddVariable("jet6_eta_", &jet6_eta_);
    //mva->AddVariable("jet6_btag_", &jet6_btag_);

    mva->AddVariable("leadPSV_", &leadPSV_);
    mva->AddVariable("subleadPSV_", &subleadPSV_);

    mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
    mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
    mva->AddVariable("met_", &met_);

    mva->AddVariable("top_tag_score_", &top_tag_score_);
    
    mva->AddVariable("lep_pt_", &lep_pt_);
    mva->AddVariable("lep_eta_", &lep_eta_);

    mva->BookMVA("BDT", "../MVAs/" + xml_file);
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

    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma");
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG") || currentFileTitle.Contains("THQ") || currentFileTitle.Contains("THW") || currentFileTitle.Contains("VBF") || currentFileTitle.Contains("GluGluHToGG") || currentFileTitle.Contains("VHToGG");
    TString mYear = currentFileTitle.Contains("2016") ? "2016" : (currentFileTitle.Contains("2017") ? "2017" : (currentFileTitle.Contains("2018") ? "2018" : "2018")); 

    if (isSignal) {
      if (!currentFileTitle.Contains("M125"))	continue;
    }

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

      if (tag.Contains("ttHLeptonic_blinded_region")) {
        blind = false;
        if (!tag.Contains("Data")) {
	  if (isData) continue;
        }
      }

      // Check golden json
      if (isData) {
        if (!pass_json(mYear, cms3.run(), cms3.lumi()))		continue;
      }

      // Blinded region 
      if (isData && blind && mass() > 120 && mass() < 130)        continue;

      // Fill mva baby before any selections
      int genPhotonId = isData ? -1 : categorize_photons(leadGenMatch(), subleadGenMatch());
      int processId = categorize_process(currentFileTitle, genPhotonId);
      if (processId == 17)
	processId = 3; // use Madgraph GJets instead of Pythia
      int genLeptonId = isData ? -1 : categorize_leptons(nGoodEls(), nGoodMus());
      int genPhotonDetailId = isData ? -1 : categorize_photons_detail(lead_photon_type(), sublead_photon_type());
      int photonLocationId = categorize_photon_locations(leadEta(), subleadEta());

      double evt_weight = 1.;
     
	/* 
      if (!no_weights && !isData) {
	if (year == "2018") // temporary hack to use 2017 mc with 2018 data
          evt_weight = scale1fb_2017(currentFileTitle) * lumi_2018 * sgn(weight());
        else if (mYear == "2016")
          evt_weight = scale1fb_2016(currentFileTitle) * lumi_2016 * sgn(weight());
        else if (mYear == "2017") 
          evt_weight = scale1fb_2017(currentFileTitle) * lumi_2017 * sgn(weight());
	else if (mYear == "2018")
          evt_weight = scale1fb_2017(currentFileTitle) * lumi_2018 * sgn(weight());
      } 
	*/
	
      if (!isData) {
        if (year == "2018") // temporary hack to use 2017 mc with 2018 data
          evt_weight = scale1fb_2017(currentFileTitle) * lumi_2018 * weight();
        else if (mYear == "2016")
          evt_weight = scale1fb_2016(currentFileTitle) * lumi_2016 * weight();
        else if (mYear == "2017") 
          evt_weight = scale1fb_2017(currentFileTitle) * lumi_2017 * weight();
        else if (mYear == "2018")
          evt_weight = scale1fb_2017(currentFileTitle) * lumi_2018 * weight();
      }

      bool pu_weight = true;
      if (pu_weight) {
	evt_weight *= puweight();
      }
      if (isnan(evt_weight) || isinf(evt_weight) || evt_weight == 0) {
	continue; //some pu weights are nan/inf and this causes problems for histos 
      }

      int label = isData ? 2 : (isSignal ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data

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

      // Selection
     
      // Skipping events/samples
      //if (is_low_stats_process(currentFileTitle))       continue;
      //if (tag != "GJet_Reweight_Preselection") {
      //  if (process_id_ == 17)                          continue; // skip MadGraph G+Jets
      //}
      if (is_wrong_tt_jets_sample(currentFileTitle, "Hadronic"))                        continue;
      if (has_ttX_overlap(currentFileTitle, lead_Prompt(), sublead_Prompt()))           continue;
      if (has_simple_qcd_overlap(currentFileTitle, genPhotonId))                        continue;
 
      double mva_value = -999;
      if (evaluate_mva) {

        // Calculate MVA value
        lep_pt_ = leps[0].Pt();
        lep_eta_ = leps[0].Eta();

        top_tag_score_ = topTag_score();

        maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
        minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
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

        leadPSV_ = leadPixelSeed();
        subleadPSV_ = subleadPixelSeed();

        dipho_cosphi_ = dipho_cosphi();
        dipho_rapidity_ = dipho_rapidity();
        dipho_pt_ = diphoton.Pt();
        met_ = MetPt();

        mva_value = convert_tmva_to_prob(mva->EvaluateMVA( "BDT" ));
      }


      if (tag == "ttHLeptonicLoose") {
        if (mass() < 100)        continue;
	if (n_jets() < 2)	continue;
	if (nb_loose() < 1)		continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHLeptonic_baseline_maxZA") {
	if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (mva_value < 0.86978543)     continue; // about 5 ttH events in mass window 
      }

      else if (tag == "ttHLeptonic_data_sideband_0b_train") {
	if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_medium() != 0)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHLeptonic_data_sideband_0b_test") {
        if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_medium() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHLeptonic_data_sideband_phoID_train") {
        if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (minIDMVA_ >= -0.2)		continue;
      }
      else if (tag == "ttHLeptonic_data_sideband_phoID_test") {
        if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (minIDMVA_ < -0.2)		continue;
      }


      else if (tag == "ttHLeptonic_ttbarCR") {
	if (mass() < 100)        continue;
	if (n_jets() < 2)       continue;
	if (nb_tight() < 1)             continue;
	if (leadIDMVA() < -0.2)         continue;
        if (subleadIDMVA() < -0.2)      continue;
	if (leps[0].Pt() < 20)		continue;
	if (MetPt() < 40)		continue;
	if (leadPixelSeed() || subleadPixelSeed())      continue;
      }

      else if (tag == "ttHLeptonic_2017_SR_like") {
	if (mass() < 100)		continue;
        //if (!(n_ele() + n_muons() == 1))        continue;
        if (isData && blind && mass() > 115 && mass() < 135)      continue;
        if (n_jets() < 1)                continue;
        if (nb_medium() < 1)            continue;
        if (leadIDMVA() < -0.2)         continue;
        if (subleadIDMVA() < -0.2)      continue;
        if (tthMVA() < mva_thresh_2017[0])      continue;
      }

      else if (tag == "ttHLeptonicLoose_2018studies") {
	if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)      continue;
      }
      else if (tag == "ttHLeptonic_2017_MVA_presel") {
	if (mass() < 100)               continue;
	if (n_jets() < 1)               continue;
	if (nb_medium() < 1)            continue;
	if (leadIDMVA() < -0.2)         continue;
	if (subleadIDMVA() < -0.2)      continue;
      }
      else if (tag == "ttHLeptonicMedium") {
	if (mass() < 100)       			continue;
	if (n_jets() < 2)       			continue;
	if (nb_medium() < 1)				continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadPixelSeed() || subleadPixelSeed())	continue;
	double lep_pt, lep_eta;
	if (get_lep_pt(lep_eta) < 15)			continue;
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

      else if (tag == "ttHLeptonicMedTight") {
	if (mass() < 100)                               continue;
        if (n_jets() < 2)                               continue;
	if (nb_medium() < 1)                            continue;
	if (nb_loose() < 2)				continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadPixelSeed() || subleadPixelSeed())      continue;
	double lep_pt, lep_eta;
	if (get_lep_pt(lep_eta) < 20)                   continue;
      }

      else if (tag == "ttHLeptonicTT") {
	if (mass() < 100)                               continue;
	if (n_jets() < 2)                               continue;
	if (nb_tight() < 1)                             continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadPixelSeed() || subleadPixelSeed())      continue;
        double lep_pt, lep_eta;
        if (get_lep_pt(lep_eta) < 20)                   continue;
        if (MetPt() < 40)                             continue;
	if (leadIDMVA() < -0.2)         continue;
        if (subleadIDMVA() < -0.2)         continue;	
      }

      else if (tag == "ttHLeptonicCustom") {
        if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	//if (mva_value < -0.9)	continue;
      }

      else if (tag == "ttHLeptonic") {
	if (mass() < 100)        continue;
        if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;  
	if (n_jets() < 2)		continue;
	if (nb_medium() < 1)            continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leps[0].Pt() < 20)          continue;
	if (lead_ptoM() < 0.5)		continue;
      }
      else if (tag == "ttHLeptonic_v2") {
	if (mass() < 80)        continue;
        if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
 	if (leadPixelSeed())    continue;
        if (subleadPixelSeed()) continue;	
	if (nb_medium() < 1)            continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHLeptonic_pixel_only") {
	if (mass() < 80)        continue;
        if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
        if (leadPixelSeed())    continue;
        if (subleadPixelSeed()) continue;
        if (nb_medium() < 1)            continue;
      }
      else if (tag == "ttHLeptonic_no_veto") {
        if (mass() < 80)        continue;  
        if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
        if (nb_medium() < 1)            continue;
      }

      else if (tag == "ttHLeptonic_veto_study") {
	if (mass() < 80)        continue;
	if (nb_medium() < 1)            continue;
	if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
      }

      else if (tag == "ttbar_cr") {
	if (n_jets() < 3) 	continue;
        if (n_bjets() < 2)	continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttbar_cr_v2") {
        if (n_jets() < 3)       continue;
        if (n_bjets() < 2)    continue;
	if (leadPixelSeed())	continue;
        if (subleadPixelSeed()) continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttbar_cr_v3") {
        if (n_jets() < 2)       continue;
	if (n_bjets() < 2) 	continue;
        if (leadPixelSeed())    continue;
        if (subleadPixelSeed()) continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHLeptonic_blinded_region") { 
	if (mass() < 120 || mass() > 130)	continue;
	if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
	if (nb_medium() < 1)            continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHLeptonic_blinded_region_wData") {
	if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
	if (nb_medium() < 1)            continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHLeptonic_blinded_region_v2") {
        if (mass() < 120 || mass() > 130)       continue;
        if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
	if (leadPixelSeed())    continue;
        if (subleadPixelSeed()) continue;
	if (nb_medium() < 1)            continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "quality_photons") {
	if (leadPixelSeed())    continue;
        if (subleadPixelSeed()) continue;
	if (leadIDMVA() < 0.5)         continue;
        if (subleadIDMVA() < 0.5)         continue;
      } 

      else if (tag == "ttHLeptonic_2017_SR1") {
	if (!(n_ele() + n_muons() == 1)) continue;
	if (isData && blind && mass() > 115 && mass() < 135)      continue;
	if (n_jets() < 1)		continue;
	if (nb_medium() < 1)		continue;
	if (leadIDMVA() < -0.2)		continue;
	if (subleadIDMVA() < -0.2)	continue;
	if (!(tthMVA() >= mva_thresh_2017[0] && tthMVA() < mva_thresh_2017[1])) continue;
      }

      else if (tag == "ttHLeptonic_2017_SR2") {
	if (!(n_ele() + n_muons() == 1))	continue;
	if (isData && blind && mass() > 115 && mass() < 135)      continue;
        if (n_jets() < 1)                continue;
        if (nb_medium() < 1)            continue;
        if (leadIDMVA() < -0.2)         continue;
        if (subleadIDMVA() < -0.2)      continue;
        if (tthMVA() < mva_thresh_2017[1])      continue;
      }

      else if (tag == "ttHLeptonic_2017_dilep") {
	if (!(n_ele() + n_muons() >= 2)) continue;
	if (isData && blind && mass() > 115 && mass() < 135)      continue;
	if (n_jets() < 1)                continue;
        if (nb_medium() < 1)            continue;
        if (leadIDMVA() < -0.2)         continue;
        if (subleadIDMVA() < -0.2)      continue;
	if (tthMVA() < 0.6)		continue;
      }

      else {
        cout << "Did not recognize tag name" << endl;
      } 

      //if (is_low_stats_process(currentFileTitle))       continue;

      // Evaluate MVA, if we choose
      if (evaluate_mva) {

        double reference_mva = mYear == "2017" ? tthMVA() : -1;
        bool pass_ref_presel = mYear == "2017" ? pass_2017_mva_presel() : true;
	//double rand = use_random_test_train_split ? rand_map->retrieve_rand(cms3.event(), cms3.run(), cms3.lumi()) : cms3.rand();
	double super_rand = rand_map->retrieve_rand(cms3.event(), cms3.run(), cms3.lumi());
        if (!is_low_stats_process(currentFileTitle)) 	
          baby->FillBabyNtuple(label, evt_weight, processId, cms3.rand(), mass(), mva_value, reference_mva, pass_ref_presel, super_rand);
      }


      int mvaCategoryId = mva_value < -0.8 ? 0 : 1;
      vector<int> vId = {genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId, mvaCategoryId}; 

      bool make_text_file = false;
      bool make_2prompts = true;
      if (make_text_file && mYear == "2016") {
	if (currentFileTitle.Contains("TTJets") || currentFileTitle.Contains("TTGJets")) {
	  if (make_2prompts && genPhotonId == 2) {
	    TString name = currentFileTitle.Contains("TTJets") ? "TTJets" : "TTGammaJets";
	    cout << name << " " << cms3.event() << " " << leadPtGen() << endl;
	    cout << name << " " << cms3.event() << " " << subleadPtGen() << endl;
	  }
	  if (!make_2prompts && genPhotonId == 1) {
	    TString name = currentFileTitle.Contains("TTJets") ? "TTJets" : "TTGammaJets";
	    double ptGen = int(leadGenMatch()) == 1 ? leadPtGen() : subleadPtGen();
	    cout << name << " " << cms3.event() << " " << ptGen << endl;
	  }
	}
        continue;
      }

      //////////////////////////////
      // Start filling histograms //
      //////////////////////////////

      // General
      vProcess[processId]->fill_histogram("hMass", mass(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hMassAN", mass(), evt_weight, vId);

      // Skip blinded region for MC after filling mass histogram
      if (!isSignal && !isData && blind && mass() > 120 && mass() < 130)     continue;

      double dipho_mass_resolution = 0.5* pow((pow(lead_sigmaEoE(),2) + pow(sublead_sigmaEoE(),2)), 0.5);
      vProcess[processId]->fill_histogram("hDiphotonMassResolution", dipho_mass_resolution, evt_weight, vId);

      vProcess[processId]->fill_histogram("hTopTagger_score", topTag_score(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hTopTagger_topMass", topTag_topMass(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hTopTagger_WMass", topTag_WMass(), evt_weight, vId);

      double helic = helicity(lead_photon,  sublead_photon);//
      vProcess[processId]->fill_histogram("hAbsCosHelicity", helic, evt_weight, vId);

      vProcess[processId]->fill_histogram("hLeadMinDr", min_dr(lead_photon, objects), evt_weight, vId);
      vProcess[processId]->fill_histogram("hSubleadMinDr", min_dr(sublead_photon, objects), evt_weight, vId);

      vProcess[processId]->fill_histogram("hPhotonDeltaR", lead_photon.DeltaR(sublead_photon), evt_weight, vId);

      vProcess[processId]->fill_histogram("hPtHiggs", diphoton.Pt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hMinDrDiphoJet", min_dr(diphoton, jets), evt_weight, vId);
      vProcess[processId]->fill_histogram("hDeltaRDiphoLep", diphoton.DeltaR(leps[0]), evt_weight, vId);

      vProcess[processId]->fill_histogram("hPixelSeed", leadPixelSeed(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPixelSeed", subleadPixelSeed(), evt_weight, vId);
      if (abs(leadEta()) < barrel_eta)
        vProcess[processId]->fill_histogram("hPixelSeedEB", leadPixelSeed(), evt_weight, vId);
      else
        vProcess[processId]->fill_histogram("hPixelSeedEE", leadPixelSeed(), evt_weight, vId);
      if (abs(subleadEta()) < barrel_eta)
        vProcess[processId]->fill_histogram("hPixelSeedEB", subleadPixelSeed(), evt_weight, vId);
      else
        vProcess[processId]->fill_histogram("hPixelSeedEE", subleadPixelSeed(), evt_weight, vId);
	
      double close_mW, deltaR_dipho_W;
      if (n_ele() + n_muons() == 1) { // only for semileptonic events where we expect W->qq 
	// Hadronic W
	close_mW = closest_mW(jets, diphoton, deltaR_dipho_W);
	vProcess[processId]->fill_histogram("hDijetClosestWMass", close_mW, evt_weight, vId);
	vProcess[processId]->fill_histogram("hDeltaRDiphoW", deltaR_dipho_W, evt_weight, vId);

	// Hadronic top
	if (jets.size() >= 3) {
	  TLorentzVector top = get_hadronic_top(jets, btag_scores_sorted);
	  if (top.Pt() > 0) {
	    vProcess[processId]->fill_histogram("hTopPt", top.Pt(), evt_weight, vId);
	    vProcess[processId]->fill_histogram("hTopEta", top.Eta(), evt_weight, vId);
	    vProcess[processId]->fill_histogram("hTopMass", top.M(), evt_weight, vId);
	    vProcess[processId]->fill_histogram("hDeltaRDiphoTop", top.DeltaR(diphoton), evt_weight, vId);
	  }
	}
      }
      for (int i = 0; i < jets.size(); i++) {
	for (int j = i + 1; j < jets.size(); j++) {
	  TLorentzVector dijet = jets[i] + jets[j];
	  vProcess[processId]->fill_histogram("hDijetMass", dijet.M(), evt_weight, vId);
	}
      }

      vProcess[processId]->fill_histogram("hLeptonicMVA", mva_value, evt_weight, vId);
      vProcess[processId]->fill_histogram("hRapidity", dipho_rapidity(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hDiphotonSumPt", dipho_sumpt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hDiphotonCosPhi", dipho_cosphi(), evt_weight, vId);

      vProcess[processId]->fill_histogram("hNVtx", nvtx(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hMetPt", MetPt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hHT", get_ht(jets), evt_weight, vId);

      vProcess[processId]->fill_histogram("hNJets", n_jets(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hNbLoose", nb_loose(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hNbMedium", nb_medium(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hNbTight", nb_tight(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hMT", mT(), evt_weight, vId);
      if (jet_pt1() != -100)      vProcess[processId]->fill_histogram("hJet1pT", jet_pt1(), evt_weight, vId);
      if (jet_pt2() != -100)      vProcess[processId]->fill_histogram("hJet2pT", jet_pt2(), evt_weight, vId);
      if (jet_pt3() != -100)      vProcess[processId]->fill_histogram("hJet3pT", jet_pt3(), evt_weight, vId);
      if (jet_pt4() != -100)      vProcess[processId]->fill_histogram("hJet4pT", jet_pt4(), evt_weight, vId);
      if (jet_pt5() != -100)      vProcess[processId]->fill_histogram("hJet5pT", jet_pt5(), evt_weight, vId);
      if (jet_pt6() != -100)      vProcess[processId]->fill_histogram("hJet6pT", jet_pt6(), evt_weight, vId);

      if (jet_pt1() != -100)      vProcess[processId]->fill_histogram("hJet1Eta", jet_eta1(), evt_weight, vId);
      if (jet_pt2() != -100)      vProcess[processId]->fill_histogram("hJet2Eta", jet_eta2(), evt_weight, vId);
      if (jet_pt3() != -100)      vProcess[processId]->fill_histogram("hJet3Eta", jet_eta3(), evt_weight, vId);
      if (jet_pt4() != -100)      vProcess[processId]->fill_histogram("hJet4Eta", jet_eta4(), evt_weight, vId);
      if (jet_pt5() != -100)      vProcess[processId]->fill_histogram("hJet5Eta", jet_eta5(), evt_weight, vId);
      if (jet_pt6() != -100)      vProcess[processId]->fill_histogram("hJet6Eta", jet_eta6(), evt_weight, vId);

      vProcess[processId]->fill_histogram("hMaxBTag", btag_scores_sorted[0].second, evt_weight, vId);
      vProcess[processId]->fill_histogram("hSecondMaxBTag", btag_scores_sorted[1].second, evt_weight, vId);


      double lep_pt, lep_eta;
      lep_pt = get_lep_pt(lep_eta);
      vProcess[processId]->fill_histogram("hLeptonPt", lep_pt, evt_weight, vId);
      vProcess[processId]->fill_histogram("hLeptonEta", lep_eta, evt_weight, vId);

      // Leading Photon
      vProcess[processId]->fill_histogram("hPhotonLeadPt", leadPt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadEt", leadEt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadEta", leadEta(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadPhi", leadPhi(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadSigmaIEtaIEta", lead_sieie(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadHOverE", lead_hoe(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadR9", leadR9(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadIDMVA", leadIDMVA(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadPToM", lead_ptoM(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadSigmaEOverE", lead_sigmaEoE(), evt_weight, vId);

      if (lead_closest_gen_dR() < 999) {
        vProcess[processId]->fill_histogram("hPhotonLeadPtGen", lead_closest_gen_Pt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonPtRatio", leadPt() / lead_closest_gen_Pt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonDeltaRGen", lead_closest_gen_dR(), evt_weight, vId);
      }
      
      // Subleading Photon
      vProcess[processId]->fill_histogram("hPhotonSubleadPt", subleadPt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadEt", subleadEt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadEta", subleadEta(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadPhi", subleadPhi(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadSigmaIEtaIEta", sublead_sieie(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadHOverE", sublead_hoe(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadR9", subleadR9(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadIDMVA", subleadIDMVA(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadPToM", sublead_ptoM(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadSigmaEOverE", sublead_sigmaEoE(), evt_weight, vId); 

      if (sublead_closest_gen_dR() < 999) {
        vProcess[processId]->fill_histogram("hPhotonSubleadPtGen", sublead_closest_gen_Pt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonPtRatio", subleadPt() / sublead_closest_gen_Pt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonDeltaRGen", sublead_closest_gen_dR(), evt_weight, vId);
      }

      double maxID = leadIDMVA() >= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      double minID = leadIDMVA() >= subleadIDMVA() ? subleadIDMVA() : leadIDMVA();

      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA", maxID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_coarse", maxID, evt_weight, vId);
      if (nb_medium() == 0) {
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse_0b", minID, evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_coarse_0b", maxID, evt_weight, vId);
      }
      vProcess[processId]->fill_histogram("hDiphoMVA", diphoMVARes(), evt_weight, vId);
     
      if (lead_photon_type() == 1) {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_prompt", leadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_prompt", leadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_prompt", leadEta(), evt_weight, vId);
      }
      else if (lead_photon_type() == 2) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_elec", leadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_elec", leadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_elec", leadEta(), evt_weight, vId);
      }
      else if (lead_photon_type() == 3) {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_fake", leadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_fake", leadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_fake", leadEta(), evt_weight, vId);
      }

      if (sublead_photon_type() == 1) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_prompt", subleadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_prompt", subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_prompt", subleadEta(), evt_weight, vId);
      }
      else if (sublead_photon_type() == 2) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_elec", subleadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_elec", subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_elec", subleadEta(), evt_weight, vId);
      }
      else if (sublead_photon_type() == 3) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_fake", subleadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_fake", subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_fake", subleadEta(), evt_weight, vId);
      }


      // Fill veto study histograms
      if (leadPassEVeto()) {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_passEVeto", leadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_passEVeto", leadPt(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonEta_passEVeto", leadEta(), evt_weight, vId);
      }
      else {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_failEVeto", leadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_failEVeto", leadPt(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonEta_failEVeto", leadEta(), evt_weight, vId);
      }
      if (subleadPassEVeto()) {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_passEVeto", subleadIDMVA(), evt_weight, vId);	
	vProcess[processId]->fill_histogram("hPhotonPt_passEVeto", subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_passEVeto", subleadEta(), evt_weight, vId);
      }
      else {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_failEVeto", subleadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_failEVeto", subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_failEVeto", subleadEta(), evt_weight, vId);
      }

      if (!leadPixelSeed()) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_passPSV", leadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_passPSV", leadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_passPSV", leadEta(), evt_weight, vId);
      }
      else {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_failPSV", leadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_failPSV", leadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_failPSV", leadEta(), evt_weight, vId);
      }
      if (!subleadPixelSeed()) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_passPSV", subleadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_passPSV", subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_passPSV", subleadEta(), evt_weight, vId);
      }
      else {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_failPSV", subleadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonPt_failPSV", subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonEta_failPSV", subleadEta(), evt_weight, vId);
      }


      if (leadPixelSeed() || subleadPixelSeed()) 
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_failPSV", minID, evt_weight, vId);
      else
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_passPSV", minID, evt_weight, vId);

      // pass E-veto
      if (leadPassEVeto() && subleadPassEVeto())
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_passEVeto", minID, evt_weight, vId);
      else
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_failEVeto", minID, evt_weight, vId);

      // pass both
      if ((!leadPixelSeed() && !subleadPixelSeed()) && (leadPassEVeto() && subleadPassEVeto()))
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_passBothVeto", minID, evt_weight, vId);
      else
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_failBothVeto", minID, evt_weight, vId);

    }
  
    // Clean Up
    delete tree;
    file.Close();
  }
  if (nEventsChain != nEventsTotal) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }
 
  baby->CloseBabyNtuple();
 
  delete rand_map;

  // Example Histograms
  f1->Write();
  f1->Close(); 
 

  // return
  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time: " << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time: " << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return 0;
}
