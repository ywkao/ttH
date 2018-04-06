#include "ttHLeptonic.h"
ttHLeptonic cms3;

void ttHLeptonic::Init(TTree *tree) {

  tree->SetMakeClass(1);

  candidate_id_branch = tree->GetBranch("candidate_id");
  if (candidate_id_branch) candidate_id_branch->SetAddress(&candidate_id_);
  weight_branch = tree->GetBranch("weight");
  if (weight_branch) weight_branch->SetAddress(&weight_);
  dipho_sumpt_branch = tree->GetBranch("dipho_sumpt");
  if (dipho_sumpt_branch) dipho_sumpt_branch->SetAddress(&dipho_sumpt_);
  dipho_cosphi_branch = tree->GetBranch("dipho_cosphi");
  if (dipho_cosphi_branch) dipho_cosphi_branch->SetAddress(&dipho_cosphi_);
  mass_branch = tree->GetBranch("mass");
  if (mass_branch) mass_branch->SetAddress(&mass_);
  leadPt_branch = tree->GetBranch("leadPt");
  if (leadPt_branch) leadPt_branch->SetAddress(&leadPt_);
  leadEt_branch = tree->GetBranch("leadEt");
  if (leadEt_branch) leadEt_branch->SetAddress(&leadEt_);
  leadEta_branch = tree->GetBranch("leadEta");
  if (leadEta_branch) leadEta_branch->SetAddress(&leadEta_);
  leadPhi_branch = tree->GetBranch("leadPhi");
  if (leadPhi_branch) leadPhi_branch->SetAddress(&leadPhi_);
  lead_sieie_branch = tree->GetBranch("lead_sieie");
  if (lead_sieie_branch) lead_sieie_branch->SetAddress(&lead_sieie_);
  lead_hoe_branch = tree->GetBranch("lead_hoe");
  if (lead_hoe_branch) lead_hoe_branch->SetAddress(&lead_hoe_);
  lead_sigmaEoE_branch = tree->GetBranch("lead_sigmaEoE");
  if (lead_sigmaEoE_branch) lead_sigmaEoE_branch->SetAddress(&lead_sigmaEoE_);
  lead_ptoM_branch = tree->GetBranch("lead_ptoM");
  if (lead_ptoM_branch) lead_ptoM_branch->SetAddress(&lead_ptoM_);
  leadR9_branch = tree->GetBranch("leadR9");
  if (leadR9_branch) leadR9_branch->SetAddress(&leadR9_);
  subleadPt_branch = tree->GetBranch("subleadPt");
  if (subleadPt_branch) subleadPt_branch->SetAddress(&subleadPt_);
  subleadEt_branch = tree->GetBranch("subleadEt");
  if (subleadEt_branch) subleadEt_branch->SetAddress(&subleadEt_);
  subleadEta_branch = tree->GetBranch("subleadEta");
  if (subleadEta_branch) subleadEta_branch->SetAddress(&subleadEta_);
  subleadPhi_branch = tree->GetBranch("subleadPhi");
  if (subleadPhi_branch) subleadPhi_branch->SetAddress(&subleadPhi_);
  sublead_sieie_branch = tree->GetBranch("sublead_sieie");
  if (sublead_sieie_branch) sublead_sieie_branch->SetAddress(&sublead_sieie_);
  sublead_hoe_branch = tree->GetBranch("sublead_hoe");
  if (sublead_hoe_branch) sublead_hoe_branch->SetAddress(&sublead_hoe_);
  sublead_sigmaEoE_branch = tree->GetBranch("sublead_sigmaEoE");
  if (sublead_sigmaEoE_branch) sublead_sigmaEoE_branch->SetAddress(&sublead_sigmaEoE_);
  sublead_ptoM_branch = tree->GetBranch("sublead_ptoM");
  if (sublead_ptoM_branch) sublead_ptoM_branch->SetAddress(&sublead_ptoM_);
  subleadR9_branch = tree->GetBranch("subleadR9");
  if (subleadR9_branch) subleadR9_branch->SetAddress(&subleadR9_);
  leadIDMVA_branch = tree->GetBranch("leadIDMVA");
  if (leadIDMVA_branch) leadIDMVA_branch->SetAddress(&leadIDMVA_);
  subleadIDMVA_branch = tree->GetBranch("subleadIDMVA");
  if (subleadIDMVA_branch) subleadIDMVA_branch->SetAddress(&subleadIDMVA_);
  dipho_rapidity_branch = tree->GetBranch("dipho_rapidity");
  if (dipho_rapidity_branch) dipho_rapidity_branch->SetAddress(&dipho_rapidity_);
  n_ele_branch = tree->GetBranch("n_ele");
  if (n_ele_branch) n_ele_branch->SetAddress(&n_ele_);
  ele1_pt_branch = tree->GetBranch("ele1_pt");
  if (ele1_pt_branch) ele1_pt_branch->SetAddress(&ele1_pt_);
  ele2_pt_branch = tree->GetBranch("ele2_pt");
  if (ele2_pt_branch) ele2_pt_branch->SetAddress(&ele2_pt_);
  n_muons_branch = tree->GetBranch("n_muons");
  if (n_muons_branch) n_muons_branch->SetAddress(&n_muons_);
  muon1_pt_branch = tree->GetBranch("muon1_pt");
  if (muon1_pt_branch) muon1_pt_branch->SetAddress(&muon1_pt_);
  muon2_pt_branch = tree->GetBranch("muon2_pt");
  if (muon2_pt_branch) muon2_pt_branch->SetAddress(&muon2_pt_);
  n_bjets_branch = tree->GetBranch("n_bjets");
  if (n_bjets_branch) n_bjets_branch->SetAddress(&n_bjets_);
  n_jets_branch = tree->GetBranch("n_jets");
  if (n_jets_branch) n_jets_branch->SetAddress(&n_jets_);
  bjet1_pt_branch = tree->GetBranch("bjet1_pt");
  if (bjet1_pt_branch) bjet1_pt_branch->SetAddress(&bjet1_pt_);
  bjet2_pt_branch = tree->GetBranch("bjet2_pt");
  if (bjet2_pt_branch) bjet2_pt_branch->SetAddress(&bjet2_pt_);
  bjet1_csv_branch = tree->GetBranch("bjet1_csv");
  if (bjet1_csv_branch) bjet1_csv_branch->SetAddress(&bjet1_csv_);
  bjet2_csv_branch = tree->GetBranch("bjet2_csv");
  if (bjet2_csv_branch) bjet2_csv_branch->SetAddress(&bjet2_csv_);
  Mjj_branch = tree->GetBranch("Mjj");
  if (Mjj_branch) Mjj_branch->SetAddress(&Mjj_);
  jet_pt1_branch = tree->GetBranch("jet_pt1");
  if (jet_pt1_branch) jet_pt1_branch->SetAddress(&jet_pt1_);
  jet_eta1_branch = tree->GetBranch("jet_eta1");
  if (jet_eta1_branch) jet_eta1_branch->SetAddress(&jet_eta1_);
  jet_phi1_branch = tree->GetBranch("jet_phi1");
  if (jet_phi1_branch) jet_phi1_branch->SetAddress(&jet_phi1_);
  jet_bdiscriminant1_branch = tree->GetBranch("jet_bdiscriminant1");
  if (jet_bdiscriminant1_branch) jet_bdiscriminant1_branch->SetAddress(&jet_bdiscriminant1_);
  jet_pt2_branch = tree->GetBranch("jet_pt2");
  if (jet_pt2_branch) jet_pt2_branch->SetAddress(&jet_pt2_);
  jet_eta2_branch = tree->GetBranch("jet_eta2");
  if (jet_eta2_branch) jet_eta2_branch->SetAddress(&jet_eta2_);
  jet_phi2_branch = tree->GetBranch("jet_phi2");
  if (jet_phi2_branch) jet_phi2_branch->SetAddress(&jet_phi2_);
  jet_bdiscriminant2_branch = tree->GetBranch("jet_bdiscriminant2");
  if (jet_bdiscriminant2_branch) jet_bdiscriminant2_branch->SetAddress(&jet_bdiscriminant2_);
  jet_pt3_branch = tree->GetBranch("jet_pt3");
  if (jet_pt3_branch) jet_pt3_branch->SetAddress(&jet_pt3_);
  jet_eta3_branch = tree->GetBranch("jet_eta3");
  if (jet_eta3_branch) jet_eta3_branch->SetAddress(&jet_eta3_);
  jet_phi3_branch = tree->GetBranch("jet_phi3");
  if (jet_phi3_branch) jet_phi3_branch->SetAddress(&jet_phi3_);
  jet_bdiscriminant3_branch = tree->GetBranch("jet_bdiscriminant3");
  if (jet_bdiscriminant3_branch) jet_bdiscriminant3_branch->SetAddress(&jet_bdiscriminant3_);
  jet_pt4_branch = tree->GetBranch("jet_pt4");
  if (jet_pt4_branch) jet_pt4_branch->SetAddress(&jet_pt4_);
  jet_eta4_branch = tree->GetBranch("jet_eta4");
  if (jet_eta4_branch) jet_eta4_branch->SetAddress(&jet_eta4_);
  jet_phi4_branch = tree->GetBranch("jet_phi4");
  if (jet_phi4_branch) jet_phi4_branch->SetAddress(&jet_phi4_);
  jet_bdiscriminant4_branch = tree->GetBranch("jet_bdiscriminant4");
  if (jet_bdiscriminant4_branch) jet_bdiscriminant4_branch->SetAddress(&jet_bdiscriminant4_);
  jet_pt5_branch = tree->GetBranch("jet_pt5");
  if (jet_pt5_branch) jet_pt5_branch->SetAddress(&jet_pt5_);
  jet_eta5_branch = tree->GetBranch("jet_eta5");
  if (jet_eta5_branch) jet_eta5_branch->SetAddress(&jet_eta5_);
  jet_phi5_branch = tree->GetBranch("jet_phi5");
  if (jet_phi5_branch) jet_phi5_branch->SetAddress(&jet_phi5_);
  jet_bdiscriminant5_branch = tree->GetBranch("jet_bdiscriminant5");
  if (jet_bdiscriminant5_branch) jet_bdiscriminant5_branch->SetAddress(&jet_bdiscriminant5_);
  jet_pt6_branch = tree->GetBranch("jet_pt6");
  if (jet_pt6_branch) jet_pt6_branch->SetAddress(&jet_pt6_);
  jet_eta6_branch = tree->GetBranch("jet_eta6");
  if (jet_eta6_branch) jet_eta6_branch->SetAddress(&jet_eta6_);
  jet_phi6_branch = tree->GetBranch("jet_phi6");
  if (jet_phi6_branch) jet_phi6_branch->SetAddress(&jet_phi6_);
  jet_bdiscriminant6_branch = tree->GetBranch("jet_bdiscriminant6");
  if (jet_bdiscriminant6_branch) jet_bdiscriminant6_branch->SetAddress(&jet_bdiscriminant6_);
  jet_pt7_branch = tree->GetBranch("jet_pt7");
  if (jet_pt7_branch) jet_pt7_branch->SetAddress(&jet_pt7_);
  jet_eta7_branch = tree->GetBranch("jet_eta7");
  if (jet_eta7_branch) jet_eta7_branch->SetAddress(&jet_eta7_);
  jet_phi7_branch = tree->GetBranch("jet_phi7");
  if (jet_phi7_branch) jet_phi7_branch->SetAddress(&jet_phi7_);
  jet_bdiscriminant7_branch = tree->GetBranch("jet_bdiscriminant7");
  if (jet_bdiscriminant7_branch) jet_bdiscriminant7_branch->SetAddress(&jet_bdiscriminant7_);
  jet_pt8_branch = tree->GetBranch("jet_pt8");
  if (jet_pt8_branch) jet_pt8_branch->SetAddress(&jet_pt8_);
  jet_eta8_branch = tree->GetBranch("jet_eta8");
  if (jet_eta8_branch) jet_eta8_branch->SetAddress(&jet_eta8_);
  jet_phi8_branch = tree->GetBranch("jet_phi8");
  if (jet_phi8_branch) jet_phi8_branch->SetAddress(&jet_phi8_);
  jet_bdiscriminant8_branch = tree->GetBranch("jet_bdiscriminant8");
  if (jet_bdiscriminant8_branch) jet_bdiscriminant8_branch->SetAddress(&jet_bdiscriminant8_);
  jet_pt9_branch = tree->GetBranch("jet_pt9");
  if (jet_pt9_branch) jet_pt9_branch->SetAddress(&jet_pt9_);
  jet_eta9_branch = tree->GetBranch("jet_eta9");
  if (jet_eta9_branch) jet_eta9_branch->SetAddress(&jet_eta9_);
  jet_phi9_branch = tree->GetBranch("jet_phi9");
  if (jet_phi9_branch) jet_phi9_branch->SetAddress(&jet_phi9_);
  jet_bdiscriminant9_branch = tree->GetBranch("jet_bdiscriminant9");
  if (jet_bdiscriminant9_branch) jet_bdiscriminant9_branch->SetAddress(&jet_bdiscriminant9_);
  jet_pt10_branch = tree->GetBranch("jet_pt10");
  if (jet_pt10_branch) jet_pt10_branch->SetAddress(&jet_pt10_);
  jet_eta10_branch = tree->GetBranch("jet_eta10");
  if (jet_eta10_branch) jet_eta10_branch->SetAddress(&jet_eta10_);
  jet_phi10_branch = tree->GetBranch("jet_phi10");
  if (jet_phi10_branch) jet_phi10_branch->SetAddress(&jet_phi10_);
  jet_bdiscriminant10_branch = tree->GetBranch("jet_bdiscriminant10");
  if (jet_bdiscriminant10_branch) jet_bdiscriminant10_branch->SetAddress(&jet_bdiscriminant10_);
  jet_pt11_branch = tree->GetBranch("jet_pt11");
  if (jet_pt11_branch) jet_pt11_branch->SetAddress(&jet_pt11_);
  jet_eta11_branch = tree->GetBranch("jet_eta11");
  if (jet_eta11_branch) jet_eta11_branch->SetAddress(&jet_eta11_);
  jet_phi11_branch = tree->GetBranch("jet_phi11");
  if (jet_phi11_branch) jet_phi11_branch->SetAddress(&jet_phi11_);
  jet_bdiscriminant11_branch = tree->GetBranch("jet_bdiscriminant11");
  if (jet_bdiscriminant11_branch) jet_bdiscriminant11_branch->SetAddress(&jet_bdiscriminant11_);
  jet_pt12_branch = tree->GetBranch("jet_pt12");
  if (jet_pt12_branch) jet_pt12_branch->SetAddress(&jet_pt12_);
  jet_eta12_branch = tree->GetBranch("jet_eta12");
  if (jet_eta12_branch) jet_eta12_branch->SetAddress(&jet_eta12_);
  jet_phi12_branch = tree->GetBranch("jet_phi12");
  if (jet_phi12_branch) jet_phi12_branch->SetAddress(&jet_phi12_);
  jet_bdiscriminant12_branch = tree->GetBranch("jet_bdiscriminant12");
  if (jet_bdiscriminant12_branch) jet_bdiscriminant12_branch->SetAddress(&jet_bdiscriminant12_);
  jet_pt13_branch = tree->GetBranch("jet_pt13");
  if (jet_pt13_branch) jet_pt13_branch->SetAddress(&jet_pt13_);
  jet_eta13_branch = tree->GetBranch("jet_eta13");
  if (jet_eta13_branch) jet_eta13_branch->SetAddress(&jet_eta13_);
  jet_phi13_branch = tree->GetBranch("jet_phi13");
  if (jet_phi13_branch) jet_phi13_branch->SetAddress(&jet_phi13_);
  jet_bdiscriminant13_branch = tree->GetBranch("jet_bdiscriminant13");
  if (jet_bdiscriminant13_branch) jet_bdiscriminant13_branch->SetAddress(&jet_bdiscriminant13_);
  jet_pt14_branch = tree->GetBranch("jet_pt14");
  if (jet_pt14_branch) jet_pt14_branch->SetAddress(&jet_pt14_);
  jet_eta14_branch = tree->GetBranch("jet_eta14");
  if (jet_eta14_branch) jet_eta14_branch->SetAddress(&jet_eta14_);
  jet_phi14_branch = tree->GetBranch("jet_phi14");
  if (jet_phi14_branch) jet_phi14_branch->SetAddress(&jet_phi14_);
  jet_bdiscriminant14_branch = tree->GetBranch("jet_bdiscriminant14");
  if (jet_bdiscriminant14_branch) jet_bdiscriminant14_branch->SetAddress(&jet_bdiscriminant14_);
  jet_pt15_branch = tree->GetBranch("jet_pt15");
  if (jet_pt15_branch) jet_pt15_branch->SetAddress(&jet_pt15_);
  jet_eta15_branch = tree->GetBranch("jet_eta15");
  if (jet_eta15_branch) jet_eta15_branch->SetAddress(&jet_eta15_);
  jet_phi15_branch = tree->GetBranch("jet_phi15");
  if (jet_phi15_branch) jet_phi15_branch->SetAddress(&jet_phi15_);
  jet_bdiscriminant15_branch = tree->GetBranch("jet_bdiscriminant15");
  if (jet_bdiscriminant15_branch) jet_bdiscriminant15_branch->SetAddress(&jet_bdiscriminant15_);
  rho_branch = tree->GetBranch("rho");
  if (rho_branch) rho_branch->SetAddress(&rho_);
  nvtx_branch = tree->GetBranch("nvtx");
  if (nvtx_branch) nvtx_branch->SetAddress(&nvtx_);
  event_branch = tree->GetBranch("event");
  if (event_branch) event_branch->SetAddress(&event_);
  lumi_branch = tree->GetBranch("lumi");
  if (lumi_branch) lumi_branch->SetAddress(&lumi_);
  processIndex_branch = tree->GetBranch("processIndex");
  if (processIndex_branch) processIndex_branch->SetAddress(&processIndex_);
  run_branch = tree->GetBranch("run");
  if (run_branch) run_branch->SetAddress(&run_);
  npu_branch = tree->GetBranch("npu");
  if (npu_branch) npu_branch->SetAddress(&npu_);
  puweight_branch = tree->GetBranch("puweight");
  if (puweight_branch) puweight_branch->SetAddress(&puweight_);

  tree->SetMakeClass(0);
}

