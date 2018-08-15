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
#include "scale1fb/scale1fb_2016.h"
#include "scale1fb/scale1fb_2017.h"
#include "MakeMVABabies_ttHLeptonic.h"

using namespace std;
using namespace tas;

const double lumi_2016 = 35.9; 
const double lumi_2017 = 41.5;

double get_lep_pt(double &lep_eta) {
  if (n_ele() + n_muons() == 1) {
    lep_eta = n_ele() == 1 ? ele1_eta() : muon1_eta();
    return n_ele() == 1 ? ele1_pt() : muon1_pt();
  }
  else {
    double highest_pt = 0;
    if (ele1_pt() > highest_pt) {
      highest_pt = ele1_pt();
      lep_eta = ele1_eta();
    }
    if (ele2_pt() > highest_pt) {
      highest_pt = ele2_pt();
      lep_eta = ele2_eta();
    }
    if (muon1_pt() > highest_pt) {
      highest_pt = muon1_pt();
      lep_eta = muon1_eta();
    }
    if (muon2_pt() > highest_pt) {
      highest_pt = muon2_pt();
      lep_eta = muon2_eta();
    }
    return highest_pt;
  }
}

void BabyMaker::ScanChain(TChain* chain, TString tag, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Make baby ntuple
  MakeBabyNtuple( Form("%s.root", "MVABaby_ttHLeptonic"));

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
      if (tag == "ttHLeptonicLoose") {
        if (mass() < 100)        continue;
        if (n_jets() < 2)       continue;
        if (nb_loose() < 1)             continue;
        if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
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

      // Fill histograms //
      evt_weight_ = 1.;
      if (!isData && !isSignal) {
	if (year == "2016")
          evt_weight_ = scale1fb_2016(currentFileTitle) * lumi_2016 * sgn(weight());
        else if (year == "2017")
          evt_weight_ = scale1fb_2017(currentFileTitle) * lumi_2017 * sgn(weight());
      }

      // Skip blinded region for MC after filling mass histogram
      bool isSignal = process_id_ == 0;
      if (!isSignal && !isData && blind && mass() > 120 && mass() < 130)	continue;

      label_ = isData ? 2 : (isSignal ? 1 : 0); // 0 = bkg, 1 = signal, 2 = data

      ht_ = 0;
      ht_ += jet_pt1() > 0 ? jet_pt1() : 0;
      ht_ += jet_pt2() > 0 ? jet_pt2() : 0;
      ht_ += jet_pt3() > 0 ? jet_pt3() : 0;
      ht_ += jet_pt4() > 0 ? jet_pt4() : 0;
      ht_ += jet_pt5() > 0 ? jet_pt5() : 0;
      ht_ += jet_pt6() > 0 ? jet_pt6() : 0;
      ht_ += jet_pt7() > 0 ? jet_pt7() : 0;
      ht_ += jet_pt8() > 0 ? jet_pt8() : 0;
      ht_ += jet_pt9() > 0 ? jet_pt9() : 0;
      ht_ += jet_pt10() > 0 ? jet_pt10() : 0;
      ht_ += jet_pt11() > 0 ? jet_pt11() : 0;
      ht_ += jet_pt12() > 0 ? jet_pt12() : 0;
      ht_ += jet_pt13() > 0 ? jet_pt13() : 0;
      ht_ += jet_pt14() > 0 ? jet_pt14() : 0;
      ht_ += jet_pt15() > 0 ? jet_pt15() : 0;


      // Variable definitions
      njets_ = n_jets();
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

      lep_pt_ = n_ele() > 0 ? ele1_pt() : muon1_pt();
      lep_eta_ = n_ele() > 0 ? ele1_eta() : muon1_eta();

      rand_ = cms3.rand();

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

/*
void BabyMaker::MakeBabyNtuple(const char *BabyFilename){
  BabyFile_ = new TFile(Form("%s", BabyFilename), "RECREATE");
  BabyFile_->cd();
  BabyTree_ = new TTree("t", "A Baby Ntuple");

  BabyTree_->Branch("njets"	, &njets	);

  return;
}

void BabyMaker::InitBabyNtuple () {
  return;
}

void BabyMaker::FillBabyNtuple(){
  BabyTree_->Fill();
  return;
}

void BabyMaker::CloseBabyNtuple(){
  BabyFile_->cd();
  BabyTree_->Write();
  BabyFile_->Close();
  return;
}
*/
