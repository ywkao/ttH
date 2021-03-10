#include "MakeMVABabies_ttHHadronic.h"
#include "ScanChain_ttHHadronic.h"

void BabyMaker::ScanChain(TChain* chain, TString tag, TString year, TString ext, TString bkg_options, TString mYear = "", TString idx = "", bool fcnc = false, bool blind = false, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  // Init{{{
  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Make baby ntuple
  MakeBabyNtuple( Form("%s.root", ("MVABaby_ttHHadronic_" + ext + "_" + mYear + idx).Data()));

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
  //}}}

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    // Get File Content, label type of samples, btag norm factor{{{
    TString currentFileTitle = currentFile->GetTitle();
    cout << currentFileTitle << endl;
    TFile file(currentFileTitle);
    TTree *tree;
    if ( (currentFileTitle.Contains("v4.") && !currentFileTitle.Contains("FCNC")) || currentFileTitle.Contains("v5."))
    {
        cout<< "check: tagsDumper/trees/_13TeV_TTHHadronicTag" <<endl;
        tree = (TTree*)file.Get("tagsDumper/trees/_13TeV_TTHHadronicTag");
    }
    else
    {
        cout<< "check: tthHadronicTagDumper/trees/tth_13TeV_all" <<endl;
        tree = (TTree*)file.Get("tthHadronicTagDumper/trees/tth_13TeV_all");
    }
    if (fast) TTreeCache::SetLearnEntries(10);
    if (fast) tree->SetCacheSize(128*1024*1024);
    cms3.Init(tree);

    // Initialize map of evt_run_lumi -> rand
    //RandomMap* rand_map = new RandomMap("Utils/random_map_Hadronic_" + ext + ".txt");
    
    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma"); 
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG") || currentFileTitle.Contains("THQ") || currentFileTitle.Contains("THW") || currentFileTitle.Contains("VBF") || currentFileTitle.Contains("GluGluHToGG") || currentFileTitle.Contains("VHToGG") || currentFileTitle.Contains("FCNC"); 

    if (mYear == "")
      mYear = (currentFileTitle.Contains("Run2016") || currentFileTitle.Contains("RunIISummer16")) ? "2016" : ((currentFileTitle.Contains("Run2017") || currentFileTitle.Contains("RunIIFall17")) ? "2017" : ((currentFileTitle.Contains("Run2018") || currentFileTitle.Contains("RunIIAutumn18")) ? "2018" : "no_year"));

    if (is_wrong_tt_jets_sample(currentFileTitle, "Hadronic"))                        continue;
    if (bkg_options.Contains("impute") && (currentFileTitle.Contains("GJets_HT") || currentFileTitle.Contains("QCD"))) {
      cout << "Skipping this sample: " << currentFileTitle << ", since we are imputing." << endl;
      continue;
    }

    cout << "mYear: " << mYear << endl;
    int yearId = mYear == "2016" ? 0 : (mYear == "2017" ? 1 : (mYear == "2018" ? 2 : -1));
    set_json(mYear);

    // btag norm factor
    unsigned int nEventsTree = tree->GetEntriesFast();

    double btag_norm_correction = 1.;

    if (!isData && btag_norm_correction == 1.) {
        double integral_no_btag = 0.;
        double integral_w_btag =  0.;
        for (unsigned int event = 0; event < nEventsTree; ++event) {
            if (fast) tree->LoadTree(event);
            cms3.GetEntry(event);
            ++nEventsTotal;

            ttHHadronic::progress( nEventsTotal, nEventsChain );

            if (!passes_btag_rescale_selection())       continue;

            //double weight_no_btag = weight() / weight_JetBTagWeight();

            //if (!(isnan(weight_no_btag) || isinf(weight_no_btag))) {
            //    integral_no_btag += weight_no_btag;
            //    integral_w_btag += weight();
            //}
        }
        btag_norm_correction = integral_no_btag / integral_w_btag;
        cout << "btag_normalization_factor: " << btag_norm_correction << endl;
    }
    nEventsTotal = 0;
    //}}}
    
    // Loop over Events in current file
    for (unsigned int event = 0; event < nEventsTree; ++event) {
      // Get Event Content, label type of samples, decide evt weight{{{
      if (nEventsTotal >= nEventsChain) continue;
      if (fast) tree->LoadTree(event);
      cms3.GetEntry(event);
      ++nEventsTotal;

      // Progress
      ttHHadronic::progress( nEventsTotal, nEventsChain );
      InitBabyNtuple();

      // Check golden json, blind signal region, skip low stat file
      if (isData) if (!pass_json(mYear, cms3.run(), cms3.lumi())) continue;
      if (isData && blind && mass() > 120 && mass() < 130) continue;
      if (is_low_stats_process(currentFileTitle)) continue;

      // Decide what type of sample this is
      int genPhotonId = categorize_photons(leadGenMatch(), subleadGenMatch());
      process_id_ = categorize_process(currentFileTitle, genPhotonId);

      // Decide evt wight
      evt_weight_ = 1.;
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

      //pu weighting
      bool pu_weight = true;
      if (pu_weight) evt_weight_ *= puweight();

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
      evt_weight_ *= scale_bkg(currentFileTitle, bkg_options, process_id_, "Hadronic", fcnc);

      // Scale FCNC to current best observed limit (ATLAS 2016 combination)
      if (currentFileTitle.Contains("FCNC"))
        evt_weight_ *= scale_fcnc(currentFileTitle);

      if (has_std_overlaps(currentFileTitle, lead_Prompt(), sublead_Prompt(), genPhotonId)) continue;
      if (!passes_selection(tag, minIDMVA_, maxIDMVA_)) continue;

      if (isnan(evt_weight_) || isinf(evt_weight_) || evt_weight_ == 0) {
        continue; //some pu weights are nan/inf and this causes problems for histos 
      }

      if (fcnc) 
	    label_ = (isData && process_id_ != 18) ? 2 : (process_id_ == 22 || process_id_ == 23 || process_id_ == 24 || process_id_ == 25) ? 1 : 0; // 0 = bkg, 1 = fcnc, 2 = data
      else { 
	    label_ = (isData && process_id_ != 18) ? 2 : (process_id_ == 0 ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data sidebands
	    if (process_id_ == 22 || process_id_ == 23 || process_id_ == 24 || process_id_ == 25)
          label_ = -1; // don't use FCNC as a bkg when ttH is signal
      }
      multi_label_ = multiclassifier_label(currentFileTitle, genPhotonId, fcnc);
      signal_mass_label_ = categorize_signal_sample(currentFileTitle);
      signal_mass_category_ = categorize_signal_mass_label(currentFileTitle);

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
      //}}}
      //------------------------------ Variable definitions ------------------------------//
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
	  top_tag_mass_  = -1;
      top_tag_pt_    = -1;
      top_tag_eta_   = -1;
      top_tag_phi_   = -1;

      year_ = mYear == "2016" ? 2016 : (mYear == "2017" ? 2017 : (mYear == "2018" ? 2018 : -1)); 

      evt_ = cms3.event();
      run_ = cms3.run();
      lumi_ = cms3.lumi();

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
      jet7_pt_   = njets_ >= 7 ? jets[6].Pt()   : -999;
      jet7_eta_  = njets_ >= 7 ? jets[6].Eta()  : -999;
      jet7_btag_ = njets_ >= 7 ? btag_scores[6] : -999;
      jet8_pt_   = njets_ >= 8 ? jets[7].Pt()   : -999;
      jet8_eta_  = njets_ >= 8 ? jets[7].Eta()  : -999;
      jet8_btag_ = njets_ >= 8 ? btag_scores[7] : -999; 

      jet1_phi_     = njets_ >= 1 ? jets[0].Phi() : -999;
      jet1_energy_  = njets_ >= 1 ? jets[0].E()   : -999;
      jet2_phi_     = njets_ >= 2 ? jets[1].Phi() : -999;
      jet2_energy_  = njets_ >= 2 ? jets[1].E()   : -999;
      jet3_phi_     = njets_ >= 3 ? jets[2].Phi() : -999;
      jet3_energy_  = njets_ >= 3 ? jets[2].E()   : -999;
      jet4_phi_     = njets_ >= 4 ? jets[3].Phi() : -999;
      jet4_energy_  = njets_ >= 4 ? jets[3].E()   : -999;
      jet5_phi_     = njets_ >= 5 ? jets[4].Phi() : -999;
      jet5_energy_  = njets_ >= 5 ? jets[4].E()   : -999;
      jet6_phi_     = njets_ >= 6 ? jets[5].Phi() : -999;
      jet6_energy_  = njets_ >= 6 ? jets[5].E()   : -999;
      jet7_phi_     = njets_ >= 7 ? jets[6].Phi() : -999;
      jet7_energy_  = njets_ >= 7 ? jets[6].E()   : -999;
      jet8_phi_     = njets_ >= 8 ? jets[7].Phi() : -999;
      jet8_energy_  = njets_ >= 8 ? jets[7].E()   : -999;

      // photons, diphoton, helicity angle, top masses
      lead_pT_      = leadPt();
      sublead_pT_   = subleadPt();
      leadptoM_     = lead_ptoM();
      subleadptoM_  = sublead_ptoM();
      leadIDMVA_    = leadID_;
      subleadIDMVA_ = subleadID_;
      lead_eta_     = leadEta();
      sublead_eta_  = subleadEta();
      lead_phi_     = leadPhi();
      sublead_phi_  = subleadPhi();

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

      calculate_masses(diphoton, jets, m_ggj_, m_jjj_);
      m_ggj_ = m_ggj_ > 0 ? log(m_ggj_) : -9;
      m_jjj_ = m_jjj_ > 0 ? log(m_jjj_) : -9;

      //------------------------------ Minimum chi-2 method (cov.) ------------------------------//
      TString json_file = "tprimetH/json/covMatrix_Era2017_M1000.json";
      double min_chi2_value_2x2 = 99999.;
      vector<int> indices_bjj_covMatrix(3, -1);
      bool has_resonable_reco = get_the_best_bjj_candidate(indices_bjj_covMatrix, jets, diphoton, btag_scores, min_chi2_value_2x2, json_file);

      TLorentzVector null;
      TLorentzVector cov_bjet   = has_resonable_reco ? jets[indices_bjj_covMatrix[0]] : null;
      TLorentzVector cov_wjet1  = has_resonable_reco ? jets[indices_bjj_covMatrix[1]] : null;
      TLorentzVector cov_wjet2  = has_resonable_reco ? jets[indices_bjj_covMatrix[2]] : null;
      TLorentzVector cov_wboson = cov_wjet1 + cov_wjet2;
      TLorentzVector cov_top    = cov_bjet + cov_wboson;
      TLorentzVector cov_tprime = cov_top + diphoton;

      bool pass_eta_criteria_on_wjets = ( cov_wjet1.Eta() < 3. && cov_wjet2.Eta() < 3. );
      //if( !passes_btag_rescale_selection ) continue;

      double mass_wboson = has_resonable_reco ? cov_wboson.M()     : -1.;
      double mass_top    = has_resonable_reco ? cov_top.M()        : -1.;
      double mass_tprime = has_resonable_reco ? cov_tprime.M()     : -1.;
      min_chi2_value_2x2 = has_resonable_reco ? min_chi2_value_2x2 : -1.;

      chi2_value_               = min_chi2_value_2x2;
      chi2_tbw_mass_            = (has_resonable_reco && pass_eta_criteria_on_wjets) ? cov_top.M()                           : -999;
      chi2_tbw_pt_              = (has_resonable_reco && pass_eta_criteria_on_wjets) ? cov_top.Pt()                          : -999;
      chi2_tbw_eta_             = (has_resonable_reco && pass_eta_criteria_on_wjets) ? cov_top.Eta()                         : -999;
      chi2_tbw_deltaR_dipho_    = (has_resonable_reco && pass_eta_criteria_on_wjets) ? cov_top.DeltaR(diphoton)              : -999;
      chi2_tprime_ptOverM_      = (has_resonable_reco && pass_eta_criteria_on_wjets) ? cov_tprime.Pt()/cov_tprime.M()        : -999;
      chi2_tprime_eta_          = (has_resonable_reco && pass_eta_criteria_on_wjets) ? cov_tprime.Eta()                      : -999;
      chi2_tprime_deltaR_tbw_   = (has_resonable_reco && pass_eta_criteria_on_wjets) ? cov_tprime.DeltaR(cov_top)            : -999;
      chi2_tprime_deltaR_dipho_ = (has_resonable_reco && pass_eta_criteria_on_wjets) ? cov_tprime.DeltaR(diphoton)           : -999;
      chi2_bjet_btagScores_     = (has_resonable_reco && pass_eta_criteria_on_wjets) ? btag_scores[indices_bjj_covMatrix[0]] : -999;
      chi2_wjet1_btagScores_    = (has_resonable_reco && pass_eta_criteria_on_wjets) ? btag_scores[indices_bjj_covMatrix[1]] : -999;
      chi2_wjet2_btagScores_    = (has_resonable_reco && pass_eta_criteria_on_wjets) ? btag_scores[indices_bjj_covMatrix[2]] : -999;
      tprime_pt_ratio_          = (has_resonable_reco && pass_eta_criteria_on_wjets) ? (cov_top.Pt() + diphoton.Pt())/ ht_   : -999;

      //----------------------------------------------------------------------------------------------------//

      rand_ = cms3.rand();
      super_rand_ = -1; //rand_map->retrieve_rand(cms3.event(), cms3.run(), cms3.lumi());
      mass_ = mass();
      lead_sigmaEtoE_ = lead_sigmaEoE();
      sublead_sigmaEtoE_ = sublead_sigmaEoE();
      // ***DNN Business{{{
      vector<vector<float>> unordered_objects;
    
      vector<vector<float>> jet_objects = make_jet_objects(year, diphoton, false); // don't boost jet p4 to higgs ref. frame
      vector<vector<float>> jet_objects_boosted = make_jet_objects(year, diphoton, true); // yes boost

      objects_ = sort_objects(jet_objects);
      objects_boosted_ = sort_objects(jet_objects_boosted);

      top_candidates_ = calculate_top_candidates(diphoton, jets, btag_scores, max1_btag_);

      // Do this a dumb hacky way because it is easier to deal with down the line in DNN/BDT prep+training
      top_candidates_1_ = top_candidates_[0] > 0 ? log(top_candidates_[0]) : -9;
      top_candidates_2_ = top_candidates_[1] > 0 ? log(top_candidates_[1]) : -9;
      top_candidates_3_ = top_candidates_[2] > 0 ? log(top_candidates_[2]) : -9;
      top_candidates_4_ = top_candidates_[3] > 0 ? log(top_candidates_[3]) : -9;
      top_candidates_5_ = top_candidates_[4] > 0 ? log(top_candidates_[4]) : -9;
      top_candidates_6_ = top_candidates_[5] > 0 ? log(top_candidates_[5]) : -9;
      top_candidates_7_ = top_candidates_[6] > 0 ? log(top_candidates_[6]) : -9;
      top_candidates_8_ = top_candidates_[7] > 0 ? log(top_candidates_[7]) : -9;
      top_candidates_9_ = top_candidates_[8] > 0 ? log(top_candidates_[8]) : -9;
      top_candidates_10_ = top_candidates_[9] > 0 ? log(top_candidates_[9]) : -9;
      top_candidates_11_ = top_candidates_[10] > 0 ? log(top_candidates_[10]) : -9;
      top_candidates_12_ = top_candidates_[11] > 0 ? log(top_candidates_[11]) : -9;
      //}}}
//      // chi-2 and DeepCSV scores{{{
//      vector<double> btag_scores_;
//      vector<double> bbtag_scores_;
//      vector<double> ctag_scores_;
//      vector<double> udsgtag_scores_;
//      for(std::size_t i=0; i!=jet_objects.size(); ++i)
//      {
//          btag_scores_.push_back(jet_objects[i][4]);
//          bbtag_scores_.push_back(jet_objects[i][5]);
//          ctag_scores_.push_back(jet_objects[i][6]);
//          udsgtag_scores_.push_back(jet_objects[i][7]);
//      }
//      chi2_bjet_ctagScores_         = is_moreThanTwoJets_and_atLeastOneBjet   ? ctag_scores_[index_jet_chi2_modified[0]] : -999;
//      chi2_wjet1_ctagScores_        = is_moreThanTwoJets_and_atLeastOneBjet   ? ctag_scores_[index_jet_chi2_modified[1]] : -999;
//      chi2_wjet2_ctagScores_        = is_moreThanTwoJets_and_atLeastOneBjet   ? ctag_scores_[index_jet_chi2_modified[2]] : -999;
//      chi2_qjet_ctagScores_         = is_moreThanThreeJets_and_atLeastOneBjet ? ctag_scores_[index_jet_chi2_modified[3]] : -999;
//
//      chi2_bjet_udsgtagScores_      = is_moreThanTwoJets_and_atLeastOneBjet   ? udsgtag_scores_[index_jet_chi2_modified[0]] : -999;
//      chi2_wjet1_udsgtagScores_     = is_moreThanTwoJets_and_atLeastOneBjet   ? udsgtag_scores_[index_jet_chi2_modified[1]] : -999;
//      chi2_wjet2_udsgtagScores_     = is_moreThanTwoJets_and_atLeastOneBjet   ? udsgtag_scores_[index_jet_chi2_modified[2]] : -999;
//      chi2_qjet_udsgtagScores_      = is_moreThanThreeJets_and_atLeastOneBjet ? udsgtag_scores_[index_jet_chi2_modified[3]] : -999;
//
//      chi2_bjet_CvsL_  = is_moreThanTwoJets_and_atLeastOneBjet   ? calculate_CvsL(ctag_scores_[index_jet_chi2_modified[0]], udsgtag_scores_[index_jet_chi2_modified[0]]) : -999;
//      chi2_wjet1_CvsL_ = is_moreThanTwoJets_and_atLeastOneBjet   ? calculate_CvsL(ctag_scores_[index_jet_chi2_modified[1]], udsgtag_scores_[index_jet_chi2_modified[1]]) : -999;
//      chi2_wjet2_CvsL_ = is_moreThanTwoJets_and_atLeastOneBjet   ? calculate_CvsL(ctag_scores_[index_jet_chi2_modified[2]], udsgtag_scores_[index_jet_chi2_modified[2]]) : -999;
//      chi2_qjet_CvsL_  = is_moreThanThreeJets_and_atLeastOneBjet ? calculate_CvsL(ctag_scores_[index_jet_chi2_modified[3]], udsgtag_scores_[index_jet_chi2_modified[3]]) : -999;
//
//      chi2_bjet_CvsB_  = is_moreThanTwoJets_and_atLeastOneBjet   ? calculate_CvsB(ctag_scores_[index_jet_chi2_modified[0]], btag_scores_[index_jet_chi2_modified[0]], bbtag_scores_[index_jet_chi2_modified[0]]) : -999;
//      chi2_wjet1_CvsB_ = is_moreThanTwoJets_and_atLeastOneBjet   ? calculate_CvsB(ctag_scores_[index_jet_chi2_modified[1]], btag_scores_[index_jet_chi2_modified[1]], bbtag_scores_[index_jet_chi2_modified[1]]) : -999;
//      chi2_wjet2_CvsB_ = is_moreThanTwoJets_and_atLeastOneBjet   ? calculate_CvsB(ctag_scores_[index_jet_chi2_modified[2]], btag_scores_[index_jet_chi2_modified[2]], bbtag_scores_[index_jet_chi2_modified[2]]) : -999;
//      chi2_qjet_CvsB_  = is_moreThanThreeJets_and_atLeastOneBjet ? calculate_CvsB(ctag_scores_[index_jet_chi2_modified[3]], btag_scores_[index_jet_chi2_modified[3]], bbtag_scores_[index_jet_chi2_modified[3]]) : -999;
//      //}}}

      const float oversample_ggh = 81.;
      const float oversample_tth = 1.;
      float oversample;
      if (fcnc) {
        if (process_id_ == 14)
          oversample = oversample_ggh;
        else if (process_id_ == 0)
          oversample = oversample_tth;
        else
          oversample = 1.;
        evt_weight_ *= 1./oversample;
        for (int i = 0; i < int(oversample); i++)
          FillBabyNtuple();
      }

      else
        FillBabyNtuple();
    }// end of event loop
    delete tree;
    file.Close();
  }// end of while loop

  if (nEventsChain != nEventsTotal) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }

  CloseBabyNtuple();
 
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