void ttHLeptonic::GetEntry(unsigned int idx) {
  // this only marks branches as not loaded, saving a lot of time
  index = idx;
  candidate_id_isLoaded = false;
  weight_isLoaded = false;
  dipho_sumpt_isLoaded = false;
  dipho_cosphi_isLoaded = false;
  mass_isLoaded = false;
  leadPt_isLoaded = false;
  leadEt_isLoaded = false;
  leadEta_isLoaded = false;
  leadPhi_isLoaded = false;
  lead_sieie_isLoaded = false;
  lead_hoe_isLoaded = false;
  lead_sigmaEoE_isLoaded = false;
  lead_ptoM_isLoaded = false;
  leadR9_isLoaded = false;
  subleadPt_isLoaded = false;
  subleadEt_isLoaded = false;
  subleadEta_isLoaded = false;
  subleadPhi_isLoaded = false;
  sublead_sieie_isLoaded = false;
  sublead_hoe_isLoaded = false;
  sublead_sigmaEoE_isLoaded = false;
  sublead_ptoM_isLoaded = false;
  subleadR9_isLoaded = false;
  leadIDMVA_isLoaded = false;
  subleadIDMVA_isLoaded = false;
  dipho_rapidity_isLoaded = false;
  n_ele_isLoaded = false;
  ele1_pt_isLoaded = false;
  ele2_pt_isLoaded = false;
  n_muons_isLoaded = false;
  muon1_pt_isLoaded = false;
  muon2_pt_isLoaded = false;
  n_bjets_isLoaded = false;
  n_jets_isLoaded = false;
  bjet1_pt_isLoaded = false;
  bjet2_pt_isLoaded = false;
  bjet1_csv_isLoaded = false;
  bjet2_csv_isLoaded = false;
  Mjj_isLoaded = false;
  jet_pt1_isLoaded = false;
  jet_eta1_isLoaded = false;
  jet_phi1_isLoaded = false;
  jet_bdiscriminant1_isLoaded = false;
  jet_pt2_isLoaded = false;
  jet_eta2_isLoaded = false;
  jet_phi2_isLoaded = false;
  jet_bdiscriminant2_isLoaded = false;
  jet_pt3_isLoaded = false;
  jet_eta3_isLoaded = false;
  jet_phi3_isLoaded = false;
  jet_bdiscriminant3_isLoaded = false;
  jet_pt4_isLoaded = false;
  jet_eta4_isLoaded = false;
  jet_phi4_isLoaded = false;
  jet_bdiscriminant4_isLoaded = false;
  jet_pt5_isLoaded = false;
  jet_eta5_isLoaded = false;
  jet_phi5_isLoaded = false;
  jet_bdiscriminant5_isLoaded = false;
  jet_pt6_isLoaded = false;
  jet_eta6_isLoaded = false;
  jet_phi6_isLoaded = false;
  jet_bdiscriminant6_isLoaded = false;
  jet_pt7_isLoaded = false;
  jet_eta7_isLoaded = false;
  jet_phi7_isLoaded = false;
  jet_bdiscriminant7_isLoaded = false;
  jet_pt8_isLoaded = false;
  jet_eta8_isLoaded = false;
  jet_phi8_isLoaded = false;
  jet_bdiscriminant8_isLoaded = false;
  jet_pt9_isLoaded = false;
  jet_eta9_isLoaded = false;
  jet_phi9_isLoaded = false;
  jet_bdiscriminant9_isLoaded = false;
  jet_pt10_isLoaded = false;
  jet_eta10_isLoaded = false;
  jet_phi10_isLoaded = false;
  jet_bdiscriminant10_isLoaded = false;
  jet_pt11_isLoaded = false;
  jet_eta11_isLoaded = false;
  jet_phi11_isLoaded = false;
  jet_bdiscriminant11_isLoaded = false;
  jet_pt12_isLoaded = false;
  jet_eta12_isLoaded = false;
  jet_phi12_isLoaded = false;
  jet_bdiscriminant12_isLoaded = false;
  jet_pt13_isLoaded = false;
  jet_eta13_isLoaded = false;
  jet_phi13_isLoaded = false;
  jet_bdiscriminant13_isLoaded = false;
  jet_pt14_isLoaded = false;
  jet_eta14_isLoaded = false;
  jet_phi14_isLoaded = false;
  jet_bdiscriminant14_isLoaded = false;
  jet_pt15_isLoaded = false;
  jet_eta15_isLoaded = false;
  jet_phi15_isLoaded = false;
  jet_bdiscriminant15_isLoaded = false;
  rho_isLoaded = false;
  nvtx_isLoaded = false;
  event_isLoaded = false;
  lumi_isLoaded = false;
  processIndex_isLoaded = false;
  run_isLoaded = false;
  npu_isLoaded = false;
  puweight_isLoaded = false;
}

