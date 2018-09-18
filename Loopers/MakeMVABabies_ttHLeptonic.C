#include "MakeMVABabies_ttHLeptonic.h"
#include "ScanChain_ttHLeptonic.h"

void BabyMaker::ScanChain(TChain* chain, TString tag, TString ext, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

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

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TString currentFileTitle = currentFile->GetTitle();
    TFile file(currentFileTitle);
    TTree *tree = (TTree*)file.Get("tthLeptonicTagDumper/trees/tth_13TeV_all");
    if (fast) TTreeCache::SetLearnEntries(10);
    if (fast) tree->SetCacheSize(128*1024*1024);
    cms3.Init(tree);

    // Initialize map of evt_run_lumi -> rand
    RandomMap* rand_map = new RandomMap("Utils/random_map_Leptonic_" + ext + ".txt");

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
      ttHLeptonic::progress( nEventsTotal, nEventsChain );

      InitBabyNtuple();

      // Blinded region
      if (isData && blind && mass() > 120 && mass() < 130)	continue;

      // Selection
      //if (has_ttX_overlap(currentFileTitle, lead_Prompt(), sublead_Prompt()))           continue;

      if (tag == "ttHLeptonicLoose") {
        if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
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
 
      // Decide what type of sample this is
      process_id_ = categorize_process(currentFileTitle);
      //if (is_low_stats_process(currentFileTitle))	continue;

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


      // Fill histograms //
      evt_weight_ = 1.;
      if (!no_weights && !isData && !isSignal) {
	if (year == "2016")
          evt_weight_ = scale1fb_2016(currentFileTitle) * lumi_2016 * sgn(weight());
        else if (year == "2017")
          evt_weight_ = scale1fb_2017(currentFileTitle) * lumi_2017 * sgn(weight());
      }

      bool isSignal = process_id_ == 0;


      label_ = isData ? 2 : (isSignal ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data

      // Variable definitions
      helic = helicity(lead_photon, sublead_photon);
      min_dr_sublead_photon = min_dr(sublead_photon, objects);
      min_dr_lead_photon = min_dr(lead_photon, objects);
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

      rand_ = cms3.rand();
      super_rand_ = rand_map->retrieve_rand(cms3.event(), cms3.run(), cms3.lumi());

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
