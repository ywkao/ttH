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

// ttHLeptonic
#include "ttHLeptonic.cc"
#include "ttHLooper.h"
#include "scale1fb/scale1fb.h"

// tmva
#include "TMVA/Reader.h"

using namespace std;
using namespace tas;

const double targetLumi = 35.9; 

int ScanChain(TChain* chain, TString tag, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag+"_histograms.root", "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

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

  float leadPSV_;
  float subleadPSV_;

  float dipho_cosphi_;
  float dipho_rapidity_;
  float met_;
  float mt_; 

  mva.reset(new TMVA::Reader( "!Color:Silent" ));
  mva->AddVariable("njets_", &njets_);
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

  mva->AddVariable("leadPSV_", &leadPSV_);
  mva->AddVariable("subleadPSV_", &subleadPSV_);

  mva->AddVariable("dipho_cosphi_", &dipho_cosphi_);
  mva->AddVariable("dipho_rapidity_", &dipho_rapidity_);
  mva->AddVariable("met_", &met_);
  mva->AddVariable("mt_", &mt_);

  mva->BookMVA("BDT", "../MVAs/Leptonic_bdt.xml");



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

      // Apply preselection before calculating MVA value
      if (mass() < 100)                continue;
      if (n_jets() < 2)               continue;
      if (nb_loose() < 1)             continue;
      if (!(leadPassEVeto() && subleadPassEVeto()))   continue;

      // Calculate MVA value
      double ht = 0;
      ht += jet_pt1() > 0 ? jet_pt1() : 0;
      ht += jet_pt2() > 0 ? jet_pt2() : 0;
      ht += jet_pt3() > 0 ? jet_pt3() : 0;
      ht += jet_pt4() > 0 ? jet_pt4() : 0;
      ht += jet_pt5() > 0 ? jet_pt5() : 0;
      ht += jet_pt6() > 0 ? jet_pt6() : 0;
      ht += jet_pt7() > 0 ? jet_pt7() : 0;
      ht += jet_pt8() > 0 ? jet_pt8() : 0;
      ht += jet_pt9() > 0 ? jet_pt9() : 0;
      ht += jet_pt10() > 0 ? jet_pt10() : 0;
      ht += jet_pt11() > 0 ? jet_pt11() : 0;
      ht += jet_pt12() > 0 ? jet_pt12() : 0;
      ht += jet_pt13() > 0 ? jet_pt13() : 0;
      ht += jet_pt14() > 0 ? jet_pt14() : 0;
      ht += jet_pt15() > 0 ? jet_pt15() : 0;
      ht_ = ht;

      njets_ = n_jets();
      //nbjets_ = nb_medium();
      jet1_pt_ = jet_pt1();
      jet1_eta_ = jet_eta1();
      jet1_btag_ = jet_bdiscriminant1();
      jet2_pt_ = jet_pt2();
      jet2_eta_ = jet_eta2();
      jet2_btag_ = jet_bdiscriminant2();
      jet3_pt_ = jet_pt3();
      jet3_eta_ = jet_eta3();
      jet3_btag_ = jet_bdiscriminant3();
      jet4_pt_ = jet_pt4();
      jet4_eta_ = jet_eta4();
      jet4_btag_ = jet_bdiscriminant4();
      jet5_pt_ = jet_pt5();
      jet5_eta_ = jet_eta5();
      jet5_btag_ = jet_bdiscriminant5();

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
      mt_ = mT();

      float mva_value = mva->EvaluateMVA( "BDT" );

      // Selection
      if (tag == "ttHLeptonicLoose") {
        if (mass() < 100)        continue;
	if (n_jets() < 2)	continue;
	if (nb_loose() < 1)		continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
      }
      else if (tag == "ttHLeptonicCustom") {
        if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
	if (mva_value < -0.9)	continue;
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


      else {
        cout << "Did not recognize tag name" << endl;
      } 

      // Decide what type of sample this is
      int processId = categorize_process(currentFileTitle);
      int genLeptonId = isData ? -1 : categorize_leptons(nGoodEls(), nGoodMus());
      int genPhotonId = isData ? -1 : categorize_photons(leadGenMatch(), subleadGenMatch());
      int genPhotonDetailId = isData ? -1 : categorize_photons_detail(lead_photon_type(), sublead_photon_type());
      int photonLocationId = categorize_photon_locations(leadEta(), subleadEta()); 

      // Fill histograms //
      double evt_weight = 1;
      if (!isData)
        evt_weight = scale1fb(currentFileTitle) * targetLumi * sgn(weight());

     
      //cout << currentFileTitle << " has weight: " << evt_weight << endl;

      cout.setf(ios::fixed);
      cout << std::setprecision(6);
      if (isData && mass() >= 100 && mass() <= 180) {
        cout << mass() << endl;     
        //cout << mass() << " , run: " << run() << "  , lumi: " << lumi() << "  , event: " << tas::event() << endl;
      } 

      if (isData && (leadIDMVA() < -0.9 || subleadIDMVA() < -0.9)) {
	nLowID++;
      }

      // General
      vProcess[processId]->fill_histogram("hMass", mass(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hMassAN", mass(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      // Skip blinded region for MC after filling mass histogram
      bool isSignal = processId == 0;
      if (!isSignal && !isData && blind && mass() > 120 && mass() < 130)     continue;



      vProcess[processId]->fill_histogram("hLeptonicMVA", mva_value, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hRapidity", dipho_rapidity(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hDiphotonSumPt", dipho_sumpt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hDiphotonCosPhi", dipho_cosphi(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      vProcess[processId]->fill_histogram("hNVtx", nvtx(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hMetPt", MetPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hHT", ht, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      vProcess[processId]->fill_histogram("hNJets", n_jets(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hNbJets", nb_loose(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hMT", mT(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet_pt1() != -100)      vProcess[processId]->fill_histogram("hJet1pT", jet_pt1(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet_pt2() != -100)      vProcess[processId]->fill_histogram("hJet2pT", jet_pt2(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet_pt3() != -100)      vProcess[processId]->fill_histogram("hJet3pT", jet_pt3(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet_pt4() != -100)      vProcess[processId]->fill_histogram("hJet4pT", jet_pt4(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      if (jet_pt1() != -100)      vProcess[processId]->fill_histogram("hJet1Eta", jet_eta1(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet_pt2() != -100)      vProcess[processId]->fill_histogram("hJet2Eta", jet_eta2(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet_pt3() != -100)      vProcess[processId]->fill_histogram("hJet3Eta", jet_eta3(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet_pt4() != -100)      vProcess[processId]->fill_histogram("hJet4Eta", jet_eta4(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      if (n_bjets() >= 1)     vProcess[processId]->fill_histogram("hbJet1pT", bjet1_pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (n_bjets() >= 2)     vProcess[processId]->fill_histogram("hbJet2pT", bjet2_pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      if (n_bjets() >= 1)     vProcess[processId]->fill_histogram("hMaxBTag", bjet1_csv(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (n_bjets() >= 2)     vProcess[processId]->fill_histogram("hSecondMaxBTag", bjet2_csv(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      // Leading Photon
      vProcess[processId]->fill_histogram("hPhotonLeadPt", leadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonLeadEt", leadEt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonLeadEta", leadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonLeadPhi", leadPhi(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonLeadSigmaIEtaIEta", lead_sieie(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonLeadHOverE", lead_hoe(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonLeadR9", leadR9(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonLeadIDMVA", leadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonLeadPToM", lead_ptoM(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonLeadSigmaEOverE", lead_sigmaEoE(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      if (lead_closest_gen_dR() < 999) {
        vProcess[processId]->fill_histogram("hPhotonLeadPtGen", lead_closest_gen_Pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonPtRatio", leadPt() / lead_closest_gen_Pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonDeltaRGen", lead_closest_gen_dR(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      
      // Subleading Photon
      vProcess[processId]->fill_histogram("hPhotonSubleadPt", subleadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonSubleadEt", subleadEt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonSubleadEta", subleadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonSubleadPhi", subleadPhi(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonSubleadSigmaIEtaIEta", sublead_sieie(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonSubleadHOverE", sublead_hoe(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonSubleadR9", subleadR9(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonSubleadIDMVA", subleadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonSubleadPToM", sublead_ptoM(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonSubleadSigmaEOverE", sublead_sigmaEoE(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId); 

      if (sublead_closest_gen_dR() < 999) {
        vProcess[processId]->fill_histogram("hPhotonSubleadPtGen", sublead_closest_gen_Pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonPtRatio", subleadPt() / sublead_closest_gen_Pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonDeltaRGen", sublead_closest_gen_dR(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }

      double maxID = leadIDMVA() >= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      double minID = leadIDMVA() >= subleadIDMVA() ? subleadIDMVA() : leadIDMVA();

      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA", maxID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA", minID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse", minID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_coarse", maxID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hDiphoMVA", diphoMVARes(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
     
      if (lead_photon_type() == 1) {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_prompt", leadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_prompt", leadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_prompt", leadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      else if (lead_photon_type() == 2) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_elec", leadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_elec", leadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_elec", leadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      else if (lead_photon_type() == 3) {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_fake", leadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_fake", leadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_fake", leadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }

      if (sublead_photon_type() == 1) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_prompt", subleadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_prompt", subleadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_prompt", subleadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      else if (sublead_photon_type() == 2) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_elec", subleadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_elec", subleadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_elec", subleadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      else if (sublead_photon_type() == 3) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_fake", subleadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_fake", subleadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_fake", subleadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }


      // Fill veto study histograms
      if (leadPassEVeto()) {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_passEVeto", leadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_passEVeto", leadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonEta_passEVeto", leadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      else {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_failEVeto", leadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_failEVeto", leadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonEta_failEVeto", leadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      if (subleadPassEVeto()) {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_passEVeto", subleadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);	
	vProcess[processId]->fill_histogram("hPhotonPt_passEVeto", subleadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_passEVeto", subleadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      else {
	vProcess[processId]->fill_histogram("hPhotonIDMVA_failEVeto", subleadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_failEVeto", subleadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_failEVeto", subleadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }

      if (!leadPixelSeed()) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_passPSV", leadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_passPSV", leadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_passPSV", leadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      else {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_failPSV", leadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_failPSV", leadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_failPSV", leadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      if (!subleadPixelSeed()) {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_passPSV", subleadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_passPSV", subleadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_passPSV", subleadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }
      else {
        vProcess[processId]->fill_histogram("hPhotonIDMVA_failPSV", subleadIDMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
	vProcess[processId]->fill_histogram("hPhotonPt_failPSV", subleadPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
        vProcess[processId]->fill_histogram("hPhotonEta_failPSV", subleadEta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      }


      if (leadPixelSeed() || subleadPixelSeed()) 
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_failPSV", minID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      else
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_passPSV", minID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      // pass E-veto
      if (leadPassEVeto() && subleadPassEVeto())
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_passEVeto", minID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      else
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_failEVeto", minID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      // pass both
      if ((!leadPixelSeed() && !subleadPixelSeed()) && (leadPassEVeto() && subleadPassEVeto()))
        vProcess[processId]->fill_histogram("hPhotonMinIDMVA_passBothVeto", minID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      else
	vProcess[processId]->fill_histogram("hPhotonMinIDMVA_failBothVeto", minID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

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