void ttHLeptonic::LoadAllBranches() {
  // load all branches
  if (candidate_id_branch != 0) candidate_id();
  if (weight_branch != 0) weight();
  if (dipho_sumpt_branch != 0) dipho_sumpt();
  if (dipho_cosphi_branch != 0) dipho_cosphi();
  if (mass_branch != 0) mass();
  if (leadPt_branch != 0) leadPt();
  if (leadEt_branch != 0) leadEt();
  if (leadEta_branch != 0) leadEta();
  if (leadPhi_branch != 0) leadPhi();
  if (lead_sieie_branch != 0) lead_sieie();
  if (lead_hoe_branch != 0) lead_hoe();
  if (lead_sigmaEoE_branch != 0) lead_sigmaEoE();
  if (lead_ptoM_branch != 0) lead_ptoM();
  if (leadR9_branch != 0) leadR9();
  if (subleadPt_branch != 0) subleadPt();
  if (subleadEt_branch != 0) subleadEt();
  if (subleadEta_branch != 0) subleadEta();
  if (subleadPhi_branch != 0) subleadPhi();
  if (sublead_sieie_branch != 0) sublead_sieie();
  if (sublead_hoe_branch != 0) sublead_hoe();
  if (sublead_sigmaEoE_branch != 0) sublead_sigmaEoE();
  if (sublead_ptoM_branch != 0) sublead_ptoM();
  if (subleadR9_branch != 0) subleadR9();
  if (leadIDMVA_branch != 0) leadIDMVA();
  if (subleadIDMVA_branch != 0) subleadIDMVA();
  if (dipho_rapidity_branch != 0) dipho_rapidity();
  if (n_ele_branch != 0) n_ele();
  if (ele1_pt_branch != 0) ele1_pt();
  if (ele2_pt_branch != 0) ele2_pt();
  if (n_muons_branch != 0) n_muons();
  if (muon1_pt_branch != 0) muon1_pt();
  if (muon2_pt_branch != 0) muon2_pt();
  if (n_bjets_branch != 0) n_bjets();
  if (n_jets_branch != 0) n_jets();
  if (bjet1_pt_branch != 0) bjet1_pt();
  if (bjet2_pt_branch != 0) bjet2_pt();
  if (bjet1_csv_branch != 0) bjet1_csv();
  if (bjet2_csv_branch != 0) bjet2_csv();
  if (Mjj_branch != 0) Mjj();
  if (jet_pt1_branch != 0) jet_pt1();
  if (jet_eta1_branch != 0) jet_eta1();
  if (jet_phi1_branch != 0) jet_phi1();
  if (jet_bdiscriminant1_branch != 0) jet_bdiscriminant1();
  if (jet_pt2_branch != 0) jet_pt2();
  if (jet_eta2_branch != 0) jet_eta2();
  if (jet_phi2_branch != 0) jet_phi2();
  if (jet_bdiscriminant2_branch != 0) jet_bdiscriminant2();
  if (jet_pt3_branch != 0) jet_pt3();
  if (jet_eta3_branch != 0) jet_eta3();
  if (jet_phi3_branch != 0) jet_phi3();
  if (jet_bdiscriminant3_branch != 0) jet_bdiscriminant3();
  if (jet_pt4_branch != 0) jet_pt4();
  if (jet_eta4_branch != 0) jet_eta4();
  if (jet_phi4_branch != 0) jet_phi4();
  if (jet_bdiscriminant4_branch != 0) jet_bdiscriminant4();
  if (jet_pt5_branch != 0) jet_pt5();
  if (jet_eta5_branch != 0) jet_eta5();
  if (jet_phi5_branch != 0) jet_phi5();
  if (jet_bdiscriminant5_branch != 0) jet_bdiscriminant5();
  if (jet_pt6_branch != 0) jet_pt6();
  if (jet_eta6_branch != 0) jet_eta6();
  if (jet_phi6_branch != 0) jet_phi6();
  if (jet_bdiscriminant6_branch != 0) jet_bdiscriminant6();
  if (jet_pt7_branch != 0) jet_pt7();
  if (jet_eta7_branch != 0) jet_eta7();
  if (jet_phi7_branch != 0) jet_phi7();
  if (jet_bdiscriminant7_branch != 0) jet_bdiscriminant7();
  if (jet_pt8_branch != 0) jet_pt8();
  if (jet_eta8_branch != 0) jet_eta8();
  if (jet_phi8_branch != 0) jet_phi8();
  if (jet_bdiscriminant8_branch != 0) jet_bdiscriminant8();
  if (jet_pt9_branch != 0) jet_pt9();
  if (jet_eta9_branch != 0) jet_eta9();
  if (jet_phi9_branch != 0) jet_phi9();
  if (jet_bdiscriminant9_branch != 0) jet_bdiscriminant9();
  if (jet_pt10_branch != 0) jet_pt10();
  if (jet_eta10_branch != 0) jet_eta10();
  if (jet_phi10_branch != 0) jet_phi10();
  if (jet_bdiscriminant10_branch != 0) jet_bdiscriminant10();
  if (jet_pt11_branch != 0) jet_pt11();
  if (jet_eta11_branch != 0) jet_eta11();
  if (jet_phi11_branch != 0) jet_phi11();
  if (jet_bdiscriminant11_branch != 0) jet_bdiscriminant11();
  if (jet_pt12_branch != 0) jet_pt12();
  if (jet_eta12_branch != 0) jet_eta12();
  if (jet_phi12_branch != 0) jet_phi12();
  if (jet_bdiscriminant12_branch != 0) jet_bdiscriminant12();
  if (jet_pt13_branch != 0) jet_pt13();
  if (jet_eta13_branch != 0) jet_eta13();
  if (jet_phi13_branch != 0) jet_phi13();
  if (jet_bdiscriminant13_branch != 0) jet_bdiscriminant13();
  if (jet_pt14_branch != 0) jet_pt14();
  if (jet_eta14_branch != 0) jet_eta14();
  if (jet_phi14_branch != 0) jet_phi14();
  if (jet_bdiscriminant14_branch != 0) jet_bdiscriminant14();
  if (jet_pt15_branch != 0) jet_pt15();
  if (jet_eta15_branch != 0) jet_eta15();
  if (jet_phi15_branch != 0) jet_phi15();
  if (jet_bdiscriminant15_branch != 0) jet_bdiscriminant15();
  if (rho_branch != 0) rho();
  if (nvtx_branch != 0) nvtx();
  if (event_branch != 0) event();
  if (lumi_branch != 0) lumi();
  if (processIndex_branch != 0) processIndex();
  if (run_branch != 0) run();
  if (npu_branch != 0) npu();
  if (puweight_branch != 0) puweight();
}

