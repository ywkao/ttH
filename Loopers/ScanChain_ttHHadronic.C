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

using namespace std;
using namespace tas;

const int nBkgCats = 7;
const double targetLumi = 1; 

int ScanChain(TChain* chain, TString tag, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag + "_histograms.root", "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // General
  vector<TH1D*> hMass = generate_1Dhist_vector("hMass", nBkgCats+1, 50, 0, 250);
  vector<TH1D*> hRapidity = generate_1Dhist_vector("hRapidity", nBkgCats+1, 25, -3, 3);
  vector<TH1D*> hDiphotonSumPt = generate_1Dhist_vector("hDiphotonSumPt", nBkgCats+1, 25, 0, 1000);
  vector<TH1D*> hDiphotonCosPhi = generate_1Dhist_vector("hDiphotonCosPhi", nBkgCats+1, 25, -1, 1);

  // Jet kinematics
  //vector<TH1D*> hHT = generate_1Dhist_vector("hHT", nBkgCats+1, 50, 0, 1000);
  //vector<TH1D*> hMET = generate_1Dhist_vector("hHT", nBkgCats+1, 50, 0, 400);

  vector<TH1D*> hNJets = generate_1Dhist_vector("hNJets", nBkgCats+1, 16, -0.5, 15.5);
  vector<TH1D*> hNbJets = generate_1Dhist_vector("hNbJets", nBkgCats+1, 16, -0.5, 15.5);
  vector<TH1D*> hJet1pT = generate_1Dhist_vector("hJet1pT", nBkgCats+1, 25, 0, 500);
  vector<TH1D*> hJet1Eta = generate_1Dhist_vector("hJet1Eta", nBkgCats+1, 25, -3, 3);
  vector<TH1D*> hJet2pT = generate_1Dhist_vector("hJet2pT", nBkgCats+1, 25, 0, 500);
  vector<TH1D*> hJet2Eta = generate_1Dhist_vector("hJet2Eta", nBkgCats+1, 25, -3, 3);  
  vector<TH1D*> hJet3pT = generate_1Dhist_vector("hJet3pT", nBkgCats+1, 25, 0, 500);
  vector<TH1D*> hJet3Eta = generate_1Dhist_vector("hJet3Eta", nBkgCats+1, 25, -3, 3);
  vector<TH1D*> hJet4pT = generate_1Dhist_vector("hJet4pT", nBkgCats+1, 25, 0, 500);
  vector<TH1D*> hJet4Eta = generate_1Dhist_vector("hJet4Eta", nBkgCats+1, 25, -3, 3);  

  vector<TH1D*> hbJet1pT = generate_1Dhist_vector("hbJet1pT", nBkgCats+1, 25, 0, 500);
  //vector<TH1D*> hbJet1Eta = generate_1Dhist_vector("hbJet1Eta", nBkgCats+1, 25, -3, 3);
  vector<TH1D*> hbJet2pT = generate_1Dhist_vector("hbJet2pT", nBkgCats+1, 25, 0, 500);
  //vector<TH1D*> hbJet2Eta = generate_1Dhist_vector("hbJet2Eta", nBkgCats+1, 50, -3, 3);    


  // Leading photon
  vector<TH1D*> hPhotonLeadPt = generate_1Dhist_vector("hPhotonLeadPt", nBkgCats+1, 25, 0, 350);
  vector<TH1D*> hPhotonLeadEt = generate_1Dhist_vector("hPhotonLeadEt", nBkgCats+1, 25, 0, 350);
  vector<TH1D*> hPhotonLeadEta = generate_1Dhist_vector("hPhotonLeadEta", nBkgCats+1, 25, -3, 3);
  vector<TH1D*> hPhotonLeadPhi = generate_1Dhist_vector("hPhotonLeadPhi", nBkgCats+1, 25, -3.142, 3.142);
  vector<TH1D*> hPhotonLeadSigmaIEtaIEta = generate_1Dhist_vector("hPhotonLeadSigmaIEtaIEta", nBkgCats+1, 50, 0, 0.05);
  vector<TH1D*> hPhotonLeadHOverE = generate_1Dhist_vector("hPhotonLeadHOverE", nBkgCats+1, 25, 0, 0.1);
  vector<TH1D*> hPhotonLeadR9 = generate_1Dhist_vector("hPhotonLeadR9", nBkgCats+1, 25, 0, 1);
  vector<TH1D*> hPhotonLeadIDMVA = generate_1Dhist_vector("hPhotonLeadIDMVA", nBkgCats+1, 25, -1, 1);
  vector<TH1D*> hPhotonLeadPToM = generate_1Dhist_vector("hPhotonLeadPToM", nBkgCats+1, 25, 0, 5);
  vector<TH1D*> hPhotonLeadSigmaEOverE = generate_1Dhist_vector("hPhotonLeadSigmaEOverE", nBkgCats+1, 25, 0, 1);

  // Subleading photon
  vector<TH1D*> hPhotonSubleadPt = generate_1Dhist_vector("hPhotonSubleadPt", nBkgCats+1, 25, 0, 350);
  vector<TH1D*> hPhotonSubleadEt = generate_1Dhist_vector("hPhotonSubleadEt", nBkgCats+1, 25, 0, 350);
  vector<TH1D*> hPhotonSubleadEta = generate_1Dhist_vector("hPhotonSubleadEta", nBkgCats+1, 25, -3, 3);
  vector<TH1D*> hPhotonSubleadPhi = generate_1Dhist_vector("hPhotonSubleadPhi", nBkgCats+1, 25, -3.142, 3.142);
  vector<TH1D*> hPhotonSubleadSigmaIEtaIEta = generate_1Dhist_vector("hPhotonSubleadSigmaIEtaIEta", nBkgCats+1, 50, 0, 0.05);
  vector<TH1D*> hPhotonSubleadHOverE = generate_1Dhist_vector("hPhotonSubleadHOverE", nBkgCats+1, 25, 0, 0.1);
  vector<TH1D*> hPhotonSubleadR9 = generate_1Dhist_vector("hPhotonSubleadR9", nBkgCats+1, 25, 0, 1);
  vector<TH1D*> hPhotonSubleadIDMVA = generate_1Dhist_vector("hPhotonSubleadIDMVA", nBkgCats+1, 25, -1, 1);
  vector<TH1D*> hPhotonSubleadPToM = generate_1Dhist_vector("hPhotonSubleadPToM", nBkgCats+1, 25, 0, 5);
  vector<TH1D*> hPhotonSubleadSigmaEOverE = generate_1Dhist_vector("hPhotonSubleadSigmaEOverE", nBkgCats+1, 25, 0, 1);

  // ttH-Hadronic Specific
  vector<TH1D*> htthMVA = generate_1Dhist_vector("htthMVA", nBkgCats+1, 50, -1, 1);
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
    TTree *tree = (TTree*)file.Get("tthHadronicTagDumper/trees/tth_13TeV_all");
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
      ttHHadronic::progress( nEventsTotal, nEventsChain );

      // Selection
      if (tag == "ttHHadronicLoose") {
	if (mass() < 80)	continue;
      }
      else if (tag == "ttHHadronic") {
	if (n_jets() < 3) 	continue;
	if (tthMVA() < 0.75)	continue;
      }
      else {
        cout << "Did not recognize tag name" << endl;
      }

      // Fill histograms //
      double evt_weight = scale1fb(currentFileTitle) * targetLumi * sgn(weight());

      // General
      hMass[processId]->Fill(mass(), evt_weight);
      hRapidity[processId]->Fill(dipho_rapidity(), evt_weight);
      hDiphotonSumPt[processId]->Fill(dipho_sumpt(), evt_weight);
      hDiphotonCosPhi[processId]->Fill(dipho_cosphi(), evt_weight);

      // Jet kinematics
      hNJets[processId]->Fill(n_jets(), evt_weight);
      hNbJets[processId]->Fill(n_bjets(), evt_weight); // medium id
      if (jet1_pt() != -1) 	hJet1pT[processId]->Fill(jet1_pt(), evt_weight);
      if (jet2_pt() != -1)	hJet2pT[processId]->Fill(jet2_pt(), evt_weight);
      if (jet3_pt() != -1)	hJet3pT[processId]->Fill(jet3_pt(), evt_weight);
      if (jet4_pt() != -1)	hJet4pT[processId]->Fill(jet4_pt(), evt_weight);
      
      if (jet1_pt() != -1)    	hJet1Eta[processId]->Fill(jet1_eta(), evt_weight);
      if (jet2_pt() != -1)    	hJet2Eta[processId]->Fill(jet2_eta(), evt_weight);
      if (jet3_pt() != -1)    	hJet3Eta[processId]->Fill(jet3_eta(), evt_weight);
      if (jet4_pt() != -1)    	hJet4Eta[processId]->Fill(jet4_eta(), evt_weight);

      if (bjet1_pt() != -1)	hbJet1pT[processId]->Fill(bjet1_pt(), evt_weight);
      if (bjet2_pt() != -1)	hbJet2pT[processId]->Fill(bjet2_pt(), evt_weight);

      // Leading Photon
      hPhotonLeadPt[processId]->Fill(leadPt(), evt_weight); 
      hPhotonLeadEt[processId]->Fill(leadEt(), evt_weight);
      hPhotonLeadEta[processId]->Fill(leadEta(), evt_weight);
      hPhotonLeadPhi[processId]->Fill(leadPhi(), evt_weight);
      hPhotonLeadSigmaIEtaIEta[processId]->Fill(lead_sieie(), evt_weight);
      hPhotonLeadHOverE[processId]->Fill(lead_hoe(), evt_weight);
      hPhotonLeadR9[processId]->Fill(leadR9(), evt_weight);
      hPhotonLeadIDMVA[processId]->Fill(leadIDMVA(), evt_weight);
      hPhotonLeadPToM[processId]->Fill(lead_ptoM(), evt_weight);
      hPhotonLeadSigmaEOverE[processId]->Fill(lead_sigmaEoE(), evt_weight);

      // Subleading Photon
      hPhotonSubleadPt[processId]->Fill(subleadPt(), evt_weight);
      hPhotonSubleadEt[processId]->Fill(subleadEt(), evt_weight);
      hPhotonSubleadEta[processId]->Fill(subleadEta(), evt_weight);
      hPhotonSubleadPhi[processId]->Fill(subleadPhi(), evt_weight);
      hPhotonSubleadSigmaIEtaIEta[processId]->Fill(sublead_sieie(), evt_weight);
      hPhotonSubleadHOverE[processId]->Fill(sublead_hoe(), evt_weight);
      hPhotonSubleadR9[processId]->Fill(subleadR9(), evt_weight);
      hPhotonSubleadIDMVA[processId]->Fill(subleadIDMVA(), evt_weight);
      hPhotonSubleadPToM[processId]->Fill(sublead_ptoM(), evt_weight);
      hPhotonSubleadSigmaEOverE[processId]->Fill(sublead_sigmaEoE(), evt_weight);

      // ttH-Hadronic Specific
      if (n_bjets() >= 1)	hMaxBTag[processId]->Fill(bjet1_csv(), evt_weight);
      if (n_bjets() >= 2)	hSecondMaxBTag[processId]->Fill(bjet2_csv(), evt_weight);
      htthMVA[processId]->Fill(tthMVA(), evt_weight);

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
