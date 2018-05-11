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

      // Blinded region 
      if (isData && blind && mass() > 120 && mass() < 130)        continue;

      // Selection
      if (tag == "ttHLeptonicLoose") {
        if (mass() < 80)        continue;
	if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;
      }
      else if (tag == "ttHLeptonic") {
	if (mass() < 80)        continue;
        if (diphoMVARes() < -0.405)        continue;
        if (leadIDMVA() < -0.9)         continue;
        if (subleadIDMVA() < -0.9)         continue;  
      }
      else if (tag == "ttbar_cr") {
	if (n_jets() < 3) 	continue;
	if (n_bjets() < 2) 	continue;
      }
      else if (tag == "ttbar_cr_loose") {
        if (n_jets() < 3)       continue;
        if (n_bjets() < 2)      continue;
      }


      else {
        cout << "Did not recognize tag name" << endl;
      } 

      // Decide what type of sample this is
      int processId = categorize_process(currentFileTitle);
      int genLeptonId = isData ? -1 : categorize_leptons(nGoodEls(), nGoodMus());
      int genPhotonId = isData ? -1 : categorize_photons(leadGenMatch(), subleadGenMatch());

      // Fill histograms //
      double evt_weight = 1;
      if (!isData)
        evt_weight = scale1fb(currentFileTitle) * targetLumi * sgn(weight());

      cout.setf(ios::fixed);
      cout << std::setprecision(6) << endl;
      if (isData && mass() >= 100 && mass() <= 180)
        cout << mass() << endl;     
 
      // General
      vProcess[processId]->fill_histogram("hMass", mass(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hMassAN", mass(), evt_weight, genLeptonId, genPhotonId);

      // Skip blinded region for MC after filling mass histogram
      if (!isData && blind && mass() > 120 && mass() < 130)     continue;

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

      vProcess[processId]->fill_histogram("hRapidity", dipho_rapidity(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hDiphotonSumPt", dipho_sumpt(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hDiphotonCosPhi", dipho_cosphi(), evt_weight, genLeptonId, genPhotonId);

      vProcess[processId]->fill_histogram("hNVtx", nvtx(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hMetPt", MetPt(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hHT", ht, evt_weight, genLeptonId, genPhotonId);

      vProcess[processId]->fill_histogram("hNJets", n_jets(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hNbJets", n_bjets(), evt_weight, genLeptonId, genPhotonId); // medium id
      if (jet_pt1() != -100)      vProcess[processId]->fill_histogram("hJet1pT", jet_pt1(), evt_weight, genLeptonId, genPhotonId);
      if (jet_pt2() != -100)      vProcess[processId]->fill_histogram("hJet2pT", jet_pt2(), evt_weight, genLeptonId, genPhotonId);
      if (jet_pt3() != -100)      vProcess[processId]->fill_histogram("hJet3pT", jet_pt3(), evt_weight, genLeptonId, genPhotonId);
      if (jet_pt4() != -100)      vProcess[processId]->fill_histogram("hJet4pT", jet_pt4(), evt_weight, genLeptonId, genPhotonId);

      if (jet_pt1() != -100)      vProcess[processId]->fill_histogram("hJet1Eta", jet_eta1(), evt_weight, genLeptonId, genPhotonId);
      if (jet_pt2() != -100)      vProcess[processId]->fill_histogram("hJet2Eta", jet_eta2(), evt_weight, genLeptonId, genPhotonId);
      if (jet_pt3() != -100)      vProcess[processId]->fill_histogram("hJet3Eta", jet_eta3(), evt_weight, genLeptonId, genPhotonId);
      if (jet_pt4() != -100)      vProcess[processId]->fill_histogram("hJet4Eta", jet_eta4(), evt_weight, genLeptonId, genPhotonId);

      if (n_bjets() >= 1)     vProcess[processId]->fill_histogram("hbJet1pT", bjet1_pt(), evt_weight, genLeptonId, genPhotonId);
      if (n_bjets() >= 2)     vProcess[processId]->fill_histogram("hbJet2pT", bjet2_pt(), evt_weight, genLeptonId, genPhotonId);

      if (n_bjets() >= 1)     vProcess[processId]->fill_histogram("hMaxBTag", bjet1_csv(), evt_weight, genLeptonId, genPhotonId);
      if (n_bjets() >= 2)     vProcess[processId]->fill_histogram("hSecondMaxBTag", bjet2_csv(), evt_weight, genLeptonId, genPhotonId);

      // Leading Photon
      vProcess[processId]->fill_histogram("hPhotonLeadPt", leadPt(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonLeadEt", leadEt(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonLeadEta", leadEta(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonLeadPhi", leadPhi(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonLeadSigmaIEtaIEta", lead_sieie(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonLeadHOverE", lead_hoe(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonLeadR9", leadR9(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonLeadIDMVA", leadIDMVA(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonLeadPToM", lead_ptoM(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonLeadSigmaEOverE", lead_sigmaEoE(), evt_weight, genLeptonId, genPhotonId);

      // Subleading Photon
      vProcess[processId]->fill_histogram("hPhotonSubleadPt", subleadPt(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonSubleadEt", subleadEt(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonSubleadEta", subleadEta(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonSubleadPhi", subleadPhi(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonSubleadSigmaIEtaIEta", sublead_sieie(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonSubleadHOverE", sublead_hoe(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonSubleadR9", subleadR9(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonSubleadIDMVA", subleadIDMVA(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonSubleadPToM", sublead_ptoM(), evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonSubleadSigmaEOverE", sublead_sigmaEoE(), evt_weight, genLeptonId, genPhotonId); 

      double maxID = leadIDMVA() >= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      double minID = leadIDMVA() >= subleadIDMVA() ? subleadIDMVA() : leadIDMVA();

      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA", maxID, evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA", minID, evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA_coarse", minID, evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA_coarse", maxID, evt_weight, genLeptonId, genPhotonId);
      vProcess[processId]->fill_histogram("hDiphoMVA", diphoMVARes(), evt_weight, genLeptonId, genPhotonId);
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