const int &ttHLeptonic::candidate_id() {
  if (not candidate_id_isLoaded) {
    if (candidate_id_branch != 0) {
      candidate_id_branch->GetEntry(index);
    } else {
      printf("branch candidate_id_branch does not exist!\n");
      exit(1);
    }
    candidate_id_isLoaded = true;
  }
  return candidate_id_;
}

const float &ttHLeptonic::weight() {
  if (not weight_isLoaded) {
    if (weight_branch != 0) {
      weight_branch->GetEntry(index);
    } else {
      printf("branch weight_branch does not exist!\n");
      exit(1);
    }
    weight_isLoaded = true;
  }
  return weight_;
}

const float &ttHLeptonic::dipho_sumpt() {
  if (not dipho_sumpt_isLoaded) {
    if (dipho_sumpt_branch != 0) {
      dipho_sumpt_branch->GetEntry(index);
    } else {
      printf("branch dipho_sumpt_branch does not exist!\n");
      exit(1);
    }
    dipho_sumpt_isLoaded = true;
  }
  return dipho_sumpt_;
}

const float &ttHLeptonic::dipho_cosphi() {
  if (not dipho_cosphi_isLoaded) {
    if (dipho_cosphi_branch != 0) {
      dipho_cosphi_branch->GetEntry(index);
    } else {
      printf("branch dipho_cosphi_branch does not exist!\n");
      exit(1);
    }
    dipho_cosphi_isLoaded = true;
  }
  return dipho_cosphi_;
}

const float &ttHLeptonic::mass() {
  if (not mass_isLoaded) {
    if (mass_branch != 0) {
      mass_branch->GetEntry(index);
    } else {
      printf("branch mass_branch does not exist!\n");
      exit(1);
    }
    mass_isLoaded = true;
  }
  return mass_;
}

const float &ttHLeptonic::leadPt() {
  if (not leadPt_isLoaded) {
    if (leadPt_branch != 0) {
      leadPt_branch->GetEntry(index);
    } else {
      printf("branch leadPt_branch does not exist!\n");
      exit(1);
    }
    leadPt_isLoaded = true;
  }
  return leadPt_;
}

const float &ttHLeptonic::leadEt() {
  if (not leadEt_isLoaded) {
    if (leadEt_branch != 0) {
      leadEt_branch->GetEntry(index);
    } else {
      printf("branch leadEt_branch does not exist!\n");
      exit(1);
    }
    leadEt_isLoaded = true;
  }
  return leadEt_;
}

const float &ttHLeptonic::leadEta() {
  if (not leadEta_isLoaded) {
    if (leadEta_branch != 0) {
      leadEta_branch->GetEntry(index);
    } else {
      printf("branch leadEta_branch does not exist!\n");
      exit(1);
    }
    leadEta_isLoaded = true;
  }
  return leadEta_;
}

const float &ttHLeptonic::leadPhi() {
  if (not leadPhi_isLoaded) {
    if (leadPhi_branch != 0) {
      leadPhi_branch->GetEntry(index);
    } else {
      printf("branch leadPhi_branch does not exist!\n");
      exit(1);
    }
    leadPhi_isLoaded = true;
  }
  return leadPhi_;
}

const float &ttHLeptonic::lead_sieie() {
  if (not lead_sieie_isLoaded) {
    if (lead_sieie_branch != 0) {
      lead_sieie_branch->GetEntry(index);
    } else {
      printf("branch lead_sieie_branch does not exist!\n");
      exit(1);
    }
    lead_sieie_isLoaded = true;
  }
  return lead_sieie_;
}

const float &ttHLeptonic::lead_hoe() {
  if (not lead_hoe_isLoaded) {
    if (lead_hoe_branch != 0) {
      lead_hoe_branch->GetEntry(index);
    } else {
      printf("branch lead_hoe_branch does not exist!\n");
      exit(1);
    }
    lead_hoe_isLoaded = true;
  }
  return lead_hoe_;
}

const float &ttHLeptonic::lead_sigmaEoE() {
  if (not lead_sigmaEoE_isLoaded) {
    if (lead_sigmaEoE_branch != 0) {
      lead_sigmaEoE_branch->GetEntry(index);
    } else {
      printf("branch lead_sigmaEoE_branch does not exist!\n");
      exit(1);
    }
    lead_sigmaEoE_isLoaded = true;
  }
  return lead_sigmaEoE_;
}

const float &ttHLeptonic::lead_ptoM() {
  if (not lead_ptoM_isLoaded) {
    if (lead_ptoM_branch != 0) {
      lead_ptoM_branch->GetEntry(index);
    } else {
      printf("branch lead_ptoM_branch does not exist!\n");
      exit(1);
    }
    lead_ptoM_isLoaded = true;
  }
  return lead_ptoM_;
}

const float &ttHLeptonic::leadR9() {
  if (not leadR9_isLoaded) {
    if (leadR9_branch != 0) {
      leadR9_branch->GetEntry(index);
    } else {
      printf("branch leadR9_branch does not exist!\n");
      exit(1);
    }
    leadR9_isLoaded = true;
  }
  return leadR9_;
}

const float &ttHLeptonic::subleadPt() {
  if (not subleadPt_isLoaded) {
    if (subleadPt_branch != 0) {
      subleadPt_branch->GetEntry(index);
    } else {
      printf("branch subleadPt_branch does not exist!\n");
      exit(1);
    }
    subleadPt_isLoaded = true;
  }
  return subleadPt_;
}

const float &ttHLeptonic::subleadEt() {
  if (not subleadEt_isLoaded) {
    if (subleadEt_branch != 0) {
      subleadEt_branch->GetEntry(index);
    } else {
      printf("branch subleadEt_branch does not exist!\n");
      exit(1);
    }
    subleadEt_isLoaded = true;
  }
  return subleadEt_;
}

const float &ttHLeptonic::subleadEta() {
  if (not subleadEta_isLoaded) {
    if (subleadEta_branch != 0) {
      subleadEta_branch->GetEntry(index);
    } else {
      printf("branch subleadEta_branch does not exist!\n");
      exit(1);
    }
    subleadEta_isLoaded = true;
  }
  return subleadEta_;
}

const float &ttHLeptonic::subleadPhi() {
  if (not subleadPhi_isLoaded) {
    if (subleadPhi_branch != 0) {
      subleadPhi_branch->GetEntry(index);
    } else {
      printf("branch subleadPhi_branch does not exist!\n");
      exit(1);
    }
    subleadPhi_isLoaded = true;
  }
  return subleadPhi_;
}

const float &ttHLeptonic::sublead_sieie() {
  if (not sublead_sieie_isLoaded) {
    if (sublead_sieie_branch != 0) {
      sublead_sieie_branch->GetEntry(index);
    } else {
      printf("branch sublead_sieie_branch does not exist!\n");
      exit(1);
    }
    sublead_sieie_isLoaded = true;
  }
  return sublead_sieie_;
}

const float &ttHLeptonic::sublead_hoe() {
  if (not sublead_hoe_isLoaded) {
    if (sublead_hoe_branch != 0) {
      sublead_hoe_branch->GetEntry(index);
    } else {
      printf("branch sublead_hoe_branch does not exist!\n");
      exit(1);
    }
    sublead_hoe_isLoaded = true;
  }
  return sublead_hoe_;
}

const float &ttHLeptonic::sublead_sigmaEoE() {
  if (not sublead_sigmaEoE_isLoaded) {
    if (sublead_sigmaEoE_branch != 0) {
      sublead_sigmaEoE_branch->GetEntry(index);
    } else {
      printf("branch sublead_sigmaEoE_branch does not exist!\n");
      exit(1);
    }
    sublead_sigmaEoE_isLoaded = true;
  }
  return sublead_sigmaEoE_;
}

const float &ttHLeptonic::sublead_ptoM() {
  if (not sublead_ptoM_isLoaded) {
    if (sublead_ptoM_branch != 0) {
      sublead_ptoM_branch->GetEntry(index);
    } else {
      printf("branch sublead_ptoM_branch does not exist!\n");
      exit(1);
    }
    sublead_ptoM_isLoaded = true;
  }
  return sublead_ptoM_;
}

