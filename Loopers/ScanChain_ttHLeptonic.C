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

const int nBkgCats = 18; 
const double targetLumi = 35.9; 

int ScanChain(TChain* chain, TString tag, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag+"_histograms.root", "RECREATE");
  f1->cd();

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // General
  vector<TH1D*> hMass = generate_1Dhist_vector("hMass", nBkgCats+2, 50, 0, 250);
  vector<TH1D*> hRapidity = generate_1Dhist_vector("hRapidity", nBkgCats+2, 25, -3, 3);
  vector<TH1D*> hDiphotonSumPt = generate_1Dhist_vector("hDiphotonSumPt", nBkgCats+2, 25, 0, 1000);
  vector<TH1D*> hDiphotonCosPhi = generate_1Dhist_vector("hDiphotonCosPhi", nBkgCats+2, 25, -1, 1);

  vector<TH1D*> hNVtx = generate_1Dhist_vector("hNVtx", nBkgCats+2, 25, 0, 75);
  vector<TH1D*> hMetPt = generate_1Dhist_vector("hMetPt", nBkgCats+2, 25, 0, 200);

  // Jet kinematics
  vector<TH1D*> hHT = generate_1Dhist_vector("hHT", nBkgCats+2, 50, 0, 2500);

  vector<TH1D*> hNJets = generate_1Dhist_vector("hNJets", nBkgCats+2, 16, -0.5, 15.5);
  vector<TH1D*> hNbJets = generate_1Dhist_vector("hNbJets", nBkgCats+2, 16, -0.5, 15.5);
  vector<TH1D*> hJet1pT = generate_1Dhist_vector("hJet1pT", nBkgCats+2, 25, 0, 500);
  vector<TH1D*> hJet1Eta = generate_1Dhist_vector("hJet1Eta", nBkgCats+2, 25, -3, 3);
  vector<TH1D*> hJet2pT = generate_1Dhist_vector("hJet2pT", nBkgCats+2, 25, 0, 500);
  vector<TH1D*> hJet2Eta = generate_1Dhist_vector("hJet2Eta", nBkgCats+2, 25, -3, 3);  
  vector<TH1D*> hJet3pT = generate_1Dhist_vector("hJet3pT", nBkgCats+2, 25, 0, 500);
  vector<TH1D*> hJet3Eta = generate_1Dhist_vector("hJet3Eta", nBkgCats+2, 25, -3, 3);
  vector<TH1D*> hJet4pT = generate_1Dhist_vector("hJet4pT", nBkgCats+2, 25, 0, 500);
  vector<TH1D*> hJet4Eta = generate_1Dhist_vector("hJet4Eta", nBkgCats+2, 25, -3, 3);  

  vector<TH1D*> hbJet1pT = generate_1Dhist_vector("hbJet1pT", nBkgCats+2, 25, 0, 500);
  //vector<TH1D*> hbJet1Eta = generate_1Dhist_vector("hbJet1Eta", nBkgCats+2, 25, -3, 3);
  vector<TH1D*> hbJet2pT = generate_1Dhist_vector("hbJet2pT", nBkgCats+2, 25, 0, 500);
  //vector<TH1D*> hbJet2Eta = generate_1Dhist_vector("hbJet2Eta", nBkgCats+2, 25, -3, 3);    

  // Leading photon
  vector<TH1D*> hPhotonLeadPt = generate_1Dhist_vector("hPhotonLeadPt", nBkgCats+2, 25, 0, 350);
  vector<TH1D*> hPhotonLeadEt = generate_1Dhist_vector("hPhotonLeadEt", nBkgCats+2, 25, 0, 350);
  vector<TH1D*> hPhotonLeadEta = generate_1Dhist_vector("hPhotonLeadEta", nBkgCats+2, 25, -3, 3);
  vector<TH1D*> hPhotonLeadPhi = generate_1Dhist_vector("hPhotonLeadPhi", nBkgCats+2, 25, -3.142, 3.142);
  vector<TH1D*> hPhotonLeadSigmaIEtaIEta = generate_1Dhist_vector("hPhotonLeadSigmaIEtaIEta", nBkgCats+2, 50, 0, 0.05);
  vector<TH1D*> hPhotonLeadHOverE = generate_1Dhist_vector("hPhotonLeadHOverE", nBkgCats+2, 25, 0, 0.1);
  vector<TH1D*> hPhotonLeadR9 = generate_1Dhist_vector("hPhotonLeadR9", nBkgCats+2, 25, 0, 1);
  vector<TH1D*> hPhotonLeadIDMVA = generate_1Dhist_vector("hPhotonLeadIDMVA", nBkgCats+2, 25, -1, 1);
  vector<TH1D*> hPhotonLeadPToM = generate_1Dhist_vector("hPhotonLeadPToM", nBkgCats+2, 25, 0, 5);
  vector<TH1D*> hPhotonLeadSigmaEOverE = generate_1Dhist_vector("hPhotonLeadSigmaEOverE", nBkgCats+2, 25, 0, 1);

  // Subleading photon
  vector<TH1D*> hPhotonSubleadPt = generate_1Dhist_vector("hPhotonSubleadPt", nBkgCats+2, 25, 0, 350);
  vector<TH1D*> hPhotonSubleadEt = generate_1Dhist_vector("hPhotonSubleadEt", nBkgCats+2, 25, 0, 350);
  vector<TH1D*> hPhotonSubleadEta = generate_1Dhist_vector("hPhotonSubleadEta", nBkgCats+2, 25, -3, 3);
  vector<TH1D*> hPhotonSubleadPhi = generate_1Dhist_vector("hPhotonSubleadPhi", nBkgCats+2, 25, -3.142, 3.142);
  vector<TH1D*> hPhotonSubleadSigmaIEtaIEta = generate_1Dhist_vector("hPhotonSubleadSigmaIEtaIEta", nBkgCats+2, 50, 0, 0.05);
  vector<TH1D*> hPhotonSubleadHOverE = generate_1Dhist_vector("hPhotonSubleadHOverE", nBkgCats+2, 25, 0, 0.1);
  vector<TH1D*> hPhotonSubleadR9 = generate_1Dhist_vector("hPhotonSubleadR9", nBkgCats+2, 25, 0, 1);
  vector<TH1D*> hPhotonSubleadIDMVA = generate_1Dhist_vector("hPhotonSubleadIDMVA", nBkgCats+2, 25, -1, 1);
  vector<TH1D*> hPhotonSubleadPToM = generate_1Dhist_vector("hPhotonSubleadPToM", nBkgCats+2, 25, 0, 5);
  vector<TH1D*> hPhotonSubleadSigmaEOverE = generate_1Dhist_vector("hPhotonSubleadSigmaEOverE", nBkgCats+2, 25, 0, 1);

  // ttH-Hadronic Specific
  vector<TH1D*> htthMVA = generate_1Dhist_vector("htthMVA", nBkgCats+2, 50, -1, 1);
  vector<TH1D*> hMaxBTag = generate_1Dhist_vector("hMaxBTag", nBkgCats+2, 50, 0, 1);
  vector<TH1D*> hSecondMaxBTag = generate_1Dhist_vector("hSecondMaxBTag", nBkgCats+2, 50, 0, 1); 


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

      // Selection
      if (isData && blind && mass() > 120 && mass() < 130)        continue;
      if (tag == "ttHLeptonicLoose") {
        if (mass() < 80)        continue;
      }
      else if (tag == "ttHLeptonic") {
        
      }
      else {
        cout << "Did not recognize tag name" << endl;
      } 

      // Decide what type of sample this is
      int processId = categorize_process(currentFileTitle);

      if (currentFileTitle.Contains("TTGJets")) {
        processId += categorize_ttgjets(nGoodEls(), nGoodMus());
      }
      else if (currentFileTitle.Contains("TT")) {
        processId += categorize_ttbar(nGoodEls(), nGoodMus()); // increment to appropriately categorize ttbar into had/semilep/dilep
      }


      // Fill histograms //
      double evt_weight = 1;
      if (!isData)
        evt_weight = scale1fb(currentFileTitle) * targetLumi * sgn(weight());
      
      // General
      hMass[processId]->Fill(mass(), evt_weight);

      if (!isData && blind && mass() > 120 && mass() < 130)     continue;

      hRapidity[processId]->Fill(dipho_rapidity(), evt_weight);
      hDiphotonSumPt[processId]->Fill(dipho_sumpt(), evt_weight);
      hDiphotonCosPhi[processId]->Fill(dipho_cosphi(), evt_weight);

      hNVtx[processId]->Fill(nvtx(), evt_weight);
      hMetPt[processId]->Fill(MetPt(), evt_weight);
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
      hHT[processId]->Fill(ht, evt_weight);


      // Jet kinematics
      hNJets[processId]->Fill(n_jets(), evt_weight);
      hNbJets[processId]->Fill(n_bjets(), evt_weight); // medium id
      if (jet_pt1() != -100)	hJet1pT[processId]->Fill(jet_pt1(), evt_weight);
      if (jet_pt2() != -100)	hJet2pT[processId]->Fill(jet_pt2(), evt_weight);
      if (jet_pt3() != -100)	hJet3pT[processId]->Fill(jet_pt3(), evt_weight);
      if (jet_pt4() != -100)	hJet4pT[processId]->Fill(jet_pt4(), evt_weight);

      if (jet_pt1() != -100)	hJet1Eta[processId]->Fill(jet_eta1(), evt_weight);
      if (jet_pt2() != -100)	hJet2Eta[processId]->Fill(jet_eta2(), evt_weight);
      if (jet_pt3() != -100)	hJet3Eta[processId]->Fill(jet_eta3(), evt_weight);
      if (jet_pt4() != -100)	hJet4Eta[processId]->Fill(jet_eta4(), evt_weight);

      hbJet1pT[processId]->Fill(bjet1_pt(), evt_weight);
      if (n_bjets() >= 2) 	hbJet2pT[processId]->Fill(bjet2_pt(), evt_weight);

      hMaxBTag[processId]->Fill(bjet1_csv(), evt_weight);
      if (n_bjets() >= 2)	hSecondMaxBTag[processId]->Fill(bjet2_csv(), evt_weight);

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
