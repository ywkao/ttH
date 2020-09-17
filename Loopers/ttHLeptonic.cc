#include "ttHLeptonic.h"
ttHLeptonic cms3;

void ttHLeptonic::Init(TTree *tree) {

  tree->SetMakeClass(1);

  candidate_id_branch = tree->GetBranch("candidate_id");
  if (candidate_id_branch) candidate_id_branch->SetAddress(&candidate_id_);
  weight_branch = tree->GetBranch("weight");
  if (weight_branch) weight_branch->SetAddress(&weight_);
  jet_pt6_branch = tree->GetBranch("jet_pt6");
  if (jet_pt6_branch) jet_pt6_branch->SetAddress(&jet_pt6_);
  jet_phi12_branch = tree->GetBranch("jet_phi12");
  if (jet_phi12_branch) jet_phi12_branch->SetAddress(&jet_phi12_);
  lead_MomID_branch = tree->GetBranch("lead_MomID");
  if (lead_MomID_branch) lead_MomID_branch->SetAddress(&lead_MomID_);
  sublead_SimpleMomStatus_branch = tree->GetBranch("sublead_SimpleMomStatus");
  if (sublead_SimpleMomStatus_branch) sublead_SimpleMomStatus_branch->SetAddress(&sublead_SimpleMomStatus_);
  jet_bdiscriminant10_branch = tree->GetBranch("jet_bdiscriminant10");
  if (jet_bdiscriminant10_branch) jet_bdiscriminant10_branch->SetAddress(&jet_bdiscriminant10_);
  jet_pt1_branch = tree->GetBranch("jet_pt1");
  if (jet_pt1_branch) jet_pt1_branch->SetAddress(&jet_pt1_);
  nGoodEls_branch = tree->GetBranch("nGoodEls");
  if (nGoodEls_branch) nGoodEls_branch->SetAddress(&nGoodEls_);
  jet_pt3_branch = tree->GetBranch("jet_pt3");
  if (jet_pt3_branch) jet_pt3_branch->SetAddress(&jet_pt3_);
  lead_SmallestDr_branch = tree->GetBranch("lead_SmallestDr");
  if (lead_SmallestDr_branch) lead_SmallestDr_branch->SetAddress(&lead_SmallestDr_);
  jet_phi8_branch = tree->GetBranch("jet_phi8");
  if (jet_phi8_branch) jet_phi8_branch->SetAddress(&jet_phi8_);
  sublead_Mad_branch = tree->GetBranch("sublead_Mad");
  if (sublead_Mad_branch) sublead_Mad_branch->SetAddress(&sublead_Mad_);
  jet_bbdiscriminant8_branch = tree->GetBranch("jet_bbdiscriminant8");
  if (jet_bbdiscriminant8_branch) jet_bbdiscriminant8_branch->SetAddress(&jet_bbdiscriminant8_);
  jet_udsgdiscriminant8_branch = tree->GetBranch("jet_udsgdiscriminant8");
  if (jet_udsgdiscriminant8_branch) jet_udsgdiscriminant8_branch->SetAddress(&jet_udsgdiscriminant8_);
  jet_hadronFlavour8_branch = tree->GetBranch("jet_hadronFlavour8");
  if (jet_hadronFlavour8_branch) jet_hadronFlavour8_branch->SetAddress(&jet_hadronFlavour8_);
  ele2_eta_branch = tree->GetBranch("ele2_eta");
  if (ele2_eta_branch) ele2_eta_branch->SetAddress(&ele2_eta_);
  jet_pt15_branch = tree->GetBranch("jet_pt15");
  if (jet_pt15_branch) jet_pt15_branch->SetAddress(&jet_pt15_);
  topTag_topMass_branch = tree->GetBranch("topTag_topMass");
  if (topTag_topMass_branch) topTag_topMass_branch->SetAddress(&topTag_topMass_);
  dnn_score_ttgg_branch = tree->GetBranch("dnn_score_ttgg");
  if (dnn_score_ttgg_branch) dnn_score_ttgg_branch->SetAddress(&dnn_score_ttgg_);
  jet_bdiscriminant9_branch = tree->GetBranch("jet_bdiscriminant9");
  if (jet_bdiscriminant9_branch) jet_bdiscriminant9_branch->SetAddress(&jet_bdiscriminant9_);
  jet_cdiscriminant4_branch = tree->GetBranch("jet_cdiscriminant4");
  if (jet_cdiscriminant4_branch) jet_cdiscriminant4_branch->SetAddress(&jet_cdiscriminant4_);
  jet_phi6_branch = tree->GetBranch("jet_phi6");
  if (jet_phi6_branch) jet_phi6_branch->SetAddress(&jet_phi6_);
  diphoMVARes_branch = tree->GetBranch("diphoMVARes");
  if (diphoMVARes_branch) diphoMVARes_branch->SetAddress(&diphoMVARes_);
  jet_cdiscriminant5_branch = tree->GetBranch("jet_cdiscriminant5");
  if (jet_cdiscriminant5_branch) jet_cdiscriminant5_branch->SetAddress(&jet_cdiscriminant5_);
  sublead_sigmaEoE_branch = tree->GetBranch("sublead_sigmaEoE");
  if (sublead_sigmaEoE_branch) sublead_sigmaEoE_branch->SetAddress(&sublead_sigmaEoE_);
  jet_udsgdiscriminant13_branch = tree->GetBranch("jet_udsgdiscriminant13");
  if (jet_udsgdiscriminant13_branch) jet_udsgdiscriminant13_branch->SetAddress(&jet_udsgdiscriminant13_);
  jet_hadronFlavour13_branch = tree->GetBranch("jet_hadronFlavour13");
  if (jet_hadronFlavour13_branch) jet_hadronFlavour13_branch->SetAddress(&jet_hadronFlavour13_);
  sublead_photon_type_branch = tree->GetBranch("sublead_photon_type");
  if (sublead_photon_type_branch) sublead_photon_type_branch->SetAddress(&sublead_photon_type_);
  leadPtGen_branch = tree->GetBranch("leadPtGen");
  if (leadPtGen_branch) leadPtGen_branch->SetAddress(&leadPtGen_);
  sublead_Pythia_branch = tree->GetBranch("sublead_Pythia");
  if (sublead_Pythia_branch) sublead_Pythia_branch->SetAddress(&sublead_Pythia_);
  dZ_branch = tree->GetBranch("dZ");
  if (dZ_branch) dZ_branch->SetAddress(&dZ_);
  jet_energy8_branch = tree->GetBranch("jet_energy8");
  if (jet_energy8_branch) jet_energy8_branch->SetAddress(&jet_energy8_);
  sublead_closest_gen_Pt_branch = tree->GetBranch("sublead_closest_gen_Pt");
  if (sublead_closest_gen_Pt_branch) sublead_closest_gen_Pt_branch->SetAddress(&sublead_closest_gen_Pt_);
  jet_cdiscriminant12_branch = tree->GetBranch("jet_cdiscriminant12");
  if (jet_cdiscriminant12_branch) jet_cdiscriminant12_branch->SetAddress(&jet_cdiscriminant12_);
  jet_eta10_branch = tree->GetBranch("jet_eta10");
  if (jet_eta10_branch) jet_eta10_branch->SetAddress(&jet_eta10_);
  jet_udsgdiscriminant12_branch = tree->GetBranch("jet_udsgdiscriminant12");
  if (jet_udsgdiscriminant12_branch) jet_udsgdiscriminant12_branch->SetAddress(&jet_udsgdiscriminant12_);
  jet_hadronFlavour12_branch = tree->GetBranch("jet_hadronFlavour12");
  if (jet_hadronFlavour12_branch) jet_hadronFlavour12_branch->SetAddress(&jet_hadronFlavour12_);
  nGoodElsFromTau_branch = tree->GetBranch("nGoodElsFromTau");
  if (nGoodElsFromTau_branch) nGoodElsFromTau_branch->SetAddress(&nGoodElsFromTau_);
  muon2_eta_branch = tree->GetBranch("muon2_eta");
  if (muon2_eta_branch) muon2_eta_branch->SetAddress(&muon2_eta_);
  jet_bbdiscriminant4_branch = tree->GetBranch("jet_bbdiscriminant4");
  if (jet_bbdiscriminant4_branch) jet_bbdiscriminant4_branch->SetAddress(&jet_bbdiscriminant4_);
  LooseMvaSFDown01sigma_branch = tree->GetBranch("LooseMvaSFDown01sigma");
  if (LooseMvaSFDown01sigma_branch) LooseMvaSFDown01sigma_branch->SetAddress(&LooseMvaSFDown01sigma_);
  jet_cdiscriminant3_branch = tree->GetBranch("jet_cdiscriminant3");
  if (jet_cdiscriminant3_branch) jet_cdiscriminant3_branch->SetAddress(&jet_cdiscriminant3_);
  TriggerWeightDown01sigma_branch = tree->GetBranch("TriggerWeightDown01sigma");
  if (TriggerWeightDown01sigma_branch) TriggerWeightDown01sigma_branch->SetAddress(&TriggerWeightDown01sigma_);
  tthMVA_branch = tree->GetBranch("tthMVA");
  if (tthMVA_branch) tthMVA_branch->SetAddress(&tthMVA_);
  jet_bdiscriminant6_branch = tree->GetBranch("jet_bdiscriminant6");
  if (jet_bdiscriminant6_branch) jet_bdiscriminant6_branch->SetAddress(&jet_bdiscriminant6_);
  jet_pt8_branch = tree->GetBranch("jet_pt8");
  if (jet_pt8_branch) jet_pt8_branch->SetAddress(&jet_pt8_);
  MetPhi_branch = tree->GetBranch("MetPhi");
  if (MetPhi_branch) MetPhi_branch->SetAddress(&MetPhi_);
  muon1_eta_branch = tree->GetBranch("muon1_eta");
  if (muon1_eta_branch) muon1_eta_branch->SetAddress(&muon1_eta_);
  sublead_Prompt_branch = tree->GetBranch("sublead_Prompt");
  if (sublead_Prompt_branch) sublead_Prompt_branch->SetAddress(&sublead_Prompt_);
  leadIDMVA_branch = tree->GetBranch("leadIDMVA");
  if (leadIDMVA_branch) leadIDMVA_branch->SetAddress(&leadIDMVA_);
  jet_udsgdiscriminant7_branch = tree->GetBranch("jet_udsgdiscriminant7");
  if (jet_udsgdiscriminant7_branch) jet_udsgdiscriminant7_branch->SetAddress(&jet_udsgdiscriminant7_);
  jet_hadronFlavour7_branch = tree->GetBranch("jet_hadronFlavour7");
  if (jet_hadronFlavour7_branch) jet_hadronFlavour7_branch->SetAddress(&jet_hadronFlavour7_);
  jet_phi3_branch = tree->GetBranch("jet_phi3");
  if (jet_phi3_branch) jet_phi3_branch->SetAddress(&jet_phi3_);
  jet_cdiscriminant9_branch = tree->GetBranch("jet_cdiscriminant9");
  if (jet_cdiscriminant9_branch) jet_cdiscriminant9_branch->SetAddress(&jet_cdiscriminant9_);
  lead_PhoGenPt_branch = tree->GetBranch("lead_PhoGenPt");
  if (lead_PhoGenPt_branch) lead_PhoGenPt_branch->SetAddress(&lead_PhoGenPt_);
  jet_pt5_branch = tree->GetBranch("jet_pt5");
  if (jet_pt5_branch) jet_pt5_branch->SetAddress(&jet_pt5_);
  jet_phi13_branch = tree->GetBranch("jet_phi13");
  if (jet_phi13_branch) jet_phi13_branch->SetAddress(&jet_phi13_);
  sublead_closest_gen_dR_branch = tree->GetBranch("sublead_closest_gen_dR");
  if (sublead_closest_gen_dR_branch) sublead_closest_gen_dR_branch->SetAddress(&sublead_closest_gen_dR_);
  lead_Pythia_branch = tree->GetBranch("lead_Pythia");
  if (lead_Pythia_branch) lead_Pythia_branch->SetAddress(&lead_Pythia_);
  nGoodMusFromTau_branch = tree->GetBranch("nGoodMusFromTau");
  if (nGoodMusFromTau_branch) nGoodMusFromTau_branch->SetAddress(&nGoodMusFromTau_);
  ele1_pt_branch = tree->GetBranch("ele1_pt");
  if (ele1_pt_branch) ele1_pt_branch->SetAddress(&ele1_pt_);
  jet_phi14_branch = tree->GetBranch("jet_phi14");
  if (jet_phi14_branch) jet_phi14_branch->SetAddress(&jet_phi14_);
  ElectronWeightDown01sigma_branch = tree->GetBranch("ElectronWeightDown01sigma");
  if (ElectronWeightDown01sigma_branch) ElectronWeightDown01sigma_branch->SetAddress(&ElectronWeightDown01sigma_);
  jet_bdiscriminant14_branch = tree->GetBranch("jet_bdiscriminant14");
  if (jet_bdiscriminant14_branch) jet_bdiscriminant14_branch->SetAddress(&jet_bdiscriminant14_);
  jet_pt9_branch = tree->GetBranch("jet_pt9");
  if (jet_pt9_branch) jet_pt9_branch->SetAddress(&jet_pt9_);
  nGoodMus_branch = tree->GetBranch("nGoodMus");
  if (nGoodMus_branch) nGoodMus_branch->SetAddress(&nGoodMus_);
  LooseMvaSFUp01sigma_branch = tree->GetBranch("LooseMvaSFUp01sigma");
  if (LooseMvaSFUp01sigma_branch) LooseMvaSFUp01sigma_branch->SetAddress(&LooseMvaSFUp01sigma_);
  muon2_pt_branch = tree->GetBranch("muon2_pt");
  if (muon2_pt_branch) muon2_pt_branch->SetAddress(&muon2_pt_);
  jet_bdiscriminant5_branch = tree->GetBranch("jet_bdiscriminant5");
  if (jet_bdiscriminant5_branch) jet_bdiscriminant5_branch->SetAddress(&jet_bdiscriminant5_);
  jet_energy14_branch = tree->GetBranch("jet_energy14");
  if (jet_energy14_branch) jet_energy14_branch->SetAddress(&jet_energy14_);
  lead_MomMomID_branch = tree->GetBranch("lead_MomMomID");
  if (lead_MomMomID_branch) lead_MomMomID_branch->SetAddress(&lead_MomMomID_);
  jet_eta6_branch = tree->GetBranch("jet_eta6");
  if (jet_eta6_branch) jet_eta6_branch->SetAddress(&jet_eta6_);
  n_jets_branch = tree->GetBranch("n_jets");
  if (n_jets_branch) n_jets_branch->SetAddress(&n_jets_);
  UnmatchedPUWeightDown01sigma_branch = tree->GetBranch("UnmatchedPUWeightDown01sigma");
  if (UnmatchedPUWeightDown01sigma_branch) UnmatchedPUWeightDown01sigma_branch->SetAddress(&UnmatchedPUWeightDown01sigma_);
  jet_bbdiscriminant10_branch = tree->GetBranch("jet_bbdiscriminant10");
  if (jet_bbdiscriminant10_branch) jet_bbdiscriminant10_branch->SetAddress(&jet_bbdiscriminant10_);
  rand_branch = tree->GetBranch("rand");
  if (rand_branch) rand_branch->SetAddress(&rand_);
  jet_udsgdiscriminant4_branch = tree->GetBranch("jet_udsgdiscriminant4");
  if (jet_udsgdiscriminant4_branch) jet_udsgdiscriminant4_branch->SetAddress(&jet_udsgdiscriminant4_);
  jet_hadronFlavour4_branch = tree->GetBranch("jet_hadronFlavour4");
  if (jet_hadronFlavour4_branch) jet_hadronFlavour4_branch->SetAddress(&jet_hadronFlavour4_);
  subleadPt_branch = tree->GetBranch("subleadPt");
  if (subleadPt_branch) subleadPt_branch->SetAddress(&subleadPt_);
  mass_branch = tree->GetBranch("mass");
  if (mass_branch) mass_branch->SetAddress(&mass_);
  jet_energy10_branch = tree->GetBranch("jet_energy10");
  if (jet_energy10_branch) jet_energy10_branch->SetAddress(&jet_energy10_);
  jet_eta7_branch = tree->GetBranch("jet_eta7");
  if (jet_eta7_branch) jet_eta7_branch->SetAddress(&jet_eta7_);
  jet_phi11_branch = tree->GetBranch("jet_phi11");
  if (jet_phi11_branch) jet_phi11_branch->SetAddress(&jet_phi11_);
  sublead_MomMomID_branch = tree->GetBranch("sublead_MomMomID");
  if (sublead_MomMomID_branch) sublead_MomMomID_branch->SetAddress(&sublead_MomMomID_);
  jet_udsgdiscriminant10_branch = tree->GetBranch("jet_udsgdiscriminant10");
  if (jet_udsgdiscriminant10_branch) jet_udsgdiscriminant10_branch->SetAddress(&jet_udsgdiscriminant10_);
  jet_hadronFlavour10_branch = tree->GetBranch("jet_hadronFlavour10");
  if (jet_hadronFlavour10_branch) jet_hadronFlavour10_branch->SetAddress(&jet_hadronFlavour10_);
  MvaLinearSystDown01sigma_branch = tree->GetBranch("MvaLinearSystDown01sigma");
  if (MvaLinearSystDown01sigma_branch) MvaLinearSystDown01sigma_branch->SetAddress(&MvaLinearSystDown01sigma_);
  jet_phi1_branch = tree->GetBranch("jet_phi1");
  if (jet_phi1_branch) jet_phi1_branch->SetAddress(&jet_phi1_);
  electronVetoSFDown01sigma_branch = tree->GetBranch("electronVetoSFDown01sigma");
  if (electronVetoSFDown01sigma_branch) electronVetoSFDown01sigma_branch->SetAddress(&electronVetoSFDown01sigma_);
  jet_cdiscriminant8_branch = tree->GetBranch("jet_cdiscriminant8");
  if (jet_cdiscriminant8_branch) jet_cdiscriminant8_branch->SetAddress(&jet_cdiscriminant8_);
  jet_phi7_branch = tree->GetBranch("jet_phi7");
  if (jet_phi7_branch) jet_phi7_branch->SetAddress(&jet_phi7_);
  sublead_sieie_branch = tree->GetBranch("sublead_sieie");
  if (sublead_sieie_branch) sublead_sieie_branch->SetAddress(&sublead_sieie_);
  JetBTagReshapeWeightUp01sigma_branch = tree->GetBranch("JetBTagReshapeWeightUp01sigma");
  if (JetBTagReshapeWeightUp01sigma_branch) JetBTagReshapeWeightUp01sigma_branch->SetAddress(&JetBTagReshapeWeightUp01sigma_);
  jet_eta1_branch = tree->GetBranch("jet_eta1");
  if (jet_eta1_branch) jet_eta1_branch->SetAddress(&jet_eta1_);
  jet_energy3_branch = tree->GetBranch("jet_energy3");
  if (jet_energy3_branch) jet_energy3_branch->SetAddress(&jet_energy3_);
  lead_Mad_branch = tree->GetBranch("lead_Mad");
  if (lead_Mad_branch) lead_Mad_branch->SetAddress(&lead_Mad_);
  lead_PassFrix_branch = tree->GetBranch("lead_PassFrix");
  if (lead_PassFrix_branch) lead_PassFrix_branch->SetAddress(&lead_PassFrix_);
  jet_cdiscriminant6_branch = tree->GetBranch("jet_cdiscriminant6");
  if (jet_cdiscriminant6_branch) jet_cdiscriminant6_branch->SetAddress(&jet_cdiscriminant6_);
  ele1_charge_branch = tree->GetBranch("ele1_charge");
  if (ele1_charge_branch) ele1_charge_branch->SetAddress(&ele1_charge_);
  jet_bbdiscriminant11_branch = tree->GetBranch("jet_bbdiscriminant11");
  if (jet_bbdiscriminant11_branch) jet_bbdiscriminant11_branch->SetAddress(&jet_bbdiscriminant11_);
  PreselSFUp01sigma_branch = tree->GetBranch("PreselSFUp01sigma");
  if (PreselSFUp01sigma_branch) PreselSFUp01sigma_branch->SetAddress(&PreselSFUp01sigma_);
  jet_cdiscriminant2_branch = tree->GetBranch("jet_cdiscriminant2");
  if (jet_cdiscriminant2_branch) jet_cdiscriminant2_branch->SetAddress(&jet_cdiscriminant2_);
  weight_JetBTagWeight_branch = tree->GetBranch("weight_JetBTagWeight");
  if (weight_JetBTagWeight_branch) weight_JetBTagWeight_branch->SetAddress(&weight_JetBTagWeight_);
  jet_udsgdiscriminant6_branch = tree->GetBranch("jet_udsgdiscriminant6");
  if (jet_udsgdiscriminant6_branch) jet_udsgdiscriminant6_branch->SetAddress(&jet_udsgdiscriminant6_);
  jet_hadronFlavour6_branch = tree->GetBranch("jet_hadronFlavour6");
  if (jet_hadronFlavour6_branch) jet_hadronFlavour6_branch->SetAddress(&jet_hadronFlavour6_);
  sublead_hoe_branch = tree->GetBranch("sublead_hoe");
  if (sublead_hoe_branch) sublead_hoe_branch->SetAddress(&sublead_hoe_);
  muonSubleadIso_branch = tree->GetBranch("muonSubleadIso");
  if (muonSubleadIso_branch) muonSubleadIso_branch->SetAddress(&muonSubleadIso_);
  leadR9_branch = tree->GetBranch("leadR9");
  if (leadR9_branch) leadR9_branch->SetAddress(&leadR9_);
  dipho_cosphi_branch = tree->GetBranch("dipho_cosphi");
  if (dipho_cosphi_branch) dipho_cosphi_branch->SetAddress(&dipho_cosphi_);
  sublead_MomID_branch = tree->GetBranch("sublead_MomID");
  if (sublead_MomID_branch) sublead_MomID_branch->SetAddress(&sublead_MomID_);
  jet_eta13_branch = tree->GetBranch("jet_eta13");
  if (jet_eta13_branch) jet_eta13_branch->SetAddress(&jet_eta13_);
  FracRVWeightUp01sigma_branch = tree->GetBranch("FracRVWeightUp01sigma");
  if (FracRVWeightUp01sigma_branch) FracRVWeightUp01sigma_branch->SetAddress(&FracRVWeightUp01sigma_);
  subleadGenMatch_branch = tree->GetBranch("subleadGenMatch");
  if (subleadGenMatch_branch) subleadGenMatch_branch->SetAddress(&subleadGenMatch_);
  lead_PhoGenPhi_branch = tree->GetBranch("lead_PhoGenPhi");
  if (lead_PhoGenPhi_branch) lead_PhoGenPhi_branch->SetAddress(&lead_PhoGenPhi_);
  nElecLoose_branch = tree->GetBranch("nElecLoose");
  if (nElecLoose_branch) nElecLoose_branch->SetAddress(&nElecLoose_);
  jet_eta12_branch = tree->GetBranch("jet_eta12");
  if (jet_eta12_branch) jet_eta12_branch->SetAddress(&jet_eta12_);
  jet_udsgdiscriminant9_branch = tree->GetBranch("jet_udsgdiscriminant9");
  if (jet_udsgdiscriminant9_branch) jet_udsgdiscriminant9_branch->SetAddress(&jet_udsgdiscriminant9_);
  jet_hadronFlavour9_branch = tree->GetBranch("jet_hadronFlavour9");
  if (jet_hadronFlavour9_branch) jet_hadronFlavour9_branch->SetAddress(&jet_hadronFlavour9_);
  electronVetoSFUp01sigma_branch = tree->GetBranch("electronVetoSFUp01sigma");
  if (electronVetoSFUp01sigma_branch) electronVetoSFUp01sigma_branch->SetAddress(&electronVetoSFUp01sigma_);
  subleadGendeltaR_branch = tree->GetBranch("subleadGendeltaR");
  if (subleadGendeltaR_branch) subleadGendeltaR_branch->SetAddress(&subleadGendeltaR_);
  muon2_energy_branch = tree->GetBranch("muon2_energy");
  if (muon2_energy_branch) muon2_energy_branch->SetAddress(&muon2_energy_);
  sublead_PhoGenEta_branch = tree->GetBranch("sublead_PhoGenEta");
  if (sublead_PhoGenEta_branch) sublead_PhoGenEta_branch->SetAddress(&sublead_PhoGenEta_);
  jet_udsgdiscriminant15_branch = tree->GetBranch("jet_udsgdiscriminant15");
  if (jet_udsgdiscriminant15_branch) jet_udsgdiscriminant15_branch->SetAddress(&jet_udsgdiscriminant15_);
  jet_hadronFlavour15_branch = tree->GetBranch("jet_hadronFlavour15");
  if (jet_hadronFlavour15_branch) jet_hadronFlavour15_branch->SetAddress(&jet_hadronFlavour15_);
  jet_phi5_branch = tree->GetBranch("jet_phi5");
  if (jet_phi5_branch) jet_phi5_branch->SetAddress(&jet_phi5_);
  jet_eta2_branch = tree->GetBranch("jet_eta2");
  if (jet_eta2_branch) jet_eta2_branch->SetAddress(&jet_eta2_);
  subleadEta_branch = tree->GetBranch("subleadEta");
  if (subleadEta_branch) subleadEta_branch->SetAddress(&subleadEta_);
  jet_pt13_branch = tree->GetBranch("jet_pt13");
  if (jet_pt13_branch) jet_pt13_branch->SetAddress(&jet_pt13_);
  leadEt_branch = tree->GetBranch("leadEt");
  if (leadEt_branch) leadEt_branch->SetAddress(&leadEt_);
  jet_pt4_branch = tree->GetBranch("jet_pt4");
  if (jet_pt4_branch) jet_pt4_branch->SetAddress(&jet_pt4_);
  ElectronWeightUp01sigma_branch = tree->GetBranch("ElectronWeightUp01sigma");
  if (ElectronWeightUp01sigma_branch) ElectronWeightUp01sigma_branch->SetAddress(&ElectronWeightUp01sigma_);
  jet_udsgdiscriminant3_branch = tree->GetBranch("jet_udsgdiscriminant3");
  if (jet_udsgdiscriminant3_branch) jet_udsgdiscriminant3_branch->SetAddress(&jet_udsgdiscriminant3_);
  jet_hadronFlavour3_branch = tree->GetBranch("jet_hadronFlavour3");
  if (jet_hadronFlavour3_branch) jet_hadronFlavour3_branch->SetAddress(&jet_hadronFlavour3_);
  lead_sieie_branch = tree->GetBranch("lead_sieie");
  if (lead_sieie_branch) lead_sieie_branch->SetAddress(&lead_sieie_);
  MetPt_branch = tree->GetBranch("MetPt");
  if (MetPt_branch) MetPt_branch->SetAddress(&MetPt_);
  jet_cdiscriminant7_branch = tree->GetBranch("jet_cdiscriminant7");
  if (jet_cdiscriminant7_branch) jet_cdiscriminant7_branch->SetAddress(&jet_cdiscriminant7_);
  jet_udsgdiscriminant14_branch = tree->GetBranch("jet_udsgdiscriminant14");
  if (jet_udsgdiscriminant14_branch) jet_udsgdiscriminant14_branch->SetAddress(&jet_udsgdiscriminant14_);
  jet_hadronFlavour14_branch = tree->GetBranch("jet_hadronFlavour14");
  if (jet_hadronFlavour14_branch) jet_hadronFlavour14_branch->SetAddress(&jet_hadronFlavour14_);
  jet_bbdiscriminant14_branch = tree->GetBranch("jet_bbdiscriminant14");
  if (jet_bbdiscriminant14_branch) jet_bbdiscriminant14_branch->SetAddress(&jet_bbdiscriminant14_);
  muon1_energy_branch = tree->GetBranch("muon1_energy");
  if (muon1_energy_branch) muon1_energy_branch->SetAddress(&muon1_energy_);
  jet_eta9_branch = tree->GetBranch("jet_eta9");
  if (jet_eta9_branch) jet_eta9_branch->SetAddress(&jet_eta9_);
  jet_bbdiscriminant7_branch = tree->GetBranch("jet_bbdiscriminant7");
  if (jet_bbdiscriminant7_branch) jet_bbdiscriminant7_branch->SetAddress(&jet_bbdiscriminant7_);
  jet_bbdiscriminant9_branch = tree->GetBranch("jet_bbdiscriminant9");
  if (jet_bbdiscriminant9_branch) jet_bbdiscriminant9_branch->SetAddress(&jet_bbdiscriminant9_);
  muon2_charge_branch = tree->GetBranch("muon2_charge");
  if (muon2_charge_branch) muon2_charge_branch->SetAddress(&muon2_charge_);
  FracRVNvtxWeightUp01sigma_branch = tree->GetBranch("FracRVNvtxWeightUp01sigma");
  if (FracRVNvtxWeightUp01sigma_branch) FracRVNvtxWeightUp01sigma_branch->SetAddress(&FracRVNvtxWeightUp01sigma_);
  ele1_energy_branch = tree->GetBranch("ele1_energy");
  if (ele1_energy_branch) ele1_energy_branch->SetAddress(&ele1_energy_);
  nMuonMedium_branch = tree->GetBranch("nMuonMedium");
  if (nMuonMedium_branch) nMuonMedium_branch->SetAddress(&nMuonMedium_);
  jet_cdiscriminant14_branch = tree->GetBranch("jet_cdiscriminant14");
  if (jet_cdiscriminant14_branch) jet_cdiscriminant14_branch->SetAddress(&jet_cdiscriminant14_);
  nMuonTight_branch = tree->GetBranch("nMuonTight");
  if (nMuonTight_branch) nMuonTight_branch->SetAddress(&nMuonTight_);
  jet_energy12_branch = tree->GetBranch("jet_energy12");
  if (jet_energy12_branch) jet_energy12_branch->SetAddress(&jet_energy12_);
  nGoodTaus_branch = tree->GetBranch("nGoodTaus");
  if (nGoodTaus_branch) nGoodTaus_branch->SetAddress(&nGoodTaus_);
  jet_bbdiscriminant12_branch = tree->GetBranch("jet_bbdiscriminant12");
  if (jet_bbdiscriminant12_branch) jet_bbdiscriminant12_branch->SetAddress(&jet_bbdiscriminant12_);
  n_ele_branch = tree->GetBranch("n_ele");
  if (n_ele_branch) n_ele_branch->SetAddress(&n_ele_);
  muon1_charge_branch = tree->GetBranch("muon1_charge");
  if (muon1_charge_branch) muon1_charge_branch->SetAddress(&muon1_charge_);
  jet_phi15_branch = tree->GetBranch("jet_phi15");
  if (jet_phi15_branch) jet_phi15_branch->SetAddress(&jet_phi15_);
  jet_eta5_branch = tree->GetBranch("jet_eta5");
  if (jet_eta5_branch) jet_eta5_branch->SetAddress(&jet_eta5_);
  subleadEt_branch = tree->GetBranch("subleadEt");
  if (subleadEt_branch) subleadEt_branch->SetAddress(&subleadEt_);
  lead_closest_gen_dR_branch = tree->GetBranch("lead_closest_gen_dR");
  if (lead_closest_gen_dR_branch) lead_closest_gen_dR_branch->SetAddress(&lead_closest_gen_dR_);
  Mjj_branch = tree->GetBranch("Mjj");
  if (Mjj_branch) Mjj_branch->SetAddress(&Mjj_);
  jet_udsgdiscriminant5_branch = tree->GetBranch("jet_udsgdiscriminant5");
  if (jet_udsgdiscriminant5_branch) jet_udsgdiscriminant5_branch->SetAddress(&jet_udsgdiscriminant5_);
  jet_hadronFlavour5_branch = tree->GetBranch("jet_hadronFlavour5");
  if (jet_hadronFlavour5_branch) jet_hadronFlavour5_branch->SetAddress(&jet_hadronFlavour5_);
  subleadIDMVA_branch = tree->GetBranch("subleadIDMVA");
  if (subleadIDMVA_branch) subleadIDMVA_branch->SetAddress(&subleadIDMVA_);
  jet_cdiscriminant11_branch = tree->GetBranch("jet_cdiscriminant11");
  if (jet_cdiscriminant11_branch) jet_cdiscriminant11_branch->SetAddress(&jet_cdiscriminant11_);
  jet_pt14_branch = tree->GetBranch("jet_pt14");
  if (jet_pt14_branch) jet_pt14_branch->SetAddress(&jet_pt14_);
  lead_closest_gen_Pt_branch = tree->GetBranch("lead_closest_gen_Pt");
  if (lead_closest_gen_Pt_branch) lead_closest_gen_Pt_branch->SetAddress(&lead_closest_gen_Pt_);
  UnmatchedPUWeightUp01sigma_branch = tree->GetBranch("UnmatchedPUWeightUp01sigma");
  if (UnmatchedPUWeightUp01sigma_branch) UnmatchedPUWeightUp01sigma_branch->SetAddress(&UnmatchedPUWeightUp01sigma_);
  lead_SimpleMomID_branch = tree->GetBranch("lead_SimpleMomID");
  if (lead_SimpleMomID_branch) lead_SimpleMomID_branch->SetAddress(&lead_SimpleMomID_);
  nElecTight_branch = tree->GetBranch("nElecTight");
  if (nElecTight_branch) nElecTight_branch->SetAddress(&nElecTight_);
  ele1_eta_branch = tree->GetBranch("ele1_eta");
  if (ele1_eta_branch) ele1_eta_branch->SetAddress(&ele1_eta_);
  jet_bdiscriminant8_branch = tree->GetBranch("jet_bdiscriminant8");
  if (jet_bdiscriminant8_branch) jet_bdiscriminant8_branch->SetAddress(&jet_bdiscriminant8_);
  jet_bbdiscriminant13_branch = tree->GetBranch("jet_bbdiscriminant13");
  if (jet_bbdiscriminant13_branch) jet_bbdiscriminant13_branch->SetAddress(&jet_bbdiscriminant13_);
  leadEta_branch = tree->GetBranch("leadEta");
  if (leadEta_branch) leadEta_branch->SetAddress(&leadEta_);
  ele2_energy_branch = tree->GetBranch("ele2_energy");
  if (ele2_energy_branch) ele2_energy_branch->SetAddress(&ele2_energy_);
  nb_medium_branch = tree->GetBranch("nb_medium");
  if (nb_medium_branch) nb_medium_branch->SetAddress(&nb_medium_);
  JetBTagCutWeightDown01sigma_branch = tree->GetBranch("JetBTagCutWeightDown01sigma");
  if (JetBTagCutWeightDown01sigma_branch) JetBTagCutWeightDown01sigma_branch->SetAddress(&JetBTagCutWeightDown01sigma_);
  jet_pt11_branch = tree->GetBranch("jet_pt11");
  if (jet_pt11_branch) jet_pt11_branch->SetAddress(&jet_pt11_);
  jet_pt10_branch = tree->GetBranch("jet_pt10");
  if (jet_pt10_branch) jet_pt10_branch->SetAddress(&jet_pt10_);
  jet_udsgdiscriminant1_branch = tree->GetBranch("jet_udsgdiscriminant1");
  if (jet_udsgdiscriminant1_branch) jet_udsgdiscriminant1_branch->SetAddress(&jet_udsgdiscriminant1_);
  jet_hadronFlavour1_branch = tree->GetBranch("jet_hadronFlavour1");
  if (jet_hadronFlavour1_branch) jet_hadronFlavour1_branch->SetAddress(&jet_hadronFlavour1_);
  muonLeadIso_branch = tree->GetBranch("muonLeadIso");
  if (muonLeadIso_branch) muonLeadIso_branch->SetAddress(&muonLeadIso_);
  subleadPhi_branch = tree->GetBranch("subleadPhi");
  if (subleadPhi_branch) subleadPhi_branch->SetAddress(&subleadPhi_);
  jet_cdiscriminant13_branch = tree->GetBranch("jet_cdiscriminant13");
  if (jet_cdiscriminant13_branch) jet_cdiscriminant13_branch->SetAddress(&jet_cdiscriminant13_);
  subleadPixelSeed_branch = tree->GetBranch("subleadPixelSeed");
  if (subleadPixelSeed_branch) subleadPixelSeed_branch->SetAddress(&subleadPixelSeed_);
  n_muons_branch = tree->GetBranch("n_muons");
  if (n_muons_branch) n_muons_branch->SetAddress(&n_muons_);
  lead_SimpleMomStatus_branch = tree->GetBranch("lead_SimpleMomStatus");
  if (lead_SimpleMomStatus_branch) lead_SimpleMomStatus_branch->SetAddress(&lead_SimpleMomStatus_);
  jet_energy6_branch = tree->GetBranch("jet_energy6");
  if (jet_energy6_branch) jet_energy6_branch->SetAddress(&jet_energy6_);
  sublead_SmallestDr_branch = tree->GetBranch("sublead_SmallestDr");
  if (sublead_SmallestDr_branch) sublead_SmallestDr_branch->SetAddress(&sublead_SmallestDr_);
  dipho_rapidity_branch = tree->GetBranch("dipho_rapidity");
  if (dipho_rapidity_branch) dipho_rapidity_branch->SetAddress(&dipho_rapidity_);
  jet_bdiscriminant12_branch = tree->GetBranch("jet_bdiscriminant12");
  if (jet_bdiscriminant12_branch) jet_bdiscriminant12_branch->SetAddress(&jet_bdiscriminant12_);
  subleadEnergy_branch = tree->GetBranch("subleadEnergy");
  if (subleadEnergy_branch) subleadEnergy_branch->SetAddress(&subleadEnergy_);
  jet_phi2_branch = tree->GetBranch("jet_phi2");
  if (jet_phi2_branch) jet_phi2_branch->SetAddress(&jet_phi2_);
  jet_cdiscriminant10_branch = tree->GetBranch("jet_cdiscriminant10");
  if (jet_cdiscriminant10_branch) jet_cdiscriminant10_branch->SetAddress(&jet_cdiscriminant10_);
  lead_hoe_branch = tree->GetBranch("lead_hoe");
  if (lead_hoe_branch) lead_hoe_branch->SetAddress(&lead_hoe_);
  sublead_ptoM_branch = tree->GetBranch("sublead_ptoM");
  if (sublead_ptoM_branch) sublead_ptoM_branch->SetAddress(&sublead_ptoM_);
  jet_bbdiscriminant6_branch = tree->GetBranch("jet_bbdiscriminant6");
  if (jet_bbdiscriminant6_branch) jet_bbdiscriminant6_branch->SetAddress(&jet_bbdiscriminant6_);
  jet_energy15_branch = tree->GetBranch("jet_energy15");
  if (jet_energy15_branch) jet_energy15_branch->SetAddress(&jet_energy15_);
  leadPixelSeed_branch = tree->GetBranch("leadPixelSeed");
  if (leadPixelSeed_branch) leadPixelSeed_branch->SetAddress(&leadPixelSeed_);
  vertex_idx_branch = tree->GetBranch("vertex_idx");
  if (vertex_idx_branch) vertex_idx_branch->SetAddress(&vertex_idx_);
  jet_bbdiscriminant1_branch = tree->GetBranch("jet_bbdiscriminant1");
  if (jet_bbdiscriminant1_branch) jet_bbdiscriminant1_branch->SetAddress(&jet_bbdiscriminant1_);
  sublead_PhoGenPt_branch = tree->GetBranch("sublead_PhoGenPt");
  if (sublead_PhoGenPt_branch) sublead_PhoGenPt_branch->SetAddress(&sublead_PhoGenPt_);
  jet_energy4_branch = tree->GetBranch("jet_energy4");
  if (jet_energy4_branch) jet_energy4_branch->SetAddress(&jet_energy4_);
  jet_energy7_branch = tree->GetBranch("jet_energy7");
  if (jet_energy7_branch) jet_energy7_branch->SetAddress(&jet_energy7_);
  JetBTagCutWeightUp01sigma_branch = tree->GetBranch("JetBTagCutWeightUp01sigma");
  if (JetBTagCutWeightUp01sigma_branch) JetBTagCutWeightUp01sigma_branch->SetAddress(&JetBTagCutWeightUp01sigma_);
  MvaLinearSystUp01sigma_branch = tree->GetBranch("MvaLinearSystUp01sigma");
  if (MvaLinearSystUp01sigma_branch) MvaLinearSystUp01sigma_branch->SetAddress(&MvaLinearSystUp01sigma_);
  jet_eta3_branch = tree->GetBranch("jet_eta3");
  if (jet_eta3_branch) jet_eta3_branch->SetAddress(&jet_eta3_);
  jet_energy2_branch = tree->GetBranch("jet_energy2");
  if (jet_energy2_branch) jet_energy2_branch->SetAddress(&jet_energy2_);
  nb_tight_branch = tree->GetBranch("nb_tight");
  if (nb_tight_branch) nb_tight_branch->SetAddress(&nb_tight_);
  jet_bbdiscriminant5_branch = tree->GetBranch("jet_bbdiscriminant5");
  if (jet_bbdiscriminant5_branch) jet_bbdiscriminant5_branch->SetAddress(&jet_bbdiscriminant5_);
  jet_bdiscriminant1_branch = tree->GetBranch("jet_bdiscriminant1");
  if (jet_bdiscriminant1_branch) jet_bdiscriminant1_branch->SetAddress(&jet_bdiscriminant1_);
  n_bjets_branch = tree->GetBranch("n_bjets");
  if (n_bjets_branch) n_bjets_branch->SetAddress(&n_bjets_);
  jet_bbdiscriminant15_branch = tree->GetBranch("jet_bbdiscriminant15");
  if (jet_bbdiscriminant15_branch) jet_bbdiscriminant15_branch->SetAddress(&jet_bbdiscriminant15_);
  subleadPtGen_branch = tree->GetBranch("subleadPtGen");
  if (subleadPtGen_branch) subleadPtGen_branch->SetAddress(&subleadPtGen_);
  jet_pt2_branch = tree->GetBranch("jet_pt2");
  if (jet_pt2_branch) jet_pt2_branch->SetAddress(&jet_pt2_);
  lead_photon_type_branch = tree->GetBranch("lead_photon_type");
  if (lead_photon_type_branch) lead_photon_type_branch->SetAddress(&lead_photon_type_);
  FracRVNvtxWeightDown01sigma_branch = tree->GetBranch("FracRVNvtxWeightDown01sigma");
  if (FracRVNvtxWeightDown01sigma_branch) FracRVNvtxWeightDown01sigma_branch->SetAddress(&FracRVNvtxWeightDown01sigma_);
  PreselSFDown01sigma_branch = tree->GetBranch("PreselSFDown01sigma");
  if (PreselSFDown01sigma_branch) PreselSFDown01sigma_branch->SetAddress(&PreselSFDown01sigma_);
  lead_sigmaEoE_branch = tree->GetBranch("lead_sigmaEoE");
  if (lead_sigmaEoE_branch) lead_sigmaEoE_branch->SetAddress(&lead_sigmaEoE_);
  muon1_phi_branch = tree->GetBranch("muon1_phi");
  if (muon1_phi_branch) muon1_phi_branch->SetAddress(&muon1_phi_);
  dipho_sumpt_branch = tree->GetBranch("dipho_sumpt");
  if (dipho_sumpt_branch) dipho_sumpt_branch->SetAddress(&dipho_sumpt_);
  jet_bdiscriminant3_branch = tree->GetBranch("jet_bdiscriminant3");
  if (jet_bdiscriminant3_branch) jet_bdiscriminant3_branch->SetAddress(&jet_bdiscriminant3_);
  subleadPassEVeto_branch = tree->GetBranch("subleadPassEVeto");
  if (subleadPassEVeto_branch) subleadPassEVeto_branch->SetAddress(&subleadPassEVeto_);
  jet_energy1_branch = tree->GetBranch("jet_energy1");
  if (jet_energy1_branch) jet_energy1_branch->SetAddress(&jet_energy1_);
  jet_eta15_branch = tree->GetBranch("jet_eta15");
  if (jet_eta15_branch) jet_eta15_branch->SetAddress(&jet_eta15_);
  topTag_score_branch = tree->GetBranch("topTag_score");
  if (topTag_score_branch) topTag_score_branch->SetAddress(&topTag_score_);
  jet_phi10_branch = tree->GetBranch("jet_phi10");
  if (jet_phi10_branch) jet_phi10_branch->SetAddress(&jet_phi10_);
  jet_eta8_branch = tree->GetBranch("jet_eta8");
  if (jet_eta8_branch) jet_eta8_branch->SetAddress(&jet_eta8_);
  CMS_hgg_mass_branch = tree->GetBranch("CMS_hgg_mass");
  if (CMS_hgg_mass_branch) CMS_hgg_mass_branch->SetAddress(&CMS_hgg_mass_);
  centralObjectWeight_branch = tree->GetBranch("centralObjectWeight");
  if (centralObjectWeight_branch) centralObjectWeight_branch->SetAddress(&centralObjectWeight_);
  jet_bbdiscriminant2_branch = tree->GetBranch("jet_bbdiscriminant2");
  if (jet_bbdiscriminant2_branch) jet_bbdiscriminant2_branch->SetAddress(&jet_bbdiscriminant2_);
  jet_eta11_branch = tree->GetBranch("jet_eta11");
  if (jet_eta11_branch) jet_eta11_branch->SetAddress(&jet_eta11_);
  nMuonLoose_branch = tree->GetBranch("nMuonLoose");
  if (nMuonLoose_branch) nMuonLoose_branch->SetAddress(&nMuonLoose_);
  jet_bbdiscriminant3_branch = tree->GetBranch("jet_bbdiscriminant3");
  if (jet_bbdiscriminant3_branch) jet_bbdiscriminant3_branch->SetAddress(&jet_bbdiscriminant3_);
  FracRVWeightDown01sigma_branch = tree->GetBranch("FracRVWeightDown01sigma");
  if (FracRVWeightDown01sigma_branch) FracRVWeightDown01sigma_branch->SetAddress(&FracRVWeightDown01sigma_);
  jet_energy9_branch = tree->GetBranch("jet_energy9");
  if (jet_energy9_branch) jet_energy9_branch->SetAddress(&jet_energy9_);
  sublead_PassFrix_branch = tree->GetBranch("sublead_PassFrix");
  if (sublead_PassFrix_branch) sublead_PassFrix_branch->SetAddress(&sublead_PassFrix_);
  ele1_phi_branch = tree->GetBranch("ele1_phi");
  if (ele1_phi_branch) ele1_phi_branch->SetAddress(&ele1_phi_);
  ele2_charge_branch = tree->GetBranch("ele2_charge");
  if (ele2_charge_branch) ele2_charge_branch->SetAddress(&ele2_charge_);
  jet_bdiscriminant15_branch = tree->GetBranch("jet_bdiscriminant15");
  if (jet_bdiscriminant15_branch) jet_bdiscriminant15_branch->SetAddress(&jet_bdiscriminant15_);
  jet_phi9_branch = tree->GetBranch("jet_phi9");
  if (jet_phi9_branch) jet_phi9_branch->SetAddress(&jet_phi9_);
  jet_energy13_branch = tree->GetBranch("jet_energy13");
  if (jet_energy13_branch) jet_energy13_branch->SetAddress(&jet_energy13_);
  muon2_phi_branch = tree->GetBranch("muon2_phi");
  if (muon2_phi_branch) muon2_phi_branch->SetAddress(&muon2_phi_);
  jet_pt7_branch = tree->GetBranch("jet_pt7");
  if (jet_pt7_branch) jet_pt7_branch->SetAddress(&jet_pt7_);
  topTag_WMass_branch = tree->GetBranch("topTag_WMass");
  if (topTag_WMass_branch) topTag_WMass_branch->SetAddress(&topTag_WMass_);
  jet_cdiscriminant15_branch = tree->GetBranch("jet_cdiscriminant15");
  if (jet_cdiscriminant15_branch) jet_cdiscriminant15_branch->SetAddress(&jet_cdiscriminant15_);
  jet_bdiscriminant7_branch = tree->GetBranch("jet_bdiscriminant7");
  if (jet_bdiscriminant7_branch) jet_bdiscriminant7_branch->SetAddress(&jet_bdiscriminant7_);
  lead_PhoGenEta_branch = tree->GetBranch("lead_PhoGenEta");
  if (lead_PhoGenEta_branch) lead_PhoGenEta_branch->SetAddress(&lead_PhoGenEta_);
  jet_bdiscriminant2_branch = tree->GetBranch("jet_bdiscriminant2");
  if (jet_bdiscriminant2_branch) jet_bdiscriminant2_branch->SetAddress(&jet_bdiscriminant2_);
  leadEnergy_branch = tree->GetBranch("leadEnergy");
  if (leadEnergy_branch) leadEnergy_branch->SetAddress(&leadEnergy_);
  jet_cdiscriminant1_branch = tree->GetBranch("jet_cdiscriminant1");
  if (jet_cdiscriminant1_branch) jet_cdiscriminant1_branch->SetAddress(&jet_cdiscriminant1_);
  leadPt_branch = tree->GetBranch("leadPt");
  if (leadPt_branch) leadPt_branch->SetAddress(&leadPt_);
  leadPassEVeto_branch = tree->GetBranch("leadPassEVeto");
  if (leadPassEVeto_branch) leadPassEVeto_branch->SetAddress(&leadPassEVeto_);
  lead_Prompt_branch = tree->GetBranch("lead_Prompt");
  if (lead_Prompt_branch) lead_Prompt_branch->SetAddress(&lead_Prompt_);
  mT_branch = tree->GetBranch("mT");
  if (mT_branch) mT_branch->SetAddress(&mT_);
  jet_bdiscriminant13_branch = tree->GetBranch("jet_bdiscriminant13");
  if (jet_bdiscriminant13_branch) jet_bdiscriminant13_branch->SetAddress(&jet_bdiscriminant13_);
  jet_phi4_branch = tree->GetBranch("jet_phi4");
  if (jet_phi4_branch) jet_phi4_branch->SetAddress(&jet_phi4_);
  jet_pt12_branch = tree->GetBranch("jet_pt12");
  if (jet_pt12_branch) jet_pt12_branch->SetAddress(&jet_pt12_);
  leadGendeltaR_branch = tree->GetBranch("leadGendeltaR");
  if (leadGendeltaR_branch) leadGendeltaR_branch->SetAddress(&leadGendeltaR_);
  jet_energy5_branch = tree->GetBranch("jet_energy5");
  if (jet_energy5_branch) jet_energy5_branch->SetAddress(&jet_energy5_);
  nElecMedium_branch = tree->GetBranch("nElecMedium");
  if (nElecMedium_branch) nElecMedium_branch->SetAddress(&nElecMedium_);
  tthMVA_RunII_branch = tree->GetBranch("tthMVA_RunII");
  if (tthMVA_RunII_branch) tthMVA_RunII_branch->SetAddress(&tthMVA_RunII_);
  prefireProbability_branch = tree->GetBranch("prefireProbability");
  if (prefireProbability_branch) prefireProbability_branch->SetAddress(&prefireProbability_);
  leadPhi_branch = tree->GetBranch("leadPhi");
  if (leadPhi_branch) leadPhi_branch->SetAddress(&leadPhi_);
  subleadR9_branch = tree->GetBranch("subleadR9");
  if (subleadR9_branch) subleadR9_branch->SetAddress(&subleadR9_);
  jet_udsgdiscriminant11_branch = tree->GetBranch("jet_udsgdiscriminant11");
  if (jet_udsgdiscriminant11_branch) jet_udsgdiscriminant11_branch->SetAddress(&jet_udsgdiscriminant11_);
  jet_hadronFlavour11_branch = tree->GetBranch("jet_hadronFlavour11");
  if (jet_hadronFlavour11_branch) jet_hadronFlavour11_branch->SetAddress(&jet_hadronFlavour11_);
  jet_eta14_branch = tree->GetBranch("jet_eta14");
  if (jet_eta14_branch) jet_eta14_branch->SetAddress(&jet_eta14_);
  leadGenMatch_branch = tree->GetBranch("leadGenMatch");
  if (leadGenMatch_branch) leadGenMatch_branch->SetAddress(&leadGenMatch_);
  sublead_SimpleMomID_branch = tree->GetBranch("sublead_SimpleMomID");
  if (sublead_SimpleMomID_branch) sublead_SimpleMomID_branch->SetAddress(&sublead_SimpleMomID_);
  ele2_phi_branch = tree->GetBranch("ele2_phi");
  if (ele2_phi_branch) ele2_phi_branch->SetAddress(&ele2_phi_);
  jet_udsgdiscriminant2_branch = tree->GetBranch("jet_udsgdiscriminant2");
  if (jet_udsgdiscriminant2_branch) jet_udsgdiscriminant2_branch->SetAddress(&jet_udsgdiscriminant2_);
  jet_hadronFlavour2_branch = tree->GetBranch("jet_hadronFlavour2");
  if (jet_hadronFlavour2_branch) jet_hadronFlavour2_branch->SetAddress(&jet_hadronFlavour2_);
  sublead_PhoGenPhi_branch = tree->GetBranch("sublead_PhoGenPhi");
  if (sublead_PhoGenPhi_branch) sublead_PhoGenPhi_branch->SetAddress(&sublead_PhoGenPhi_);
  jet_eta4_branch = tree->GetBranch("jet_eta4");
  if (jet_eta4_branch) jet_eta4_branch->SetAddress(&jet_eta4_);
  nb_loose_branch = tree->GetBranch("nb_loose");
  if (nb_loose_branch) nb_loose_branch->SetAddress(&nb_loose_);
  JetBTagReshapeWeightDown01sigma_branch = tree->GetBranch("JetBTagReshapeWeightDown01sigma");
  if (JetBTagReshapeWeightDown01sigma_branch) JetBTagReshapeWeightDown01sigma_branch->SetAddress(&JetBTagReshapeWeightDown01sigma_);
  lead_ptoM_branch = tree->GetBranch("lead_ptoM");
  if (lead_ptoM_branch) lead_ptoM_branch->SetAddress(&lead_ptoM_);
  ele2_pt_branch = tree->GetBranch("ele2_pt");
  if (ele2_pt_branch) ele2_pt_branch->SetAddress(&ele2_pt_);
  jet_bdiscriminant11_branch = tree->GetBranch("jet_bdiscriminant11");
  if (jet_bdiscriminant11_branch) jet_bdiscriminant11_branch->SetAddress(&jet_bdiscriminant11_);
  TriggerWeightUp01sigma_branch = tree->GetBranch("TriggerWeightUp01sigma");
  if (TriggerWeightUp01sigma_branch) TriggerWeightUp01sigma_branch->SetAddress(&TriggerWeightUp01sigma_);
  jet_energy11_branch = tree->GetBranch("jet_energy11");
  if (jet_energy11_branch) jet_energy11_branch->SetAddress(&jet_energy11_);
  muon1_pt_branch = tree->GetBranch("muon1_pt");
  if (muon1_pt_branch) muon1_pt_branch->SetAddress(&muon1_pt_);
  jet_bdiscriminant4_branch = tree->GetBranch("jet_bdiscriminant4");
  if (jet_bdiscriminant4_branch) jet_bdiscriminant4_branch->SetAddress(&jet_bdiscriminant4_);
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
  jet_pt6_isLoaded = false;
  jet_phi12_isLoaded = false;
  lead_MomID_isLoaded = false;
  sublead_SimpleMomStatus_isLoaded = false;
  jet_bdiscriminant10_isLoaded = false;
  jet_pt1_isLoaded = false;
  nGoodEls_isLoaded = false;
  jet_pt3_isLoaded = false;
  lead_SmallestDr_isLoaded = false;
  jet_phi8_isLoaded = false;
  sublead_Mad_isLoaded = false;
  jet_bbdiscriminant8_isLoaded = false;
  jet_udsgdiscriminant8_isLoaded = false;
  jet_hadronFlavour8_isLoaded = false;
  ele2_eta_isLoaded = false;
  jet_pt15_isLoaded = false;
  topTag_topMass_isLoaded = false;
  dnn_score_ttgg_isLoaded = false;
  jet_bdiscriminant9_isLoaded = false;
  jet_cdiscriminant4_isLoaded = false;
  jet_phi6_isLoaded = false;
  diphoMVARes_isLoaded = false;
  jet_cdiscriminant5_isLoaded = false;
  sublead_sigmaEoE_isLoaded = false;
  jet_udsgdiscriminant13_isLoaded = false;
  jet_hadronFlavour13_isLoaded = false;
  sublead_photon_type_isLoaded = false;
  leadPtGen_isLoaded = false;
  sublead_Pythia_isLoaded = false;
  dZ_isLoaded = false;
  jet_energy8_isLoaded = false;
  sublead_closest_gen_Pt_isLoaded = false;
  jet_cdiscriminant12_isLoaded = false;
  jet_eta10_isLoaded = false;
  jet_udsgdiscriminant12_isLoaded = false;
  jet_hadronFlavour12_isLoaded = false;
  nGoodElsFromTau_isLoaded = false;
  muon2_eta_isLoaded = false;
  jet_bbdiscriminant4_isLoaded = false;
  LooseMvaSFDown01sigma_isLoaded = false;
  jet_cdiscriminant3_isLoaded = false;
  TriggerWeightDown01sigma_isLoaded = false;
  tthMVA_isLoaded = false;
  jet_bdiscriminant6_isLoaded = false;
  jet_pt8_isLoaded = false;
  MetPhi_isLoaded = false;
  muon1_eta_isLoaded = false;
  sublead_Prompt_isLoaded = false;
  leadIDMVA_isLoaded = false;
  jet_udsgdiscriminant7_isLoaded = false;
  jet_hadronFlavour7_isLoaded = false;
  jet_phi3_isLoaded = false;
  jet_cdiscriminant9_isLoaded = false;
  lead_PhoGenPt_isLoaded = false;
  jet_pt5_isLoaded = false;
  jet_phi13_isLoaded = false;
  sublead_closest_gen_dR_isLoaded = false;
  lead_Pythia_isLoaded = false;
  nGoodMusFromTau_isLoaded = false;
  ele1_pt_isLoaded = false;
  jet_phi14_isLoaded = false;
  ElectronWeightDown01sigma_isLoaded = false;
  jet_bdiscriminant14_isLoaded = false;
  jet_pt9_isLoaded = false;
  nGoodMus_isLoaded = false;
  LooseMvaSFUp01sigma_isLoaded = false;
  muon2_pt_isLoaded = false;
  jet_bdiscriminant5_isLoaded = false;
  jet_energy14_isLoaded = false;
  lead_MomMomID_isLoaded = false;
  jet_eta6_isLoaded = false;
  n_jets_isLoaded = false;
  UnmatchedPUWeightDown01sigma_isLoaded = false;
  jet_bbdiscriminant10_isLoaded = false;
  rand_isLoaded = false;
  jet_udsgdiscriminant4_isLoaded = false;
  jet_hadronFlavour4_isLoaded = false;
  subleadPt_isLoaded = false;
  mass_isLoaded = false;
  jet_energy10_isLoaded = false;
  jet_eta7_isLoaded = false;
  jet_phi11_isLoaded = false;
  sublead_MomMomID_isLoaded = false;
  jet_udsgdiscriminant10_isLoaded = false;
  jet_hadronFlavour10_isLoaded = false;
  MvaLinearSystDown01sigma_isLoaded = false;
  jet_phi1_isLoaded = false;
  electronVetoSFDown01sigma_isLoaded = false;
  jet_cdiscriminant8_isLoaded = false;
  jet_phi7_isLoaded = false;
  sublead_sieie_isLoaded = false;
  JetBTagReshapeWeightUp01sigma_isLoaded = false;
  jet_eta1_isLoaded = false;
  jet_energy3_isLoaded = false;
  lead_Mad_isLoaded = false;
  lead_PassFrix_isLoaded = false;
  jet_cdiscriminant6_isLoaded = false;
  ele1_charge_isLoaded = false;
  jet_bbdiscriminant11_isLoaded = false;
  PreselSFUp01sigma_isLoaded = false;
  jet_cdiscriminant2_isLoaded = false;
  weight_JetBTagWeight_isLoaded = false;
  jet_udsgdiscriminant6_isLoaded = false;
  jet_hadronFlavour6_isLoaded = false;
  sublead_hoe_isLoaded = false;
  muonSubleadIso_isLoaded = false;
  leadR9_isLoaded = false;
  dipho_cosphi_isLoaded = false;
  sublead_MomID_isLoaded = false;
  jet_eta13_isLoaded = false;
  FracRVWeightUp01sigma_isLoaded = false;
  subleadGenMatch_isLoaded = false;
  lead_PhoGenPhi_isLoaded = false;
  nElecLoose_isLoaded = false;
  jet_eta12_isLoaded = false;
  jet_udsgdiscriminant9_isLoaded = false;
  jet_hadronFlavour9_isLoaded = false;
  electronVetoSFUp01sigma_isLoaded = false;
  subleadGendeltaR_isLoaded = false;
  muon2_energy_isLoaded = false;
  sublead_PhoGenEta_isLoaded = false;
  jet_udsgdiscriminant15_isLoaded = false;
  jet_hadronFlavour15_isLoaded = false;
  jet_phi5_isLoaded = false;
  jet_eta2_isLoaded = false;
  subleadEta_isLoaded = false;
  jet_pt13_isLoaded = false;
  leadEt_isLoaded = false;
  jet_pt4_isLoaded = false;
  ElectronWeightUp01sigma_isLoaded = false;
  jet_udsgdiscriminant3_isLoaded = false;
  jet_hadronFlavour3_isLoaded = false;
  lead_sieie_isLoaded = false;
  MetPt_isLoaded = false;
  jet_cdiscriminant7_isLoaded = false;
  jet_udsgdiscriminant14_isLoaded = false;
  jet_hadronFlavour14_isLoaded = false;
  jet_bbdiscriminant14_isLoaded = false;
  muon1_energy_isLoaded = false;
  jet_eta9_isLoaded = false;
  jet_bbdiscriminant7_isLoaded = false;
  jet_bbdiscriminant9_isLoaded = false;
  muon2_charge_isLoaded = false;
  FracRVNvtxWeightUp01sigma_isLoaded = false;
  ele1_energy_isLoaded = false;
  nMuonMedium_isLoaded = false;
  jet_cdiscriminant14_isLoaded = false;
  nMuonTight_isLoaded = false;
  jet_energy12_isLoaded = false;
  nGoodTaus_isLoaded = false;
  jet_bbdiscriminant12_isLoaded = false;
  n_ele_isLoaded = false;
  muon1_charge_isLoaded = false;
  jet_phi15_isLoaded = false;
  jet_eta5_isLoaded = false;
  subleadEt_isLoaded = false;
  lead_closest_gen_dR_isLoaded = false;
  Mjj_isLoaded = false;
  jet_udsgdiscriminant5_isLoaded = false;
  jet_hadronFlavour5_isLoaded = false;
  subleadIDMVA_isLoaded = false;
  jet_cdiscriminant11_isLoaded = false;
  jet_pt14_isLoaded = false;
  lead_closest_gen_Pt_isLoaded = false;
  UnmatchedPUWeightUp01sigma_isLoaded = false;
  lead_SimpleMomID_isLoaded = false;
  nElecTight_isLoaded = false;
  ele1_eta_isLoaded = false;
  jet_bdiscriminant8_isLoaded = false;
  jet_bbdiscriminant13_isLoaded = false;
  leadEta_isLoaded = false;
  ele2_energy_isLoaded = false;
  nb_medium_isLoaded = false;
  JetBTagCutWeightDown01sigma_isLoaded = false;
  jet_pt11_isLoaded = false;
  jet_pt10_isLoaded = false;
  jet_udsgdiscriminant1_isLoaded = false;
  jet_hadronFlavour1_isLoaded = false;
  muonLeadIso_isLoaded = false;
  subleadPhi_isLoaded = false;
  jet_cdiscriminant13_isLoaded = false;
  subleadPixelSeed_isLoaded = false;
  n_muons_isLoaded = false;
  lead_SimpleMomStatus_isLoaded = false;
  jet_energy6_isLoaded = false;
  sublead_SmallestDr_isLoaded = false;
  dipho_rapidity_isLoaded = false;
  jet_bdiscriminant12_isLoaded = false;
  subleadEnergy_isLoaded = false;
  jet_phi2_isLoaded = false;
  jet_cdiscriminant10_isLoaded = false;
  lead_hoe_isLoaded = false;
  sublead_ptoM_isLoaded = false;
  jet_bbdiscriminant6_isLoaded = false;
  jet_energy15_isLoaded = false;
  leadPixelSeed_isLoaded = false;
  vertex_idx_isLoaded = false;
  jet_bbdiscriminant1_isLoaded = false;
  sublead_PhoGenPt_isLoaded = false;
  jet_energy4_isLoaded = false;
  jet_energy7_isLoaded = false;
  JetBTagCutWeightUp01sigma_isLoaded = false;
  MvaLinearSystUp01sigma_isLoaded = false;
  jet_eta3_isLoaded = false;
  jet_energy2_isLoaded = false;
  nb_tight_isLoaded = false;
  jet_bbdiscriminant5_isLoaded = false;
  jet_bdiscriminant1_isLoaded = false;
  n_bjets_isLoaded = false;
  jet_bbdiscriminant15_isLoaded = false;
  subleadPtGen_isLoaded = false;
  jet_pt2_isLoaded = false;
  lead_photon_type_isLoaded = false;
  FracRVNvtxWeightDown01sigma_isLoaded = false;
  PreselSFDown01sigma_isLoaded = false;
  lead_sigmaEoE_isLoaded = false;
  muon1_phi_isLoaded = false;
  dipho_sumpt_isLoaded = false;
  jet_bdiscriminant3_isLoaded = false;
  subleadPassEVeto_isLoaded = false;
  jet_energy1_isLoaded = false;
  jet_eta15_isLoaded = false;
  topTag_score_isLoaded = false;
  jet_phi10_isLoaded = false;
  jet_eta8_isLoaded = false;
  CMS_hgg_mass_isLoaded = false;
  centralObjectWeight_isLoaded = false;
  jet_bbdiscriminant2_isLoaded = false;
  jet_eta11_isLoaded = false;
  nMuonLoose_isLoaded = false;
  jet_bbdiscriminant3_isLoaded = false;
  FracRVWeightDown01sigma_isLoaded = false;
  jet_energy9_isLoaded = false;
  sublead_PassFrix_isLoaded = false;
  ele1_phi_isLoaded = false;
  ele2_charge_isLoaded = false;
  jet_bdiscriminant15_isLoaded = false;
  jet_phi9_isLoaded = false;
  jet_energy13_isLoaded = false;
  muon2_phi_isLoaded = false;
  jet_pt7_isLoaded = false;
  topTag_WMass_isLoaded = false;
  jet_cdiscriminant15_isLoaded = false;
  jet_bdiscriminant7_isLoaded = false;
  lead_PhoGenEta_isLoaded = false;
  jet_bdiscriminant2_isLoaded = false;
  leadEnergy_isLoaded = false;
  jet_cdiscriminant1_isLoaded = false;
  leadPt_isLoaded = false;
  leadPassEVeto_isLoaded = false;
  lead_Prompt_isLoaded = false;
  mT_isLoaded = false;
  jet_bdiscriminant13_isLoaded = false;
  jet_phi4_isLoaded = false;
  jet_pt12_isLoaded = false;
  leadGendeltaR_isLoaded = false;
  jet_energy5_isLoaded = false;
  nElecMedium_isLoaded = false;
  tthMVA_RunII_isLoaded = false;
  prefireProbability_isLoaded = false;
  leadPhi_isLoaded = false;
  subleadR9_isLoaded = false;
  jet_udsgdiscriminant11_isLoaded = false;
  jet_hadronFlavour11_isLoaded = false;
  jet_eta14_isLoaded = false;
  leadGenMatch_isLoaded = false;
  sublead_SimpleMomID_isLoaded = false;
  ele2_phi_isLoaded = false;
  jet_udsgdiscriminant2_isLoaded = false;
  jet_hadronFlavour2_isLoaded = false;
  sublead_PhoGenPhi_isLoaded = false;
  jet_eta4_isLoaded = false;
  nb_loose_isLoaded = false;
  JetBTagReshapeWeightDown01sigma_isLoaded = false;
  lead_ptoM_isLoaded = false;
  ele2_pt_isLoaded = false;
  jet_bdiscriminant11_isLoaded = false;
  TriggerWeightUp01sigma_isLoaded = false;
  jet_energy11_isLoaded = false;
  muon1_pt_isLoaded = false;
  jet_bdiscriminant4_isLoaded = false;
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
  if (jet_pt6_branch != 0) jet_pt6();
  if (jet_phi12_branch != 0) jet_phi12();
  if (lead_MomID_branch != 0) lead_MomID();
  if (sublead_SimpleMomStatus_branch != 0) sublead_SimpleMomStatus();
  if (jet_bdiscriminant10_branch != 0) jet_bdiscriminant10();
  if (jet_pt1_branch != 0) jet_pt1();
  if (nGoodEls_branch != 0) nGoodEls();
  if (jet_pt3_branch != 0) jet_pt3();
  if (lead_SmallestDr_branch != 0) lead_SmallestDr();
  if (jet_phi8_branch != 0) jet_phi8();
  if (sublead_Mad_branch != 0) sublead_Mad();
  if (jet_bbdiscriminant8_branch != 0) jet_bbdiscriminant8();
  if (jet_udsgdiscriminant8_branch != 0) jet_udsgdiscriminant8();
  if (jet_hadronFlavour8_branch != 0) jet_hadronFlavour8();
  if (ele2_eta_branch != 0) ele2_eta();
  if (jet_pt15_branch != 0) jet_pt15();
  if (topTag_topMass_branch != 0) topTag_topMass();
  if (dnn_score_ttgg_branch != 0) dnn_score_ttgg();
  if (jet_bdiscriminant9_branch != 0) jet_bdiscriminant9();
  if (jet_cdiscriminant4_branch != 0) jet_cdiscriminant4();
  if (jet_phi6_branch != 0) jet_phi6();
  if (diphoMVARes_branch != 0) diphoMVARes();
  if (jet_cdiscriminant5_branch != 0) jet_cdiscriminant5();
  if (sublead_sigmaEoE_branch != 0) sublead_sigmaEoE();
  if (jet_udsgdiscriminant13_branch != 0) jet_udsgdiscriminant13();
  if (jet_hadronFlavour13_branch != 0) jet_hadronFlavour13();
  if (sublead_photon_type_branch != 0) sublead_photon_type();
  if (leadPtGen_branch != 0) leadPtGen();
  if (sublead_Pythia_branch != 0) sublead_Pythia();
  if (dZ_branch != 0) dZ();
  if (jet_energy8_branch != 0) jet_energy8();
  if (sublead_closest_gen_Pt_branch != 0) sublead_closest_gen_Pt();
  if (jet_cdiscriminant12_branch != 0) jet_cdiscriminant12();
  if (jet_eta10_branch != 0) jet_eta10();
  if (jet_udsgdiscriminant12_branch != 0) jet_udsgdiscriminant12();
  if (jet_hadronFlavour12_branch != 0) jet_hadronFlavour12();
  if (nGoodElsFromTau_branch != 0) nGoodElsFromTau();
  if (muon2_eta_branch != 0) muon2_eta();
  if (jet_bbdiscriminant4_branch != 0) jet_bbdiscriminant4();
  if (LooseMvaSFDown01sigma_branch != 0) LooseMvaSFDown01sigma();
  if (jet_cdiscriminant3_branch != 0) jet_cdiscriminant3();
  if (TriggerWeightDown01sigma_branch != 0) TriggerWeightDown01sigma();
  if (tthMVA_branch != 0) tthMVA();
  if (jet_bdiscriminant6_branch != 0) jet_bdiscriminant6();
  if (jet_pt8_branch != 0) jet_pt8();
  if (MetPhi_branch != 0) MetPhi();
  if (muon1_eta_branch != 0) muon1_eta();
  if (sublead_Prompt_branch != 0) sublead_Prompt();
  if (leadIDMVA_branch != 0) leadIDMVA();
  if (jet_udsgdiscriminant7_branch != 0) jet_udsgdiscriminant7();
  if (jet_hadronFlavour7_branch != 0) jet_hadronFlavour7();
  if (jet_phi3_branch != 0) jet_phi3();
  if (jet_cdiscriminant9_branch != 0) jet_cdiscriminant9();
  if (lead_PhoGenPt_branch != 0) lead_PhoGenPt();
  if (jet_pt5_branch != 0) jet_pt5();
  if (jet_phi13_branch != 0) jet_phi13();
  if (sublead_closest_gen_dR_branch != 0) sublead_closest_gen_dR();
  if (lead_Pythia_branch != 0) lead_Pythia();
  if (nGoodMusFromTau_branch != 0) nGoodMusFromTau();
  if (ele1_pt_branch != 0) ele1_pt();
  if (jet_phi14_branch != 0) jet_phi14();
  if (ElectronWeightDown01sigma_branch != 0) ElectronWeightDown01sigma();
  if (jet_bdiscriminant14_branch != 0) jet_bdiscriminant14();
  if (jet_pt9_branch != 0) jet_pt9();
  if (nGoodMus_branch != 0) nGoodMus();
  if (LooseMvaSFUp01sigma_branch != 0) LooseMvaSFUp01sigma();
  if (muon2_pt_branch != 0) muon2_pt();
  if (jet_bdiscriminant5_branch != 0) jet_bdiscriminant5();
  if (jet_energy14_branch != 0) jet_energy14();
  if (lead_MomMomID_branch != 0) lead_MomMomID();
  if (jet_eta6_branch != 0) jet_eta6();
  if (n_jets_branch != 0) n_jets();
  if (UnmatchedPUWeightDown01sigma_branch != 0) UnmatchedPUWeightDown01sigma();
  if (jet_bbdiscriminant10_branch != 0) jet_bbdiscriminant10();
  if (rand_branch != 0) rand();
  if (jet_udsgdiscriminant4_branch != 0) jet_udsgdiscriminant4();
  if (jet_hadronFlavour4_branch != 0) jet_hadronFlavour4();
  if (subleadPt_branch != 0) subleadPt();
  if (mass_branch != 0) mass();
  if (jet_energy10_branch != 0) jet_energy10();
  if (jet_eta7_branch != 0) jet_eta7();
  if (jet_phi11_branch != 0) jet_phi11();
  if (sublead_MomMomID_branch != 0) sublead_MomMomID();
  if (jet_udsgdiscriminant10_branch != 0) jet_udsgdiscriminant10();
  if (jet_hadronFlavour10_branch != 0) jet_hadronFlavour10();
  if (MvaLinearSystDown01sigma_branch != 0) MvaLinearSystDown01sigma();
  if (jet_phi1_branch != 0) jet_phi1();
  if (electronVetoSFDown01sigma_branch != 0) electronVetoSFDown01sigma();
  if (jet_cdiscriminant8_branch != 0) jet_cdiscriminant8();
  if (jet_phi7_branch != 0) jet_phi7();
  if (sublead_sieie_branch != 0) sublead_sieie();
  if (JetBTagReshapeWeightUp01sigma_branch != 0) JetBTagReshapeWeightUp01sigma();
  if (jet_eta1_branch != 0) jet_eta1();
  if (jet_energy3_branch != 0) jet_energy3();
  if (lead_Mad_branch != 0) lead_Mad();
  if (lead_PassFrix_branch != 0) lead_PassFrix();
  if (jet_cdiscriminant6_branch != 0) jet_cdiscriminant6();
  if (ele1_charge_branch != 0) ele1_charge();
  if (jet_bbdiscriminant11_branch != 0) jet_bbdiscriminant11();
  if (PreselSFUp01sigma_branch != 0) PreselSFUp01sigma();
  if (jet_cdiscriminant2_branch != 0) jet_cdiscriminant2();
  if (weight_JetBTagWeight_branch != 0) weight_JetBTagWeight();
  if (jet_udsgdiscriminant6_branch != 0) jet_udsgdiscriminant6();
  if (jet_hadronFlavour6_branch != 0) jet_hadronFlavour6();
  if (sublead_hoe_branch != 0) sublead_hoe();
  if (muonSubleadIso_branch != 0) muonSubleadIso();
  if (leadR9_branch != 0) leadR9();
  if (dipho_cosphi_branch != 0) dipho_cosphi();
  if (sublead_MomID_branch != 0) sublead_MomID();
  if (jet_eta13_branch != 0) jet_eta13();
  if (FracRVWeightUp01sigma_branch != 0) FracRVWeightUp01sigma();
  if (subleadGenMatch_branch != 0) subleadGenMatch();
  if (lead_PhoGenPhi_branch != 0) lead_PhoGenPhi();
  if (nElecLoose_branch != 0) nElecLoose();
  if (jet_eta12_branch != 0) jet_eta12();
  if (jet_udsgdiscriminant9_branch != 0) jet_udsgdiscriminant9();
  if (jet_hadronFlavour9_branch != 0) jet_hadronFlavour9();
  if (electronVetoSFUp01sigma_branch != 0) electronVetoSFUp01sigma();
  if (subleadGendeltaR_branch != 0) subleadGendeltaR();
  if (muon2_energy_branch != 0) muon2_energy();
  if (sublead_PhoGenEta_branch != 0) sublead_PhoGenEta();
  if (jet_udsgdiscriminant15_branch != 0) jet_udsgdiscriminant15();
  if (jet_hadronFlavour15_branch != 0) jet_hadronFlavour15();
  if (jet_phi5_branch != 0) jet_phi5();
  if (jet_eta2_branch != 0) jet_eta2();
  if (subleadEta_branch != 0) subleadEta();
  if (jet_pt13_branch != 0) jet_pt13();
  if (leadEt_branch != 0) leadEt();
  if (jet_pt4_branch != 0) jet_pt4();
  if (ElectronWeightUp01sigma_branch != 0) ElectronWeightUp01sigma();
  if (jet_udsgdiscriminant3_branch != 0) jet_udsgdiscriminant3();
  if (jet_hadronFlavour3_branch != 0) jet_hadronFlavour3();
  if (lead_sieie_branch != 0) lead_sieie();
  if (MetPt_branch != 0) MetPt();
  if (jet_cdiscriminant7_branch != 0) jet_cdiscriminant7();
  if (jet_udsgdiscriminant14_branch != 0) jet_udsgdiscriminant14();
  if (jet_hadronFlavour14_branch != 0) jet_hadronFlavour14();
  if (jet_bbdiscriminant14_branch != 0) jet_bbdiscriminant14();
  if (muon1_energy_branch != 0) muon1_energy();
  if (jet_eta9_branch != 0) jet_eta9();
  if (jet_bbdiscriminant7_branch != 0) jet_bbdiscriminant7();
  if (jet_bbdiscriminant9_branch != 0) jet_bbdiscriminant9();
  if (muon2_charge_branch != 0) muon2_charge();
  if (FracRVNvtxWeightUp01sigma_branch != 0) FracRVNvtxWeightUp01sigma();
  if (ele1_energy_branch != 0) ele1_energy();
  if (nMuonMedium_branch != 0) nMuonMedium();
  if (jet_cdiscriminant14_branch != 0) jet_cdiscriminant14();
  if (nMuonTight_branch != 0) nMuonTight();
  if (jet_energy12_branch != 0) jet_energy12();
  if (nGoodTaus_branch != 0) nGoodTaus();
  if (jet_bbdiscriminant12_branch != 0) jet_bbdiscriminant12();
  if (n_ele_branch != 0) n_ele();
  if (muon1_charge_branch != 0) muon1_charge();
  if (jet_phi15_branch != 0) jet_phi15();
  if (jet_eta5_branch != 0) jet_eta5();
  if (subleadEt_branch != 0) subleadEt();
  if (lead_closest_gen_dR_branch != 0) lead_closest_gen_dR();
  if (Mjj_branch != 0) Mjj();
  if (jet_udsgdiscriminant5_branch != 0) jet_udsgdiscriminant5();
  if (jet_hadronFlavour5_branch != 0) jet_hadronFlavour5();
  if (subleadIDMVA_branch != 0) subleadIDMVA();
  if (jet_cdiscriminant11_branch != 0) jet_cdiscriminant11();
  if (jet_pt14_branch != 0) jet_pt14();
  if (lead_closest_gen_Pt_branch != 0) lead_closest_gen_Pt();
  if (UnmatchedPUWeightUp01sigma_branch != 0) UnmatchedPUWeightUp01sigma();
  if (lead_SimpleMomID_branch != 0) lead_SimpleMomID();
  if (nElecTight_branch != 0) nElecTight();
  if (ele1_eta_branch != 0) ele1_eta();
  if (jet_bdiscriminant8_branch != 0) jet_bdiscriminant8();
  if (jet_bbdiscriminant13_branch != 0) jet_bbdiscriminant13();
  if (leadEta_branch != 0) leadEta();
  if (ele2_energy_branch != 0) ele2_energy();
  if (nb_medium_branch != 0) nb_medium();
  if (JetBTagCutWeightDown01sigma_branch != 0) JetBTagCutWeightDown01sigma();
  if (jet_pt11_branch != 0) jet_pt11();
  if (jet_pt10_branch != 0) jet_pt10();
  if (jet_udsgdiscriminant1_branch != 0) jet_udsgdiscriminant1();
  if (jet_hadronFlavour1_branch != 0) jet_hadronFlavour1();
  if (muonLeadIso_branch != 0) muonLeadIso();
  if (subleadPhi_branch != 0) subleadPhi();
  if (jet_cdiscriminant13_branch != 0) jet_cdiscriminant13();
  if (subleadPixelSeed_branch != 0) subleadPixelSeed();
  if (n_muons_branch != 0) n_muons();
  if (lead_SimpleMomStatus_branch != 0) lead_SimpleMomStatus();
  if (jet_energy6_branch != 0) jet_energy6();
  if (sublead_SmallestDr_branch != 0) sublead_SmallestDr();
  if (dipho_rapidity_branch != 0) dipho_rapidity();
  if (jet_bdiscriminant12_branch != 0) jet_bdiscriminant12();
  if (subleadEnergy_branch != 0) subleadEnergy();
  if (jet_phi2_branch != 0) jet_phi2();
  if (jet_cdiscriminant10_branch != 0) jet_cdiscriminant10();
  if (lead_hoe_branch != 0) lead_hoe();
  if (sublead_ptoM_branch != 0) sublead_ptoM();
  if (jet_bbdiscriminant6_branch != 0) jet_bbdiscriminant6();
  if (jet_energy15_branch != 0) jet_energy15();
  if (leadPixelSeed_branch != 0) leadPixelSeed();
  if (vertex_idx_branch != 0) vertex_idx();
  if (jet_bbdiscriminant1_branch != 0) jet_bbdiscriminant1();
  if (sublead_PhoGenPt_branch != 0) sublead_PhoGenPt();
  if (jet_energy4_branch != 0) jet_energy4();
  if (jet_energy7_branch != 0) jet_energy7();
  if (JetBTagCutWeightUp01sigma_branch != 0) JetBTagCutWeightUp01sigma();
  if (MvaLinearSystUp01sigma_branch != 0) MvaLinearSystUp01sigma();
  if (jet_eta3_branch != 0) jet_eta3();
  if (jet_energy2_branch != 0) jet_energy2();
  if (nb_tight_branch != 0) nb_tight();
  if (jet_bbdiscriminant5_branch != 0) jet_bbdiscriminant5();
  if (jet_bdiscriminant1_branch != 0) jet_bdiscriminant1();
  if (n_bjets_branch != 0) n_bjets();
  if (jet_bbdiscriminant15_branch != 0) jet_bbdiscriminant15();
  if (subleadPtGen_branch != 0) subleadPtGen();
  if (jet_pt2_branch != 0) jet_pt2();
  if (lead_photon_type_branch != 0) lead_photon_type();
  if (FracRVNvtxWeightDown01sigma_branch != 0) FracRVNvtxWeightDown01sigma();
  if (PreselSFDown01sigma_branch != 0) PreselSFDown01sigma();
  if (lead_sigmaEoE_branch != 0) lead_sigmaEoE();
  if (muon1_phi_branch != 0) muon1_phi();
  if (dipho_sumpt_branch != 0) dipho_sumpt();
  if (jet_bdiscriminant3_branch != 0) jet_bdiscriminant3();
  if (subleadPassEVeto_branch != 0) subleadPassEVeto();
  if (jet_energy1_branch != 0) jet_energy1();
  if (jet_eta15_branch != 0) jet_eta15();
  if (topTag_score_branch != 0) topTag_score();
  if (jet_phi10_branch != 0) jet_phi10();
  if (jet_eta8_branch != 0) jet_eta8();
  if (CMS_hgg_mass_branch != 0) CMS_hgg_mass();
  if (centralObjectWeight_branch != 0) centralObjectWeight();
  if (jet_bbdiscriminant2_branch != 0) jet_bbdiscriminant2();
  if (jet_eta11_branch != 0) jet_eta11();
  if (nMuonLoose_branch != 0) nMuonLoose();
  if (jet_bbdiscriminant3_branch != 0) jet_bbdiscriminant3();
  if (FracRVWeightDown01sigma_branch != 0) FracRVWeightDown01sigma();
  if (jet_energy9_branch != 0) jet_energy9();
  if (sublead_PassFrix_branch != 0) sublead_PassFrix();
  if (ele1_phi_branch != 0) ele1_phi();
  if (ele2_charge_branch != 0) ele2_charge();
  if (jet_bdiscriminant15_branch != 0) jet_bdiscriminant15();
  if (jet_phi9_branch != 0) jet_phi9();
  if (jet_energy13_branch != 0) jet_energy13();
  if (muon2_phi_branch != 0) muon2_phi();
  if (jet_pt7_branch != 0) jet_pt7();
  if (topTag_WMass_branch != 0) topTag_WMass();
  if (jet_cdiscriminant15_branch != 0) jet_cdiscriminant15();
  if (jet_bdiscriminant7_branch != 0) jet_bdiscriminant7();
  if (lead_PhoGenEta_branch != 0) lead_PhoGenEta();
  if (jet_bdiscriminant2_branch != 0) jet_bdiscriminant2();
  if (leadEnergy_branch != 0) leadEnergy();
  if (jet_cdiscriminant1_branch != 0) jet_cdiscriminant1();
  if (leadPt_branch != 0) leadPt();
  if (leadPassEVeto_branch != 0) leadPassEVeto();
  if (lead_Prompt_branch != 0) lead_Prompt();
  if (mT_branch != 0) mT();
  if (jet_bdiscriminant13_branch != 0) jet_bdiscriminant13();
  if (jet_phi4_branch != 0) jet_phi4();
  if (jet_pt12_branch != 0) jet_pt12();
  if (leadGendeltaR_branch != 0) leadGendeltaR();
  if (jet_energy5_branch != 0) jet_energy5();
  if (nElecMedium_branch != 0) nElecMedium();
  if (tthMVA_RunII_branch != 0) tthMVA_RunII();
  if (prefireProbability_branch != 0) prefireProbability();
  if (leadPhi_branch != 0) leadPhi();
  if (subleadR9_branch != 0) subleadR9();
  if (jet_udsgdiscriminant11_branch != 0) jet_udsgdiscriminant11();
  if (jet_hadronFlavour11_branch != 0) jet_hadronFlavour11();
  if (jet_eta14_branch != 0) jet_eta14();
  if (leadGenMatch_branch != 0) leadGenMatch();
  if (sublead_SimpleMomID_branch != 0) sublead_SimpleMomID();
  if (ele2_phi_branch != 0) ele2_phi();
  if (jet_udsgdiscriminant2_branch != 0) jet_udsgdiscriminant2();
  if (jet_hadronFlavour2_branch != 0) jet_hadronFlavour2();
  if (sublead_PhoGenPhi_branch != 0) sublead_PhoGenPhi();
  if (jet_eta4_branch != 0) jet_eta4();
  if (nb_loose_branch != 0) nb_loose();
  if (JetBTagReshapeWeightDown01sigma_branch != 0) JetBTagReshapeWeightDown01sigma();
  if (lead_ptoM_branch != 0) lead_ptoM();
  if (ele2_pt_branch != 0) ele2_pt();
  if (jet_bdiscriminant11_branch != 0) jet_bdiscriminant11();
  if (TriggerWeightUp01sigma_branch != 0) TriggerWeightUp01sigma();
  if (jet_energy11_branch != 0) jet_energy11();
  if (muon1_pt_branch != 0) muon1_pt();
  if (jet_bdiscriminant4_branch != 0) jet_bdiscriminant4();
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

const float &ttHLeptonic::lead_MomID() {
  if (not lead_MomID_isLoaded) {
    if (lead_MomID_branch != 0) {
      lead_MomID_branch->GetEntry(index);
    } else {
      printf("branch lead_MomID_branch does not exist!\n");
      exit(1);
    }
    lead_MomID_isLoaded = true;
  }
  return lead_MomID_;
}

const float &ttHLeptonic::sublead_SimpleMomStatus() {
  if (not sublead_SimpleMomStatus_isLoaded) {
    if (sublead_SimpleMomStatus_branch != 0) {
      sublead_SimpleMomStatus_branch->GetEntry(index);
    } else {
      printf("branch sublead_SimpleMomStatus_branch does not exist!\n");
      exit(1);
    }
    sublead_SimpleMomStatus_isLoaded = true;
  }
  return sublead_SimpleMomStatus_;
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

const float &ttHLeptonic::nGoodEls() {
  if (not nGoodEls_isLoaded) {
    if (nGoodEls_branch != 0) {
      nGoodEls_branch->GetEntry(index);
    } else {
      printf("branch nGoodEls_branch does not exist!\n");
      exit(1);
    }
    nGoodEls_isLoaded = true;
  }
  return nGoodEls_;
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

const float &ttHLeptonic::lead_SmallestDr() {
  if (not lead_SmallestDr_isLoaded) {
    if (lead_SmallestDr_branch != 0) {
      lead_SmallestDr_branch->GetEntry(index);
    } else {
      printf("branch lead_SmallestDr_branch does not exist!\n");
      exit(1);
    }
    lead_SmallestDr_isLoaded = true;
  }
  return lead_SmallestDr_;
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

const float &ttHLeptonic::sublead_Mad() {
  if (not sublead_Mad_isLoaded) {
    if (sublead_Mad_branch != 0) {
      sublead_Mad_branch->GetEntry(index);
    } else {
      printf("branch sublead_Mad_branch does not exist!\n");
      exit(1);
    }
    sublead_Mad_isLoaded = true;
  }
  return sublead_Mad_;
}

const float &ttHLeptonic::jet_bbdiscriminant8() {
  if (not jet_bbdiscriminant8_isLoaded) {
    if (jet_bbdiscriminant8_branch != 0) {
      jet_bbdiscriminant8_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant8_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant8_isLoaded = true;
  }
  return jet_bbdiscriminant8_;
}

const float &ttHLeptonic::jet_udsgdiscriminant8() {
  if (not jet_udsgdiscriminant8_isLoaded) {
    if (jet_udsgdiscriminant8_branch != 0) {
      jet_udsgdiscriminant8_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant8_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant8_isLoaded = true;
  }
  return jet_udsgdiscriminant8_;
}

const float &ttHLeptonic::jet_hadronFlavour8() {
  if (not jet_hadronFlavour8_isLoaded) {
    if (jet_hadronFlavour8_branch != 0) {
      jet_hadronFlavour8_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour8_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour8_isLoaded = true;
  }
  return jet_hadronFlavour8_;
}

const float &ttHLeptonic::ele2_eta() {
  if (not ele2_eta_isLoaded) {
    if (ele2_eta_branch != 0) {
      ele2_eta_branch->GetEntry(index);
    } else {
      printf("branch ele2_eta_branch does not exist!\n");
      exit(1);
    }
    ele2_eta_isLoaded = true;
  }
  return ele2_eta_;
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

const float &ttHLeptonic::topTag_topMass() {
  if (not topTag_topMass_isLoaded) {
    if (topTag_topMass_branch != 0) {
      topTag_topMass_branch->GetEntry(index);
    } else {
      printf("branch topTag_topMass_branch does not exist!\n");
      exit(1);
    }
    topTag_topMass_isLoaded = true;
  }
  return topTag_topMass_;
}

const float &ttHLeptonic::dnn_score_ttgg() {
  if (not dnn_score_ttgg_isLoaded) {
    if (dnn_score_ttgg_branch != 0) {
      dnn_score_ttgg_branch->GetEntry(index);
    } else {
      printf("branch dnn_score_ttgg_branch does not exist!\n");
      exit(1);
    }
    dnn_score_ttgg_isLoaded = true;
  }
  return dnn_score_ttgg_;
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

const float &ttHLeptonic::jet_cdiscriminant4() {
  if (not jet_cdiscriminant4_isLoaded) {
    if (jet_cdiscriminant4_branch != 0) {
      jet_cdiscriminant4_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant4_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant4_isLoaded = true;
  }
  return jet_cdiscriminant4_;
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

const float &ttHLeptonic::diphoMVARes() {
  if (not diphoMVARes_isLoaded) {
    if (diphoMVARes_branch != 0) {
      diphoMVARes_branch->GetEntry(index);
    } else {
      printf("branch diphoMVARes_branch does not exist!\n");
      exit(1);
    }
    diphoMVARes_isLoaded = true;
  }
  return diphoMVARes_;
}

const float &ttHLeptonic::jet_cdiscriminant5() {
  if (not jet_cdiscriminant5_isLoaded) {
    if (jet_cdiscriminant5_branch != 0) {
      jet_cdiscriminant5_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant5_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant5_isLoaded = true;
  }
  return jet_cdiscriminant5_;
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

const float &ttHLeptonic::jet_udsgdiscriminant13() {
  if (not jet_udsgdiscriminant13_isLoaded) {
    if (jet_udsgdiscriminant13_branch != 0) {
      jet_udsgdiscriminant13_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant13_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant13_isLoaded = true;
  }
  return jet_udsgdiscriminant13_;
}

const float &ttHLeptonic::jet_hadronFlavour13() {
  if (not jet_hadronFlavour13_isLoaded) {
    if (jet_hadronFlavour13_branch != 0) {
      jet_hadronFlavour13_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour13_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour13_isLoaded = true;
  }
  return jet_hadronFlavour13_;
}

const float &ttHLeptonic::sublead_photon_type() {
  if (not sublead_photon_type_isLoaded) {
    if (sublead_photon_type_branch != 0) {
      sublead_photon_type_branch->GetEntry(index);
    } else {
      printf("branch sublead_photon_type_branch does not exist!\n");
      exit(1);
    }
    sublead_photon_type_isLoaded = true;
  }
  return sublead_photon_type_;
}

const float &ttHLeptonic::leadPtGen() {
  if (not leadPtGen_isLoaded) {
    if (leadPtGen_branch != 0) {
      leadPtGen_branch->GetEntry(index);
    } else {
      printf("branch leadPtGen_branch does not exist!\n");
      exit(1);
    }
    leadPtGen_isLoaded = true;
  }
  return leadPtGen_;
}

const float &ttHLeptonic::sublead_Pythia() {
  if (not sublead_Pythia_isLoaded) {
    if (sublead_Pythia_branch != 0) {
      sublead_Pythia_branch->GetEntry(index);
    } else {
      printf("branch sublead_Pythia_branch does not exist!\n");
      exit(1);
    }
    sublead_Pythia_isLoaded = true;
  }
  return sublead_Pythia_;
}

const float &ttHLeptonic::dZ() {
  if (not dZ_isLoaded) {
    if (dZ_branch != 0) {
      dZ_branch->GetEntry(index);
    } else {
      printf("branch dZ_branch does not exist!\n");
      exit(1);
    }
    dZ_isLoaded = true;
  }
  return dZ_;
}

const float &ttHLeptonic::jet_energy8() {
  if (not jet_energy8_isLoaded) {
    if (jet_energy8_branch != 0) {
      jet_energy8_branch->GetEntry(index);
    } else {
      printf("branch jet_energy8_branch does not exist!\n");
      exit(1);
    }
    jet_energy8_isLoaded = true;
  }
  return jet_energy8_;
}

const float &ttHLeptonic::sublead_closest_gen_Pt() {
  if (not sublead_closest_gen_Pt_isLoaded) {
    if (sublead_closest_gen_Pt_branch != 0) {
      sublead_closest_gen_Pt_branch->GetEntry(index);
    } else {
      printf("branch sublead_closest_gen_Pt_branch does not exist!\n");
      exit(1);
    }
    sublead_closest_gen_Pt_isLoaded = true;
  }
  return sublead_closest_gen_Pt_;
}

const float &ttHLeptonic::jet_cdiscriminant12() {
  if (not jet_cdiscriminant12_isLoaded) {
    if (jet_cdiscriminant12_branch != 0) {
      jet_cdiscriminant12_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant12_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant12_isLoaded = true;
  }
  return jet_cdiscriminant12_;
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

const float &ttHLeptonic::jet_udsgdiscriminant12() {
  if (not jet_udsgdiscriminant12_isLoaded) {
    if (jet_udsgdiscriminant12_branch != 0) {
      jet_udsgdiscriminant12_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant12_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant12_isLoaded = true;
  }
  return jet_udsgdiscriminant12_;
}

const float &ttHLeptonic::jet_hadronFlavour12() {
  if (not jet_hadronFlavour12_isLoaded) {
    if (jet_hadronFlavour12_branch != 0) {
      jet_hadronFlavour12_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour12_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour12_isLoaded = true;
  }
  return jet_hadronFlavour12_;
}

const float &ttHLeptonic::nGoodElsFromTau() {
  if (not nGoodElsFromTau_isLoaded) {
    if (nGoodElsFromTau_branch != 0) {
      nGoodElsFromTau_branch->GetEntry(index);
    } else {
      printf("branch nGoodElsFromTau_branch does not exist!\n");
      exit(1);
    }
    nGoodElsFromTau_isLoaded = true;
  }
  return nGoodElsFromTau_;
}

const float &ttHLeptonic::muon2_eta() {
  if (not muon2_eta_isLoaded) {
    if (muon2_eta_branch != 0) {
      muon2_eta_branch->GetEntry(index);
    } else {
      printf("branch muon2_eta_branch does not exist!\n");
      exit(1);
    }
    muon2_eta_isLoaded = true;
  }
  return muon2_eta_;
}

const float &ttHLeptonic::jet_bbdiscriminant4() {
  if (not jet_bbdiscriminant4_isLoaded) {
    if (jet_bbdiscriminant4_branch != 0) {
      jet_bbdiscriminant4_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant4_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant4_isLoaded = true;
  }
  return jet_bbdiscriminant4_;
}

const float &ttHLeptonic::LooseMvaSFDown01sigma() {
  if (not LooseMvaSFDown01sigma_isLoaded) {
    if (LooseMvaSFDown01sigma_branch != 0) {
      LooseMvaSFDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch LooseMvaSFDown01sigma_branch does not exist!\n");
      exit(1);
    }
    LooseMvaSFDown01sigma_isLoaded = true;
  }
  return LooseMvaSFDown01sigma_;
}

const float &ttHLeptonic::jet_cdiscriminant3() {
  if (not jet_cdiscriminant3_isLoaded) {
    if (jet_cdiscriminant3_branch != 0) {
      jet_cdiscriminant3_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant3_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant3_isLoaded = true;
  }
  return jet_cdiscriminant3_;
}

const float &ttHLeptonic::TriggerWeightDown01sigma() {
  if (not TriggerWeightDown01sigma_isLoaded) {
    if (TriggerWeightDown01sigma_branch != 0) {
      TriggerWeightDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch TriggerWeightDown01sigma_branch does not exist!\n");
      exit(1);
    }
    TriggerWeightDown01sigma_isLoaded = true;
  }
  return TriggerWeightDown01sigma_;
}

const float &ttHLeptonic::tthMVA() {
  if (not tthMVA_isLoaded) {
    if (tthMVA_branch != 0) {
      tthMVA_branch->GetEntry(index);
    } else {
      printf("branch tthMVA_branch does not exist!\n");
      exit(1);
    }
    tthMVA_isLoaded = true;
  }
  return tthMVA_;
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

const float &ttHLeptonic::MetPhi() {
  if (not MetPhi_isLoaded) {
    if (MetPhi_branch != 0) {
      MetPhi_branch->GetEntry(index);
    } else {
      printf("branch MetPhi_branch does not exist!\n");
      exit(1);
    }
    MetPhi_isLoaded = true;
  }
  return MetPhi_;
}

const float &ttHLeptonic::muon1_eta() {
  if (not muon1_eta_isLoaded) {
    if (muon1_eta_branch != 0) {
      muon1_eta_branch->GetEntry(index);
    } else {
      printf("branch muon1_eta_branch does not exist!\n");
      exit(1);
    }
    muon1_eta_isLoaded = true;
  }
  return muon1_eta_;
}

const float &ttHLeptonic::sublead_Prompt() {
  if (not sublead_Prompt_isLoaded) {
    if (sublead_Prompt_branch != 0) {
      sublead_Prompt_branch->GetEntry(index);
    } else {
      printf("branch sublead_Prompt_branch does not exist!\n");
      exit(1);
    }
    sublead_Prompt_isLoaded = true;
  }
  return sublead_Prompt_;
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

const float &ttHLeptonic::jet_udsgdiscriminant7() {
  if (not jet_udsgdiscriminant7_isLoaded) {
    if (jet_udsgdiscriminant7_branch != 0) {
      jet_udsgdiscriminant7_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant7_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant7_isLoaded = true;
  }
  return jet_udsgdiscriminant7_;
}

const float &ttHLeptonic::jet_hadronFlavour7() {
  if (not jet_hadronFlavour7_isLoaded) {
    if (jet_hadronFlavour7_branch != 0) {
      jet_hadronFlavour7_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour7_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour7_isLoaded = true;
  }
  return jet_hadronFlavour7_;
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

const float &ttHLeptonic::jet_cdiscriminant9() {
  if (not jet_cdiscriminant9_isLoaded) {
    if (jet_cdiscriminant9_branch != 0) {
      jet_cdiscriminant9_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant9_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant9_isLoaded = true;
  }
  return jet_cdiscriminant9_;
}

const float &ttHLeptonic::lead_PhoGenPt() {
  if (not lead_PhoGenPt_isLoaded) {
    if (lead_PhoGenPt_branch != 0) {
      lead_PhoGenPt_branch->GetEntry(index);
    } else {
      printf("branch lead_PhoGenPt_branch does not exist!\n");
      exit(1);
    }
    lead_PhoGenPt_isLoaded = true;
  }
  return lead_PhoGenPt_;
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

const float &ttHLeptonic::sublead_closest_gen_dR() {
  if (not sublead_closest_gen_dR_isLoaded) {
    if (sublead_closest_gen_dR_branch != 0) {
      sublead_closest_gen_dR_branch->GetEntry(index);
    } else {
      printf("branch sublead_closest_gen_dR_branch does not exist!\n");
      exit(1);
    }
    sublead_closest_gen_dR_isLoaded = true;
  }
  return sublead_closest_gen_dR_;
}

const float &ttHLeptonic::lead_Pythia() {
  if (not lead_Pythia_isLoaded) {
    if (lead_Pythia_branch != 0) {
      lead_Pythia_branch->GetEntry(index);
    } else {
      printf("branch lead_Pythia_branch does not exist!\n");
      exit(1);
    }
    lead_Pythia_isLoaded = true;
  }
  return lead_Pythia_;
}

const float &ttHLeptonic::nGoodMusFromTau() {
  if (not nGoodMusFromTau_isLoaded) {
    if (nGoodMusFromTau_branch != 0) {
      nGoodMusFromTau_branch->GetEntry(index);
    } else {
      printf("branch nGoodMusFromTau_branch does not exist!\n");
      exit(1);
    }
    nGoodMusFromTau_isLoaded = true;
  }
  return nGoodMusFromTau_;
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

const float &ttHLeptonic::ElectronWeightDown01sigma() {
  if (not ElectronWeightDown01sigma_isLoaded) {
    if (ElectronWeightDown01sigma_branch != 0) {
      ElectronWeightDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch ElectronWeightDown01sigma_branch does not exist!\n");
      exit(1);
    }
    ElectronWeightDown01sigma_isLoaded = true;
  }
  return ElectronWeightDown01sigma_;
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

const float &ttHLeptonic::nGoodMus() {
  if (not nGoodMus_isLoaded) {
    if (nGoodMus_branch != 0) {
      nGoodMus_branch->GetEntry(index);
    } else {
      printf("branch nGoodMus_branch does not exist!\n");
      exit(1);
    }
    nGoodMus_isLoaded = true;
  }
  return nGoodMus_;
}

const float &ttHLeptonic::LooseMvaSFUp01sigma() {
  if (not LooseMvaSFUp01sigma_isLoaded) {
    if (LooseMvaSFUp01sigma_branch != 0) {
      LooseMvaSFUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch LooseMvaSFUp01sigma_branch does not exist!\n");
      exit(1);
    }
    LooseMvaSFUp01sigma_isLoaded = true;
  }
  return LooseMvaSFUp01sigma_;
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

const float &ttHLeptonic::jet_energy14() {
  if (not jet_energy14_isLoaded) {
    if (jet_energy14_branch != 0) {
      jet_energy14_branch->GetEntry(index);
    } else {
      printf("branch jet_energy14_branch does not exist!\n");
      exit(1);
    }
    jet_energy14_isLoaded = true;
  }
  return jet_energy14_;
}

const float &ttHLeptonic::lead_MomMomID() {
  if (not lead_MomMomID_isLoaded) {
    if (lead_MomMomID_branch != 0) {
      lead_MomMomID_branch->GetEntry(index);
    } else {
      printf("branch lead_MomMomID_branch does not exist!\n");
      exit(1);
    }
    lead_MomMomID_isLoaded = true;
  }
  return lead_MomMomID_;
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

const float &ttHLeptonic::UnmatchedPUWeightDown01sigma() {
  if (not UnmatchedPUWeightDown01sigma_isLoaded) {
    if (UnmatchedPUWeightDown01sigma_branch != 0) {
      UnmatchedPUWeightDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch UnmatchedPUWeightDown01sigma_branch does not exist!\n");
      exit(1);
    }
    UnmatchedPUWeightDown01sigma_isLoaded = true;
  }
  return UnmatchedPUWeightDown01sigma_;
}

const float &ttHLeptonic::jet_bbdiscriminant10() {
  if (not jet_bbdiscriminant10_isLoaded) {
    if (jet_bbdiscriminant10_branch != 0) {
      jet_bbdiscriminant10_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant10_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant10_isLoaded = true;
  }
  return jet_bbdiscriminant10_;
}

const float &ttHLeptonic::rand() {
  if (not rand_isLoaded) {
    if (rand_branch != 0) {
      rand_branch->GetEntry(index);
    } else {
      printf("branch rand_branch does not exist!\n");
      exit(1);
    }
    rand_isLoaded = true;
  }
  return rand_;
}

const float &ttHLeptonic::jet_udsgdiscriminant4() {
  if (not jet_udsgdiscriminant4_isLoaded) {
    if (jet_udsgdiscriminant4_branch != 0) {
      jet_udsgdiscriminant4_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant4_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant4_isLoaded = true;
  }
  return jet_udsgdiscriminant4_;
}

const float &ttHLeptonic::jet_hadronFlavour4() {
  if (not jet_hadronFlavour4_isLoaded) {
    if (jet_hadronFlavour4_branch != 0) {
      jet_hadronFlavour4_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour4_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour4_isLoaded = true;
  }
  return jet_hadronFlavour4_;
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

const float &ttHLeptonic::jet_energy10() {
  if (not jet_energy10_isLoaded) {
    if (jet_energy10_branch != 0) {
      jet_energy10_branch->GetEntry(index);
    } else {
      printf("branch jet_energy10_branch does not exist!\n");
      exit(1);
    }
    jet_energy10_isLoaded = true;
  }
  return jet_energy10_;
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

const float &ttHLeptonic::sublead_MomMomID() {
  if (not sublead_MomMomID_isLoaded) {
    if (sublead_MomMomID_branch != 0) {
      sublead_MomMomID_branch->GetEntry(index);
    } else {
      printf("branch sublead_MomMomID_branch does not exist!\n");
      exit(1);
    }
    sublead_MomMomID_isLoaded = true;
  }
  return sublead_MomMomID_;
}

const float &ttHLeptonic::jet_udsgdiscriminant10() {
  if (not jet_udsgdiscriminant10_isLoaded) {
    if (jet_udsgdiscriminant10_branch != 0) {
      jet_udsgdiscriminant10_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant10_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant10_isLoaded = true;
  }
  return jet_udsgdiscriminant10_;
}

const float &ttHLeptonic::jet_hadronFlavour10() {
  if (not jet_hadronFlavour10_isLoaded) {
    if (jet_hadronFlavour10_branch != 0) {
      jet_hadronFlavour10_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour10_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour10_isLoaded = true;
  }
  return jet_hadronFlavour10_;
}

const float &ttHLeptonic::MvaLinearSystDown01sigma() {
  if (not MvaLinearSystDown01sigma_isLoaded) {
    if (MvaLinearSystDown01sigma_branch != 0) {
      MvaLinearSystDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch MvaLinearSystDown01sigma_branch does not exist!\n");
      exit(1);
    }
    MvaLinearSystDown01sigma_isLoaded = true;
  }
  return MvaLinearSystDown01sigma_;
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

const float &ttHLeptonic::electronVetoSFDown01sigma() {
  if (not electronVetoSFDown01sigma_isLoaded) {
    if (electronVetoSFDown01sigma_branch != 0) {
      electronVetoSFDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch electronVetoSFDown01sigma_branch does not exist!\n");
      exit(1);
    }
    electronVetoSFDown01sigma_isLoaded = true;
  }
  return electronVetoSFDown01sigma_;
}

const float &ttHLeptonic::jet_cdiscriminant8() {
  if (not jet_cdiscriminant8_isLoaded) {
    if (jet_cdiscriminant8_branch != 0) {
      jet_cdiscriminant8_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant8_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant8_isLoaded = true;
  }
  return jet_cdiscriminant8_;
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

const float &ttHLeptonic::JetBTagReshapeWeightUp01sigma() {
  if (not JetBTagReshapeWeightUp01sigma_isLoaded) {
    if (JetBTagReshapeWeightUp01sigma_branch != 0) {
      JetBTagReshapeWeightUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch JetBTagReshapeWeightUp01sigma_branch does not exist!\n");
      exit(1);
    }
    JetBTagReshapeWeightUp01sigma_isLoaded = true;
  }
  return JetBTagReshapeWeightUp01sigma_;
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

const float &ttHLeptonic::jet_energy3() {
  if (not jet_energy3_isLoaded) {
    if (jet_energy3_branch != 0) {
      jet_energy3_branch->GetEntry(index);
    } else {
      printf("branch jet_energy3_branch does not exist!\n");
      exit(1);
    }
    jet_energy3_isLoaded = true;
  }
  return jet_energy3_;
}

const float &ttHLeptonic::lead_Mad() {
  if (not lead_Mad_isLoaded) {
    if (lead_Mad_branch != 0) {
      lead_Mad_branch->GetEntry(index);
    } else {
      printf("branch lead_Mad_branch does not exist!\n");
      exit(1);
    }
    lead_Mad_isLoaded = true;
  }
  return lead_Mad_;
}

const float &ttHLeptonic::lead_PassFrix() {
  if (not lead_PassFrix_isLoaded) {
    if (lead_PassFrix_branch != 0) {
      lead_PassFrix_branch->GetEntry(index);
    } else {
      printf("branch lead_PassFrix_branch does not exist!\n");
      exit(1);
    }
    lead_PassFrix_isLoaded = true;
  }
  return lead_PassFrix_;
}

const float &ttHLeptonic::jet_cdiscriminant6() {
  if (not jet_cdiscriminant6_isLoaded) {
    if (jet_cdiscriminant6_branch != 0) {
      jet_cdiscriminant6_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant6_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant6_isLoaded = true;
  }
  return jet_cdiscriminant6_;
}

const float &ttHLeptonic::ele1_charge() {
  if (not ele1_charge_isLoaded) {
    if (ele1_charge_branch != 0) {
      ele1_charge_branch->GetEntry(index);
    } else {
      printf("branch ele1_charge_branch does not exist!\n");
      exit(1);
    }
    ele1_charge_isLoaded = true;
  }
  return ele1_charge_;
}

const float &ttHLeptonic::jet_bbdiscriminant11() {
  if (not jet_bbdiscriminant11_isLoaded) {
    if (jet_bbdiscriminant11_branch != 0) {
      jet_bbdiscriminant11_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant11_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant11_isLoaded = true;
  }
  return jet_bbdiscriminant11_;
}

const float &ttHLeptonic::PreselSFUp01sigma() {
  if (not PreselSFUp01sigma_isLoaded) {
    if (PreselSFUp01sigma_branch != 0) {
      PreselSFUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch PreselSFUp01sigma_branch does not exist!\n");
      exit(1);
    }
    PreselSFUp01sigma_isLoaded = true;
  }
  return PreselSFUp01sigma_;
}

const float &ttHLeptonic::jet_cdiscriminant2() {
  if (not jet_cdiscriminant2_isLoaded) {
    if (jet_cdiscriminant2_branch != 0) {
      jet_cdiscriminant2_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant2_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant2_isLoaded = true;
  }
  return jet_cdiscriminant2_;
}

const float &ttHLeptonic::weight_JetBTagWeight() {
  if (not weight_JetBTagWeight_isLoaded) {
    if (weight_JetBTagWeight_branch != 0) {
      weight_JetBTagWeight_branch->GetEntry(index);
    } else {
      printf("branch weight_JetBTagWeight_branch does not exist!\n");
      exit(1);
    }
    weight_JetBTagWeight_isLoaded = true;
  }
  return weight_JetBTagWeight_;
}

const float &ttHLeptonic::jet_udsgdiscriminant6() {
  if (not jet_udsgdiscriminant6_isLoaded) {
    if (jet_udsgdiscriminant6_branch != 0) {
      jet_udsgdiscriminant6_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant6_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant6_isLoaded = true;
  }
  return jet_udsgdiscriminant6_;
}

const float &ttHLeptonic::jet_hadronFlavour6() {
  if (not jet_hadronFlavour6_isLoaded) {
    if (jet_hadronFlavour6_branch != 0) {
      jet_hadronFlavour6_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour6_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour6_isLoaded = true;
  }
  return jet_hadronFlavour6_;
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

const float &ttHLeptonic::muonSubleadIso() {
  if (not muonSubleadIso_isLoaded) {
    if (muonSubleadIso_branch != 0) {
      muonSubleadIso_branch->GetEntry(index);
    } else {
      printf("branch muonSubleadIso_branch does not exist!\n");
      exit(1);
    }
    muonSubleadIso_isLoaded = true;
  }
  return muonSubleadIso_;
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

const float &ttHLeptonic::sublead_MomID() {
  if (not sublead_MomID_isLoaded) {
    if (sublead_MomID_branch != 0) {
      sublead_MomID_branch->GetEntry(index);
    } else {
      printf("branch sublead_MomID_branch does not exist!\n");
      exit(1);
    }
    sublead_MomID_isLoaded = true;
  }
  return sublead_MomID_;
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

const float &ttHLeptonic::FracRVWeightUp01sigma() {
  if (not FracRVWeightUp01sigma_isLoaded) {
    if (FracRVWeightUp01sigma_branch != 0) {
      FracRVWeightUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch FracRVWeightUp01sigma_branch does not exist!\n");
      exit(1);
    }
    FracRVWeightUp01sigma_isLoaded = true;
  }
  return FracRVWeightUp01sigma_;
}

const float &ttHLeptonic::subleadGenMatch() {
  if (not subleadGenMatch_isLoaded) {
    if (subleadGenMatch_branch != 0) {
      subleadGenMatch_branch->GetEntry(index);
    } else {
      printf("branch subleadGenMatch_branch does not exist!\n");
      exit(1);
    }
    subleadGenMatch_isLoaded = true;
  }
  return subleadGenMatch_;
}

const float &ttHLeptonic::lead_PhoGenPhi() {
  if (not lead_PhoGenPhi_isLoaded) {
    if (lead_PhoGenPhi_branch != 0) {
      lead_PhoGenPhi_branch->GetEntry(index);
    } else {
      printf("branch lead_PhoGenPhi_branch does not exist!\n");
      exit(1);
    }
    lead_PhoGenPhi_isLoaded = true;
  }
  return lead_PhoGenPhi_;
}

const float &ttHLeptonic::nElecLoose() {
  if (not nElecLoose_isLoaded) {
    if (nElecLoose_branch != 0) {
      nElecLoose_branch->GetEntry(index);
    } else {
      printf("branch nElecLoose_branch does not exist!\n");
      exit(1);
    }
    nElecLoose_isLoaded = true;
  }
  return nElecLoose_;
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

const float &ttHLeptonic::jet_udsgdiscriminant9() {
  if (not jet_udsgdiscriminant9_isLoaded) {
    if (jet_udsgdiscriminant9_branch != 0) {
      jet_udsgdiscriminant9_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant9_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant9_isLoaded = true;
  }
  return jet_udsgdiscriminant9_;
}

const float &ttHLeptonic::jet_hadronFlavour9() {
  if (not jet_hadronFlavour9_isLoaded) {
    if (jet_hadronFlavour9_branch != 0) {
      jet_hadronFlavour9_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour9_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour9_isLoaded = true;
  }
  return jet_hadronFlavour9_;
}

const float &ttHLeptonic::electronVetoSFUp01sigma() {
  if (not electronVetoSFUp01sigma_isLoaded) {
    if (electronVetoSFUp01sigma_branch != 0) {
      electronVetoSFUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch electronVetoSFUp01sigma_branch does not exist!\n");
      exit(1);
    }
    electronVetoSFUp01sigma_isLoaded = true;
  }
  return electronVetoSFUp01sigma_;
}

const float &ttHLeptonic::subleadGendeltaR() {
  if (not subleadGendeltaR_isLoaded) {
    if (subleadGendeltaR_branch != 0) {
      subleadGendeltaR_branch->GetEntry(index);
    } else {
      printf("branch subleadGendeltaR_branch does not exist!\n");
      exit(1);
    }
    subleadGendeltaR_isLoaded = true;
  }
  return subleadGendeltaR_;
}

const float &ttHLeptonic::muon2_energy() {
  if (not muon2_energy_isLoaded) {
    if (muon2_energy_branch != 0) {
      muon2_energy_branch->GetEntry(index);
    } else {
      printf("branch muon2_energy_branch does not exist!\n");
      exit(1);
    }
    muon2_energy_isLoaded = true;
  }
  return muon2_energy_;
}

const float &ttHLeptonic::sublead_PhoGenEta() {
  if (not sublead_PhoGenEta_isLoaded) {
    if (sublead_PhoGenEta_branch != 0) {
      sublead_PhoGenEta_branch->GetEntry(index);
    } else {
      printf("branch sublead_PhoGenEta_branch does not exist!\n");
      exit(1);
    }
    sublead_PhoGenEta_isLoaded = true;
  }
  return sublead_PhoGenEta_;
}

const float &ttHLeptonic::jet_udsgdiscriminant15() {
  if (not jet_udsgdiscriminant15_isLoaded) {
    if (jet_udsgdiscriminant15_branch != 0) {
      jet_udsgdiscriminant15_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant15_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant15_isLoaded = true;
  }
  return jet_udsgdiscriminant15_;
}

const float &ttHLeptonic::jet_hadronFlavour15() {
  if (not jet_hadronFlavour15_isLoaded) {
    if (jet_hadronFlavour15_branch != 0) {
      jet_hadronFlavour15_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour15_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour15_isLoaded = true;
  }
  return jet_hadronFlavour15_;
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

const float &ttHLeptonic::ElectronWeightUp01sigma() {
  if (not ElectronWeightUp01sigma_isLoaded) {
    if (ElectronWeightUp01sigma_branch != 0) {
      ElectronWeightUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch ElectronWeightUp01sigma_branch does not exist!\n");
      exit(1);
    }
    ElectronWeightUp01sigma_isLoaded = true;
  }
  return ElectronWeightUp01sigma_;
}

const float &ttHLeptonic::jet_udsgdiscriminant3() {
  if (not jet_udsgdiscriminant3_isLoaded) {
    if (jet_udsgdiscriminant3_branch != 0) {
      jet_udsgdiscriminant3_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant3_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant3_isLoaded = true;
  }
  return jet_udsgdiscriminant3_;
}

const float &ttHLeptonic::jet_hadronFlavour3() {
  if (not jet_hadronFlavour3_isLoaded) {
    if (jet_hadronFlavour3_branch != 0) {
      jet_hadronFlavour3_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour3_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour3_isLoaded = true;
  }
  return jet_hadronFlavour3_;
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

const float &ttHLeptonic::MetPt() {
  if (not MetPt_isLoaded) {
    if (MetPt_branch != 0) {
      MetPt_branch->GetEntry(index);
    } else {
      printf("branch MetPt_branch does not exist!\n");
      exit(1);
    }
    MetPt_isLoaded = true;
  }
  return MetPt_;
}

const float &ttHLeptonic::jet_cdiscriminant7() {
  if (not jet_cdiscriminant7_isLoaded) {
    if (jet_cdiscriminant7_branch != 0) {
      jet_cdiscriminant7_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant7_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant7_isLoaded = true;
  }
  return jet_cdiscriminant7_;
}

const float &ttHLeptonic::jet_udsgdiscriminant14() {
  if (not jet_udsgdiscriminant14_isLoaded) {
    if (jet_udsgdiscriminant14_branch != 0) {
      jet_udsgdiscriminant14_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant14_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant14_isLoaded = true;
  }
  return jet_udsgdiscriminant14_;
}

const float &ttHLeptonic::jet_hadronFlavour14() {
  if (not jet_hadronFlavour14_isLoaded) {
    if (jet_hadronFlavour14_branch != 0) {
      jet_hadronFlavour14_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour14_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour14_isLoaded = true;
  }
  return jet_hadronFlavour14_;
}

const float &ttHLeptonic::jet_bbdiscriminant14() {
  if (not jet_bbdiscriminant14_isLoaded) {
    if (jet_bbdiscriminant14_branch != 0) {
      jet_bbdiscriminant14_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant14_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant14_isLoaded = true;
  }
  return jet_bbdiscriminant14_;
}

const float &ttHLeptonic::muon1_energy() {
  if (not muon1_energy_isLoaded) {
    if (muon1_energy_branch != 0) {
      muon1_energy_branch->GetEntry(index);
    } else {
      printf("branch muon1_energy_branch does not exist!\n");
      exit(1);
    }
    muon1_energy_isLoaded = true;
  }
  return muon1_energy_;
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

const float &ttHLeptonic::jet_bbdiscriminant7() {
  if (not jet_bbdiscriminant7_isLoaded) {
    if (jet_bbdiscriminant7_branch != 0) {
      jet_bbdiscriminant7_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant7_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant7_isLoaded = true;
  }
  return jet_bbdiscriminant7_;
}

const float &ttHLeptonic::jet_bbdiscriminant9() {
  if (not jet_bbdiscriminant9_isLoaded) {
    if (jet_bbdiscriminant9_branch != 0) {
      jet_bbdiscriminant9_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant9_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant9_isLoaded = true;
  }
  return jet_bbdiscriminant9_;
}

const float &ttHLeptonic::muon2_charge() {
  if (not muon2_charge_isLoaded) {
    if (muon2_charge_branch != 0) {
      muon2_charge_branch->GetEntry(index);
    } else {
      printf("branch muon2_charge_branch does not exist!\n");
      exit(1);
    }
    muon2_charge_isLoaded = true;
  }
  return muon2_charge_;
}

const float &ttHLeptonic::FracRVNvtxWeightUp01sigma() {
  if (not FracRVNvtxWeightUp01sigma_isLoaded) {
    if (FracRVNvtxWeightUp01sigma_branch != 0) {
      FracRVNvtxWeightUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch FracRVNvtxWeightUp01sigma_branch does not exist!\n");
      exit(1);
    }
    FracRVNvtxWeightUp01sigma_isLoaded = true;
  }
  return FracRVNvtxWeightUp01sigma_;
}

const float &ttHLeptonic::ele1_energy() {
  if (not ele1_energy_isLoaded) {
    if (ele1_energy_branch != 0) {
      ele1_energy_branch->GetEntry(index);
    } else {
      printf("branch ele1_energy_branch does not exist!\n");
      exit(1);
    }
    ele1_energy_isLoaded = true;
  }
  return ele1_energy_;
}

const float &ttHLeptonic::nMuonMedium() {
  if (not nMuonMedium_isLoaded) {
    if (nMuonMedium_branch != 0) {
      nMuonMedium_branch->GetEntry(index);
    } else {
      printf("branch nMuonMedium_branch does not exist!\n");
      exit(1);
    }
    nMuonMedium_isLoaded = true;
  }
  return nMuonMedium_;
}

const float &ttHLeptonic::jet_cdiscriminant14() {
  if (not jet_cdiscriminant14_isLoaded) {
    if (jet_cdiscriminant14_branch != 0) {
      jet_cdiscriminant14_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant14_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant14_isLoaded = true;
  }
  return jet_cdiscriminant14_;
}

const float &ttHLeptonic::nMuonTight() {
  if (not nMuonTight_isLoaded) {
    if (nMuonTight_branch != 0) {
      nMuonTight_branch->GetEntry(index);
    } else {
      printf("branch nMuonTight_branch does not exist!\n");
      exit(1);
    }
    nMuonTight_isLoaded = true;
  }
  return nMuonTight_;
}

const float &ttHLeptonic::jet_energy12() {
  if (not jet_energy12_isLoaded) {
    if (jet_energy12_branch != 0) {
      jet_energy12_branch->GetEntry(index);
    } else {
      printf("branch jet_energy12_branch does not exist!\n");
      exit(1);
    }
    jet_energy12_isLoaded = true;
  }
  return jet_energy12_;
}

const float &ttHLeptonic::nGoodTaus() {
  if (not nGoodTaus_isLoaded) {
    if (nGoodTaus_branch != 0) {
      nGoodTaus_branch->GetEntry(index);
    } else {
      printf("branch nGoodTaus_branch does not exist!\n");
      exit(1);
    }
    nGoodTaus_isLoaded = true;
  }
  return nGoodTaus_;
}

const float &ttHLeptonic::jet_bbdiscriminant12() {
  if (not jet_bbdiscriminant12_isLoaded) {
    if (jet_bbdiscriminant12_branch != 0) {
      jet_bbdiscriminant12_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant12_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant12_isLoaded = true;
  }
  return jet_bbdiscriminant12_;
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

const float &ttHLeptonic::muon1_charge() {
  if (not muon1_charge_isLoaded) {
    if (muon1_charge_branch != 0) {
      muon1_charge_branch->GetEntry(index);
    } else {
      printf("branch muon1_charge_branch does not exist!\n");
      exit(1);
    }
    muon1_charge_isLoaded = true;
  }
  return muon1_charge_;
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

const float &ttHLeptonic::lead_closest_gen_dR() {
  if (not lead_closest_gen_dR_isLoaded) {
    if (lead_closest_gen_dR_branch != 0) {
      lead_closest_gen_dR_branch->GetEntry(index);
    } else {
      printf("branch lead_closest_gen_dR_branch does not exist!\n");
      exit(1);
    }
    lead_closest_gen_dR_isLoaded = true;
  }
  return lead_closest_gen_dR_;
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

const float &ttHLeptonic::jet_udsgdiscriminant5() {
  if (not jet_udsgdiscriminant5_isLoaded) {
    if (jet_udsgdiscriminant5_branch != 0) {
      jet_udsgdiscriminant5_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant5_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant5_isLoaded = true;
  }
  return jet_udsgdiscriminant5_;
}

const float &ttHLeptonic::jet_hadronFlavour5() {
  if (not jet_hadronFlavour5_isLoaded) {
    if (jet_hadronFlavour5_branch != 0) {
      jet_hadronFlavour5_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour5_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour5_isLoaded = true;
  }
  return jet_hadronFlavour5_;
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

const float &ttHLeptonic::jet_cdiscriminant11() {
  if (not jet_cdiscriminant11_isLoaded) {
    if (jet_cdiscriminant11_branch != 0) {
      jet_cdiscriminant11_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant11_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant11_isLoaded = true;
  }
  return jet_cdiscriminant11_;
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

const float &ttHLeptonic::lead_closest_gen_Pt() {
  if (not lead_closest_gen_Pt_isLoaded) {
    if (lead_closest_gen_Pt_branch != 0) {
      lead_closest_gen_Pt_branch->GetEntry(index);
    } else {
      printf("branch lead_closest_gen_Pt_branch does not exist!\n");
      exit(1);
    }
    lead_closest_gen_Pt_isLoaded = true;
  }
  return lead_closest_gen_Pt_;
}

const float &ttHLeptonic::UnmatchedPUWeightUp01sigma() {
  if (not UnmatchedPUWeightUp01sigma_isLoaded) {
    if (UnmatchedPUWeightUp01sigma_branch != 0) {
      UnmatchedPUWeightUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch UnmatchedPUWeightUp01sigma_branch does not exist!\n");
      exit(1);
    }
    UnmatchedPUWeightUp01sigma_isLoaded = true;
  }
  return UnmatchedPUWeightUp01sigma_;
}

const float &ttHLeptonic::lead_SimpleMomID() {
  if (not lead_SimpleMomID_isLoaded) {
    if (lead_SimpleMomID_branch != 0) {
      lead_SimpleMomID_branch->GetEntry(index);
    } else {
      printf("branch lead_SimpleMomID_branch does not exist!\n");
      exit(1);
    }
    lead_SimpleMomID_isLoaded = true;
  }
  return lead_SimpleMomID_;
}

const float &ttHLeptonic::nElecTight() {
  if (not nElecTight_isLoaded) {
    if (nElecTight_branch != 0) {
      nElecTight_branch->GetEntry(index);
    } else {
      printf("branch nElecTight_branch does not exist!\n");
      exit(1);
    }
    nElecTight_isLoaded = true;
  }
  return nElecTight_;
}

const float &ttHLeptonic::ele1_eta() {
  if (not ele1_eta_isLoaded) {
    if (ele1_eta_branch != 0) {
      ele1_eta_branch->GetEntry(index);
    } else {
      printf("branch ele1_eta_branch does not exist!\n");
      exit(1);
    }
    ele1_eta_isLoaded = true;
  }
  return ele1_eta_;
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

const float &ttHLeptonic::jet_bbdiscriminant13() {
  if (not jet_bbdiscriminant13_isLoaded) {
    if (jet_bbdiscriminant13_branch != 0) {
      jet_bbdiscriminant13_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant13_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant13_isLoaded = true;
  }
  return jet_bbdiscriminant13_;
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

const float &ttHLeptonic::ele2_energy() {
  if (not ele2_energy_isLoaded) {
    if (ele2_energy_branch != 0) {
      ele2_energy_branch->GetEntry(index);
    } else {
      printf("branch ele2_energy_branch does not exist!\n");
      exit(1);
    }
    ele2_energy_isLoaded = true;
  }
  return ele2_energy_;
}

const float &ttHLeptonic::nb_medium() {
  if (not nb_medium_isLoaded) {
    if (nb_medium_branch != 0) {
      nb_medium_branch->GetEntry(index);
    } else {
      printf("branch nb_medium_branch does not exist!\n");
      exit(1);
    }
    nb_medium_isLoaded = true;
  }
  return nb_medium_;
}

const float &ttHLeptonic::JetBTagCutWeightDown01sigma() {
  if (not JetBTagCutWeightDown01sigma_isLoaded) {
    if (JetBTagCutWeightDown01sigma_branch != 0) {
      JetBTagCutWeightDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch JetBTagCutWeightDown01sigma_branch does not exist!\n");
      exit(1);
    }
    JetBTagCutWeightDown01sigma_isLoaded = true;
  }
  return JetBTagCutWeightDown01sigma_;
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

const float &ttHLeptonic::jet_udsgdiscriminant1() {
  if (not jet_udsgdiscriminant1_isLoaded) {
    if (jet_udsgdiscriminant1_branch != 0) {
      jet_udsgdiscriminant1_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant1_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant1_isLoaded = true;
  }
  return jet_udsgdiscriminant1_;
}

const float &ttHLeptonic::jet_hadronFlavour1() {
  if (not jet_hadronFlavour1_isLoaded) {
    if (jet_hadronFlavour1_branch != 0) {
      jet_hadronFlavour1_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour1_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour1_isLoaded = true;
  }
  return jet_hadronFlavour1_;
}

const float &ttHLeptonic::muonLeadIso() {
  if (not muonLeadIso_isLoaded) {
    if (muonLeadIso_branch != 0) {
      muonLeadIso_branch->GetEntry(index);
    } else {
      printf("branch muonLeadIso_branch does not exist!\n");
      exit(1);
    }
    muonLeadIso_isLoaded = true;
  }
  return muonLeadIso_;
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

const float &ttHLeptonic::jet_cdiscriminant13() {
  if (not jet_cdiscriminant13_isLoaded) {
    if (jet_cdiscriminant13_branch != 0) {
      jet_cdiscriminant13_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant13_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant13_isLoaded = true;
  }
  return jet_cdiscriminant13_;
}

const float &ttHLeptonic::subleadPixelSeed() {
  if (not subleadPixelSeed_isLoaded) {
    if (subleadPixelSeed_branch != 0) {
      subleadPixelSeed_branch->GetEntry(index);
    } else {
      printf("branch subleadPixelSeed_branch does not exist!\n");
      exit(1);
    }
    subleadPixelSeed_isLoaded = true;
  }
  return subleadPixelSeed_;
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

const float &ttHLeptonic::lead_SimpleMomStatus() {
  if (not lead_SimpleMomStatus_isLoaded) {
    if (lead_SimpleMomStatus_branch != 0) {
      lead_SimpleMomStatus_branch->GetEntry(index);
    } else {
      printf("branch lead_SimpleMomStatus_branch does not exist!\n");
      exit(1);
    }
    lead_SimpleMomStatus_isLoaded = true;
  }
  return lead_SimpleMomStatus_;
}

const float &ttHLeptonic::jet_energy6() {
  if (not jet_energy6_isLoaded) {
    if (jet_energy6_branch != 0) {
      jet_energy6_branch->GetEntry(index);
    } else {
      printf("branch jet_energy6_branch does not exist!\n");
      exit(1);
    }
    jet_energy6_isLoaded = true;
  }
  return jet_energy6_;
}

const float &ttHLeptonic::sublead_SmallestDr() {
  if (not sublead_SmallestDr_isLoaded) {
    if (sublead_SmallestDr_branch != 0) {
      sublead_SmallestDr_branch->GetEntry(index);
    } else {
      printf("branch sublead_SmallestDr_branch does not exist!\n");
      exit(1);
    }
    sublead_SmallestDr_isLoaded = true;
  }
  return sublead_SmallestDr_;
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

const float &ttHLeptonic::subleadEnergy() {
  if (not subleadEnergy_isLoaded) {
    if (subleadEnergy_branch != 0) {
      subleadEnergy_branch->GetEntry(index);
    } else {
      printf("branch subleadEnergy_branch does not exist!\n");
      exit(1);
    }
    subleadEnergy_isLoaded = true;
  }
  return subleadEnergy_;
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

const float &ttHLeptonic::jet_cdiscriminant10() {
  if (not jet_cdiscriminant10_isLoaded) {
    if (jet_cdiscriminant10_branch != 0) {
      jet_cdiscriminant10_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant10_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant10_isLoaded = true;
  }
  return jet_cdiscriminant10_;
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

const float &ttHLeptonic::jet_bbdiscriminant6() {
  if (not jet_bbdiscriminant6_isLoaded) {
    if (jet_bbdiscriminant6_branch != 0) {
      jet_bbdiscriminant6_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant6_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant6_isLoaded = true;
  }
  return jet_bbdiscriminant6_;
}

const float &ttHLeptonic::jet_energy15() {
  if (not jet_energy15_isLoaded) {
    if (jet_energy15_branch != 0) {
      jet_energy15_branch->GetEntry(index);
    } else {
      printf("branch jet_energy15_branch does not exist!\n");
      exit(1);
    }
    jet_energy15_isLoaded = true;
  }
  return jet_energy15_;
}

const float &ttHLeptonic::leadPixelSeed() {
  if (not leadPixelSeed_isLoaded) {
    if (leadPixelSeed_branch != 0) {
      leadPixelSeed_branch->GetEntry(index);
    } else {
      printf("branch leadPixelSeed_branch does not exist!\n");
      exit(1);
    }
    leadPixelSeed_isLoaded = true;
  }
  return leadPixelSeed_;
}

const float &ttHLeptonic::vertex_idx() {
  if (not vertex_idx_isLoaded) {
    if (vertex_idx_branch != 0) {
      vertex_idx_branch->GetEntry(index);
    } else {
      printf("branch vertex_idx_branch does not exist!\n");
      exit(1);
    }
    vertex_idx_isLoaded = true;
  }
  return vertex_idx_;
}

const float &ttHLeptonic::jet_bbdiscriminant1() {
  if (not jet_bbdiscriminant1_isLoaded) {
    if (jet_bbdiscriminant1_branch != 0) {
      jet_bbdiscriminant1_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant1_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant1_isLoaded = true;
  }
  return jet_bbdiscriminant1_;
}

const float &ttHLeptonic::sublead_PhoGenPt() {
  if (not sublead_PhoGenPt_isLoaded) {
    if (sublead_PhoGenPt_branch != 0) {
      sublead_PhoGenPt_branch->GetEntry(index);
    } else {
      printf("branch sublead_PhoGenPt_branch does not exist!\n");
      exit(1);
    }
    sublead_PhoGenPt_isLoaded = true;
  }
  return sublead_PhoGenPt_;
}

const float &ttHLeptonic::jet_energy4() {
  if (not jet_energy4_isLoaded) {
    if (jet_energy4_branch != 0) {
      jet_energy4_branch->GetEntry(index);
    } else {
      printf("branch jet_energy4_branch does not exist!\n");
      exit(1);
    }
    jet_energy4_isLoaded = true;
  }
  return jet_energy4_;
}

const float &ttHLeptonic::jet_energy7() {
  if (not jet_energy7_isLoaded) {
    if (jet_energy7_branch != 0) {
      jet_energy7_branch->GetEntry(index);
    } else {
      printf("branch jet_energy7_branch does not exist!\n");
      exit(1);
    }
    jet_energy7_isLoaded = true;
  }
  return jet_energy7_;
}

const float &ttHLeptonic::JetBTagCutWeightUp01sigma() {
  if (not JetBTagCutWeightUp01sigma_isLoaded) {
    if (JetBTagCutWeightUp01sigma_branch != 0) {
      JetBTagCutWeightUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch JetBTagCutWeightUp01sigma_branch does not exist!\n");
      exit(1);
    }
    JetBTagCutWeightUp01sigma_isLoaded = true;
  }
  return JetBTagCutWeightUp01sigma_;
}

const float &ttHLeptonic::MvaLinearSystUp01sigma() {
  if (not MvaLinearSystUp01sigma_isLoaded) {
    if (MvaLinearSystUp01sigma_branch != 0) {
      MvaLinearSystUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch MvaLinearSystUp01sigma_branch does not exist!\n");
      exit(1);
    }
    MvaLinearSystUp01sigma_isLoaded = true;
  }
  return MvaLinearSystUp01sigma_;
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

const float &ttHLeptonic::jet_energy2() {
  if (not jet_energy2_isLoaded) {
    if (jet_energy2_branch != 0) {
      jet_energy2_branch->GetEntry(index);
    } else {
      printf("branch jet_energy2_branch does not exist!\n");
      exit(1);
    }
    jet_energy2_isLoaded = true;
  }
  return jet_energy2_;
}

const float &ttHLeptonic::nb_tight() {
  if (not nb_tight_isLoaded) {
    if (nb_tight_branch != 0) {
      nb_tight_branch->GetEntry(index);
    } else {
      printf("branch nb_tight_branch does not exist!\n");
      exit(1);
    }
    nb_tight_isLoaded = true;
  }
  return nb_tight_;
}

const float &ttHLeptonic::jet_bbdiscriminant5() {
  if (not jet_bbdiscriminant5_isLoaded) {
    if (jet_bbdiscriminant5_branch != 0) {
      jet_bbdiscriminant5_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant5_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant5_isLoaded = true;
  }
  return jet_bbdiscriminant5_;
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

const float &ttHLeptonic::jet_bbdiscriminant15() {
  if (not jet_bbdiscriminant15_isLoaded) {
    if (jet_bbdiscriminant15_branch != 0) {
      jet_bbdiscriminant15_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant15_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant15_isLoaded = true;
  }
  return jet_bbdiscriminant15_;
}

const float &ttHLeptonic::subleadPtGen() {
  if (not subleadPtGen_isLoaded) {
    if (subleadPtGen_branch != 0) {
      subleadPtGen_branch->GetEntry(index);
    } else {
      printf("branch subleadPtGen_branch does not exist!\n");
      exit(1);
    }
    subleadPtGen_isLoaded = true;
  }
  return subleadPtGen_;
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

const float &ttHLeptonic::lead_photon_type() {
  if (not lead_photon_type_isLoaded) {
    if (lead_photon_type_branch != 0) {
      lead_photon_type_branch->GetEntry(index);
    } else {
      printf("branch lead_photon_type_branch does not exist!\n");
      exit(1);
    }
    lead_photon_type_isLoaded = true;
  }
  return lead_photon_type_;
}

const float &ttHLeptonic::FracRVNvtxWeightDown01sigma() {
  if (not FracRVNvtxWeightDown01sigma_isLoaded) {
    if (FracRVNvtxWeightDown01sigma_branch != 0) {
      FracRVNvtxWeightDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch FracRVNvtxWeightDown01sigma_branch does not exist!\n");
      exit(1);
    }
    FracRVNvtxWeightDown01sigma_isLoaded = true;
  }
  return FracRVNvtxWeightDown01sigma_;
}

const float &ttHLeptonic::PreselSFDown01sigma() {
  if (not PreselSFDown01sigma_isLoaded) {
    if (PreselSFDown01sigma_branch != 0) {
      PreselSFDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch PreselSFDown01sigma_branch does not exist!\n");
      exit(1);
    }
    PreselSFDown01sigma_isLoaded = true;
  }
  return PreselSFDown01sigma_;
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

const float &ttHLeptonic::muon1_phi() {
  if (not muon1_phi_isLoaded) {
    if (muon1_phi_branch != 0) {
      muon1_phi_branch->GetEntry(index);
    } else {
      printf("branch muon1_phi_branch does not exist!\n");
      exit(1);
    }
    muon1_phi_isLoaded = true;
  }
  return muon1_phi_;
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

const float &ttHLeptonic::subleadPassEVeto() {
  if (not subleadPassEVeto_isLoaded) {
    if (subleadPassEVeto_branch != 0) {
      subleadPassEVeto_branch->GetEntry(index);
    } else {
      printf("branch subleadPassEVeto_branch does not exist!\n");
      exit(1);
    }
    subleadPassEVeto_isLoaded = true;
  }
  return subleadPassEVeto_;
}

const float &ttHLeptonic::jet_energy1() {
  if (not jet_energy1_isLoaded) {
    if (jet_energy1_branch != 0) {
      jet_energy1_branch->GetEntry(index);
    } else {
      printf("branch jet_energy1_branch does not exist!\n");
      exit(1);
    }
    jet_energy1_isLoaded = true;
  }
  return jet_energy1_;
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

const float &ttHLeptonic::topTag_score() {
  if (not topTag_score_isLoaded) {
    if (topTag_score_branch != 0) {
      topTag_score_branch->GetEntry(index);
    } else {
      printf("branch topTag_score_branch does not exist!\n");
      exit(1);
    }
    topTag_score_isLoaded = true;
  }
  return topTag_score_;
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

const float &ttHLeptonic::CMS_hgg_mass() {
  if (not CMS_hgg_mass_isLoaded) {
    if (CMS_hgg_mass_branch != 0) {
      CMS_hgg_mass_branch->GetEntry(index);
    } else {
      printf("branch CMS_hgg_mass_branch does not exist!\n");
      exit(1);
    }
    CMS_hgg_mass_isLoaded = true;
  }
  return CMS_hgg_mass_;
}

const float &ttHLeptonic::centralObjectWeight() {
  if (not centralObjectWeight_isLoaded) {
    if (centralObjectWeight_branch != 0) {
      centralObjectWeight_branch->GetEntry(index);
    } else {
      printf("branch centralObjectWeight_branch does not exist!\n");
      exit(1);
    }
    centralObjectWeight_isLoaded = true;
  }
  return centralObjectWeight_;
}

const float &ttHLeptonic::jet_bbdiscriminant2() {
  if (not jet_bbdiscriminant2_isLoaded) {
    if (jet_bbdiscriminant2_branch != 0) {
      jet_bbdiscriminant2_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant2_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant2_isLoaded = true;
  }
  return jet_bbdiscriminant2_;
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

const float &ttHLeptonic::nMuonLoose() {
  if (not nMuonLoose_isLoaded) {
    if (nMuonLoose_branch != 0) {
      nMuonLoose_branch->GetEntry(index);
    } else {
      printf("branch nMuonLoose_branch does not exist!\n");
      exit(1);
    }
    nMuonLoose_isLoaded = true;
  }
  return nMuonLoose_;
}

const float &ttHLeptonic::jet_bbdiscriminant3() {
  if (not jet_bbdiscriminant3_isLoaded) {
    if (jet_bbdiscriminant3_branch != 0) {
      jet_bbdiscriminant3_branch->GetEntry(index);
    } else {
      printf("branch jet_bbdiscriminant3_branch does not exist!\n");
      exit(1);
    }
    jet_bbdiscriminant3_isLoaded = true;
  }
  return jet_bbdiscriminant3_;
}

const float &ttHLeptonic::FracRVWeightDown01sigma() {
  if (not FracRVWeightDown01sigma_isLoaded) {
    if (FracRVWeightDown01sigma_branch != 0) {
      FracRVWeightDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch FracRVWeightDown01sigma_branch does not exist!\n");
      exit(1);
    }
    FracRVWeightDown01sigma_isLoaded = true;
  }
  return FracRVWeightDown01sigma_;
}

const float &ttHLeptonic::jet_energy9() {
  if (not jet_energy9_isLoaded) {
    if (jet_energy9_branch != 0) {
      jet_energy9_branch->GetEntry(index);
    } else {
      printf("branch jet_energy9_branch does not exist!\n");
      exit(1);
    }
    jet_energy9_isLoaded = true;
  }
  return jet_energy9_;
}

const float &ttHLeptonic::sublead_PassFrix() {
  if (not sublead_PassFrix_isLoaded) {
    if (sublead_PassFrix_branch != 0) {
      sublead_PassFrix_branch->GetEntry(index);
    } else {
      printf("branch sublead_PassFrix_branch does not exist!\n");
      exit(1);
    }
    sublead_PassFrix_isLoaded = true;
  }
  return sublead_PassFrix_;
}

const float &ttHLeptonic::ele1_phi() {
  if (not ele1_phi_isLoaded) {
    if (ele1_phi_branch != 0) {
      ele1_phi_branch->GetEntry(index);
    } else {
      printf("branch ele1_phi_branch does not exist!\n");
      exit(1);
    }
    ele1_phi_isLoaded = true;
  }
  return ele1_phi_;
}

const float &ttHLeptonic::ele2_charge() {
  if (not ele2_charge_isLoaded) {
    if (ele2_charge_branch != 0) {
      ele2_charge_branch->GetEntry(index);
    } else {
      printf("branch ele2_charge_branch does not exist!\n");
      exit(1);
    }
    ele2_charge_isLoaded = true;
  }
  return ele2_charge_;
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

const float &ttHLeptonic::jet_energy13() {
  if (not jet_energy13_isLoaded) {
    if (jet_energy13_branch != 0) {
      jet_energy13_branch->GetEntry(index);
    } else {
      printf("branch jet_energy13_branch does not exist!\n");
      exit(1);
    }
    jet_energy13_isLoaded = true;
  }
  return jet_energy13_;
}

const float &ttHLeptonic::muon2_phi() {
  if (not muon2_phi_isLoaded) {
    if (muon2_phi_branch != 0) {
      muon2_phi_branch->GetEntry(index);
    } else {
      printf("branch muon2_phi_branch does not exist!\n");
      exit(1);
    }
    muon2_phi_isLoaded = true;
  }
  return muon2_phi_;
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

const float &ttHLeptonic::topTag_WMass() {
  if (not topTag_WMass_isLoaded) {
    if (topTag_WMass_branch != 0) {
      topTag_WMass_branch->GetEntry(index);
    } else {
      printf("branch topTag_WMass_branch does not exist!\n");
      exit(1);
    }
    topTag_WMass_isLoaded = true;
  }
  return topTag_WMass_;
}

const float &ttHLeptonic::jet_cdiscriminant15() {
  if (not jet_cdiscriminant15_isLoaded) {
    if (jet_cdiscriminant15_branch != 0) {
      jet_cdiscriminant15_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant15_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant15_isLoaded = true;
  }
  return jet_cdiscriminant15_;
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

const float &ttHLeptonic::lead_PhoGenEta() {
  if (not lead_PhoGenEta_isLoaded) {
    if (lead_PhoGenEta_branch != 0) {
      lead_PhoGenEta_branch->GetEntry(index);
    } else {
      printf("branch lead_PhoGenEta_branch does not exist!\n");
      exit(1);
    }
    lead_PhoGenEta_isLoaded = true;
  }
  return lead_PhoGenEta_;
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

const float &ttHLeptonic::leadEnergy() {
  if (not leadEnergy_isLoaded) {
    if (leadEnergy_branch != 0) {
      leadEnergy_branch->GetEntry(index);
    } else {
      printf("branch leadEnergy_branch does not exist!\n");
      exit(1);
    }
    leadEnergy_isLoaded = true;
  }
  return leadEnergy_;
}

const float &ttHLeptonic::jet_cdiscriminant1() {
  if (not jet_cdiscriminant1_isLoaded) {
    if (jet_cdiscriminant1_branch != 0) {
      jet_cdiscriminant1_branch->GetEntry(index);
    } else {
      printf("branch jet_cdiscriminant1_branch does not exist!\n");
      exit(1);
    }
    jet_cdiscriminant1_isLoaded = true;
  }
  return jet_cdiscriminant1_;
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

const float &ttHLeptonic::leadPassEVeto() {
  if (not leadPassEVeto_isLoaded) {
    if (leadPassEVeto_branch != 0) {
      leadPassEVeto_branch->GetEntry(index);
    } else {
      printf("branch leadPassEVeto_branch does not exist!\n");
      exit(1);
    }
    leadPassEVeto_isLoaded = true;
  }
  return leadPassEVeto_;
}

const float &ttHLeptonic::lead_Prompt() {
  if (not lead_Prompt_isLoaded) {
    if (lead_Prompt_branch != 0) {
      lead_Prompt_branch->GetEntry(index);
    } else {
      printf("branch lead_Prompt_branch does not exist!\n");
      exit(1);
    }
    lead_Prompt_isLoaded = true;
  }
  return lead_Prompt_;
}

const float &ttHLeptonic::mT() {
  if (not mT_isLoaded) {
    if (mT_branch != 0) {
      mT_branch->GetEntry(index);
    } else {
      printf("branch mT_branch does not exist!\n");
      exit(1);
    }
    mT_isLoaded = true;
  }
  return mT_;
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

const float &ttHLeptonic::leadGendeltaR() {
  if (not leadGendeltaR_isLoaded) {
    if (leadGendeltaR_branch != 0) {
      leadGendeltaR_branch->GetEntry(index);
    } else {
      printf("branch leadGendeltaR_branch does not exist!\n");
      exit(1);
    }
    leadGendeltaR_isLoaded = true;
  }
  return leadGendeltaR_;
}

const float &ttHLeptonic::jet_energy5() {
  if (not jet_energy5_isLoaded) {
    if (jet_energy5_branch != 0) {
      jet_energy5_branch->GetEntry(index);
    } else {
      printf("branch jet_energy5_branch does not exist!\n");
      exit(1);
    }
    jet_energy5_isLoaded = true;
  }
  return jet_energy5_;
}

const float &ttHLeptonic::nElecMedium() {
  if (not nElecMedium_isLoaded) {
    if (nElecMedium_branch != 0) {
      nElecMedium_branch->GetEntry(index);
    } else {
      printf("branch nElecMedium_branch does not exist!\n");
      exit(1);
    }
    nElecMedium_isLoaded = true;
  }
  return nElecMedium_;
}

const float &ttHLeptonic::tthMVA_RunII() {
  if (not tthMVA_RunII_isLoaded) {
    if (tthMVA_RunII_branch != 0) {
      tthMVA_RunII_branch->GetEntry(index);
    } else {
      printf("branch tthMVA_RunII_branch does not exist!\n");
      exit(1);
    }
    tthMVA_RunII_isLoaded = true;
  }
  return tthMVA_RunII_;
}

const float &ttHLeptonic::prefireProbability() {
  if (not prefireProbability_isLoaded) {
    if (prefireProbability_branch != 0) {
      prefireProbability_branch->GetEntry(index);
    } else {
      printf("branch prefireProbability_branch does not exist!\n");
      exit(1);
    }
    prefireProbability_isLoaded = true;
  }
  return prefireProbability_;
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

const float &ttHLeptonic::jet_udsgdiscriminant11() {
  if (not jet_udsgdiscriminant11_isLoaded) {
    if (jet_udsgdiscriminant11_branch != 0) {
      jet_udsgdiscriminant11_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant11_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant11_isLoaded = true;
  }
  return jet_udsgdiscriminant11_;
}

const float &ttHLeptonic::jet_hadronFlavour11() {
  if (not jet_hadronFlavour11_isLoaded) {
    if (jet_hadronFlavour11_branch != 0) {
      jet_hadronFlavour11_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour11_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour11_isLoaded = true;
  }
  return jet_hadronFlavour11_;
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

const float &ttHLeptonic::leadGenMatch() {
  if (not leadGenMatch_isLoaded) {
    if (leadGenMatch_branch != 0) {
      leadGenMatch_branch->GetEntry(index);
    } else {
      printf("branch leadGenMatch_branch does not exist!\n");
      exit(1);
    }
    leadGenMatch_isLoaded = true;
  }
  return leadGenMatch_;
}

const float &ttHLeptonic::sublead_SimpleMomID() {
  if (not sublead_SimpleMomID_isLoaded) {
    if (sublead_SimpleMomID_branch != 0) {
      sublead_SimpleMomID_branch->GetEntry(index);
    } else {
      printf("branch sublead_SimpleMomID_branch does not exist!\n");
      exit(1);
    }
    sublead_SimpleMomID_isLoaded = true;
  }
  return sublead_SimpleMomID_;
}

const float &ttHLeptonic::ele2_phi() {
  if (not ele2_phi_isLoaded) {
    if (ele2_phi_branch != 0) {
      ele2_phi_branch->GetEntry(index);
    } else {
      printf("branch ele2_phi_branch does not exist!\n");
      exit(1);
    }
    ele2_phi_isLoaded = true;
  }
  return ele2_phi_;
}

const float &ttHLeptonic::jet_udsgdiscriminant2() {
  if (not jet_udsgdiscriminant2_isLoaded) {
    if (jet_udsgdiscriminant2_branch != 0) {
      jet_udsgdiscriminant2_branch->GetEntry(index);
    } else {
      printf("branch jet_udsgdiscriminant2_branch does not exist!\n");
      exit(1);
    }
    jet_udsgdiscriminant2_isLoaded = true;
  }
  return jet_udsgdiscriminant2_;
}

const float &ttHLeptonic::jet_hadronFlavour2() {
  if (not jet_hadronFlavour2_isLoaded) {
    if (jet_hadronFlavour2_branch != 0) {
      jet_hadronFlavour2_branch->GetEntry(index);
    } else {
      printf("branch jet_hadronFlavour2_branch does not exist!\n");
      exit(1);
    }
    jet_hadronFlavour2_isLoaded = true;
  }
  return jet_hadronFlavour2_;
}

const float &ttHLeptonic::sublead_PhoGenPhi() {
  if (not sublead_PhoGenPhi_isLoaded) {
    if (sublead_PhoGenPhi_branch != 0) {
      sublead_PhoGenPhi_branch->GetEntry(index);
    } else {
      printf("branch sublead_PhoGenPhi_branch does not exist!\n");
      exit(1);
    }
    sublead_PhoGenPhi_isLoaded = true;
  }
  return sublead_PhoGenPhi_;
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

const float &ttHLeptonic::nb_loose() {
  if (not nb_loose_isLoaded) {
    if (nb_loose_branch != 0) {
      nb_loose_branch->GetEntry(index);
    } else {
      printf("branch nb_loose_branch does not exist!\n");
      exit(1);
    }
    nb_loose_isLoaded = true;
  }
  return nb_loose_;
}

const float &ttHLeptonic::JetBTagReshapeWeightDown01sigma() {
  if (not JetBTagReshapeWeightDown01sigma_isLoaded) {
    if (JetBTagReshapeWeightDown01sigma_branch != 0) {
      JetBTagReshapeWeightDown01sigma_branch->GetEntry(index);
    } else {
      printf("branch JetBTagReshapeWeightDown01sigma_branch does not exist!\n");
      exit(1);
    }
    JetBTagReshapeWeightDown01sigma_isLoaded = true;
  }
  return JetBTagReshapeWeightDown01sigma_;
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

const float &ttHLeptonic::TriggerWeightUp01sigma() {
  if (not TriggerWeightUp01sigma_isLoaded) {
    if (TriggerWeightUp01sigma_branch != 0) {
      TriggerWeightUp01sigma_branch->GetEntry(index);
    } else {
      printf("branch TriggerWeightUp01sigma_branch does not exist!\n");
      exit(1);
    }
    TriggerWeightUp01sigma_isLoaded = true;
  }
  return TriggerWeightUp01sigma_;
}

const float &ttHLeptonic::jet_energy11() {
  if (not jet_energy11_isLoaded) {
    if (jet_energy11_branch != 0) {
      jet_energy11_branch->GetEntry(index);
    } else {
      printf("branch jet_energy11_branch does not exist!\n");
      exit(1);
    }
    jet_energy11_isLoaded = true;
  }
  return jet_energy11_;
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
const float &jet_pt6() { return cms3.jet_pt6(); }
const float &jet_phi12() { return cms3.jet_phi12(); }
const float &lead_MomID() { return cms3.lead_MomID(); }
const float &sublead_SimpleMomStatus() { return cms3.sublead_SimpleMomStatus(); }
const float &jet_bdiscriminant10() { return cms3.jet_bdiscriminant10(); }
const float &jet_pt1() { return cms3.jet_pt1(); }
const float &nGoodEls() { return cms3.nGoodEls(); }
const float &jet_pt3() { return cms3.jet_pt3(); }
const float &lead_SmallestDr() { return cms3.lead_SmallestDr(); }
const float &jet_phi8() { return cms3.jet_phi8(); }
const float &sublead_Mad() { return cms3.sublead_Mad(); }
const float &jet_bbdiscriminant8() { return cms3.jet_bbdiscriminant8(); }
const float &jet_udsgdiscriminant8() { return cms3.jet_udsgdiscriminant8(); }
const float &jet_hadronFlavour8() { return cms3.jet_hadronFlavour8(); }
const float &ele2_eta() { return cms3.ele2_eta(); }
const float &jet_pt15() { return cms3.jet_pt15(); }
const float &topTag_topMass() { return cms3.topTag_topMass(); }
const float &dnn_score_ttgg() { return cms3.dnn_score_ttgg(); }
const float &jet_bdiscriminant9() { return cms3.jet_bdiscriminant9(); }
const float &jet_cdiscriminant4() { return cms3.jet_cdiscriminant4(); }
const float &jet_phi6() { return cms3.jet_phi6(); }
const float &diphoMVARes() { return cms3.diphoMVARes(); }
const float &jet_cdiscriminant5() { return cms3.jet_cdiscriminant5(); }
const float &sublead_sigmaEoE() { return cms3.sublead_sigmaEoE(); }
const float &jet_udsgdiscriminant13() { return cms3.jet_udsgdiscriminant13(); }
const float &jet_hadronFlavour13() { return cms3.jet_hadronFlavour13(); }
const float &sublead_photon_type() { return cms3.sublead_photon_type(); }
const float &leadPtGen() { return cms3.leadPtGen(); }
const float &sublead_Pythia() { return cms3.sublead_Pythia(); }
const float &dZ() { return cms3.dZ(); }
const float &jet_energy8() { return cms3.jet_energy8(); }
const float &sublead_closest_gen_Pt() { return cms3.sublead_closest_gen_Pt(); }
const float &jet_cdiscriminant12() { return cms3.jet_cdiscriminant12(); }
const float &jet_eta10() { return cms3.jet_eta10(); }
const float &jet_udsgdiscriminant12() { return cms3.jet_udsgdiscriminant12(); }
const float &jet_hadronFlavour12() { return cms3.jet_hadronFlavour12(); }
const float &nGoodElsFromTau() { return cms3.nGoodElsFromTau(); }
const float &muon2_eta() { return cms3.muon2_eta(); }
const float &jet_bbdiscriminant4() { return cms3.jet_bbdiscriminant4(); }
const float &LooseMvaSFDown01sigma() { return cms3.LooseMvaSFDown01sigma(); }
const float &jet_cdiscriminant3() { return cms3.jet_cdiscriminant3(); }
const float &TriggerWeightDown01sigma() { return cms3.TriggerWeightDown01sigma(); }
const float &tthMVA() { return cms3.tthMVA(); }
const float &jet_bdiscriminant6() { return cms3.jet_bdiscriminant6(); }
const float &jet_pt8() { return cms3.jet_pt8(); }
const float &MetPhi() { return cms3.MetPhi(); }
const float &muon1_eta() { return cms3.muon1_eta(); }
const float &sublead_Prompt() { return cms3.sublead_Prompt(); }
const float &leadIDMVA() { return cms3.leadIDMVA(); }
const float &jet_udsgdiscriminant7() { return cms3.jet_udsgdiscriminant7(); }
const float &jet_hadronFlavour7() { return cms3.jet_hadronFlavour7(); }
const float &jet_phi3() { return cms3.jet_phi3(); }
const float &jet_cdiscriminant9() { return cms3.jet_cdiscriminant9(); }
const float &lead_PhoGenPt() { return cms3.lead_PhoGenPt(); }
const float &jet_pt5() { return cms3.jet_pt5(); }
const float &jet_phi13() { return cms3.jet_phi13(); }
const float &sublead_closest_gen_dR() { return cms3.sublead_closest_gen_dR(); }
const float &lead_Pythia() { return cms3.lead_Pythia(); }
const float &nGoodMusFromTau() { return cms3.nGoodMusFromTau(); }
const float &ele1_pt() { return cms3.ele1_pt(); }
const float &jet_phi14() { return cms3.jet_phi14(); }
const float &ElectronWeightDown01sigma() { return cms3.ElectronWeightDown01sigma(); }
const float &jet_bdiscriminant14() { return cms3.jet_bdiscriminant14(); }
const float &jet_pt9() { return cms3.jet_pt9(); }
const float &nGoodMus() { return cms3.nGoodMus(); }
const float &LooseMvaSFUp01sigma() { return cms3.LooseMvaSFUp01sigma(); }
const float &muon2_pt() { return cms3.muon2_pt(); }
const float &jet_bdiscriminant5() { return cms3.jet_bdiscriminant5(); }
const float &jet_energy14() { return cms3.jet_energy14(); }
const float &lead_MomMomID() { return cms3.lead_MomMomID(); }
const float &jet_eta6() { return cms3.jet_eta6(); }
const float &n_jets() { return cms3.n_jets(); }
const float &UnmatchedPUWeightDown01sigma() { return cms3.UnmatchedPUWeightDown01sigma(); }
const float &jet_bbdiscriminant10() { return cms3.jet_bbdiscriminant10(); }
const float &rand() { return cms3.rand(); }
const float &jet_udsgdiscriminant4() { return cms3.jet_udsgdiscriminant4(); }
const float &jet_hadronFlavour4() { return cms3.jet_hadronFlavour4(); }
const float &subleadPt() { return cms3.subleadPt(); }
const float &mass() { return cms3.mass(); }
const float &jet_energy10() { return cms3.jet_energy10(); }
const float &jet_eta7() { return cms3.jet_eta7(); }
const float &jet_phi11() { return cms3.jet_phi11(); }
const float &sublead_MomMomID() { return cms3.sublead_MomMomID(); }
const float &jet_udsgdiscriminant10() { return cms3.jet_udsgdiscriminant10(); }
const float &jet_hadronFlavour10() { return cms3.jet_hadronFlavour10(); }
const float &MvaLinearSystDown01sigma() { return cms3.MvaLinearSystDown01sigma(); }
const float &jet_phi1() { return cms3.jet_phi1(); }
const float &electronVetoSFDown01sigma() { return cms3.electronVetoSFDown01sigma(); }
const float &jet_cdiscriminant8() { return cms3.jet_cdiscriminant8(); }
const float &jet_phi7() { return cms3.jet_phi7(); }
const float &sublead_sieie() { return cms3.sublead_sieie(); }
const float &JetBTagReshapeWeightUp01sigma() { return cms3.JetBTagReshapeWeightUp01sigma(); }
const float &jet_eta1() { return cms3.jet_eta1(); }
const float &jet_energy3() { return cms3.jet_energy3(); }
const float &lead_Mad() { return cms3.lead_Mad(); }
const float &lead_PassFrix() { return cms3.lead_PassFrix(); }
const float &jet_cdiscriminant6() { return cms3.jet_cdiscriminant6(); }
const float &ele1_charge() { return cms3.ele1_charge(); }
const float &jet_bbdiscriminant11() { return cms3.jet_bbdiscriminant11(); }
const float &PreselSFUp01sigma() { return cms3.PreselSFUp01sigma(); }
const float &jet_cdiscriminant2() { return cms3.jet_cdiscriminant2(); }
const float &weight_JetBTagWeight() { return cms3.weight_JetBTagWeight(); }
const float &jet_udsgdiscriminant6() { return cms3.jet_udsgdiscriminant6(); }
const float &jet_hadronFlavour6() { return cms3.jet_hadronFlavour6(); }
const float &sublead_hoe() { return cms3.sublead_hoe(); }
const float &muonSubleadIso() { return cms3.muonSubleadIso(); }
const float &leadR9() { return cms3.leadR9(); }
const float &dipho_cosphi() { return cms3.dipho_cosphi(); }
const float &sublead_MomID() { return cms3.sublead_MomID(); }
const float &jet_eta13() { return cms3.jet_eta13(); }
const float &FracRVWeightUp01sigma() { return cms3.FracRVWeightUp01sigma(); }
const float &subleadGenMatch() { return cms3.subleadGenMatch(); }
const float &lead_PhoGenPhi() { return cms3.lead_PhoGenPhi(); }
const float &nElecLoose() { return cms3.nElecLoose(); }
const float &jet_eta12() { return cms3.jet_eta12(); }
const float &jet_udsgdiscriminant9() { return cms3.jet_udsgdiscriminant9(); }
const float &jet_hadronFlavour9() { return cms3.jet_hadronFlavour9(); }
const float &electronVetoSFUp01sigma() { return cms3.electronVetoSFUp01sigma(); }
const float &subleadGendeltaR() { return cms3.subleadGendeltaR(); }
const float &muon2_energy() { return cms3.muon2_energy(); }
const float &sublead_PhoGenEta() { return cms3.sublead_PhoGenEta(); }
const float &jet_udsgdiscriminant15() { return cms3.jet_udsgdiscriminant15(); }
const float &jet_hadronFlavour15() { return cms3.jet_hadronFlavour15(); }
const float &jet_phi5() { return cms3.jet_phi5(); }
const float &jet_eta2() { return cms3.jet_eta2(); }
const float &subleadEta() { return cms3.subleadEta(); }
const float &jet_pt13() { return cms3.jet_pt13(); }
const float &leadEt() { return cms3.leadEt(); }
const float &jet_pt4() { return cms3.jet_pt4(); }
const float &ElectronWeightUp01sigma() { return cms3.ElectronWeightUp01sigma(); }
const float &jet_udsgdiscriminant3() { return cms3.jet_udsgdiscriminant3(); }
const float &jet_hadronFlavour3() { return cms3.jet_hadronFlavour3(); }
const float &lead_sieie() { return cms3.lead_sieie(); }
const float &MetPt() { return cms3.MetPt(); }
const float &jet_cdiscriminant7() { return cms3.jet_cdiscriminant7(); }
const float &jet_udsgdiscriminant14() { return cms3.jet_udsgdiscriminant14(); }
const float &jet_hadronFlavour14() { return cms3.jet_hadronFlavour14(); }
const float &jet_bbdiscriminant14() { return cms3.jet_bbdiscriminant14(); }
const float &muon1_energy() { return cms3.muon1_energy(); }
const float &jet_eta9() { return cms3.jet_eta9(); }
const float &jet_bbdiscriminant7() { return cms3.jet_bbdiscriminant7(); }
const float &jet_bbdiscriminant9() { return cms3.jet_bbdiscriminant9(); }
const float &muon2_charge() { return cms3.muon2_charge(); }
const float &FracRVNvtxWeightUp01sigma() { return cms3.FracRVNvtxWeightUp01sigma(); }
const float &ele1_energy() { return cms3.ele1_energy(); }
const float &nMuonMedium() { return cms3.nMuonMedium(); }
const float &jet_cdiscriminant14() { return cms3.jet_cdiscriminant14(); }
const float &nMuonTight() { return cms3.nMuonTight(); }
const float &jet_energy12() { return cms3.jet_energy12(); }
const float &nGoodTaus() { return cms3.nGoodTaus(); }
const float &jet_bbdiscriminant12() { return cms3.jet_bbdiscriminant12(); }
const float &n_ele() { return cms3.n_ele(); }
const float &muon1_charge() { return cms3.muon1_charge(); }
const float &jet_phi15() { return cms3.jet_phi15(); }
const float &jet_eta5() { return cms3.jet_eta5(); }
const float &subleadEt() { return cms3.subleadEt(); }
const float &lead_closest_gen_dR() { return cms3.lead_closest_gen_dR(); }
const float &Mjj() { return cms3.Mjj(); }
const float &jet_udsgdiscriminant5() { return cms3.jet_udsgdiscriminant5(); }
const float &jet_hadronFlavour5() { return cms3.jet_hadronFlavour5(); }
const float &subleadIDMVA() { return cms3.subleadIDMVA(); }
const float &jet_cdiscriminant11() { return cms3.jet_cdiscriminant11(); }
const float &jet_pt14() { return cms3.jet_pt14(); }
const float &lead_closest_gen_Pt() { return cms3.lead_closest_gen_Pt(); }
const float &UnmatchedPUWeightUp01sigma() { return cms3.UnmatchedPUWeightUp01sigma(); }
const float &lead_SimpleMomID() { return cms3.lead_SimpleMomID(); }
const float &nElecTight() { return cms3.nElecTight(); }
const float &ele1_eta() { return cms3.ele1_eta(); }
const float &jet_bdiscriminant8() { return cms3.jet_bdiscriminant8(); }
const float &jet_bbdiscriminant13() { return cms3.jet_bbdiscriminant13(); }
const float &leadEta() { return cms3.leadEta(); }
const float &ele2_energy() { return cms3.ele2_energy(); }
const float &nb_medium() { return cms3.nb_medium(); }
const float &JetBTagCutWeightDown01sigma() { return cms3.JetBTagCutWeightDown01sigma(); }
const float &jet_pt11() { return cms3.jet_pt11(); }
const float &jet_pt10() { return cms3.jet_pt10(); }
const float &jet_udsgdiscriminant1() { return cms3.jet_udsgdiscriminant1(); }
const float &jet_hadronFlavour1() { return cms3.jet_hadronFlavour1(); }
const float &muonLeadIso() { return cms3.muonLeadIso(); }
const float &subleadPhi() { return cms3.subleadPhi(); }
const float &jet_cdiscriminant13() { return cms3.jet_cdiscriminant13(); }
const float &subleadPixelSeed() { return cms3.subleadPixelSeed(); }
const float &n_muons() { return cms3.n_muons(); }
const float &lead_SimpleMomStatus() { return cms3.lead_SimpleMomStatus(); }
const float &jet_energy6() { return cms3.jet_energy6(); }
const float &sublead_SmallestDr() { return cms3.sublead_SmallestDr(); }
const float &dipho_rapidity() { return cms3.dipho_rapidity(); }
const float &jet_bdiscriminant12() { return cms3.jet_bdiscriminant12(); }
const float &subleadEnergy() { return cms3.subleadEnergy(); }
const float &jet_phi2() { return cms3.jet_phi2(); }
const float &jet_cdiscriminant10() { return cms3.jet_cdiscriminant10(); }
const float &lead_hoe() { return cms3.lead_hoe(); }
const float &sublead_ptoM() { return cms3.sublead_ptoM(); }
const float &jet_bbdiscriminant6() { return cms3.jet_bbdiscriminant6(); }
const float &jet_energy15() { return cms3.jet_energy15(); }
const float &leadPixelSeed() { return cms3.leadPixelSeed(); }
const float &vertex_idx() { return cms3.vertex_idx(); }
const float &jet_bbdiscriminant1() { return cms3.jet_bbdiscriminant1(); }
const float &sublead_PhoGenPt() { return cms3.sublead_PhoGenPt(); }
const float &jet_energy4() { return cms3.jet_energy4(); }
const float &jet_energy7() { return cms3.jet_energy7(); }
const float &JetBTagCutWeightUp01sigma() { return cms3.JetBTagCutWeightUp01sigma(); }
const float &MvaLinearSystUp01sigma() { return cms3.MvaLinearSystUp01sigma(); }
const float &jet_eta3() { return cms3.jet_eta3(); }
const float &jet_energy2() { return cms3.jet_energy2(); }
const float &nb_tight() { return cms3.nb_tight(); }
const float &jet_bbdiscriminant5() { return cms3.jet_bbdiscriminant5(); }
const float &jet_bdiscriminant1() { return cms3.jet_bdiscriminant1(); }
const float &n_bjets() { return cms3.n_bjets(); }
const float &jet_bbdiscriminant15() { return cms3.jet_bbdiscriminant15(); }
const float &subleadPtGen() { return cms3.subleadPtGen(); }
const float &jet_pt2() { return cms3.jet_pt2(); }
const float &lead_photon_type() { return cms3.lead_photon_type(); }
const float &FracRVNvtxWeightDown01sigma() { return cms3.FracRVNvtxWeightDown01sigma(); }
const float &PreselSFDown01sigma() { return cms3.PreselSFDown01sigma(); }
const float &lead_sigmaEoE() { return cms3.lead_sigmaEoE(); }
const float &muon1_phi() { return cms3.muon1_phi(); }
const float &dipho_sumpt() { return cms3.dipho_sumpt(); }
const float &jet_bdiscriminant3() { return cms3.jet_bdiscriminant3(); }
const float &subleadPassEVeto() { return cms3.subleadPassEVeto(); }
const float &jet_energy1() { return cms3.jet_energy1(); }
const float &jet_eta15() { return cms3.jet_eta15(); }
const float &topTag_score() { return cms3.topTag_score(); }
const float &jet_phi10() { return cms3.jet_phi10(); }
const float &jet_eta8() { return cms3.jet_eta8(); }
const float &CMS_hgg_mass() { return cms3.CMS_hgg_mass(); }
const float &centralObjectWeight() { return cms3.centralObjectWeight(); }
const float &jet_bbdiscriminant2() { return cms3.jet_bbdiscriminant2(); }
const float &jet_eta11() { return cms3.jet_eta11(); }
const float &nMuonLoose() { return cms3.nMuonLoose(); }
const float &jet_bbdiscriminant3() { return cms3.jet_bbdiscriminant3(); }
const float &FracRVWeightDown01sigma() { return cms3.FracRVWeightDown01sigma(); }
const float &jet_energy9() { return cms3.jet_energy9(); }
const float &sublead_PassFrix() { return cms3.sublead_PassFrix(); }
const float &ele1_phi() { return cms3.ele1_phi(); }
const float &ele2_charge() { return cms3.ele2_charge(); }
const float &jet_bdiscriminant15() { return cms3.jet_bdiscriminant15(); }
const float &jet_phi9() { return cms3.jet_phi9(); }
const float &jet_energy13() { return cms3.jet_energy13(); }
const float &muon2_phi() { return cms3.muon2_phi(); }
const float &jet_pt7() { return cms3.jet_pt7(); }
const float &topTag_WMass() { return cms3.topTag_WMass(); }
const float &jet_cdiscriminant15() { return cms3.jet_cdiscriminant15(); }
const float &jet_bdiscriminant7() { return cms3.jet_bdiscriminant7(); }
const float &lead_PhoGenEta() { return cms3.lead_PhoGenEta(); }
const float &jet_bdiscriminant2() { return cms3.jet_bdiscriminant2(); }
const float &leadEnergy() { return cms3.leadEnergy(); }
const float &jet_cdiscriminant1() { return cms3.jet_cdiscriminant1(); }
const float &leadPt() { return cms3.leadPt(); }
const float &leadPassEVeto() { return cms3.leadPassEVeto(); }
const float &lead_Prompt() { return cms3.lead_Prompt(); }
const float &mT() { return cms3.mT(); }
const float &jet_bdiscriminant13() { return cms3.jet_bdiscriminant13(); }
const float &jet_phi4() { return cms3.jet_phi4(); }
const float &jet_pt12() { return cms3.jet_pt12(); }
const float &leadGendeltaR() { return cms3.leadGendeltaR(); }
const float &jet_energy5() { return cms3.jet_energy5(); }
const float &nElecMedium() { return cms3.nElecMedium(); }
const float &tthMVA_RunII() { return cms3.tthMVA_RunII(); }
const float &prefireProbability() { return cms3.prefireProbability(); }
const float &leadPhi() { return cms3.leadPhi(); }
const float &subleadR9() { return cms3.subleadR9(); }
const float &jet_udsgdiscriminant11() { return cms3.jet_udsgdiscriminant11(); }
const float &jet_hadronFlavour11() { return cms3.jet_hadronFlavour11(); }
const float &jet_eta14() { return cms3.jet_eta14(); }
const float &leadGenMatch() { return cms3.leadGenMatch(); }
const float &sublead_SimpleMomID() { return cms3.sublead_SimpleMomID(); }
const float &ele2_phi() { return cms3.ele2_phi(); }
const float &jet_udsgdiscriminant2() { return cms3.jet_udsgdiscriminant2(); }
const float &jet_hadronFlavour2() { return cms3.jet_hadronFlavour2(); }
const float &sublead_PhoGenPhi() { return cms3.sublead_PhoGenPhi(); }
const float &jet_eta4() { return cms3.jet_eta4(); }
const float &nb_loose() { return cms3.nb_loose(); }
const float &JetBTagReshapeWeightDown01sigma() { return cms3.JetBTagReshapeWeightDown01sigma(); }
const float &lead_ptoM() { return cms3.lead_ptoM(); }
const float &ele2_pt() { return cms3.ele2_pt(); }
const float &jet_bdiscriminant11() { return cms3.jet_bdiscriminant11(); }
const float &TriggerWeightUp01sigma() { return cms3.TriggerWeightUp01sigma(); }
const float &jet_energy11() { return cms3.jet_energy11(); }
const float &muon1_pt() { return cms3.muon1_pt(); }
const float &jet_bdiscriminant4() { return cms3.jet_bdiscriminant4(); }
const float &rho() { return cms3.rho(); }
const int &nvtx() { return cms3.nvtx(); }
const unsigned long long &event() { return cms3.event(); }
const unsigned int &lumi() { return cms3.lumi(); }
const int &processIndex() { return cms3.processIndex(); }
const unsigned int &run() { return cms3.run(); }
const float &npu() { return cms3.npu(); }
const float &puweight() { return cms3.puweight(); }

}
