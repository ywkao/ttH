// -*- C++ -*-
// Usage:
// > root -b -q doAll.C

#include <iostream>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"

// ttHHadronic
#include "ttHHadronic.cc"
#include "ttHLooper.h"
#include "scale1fb/scale1fb.h"
#include "MakeMVAOptimizationBabies.h"

// tmva
#include "TMVA/Reader.h" 

using namespace std;
using namespace tas;

const double targetLumi = 35.9; 

int ScanChain(TChain* chain, TString tag, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag + "_histograms.root", "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");


  // Make MVA Optimization Baby
  BabyMaker* baby = new BabyMaker();
  baby->MakeBabyNtuple( Form("%s.root", "Optimization/MVAOptimizationBaby_ttHHadronic"));

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

  int nLowID = 0;

  unique_ptr<TMVA::Reader> mva;
  float njets_;
  //float nbjets_;
  float ht_;
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

  float max1_btag_;
  float max2_btag_;
  
  float leadPSV_;
  float subleadPSV_;
  
  float dipho_cosphi_;
  float dipho_rapidity_;
  float met_;

  mva.reset(new TMVA::Reader( "!Color:Silent" ));
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
  mva->AddVariable("jet5_pt_", &jet5_pt_);
  mva->AddVariable("jet5_eta_", &jet5_eta_);
  mva->AddVariable("jet5_btag_", &jet5_btag_);
  mva->AddVariable("jet6_pt_", &jet6_pt_);
  mva->AddVariable("jet6_eta_", &jet6_eta_);
  mva->AddVariable("jet6_btag_", &jet6_btag_);

  mva->AddVariable("max1_btag_", &max1_btag_);
  mva->AddVariable("max2_btag_", &max2_btag_);

  mva->AddVariable("leadPSV_", &leadPSV_);
  mva->AddVariable("subleadPSV_", &subleadPSV_);

  mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
  mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
  mva->AddVariable("met_", &met_);

  mva->BookMVA("BDT", "../MVAs/Hadronic_bdt.xml");

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TString currentFileTitle = currentFile->GetTitle();
    TFile file(currentFileTitle);
    TTree *tree = (TTree*)file.Get("tthHadronicTagDumper/trees/tth_13TeV_all");
    if (fast) TTreeCache::SetLearnEntries(10);
    if (fast) tree->SetCacheSize(128*1024*1024);
    cms3.Init(tree);

    // Decide what type of sample this is
    bool isData = currentFileTitle.Contains("DoubleEG"); 
    bool isSignal = currentFileTitle.Contains("ttHJetToGG") || currentFileTitle.Contains("ttHToGG");

    bool print = true;

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

      // Blinded region
      if (isData && blind && mass() > 120 && mass() < 130)	continue;

      // Apply preselection before calculating MVA value
      if (mass() < 100)                continue;
      if (n_jets() < 3)               continue;
      if (nb_loose() < 1)             continue;
      if (!(leadPassEVeto() && subleadPassEVeto()))   continue;

      // Calculate MVA value
      ht_ = 0;
      ht_ += jet1_pt() > 0 ? jet1_pt() : 0;
      ht_ += jet2_pt() > 0 ? jet2_pt() : 0;
      ht_ += jet3_pt() > 0 ? jet3_pt() : 0;
      ht_ += jet4_pt() > 0 ? jet4_pt() : 0;
      ht_ += jet5_pt() > 0 ? jet5_pt() : 0;
      ht_ += jet6_pt() > 0 ? jet6_pt() : 0;
      ht_ += jet7_pt() > 0 ? jet7_pt() : 0;
      ht_ += jet8_pt() > 0 ? jet8_pt() : 0;
      ht_ += jet9_pt() > 0 ? jet9_pt() : 0;
      ht_ += jet10_pt() > 0 ? jet10_pt() : 0;
      ht_ += jet11_pt() > 0 ? jet11_pt() : 0;
      ht_ += jet12_pt() > 0 ? jet12_pt() : 0;
      ht_ += jet13_pt() > 0 ? jet13_pt() : 0;
      ht_ += jet14_pt() > 0 ? jet14_pt() : 0;
      ht_ += jet15_pt() > 0 ? jet15_pt() : 0;

      njets_ = n_jets();
      //nbjets_ = nb_medium();
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

      max1_btag_ = bjet1_csv();
      max2_btag_ = bjet2_csv();

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
      met_ = MetPt();

      float mva_value = mva->EvaluateMVA( "BDT" );
      // Fill mva baby before any selections
      int processId = categorize_process(currentFileTitle);
      int genLeptonId = isData ? -1 : categorize_leptons(nGoodEls(), nGoodMus());
      int genPhotonId = isData ? -1 : categorize_photons(leadGenMatch(), subleadGenMatch());
      int genPhotonDetailId = isData ? -1 : categorize_photons_detail(lead_photon_type(), sublead_photon_type());
      int photonLocationId = categorize_photon_locations(leadEta(), subleadEta());
      int mvaCategoryId = mva_value < -0.92 ? 0 : 1;

      double evt_weight = 1.;
      if (!isData)
        evt_weight = scale1fb(currentFileTitle) * targetLumi * sgn(weight());
    
      int label = isData ? 2 : (isSignal ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data
      baby->FillBabyNtuple(label, evt_weight, processId, mass(), mva_value);

      // Selection
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
      else if (tag == "ttHHadronicCustom") {
	if (mass() < 100)                continue;
        if (n_jets() < 3)               continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (mva_value < -0.9)		continue;
      }
      else if (tag == "2017MVAPreSel") {
        if (mass() < 100)       continue;
        if (n_jets() < 2)       continue;
        if (leadIDMVA() < -0.1)         continue;
        if (subleadIDMVA() < -0.1)         continue;
      }
      else {
        cout << "Did not recognize tag name" << endl;
      }
 
      // Decide what type of sample this is
      vector<int> vId = {genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId, mvaCategoryId};

      // Fill histograms //
      vProcess[processId]->fill_histogram("hMass", mass(), evt_weight, vId);   
      vProcess[processId]->fill_histogram("hMassAN", mass(), evt_weight, vId);
      cout.setf(ios::fixed);
      cout << std::setprecision(6);
      if (isData && mass() >= 100 && mass() <= 180) {
	//cout << mass() << endl;
	//cout << "Mass: " << mass() << " , Event: " << cms3.event() << " , Run: " << run() << " , Lumi: " << lumi() << endl;
      }

      if (isData && (leadIDMVA() < -0.9 || subleadIDMVA() < -0.9)) {
        nLowID++;
      }

      // Skip blinded region for MC after filling mass histogram
      if (!isSignal && !isData && blind && mass() > 120 && mass() < 130)	continue;


      vProcess[processId]->fill_histogram("hHadronicMVA", mva_value, evt_weight, vId);

      vProcess[processId]->fill_histogram("hRapidity", dipho_rapidity(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hDiphotonSumPt", dipho_sumpt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hDiphotonCosPhi", dipho_cosphi(), evt_weight, vId);

      vProcess[processId]->fill_histogram("hNVtx", nvtx(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hMetPt", MetPt(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hHT", ht_, evt_weight, vId);

      vProcess[processId]->fill_histogram("hNJets", n_jets(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hNbJets", nb_loose(), evt_weight, vId); // medium id
      if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("hJet1pT", jet1_pt(), evt_weight, vId);
      if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("hJet2pT", jet2_pt(), evt_weight, vId);
      if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("hJet3pT", jet3_pt(), evt_weight, vId);
      if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("hJet4pT", jet4_pt(), evt_weight, vId);

      if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("hJet1Eta", jet1_eta(), evt_weight, vId);
      if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("hJet2Eta", jet2_eta(), evt_weight, vId);
      if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("hJet3Eta", jet3_eta(), evt_weight, vId);
      if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("hJet4Eta", jet4_eta(), evt_weight, vId);

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
      vProcess[processId]->fill_histogram("hPhotonLeadIDMVA", leadIDMVA(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadPToM", lead_ptoM(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonLeadSigmaEOverE", lead_sigmaEoE(), evt_weight, vId);

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
      vProcess[processId]->fill_histogram("hPhotonSubleadIDMVA", subleadIDMVA(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadPToM", sublead_ptoM(), evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonSubleadSigmaEOverE", sublead_sigmaEoE(), evt_weight, vId);

      if (sublead_closest_gen_dR() < 999) {
        vProcess[processId]->fill_histogram("hPhotonSubleadPtGen", sublead_closest_gen_Pt(), evt_weight, vId);
	if (sublead_photon_type() == 1)
          vProcess[processId]->fill_histogram("hPhotonPtRatio", subleadPt() / subleadPtGen(), evt_weight, vId);
        vProcess[processId]->fill_histogram("hPhotonDeltaRGen", sublead_closest_gen_dR(), evt_weight, vId);
      }

      double maxID = leadIDMVA() >= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      double minID = leadIDMVA() >= subleadIDMVA() ? subleadIDMVA() : leadIDMVA();

      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA", maxID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_entries", maxID, 1, vId);

      vProcess[processId]->fill_histogram("hPhotonMinIDMVA", minID, evt_weight, vId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_entries", minID, 1, vId);
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
 
  baby->CloseBabyNtuple();
 
  // Example Histograms
  f1->Write();
  f1->Close(); 

  cout << "Number of data events with one photon with ID < -0.9: " << nLowID << endl;
 
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
