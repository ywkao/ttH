#include "MakeMVABabies_ttHHadronic.h"
#include "ScanChain_ttHHadronic.h"

void BabyMaker::ScanChain(TChain* chain, TString tag, TString ext, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Make baby ntuple
  MakeBabyNtuple( Form("%s.root", ("MVABabies/MVABaby_ttHHadronic_" + ext).Data()));

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if (nEvents >= 0) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  int nLowID = 0;

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TString currentFileTitle = currentFile->GetTitle();
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
    for (unsigned int event = 0; event < nEventsTree; ++event) {


      // Get Event Content
      if (nEventsTotal >= nEventsChain) continue;
      if (fast) tree->LoadTree(event);
      cms3.GetEntry(event);
      ++nEventsTotal;

      // Progress
      ttHHadronic::progress( nEventsTotal, nEventsChain );

      InitBabyNtuple();

      // Blinded region
      if (isData && blind && mass() > 120 && mass() < 130)	continue;

      // Selection
      //if (has_ttX_overlap(currentFileTitle, lead_Prompt(), sublead_Prompt()))           continue;

      if (tag == "ttHHadronicLoose") {
        if (mass() < 100)                continue;
	if (n_jets() < 3)		continue;
	if (nb_loose() < 1)		continue;
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
      else {
        cout << "Did not recognize tag name" << endl;
      }
 
      // Decide what type of sample this is
      process_id_ = categorize_process(currentFileTitle);

      vector<TLorentzVector> jets;
      TLorentzVector lead_photon;
      TLorentzVector sublead_photon;
      jets = make_jets();
      lead_photon = make_lead_photon();
      sublead_photon = make_sublead_photon();
      TLorentzVector diphoton = lead_photon + sublead_photon;

      // Fill histograms //
      evt_weight_ = 1.;
      if (!isData /*&& !isSignal*/) {
	if (year == "2016")
          evt_weight_ = scale1fb_2016(currentFileTitle) * lumi_2016 * sgn(weight());
        else if (year == "2017")
          evt_weight_ = scale1fb_2017(currentFileTitle) * lumi_2017 * sgn(weight());
      }

      int genPhotonId = isData ? -1 : categorize_photons(leadGenMatch(), subleadGenMatch());
      /*
      if ((currentFileTitle.Contains("DiPhoton") || currentFileTitle.Contains("QCD")) && !useEventForTemplate(currentFileTitle, genPhotonId) ) continue;
      if (currentFileTitle.Contains("GJet")) continue;
      if (currentFileTitle.Contains("DiPhoton")) evt_weight_ *= 0.32;
      if (currentFileTitle.Contains("QCD") && genPhotonId == 1) evt_weight_ *= 0.96;
      if (currentFileTitle.Contains("QCD") && genPhotonId == 0) evt_weight_ *= 1.52;
      */
      // Skip blinded region for MC after filling mass histogram
      bool isSignal = process_id_ == 0;

      //      label_ = isData ? 2 : (isSignal ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data

      int eventCat = -1;
      if (leadGenMatch() == 1 && subleadGenMatch() == 1) eventCat = 0; 
      //      if ((leadGenMatch() + subleadGenMatch() == 1) || (leadGenMatch() + subleadGenMatch() == 3) ) eventCat = 1; 
      if ((leadGenMatch() + subleadGenMatch() == 1) ) eventCat = 1; 
      if (leadGenMatch() != 1 && subleadGenMatch() != 1) eventCat = 2;

      // ttH: 0
      // ttGG: 5
      // ttG, tt: 6,9
      // diPhoton, GJet, QCD: 2,3,4

      bool fromTTX = false;
      if (process_id_ == 5 || process_id_ == 6 || process_id_ == 9) fromTTX = true;
      
      
      label_ = 5;
      //if (process_id_ == 0) label_ = 1;
      //if (process_id_ == 9) label_ = 0;
      //if (process_id_ == 5 || process_id_ == 6 || process_id_ == 9) label_ = 1;
      if (process_id_ == 2 || process_id_ == 3 || process_id_ == 4) label_ = 4;
      if (fromTTX && eventCat == 0) label_ = 1;
      if (fromTTX && eventCat == 1) label_ = 2;
      if (fromTTX && eventCat == 2) label_ = 3;
      if (process_id_ == 0) label_ = 0;
      
      label_ttH_ttgg_bdt_ = 2;
      if (process_id_ == 0) label_ttH_ttgg_bdt_ = 1;
      if (eventCat == 0 && (process_id_ == 5 || process_id_ == 6) ) label_ttH_ttgg_bdt_ = 0;

      TLorentzVector diPho = lead_photon + sublead_photon;
      // Variable definitions
      topTag_score_ = topTag_score();
      topTag_pT_ = topTag_pT();
      topTag_eta_ = topTag_eta();
      topTag_phi_ = topTag_phi();
      topTag_topMass_ = topTag_topMass();

      dipho_delta_R = lead_photon.DeltaR(sublead_photon);
      ht_ = get_ht(jets);
      njets_ = n_jets();
      //nbjets_ = nb_medium();
      nb_loose_ = nb_loose();
      minIDMVA_ = leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      maxIDMVA_ = leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA();

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
      jet1_phi_ =  jet1_phi() > 0 ? abs(diPho.Phi() - jet1_phi()) : -999;
      jet2_phi_ =  jet2_phi() > 0 ? abs(diPho.Phi() - jet2_phi()) : -999;
      jet3_phi_ =  jet3_phi() > 0 ? abs(diPho.Phi() - jet3_phi()) : -999;
      jet4_phi_ =  jet4_phi() > 0 ? abs(diPho.Phi() - jet4_phi()) : -999;
      jet5_phi_ =  jet5_phi() > 0 ? abs(diPho.Phi() - jet5_phi()) : -999;
      jet6_phi_ =  jet6_phi() > 0 ? abs(diPho.Phi() - jet6_phi()) : -999;

      max1_btag_ = bjet1_csv();
      max2_btag_ = bjet2_csv();

      leadptoM_ = lead_ptoM();
      subleadptoM_ = sublead_ptoM();
      
      //      leadIDMVA_ = leadIDMVA(); 
      //subleadIDMVA_ = subleadIDMVA();
      lead_eta_ = leadEta();
      sublead_eta_ = subleadEta();

      lead_phi_ = leadPhi();
      sublead_phi_ = subleadPhi();

      leadPSV_ = leadPixelSeed();
      subleadPSV_ = subleadPixelSeed();

      dipho_cosphi_ = dipho_cosphi();
      dipho_rapidity_ = dipho_rapidity();
      diphoptom_ = dipho_sumpt()/mass();
      met_ = MetPt();

      rand_ = cms3.rand();
      super_rand_ = rand_map->retrieve_rand(cms3.event(), cms3.run(), cms3.lumi());

      mass_ = mass();
      lead_sigmaEOverE_ = lead_sigmaEoE();
      sublead_sigmaEOverE_ = sublead_sigmaEoE();

      eventCat_ = eventCat;

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