const float &ttHLeptonic::subleadR9() {
  if (not subleadR9_isLoaded) {
    if (subleadR9_branch != 0) {
      subleadR9_branch->GetEntry(index);
    } else {
      printf("branch subleadR9_branch does not exist!\n");
      exit(1);
    }
    subleadR9_isLoaded = true;
  }
  return subleadR9_;
}

const float &ttHLeptonic::leadIDMVA() {
  if (not leadIDMVA_isLoaded) {
    if (leadIDMVA_branch != 0) {
      leadIDMVA_branch->GetEntry(index);
    } else {
      printf("branch leadIDMVA_branch does not exist!\n");
      exit(1);
    }
    leadIDMVA_isLoaded = true;
  }
  return leadIDMVA_;
}

const float &ttHLeptonic::subleadIDMVA() {
  if (not subleadIDMVA_isLoaded) {
    if (subleadIDMVA_branch != 0) {
      subleadIDMVA_branch->GetEntry(index);
    } else {
      printf("branch subleadIDMVA_branch does not exist!\n");
      exit(1);
    }
    subleadIDMVA_isLoaded = true;
  }
  return subleadIDMVA_;
}

const float &ttHLeptonic::dipho_rapidity() {
  if (not dipho_rapidity_isLoaded) {
    if (dipho_rapidity_branch != 0) {
      dipho_rapidity_branch->GetEntry(index);
    } else {
      printf("branch dipho_rapidity_branch does not exist!\n");
      exit(1);
    }
    dipho_rapidity_isLoaded = true;
  }
  return dipho_rapidity_;
}

const float &ttHLeptonic::n_ele() {
  if (not n_ele_isLoaded) {
    if (n_ele_branch != 0) {
      n_ele_branch->GetEntry(index);
    } else {
      printf("branch n_ele_branch does not exist!\n");
      exit(1);
    }
    n_ele_isLoaded = true;
  }
  return n_ele_;
}

const float &ttHLeptonic::ele1_pt() {
  if (not ele1_pt_isLoaded) {
    if (ele1_pt_branch != 0) {
      ele1_pt_branch->GetEntry(index);
    } else {
      printf("branch ele1_pt_branch does not exist!\n");
      exit(1);
    }
    ele1_pt_isLoaded = true;
  }
  return ele1_pt_;
}

const float &ttHLeptonic::ele2_pt() {
  if (not ele2_pt_isLoaded) {
    if (ele2_pt_branch != 0) {
      ele2_pt_branch->GetEntry(index);
    } else {
      printf("branch ele2_pt_branch does not exist!\n");
      exit(1);
    }
    ele2_pt_isLoaded = true;
  }
  return ele2_pt_;
}

const float &ttHLeptonic::n_muons() {
  if (not n_muons_isLoaded) {
    if (n_muons_branch != 0) {
      n_muons_branch->GetEntry(index);
    } else {
      printf("branch n_muons_branch does not exist!\n");
      exit(1);
    }
    n_muons_isLoaded = true;
  }
  return n_muons_;
}

const float &ttHLeptonic::muon1_pt() {
  if (not muon1_pt_isLoaded) {
    if (muon1_pt_branch != 0) {
      muon1_pt_branch->GetEntry(index);
    } else {
      printf("branch muon1_pt_branch does not exist!\n");
      exit(1);
    }
    muon1_pt_isLoaded = true;
  }
  return muon1_pt_;
}

const float &ttHLeptonic::muon2_pt() {
  if (not muon2_pt_isLoaded) {
    if (muon2_pt_branch != 0) {
      muon2_pt_branch->GetEntry(index);
    } else {
      printf("branch muon2_pt_branch does not exist!\n");
      exit(1);
    }
    muon2_pt_isLoaded = true;
  }
  return muon2_pt_;
}

const float &ttHLeptonic::n_bjets() {
  if (not n_bjets_isLoaded) {
    if (n_bjets_branch != 0) {
      n_bjets_branch->GetEntry(index);
    } else {
      printf("branch n_bjets_branch does not exist!\n");
      exit(1);
    }
    n_bjets_isLoaded = true;
  }
  return n_bjets_;
}

const float &ttHLeptonic::n_jets() {
  if (not n_jets_isLoaded) {
    if (n_jets_branch != 0) {
      n_jets_branch->GetEntry(index);
    } else {
      printf("branch n_jets_branch does not exist!\n");
      exit(1);
    }
    n_jets_isLoaded = true;
  }
  return n_jets_;
}

const float &ttHLeptonic::bjet1_pt() {
  if (not bjet1_pt_isLoaded) {
    if (bjet1_pt_branch != 0) {
      bjet1_pt_branch->GetEntry(index);
    } else {
      printf("branch bjet1_pt_branch does not exist!\n");
      exit(1);
    }
    bjet1_pt_isLoaded = true;
  }
  return bjet1_pt_;
}

const float &ttHLeptonic::bjet2_pt() {
  if (not bjet2_pt_isLoaded) {
    if (bjet2_pt_branch != 0) {
      bjet2_pt_branch->GetEntry(index);
    } else {
      printf("branch bjet2_pt_branch does not exist!\n");
      exit(1);
    }
    bjet2_pt_isLoaded = true;
  }
  return bjet2_pt_;
}

const float &ttHLeptonic::bjet1_csv() {
  if (not bjet1_csv_isLoaded) {
    if (bjet1_csv_branch != 0) {
      bjet1_csv_branch->GetEntry(index);
    } else {
      printf("branch bjet1_csv_branch does not exist!\n");
      exit(1);
    }
    bjet1_csv_isLoaded = true;
  }
  return bjet1_csv_;
}

const float &ttHLeptonic::bjet2_csv() {
  if (not bjet2_csv_isLoaded) {
    if (bjet2_csv_branch != 0) {
      bjet2_csv_branch->GetEntry(index);
    } else {
      printf("branch bjet2_csv_branch does not exist!\n");
      exit(1);
    }
    bjet2_csv_isLoaded = true;
  }
  return bjet2_csv_;
}

const float &ttHLeptonic::Mjj() {
  if (not Mjj_isLoaded) {
    if (Mjj_branch != 0) {
      Mjj_branch->GetEntry(index);
    } else {
      printf("branch Mjj_branch does not exist!\n");
      exit(1);
    }
    Mjj_isLoaded = true;
  }
  return Mjj_;
}

const float &ttHLeptonic::jet_pt1() {
  if (not jet_pt1_isLoaded) {
    if (jet_pt1_branch != 0) {
      jet_pt1_branch->GetEntry(index);
    } else {
      printf("branch jet_pt1_branch does not exist!\n");
      exit(1);
    }
    jet_pt1_isLoaded = true;
  }
  return jet_pt1_;
}

const float &ttHLeptonic::jet_eta1() {
  if (not jet_eta1_isLoaded) {
    if (jet_eta1_branch != 0) {
      jet_eta1_branch->GetEntry(index);
    } else {
      printf("branch jet_eta1_branch does not exist!\n");
      exit(1);
    }
    jet_eta1_isLoaded = true;
  }
  return jet_eta1_;
}

const float &ttHLeptonic::jet_phi1() {
  if (not jet_phi1_isLoaded) {
    if (jet_phi1_branch != 0) {
      jet_phi1_branch->GetEntry(index);
    } else {
      printf("branch jet_phi1_branch does not exist!\n");
      exit(1);
    }
    jet_phi1_isLoaded = true;
  }
  return jet_phi1_;
}

