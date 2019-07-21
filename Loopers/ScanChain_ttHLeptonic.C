#include "ScanChain_ttHLeptonic.h"

int ScanChain(TChain* chain, TString tag, TString year, TString ext, TString xml_file, TString bkg_options, TString mYear = "", TString idx = "", bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag + "_" + ext + "_histograms" + year + idx + ".root", "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  bool evaluate_mva = xml_file != "none";

  // Make MVA Optimization Baby
  //OptimizationBabyMaker* baby = new OptimizationBabyMaker();
  TString xml_file_noExt = xml_file;
  xml_file_noExt.ReplaceAll(".xml", "");
  //TString optimization_baby_name = "Optimization/MVAOptimizationBaby_" + ext + "_" + xml_file_noExt + "_" + tag;
  //baby->MakeBabyNtuple( Form("%s.root", optimization_baby_name.Data()));

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

  // Dumb hacky stuff to use 2017 MC samples as placeholders for 2018
  bool already_looped_dipho = false;
  bool already_looped_qcd = false;
  bool already_looped_dy = false;


  // Initialize map of evt_run_lumi -> rand
  //RandomMap* rand_map = new RandomMap("Utils/random_map_Leptonic_" + ext + ".txt");

  TF1* photon_fakeID_shape = get_photon_ID_shape("fake");
  TF1* photon_fakeID_shape_runII = get_photon_ID_shape("fake_runII");

  // MVA Business
  unique_ptr<TMVA::Reader> mva;

  // BDT bins
  int nBins = 4;
  float binBounds[5] = {0.8435,0.9346,0.9625,0.9890,1};

  // Declare BDT vars
  float lep_pt_;
  float lep_eta_;

  float n_lep_loose_;
  float n_lep_medium_;
  float n_lep_tight_;

  float muon1_mini_iso_;
  float muon2_mini_iso_;

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

  float dipho_pt_over_mass_;
  float helicity_angle_;


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

    //mva->AddVariable("top_tag_score_", &top_tag_score_);
    
    mva->AddVariable("dipho_pt_over_mass_", &dipho_pt_over_mass_);
    mva->AddVariable("helicity_angle_", &helicity_angle_);

    mva->AddVariable("lep_pt_", &lep_pt_);
    mva->AddVariable("lep_eta_", &lep_eta_);

    //mva->AddVariable("n_lep_loose_", &n_lep_loose_);
    //mva->AddVariable("n_lep_medium_", &n_lep_medium_);
    mva->AddVariable("n_lep_tight_", &n_lep_tight_);

    //mva->AddVariable("muon1_mini_iso_", &muon1_mini_iso_);
    //mva->AddVariable("muon2_mini_iso_", &muon2_mini_iso_);

    mva->BookMVA("BDT", xml_file);
  }

  double w_gamma_yield = 0;

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
    if (currentFileTitle.Contains("GJet_Pt")) {
      cout << "Skipping Pythia GJets sample: " << currentFileTitle << endl;
      continue;
    }

    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma");
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG") || currentFileTitle.Contains("THQ") || currentFileTitle.Contains("THW") || currentFileTitle.Contains("VBF") || currentFileTitle.Contains("GluGluHToGG") || currentFileTitle.Contains("VHToGG") || currentFileTitle.Contains("FCNC");
    if (mYear == "")
      mYear = (currentFileTitle.Contains("Run2016") || currentFileTitle.Contains("RunIISummer16")) ? "2016" : ((currentFileTitle.Contains("Run2017") || currentFileTitle.Contains("RunIIFall17")) ? "2017" : ((currentFileTitle.Contains("Run2018") || currentFileTitle.Contains("RunIIAutumn18")) ? "2018" : "no_year")); 

    if (isSignal) {
      if (categorize_signal_sample(currentFileTitle) != 0)
        continue;
    }

    if (is_wrong_tt_jets_sample(currentFileTitle, "Leptonic"))                        continue;
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
      ttHLeptonic::progress( nEventsTotal, nEventsChain );

      // Check golden json
      if (isData) {
        if (!pass_json(mYear, cms3.run(), cms3.lumi()))		continue;
      }


      // Fill mva baby before any selections
      int genPhotonId = isData ? -1 : categorize_photons(leadGenMatch(), subleadGenMatch());
      int processId = categorize_process(currentFileTitle, genPhotonId);
      if (processId == 17)
	    processId = 3; // use Madgraph GJets instead of Pythia

      int genLeptonId = isData ? -1 : categorize_leptons(nGoodEls(), nGoodMus());
      int genPhotonDetailId = isData ? -1 : categorize_photons_detail(lead_photon_type(), sublead_photon_type());
      int photonLocationId = categorize_photon_locations(leadEta(), subleadEta());

      float evt_weight = 1.;
     
      if (year.Contains("RunII") && !isData) {
	double scale1fb = currentFileTitle.Contains("RunIISummer16MiniAOD") ? scale1fb_2016_RunII(currentFileTitle) : ( currentFileTitle.Contains("RunIIFall17MiniAOD") ? scale1fb_2017_RunII(currentFileTitle) : ( currentFileTitle.Contains("RunIIAutumn18MiniAOD") ? scale1fb_2018_RunII(currentFileTitle) : 0 ));
	if (mYear == "2016")
	  evt_weight *= scale1fb * lumi_2016 * weight();
	else if (mYear == "2017")
	  evt_weight *= scale1fb * lumi_2017 * weight();
	else if (mYear == "2018")
          evt_weight *= scale1fb * lumi_2018 * weight();
      }	

      else if (!isData) {
        if (year == "2018") // temporary hack to use 2017 mc with 2018 data
          evt_weight *= scale1fb_2017(currentFileTitle) * lumi_2018 * weight();
        else if (mYear == "2016")
          evt_weight *= scale1fb_2016(currentFileTitle) * lumi_2016 * weight();
        else if (mYear == "2017") 
          evt_weight *= scale1fb_2017(currentFileTitle) * lumi_2017 * weight();
        else if (mYear == "2018")
          evt_weight *= scale1fb_2017(currentFileTitle) * lumi_2018 * weight();
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

      int recoLeptonId = categorize_reco_leptons(electrons.size(), muons.size());

      double mva_value = -999;

      lep_pt_ = leps[0].Pt();
      lep_eta_ = leps[0].Eta();

      n_lep_loose_ = nElecLoose() + nMuonLoose();
      n_lep_medium_ = nElecMedium() + nMuonMedium();
      n_lep_tight_ = nElecTight() + nMuonTight();

      muon1_mini_iso_ = muon1_pt() > 0 ? muonLeadIso() / muon1_pt() : -999;
      muon2_mini_iso_ = muon2_pt() > 0 ? muonSubleadIso() / muon2_pt() : -999;

      top_tag_score_ = topTag_score();

      maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();

      // Impute, if applicable
      if (bkg_options.Contains("impute")) {
        if (isData)
          impute_photon_and_lepton_id(min_photon_ID_presel_cut, maxIDMVA_, photon_fakeID_shape_runII, minIDMVA_, n_lep_medium_, n_lep_tight_, evt_weight, processId);
      }

      // Scale bkg weight
      evt_weight *= scale_bkg(currentFileTitle, bkg_options, processId, "Leptonic");

      // Blinded region 
      if (isData && processId != 18 && blind && mass() > 120 && mass() < 130)        continue;

      if (bkg_options == "old_vgamma") {
	if (has_ttX_overlap(currentFileTitle, lead_Prompt(), sublead_Prompt()))	continue;
	if (has_simple_qcd_overlap(currentFileTitle, genPhotonId))		continue;
	if (currentFileTitle.Contains("ZGTo2LG") || currentFileTitle.Contains("ZGToLLG"))	continue;
	if (currentFileTitle.Contains("DYJetsToLL") && genPhotonId >= 1)
	  processId = 7; 
      }	
      else
        if (has_std_overlaps(currentFileTitle, lead_Prompt(), sublead_Prompt(), genPhotonId))     continue;

      if (!passes_selection(tag, minIDMVA_, maxIDMVA_, n_lep_medium_, n_lep_tight_)) continue;

      if (currentFileTitle.Contains("WGToLNuG"))
        w_gamma_yield += evt_weight;

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

      dipho_pt_over_mass_ = diphoton.Pt() / mass();
      helicity_angle_ = helicity(lead_photon, sublead_photon);

      if (evaluate_mva) {
        mva_value = convert_tmva_to_prob(mva->EvaluateMVA( "BDT" ));
      }

      if (!passes_selection(tag, minIDMVA_, maxIDMVA_, n_lep_medium_, n_lep_tight_)) continue;

      int mvaCategoryId = mva_value < -0.8 ? 0 : 1;

      vector<int> vId = {genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId, mvaCategoryId, recoLeptonId, yearId}; 

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
      vProcess[processId]->fill_histogram("hDiphotonPtOverMass", diphoton.Pt() / mass(), evt_weight, vId);

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

      vProcess[processId]->fill_histogram("htthMVA_RunII", tthMVA_RunII(), evt_weight, vId);
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

      if (jet_pt1() != -100)      vProcess[processId]->fill_histogram("hJet1BTag", btag_scores[0], evt_weight, vId);
      if (jet_pt2() != -100)      vProcess[processId]->fill_histogram("hJet2BTag", btag_scores[1], evt_weight, vId);
      if (jet_pt3() != -100)      vProcess[processId]->fill_histogram("hJet3BTag", btag_scores[2], evt_weight, vId);
      if (jet_pt4() != -100)      vProcess[processId]->fill_histogram("hJet4BTag", btag_scores[3], evt_weight, vId);

      vProcess[processId]->fill_histogram("hMaxBTag", btag_scores_sorted[0].second, evt_weight, vId);
      vProcess[processId]->fill_histogram("hSecondMaxBTag", btag_scores_sorted[1].second, evt_weight, vId);


      double lep_pt, lep_eta;
      lep_pt = get_lep_pt(lep_eta);
      vProcess[processId]->fill_histogram("hLeptonPt", lep_pt, evt_weight, vId);
      vProcess[processId]->fill_histogram("hLeptonEta", lep_eta, evt_weight, vId);

      if (muon1_pt() > 0)
        vProcess[processId]->fill_histogram("hMuonMiniIsolation", muonLeadIso() / muon1_pt(), evt_weight, vId);
      if (muon2_pt() > 0)
        vProcess[processId]->fill_histogram("hMuonMiniIsolation", muonSubleadIso() / muon2_pt(), evt_weight, vId); 

      vProcess[processId]->fill_histogram("hNLepLoose", n_lep_loose_, evt_weight, vId);
      vProcess[processId]->fill_histogram("hNLepMedium", n_lep_medium_, evt_weight, vId);
      vProcess[processId]->fill_histogram("hNLepTight", n_lep_tight_, evt_weight, vId);

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
      vProcess[processId]->fill_histogram("hPhotonLeadPixelSeed", leadPixelSeed(), evt_weight, vId);

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
      vProcess[processId]->fill_histogram("hPhotonSubleadPixelSeed", subleadPixelSeed(), evt_weight, vId);

      if (sublead_closest_gen_dR() < 999) {
        vProcess[processId]->fill_histogram("hPhotonSubleadPtGen", sublead_closest_gen_Pt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonPtRatio", subleadPt() / sublead_closest_gen_Pt(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonDeltaRGen", sublead_closest_gen_dR(), evt_weight, vId);
      }

      double maxID = maxIDMVA_;
      double minID = minIDMVA_;
      //double maxID = leadIDMVA() >= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      //double minID = leadIDMVA() >= subleadIDMVA() ? subleadIDMVA() : leadIDMVA();

      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA", maxID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_entries", maxID, 1, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_entries", minID, 1, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_coarse", maxID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse_entries", minID, 1, vId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_coarse_entries", maxID, 1, vId);

      if (njets_ >= 5)
        vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_NJets5+", maxID, evt_weight, vId);
      if (njets_ >= 7)
        vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_NJets7+", maxID, evt_weight, vId);

      if (njets_ >= 5)
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_NJets5+", minID, evt_weight, vId);
      if (njets_ >= 7)
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_NJets7+", minID, evt_weight, vId);


      if (nb_medium() == 0) {
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse_0b", minID, evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_coarse_0b", maxID, evt_weight, vId);
      }
      vProcess[processId]->fill_histogram("hDiphoMVA", diphoMVARes(), evt_weight, vId);

      vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_NJets", maxID, n_jets(), evt_weight, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonMinIDMVA_NJets", minID, n_jets(), evt_weight, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_NJets_entries", maxID, n_jets(), 1, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonMinIDMVA_NJets_entries", minID, n_jets(), 1, vId);

      vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_NTightLeps", maxID, nMuonTight() + nElecTight(), evt_weight, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonMinIDMVA_NTightLeps", minID, nMuonTight() + nElecTight(), evt_weight, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_NTightLeps_entries", maxID, nMuonTight() + nElecTight(), 1, vId);
      vProcess[processId]->fill_2D_histogram("hPhotonMinIDMVA_NTightLeps_entries", minID, nMuonTight() + nElecTight(), 1, vId);

      vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_MinIDMVA", maxID, minID, evt_weight, vId);
     
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
 
  //baby->CloseBabyNtuple();
 
  //delete rand_map;

  // Example Histograms
  f1->Write();
  f1->Close(); 

  cout << "W+gamma yield: " << w_gamma_yield << endl; 

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
