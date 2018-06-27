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

const double targetLumi = 35.9; 

int ScanChain(TChain* chain, TString tag, bool blind = true, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {
  TFile* f1 = new TFile(tag + "_histograms.root", "RECREATE");
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

      // Selection
      if (tag == "ttHHadronicLoose") {
        if (mass() < 100)                continue;
	if (n_jets() < 2)		continue;
	if (nb_loose() < 1)		continue;
	if (!(leadPassEVeto() && subleadPassEVeto()))   continue;
        //if (diphoMVARes() < 0.6)        continue;
        //if (leadIDMVA() < -0.9)         continue;
        //if (subleadIDMVA() < -0.9)              continue;
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
      int processId = categorize_process(currentFileTitle);
      int genLeptonId = isData ? -1 : categorize_leptons(nGoodEls(), nGoodMus());
      int genPhotonId = isData ? -1 : categorize_photons(leadGenMatch(), subleadGenMatch());
      int genPhotonDetailId = isData ? -1 : categorize_photons_detail(lead_photon_type(), sublead_photon_type());
      int photonLocationId = categorize_photon_locations(leadEta(), subleadEta());

      // Fill histograms //
      double evt_weight = 1.;
      if (!isData)
        evt_weight = scale1fb(currentFileTitle) * targetLumi * sgn(weight());

      vProcess[processId]->fill_histogram("hMass", mass(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);   
      vProcess[processId]->fill_histogram("hMassAN", mass(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      cout.setf(ios::fixed);
      cout << std::setprecision(6);
      if (isData && mass() >= 100 && mass() <= 180) {
	cout << mass() << endl;
	//cout << "Mass: " << mass() << " , Event: " << cms3.event() << " , Run: " << run() << " , Lumi: " << lumi() << endl;
      }

      if (isData && (leadIDMVA() < -0.9 || subleadIDMVA() < -0.9)) {
        nLowID++;
      }

      // Skip blinded region for MC after filling mass histogram
      if (!isData && blind && mass() > 120 && mass() < 130)	continue;

      double ht = 0;
      ht += jet1_pt() > 0 ? jet1_pt() : 0;
      ht += jet2_pt() > 0 ? jet2_pt() : 0;
      ht += jet3_pt() > 0 ? jet3_pt() : 0;
      ht += jet4_pt() > 0 ? jet4_pt() : 0;
      ht += jet5_pt() > 0 ? jet5_pt() : 0;
      ht += jet6_pt() > 0 ? jet6_pt() : 0;
      ht += jet7_pt() > 0 ? jet7_pt() : 0;
      ht += jet8_pt() > 0 ? jet8_pt() : 0;
      ht += jet9_pt() > 0 ? jet9_pt() : 0;
      ht += jet10_pt() > 0 ? jet10_pt() : 0;
      ht += jet11_pt() > 0 ? jet11_pt() : 0;
      ht += jet12_pt() > 0 ? jet12_pt() : 0;
      ht += jet13_pt() > 0 ? jet13_pt() : 0;
      ht += jet14_pt() > 0 ? jet14_pt() : 0;
      ht += jet15_pt() > 0 ? jet15_pt() : 0;


      vProcess[processId]->fill_histogram("hRapidity", dipho_rapidity(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hDiphotonSumPt", dipho_sumpt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hDiphotonCosPhi", dipho_cosphi(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      vProcess[processId]->fill_histogram("hNVtx", nvtx(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hMetPt", MetPt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hHT", ht, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      vProcess[processId]->fill_histogram("hNJets", n_jets(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hNbJets", nb_loose(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId); // medium id
      if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("hJet1pT", jet1_pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("hJet2pT", jet2_pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("hJet3pT", jet3_pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("hJet4pT", jet4_pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      if (jet1_pt() != -1)      vProcess[processId]->fill_histogram("hJet1Eta", jet1_eta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet2_pt() != -1)      vProcess[processId]->fill_histogram("hJet2Eta", jet2_eta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet3_pt() != -1)      vProcess[processId]->fill_histogram("hJet3Eta", jet3_eta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (jet4_pt() != -1)      vProcess[processId]->fill_histogram("hJet4Eta", jet4_eta(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      if (bjet1_pt() != -1)     vProcess[processId]->fill_histogram("hbJet1pT", bjet1_pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      if (bjet2_pt() != -1)     vProcess[processId]->fill_histogram("hbJet2pT", bjet2_pt(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

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


      double maxID = leadIDMVA() >= subleadIDMVA() ? leadIDMVA() : subleadIDMVA();
      double minID = leadIDMVA() >= subleadIDMVA() ? subleadIDMVA() : leadIDMVA();

      vProcess[processId]->fill_histogram("hPhotonMaxIDMVA", maxID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hPhotonMinIDMVA", minID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hDiphoMVA", diphoMVARes(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      // ttH-Hadronic Specific
      vProcess[processId]->fill_histogram("hMaxBTag", bjet1_csv(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("hSecondMaxBTag", bjet2_csv(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      vProcess[processId]->fill_histogram("htthMVA", tthMVA(), evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      if (lead_photon_type() == 1)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_prompt", maxID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      else if (lead_photon_type() == 2)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_elec", maxID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      else if (lead_photon_type() == 3)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_fake", maxID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

      if (sublead_photon_type() == 1)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_prompt", maxID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      else if (sublead_photon_type() == 2)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_elec", maxID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);
      else if (sublead_photon_type() == 3)
        vProcess[processId]->fill_histogram("hPhotonIDMVA_fake", maxID, evt_weight, genLeptonId, genPhotonId, genPhotonDetailId, photonLocationId);

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