const float &ttHLeptonic::jet_bdiscriminant1() {
  if (not jet_bdiscriminant1_isLoaded) {
    if (jet_bdiscriminant1_branch != 0) {
      jet_bdiscriminant1_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant1_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant1_isLoaded = true;
  }
  return jet_bdiscriminant1_;
}

const float &ttHLeptonic::jet_pt2() {
  if (not jet_pt2_isLoaded) {
    if (jet_pt2_branch != 0) {
      jet_pt2_branch->GetEntry(index);
    } else {
      printf("branch jet_pt2_branch does not exist!\n");
      exit(1);
    }
    jet_pt2_isLoaded = true;
  }
  return jet_pt2_;
}

const float &ttHLeptonic::jet_eta2() {
  if (not jet_eta2_isLoaded) {
    if (jet_eta2_branch != 0) {
      jet_eta2_branch->GetEntry(index);
    } else {
      printf("branch jet_eta2_branch does not exist!\n");
      exit(1);
    }
    jet_eta2_isLoaded = true;
  }
  return jet_eta2_;
}

const float &ttHLeptonic::jet_phi2() {
  if (not jet_phi2_isLoaded) {
    if (jet_phi2_branch != 0) {
      jet_phi2_branch->GetEntry(index);
    } else {
      printf("branch jet_phi2_branch does not exist!\n");
      exit(1);
    }
    jet_phi2_isLoaded = true;
  }
  return jet_phi2_;
}

const float &ttHLeptonic::jet_bdiscriminant2() {
  if (not jet_bdiscriminant2_isLoaded) {
    if (jet_bdiscriminant2_branch != 0) {
      jet_bdiscriminant2_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant2_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant2_isLoaded = true;
  }
  return jet_bdiscriminant2_;
}

const float &ttHLeptonic::jet_pt3() {
  if (not jet_pt3_isLoaded) {
    if (jet_pt3_branch != 0) {
      jet_pt3_branch->GetEntry(index);
    } else {
      printf("branch jet_pt3_branch does not exist!\n");
      exit(1);
    }
    jet_pt3_isLoaded = true;
  }
  return jet_pt3_;
}

const float &ttHLeptonic::jet_eta3() {
  if (not jet_eta3_isLoaded) {
    if (jet_eta3_branch != 0) {
      jet_eta3_branch->GetEntry(index);
    } else {
      printf("branch jet_eta3_branch does not exist!\n");
      exit(1);
    }
    jet_eta3_isLoaded = true;
  }
  return jet_eta3_;
}

const float &ttHLeptonic::jet_phi3() {
  if (not jet_phi3_isLoaded) {
    if (jet_phi3_branch != 0) {
      jet_phi3_branch->GetEntry(index);
    } else {
      printf("branch jet_phi3_branch does not exist!\n");
      exit(1);
    }
    jet_phi3_isLoaded = true;
  }
  return jet_phi3_;
}

const float &ttHLeptonic::jet_bdiscriminant3() {
  if (not jet_bdiscriminant3_isLoaded) {
    if (jet_bdiscriminant3_branch != 0) {
      jet_bdiscriminant3_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant3_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant3_isLoaded = true;
  }
  return jet_bdiscriminant3_;
}

const float &ttHLeptonic::jet_pt4() {
  if (not jet_pt4_isLoaded) {
    if (jet_pt4_branch != 0) {
      jet_pt4_branch->GetEntry(index);
    } else {
      printf("branch jet_pt4_branch does not exist!\n");
      exit(1);
    }
    jet_pt4_isLoaded = true;
  }
  return jet_pt4_;
}

const float &ttHLeptonic::jet_eta4() {
  if (not jet_eta4_isLoaded) {
    if (jet_eta4_branch != 0) {
      jet_eta4_branch->GetEntry(index);
    } else {
      printf("branch jet_eta4_branch does not exist!\n");
      exit(1);
    }
    jet_eta4_isLoaded = true;
  }
  return jet_eta4_;
}

const float &ttHLeptonic::jet_phi4() {
  if (not jet_phi4_isLoaded) {
    if (jet_phi4_branch != 0) {
      jet_phi4_branch->GetEntry(index);
    } else {
      printf("branch jet_phi4_branch does not exist!\n");
      exit(1);
    }
    jet_phi4_isLoaded = true;
  }
  return jet_phi4_;
}

const float &ttHLeptonic::jet_bdiscriminant4() {
  if (not jet_bdiscriminant4_isLoaded) {
    if (jet_bdiscriminant4_branch != 0) {
      jet_bdiscriminant4_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant4_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant4_isLoaded = true;
  }
  return jet_bdiscriminant4_;
}

const float &ttHLeptonic::jet_pt5() {
  if (not jet_pt5_isLoaded) {
    if (jet_pt5_branch != 0) {
      jet_pt5_branch->GetEntry(index);
    } else {
      printf("branch jet_pt5_branch does not exist!\n");
      exit(1);
    }
    jet_pt5_isLoaded = true;
  }
  return jet_pt5_;
}

const float &ttHLeptonic::jet_eta5() {
  if (not jet_eta5_isLoaded) {
    if (jet_eta5_branch != 0) {
      jet_eta5_branch->GetEntry(index);
    } else {
      printf("branch jet_eta5_branch does not exist!\n");
      exit(1);
    }
    jet_eta5_isLoaded = true;
  }
  return jet_eta5_;
}

const float &ttHLeptonic::jet_phi5() {
  if (not jet_phi5_isLoaded) {
    if (jet_phi5_branch != 0) {
      jet_phi5_branch->GetEntry(index);
    } else {
      printf("branch jet_phi5_branch does not exist!\n");
      exit(1);
    }
    jet_phi5_isLoaded = true;
  }
  return jet_phi5_;
}

const float &ttHLeptonic::jet_bdiscriminant5() {
  if (not jet_bdiscriminant5_isLoaded) {
    if (jet_bdiscriminant5_branch != 0) {
      jet_bdiscriminant5_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant5_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant5_isLoaded = true;
  }
  return jet_bdiscriminant5_;
}

const float &ttHLeptonic::jet_pt6() {
  if (not jet_pt6_isLoaded) {
    if (jet_pt6_branch != 0) {
      jet_pt6_branch->GetEntry(index);
    } else {
      printf("branch jet_pt6_branch does not exist!\n");
      exit(1);
    }
    jet_pt6_isLoaded = true;
  }
  return jet_pt6_;
}

const float &ttHLeptonic::jet_eta6() {
  if (not jet_eta6_isLoaded) {
    if (jet_eta6_branch != 0) {
      jet_eta6_branch->GetEntry(index);
    } else {
      printf("branch jet_eta6_branch does not exist!\n");
      exit(1);
    }
    jet_eta6_isLoaded = true;
  }
  return jet_eta6_;
}

const float &ttHLeptonic::jet_phi6() {
  if (not jet_phi6_isLoaded) {
    if (jet_phi6_branch != 0) {
      jet_phi6_branch->GetEntry(index);
    } else {
      printf("branch jet_phi6_branch does not exist!\n");
      exit(1);
    }
    jet_phi6_isLoaded = true;
  }
  return jet_phi6_;
}

const float &ttHLeptonic::jet_bdiscriminant6() {
  if (not jet_bdiscriminant6_isLoaded) {
    if (jet_bdiscriminant6_branch != 0) {
      jet_bdiscriminant6_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant6_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant6_isLoaded = true;
  }
  return jet_bdiscriminant6_;
}

const float &ttHLeptonic::jet_pt7() {
  if (not jet_pt7_isLoaded) {
    if (jet_pt7_branch != 0) {
      jet_pt7_branch->GetEntry(index);
    } else {
      printf("branch jet_pt7_branch does not exist!\n");
      exit(1);
    }
    jet_pt7_isLoaded = true;
  }
  return jet_pt7_;
}

const float &ttHLeptonic::jet_eta7() {
  if (not jet_eta7_isLoaded) {
    if (jet_eta7_branch != 0) {
      jet_eta7_branch->GetEntry(index);
    } else {
      printf("branch jet_eta7_branch does not exist!\n");
      exit(1);
    }
    jet_eta7_isLoaded = true;
  }
  return jet_eta7_;
}

const float &ttHLeptonic::jet_phi7() {
  if (not jet_phi7_isLoaded) {
    if (jet_phi7_branch != 0) {
      jet_phi7_branch->GetEntry(index);
    } else {
      printf("branch jet_phi7_branch does not exist!\n");
      exit(1);
    }
    jet_phi7_isLoaded = true;
  }
  return jet_phi7_;
}

const float &ttHLeptonic::jet_bdiscriminant7() {
  if (not jet_bdiscriminant7_isLoaded) {
    if (jet_bdiscriminant7_branch != 0) {
      jet_bdiscriminant7_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant7_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant7_isLoaded = true;
  }
  return jet_bdiscriminant7_;
}

const float &ttHLeptonic::jet_pt8() {
  if (not jet_pt8_isLoaded) {
    if (jet_pt8_branch != 0) {
      jet_pt8_branch->GetEntry(index);
    } else {
      printf("branch jet_pt8_branch does not exist!\n");
      exit(1);
    }
    jet_pt8_isLoaded = true;
  }
  return jet_pt8_;
}

const float &ttHLeptonic::jet_eta8() {
  if (not jet_eta8_isLoaded) {
    if (jet_eta8_branch != 0) {
      jet_eta8_branch->GetEntry(index);
    } else {
      printf("branch jet_eta8_branch does not exist!\n");
      exit(1);
    }
    jet_eta8_isLoaded = true;
  }
  return jet_eta8_;
}

const float &ttHLeptonic::jet_phi8() {
  if (not jet_phi8_isLoaded) {
    if (jet_phi8_branch != 0) {
      jet_phi8_branch->GetEntry(index);
    } else {
      printf("branch jet_phi8_branch does not exist!\n");
      exit(1);
    }
    jet_phi8_isLoaded = true;
  }
  return jet_phi8_;
}

const float &ttHLeptonic::jet_bdiscriminant8() {
  if (not jet_bdiscriminant8_isLoaded) {
    if (jet_bdiscriminant8_branch != 0) {
      jet_bdiscriminant8_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant8_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant8_isLoaded = true;
  }
  return jet_bdiscriminant8_;
}

const float &ttHLeptonic::jet_pt9() {
  if (not jet_pt9_isLoaded) {
    if (jet_pt9_branch != 0) {
      jet_pt9_branch->GetEntry(index);
    } else {
      printf("branch jet_pt9_branch does not exist!\n");
      exit(1);
    }
    jet_pt9_isLoaded = true;
  }
  return jet_pt9_;
}

const float &ttHLeptonic::jet_eta9() {
  if (not jet_eta9_isLoaded) {
    if (jet_eta9_branch != 0) {
      jet_eta9_branch->GetEntry(index);
    } else {
      printf("branch jet_eta9_branch does not exist!\n");
      exit(1);
    }
    jet_eta9_isLoaded = true;
  }
  return jet_eta9_;
}

const float &ttHLeptonic::jet_phi9() {
  if (not jet_phi9_isLoaded) {
    if (jet_phi9_branch != 0) {
      jet_phi9_branch->GetEntry(index);
    } else {
      printf("branch jet_phi9_branch does not exist!\n");
      exit(1);
    }
    jet_phi9_isLoaded = true;
  }
  return jet_phi9_;
}

const float &ttHLeptonic::jet_bdiscriminant9() {
  if (not jet_bdiscriminant9_isLoaded) {
    if (jet_bdiscriminant9_branch != 0) {
      jet_bdiscriminant9_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant9_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant9_isLoaded = true;
  }
  return jet_bdiscriminant9_;
}

const float &ttHLeptonic::jet_pt10() {
  if (not jet_pt10_isLoaded) {
    if (jet_pt10_branch != 0) {
      jet_pt10_branch->GetEntry(index);
    } else {
      printf("branch jet_pt10_branch does not exist!\n");
      exit(1);
    }
    jet_pt10_isLoaded = true;
  }
  return jet_pt10_;
}

const float &ttHLeptonic::jet_eta10() {
  if (not jet_eta10_isLoaded) {
    if (jet_eta10_branch != 0) {
      jet_eta10_branch->GetEntry(index);
    } else {
      printf("branch jet_eta10_branch does not exist!\n");
      exit(1);
    }
    jet_eta10_isLoaded = true;
  }
  return jet_eta10_;
}

const float &ttHLeptonic::jet_phi10() {
  if (not jet_phi10_isLoaded) {
    if (jet_phi10_branch != 0) {
      jet_phi10_branch->GetEntry(index);
    } else {
      printf("branch jet_phi10_branch does not exist!\n");
      exit(1);
    }
    jet_phi10_isLoaded = true;
  }
  return jet_phi10_;
}

const float &ttHLeptonic::jet_bdiscriminant10() {
  if (not jet_bdiscriminant10_isLoaded) {
    if (jet_bdiscriminant10_branch != 0) {
      jet_bdiscriminant10_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant10_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant10_isLoaded = true;
  }
  return jet_bdiscriminant10_;
}

const float &ttHLeptonic::jet_pt11() {
  if (not jet_pt11_isLoaded) {
    if (jet_pt11_branch != 0) {
      jet_pt11_branch->GetEntry(index);
    } else {
      printf("branch jet_pt11_branch does not exist!\n");
      exit(1);
    }
    jet_pt11_isLoaded = true;
  }
  return jet_pt11_;
}

const float &ttHLeptonic::jet_eta11() {
  if (not jet_eta11_isLoaded) {
    if (jet_eta11_branch != 0) {
      jet_eta11_branch->GetEntry(index);
    } else {
      printf("branch jet_eta11_branch does not exist!\n");
      exit(1);
    }
    jet_eta11_isLoaded = true;
  }
  return jet_eta11_;
}

const float &ttHLeptonic::jet_phi11() {
  if (not jet_phi11_isLoaded) {
    if (jet_phi11_branch != 0) {
      jet_phi11_branch->GetEntry(index);
    } else {
      printf("branch jet_phi11_branch does not exist!\n");
      exit(1);
    }
    jet_phi11_isLoaded = true;
  }
  return jet_phi11_;
}

const float &ttHLeptonic::jet_bdiscriminant11() {
  if (not jet_bdiscriminant11_isLoaded) {
    if (jet_bdiscriminant11_branch != 0) {
      jet_bdiscriminant11_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant11_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant11_isLoaded = true;
  }
  return jet_bdiscriminant11_;
}

const float &ttHLeptonic::jet_pt12() {
  if (not jet_pt12_isLoaded) {
    if (jet_pt12_branch != 0) {
      jet_pt12_branch->GetEntry(index);
    } else {
      printf("branch jet_pt12_branch does not exist!\n");
      exit(1);
    }
    jet_pt12_isLoaded = true;
  }
  return jet_pt12_;
}

const float &ttHLeptonic::jet_eta12() {
  if (not jet_eta12_isLoaded) {
    if (jet_eta12_branch != 0) {
      jet_eta12_branch->GetEntry(index);
    } else {
      printf("branch jet_eta12_branch does not exist!\n");
      exit(1);
    }
    jet_eta12_isLoaded = true;
  }
  return jet_eta12_;
}

const float &ttHLeptonic::jet_phi12() {
  if (not jet_phi12_isLoaded) {
    if (jet_phi12_branch != 0) {
      jet_phi12_branch->GetEntry(index);
    } else {
      printf("branch jet_phi12_branch does not exist!\n");
      exit(1);
    }
    jet_phi12_isLoaded = true;
  }
  return jet_phi12_;
}

const float &ttHLeptonic::jet_bdiscriminant12() {
  if (not jet_bdiscriminant12_isLoaded) {
    if (jet_bdiscriminant12_branch != 0) {
      jet_bdiscriminant12_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant12_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant12_isLoaded = true;
  }
  return jet_bdiscriminant12_;
}

const float &ttHLeptonic::jet_pt13() {
  if (not jet_pt13_isLoaded) {
    if (jet_pt13_branch != 0) {
      jet_pt13_branch->GetEntry(index);
    } else {
      printf("branch jet_pt13_branch does not exist!\n");
      exit(1);
    }
    jet_pt13_isLoaded = true;
  }
  return jet_pt13_;
}

const float &ttHLeptonic::jet_eta13() {
  if (not jet_eta13_isLoaded) {
    if (jet_eta13_branch != 0) {
      jet_eta13_branch->GetEntry(index);
    } else {
      printf("branch jet_eta13_branch does not exist!\n");
      exit(1);
    }
    jet_eta13_isLoaded = true;
  }
  return jet_eta13_;
}

const float &ttHLeptonic::jet_phi13() {
  if (not jet_phi13_isLoaded) {
    if (jet_phi13_branch != 0) {
      jet_phi13_branch->GetEntry(index);
    } else {
      printf("branch jet_phi13_branch does not exist!\n");
      exit(1);
    }
    jet_phi13_isLoaded = true;
  }
  return jet_phi13_;
}

const float &ttHLeptonic::jet_bdiscriminant13() {
  if (not jet_bdiscriminant13_isLoaded) {
    if (jet_bdiscriminant13_branch != 0) {
      jet_bdiscriminant13_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant13_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant13_isLoaded = true;
  }
  return jet_bdiscriminant13_;
}

const float &ttHLeptonic::jet_pt14() {
  if (not jet_pt14_isLoaded) {
    if (jet_pt14_branch != 0) {
      jet_pt14_branch->GetEntry(index);
    } else {
      printf("branch jet_pt14_branch does not exist!\n");
      exit(1);
    }
    jet_pt14_isLoaded = true;
  }
  return jet_pt14_;
}

const float &ttHLeptonic::jet_eta14() {
  if (not jet_eta14_isLoaded) {
    if (jet_eta14_branch != 0) {
      jet_eta14_branch->GetEntry(index);
    } else {
      printf("branch jet_eta14_branch does not exist!\n");
      exit(1);
    }
    jet_eta14_isLoaded = true;
  }
  return jet_eta14_;
}

const float &ttHLeptonic::jet_phi14() {
  if (not jet_phi14_isLoaded) {
    if (jet_phi14_branch != 0) {
      jet_phi14_branch->GetEntry(index);
    } else {
      printf("branch jet_phi14_branch does not exist!\n");
      exit(1);
    }
    jet_phi14_isLoaded = true;
  }
  return jet_phi14_;
}

const float &ttHLeptonic::jet_bdiscriminant14() {
  if (not jet_bdiscriminant14_isLoaded) {
    if (jet_bdiscriminant14_branch != 0) {
      jet_bdiscriminant14_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant14_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant14_isLoaded = true;
  }
  return jet_bdiscriminant14_;
}

const float &ttHLeptonic::jet_pt15() {
  if (not jet_pt15_isLoaded) {
    if (jet_pt15_branch != 0) {
      jet_pt15_branch->GetEntry(index);
    } else {
      printf("branch jet_pt15_branch does not exist!\n");
      exit(1);
    }
    jet_pt15_isLoaded = true;
  }
  return jet_pt15_;
}

const float &ttHLeptonic::jet_eta15() {
  if (not jet_eta15_isLoaded) {
    if (jet_eta15_branch != 0) {
      jet_eta15_branch->GetEntry(index);
    } else {
      printf("branch jet_eta15_branch does not exist!\n");
      exit(1);
    }
    jet_eta15_isLoaded = true;
  }
  return jet_eta15_;
}

const float &ttHLeptonic::jet_phi15() {
  if (not jet_phi15_isLoaded) {
    if (jet_phi15_branch != 0) {
      jet_phi15_branch->GetEntry(index);
    } else {
      printf("branch jet_phi15_branch does not exist!\n");
      exit(1);
    }
    jet_phi15_isLoaded = true;
  }
  return jet_phi15_;
}

const float &ttHLeptonic::jet_bdiscriminant15() {
  if (not jet_bdiscriminant15_isLoaded) {
    if (jet_bdiscriminant15_branch != 0) {
      jet_bdiscriminant15_branch->GetEntry(index);
    } else {
      printf("branch jet_bdiscriminant15_branch does not exist!\n");
      exit(1);
    }
    jet_bdiscriminant15_isLoaded = true;
  }
  return jet_bdiscriminant15_;
}

const float &ttHLeptonic::rho() {
  if (not rho_isLoaded) {
    if (rho_branch != 0) {
      rho_branch->GetEntry(index);
    } else {
      printf("branch rho_branch does not exist!\n");
      exit(1);
    }
    rho_isLoaded = true;
  }
  return rho_;
}

const int &ttHLeptonic::nvtx() {
  if (not nvtx_isLoaded) {
    if (nvtx_branch != 0) {
      nvtx_branch->GetEntry(index);
    } else {
      printf("branch nvtx_branch does not exist!\n");
      exit(1);
    }
    nvtx_isLoaded = true;
  }
  return nvtx_;
}

const unsigned long long &ttHLeptonic::event() {
  if (not event_isLoaded) {
    if (event_branch != 0) {
      event_branch->GetEntry(index);
    } else {
      printf("branch event_branch does not exist!\n");
      exit(1);
    }
    event_isLoaded = true;
  }
  return event_;
}

const unsigned int &ttHLeptonic::lumi() {
  if (not lumi_isLoaded) {
    if (lumi_branch != 0) {
      lumi_branch->GetEntry(index);
    } else {
      printf("branch lumi_branch does not exist!\n");
      exit(1);
    }
    lumi_isLoaded = true;
  }
  return lumi_;
}

const int &ttHLeptonic::processIndex() {
  if (not processIndex_isLoaded) {
    if (processIndex_branch != 0) {
      processIndex_branch->GetEntry(index);
    } else {
      printf("branch processIndex_branch does not exist!\n");
      exit(1);
    }
    processIndex_isLoaded = true;
  }
  return processIndex_;
}

const unsigned int &ttHLeptonic::run() {
  if (not run_isLoaded) {
    if (run_branch != 0) {
      run_branch->GetEntry(index);
    } else {
      printf("branch run_branch does not exist!\n");
      exit(1);
    }
    run_isLoaded = true;
  }
  return run_;
}

const float &ttHLeptonic::npu() {
  if (not npu_isLoaded) {
    if (npu_branch != 0) {
      npu_branch->GetEntry(index);
    } else {
      printf("branch npu_branch does not exist!\n");
      exit(1);
    }
    npu_isLoaded = true;
  }
  return npu_;
}

const float &ttHLeptonic::puweight() {
  if (not puweight_isLoaded) {
    if (puweight_branch != 0) {
      puweight_branch->GetEntry(index);
    } else {
      printf("branch puweight_branch does not exist!\n");
      exit(1);
    }
    puweight_isLoaded = true;
  }
  return puweight_;
}


void ttHLeptonic::progress( int nEventsTotal, int nEventsChain ){
  int period = 1000;
  if (nEventsTotal%1000 == 0) {
    // xterm magic from L. Vacavant and A. Cerri
    if (isatty(1)) {
      if ((nEventsChain - nEventsTotal) > period) {
        float frac = (float)nEventsTotal/(nEventsChain*0.01);
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
             "\033[0m\033[32m <---\033[0m\015", frac);
        fflush(stdout);
      }
      else {
        printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
               "\033[0m\033[32m <---\033[0m\015", 100.);
        cout << endl;
      }
    }
  }
}

namespace tas {

const int &candidate_id() { return cms3.candidate_id(); }
const float &weight() { return cms3.weight(); }
const float &dipho_sumpt() { return cms3.dipho_sumpt(); }
const float &dipho_cosphi() { return cms3.dipho_cosphi(); }
const float &mass() { return cms3.mass(); }
const float &leadPt() { return cms3.leadPt(); }
const float &leadEt() { return cms3.leadEt(); }
const float &leadEta() { return cms3.leadEta(); }
const float &leadPhi() { return cms3.leadPhi(); }
const float &lead_sieie() { return cms3.lead_sieie(); }
const float &lead_hoe() { return cms3.lead_hoe(); }
const float &lead_sigmaEoE() { return cms3.lead_sigmaEoE(); }
const float &lead_ptoM() { return cms3.lead_ptoM(); }
const float &leadR9() { return cms3.leadR9(); }
const float &subleadPt() { return cms3.subleadPt(); }
const float &subleadEt() { return cms3.subleadEt(); }
const float &subleadEta() { return cms3.subleadEta(); }
const float &subleadPhi() { return cms3.subleadPhi(); }
const float &sublead_sieie() { return cms3.sublead_sieie(); }
const float &sublead_hoe() { return cms3.sublead_hoe(); }
const float &sublead_sigmaEoE() { return cms3.sublead_sigmaEoE(); }
const float &sublead_ptoM() { return cms3.sublead_ptoM(); }
const float &subleadR9() { return cms3.subleadR9(); }
const float &leadIDMVA() { return cms3.leadIDMVA(); }
const float &subleadIDMVA() { return cms3.subleadIDMVA(); }
const float &dipho_rapidity() { return cms3.dipho_rapidity(); }
const float &n_ele() { return cms3.n_ele(); }
const float &ele1_pt() { return cms3.ele1_pt(); }
const float &ele2_pt() { return cms3.ele2_pt(); }
const float &n_muons() { return cms3.n_muons(); }
const float &muon1_pt() { return cms3.muon1_pt(); }
const float &muon2_pt() { return cms3.muon2_pt(); }
const float &n_bjets() { return cms3.n_bjets(); }
const float &n_jets() { return cms3.n_jets(); }
const float &bjet1_pt() { return cms3.bjet1_pt(); }
const float &bjet2_pt() { return cms3.bjet2_pt(); }
const float &bjet1_csv() { return cms3.bjet1_csv(); }
const float &bjet2_csv() { return cms3.bjet2_csv(); }
const float &Mjj() { return cms3.Mjj(); }
const float &jet_pt1() { return cms3.jet_pt1(); }
const float &jet_eta1() { return cms3.jet_eta1(); }
const float &jet_phi1() { return cms3.jet_phi1(); }
const float &jet_bdiscriminant1() { return cms3.jet_bdiscriminant1(); }
const float &jet_pt2() { return cms3.jet_pt2(); }
const float &jet_eta2() { return cms3.jet_eta2(); }
const float &jet_phi2() { return cms3.jet_phi2(); }
const float &jet_bdiscriminant2() { return cms3.jet_bdiscriminant2(); }
const float &jet_pt3() { return cms3.jet_pt3(); }
const float &jet_eta3() { return cms3.jet_eta3(); }
const float &jet_phi3() { return cms3.jet_phi3(); }
const float &jet_bdiscriminant3() { return cms3.jet_bdiscriminant3(); }
const float &jet_pt4() { return cms3.jet_pt4(); }
const float &jet_eta4() { return cms3.jet_eta4(); }
const float &jet_phi4() { return cms3.jet_phi4(); }
const float &jet_bdiscriminant4() { return cms3.jet_bdiscriminant4(); }
const float &jet_pt5() { return cms3.jet_pt5(); }
const float &jet_eta5() { return cms3.jet_eta5(); }
const float &jet_phi5() { return cms3.jet_phi5(); }
const float &jet_bdiscriminant5() { return cms3.jet_bdiscriminant5(); }
const float &jet_pt6() { return cms3.jet_pt6(); }
const float &jet_eta6() { return cms3.jet_eta6(); }
const float &jet_phi6() { return cms3.jet_phi6(); }
const float &jet_bdiscriminant6() { return cms3.jet_bdiscriminant6(); }
const float &jet_pt7() { return cms3.jet_pt7(); }
const float &jet_eta7() { return cms3.jet_eta7(); }
const float &jet_phi7() { return cms3.jet_phi7(); }
const float &jet_bdiscriminant7() { return cms3.jet_bdiscriminant7(); }
const float &jet_pt8() { return cms3.jet_pt8(); }
const float &jet_eta8() { return cms3.jet_eta8(); }
const float &jet_phi8() { return cms3.jet_phi8(); }
const float &jet_bdiscriminant8() { return cms3.jet_bdiscriminant8(); }
const float &jet_pt9() { return cms3.jet_pt9(); }
const float &jet_eta9() { return cms3.jet_eta9(); }
const float &jet_phi9() { return cms3.jet_phi9(); }
const float &jet_bdiscriminant9() { return cms3.jet_bdiscriminant9(); }
const float &jet_pt10() { return cms3.jet_pt10(); }
const float &jet_eta10() { return cms3.jet_eta10(); }
const float &jet_phi10() { return cms3.jet_phi10(); }
const float &jet_bdiscriminant10() { return cms3.jet_bdiscriminant10(); }
const float &jet_pt11() { return cms3.jet_pt11(); }
const float &jet_eta11() { return cms3.jet_eta11(); }
const float &jet_phi11() { return cms3.jet_phi11(); }
const float &jet_bdiscriminant11() { return cms3.jet_bdiscriminant11(); }
const float &jet_pt12() { return cms3.jet_pt12(); }
const float &jet_eta12() { return cms3.jet_eta12(); }
const float &jet_phi12() { return cms3.jet_phi12(); }
const float &jet_bdiscriminant12() { return cms3.jet_bdiscriminant12(); }
const float &jet_pt13() { return cms3.jet_pt13(); }
const float &jet_eta13() { return cms3.jet_eta13(); }
const float &jet_phi13() { return cms3.jet_phi13(); }
const float &jet_bdiscriminant13() { return cms3.jet_bdiscriminant13(); }
const float &jet_pt14() { return cms3.jet_pt14(); }
const float &jet_eta14() { return cms3.jet_eta14(); }
const float &jet_phi14() { return cms3.jet_phi14(); }
const float &jet_bdiscriminant14() { return cms3.jet_bdiscriminant14(); }
const float &jet_pt15() { return cms3.jet_pt15(); }
const float &jet_eta15() { return cms3.jet_eta15(); }
const float &jet_phi15() { return cms3.jet_phi15(); }
const float &jet_bdiscriminant15() { return cms3.jet_bdiscriminant15(); }
const float &rho() { return cms3.rho(); }
const int &nvtx() { return cms3.nvtx(); }
const unsigned long long &event() { return cms3.event(); }
const unsigned int &lumi() { return cms3.lumi(); }
const int &processIndex() { return cms3.processIndex(); }
const unsigned int &run() { return cms3.run(); }
const float &npu() { return cms3.npu(); }
const float &puweight() { return cms3.puweight(); }

}
