#include "ScanChain_ttHLeptonic.h"

int ScanChain(TChain* chain, TString tag, TString year, TString xml_file, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag+"_histograms" + year + ".root", "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  bool evaluate_mva = xml_file != "none";

  // Make MVA Optimization Baby
  OptimizationBabyMaker* baby = new OptimizationBabyMaker();
  TString xml_file_noExt = xml_file;
  xml_file_noExt.ReplaceAll(".xml", "");
  TString optimization_baby_name = "Optimization/MVAOptimizationBaby_" + xml_file_noExt;
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


  unique_ptr<TMVA::Reader> mva;

  // Declare BDT vars
  float n_leps_;
  float lep_pt_;
  float lep_eta_;
  float nb_loose_;
  float max2_btag_;
  float max1_btag_;
  float dR_higgs_lep;
  float dR_higgs_W;
  float pt_higgs;
  float dipho_deltaR;
  float maxIDMVA_;
  float minIDMVA_;
  float njets_;
  float ht_;
  float leadptoM_;
  float subleadptoM_;
  float lead_eta_;
  float sublead_eta_;

  float jet1_pt_;
  float jet1_eta_;
  float jet2_pt_;
  float jet2_eta_;
  float jet3_pt_;
  float jet3_eta_;
  float jet4_pt_;
  float jet4_eta_;
  float jet5_pt_;
  float jet5_eta_;

  float leadPSV_;
  float subleadPSV_;

  float dipho_cosphi_;
  float dipho_rapidity_;
  float met_;
  float mt_; 


  if (evaluate_mva) {
    mva.reset(new TMVA::Reader( "!Color:Silent" ));
    mva->AddVariable("n_leps_", &n_leps_);
    mva->AddVariable("lep_pt_", &lep_pt_);
    mva->AddVariable("lep_eta_", &lep_eta_);
    mva->AddVariable("nb_loose_", &nb_loose_);
    mva->AddVariable("max2_btag_", &max2_btag_);
    mva->AddVariable("max1_btag_", &max1_btag_);
    mva->AddVariable("dR_higgs_lep", &dR_higgs_lep);
    mva->AddVariable("dR_higgs_W", &dR_higgs_W);
    mva->AddVariable("pt_higgs", &pt_higgs);
    mva->AddVariable("dipho_deltaR", &dipho_deltaR);
    mva->AddVariable("maxIDMVA_", &maxIDMVA_);
    mva->AddVariable("minIDMVA_", &minIDMVA_);
    mva->AddVariable("njets_", &njets_);
    mva->AddVariable("ht_", &ht_);
    mva->AddVariable("leadptoM_", &leadptoM_);
    mva->AddVariable("subleadptoM_", &subleadptoM_);
    mva->AddVariable("lead_eta_", &lead_eta_);
    mva->AddVariable("sublead_eta_", &sublead_eta_);

    mva->AddVariable("jet1_pt_", &jet1_pt_);
    mva->AddVariable("jet1_eta_", &jet1_eta_);
    mva->AddVariable("jet2_pt_", &jet2_pt_);
    mva->AddVariable("jet2_eta_", &jet2_eta_);
    mva->AddVariable("jet3_pt_", &jet3_pt_);
    mva->AddVariable("jet3_eta_", &jet3_eta_);
    mva->AddVariable("jet4_pt_", &jet4_pt_);
    mva->AddVariable("jet4_eta_", &jet4_eta_);
    mva->AddVariable("jet5_pt_", &jet5_pt_);
    mva->AddVariable("jet5_eta_", &jet5_eta_);

    mva->AddVariable("leadPSV_", &leadPSV_);
    mva->AddVariable("subleadPSV_", &subleadPSV_);

    mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
    mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
    mva->AddVariable("met_", &met_);
    mva->AddVariable("mt_", &mt_);


    mva->BookMVA("BDT", "../MVAs/" + xml_file);
  }



  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TString currentFileTitle = currentFile->GetTitle();
    TFile file(currentFileTitle);
    TTree *tree = (TTree*)file.Get("tthLeptonicTagDumper/trees/tth_13TeV_all");
    if (fast) TTreeCache::SetLearnEntries(10);
    if (fast) tree->SetCacheSize(128*1024*1024);
    cms3.Init(tree);

    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG");
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG");
    year = currentFileTitle.Contains("2016") ? "2016" : "2017";


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

      // Blinded region 
      if (isData && blind && mass() > 120 && mass() < 130)        continue;

      //if (currentFileTitle.Contains("TTGJet") && (lead_Prompt() == 0 || sublead_Prompt() == 0) ) continue;
      //if (currentFileTitle.Contains("TTJet") && (lead_Prompt() == 0 || sublead_Prompt() == 0) ) continue;
      
//      if (currentFileTitle.Contains("TTGG" && !(lead_Prompt() == 1 && sublead_Prompt() == 1 ) ) )continue;
//      if (currentFileTitle.Contains("TTGJet" && !(lead_Prompt() + sublead_Prompt() == -998 ) ) )continue;
//      if (currentFileTitle.Contains("TTJet" && !(lead_Prompt() == -999 && sublead_Prompt() == -999 ) ) ) continue;
      

      // Fill mva baby before any selections
      int processId = categorize_process(currentFileTitle);
      int genLeptonId = isData ? -1 : categorize_leptons(nGoodEls(), nGoodMus());
      int genPhotonId = isData ? -1 : categorize_photons(leadGenMatch(), subleadGenMatch());
      int genPhotonDetailId = isData ? -1 : categorize_photons_detail(lead_photon_type(), sublead_photon_type());
      int photonLocationId = categorize_photon_locations(leadEta(), subleadEta());

      double evt_weight = 1.;
      if (!isData) {
        if (year == "2016")
          evt_weight = scale1fb_2016(currentFileTitle) * lumi_2016 * sgn(weight());
        else if (year == "2017")
          evt_weight = scale1fb_2017(currentFileTitle) * lumi_2017 * sgn(weight());
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
      jets = make_jets(btag_scores);
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
      if (tag == "ttHLeptonicLoose") {
        if (mass() < 100)        continue;
	if (n_jets() < 2)	continue;
	if (nb_tight() < 1)		continue;
        double lep_pt, lep_eta;
        if (get_lep_pt(lep_eta) < 20)                   continue;
        if (MetPt() < 40)                             continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        if (leadPixelSeed() || subleadPixelSeed())      continue;
        if (leadIDMVA() < 0 || subleadIDMVA() <0 ) continue;
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
	if (nb_medium() < 1)            continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
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

      // Evaluate MVA, if we choose
      double mva_value = -999;
      if (evaluate_mva) {

        // Calculate MVA value
        n_leps_ = leps.size();
        lep_pt_ = leps[0].Pt();
        lep_eta_ = leps[0].Eta();
        nb_loose_ = nb_loose();
        max2_btag_ = btag_scores_sorted[1].second;
        max1_btag_ = btag_scores_sorted[0].second;
        dR_higgs_lep = diphoton.DeltaR(leps[0]);
        dR_higgs_W = deltaR_Higgs_W(jets, diphoton);
        pt_higgs = diphoton.Pt();
        dipho_deltaR = lead_photon.DeltaR(sublead_photon);
        maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
        minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
        ht_ = get_ht(jets);
        njets_ = n_jets();
        jet1_pt_ = jet_pt1();
        jet1_eta_ = jet_eta1();
        jet2_pt_ = jet_pt2();
        jet2_eta_ = jet_eta2();
        jet3_pt_ = jet_pt3();
        jet3_eta_ = jet_eta3();
        jet4_pt_ = jet_pt4();
        jet4_eta_ = jet_eta4();
        jet5_pt_ = jet_pt5();
        jet5_eta_ = jet_eta5();

        leadptoM_ = lead_ptoM();
        subleadptoM_ = sublead_ptoM();
        lead_eta_ = leadEta();
        sublead_eta_ = subleadEta();

        leadPSV_ = leadPixelSeed();
        subleadPSV_ = subleadPixelSeed();

        dipho_cosphi_ = dipho_cosphi();
        dipho_rapidity_ = dipho_rapidity();
        met_ = MetPt();
        mt_ = mT();

        mva_value = mva->EvaluateMVA( "BDT" );
        double reference_mva = year == "2017" ? tthMVA() : -1;
        bool pass_ref_presel = year == "2017" ? pass_2017_mva_presel() : true;
        baby->FillBabyNtuple(label, evt_weight, processId, cms3.rand(), mass(), mva_value, reference_mva, pass_ref_presel);
      }

      int mvaCategoryId = mva_value < -0.8 ? 0 : 1;
      vector<int> vId = {genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId, mvaCategoryId}; 

      bool make_text_file = false;
      bool make_2prompts = true;
      if (make_text_file && year == "2016") {
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

      vProcess[processId]->fill_histogram("hPhotonDeltaR", lead_photon.DeltaR(sublead_photon), evt_weight, vId);

      vProcess[processId]->fill_histogram("hPtHiggs", diphoton.Pt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hMinDrDiphoJet", min_dr(diphoton, jets), evt_weight, vId);
      vProcess[processId]->fill_histogram("hDeltaRDiphoLep", diphoton.DeltaR(leps[0]), evt_weight, vId);
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

      // General
      vProcess[processId]->fill_histogram("hMass", mass(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hMassAN", mass(), evt_weight, vId);

      // Skip blinded region for MC after filling mass histogram
      if (!isSignal && !isData && blind && mass() > 120 && mass() < 130)     continue;

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
