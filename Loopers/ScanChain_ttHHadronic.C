#include "ScanChain_ttHHadronic.h"

int ScanChain(TChain* chain, TString tag, TString year, TString ext, TString xml_file, TString bkg_options, bool doSyst = false, TString l1_prefire = "", TString mYear = "", TString idx = "", bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
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
  vector<TString> syst_labels = {};
  if (doSyst) {
    for (unsigned int i = 0; i < independent_systematic_collections.size(); i++) {
      syst_labels.push_back(independent_systematic_collections[i]);
    }
    for (unsigned int i = 0; i < weight_systematics.size(); i++) {
      syst_labels.push_back(weight_systematics[i]);
    }
  }
  if (doSyst)
    add_variables(vProcess, tag, syst_labels); // defined in ttHLooper
  else
    add_variables(vProcess, tag);

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if (nEvents >= 0) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

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

    if (xml_file.Contains("FCNC") || xml_file.Contains("hut") || xml_file.Contains("hct")) {
      mva->AddVariable("m_ggj_", &m1_);
      mva->AddVariable("m_jjj_", &m2_);
    }

    mva->BookMVA("BDT", xml_file);
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
    
    /*
    TTree *tree;
    if (currentFileTitle.Contains("v4."))
        tree = (TTree*)file.Get("tagsDumper/trees/_13TeV_TTHHadronicTag");
    else
        tree = (TTree*)file.Get("tthHadronicTagDumper/trees/tth_13TeV_all");
    if (fast) TTreeCache::SetLearnEntries(10);
    if (fast) tree->SetCacheSize(128*1024*1024);
    cms3.Init(tree);
    */

    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG") || currentFileTitle.Contains("EGamma"); 
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG") || currentFileTitle.Contains("THQ") || currentFileTitle.Contains("THW") || currentFileTitle.Contains("VBF") || currentFileTitle.Contains("GluGluHToGG") || currentFileTitle.Contains("VHToGG") || currentFileTitle.Contains("FCNC"); 

    if (isSignal) {
      if (categorize_signal_sample(currentFileTitle) != 0 && !currentFileTitle.Contains("FCNC"))
        continue;
      if ((currentFileTitle.Contains("M120") || currentFileTitle.Contains("M130")) && !currentFileTitle.Contains("FCNC"))
        continue;
    }
    
    if (mYear == "")
      mYear = (currentFileTitle.Contains("Run2016") || currentFileTitle.Contains("RunIISummer16")) ? "2016" : ((currentFileTitle.Contains("Run2017") || currentFileTitle.Contains("RunIIFall17")) ? "2017" : ((currentFileTitle.Contains("Run2018") || currentFileTitle.Contains("RunIIAutumn18")) ? "2018" : "no_year"));

    if (is_wrong_tt_jets_sample(currentFileTitle, "Hadronic"))                        continue;
    if (bkg_options.Contains("impute") && (currentFileTitle.Contains("GJets_HT") || currentFileTitle.Contains("QCD"))) {
      cout << "Skipping this sample: " << currentFileTitle << ", since we are imputing." << endl;
      continue;
    }

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

    double btag_norm_correction = 1.;

    // Loop over each tree
    for (unsigned int i = 0; i < independent_systematic_collections.size(); i++) {
        if ((isData || !doSyst) && i >= 1)
            continue;

        TString syst_ext = independent_systematic_collections[i];
        std::vector<TString> weight_systs = {""};
        if (syst_ext == "" && doSyst && !isData) {
            for (unsigned int j = 0; j < weight_systematics.size(); j++)
                weight_systs.push_back(weight_systematics[j]);
        }

        if (syst_ext != "")
            syst_ext = "_" + syst_ext;

        // Loop over each syst weight
        for (unsigned int j = 0; j < weight_systs.size(); j++) {
            syst_ext = independent_systematic_collections[i];
            if (syst_ext != "")
                syst_ext = "_" + syst_ext;
            cout << "Grabbing this tree: " << "tagsDumper/trees/_13TeV_TTHHadronicTag" + syst_ext << endl;
            TTree *tree; 
            if (currentFileTitle.Contains("v4.") && !currentFileTitle.Contains("FCNC"))
                tree = (TTree*)file.Get("tagsDumper/trees/_13TeV_TTHHadronicTag" + syst_ext);
            else
                tree = (TTree*)file.Get("tthHadronicTagDumper/trees/tth_13TeV_all");

            if (fast) tree->SetCacheSize(128*1024*1024);
            cms3.Init(tree);

            syst_ext = independent_systematic_collections[i] + weight_systs[j];

            cout << "Looping over syst tree " << i << ", weight syst " << j << ", with syst label: " << syst_ext << endl;

            // Loop over Events in current file
            //if (nEventsTotal >= nEventsChain) continue;
            unsigned int nEventsTree = tree->GetEntriesFast();
            nEventsChain = nEventsTree;
            nEventsTotal = 0;
   
            /*
            if (!isData && btag_norm_correction == 1.) {
                double integral_no_btag = 0.;
                double integral_w_btag =  0.;
                for (unsigned int event = 0; event < nEventsTree; ++event) {
                    if (fast) tree->LoadTree(event);
                    cms3.GetEntry(event);
                    ++nEventsTotal;

                    ttHHadronic::progress( nEventsTotal, nEventsChain );

                    if (!passes_btag_rescale_selection())       continue;

                    double weight_no_btag = weight() / weight_JetBTagWeight();

                    if (!(isnan(weight_no_btag) || isinf(weight_no_btag))) {
                        integral_no_btag += weight_no_btag;
                        integral_w_btag += weight();
                    }
                }
                btag_norm_correction = integral_no_btag / integral_w_btag;
                cout << "btag_normalization_factor: " << btag_norm_correction << endl;
            }
            */

            nEventsTotal = 0;

            for (unsigned int event = 0; event < nEventsTree; ++event) {

              // Get Event Content
              //if (nEventsTotal >= nEventsChain) continue;
              if (fast) tree->LoadTree(event);
              cms3.GetEntry(event);
              ++nEventsTotal;

              // Progress
              ttHHadronic::progress( nEventsTotal, nEventsChain );

              // Check golden json
              if (isData) {
                if (!pass_json(mYear, cms3.run(), cms3.lumi()))            continue;
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
              evt_weight *= btag_norm_correction;

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

              if (l1_prefire != "" && !isData && mYear != "2018") {
                //cout << "Prefire prob: " << prefireProbability() << endl;
                //cout << "Undoing L1 prefire prob: " << 1. / (1. - prefireProbability()) << endl;
                //cout << "Varying L1 prefire up: "   << (1. - (prefireProbability()*1.2)) << endl;
                //cout << "Varying L1 prefire down: " << (1. - (prefireProbability()*0.8)) << endl;
                evt_weight *= 1. / (1. - prefireProbability()); // undo
                if (l1_prefire == "up")
                    evt_weight *= (1. - (prefireProbability()*1.2));
                else if (l1_prefire == "down")
                    evt_weight *= (1. - (prefireProbability()*0.8));
              }

              // Impute, if applicable
              maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
              minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
              if (bkg_options.Contains("impute")) {
                if (isData)
                  impute_photon_id(min_photon_ID_presel_cut, maxIDMVA_, photon_fakeID_shape_runII, minIDMVA_, evt_weight, processId);
              }

              // Blinded region
              if (isData && processId != 18 && blind && mass() > 120 && mass() < 130)  continue;

              // Scale bkg weight
              evt_weight *= scale_bkg(currentFileTitle, bkg_options, processId, "Hadronic");
            
              // Scale FCNC to current best observed limit (ATLAS 2016 combination)
              //if (currentFileTitle.Contains("FCNC"))
              //  evt_weight *= scale_fcnc(currentFileTitle);
              if (currentFileTitle.Contains("FCNC"))
                evt_weight *= scale_fcnc_to_atlas_limit(currentFileTitle);


              // Modify weight for systematics affecting weights
              if (j >= 1) {
                double nominal_weight, up_variation, down_variation;
                if (syst_ext.Contains("UnmatchedPUWeight")) {
                  nominal_weight = (UnmatchedPUWeightUp01sigma() + UnmatchedPUWeightDown01sigma())/2.;
                  up_variation = UnmatchedPUWeightUp01sigma() / nominal_weight;
                  down_variation = UnmatchedPUWeightDown01sigma() / nominal_weight;
                }
                else if (syst_ext.Contains("MvaLinearSyst")) {
                  nominal_weight = (MvaLinearSystUp01sigma() + MvaLinearSystDown01sigma())/2.;
                  up_variation = MvaLinearSystUp01sigma() / nominal_weight;
                  down_variation = MvaLinearSystDown01sigma() / nominal_weight;
                }
                else if (syst_ext.Contains("LooseMvaSF")) {
                  nominal_weight = (LooseMvaSFUp01sigma() + LooseMvaSFDown01sigma())/2.;
                  up_variation = LooseMvaSFUp01sigma() / nominal_weight;
                  down_variation = LooseMvaSFDown01sigma() / nominal_weight;
                }
                else if (syst_ext.Contains("PreselSF")) {
                  nominal_weight = (PreselSFUp01sigma() + PreselSFDown01sigma())/2.;
                  up_variation = PreselSFUp01sigma() / nominal_weight;
                  down_variation = PreselSFDown01sigma() / nominal_weight;
                }
                else if (syst_ext.Contains("electronVetoSF")) {
                  nominal_weight = (electronVetoSFUp01sigma() + electronVetoSFDown01sigma())/2.;
                  up_variation = electronVetoSFUp01sigma() / nominal_weight;
                  down_variation = electronVetoSFDown01sigma() / nominal_weight;
                }
                else if (syst_ext.Contains("TriggerWeight")) {
                  nominal_weight = (TriggerWeightUp01sigma() + TriggerWeightDown01sigma())/2.;
                  up_variation = TriggerWeightUp01sigma() / nominal_weight;
                  down_variation = TriggerWeightDown01sigma() / nominal_weight;
                }
                else if (syst_ext.Contains("FracRVWeight")) {
                  nominal_weight = (FracRVWeightUp01sigma() + FracRVWeightDown01sigma())/2.;
                  up_variation = FracRVWeightUp01sigma() / nominal_weight;
                  down_variation = FracRVWeightDown01sigma() / nominal_weight;
                }
                else if (syst_ext.Contains("ElectronWeight")) {
                  nominal_weight = (ElectronWeightUp01sigma() + ElectronWeightDown01sigma())/2.;
                  up_variation = ElectronWeightUp01sigma() / nominal_weight;
                  down_variation = ElectronWeightDown01sigma() / nominal_weight;
                }
                else if (syst_ext.Contains("JetBTagCutWeight")) {
                  nominal_weight = (JetBTagCutWeightUp01sigma() + JetBTagCutWeightDown01sigma())/2.;
                  up_variation = JetBTagCutWeightUp01sigma() / nominal_weight;
                  down_variation = JetBTagCutWeightDown01sigma() / nominal_weight;
                }
                else if (syst_ext.Contains("JetBTagReshapeWeight")) {
                  nominal_weight = (JetBTagReshapeWeightUp01sigma() + JetBTagReshapeWeightDown01sigma())/2.;
                  up_variation = JetBTagReshapeWeightUp01sigma() / nominal_weight;
                  down_variation = JetBTagReshapeWeightDown01sigma() / nominal_weight;
                }

                if (syst_ext.Contains("Up01sigma"))
                    evt_weight *= up_variation; 
                else if (syst_ext.Contains("Down01sigma"))
                    evt_weight *= down_variation; 
              }

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

              vector<float> top_candidates = calculate_top_candidates(diphoton, jets, btag_scores, max1_btag_);

              if (evaluate_mva) 
                mva_value = convert_tmva_to_prob(mva->EvaluateMVA( "BDT" ));
              double reference_mva = tthMVA();
              double super_rand = -999;

              int mvaCategoryId = mva_value < -0.8 ? 0 : 1;
              vector<int> vId = {genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId, mvaCategoryId, -1, yearId};


              // Gamma + jets reweighting
              double gjet_mva_value = -999;	

              if (isnan(evt_weight) || isinf(evt_weight) || evt_weight == 0) {
                continue; //some pu weights are nan/inf and this causes problems for histos 
              }


              if (has_std_overlaps(currentFileTitle, lead_Prompt(), sublead_Prompt(), genPhotonId))     continue;
              if (!passes_selection(tag, minIDMVA_, maxIDMVA_, mva_value))	continue;

              // Fill histograms //
              //vProcess[processId]->fill_histogram("h" + syst_ext + "GJet_BDT", gjet_mva_value, evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "Mass", mass(), evt_weight, vId);   
              vProcess[processId]->fill_histogram("h" + syst_ext + "Mass_v2", mass(), evt_weight, vId);   

              //vProcess[processId]->fill_histogram("h" + syst_ext + "MassAN", mass(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "MassTop1", m1_, evt_weight, vId);   
              vProcess[processId]->fill_histogram("h" + syst_ext + "MassTop2", m2_, evt_weight, vId);   
              //vProcess[processId]->fill_histogram("h" + syst_ext + "MassTop_Hq_1", top_candidates[0], evt_weight, vId);
              //vProcess[processId]->fill_histogram("h" + syst_ext + "MassTop_Hq_2", top_candidates[6], evt_weight, vId);

              //vProcess[processId]->fill_histogram("h" + syst_ext + "MassTop_qqq_1", top_candidates[1], evt_weight, vId);
              //vProcess[processId]->fill_histogram("h" + syst_ext + "MassTop_qqq_2", top_candidates[7], evt_weight, vId);

              /*
              if (leadptoM_ > 0.33 && subleadptoM_ > 0.25) {
            vProcess[processId]->fill_histogram("h" + syst_ext + "Mass_PassPtToM", mass(), evt_weight, vId);
            if (mva_value > 0.9)
              vProcess[processId]->fill_histogram("h" + syst_ext + "Mass_PassPtToM_AfterBDTCut", mass(), evt_weight, vId);
              }
              else {
            vProcess[processId]->fill_histogram("h" + syst_ext + "Mass_FailPtToM", mass(), evt_weight, vId);
            if (mva_value > 0.9)
              vProcess[processId]->fill_histogram("h" + syst_ext + "Mass_FailPtToM_AfterBDTCut", mass(), evt_weight, vId);
              }

              vProcess[processId]->fill_2D_histogram("hMass_MVAScore", mass(), tthMVA_RunII(), evt_weight, vId);
              */

              cout.setf(ios::fixed);
              cout << std::setprecision(6);

              // Skip blinded region for MC after filling mass histogram
              if (!(tag.Contains("GJet_Reweight_Preselection"))) {
                if (!isSignal && blind && mass() > 120 && mass() < 130)	continue;
              }

              // Fill rest of histograms //
              if (!currentFileTitle.Contains("FCNC")) {
                vProcess[processId]->fill_histogram("h" + syst_ext + "DNNScore_ttH_vs_dipho", dnn_score_dipho(), evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "DNNScore_ttH_vs_ttGG", dnn_score_ttgg(), evt_weight, vId); 
              }

              double dipho_mass_resolution = 0.5* pow((pow(lead_sigmaEoE(),2) + pow(sublead_sigmaEoE(),2)), 0.5);
              /*
              vProcess[processId]->fill_histogram("h" + syst_ext + "DiphotonMassResolution", dipho_mass_resolution, evt_weight, vId);
              if (tthMVA() >= 0.75)
                vProcess[processId]->fill_histogram("h" + syst_ext + "DiphotonMassResolutionHighMVA", dipho_mass_resolution, evt_weight, vId);
              else if (tthMVA() >= 0.3)
                vProcess[processId]->fill_histogram("h" + syst_ext + "DiphotonMassResolutionMedMVA", dipho_mass_resolution, evt_weight, vId);
              else
                vProcess[processId]->fill_histogram("h" + syst_ext + "DiphotonMassResolutionLowMVA", dipho_mass_resolution, evt_weight, vId);
              */

              vProcess[processId]->fill_histogram("h" + syst_ext + "TopTagger_score", topTag_score(), evt_weight, vId);
              //vProcess[processId]->fill_histogram("h" + syst_ext + "TopTagger_topMass", topTag_topMass(), evt_weight, vId);
              //vProcess[processId]->fill_histogram("h" + syst_ext + "TopTagger_WMass", topTag_WMass(), evt_weight, vId);

              vProcess[processId]->fill_histogram("h" + syst_ext + "PixelSeed", leadPixelSeed(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PixelSeed", subleadPixelSeed(), evt_weight, vId);

              /*
              if (abs(leadEta()) < barrel_eta)
            vProcess[processId]->fill_histogram("h" + syst_ext + "PixelSeedEB", leadPixelSeed(), evt_weight, vId);
              else
            vProcess[processId]->fill_histogram("h" + syst_ext + "PixelSeedEE", leadPixelSeed(), evt_weight, vId);
              if (abs(subleadEta()) < barrel_eta)
                vProcess[processId]->fill_histogram("h" + syst_ext + "PixelSeedEB", subleadPixelSeed(), evt_weight, vId);
              else
                vProcess[processId]->fill_histogram("h" + syst_ext + "PixelSeedEE", subleadPixelSeed(), evt_weight, vId);
              */

              double helic = helicity(lead_photon, sublead_photon);
              vProcess[processId]->fill_histogram("h" + syst_ext + "AbsCosHelicity", helic, evt_weight, vId);

              //vProcess[processId]->fill_histogram("h" + syst_ext + "LeadMinDr", min_dr(lead_photon, jets), evt_weight, vId);
              //vProcess[processId]->fill_histogram("h" + syst_ext + "SubleadMinDr", min_dr(sublead_photon, jets), evt_weight, vId);

              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonDeltaR", lead_photon.DeltaR(sublead_photon), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "DiphotonPtOverMass", diphoton.Pt() / mass(), evt_weight, vId);

              vProcess[processId]->fill_histogram("h" + syst_ext + "PtHiggs", diphoton.Pt(), evt_weight, vId);
              //vProcess[processId]->fill_histogram("h" + syst_ext + "MinDrDiphoJet", min_dr(diphoton, jets), evt_weight, vId);

              //vProcess[processId]->fill_histogram("h" + syst_ext + "HadronicMVA", mva_value, evt_weight, vId);
              //vProcess[processId]->fill_histogram("h" + syst_ext + "HadronicMVA_coarse", mva_value, evt_weight, vId);
              //vProcess[processId]->fill_histogram("h" + syst_ext + "HadronicMVA_fine", mva_value, evt_weight, vId);

              vProcess[processId]->fill_histogram("h" + syst_ext + "Rapidity", dipho_rapidity(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "DiphotonSumPt", dipho_sumpt(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "DiphotonCosPhi", dipho_cosphi(), evt_weight, vId);

              vProcess[processId]->fill_histogram("h" + syst_ext + "NVtx", nvtx(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "Rho", rho(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "MetPt", MetPt(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "HT", get_ht(jets), evt_weight, vId);

              vProcess[processId]->fill_histogram("h" + syst_ext + "NJets", n_jets(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "NbLoose", nb_loose(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "NbMedium", nb_medium(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "NbTight", nb_tight(), evt_weight, vId); 


              if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet1pT", jet1_pt(), evt_weight, vId);
              if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet2pT", jet2_pt(), evt_weight, vId);
              if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet3pT", jet3_pt(), evt_weight, vId);
              if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet4pT", jet4_pt(), evt_weight, vId);

              if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet1Eta", jet1_eta(), evt_weight, vId);
              if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet2Eta", jet2_eta(), evt_weight, vId);
              if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet3Eta", jet3_eta(), evt_weight, vId);
              if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet4Eta", jet4_eta(), evt_weight, vId);

              if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet1BTag", btag_scores[0], evt_weight, vId);
              if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet2BTag", btag_scores[1], evt_weight, vId);
              if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet3BTag", btag_scores[2], evt_weight, vId);
              if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("h" + syst_ext + "Jet4BTag", btag_scores[3], evt_weight, vId);

              if (bjet1_pt() != -1)     vProcess[processId]->fill_histogram("h" + syst_ext + "bJet1pT", bjet1_pt(), evt_weight, vId);
              if (bjet2_pt() != -1)     vProcess[processId]->fill_histogram("h" + syst_ext + "bJet2pT", bjet2_pt(), evt_weight, vId);

              // Leading Photon
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadPt", leadPt(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadEt", leadEt(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadEta", leadEta(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadPhi", leadPhi(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadSigmaIEtaIEta", lead_sieie(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadHOverE", lead_hoe(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadR9", leadR9(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadIDMVA", leadID_, evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadPToM", lead_ptoM(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadSigmaEOverE", lead_sigmaEoE(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadPixelSeed", leadPixelSeed(), evt_weight, vId);

              /*
              if (lead_closest_gen_dR() < 999) {
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonLeadPtGen", lead_closest_gen_Pt(), evt_weight, vId);
            if (lead_photon_type() == 1)
                  vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonPtRatio", leadPt() / leadPtGen(), evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonDeltaRGen", lead_closest_gen_dR(), evt_weight, vId);
              }
              */

              // Subleading Photon
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadPt", subleadPt(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadEt", subleadEt(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadEta", subleadEta(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadPhi", subleadPhi(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadSigmaIEtaIEta", sublead_sieie(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadHOverE", sublead_hoe(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadR9", subleadR9(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadIDMVA", subleadID_, evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadPToM", sublead_ptoM(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadSigmaEOverE", sublead_sigmaEoE(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadPixelSeed", subleadPixelSeed(), evt_weight, vId);

              /*
              if (sublead_closest_gen_dR() < 999) {
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonSubleadPtGen", sublead_closest_gen_Pt(), evt_weight, vId);
            if (sublead_photon_type() == 1)
                  vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonPtRatio", subleadPt() / subleadPtGen(), evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonDeltaRGen", sublead_closest_gen_dR(), evt_weight, vId);
              }
              */

              double maxID = maxIDMVA_;
              double minID = minIDMVA_; 

              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA", maxID, evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_fine", maxID, evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_coarse", maxID, evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_entries", maxID, 1, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_fine_entries", maxID, 1, vId);
              //if (njets_ >= 5)
                //vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_NJets5+", maxID, evt_weight, vId);
              //if (njets_ >= 7)
                //vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_NJets7+", maxID, evt_weight, vId);

              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA", minID, evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_fine", minID, evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_coarse", minID, evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_entries", minID, 1, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_fine_entries", minID, 1, vId);


              vector<int> _null_vector_;
              vector<int> indices_bjet = get_bjet_indices(jets, btag_scores);

              bool is_moreThanThreeJets_and_atLeastOneBjet = jets.size() > 3 && indices_bjet.size() > 0;
              bool is_moreThanTwoJets_and_atLeastOneBjet   = jets.size() > 2 && indices_bjet.size() > 0;
              vector<int> index_jet_chi2_modified = is_moreThanThreeJets_and_atLeastOneBjet ? get_bjjq_indices_min_chi2(jets, indices_bjet, diphoton, true) :
                                                   (is_moreThanTwoJets_and_atLeastOneBjet   ? get_bjj_indices_min_chi2(jets, indices_bjet, true) : _null_vector_);
              vector<int> index_jet_chi2_improved = is_moreThanThreeJets_and_atLeastOneBjet ? get_bjjq_indices_min_chi2_3x3(jets, indices_bjet, diphoton) : _null_vector_;

              //values{{{
              TLorentzVector _nothing_;
              TLorentzVector chi2_bjet      = is_moreThanTwoJets_and_atLeastOneBjet   ? jets[index_jet_chi2_modified[0]]                : _nothing_;
              TLorentzVector chi2_wjet1     = is_moreThanTwoJets_and_atLeastOneBjet   ? jets[index_jet_chi2_modified[1]]                : _nothing_;
              TLorentzVector chi2_wjet2     = is_moreThanTwoJets_and_atLeastOneBjet   ? jets[index_jet_chi2_modified[2]]                : _nothing_;
              TLorentzVector chi2_qjet      = is_moreThanThreeJets_and_atLeastOneBjet ? jets[index_jet_chi2_modified[3]]                : _nothing_;
              TLorentzVector chi2_tbw       = is_moreThanTwoJets_and_atLeastOneBjet   ? chi2_bjet + chi2_wjet1 + chi2_wjet2             : _nothing_;
              TLorentzVector chi2_tqh       = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_qjet + diphoton                            : _nothing_;

              float chi2_tbw_mass_                = is_moreThanTwoJets_and_atLeastOneBjet   ? chi2_tbw.M()                                    : -999;
              float chi2_tbw_pt_                  = is_moreThanTwoJets_and_atLeastOneBjet   ? chi2_tbw.Pt()                                   : -999;
              float chi2_tbw_eta_                 = is_moreThanTwoJets_and_atLeastOneBjet   ? chi2_tbw.Eta()                                  : -999;
              float chi2_tbw_deltaR_dipho_        = is_moreThanTwoJets_and_atLeastOneBjet   ? chi2_tbw.DeltaR(diphoton)                       : -999;
              float chi2_qjet_pt_                 = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_qjet.Pt()                                  : -999;
              float chi2_qjet_eta_                = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_qjet.Eta()                                 : -999;
              float chi2_qjet_btag_               = is_moreThanThreeJets_and_atLeastOneBjet ? btag_scores[index_jet_chi2_modified[3]]         : -999;
              float chi2_qjet_deltaR_dipho_       = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_qjet.DeltaR(diphoton)                      : -999;
              float chi2_tqh_ptOverM_             = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_tqh.Pt()/chi2_tqh.M()                      : -999;
              float chi2_tqh_eta_                 = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_tqh.Eta()                                  : -999;
              float chi2_tqh_deltaR_tbw_          = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_tqh.DeltaR(chi2_tbw)                       : -999; 

              TLorentzVector chi2_3x3_bjet  = is_moreThanThreeJets_and_atLeastOneBjet ? jets[index_jet_chi2_improved[0]]                : _nothing_;
              TLorentzVector chi2_3x3_wjet1 = is_moreThanThreeJets_and_atLeastOneBjet ? jets[index_jet_chi2_improved[1]]                : _nothing_;
              TLorentzVector chi2_3x3_wjet2 = is_moreThanThreeJets_and_atLeastOneBjet ? jets[index_jet_chi2_improved[2]]                : _nothing_;
              TLorentzVector chi2_3x3_qjet  = is_moreThanThreeJets_and_atLeastOneBjet ? jets[index_jet_chi2_improved[3]]                : _nothing_;
              TLorentzVector chi2_3x3_tbw   = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_bjet + chi2_3x3_wjet1 + chi2_3x3_wjet2 : _nothing_;
              TLorentzVector chi2_3x3_tqh   = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_qjet + diphoton                        : _nothing_;

              float chi2_3x3_tbw_mass_            = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_tbw.M()                                : -999;
              float chi2_3x3_tbw_pt_              = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_tbw.Pt()                               : -999;
              float chi2_3x3_tbw_eta_             = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_tbw.Eta()                              : -999;
              float chi2_3x3_tbw_deltaR_dipho_    = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_tbw.DeltaR(diphoton)                   : -999;
              float chi2_3x3_qjet_pt_             = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_qjet.Pt()                              : -999;
              float chi2_3x3_qjet_eta_            = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_qjet.Eta()                             : -999;
              float chi2_3x3_qjet_btag_           = is_moreThanThreeJets_and_atLeastOneBjet ? btag_scores[index_jet_chi2_improved[3]]         : -999;
              float chi2_3x3_qjet_deltaR_dipho_   = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_qjet.DeltaR(diphoton)                  : -999;
              float chi2_3x3_tqh_ptOverM_         = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_tqh.Pt()/chi2_3x3_tqh.M()              : -999;
              float chi2_3x3_tqh_eta_             = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_tqh.Eta()                              : -999;
              float chi2_3x3_tqh_deltaR_tbw_      = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_tqh.DeltaR(chi2_3x3_tbw)               : -999;
              float chi2_3x3_tqh_deltaR_dipho_    = is_moreThanThreeJets_and_atLeastOneBjet ? chi2_3x3_tqh.DeltaR(diphoton)                   : -999; 

              if (is_moreThanTwoJets_and_atLeastOneBjet) {
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_tbw_mass", chi2_tbw_mass_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_tbw_pt", chi2_tbw_pt_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_tbw_eta", chi2_tbw_eta_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_tbw_deltaR_dipho", chi2_tbw_deltaR_dipho_, evt_weight, vId);
              }

              if (is_moreThanThreeJets_and_atLeastOneBjet) {
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_qjet_pt", chi2_qjet_pt_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_qjet_eta", chi2_qjet_eta_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_qjet_btag", chi2_qjet_btag_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_qjet_deltaR_dipho", chi2_qjet_deltaR_dipho_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_tqh_ptOverM", chi2_tqh_ptOverM_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_tqh_eta", chi2_tqh_eta_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_tqh_deltaR_tbw", chi2_tqh_deltaR_tbw_, evt_weight, vId);

                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_3x3_tbw_mass", chi2_3x3_tbw_mass_ , evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_3x3_tbw_pt", chi2_3x3_tbw_pt_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_3x3_tbw_eta", chi2_3x3_tbw_eta_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_3x3_tbw_deltaR_dipho", chi2_3x3_tbw_deltaR_dipho_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_3x3_tqh_ptOverM", chi2_3x3_tqh_ptOverM_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_3x3_tqh_eta", chi2_3x3_tqh_eta_ , evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_3x3_tqh_deltaR_tbw", chi2_3x3_tqh_deltaR_tbw_, evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "chi2_3x3_tqh_deltaR_dipho", chi2_3x3_tqh_deltaR_dipho_, evt_weight, vId);
              }

              //if (njets_ >= 5)
              //  vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_NJets5+", minID, evt_weight, vId);
              //if (njets_ >= 7)
              //  vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_NJets7+", minID, evt_weight, vId);


              /*
              if (nb_medium() == 0) {
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_coarse_0b", minID, evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_coarse_0b", maxID, evt_weight, vId);
              }

              vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_NJets", maxID, n_jets(), evt_weight, vId);
              vProcess[processId]->fill_2D_histogram("hPhotonMinIDMVA_NJets", minID, n_jets(), evt_weight, vId); 
              vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_NJets_entries", maxID, n_jets(), 1, vId);
              vProcess[processId]->fill_2D_histogram("hPhotonMinIDMVA_NJets_entries", minID, n_jets(), 1, vId);

              vProcess[processId]->fill_2D_histogram("hPhotonMaxIDMVA_MinIDMVA", maxID, minID, evt_weight, vId);


              if (leadIDMVA() >= subleadIDMVA()) {
            vProcess[processId]->fill_histogram("h" + syst_ext + "MaxIDPhotonPt", leadPt(), evt_weight, vId);
            vProcess[processId]->fill_histogram("h" + syst_ext + "MaxIDPhotonEta", leadEta(), evt_weight, vId);
            vProcess[processId]->fill_histogram("h" + syst_ext + "MinIDPhotonPt", subleadPt(), evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "MinIDPhotonEta", subleadEta(), evt_weight, vId);
              }
              else {
            vProcess[processId]->fill_histogram("h" + syst_ext + "MaxIDPhotonPt", subleadPt(), evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "MaxIDPhotonEta", subleadEta(), evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "MinIDPhotonPt", leadPt(), evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "MinIDPhotonEta", leadEta(), evt_weight, vId);
              }

              if (lead_photon_type() == 3) {
            vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA", leadIDMVA(), evt_weight, vId);
            if (leadPt() > 40 && abs(leadEta()) < barrel_eta)
              vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA_Barrel_HighPt", leadIDMVA(), evt_weight, vId);
                else if (leadPt() < 40 && abs(leadEta()) < barrel_eta)
              vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA_Barrel_LowPt", leadIDMVA(), evt_weight, vId); 
            else if (leadPt() > 40 && abs(leadEta()) > barrel_eta)
                  vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA_Endcap_HighPt", leadIDMVA(), evt_weight, vId); 
                else if (leadPt() < 40 && abs(leadEta()) > barrel_eta)
                  vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA_Endcap_LowPt", leadIDMVA(), evt_weight, vId); 
                vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_NJets", leadIDMVA(), njets_, evt_weight, vId);
            vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_MaxBTag", leadIDMVA(), max1_btag_, evt_weight, vId); 
            vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_2ndMaxBTag", leadIDMVA(), max2_btag_, evt_weight, vId);
            vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_DiPhotonPt", leadIDMVA(), diphoton.Pt(), evt_weight, vId);
            vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_Pt", leadIDMVA(), leadPt(), evt_weight, vId);
            vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_Eta", leadIDMVA(), abs(leadEta()), evt_weight, vId);
              }
              else if (lead_photon_type() == 1) {
            vProcess[processId]->fill_histogram("h" + syst_ext + "PromptPhotonIDMVA", leadIDMVA(), evt_weight, vId);
            vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_NJets", leadIDMVA(), njets_, evt_weight, vId);
                vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_MaxBTag", leadIDMVA(), max1_btag_, evt_weight, vId);
                vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_2ndMaxBTag", leadIDMVA(), max2_btag_, evt_weight, vId);
                vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_DiPhotonPt", leadIDMVA(), diphoton.Pt(), evt_weight, vId);
            vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_Pt", leadIDMVA(), leadPt(), evt_weight, vId);
                vProcess[processId]->fill_2D_histogram("hPromptPhotonIDMVA_Eta", leadIDMVA(), abs(leadEta()), evt_weight, vId);
              }

              if (sublead_photon_type() == 3) {
            vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA", subleadIDMVA(), evt_weight, vId);
            if (subleadPt() > 40 && abs(subleadEta()) < barrel_eta)
                  vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA_Barrel_HighPt", subleadIDMVA(), evt_weight, vId);
                else if (subleadPt() < 40 && abs(subleadEta()) < barrel_eta)
                  vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA_Barrel_LowPt", subleadIDMVA(), evt_weight, vId);
                else if (subleadPt() > 40 && abs(subleadEta()) > barrel_eta)
                  vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA_Endcap_HighPt", subleadIDMVA(), evt_weight, vId); 
                else if (subleadPt() < 40 && abs(subleadEta()) > barrel_eta)
                  vProcess[processId]->fill_histogram("h" + syst_ext + "FakePhotonIDMVA_Endcap_LowPt", subleadIDMVA(), evt_weight, vId); 
            vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_NJets", subleadIDMVA(), njets_, evt_weight, vId);
                vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_MaxBTag", subleadIDMVA(), max1_btag_, evt_weight, vId);
                vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_2ndMaxBTag", subleadIDMVA(), max2_btag_, evt_weight, vId);
                vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_DiPhotonPt", subleadIDMVA(), diphoton.Pt(), evt_weight, vId);
            vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_Pt", subleadIDMVA(), subleadPt(), evt_weight, vId);
                vProcess[processId]->fill_2D_histogram("hFakePhotonIDMVA_Eta", subleadIDMVA(), abs(subleadEta()), evt_weight, vId);
              }
              else if (sublead_photon_type() == 1) {
            vProcess[processId]->fill_histogram("h" + syst_ext + "PromptPhotonIDMVA", subleadIDMVA(), evt_weight, vId);
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
            vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_NJets2", maxID, evt_weight, vId);
            vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_NJets2", minID, evt_weight, vId);
              }
              else if (n_jets() == 3) {
            vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_NJets3", maxID, evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_NJets3", minID, evt_weight, vId);
              }
              else if (n_jets() >= 4) {
            vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMaxIDMVA_NJets4+", maxID, evt_weight, vId);
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonMinIDMVA_NJets4+", minID, evt_weight, vId);
              }

              vProcess[processId]->fill_histogram("h" + syst_ext + "DiphoMVA", diphoMVARes(), evt_weight, vId);
              */

              vProcess[processId]->fill_histogram("h" + syst_ext + "MVA_transf", -log(1 - mva_value), evt_weight, vId);

              // ttH-Hadronic Specific
              vProcess[processId]->fill_histogram("h" + syst_ext + "MaxBTag", bjet1_csv(), evt_weight, vId);
              vProcess[processId]->fill_histogram("h" + syst_ext + "SecondMaxBTag", bjet2_csv(), evt_weight, vId);

              if (!currentFileTitle.Contains("FCNC")) {
                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA", tthMVA(), evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII", tthMVA_RunII(), evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf", -log(1-tthMVA_RunII()), evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_ttZ", -log(1-tthMVA_RunII()), evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_ttZ_v2", -log(1-tthMVA_RunII()), evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_ttZ_v3", -log(1-tthMVA_RunII()), evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_ttZ_v4", -log(1-tthMVA_RunII()), evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_ttZ_v5", -log(1-tthMVA_RunII()), evt_weight, vId);

                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_bounded", -log(1-tthMVA_RunII())/8., evt_weight, vId);
                  vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_bounded_v2", -log(1-tthMVA_RunII())/8., evt_weight, vId);

                  if (diphoton.Pt() < 60.)
                      vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_low_pT", -log(1-tthMVA_RunII()), evt_weight, vId);
                  else if (diphoton.Pt() >= 60. && diphoton.Pt() < 120.)
                      vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_med_pT", -log(1-tthMVA_RunII()), evt_weight, vId);
                  else if (diphoton.Pt() >= 120. && diphoton.Pt() < 200.)
                      vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_high_pT", -log(1-tthMVA_RunII()), evt_weight, vId);
                  else if (diphoton.Pt() >= 200.)
                      vProcess[processId]->fill_histogram("h" + syst_ext + "tthMVA_RunII_transf_Vhigh_pT", -log(1-tthMVA_RunII()), evt_weight, vId);

              }

              /*
              if (lead_photon_type() == 1)
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonIDMVA_prompt", maxID, evt_weight, vId);
              else if (lead_photon_type() == 2)
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonIDMVA_elec", maxID, evt_weight, vId);
              else if (lead_photon_type() == 3)
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonIDMVA_fake", maxID, evt_weight, vId);

              if (sublead_photon_type() == 1)
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonIDMVA_prompt", maxID, evt_weight, vId);
              else if (sublead_photon_type() == 2)
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonIDMVA_elec", maxID, evt_weight, vId);
              else if (sublead_photon_type() == 3)
                vProcess[processId]->fill_histogram("h" + syst_ext + "PhotonIDMVA_fake", maxID, evt_weight, vId);
              */

            }
            cout << "Deleting tree" << endl;
            delete tree;
        }
    } 
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
