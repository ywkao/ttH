#include "ScanChain_ttHHadronic.h"

int ScanChain(TChain* chain, TString tag, TString year, TString ext, TString xml_file, TString bkg_options, TString mYear = "", TString idx = "", bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag + "_" + ext + "_histograms" + year + idx + ".root", "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  bool evaluate_mva = xml_file != "none";

  bool combine_gjets_samples = false; //tag != "ttHHadronic_GJet_Reweight_Preselection"; 
  //bool scale_gjets_normalization = !(tag.Contains("GJet_Reweight_Preselection"));
  bool scale_gjets_normalization = false; //true;

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

  TF1* gjet_minID_shape = get_photon_ID_shape("min");
  TF1* gjet_maxID_shape = get_photon_ID_shape("max");
  TF1* gjet_leadID_shape = get_photon_ID_shape("lead");
  TF1* gjet_subleadID_shape = get_photon_ID_shape("sublead");
  TF1* photon_fakeID_shape = get_photon_ID_shape("fake");
  TF1* photon_fakeID_shape_runII = get_photon_ID_shape("fake_runII");

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

  float m1_;
  float m2_;
  
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

    //mva->AddVariable("top_tag_score_", &top_tag_score_);
    mva->AddVariable("dipho_pt_over_mass_", &dipho_pt_over_mass_);

    mva->AddVariable("helicity_angle_", &helicity_angle_);

    if (xml_file.Contains("FCNC")) {
      mva->AddVariable("m_ggj_", &m1_);
      mva->AddVariable("m_jjj_", &m2_);
    }

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

  //gjet_mva->BookMVA("BDT", gjet_bdt_file); 


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
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG") || currentFileTitle.Contains("THQ") || currentFileTitle.Contains("THW") || currentFileTitle.Contains("VBF") || currentFileTitle.Contains("GluGluHToGG") || currentFileTitle.Contains("VHToGG") || currentFileTitle.Contains("FCNC"); 

    if (isSignal) {
      if (categorize_signal_sample(currentFileTitle) != 0)
        continue;
    }
    
    if (mYear == "")
      mYear = (currentFileTitle.Contains("Run2016") || currentFileTitle.Contains("RunIISummer16")) ? "2016" : ((currentFileTitle.Contains("Run2017") || currentFileTitle.Contains("RunIIFall17")) ? "2017" : ((currentFileTitle.Contains("Run2018") || currentFileTitle.Contains("RunIIAutumn18")) ? "2018" : "no_year"));

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
      if (processId == 17)
        processId = 3; // use Madgraph GJets instead of Pythia

      int genLeptonId = isData ? -1 : categorize_leptons(nGoodEls(), nGoodMus());
      int genPhotonDetailId = isData ? -1 : categorize_photons_detail(lead_photon_type(), sublead_photon_type());
      int photonLocationId = categorize_photon_locations(leadEta(), subleadEta());

      float evt_weight = 1.;
     
      int label = isData ? 2 : (isSignal ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data

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

      // Impute, if applicable
      maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      if (bkg_options.Contains("impute")) {
        if (isData)
          impute_photon_id(min_photon_ID_presel_cut, maxIDMVA_, photon_fakeID_shape_runII, minIDMVA_, evt_weight, processId);
      }

      // Scale bkg weight
      evt_weight *= scale_bkg(currentFileTitle, bkg_options, processId, "Hadronic");

      double leadID_ = leadIDMVA() == maxIDMVA_ ? maxIDMVA_ : minIDMVA_;
      double subleadID_ = leadIDMVA() == maxIDMVA_ ? minIDMVA_ : maxIDMVA_; 

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

      double mva_value = -999;

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

      calculate_masses(diphoton, jets, m1_, m2_);

      if (evaluate_mva) 
        mva_value = convert_tmva_to_prob(mva->EvaluateMVA( "BDT" ));
      double reference_mva = tthMVA();
      double super_rand = -999;

      int mvaCategoryId = mva_value < -0.8 ? 0 : 1;
      vector<int> vId = {genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId, mvaCategoryId, -1, yearId};


      // Gamma + jets reweighting
      double gjet_mva_value = -999;	

      //gjet_mva_value = convert_tmva_to_prob(gjet_mva->EvaluateMVA( "BDT" )); 

      /* turn this off for now
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
      */

      if (isnan(evt_weight) || isinf(evt_weight) || evt_weight == 0) {
        continue; //some pu weights are nan/inf and this causes problems for histos 
      }


      if (has_std_overlaps(currentFileTitle, lead_Prompt(), sublead_Prompt(), genPhotonId))     continue;
      if (!passes_selection(tag, minIDMVA_, maxIDMVA_))	continue;

      // Fill histograms //
      vProcess[processId]->fill_histogram("hGJet_BDT", gjet_mva_value, evt_weight, vId);
      vProcess[processId]->fill_histogram("hMass", mass(), evt_weight, vId);   
      vProcess[processId]->fill_histogram("hMassAN", mass(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hMassTop1", m1_, evt_weight, vId);   
      vProcess[processId]->fill_histogram("hMassTop2", m2_, evt_weight, vId);   
      
      if (leadptoM_ > 0.33 && subleadptoM_ > 0.25) {
	vProcess[processId]->fill_histogram("hMass_PassPtToM", mass(), evt_weight, vId);
	if (mva_value > 0.9)
	  vProcess[processId]->fill_histogram("hMass_PassPtToM_AfterBDTCut", mass(), evt_weight, vId);
      }
      else {
	vProcess[processId]->fill_histogram("hMass_FailPtToM", mass(), evt_weight, vId);
	if (mva_value > 0.9)
	  vProcess[processId]->fill_histogram("hMass_FailPtToM_AfterBDTCut", mass(), evt_weight, vId);
      }

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
      if (njets_ >= 5)
	vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_NJets5+", maxID, evt_weight, vId);
      if (njets_ >= 7)
        vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_NJets7+", maxID, evt_weight, vId);

      vProcess[processId]->fill_histogram("hPhotonMinIDMVA", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_fine", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_entries", minID, 1, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_fine_entries", minID, 1, vId);
      if (njets_ >= 5)
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_NJets5+", minID, evt_weight, vId);
      if (njets_ >= 7)
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_NJets7+", minID, evt_weight, vId);



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

      // Checks for p_T/mGG cuts
      if (mass() > 100 && mass() < 180) {
	vProcess[processId]->fill_2D_histogram("hBDT_LeadPtoM", mva_value, leadptoM_, evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hBDT_SubleadPtoM", mva_value, subleadptoM_, evt_weight, vId); 
	vProcess[processId]->fill_2D_histogram("hMass_LeadPtoM", mass(), leadptoM_, evt_weight, vId);
	vProcess[processId]->fill_2D_histogram("hMass_SubleadPtoM", mass(), subleadptoM_, evt_weight, vId);
	if (mva_value > 0.9) {
	  vProcess[processId]->fill_2D_histogram("hMass_LeadPtoM_afterBDTCut", mass(), leadptoM_, evt_weight, vId);
	  vProcess[processId]->fill_2D_histogram("hMass_SubleadPtoM_afterBDTCut", mass(), subleadptoM_, evt_weight, vId);
	}
      }

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
      vProcess[processId]->fill_histogram("htthMVA_RunII", tthMVA_RunII(), evt_weight, vId);

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
