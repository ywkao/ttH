#include "ScanChain_ttHHadronic.h"

int ScanChain(TChain* chain, TString tag, TString year, TString ext, TString xml_file, TString scale_qcd, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag + "_" + ext + "_histograms" + year + ".root", "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  bool evaluate_mva = xml_file != "none";

  bool combine_gjets_samples = tag != "ttHHadronic_GJet_Reweight_Preselection"; 
  //bool scale_gjets_normalization = !(tag.Contains("GJet_Reweight_Preselection"));
  bool scale_gjets_normalization = true;

  // Make MVA Optimization Baby
  /*
  OptimizationBabyMaker* baby = new OptimizationBabyMaker();
  TString xml_file_noExt = xml_file;
  xml_file_noExt.ReplaceAll(".xml", "");
  TString optimization_baby_name = "Optimization/MVAOptimizationBaby_" + ext + "_" + xml_file_noExt + "_" + tag;
  baby->MakeBabyNtuple( Form("%s.root", optimization_baby_name.Data()));
  */

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
  //RandomMap* rand_map = new RandomMap("Utils/random_map_Hadronic_" + ext + ".txt");

  TF1* gjet_minID_shape = get_photon_ID_shape("min");
  TF1* gjet_maxID_shape = get_photon_ID_shape("max");
  TF1* gjet_leadID_shape = get_photon_ID_shape("lead");
  TF1* gjet_subleadID_shape = get_photon_ID_shape("sublead");
  TF1* photon_fakeID_shape = get_photon_ID_shape("fake");

  TF1* photon_fakeID_shape_Barrel_HighPt = get_photon_ID_shape("fake_barrel_highPt");
  TF1* photon_fakeID_shape_Barrel_LowPt = get_photon_ID_shape("fake_barrel_lowPt");
  TF1* photon_fakeID_shape_Endcap_HighPt = get_photon_ID_shape("fake_endcap_highPt");
  TF1* photon_fakeID_shape_Endcap_LowPt = get_photon_ID_shape("fake_endcap_lowPt");
  vector<TF1*> photon_fakeID_shapes = {photon_fakeID_shape_Barrel_HighPt, photon_fakeID_shape_Barrel_LowPt, photon_fakeID_shape_Endcap_HighPt, photon_fakeID_shape_Endcap_LowPt};

  TF2* gjet_ID_shape = get_photon_ID_shape();

  // MVA Business
  unique_ptr<TMVA::Reader> mva;
  unique_ptr<TMVA::Reader> gjet_mva;

  // Declare BDT vars
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
  float dipho_pt_over_mass_;

  float helicity_angle_;

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
    //mva->AddVariable("leadIDMVA_", &leadIDMVA_);
    //mva->AddVariable("subleadIDMVA_", &subleadIDMVA_); 
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
    mva->AddVariable("dipho_pt_over_mass_", &dipho_pt_over_mass_);

    mva->AddVariable("helicity_angle_", &helicity_angle_);



    mva->BookMVA("BDT", "../MVAs/" + xml_file);
  }

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

  double dipho_yield = 0;

  double reverse_yield = 0; // yield of events in which there is one photon in the sideband and it is not a fake
  double reverse_yield_denom = 0; // yield of events in which there is one photon in the sideband

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

    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma"); 
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG") || currentFileTitle.Contains("THQ") || currentFileTitle.Contains("THW") || currentFileTitle.Contains("VBF") || currentFileTitle.Contains("GluGluHToGG") || currentFileTitle.Contains("VHToGG"); 

    if (isSignal) {
      if (categorize_signal_sample(currentFileTitle) != 0)
        continue;
    }

    TString mYear = currentFileTitle.Contains("2016") ? "2016" : (currentFileTitle.Contains("2017") ? "2017" : (currentFileTitle.Contains("2018") ? "2018" : "2018"));

    // Set json file
    set_json(mYear);

    bool reweight_gjets;
    if (tag.Contains("GJet_Reweight_Preselection") || (tag == "ttHHadronic_correlation_presel")) {
      if (!(currentFileTitle.Contains("GJet_Pt") || currentFileTitle.Contains("GJets_HT"))) {
        cout << "Skipping " << currentFileTitle << endl;
        continue;
      }
    }

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

      // Check golden json
      if (isData) {
        if (!pass_json(mYear, cms3.run(), cms3.lumi()))            continue;
      }

      // Blinded region
      if (isData && blind && mass() > 120 && mass() < 130)	continue;

      // Fill mva baby before any selections
      int genPhotonId = isData ? -1 : categorize_photons(leadGenMatch(), subleadGenMatch());
      int processId = categorize_process(currentFileTitle, genPhotonId);

      int genLeptonId = isData ? -1 : categorize_leptons(nGoodEls(), nGoodMus());
      int genPhotonDetailId = isData ? -1 : categorize_photons_detail(lead_photon_type(), sublead_photon_type());
      int photonLocationId = categorize_photon_locations(leadEta(), subleadEta());

      float evt_weight = 1.;
     
	/* 
      if (!isData) {
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


      int label = isData ? 2 : (isSignal ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data

      vector<TLorentzVector> jets;
      TLorentzVector lead_photon;
      TLorentzVector sublead_photon;
      lead_photon = make_lead_photon();
      sublead_photon = make_sublead_photon();
      TLorentzVector diphoton = lead_photon + sublead_photon;

      vector<double> btag_scores;
      vector<std::pair<int, double>> btag_scores_sorted;
      jets = make_jets(btag_scores, year);
      btag_scores_sorted = sortVector(btag_scores);
      
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

      bool do_scale_qcd = scale_qcd != "none";
      if (do_scale_qcd) {
	evt_weight *= qcdX_factor(currentFileTitle, scale_qcd, n_jets());
      }

      // Evaluate MVA, if we choose
      double mva_value = -999;

      // Calculate MVA value
      maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();

      float leadID_ = leadIDMVA();
      float subleadID_ = subleadIDMVA();

      if (tag == "ttHHadronicLoose_impute" || tag == "ttHHadronicLoose_impute_tightenPhoID" || tag == "ttHHadronic_impute_SR" || tag == "ttHHadronicLoose_impute_fakesRegion") {
	if (processId == 3 || processId == 4 || processId == 17) continue;
	if (maxIDMVA_ < -0.7)					 continue;
	if (minIDMVA_ < -0.7) {
          if (!isData)
            continue;
	  
          //minIDMVA_ = impute_photon_id(-0.7, maxIDMVA_, cms3.event(), gjet_minID_shape, gjet_maxID_shape, evt_weight, gjet_ID_shape);
	  //if (minIDMVA_ >= maxIDMVA_)
          //  swap(minIDMVA_, maxIDMVA_);

	  //impute_lead_sublead_photon_id(-0.7, leadID_, subleadID_, cms3.event(), gjet_leadID_shape, gjet_subleadID_shape, evt_weight);
          //maxIDMVA_ = leadID_ > subleadID_ ? leadID_ : subleadID_;
          //minIDMVA_ = leadID_ <= subleadID_ ? leadID_ : subleadID_;

	  minIDMVA_ = impute_from_fakePDF(-0.7, maxIDMVA_, cms3.event(), photon_fakeID_shape, evt_weight);
	  //minIDMVA_ = impute_from_fakePDF_withKinematics(-0.7, maxIDMVA_, cms3.event(), photon_fakeID_shapes, leadID_ > subleadID_ ? leadPt() : subleadPt(), leadID_ > subleadID_ ? abs(leadEta()) : abs(subleadEta()), evt_weight);

          processId = 18;
        }
      }

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

      leadPSV_ = leadPixelSeed();
      subleadPSV_ = subleadPixelSeed();

      dipho_cosphi_ = dipho_cosphi();
      dipho_rapidity_ = dipho_rapidity();
      dipho_pt_ = diphoton.Pt();
      dipho_pt_over_mass_ = diphoton.Pt() / mass();

      helicity_angle_ = helicity(lead_photon, sublead_photon);

      met_ = MetPt();

      if (evaluate_mva) 
        mva_value = convert_tmva_to_prob(mva->EvaluateMVA( "BDT" ));
      double reference_mva = tthMVA();
      bool pass_ref_presel = mYear == "2017" ? pass_2017_mva_presel() : pass_2016_mva_presel();
      double super_rand = -999;//rand_map->retrieve_rand(cms3.event(), cms3.run(), cms3.lumi());

      int mvaCategoryId = mva_value < -0.8 ? 0 : 1;
      vector<int> vId = {genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId, mvaCategoryId};


      // Gamma + jets reweighting
      double gjet_mva_value = -999;	

      njets_ = n_jets();
      jet1_pt_  = jet1_pt() > 0 ? jet1_pt() : -999;	
      ht_ = get_ht(jets);

      gjet_mva_value = convert_tmva_to_prob(gjet_mva->EvaluateMVA( "BDT" )); 

      if (combine_gjets_samples) {
	if (processId == 17) 
	  processId = 3;
      }
      if (processId == 3 && combine_gjets_samples) {
	double prob = gjet_mva_value;
	double prob_ratio = prob / ( 1 - prob);
	evt_weight *= prob_ratio;
	if (scale_gjets_normalization)
	  evt_weight *= gjet_normalization;
	//cout << "Reweighting GJets event by: " << prob_ratio * gjet_normalization << endl;
      } 

      if (!combine_gjets_samples) { // skip Pythia if we aren't combining Pythia and MadGraph
	if (processId == 3) {
	  continue; 
	}
      }

      if (isnan(evt_weight) || isinf(evt_weight) || evt_weight == 0) {
        continue; //some pu weights are nan/inf and this causes problems for histos 
      }


      //if (evaluate_mva) 
      //  baby->FillBabyNtuple(label, evt_weight, processId, cms3.rand(), mass(), mva_value, reference_mva, pass_ref_presel, super_rand);

      // Selection

      // Skipping events/samples
      //if (is_low_stats_process(currentFileTitle))       continue;
      //if (tag != "GJet_Reweight_Preselection") {
      //  if (process_id_ == 17)                          continue; // skip MadGraph G+Jets
      //}
      if (is_wrong_tt_jets_sample(currentFileTitle, "Hadronic"))                        continue;
      if (has_ttX_overlap(currentFileTitle, lead_Prompt(), sublead_Prompt()))           continue;
      if (has_simple_qcd_overlap(currentFileTitle, genPhotonId))                        continue;

      if (tag == "ttHHadronicLoose") {
        if (mass() < 100)                continue;
	if (n_jets() < 3)		continue;
	if (nb_loose() < 1)		continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadIDMVA() < -0.9)                 continue;
        if (subleadIDMVA() < -0.9)              continue;
      }

      else if (tag == "ttHHadronicLoose_impute") {
	if (mass() < 100)                continue;
        if (n_jets() < 3)               continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (minIDMVA_ < -0.7)		continue;	
	if (leadIDMVA() < -0.9)                 continue;
        if (subleadIDMVA() < -0.9)              continue;
      }

      else if (tag == "ttHHadronic_impute_SR") {
        if (mass() < 100)                continue;
        if (n_jets() < 3)               continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (minIDMVA_ < -0.7)           continue;
        if (leadIDMVA() < -0.9)                 continue;
        if (subleadIDMVA() < -0.9)              continue;
	if (mva_value < 0.97997838)	continue; // based on "Hadronic_1617_impute_18Feb2019__bdt.xml", should give ~50% signal eff. 
      }

      else if (tag == "ttHHadronicLoose_impute_tightenPhoID") {
        if (mass() < 100)                continue;
        if (n_jets() < 3)               continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (minIDMVA_ < -0.2)           continue;
        if (leadIDMVA() < -0.9)                 continue;
        if (subleadIDMVA() < -0.9)              continue;
      }

      else if (tag == "ttHHadronicLoose_impute_fakesRegion") {
        if (mass() < 100)                continue;
        if (n_jets() < 3)               continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (minIDMVA_ < -0.2 || minIDMVA_ > 0.5)           continue;
        if (leadIDMVA() < -0.9)                 continue;
        if (subleadIDMVA() < -0.9)              continue;
      }

      else if (tag == "ttHHadronic_MC_SR") {
        if (mass() < 100)                continue;
        if (n_jets() < 3)               continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (minIDMVA_ < -0.7)           continue;
        if (leadIDMVA() < -0.9)                 continue;
        if (subleadIDMVA() < -0.9)              continue;
	//if (mva_value < 0.98490679)     continue; // based on "Hadronic_1617_ttHHadronicLoose_phoIDCut_18Feb2019__bdt.xml", should give ~50% signal eff.
	if (mva_value < 0.983)		continue;
	cout << "Process id: " << processId << " mva value: " << mva_value << endl;
      }


      else if (tag == "ttHHadronicLoose_impute_presel") {
	if (mass() < 100)               continue;
        if (n_jets() < 3)               continue;
        if (nb_loose() < 1)             continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (minIDMVA_ < -0.7)		continue;
	if (leadIDMVA() < -0.9)                 continue;
        if (subleadIDMVA() < -0.9)              continue;
      }

      else if (tag == "ttHHadronicLoose_impute_sideband") {
	if (mass() < 100)               continue;
	if (n_jets() < 3)               continue;
	if (nb_loose() < 1)             continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (maxIDMVA_ < -0.7)		continue;
        if (minIDMVA_ >= -0.7)           continue;
	if (leadIDMVA() < -0.9)                 continue;
        if (subleadIDMVA() < -0.9)              continue;
      }

      else if (tag == "ttHHadronic_baseline_maxZA") {
	if (mass() < 100)                continue;
	if (n_jets() < 3)               continue;
	if (nb_loose() < 1)             continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (mva_value <= 0.9947173)      continue; // should give about 6 * 1.5 signal events 
      }
      else if (tag == "ttHHadronic_data_sideband_0b_train") {
	if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_medium() != 0)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHHadronic_data_sideband_0b_test") {
	if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_medium() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHHadronic_data_sideband_0b_maxZA") {
        if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_medium() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (max1_btag_ < 0.83357537)	 continue;
	//if (max2_btag_ < 0.0059)	 continue;
	//cout << convert_tmva_to_prob(mva_value) << endl;
	if (mva_value <= 0.99688774)	 continue; // should give about 6.3 * 1.5 signal events
      }
      else if (tag == "ttHHadronic_data_sideband_phoID_train") {
        if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_tight() < 1)             continue;
	if (minIDMVA_ >= -0.2)		 continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHHadronic_data_sideband_phoID_v3_train") {
	if (mass() < 100)               continue;
	if (n_jets() < 2)		continue;
	if (nb_loose() < 1)		continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (minIDMVA_ >= -0.2)           continue;
      }	

      else if (tag == "ttHHadronic_data_sideband_phoID_test") {
        if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_tight() < 1)             continue;
	if (minIDMVA_ < -0.2)		 continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHHadronic_data_sideband_phoID_maxZA") {
        if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_tight() < 1)              continue;
	if (maxIDMVA_ <= 0.874744)	 continue;
	if (minIDMVA_ <= -0.2)	 	 continue;
	if (mva_value <= 0.98031139)	 continue; // should give about 6 * 1.5 signal events 
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHHadronic_data_sideband_train") {
	if (mass() < 100)                continue;
	if (n_jets() < 2)		 continue;
	if (nb_tight() != 0)		 continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHHadronic_data_sideband_test") {
        if (mass() < 100)                continue;
        if (n_jets() < 2)                continue;
        if (nb_tight() < 1)              continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHHadronic_QCDFits_Presel") {
	if (mass() < 100)		continue; 
	if (n_jets() < 2)               continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }

      else if (tag == "ttHHadronic_QCDFits_Presel_QCDEnriched") {
        if (mass() < 100)               continue;
        if (n_jets() < 2)               continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadIDMVA() >= 0.25)	continue;
	if (subleadIDMVA() >= 0.25)	continue;
      }

      else if (tag == "ttHHadronic_QCDFits_Presel_GJetEnriched") {
        if (mass() < 100)               continue;
        if (n_jets() < 2)               continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	double maxID = leadIDMVA() >= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
	double minID = leadIDMVA() < subleadIDMVA() ? leadIDMVA() : subleadIDMVA(); 
        if (maxID < 0.25)		continue;
        if (minID > 0.25)		continue;
      }

      else if (tag == "ttHHadronic_QCDFits_Presel_DiphoEnriched") {
        if (mass() < 100)               continue;
        if (n_jets() < 2)               continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (leadIDMVA() < 0.25)        continue;
        if (subleadIDMVA() < 0.25)     continue;
      }

      else if (tag == "ttHHadronicTight") {
        if (mass() < 100)                continue;
        if (n_jets() < 4)               continue;
        if (nb_medium() < 2)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadPixelSeed() || subleadPixelSeed())      continue;
	if (leadIDMVA() < -0.2)                 continue;
        if (subleadIDMVA() < -0.2)              continue;
      }

      else if (tag == "ttHHadronic_GJet_Reweight_Preselection" || tag == "ttHHadronic_GJet_Reweight_Preselection_wWeights" || tag == "ttHHadronic_correlation_presel") {
	if (mass() < 100)	continue;
        if (n_jets() < 2)       continue;
      }

      else if (tag == "ttHHadronic_2017_Presel") {
	if (mass() < 100 || mass() > 180 || (mass() > 115 && mass() < 135)) 	continue;
	if (leadIDMVA() < -0.2)                 continue;
        if (subleadIDMVA() < -0.2)              continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (n_jets() < 2)                       continue;
	if (btag_scores_sorted[1].second <= 0)		continue;
      }

      else if (tag == "ttHHadronic_2017_SR_like") {
	if (mass() < 100)                       continue;
	if (!isSignal) { if (mass() < 100 || mass() > 180 || (mass() > 115 && mass() < 135))     continue;}
        //if (isData && blind && mass() > 115 && mass() < 135)      continue;
        if (leadIDMVA() < -0.2)                 continue;
        if (subleadIDMVA() < -0.2)              continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (n_jets() < 2)                       continue;
        if (tthMVA() < mva_thresh_2017[0])      continue;
        if (btag_scores_sorted[1].second <= 0)          continue;	
      }

      else if (tag == "ttHHadronicLoose_2018studies") {
	if (mass() < 100)                continue;
        if (n_jets() < 3)               continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
      }
      else if (tag == "ttHHadronic_HLLHC_presel") {
	if (mass() < 100 || mass() > 180)                continue;
	if (n_jets() < 3)               continue;
	if (nb_medium() < 1)		continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHHadronic_HLLHC_presel2") {
        if (mass() < 100 || mass() > 180)                continue;
        if (n_jets() < 3)               continue;
        if (nb_medium() < 1)            continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (leadIDMVA() < 0.55)         continue;
        if (subleadIDMVA() < 0.55)         continue;
      }
      else if (tag == "ttHHadronic_HLLHC_presel_withBDT") {
        if (mass() < 100 || mass() > 180)                continue;
        if (n_jets() < 3)               continue;
        if (nb_medium() < 1)            continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (tthMVA() < 0.38)		continue;
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
      else if (tag == "ttHHadronicCustom") {
	if (mass() < 100)                continue;
        if (n_jets() < 3)               continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	//if (mva_value < -0.9)		continue;
      }
      else if (tag == "2017MVAPreSel") {
        if (mass() < 100)       continue;
        if (n_jets() < 2)       continue;
        if (leadIDMVA() < -0.1)         continue;
        if (subleadIDMVA() < -0.1)         continue;
      }
      else if (tag == "ttHHadronic_2017_SR1") {
	if (mass() < 100)			continue;
	if (isData && blind && mass() > 115 && mass() < 135)      continue;
	if (leadIDMVA() < -0.2)			continue;
	if (subleadIDMVA() < -0.2)		continue;
	if (n_jets() < 2)			continue;
	if (!(tthMVA() >= mva_thresh_2017[0] && tthMVA() < mva_thresh_2017[1]))	continue; 
      }
      else if (tag == "ttHHadronic_2017_SR2") {
        if (mass() < 100)                       continue;
	if (isData && blind && mass() > 115 && mass() < 135)      continue;
        if (leadIDMVA() < -0.2)                 continue;
        if (subleadIDMVA() < -0.2)              continue;
        if (n_jets() < 2)                       continue;
        if (!(tthMVA() >= mva_thresh_2017[1] && tthMVA() < mva_thresh_2017[2])) continue;
      }
      else if (tag == "ttHHadronic_2017_SR3") {
        if (mass() < 100)                       continue;
	if (isData && blind && mass() > 115 && mass() < 135)      continue;
        if (leadIDMVA() < -0.2)                 continue;
        if (subleadIDMVA() < -0.2)              continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (n_jets() < 2)                       continue;
	if (tthMVA() < mva_thresh_2017[2])	continue;
      }

      else {
        cout << "Did not recognize tag name" << endl;
      }
 
 

      // Fill histograms //
      vProcess[processId]->fill_histogram("hGJet_BDT", gjet_mva_value, evt_weight, vId);
      vProcess[processId]->fill_histogram("hMass", mass(), evt_weight, vId);   
      vProcess[processId]->fill_histogram("hMassAN", mass(), evt_weight, vId);
      cout.setf(ios::fixed);
      cout << std::setprecision(6);

      // Skip blinded region for MC after filling mass histogram
      if (!(tag.Contains("GJet_Reweight_Preselection"))) {
        if (!isSignal && blind && mass() > 120 && mass() < 130)	continue;
      }

      // Fill rest of histograms //
      double dipho_mass_resolution = 0.5* pow((pow(lead_sigmaEoE(),2) + pow(sublead_sigmaEoE(),2)), 0.5);
      vProcess[processId]->fill_histogram("hDiphotonMassResolution", dipho_mass_resolution, evt_weight, vId);
      if (tthMVA() >= 0.75)
        vProcess[processId]->fill_histogram("hDiphotonMassResolutionHighMVA", dipho_mass_resolution, evt_weight, vId);
      else if (tthMVA() >= 0.3)
        vProcess[processId]->fill_histogram("hDiphotonMassResolutionMedMVA", dipho_mass_resolution, evt_weight, vId);
      else
        vProcess[processId]->fill_histogram("hDiphotonMassResolutionLowMVA", dipho_mass_resolution, evt_weight, vId);

      vProcess[processId]->fill_histogram("hTopTagger_score", topTag_score(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hTopTagger_topMass", topTag_topMass(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hTopTagger_WMass", topTag_WMass(), evt_weight, vId);

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
	
      double helic = helicity(lead_photon, sublead_photon);
      vProcess[processId]->fill_histogram("hAbsCosHelicity", helic, evt_weight, vId);

      vProcess[processId]->fill_histogram("hLeadMinDr", min_dr(lead_photon, jets), evt_weight, vId);
      vProcess[processId]->fill_histogram("hSubleadMinDr", min_dr(sublead_photon, jets), evt_weight, vId);

      vProcess[processId]->fill_histogram("hPhotonDeltaR", lead_photon.DeltaR(sublead_photon), evt_weight, vId);
      vProcess[processId]->fill_histogram("hDiphotonPtOverMass", diphoton.Pt() / mass(), evt_weight, vId);

      vProcess[processId]->fill_histogram("hPtHiggs", diphoton.Pt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hMinDrDiphoJet", min_dr(diphoton, jets), evt_weight, vId);

      vProcess[processId]->fill_histogram("hHadronicMVA", mva_value, evt_weight, vId);
      vProcess[processId]->fill_histogram("hHadronicMVA_coarse", mva_value, evt_weight, vId);
      vProcess[processId]->fill_histogram("hHadronicMVA_fine", mva_value, evt_weight, vId);

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


      if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("hJet1pT", jet1_pt(), evt_weight, vId);
      if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("hJet2pT", jet2_pt(), evt_weight, vId);
      if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("hJet3pT", jet3_pt(), evt_weight, vId);
      if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("hJet4pT", jet4_pt(), evt_weight, vId);

      if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("hJet1Eta", jet1_eta(), evt_weight, vId);
      if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("hJet2Eta", jet2_eta(), evt_weight, vId);
      if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("hJet3Eta", jet3_eta(), evt_weight, vId);
      if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("hJet4Eta", jet4_eta(), evt_weight, vId);

      if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("hJet1BTag", btag_scores[0], evt_weight, vId);
      if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("hJet2BTag", btag_scores[1], evt_weight, vId);
      if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("hJet3BTag", btag_scores[2], evt_weight, vId);
      if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("hJet4BTag", btag_scores[3], evt_weight, vId);

      if (bjet1_pt() != -1)     vProcess[processId]->fill_histogram("hbJet1pT", bjet1_pt(), evt_weight, vId);
      if (bjet2_pt() != -1)     vProcess[processId]->fill_histogram("hbJet2pT", bjet2_pt(), evt_weight, vId);

      // Leading Photon
      vProcess[processId]->fill_histogram("hPhotonLeadPt", leadPt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadEt", leadEt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadEta", leadEta(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadPhi", leadPhi(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadSigmaIEtaIEta", lead_sieie(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadHOverE", lead_hoe(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadR9", leadR9(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadIDMVA", leadID_, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadPToM", lead_ptoM(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadSigmaEOverE", lead_sigmaEoE(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadPixelSeed", leadPixelSeed(), evt_weight, vId);

      if (lead_closest_gen_dR() < 999) {
        vProcess[processId]->fill_histogram("hPhotonLeadPtGen", lead_closest_gen_Pt(), evt_weight, vId);
	if (lead_photon_type() == 1)
          vProcess[processId]->fill_histogram("hPhotonPtRatio", leadPt() / leadPtGen(), evt_weight, vId);
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
      vProcess[processId]->fill_histogram("hPhotonSubleadIDMVA", subleadID_, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadPToM", sublead_ptoM(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadSigmaEOverE", sublead_sigmaEoE(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadPixelSeed", subleadPixelSeed(), evt_weight, vId);

      if (sublead_closest_gen_dR() < 999) {
        vProcess[processId]->fill_histogram("hPhotonSubleadPtGen", sublead_closest_gen_Pt(), evt_weight, vId);
	if (sublead_photon_type() == 1)
          vProcess[processId]->fill_histogram("hPhotonPtRatio", subleadPt() / subleadPtGen(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonDeltaRGen", sublead_closest_gen_dR(), evt_weight, vId);
      }

      double maxID = maxIDMVA_;
      double minID = minIDMVA_; 

      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA", maxID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_fine", maxID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_coarse", maxID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_entries", maxID, 1, vId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_fine_entries", maxID, 1, vId);

      vProcess[processId]->fill_histogram("hPhotonMinIDMVA", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_fine", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_entries", minID, 1, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_fine_entries", minID, 1, vId);

      if (nb_medium() == 0) {
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse_0b", minID, evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_coarse_0b", maxID, evt_weight, vId);
      }

      vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_NJets", maxID, n_jets(), evt_weight, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonMinIDMVA_NJets", minID, n_jets(), evt_weight, vId); 
      vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_NJets_entries", maxID, n_jets(), 1, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonMinIDMVA_NJets_entries", minID, n_jets(), 1, vId);

      vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_MinIDMVA", maxID, minID, evt_weight, vId);


      if (leadIDMVA() >= subleadIDMVA()) {
	vProcess[processId]->fill_histogram("hMaxIDPhotonPt", leadPt(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hMaxIDPhotonEta", leadEta(), evt_weight, vId);
	vProcess[processId]->fill_histogram("hMinIDPhotonPt", subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hMinIDPhotonEta", subleadEta(), evt_weight, vId);
      }
      else {
	vProcess[processId]->fill_histogram("hMaxIDPhotonPt", subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hMaxIDPhotonEta", subleadEta(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hMinIDPhotonPt", leadPt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hMinIDPhotonEta", leadEta(), evt_weight, vId);
      }


      if (minID < -0.7 && maxID > -0.7) {
	reverse_yield_denom += evt_weight;
	if (lead_photon_type() == 1 && sublead_photon_type() == 3) {
	  if (leadIDMVA() < subleadIDMVA()) {
	    reverse_yield += evt_weight;
	    //cout << "Prompt IDMVA: " << leadIDMVA() << endl;
	    //cout << "Fake IDMVA:   " << subleadIDMVA() << endl;
	  }
	}
	else if (lead_photon_type() == 3 && sublead_photon_type() == 1) {
	  if (leadIDMVA() > subleadIDMVA()) {
	    reverse_yield += evt_weight;
	    //cout << "Prompt IDMVA: " << subleadIDMVA() << endl;
            //cout << "Fake IDMVA:   " << leadIDMVA() << endl;
	  }
	}

      }

      if (lead_photon_type() == 3) {
	vProcess[processId]->fill_histogram("hFakePhotonIDMVA", leadIDMVA(), evt_weight, vId);
	if (leadPt() > 40 && abs(leadEta()) < barrel_eta)
	  vProcess[processId]->fill_histogram("hFakePhotonIDMVA_Barrel_HighPt", leadIDMVA(), evt_weight, vId);
        else if (leadPt() < 40 && abs(leadEta()) < barrel_eta)
	  vProcess[processId]->fill_histogram("hFakePhotonIDMVA_Barrel_LowPt", leadIDMVA(), evt_weight, vId); 
	else if (leadPt() > 40 && abs(leadEta()) > barrel_eta)
          vProcess[processId]->fill_histogram("hFakePhotonIDMVA_Endcap_HighPt", leadIDMVA(), evt_weight, vId); 
        else if (leadPt() < 40 && abs(leadEta()) > barrel_eta)
          vProcess[processId]->fill_histogram("hFakePhotonIDMVA_Endcap_LowPt", leadIDMVA(), evt_weight, vId); 
        vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_NJets", leadIDMVA(), njets_, evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_MaxBTag", leadIDMVA(), max1_btag_, evt_weight, vId); 
	vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_2ndMaxBTag", leadIDMVA(), max2_btag_, evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_DiPhotonPt", leadIDMVA(), diphoton.Pt(), evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_Pt", leadIDMVA(), leadPt(), evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_Eta", leadIDMVA(), abs(leadEta()), evt_weight, vId);
      }
      else if (lead_photon_type() == 1) {
	vProcess[processId]->fill_histogram("hPromptPhotonIDMVA", leadIDMVA(), evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_NJets", leadIDMVA(), njets_, evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_MaxBTag", leadIDMVA(), max1_btag_, evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_2ndMaxBTag", leadIDMVA(), max2_btag_, evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_DiPhotonPt", leadIDMVA(), diphoton.Pt(), evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_Pt", leadIDMVA(), leadPt(), evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_Eta", leadIDMVA(), abs(leadEta()), evt_weight, vId);
      }

      if (sublead_photon_type() == 3) {
	vProcess[processId]->fill_histogram("hFakePhotonIDMVA", subleadIDMVA(), evt_weight, vId);
	if (subleadPt() > 40 && abs(subleadEta()) < barrel_eta)
          vProcess[processId]->fill_histogram("hFakePhotonIDMVA_Barrel_HighPt", subleadIDMVA(), evt_weight, vId);
        else if (subleadPt() < 40 && abs(subleadEta()) < barrel_eta)
          vProcess[processId]->fill_histogram("hFakePhotonIDMVA_Barrel_LowPt", subleadIDMVA(), evt_weight, vId);
        else if (subleadPt() > 40 && abs(subleadEta()) > barrel_eta)
          vProcess[processId]->fill_histogram("hFakePhotonIDMVA_Endcap_HighPt", subleadIDMVA(), evt_weight, vId); 
        else if (subleadPt() < 40 && abs(subleadEta()) > barrel_eta)
          vProcess[processId]->fill_histogram("hFakePhotonIDMVA_Endcap_LowPt", subleadIDMVA(), evt_weight, vId); 
	vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_NJets", subleadIDMVA(), njets_, evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_MaxBTag", subleadIDMVA(), max1_btag_, evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_2ndMaxBTag", subleadIDMVA(), max2_btag_, evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_DiPhotonPt", subleadIDMVA(), diphoton.Pt(), evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_Pt", subleadIDMVA(), subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_Eta", subleadIDMVA(), abs(subleadEta()), evt_weight, vId);
      }
      else if (sublead_photon_type() == 1) {
	vProcess[processId]->fill_histogram("hPromptPhotonIDMVA", subleadIDMVA(), evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_NJets", subleadIDMVA(), njets_, evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_MaxBTag", subleadIDMVA(), max1_btag_, evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_2ndMaxBTag", subleadIDMVA(), max2_btag_, evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_DiPhotonPt", subleadIDMVA(), diphoton.Pt(), evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_Pt", subleadIDMVA(), subleadPt(), evt_weight, vId);
        vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_Eta", subleadIDMVA(), abs(subleadEta()), evt_weight, vId);
      }

      vProcess[processId]->fill_2D_histogram("hPhotonLeadIDMVA_LeadPt", leadIDMVA(), leadPt(), evt_weight, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonLeadIDMVA_LeadEta", leadIDMVA(), abs(leadEta()), evt_weight, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonSubleadIDMVA_SubleadPt", subleadIDMVA(), subleadPt(), evt_weight, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonSubleadIDMVA_SubleadEta", subleadIDMVA(), abs(subleadEta()), evt_weight, vId); 

      if (n_jets() == 2) {
	vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_NJets2", maxID, evt_weight, vId);
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_NJets2", minID, evt_weight, vId);
      }
      else if (n_jets() == 3) {
	vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_NJets3", maxID, evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_NJets3", minID, evt_weight, vId);
      }
      else if (n_jets() >= 4) {
	vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_NJets4+", maxID, evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_NJets4+", minID, evt_weight, vId);
      }

      vProcess[processId]->fill_histogram("hDiphoMVA", diphoMVARes(), evt_weight, vId);

      // ttH-Hadronic Specific
      vProcess[processId]->fill_histogram("hMaxBTag", bjet1_csv(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hSecondMaxBTag", bjet2_csv(), evt_weight, vId);
      vProcess[processId]->fill_histogram("htthMVA", tthMVA(), evt_weight, vId);

      if (lead_photon_type() == 1)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_prompt", maxID, evt_weight, vId);
      else if (lead_photon_type() == 2)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_elec", maxID, evt_weight, vId);
      else if (lead_photon_type() == 3)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_fake", maxID, evt_weight, vId);

      if (sublead_photon_type() == 1)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_prompt", maxID, evt_weight, vId);
      else if (sublead_photon_type() == 2)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_elec", maxID, evt_weight, vId);
      else if (sublead_photon_type() == 3)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_fake", maxID, evt_weight, vId);

    }
  
    // Clean Up
    delete tree;
    file.Close();
  }
  if (nEventsChain != nEventsTotal) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }
 
  //baby->CloseBabyNtuple();

  cout << "Number of events from sideband where minID is prompt: " << reverse_yield << endl;
  cout << "Number of events from sideband: " << reverse_yield_denom << endl;
  cout << "Fraction of events: " << reverse_yield / reverse_yield_denom << endl; 

  //delete rand_map;

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
