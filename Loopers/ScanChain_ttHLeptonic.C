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

using namespace std;
using namespace tas;

const int nBkgCats = 7;
const double targetLumi = 1; // ? don't fully understand how weights work

int ScanChain(TChain* chain, TString filename, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(filename, "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // General
  vector<TH1D*> hMass = generate_1Dhist_vector("hMass", nBkgCats+1, 100, 0, 250);
  vector<TH1D*> hRapidity = generate_1Dhist_vector("hRapidity", nBkgCats+1, 50, -3, 3);
  vector<TH1D*> hDiphotonSumPt = generate_1Dhist_vector("hDiphotonSumPt", nBkgCats+1, 100, 0, 500);
  vector<TH1D*> hDiphotonCosPhi = generate_1Dhist_vector("hDiphotonCosPhi", nBkgCats+1, 50, -1, 1);

  // Leading photon
  vector<TH1D*> hPhotonLeadPt = generate_1Dhist_vector("hPhotonLeadPt", nBkgCats+1, 100, 0, 350);
  vector<TH1D*> hPhotonLeadEt = generate_1Dhist_vector("hPhotonLeadEt", nBkgCats+1, 100, 0, 350);
  vector<TH1D*> hPhotonLeadEta = generate_1Dhist_vector("hPhotonLeadEta", nBkgCats+1, 100, -3, 3);
  vector<TH1D*> hPhotonLeadPhi = generate_1Dhist_vector("hPhotonLeadPhi", nBkgCats+1, 100, -3.142, 3.142);
  vector<TH1D*> hPhotonLeadSigmaIEtaIEta = generate_1Dhist_vector("hPhotonLeadSigmaIEtaIEta", nBkgCats+1, 100, 0, 0.05);
  vector<TH1D*> hPhotonLeadHOverE = generate_1Dhist_vector("hPhotonLeadHOverE", nBkgCats+1, 100, 0, 0.1);
  vector<TH1D*> hPhotonLeadR9 = generate_1Dhist_vector("hPhotonLeadR9", nBkgCats+1, 100, 0, 1);
  vector<TH1D*> hPhotonLeadIDMVA = generate_1Dhist_vector("hPhotonLeadIDMVA", nBkgCats+1, 100, -1, 1);
  vector<TH1D*> hPhotonLeadPToM = generate_1Dhist_vector("hPhotonLeadPToM", nBkgCats+1, 100, 0, 1);
  vector<TH1D*> hPhotonLeadSigmaEOverE = generate_1Dhist_vector("hPhotonLeadSigmaEOverE", nBkgCats+1, 100, 0, 1);

  // Subleading photon
  vector<TH1D*> hPhotonSubleadPt = generate_1Dhist_vector("hPhotonSubleadPt", nBkgCats+1, 100, 0, 350);
  vector<TH1D*> hPhotonSubleadEt = generate_1Dhist_vector("hPhotonSubleadEt", nBkgCats+1, 100, 0, 350);
  vector<TH1D*> hPhotonSubleadEta = generate_1Dhist_vector("hPhotonSubleadEta", nBkgCats+1, 100, -3, 3);
  vector<TH1D*> hPhotonSubleadPhi = generate_1Dhist_vector("hPhotonSubleadPhi", nBkgCats+1, 100, -3.142, 3.142);
  vector<TH1D*> hPhotonSubleadSigmaIEtaIEta = generate_1Dhist_vector("hPhotonSubleadSigmaIEtaIEta", nBkgCats+1, 100, 0, 0.05);
  vector<TH1D*> hPhotonSubleadHOverE = generate_1Dhist_vector("hPhotonSubleadHOverE", nBkgCats+1, 100, 0, 0.1);
  vector<TH1D*> hPhotonSubleadR9 = generate_1Dhist_vector("hPhotonSubleadR9", nBkgCats+1, 100, 0, 1);
  vector<TH1D*> hPhotonSubleadIDMVA = generate_1Dhist_vector("hPhotonSubleadIDMVA", nBkgCats+1, 100, -1, 1);
  vector<TH1D*> hPhotonSubleadPToM = generate_1Dhist_vector("hPhotonSubleadPToM", nBkgCats+1, 100, 0, 1);
  vector<TH1D*> hPhotonSubleadSigmaEOverE = generate_1Dhist_vector("hPhotonSubleadSigmaEOverE", nBkgCats+1, 100, 0, 1);

  // ttH-Hadronic Specific
  vector<TH1D*> htthMVA = generate_1Dhist_vector("htthMVA", nBkgCats+1, 100, -1, 1);
  vector<TH1D*> hMaxBTag = generate_1Dhist_vector("hMaxBTag", nBkgCats+1, 50, 0, 1);
  vector<TH1D*> hSecondMaxBTag = generate_1Dhist_vector("hSecondMaxBTag", nBkgCats+1, 50, 0, 1); 


  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if (nEvents >= 0) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

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
    int processId = categorize_process(currentFileTitle);

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

      // Fill histograms //
      double evt_weight = scale1fb(currentFileTitle) * targetLumi * sgn(weight());
      
      // General
      hMass[processId]->Fill(mass(), evt_weight);
      hRapidity[processId]->Fill(dipho_rapidity(), evt_weight);
      hDiphotonSumPt[processId]->Fill(dipho_sumpt(), evt_weight);
      hDiphotonCosPhi[processId]->Fill(dipho_cosphi(), evt_weight);

      // Leading Photon
      hPhotonLeadPt[processId]->Fill(leadPt(), evt_weight);
      hPhotonLeadEt[processId]->Fill(leadEt(), evt_weight);
      hPhotonLeadEta[processId]->Fill(leadEta(), evt_weight);
      hPhotonLeadPhi[processId]->Fill(leadPhi(), evt_weight);
      hPhotonLeadSigmaIEtaIEta[processId]->Fill(lead_sieie(), evt_weight);
      hPhotonLeadHOverE[processId]->Fill(lead_hoe(), evt_weight);
      hPhotonLeadR9[processId]->Fill(leadR9(), evt_weight);
      hPhotonLeadIDMVA[processId]->Fill(leadIDMVA(), evt_weight);
      hPhotonLeadPToM[processId]->Fill(subleadIDMVA(), evt_weight);
      hPhotonLeadSigmaEOverE[processId]->Fill(lead_sigmaEoE(), evt_weight);

      // Subleading Photon
      hPhotonSubleadPt[processId]->Fill(subleadPt(), evt_weight);
      hPhotonSubleadEt[processId]->Fill(subleadEt(), evt_weight);
      hPhotonSubleadEta[processId]->Fill(subleadEta(), evt_weight);
      hPhotonSubleadPhi[processId]->Fill(subleadPhi(), evt_weight);
      hPhotonSubleadSigmaIEtaIEta[processId]->Fill(sublead_sieie(), evt_weight);
      hPhotonSubleadHOverE[processId]->Fill(sublead_hoe(), evt_weight);
      hPhotonSubleadR9[processId]->Fill(subleadR9(), evt_weight);
      hPhotonSubleadIDMVA[processId]->Fill(leadIDMVA(), evt_weight);
      hPhotonSubleadPToM[processId]->Fill(subleadIDMVA(), evt_weight);
      hPhotonSubleadSigmaEOverE[processId]->Fill(sublead_sigmaEoE(), evt_weight);

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
