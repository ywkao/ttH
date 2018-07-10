import os, sys
import h5py
import ROOT
import numpy
import root_numpy

babies_version = "v3.6"

signal_hadronic = ["ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2__ttH_Babies"]

bkg_hadronic = ["QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies", "QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies", "QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies", "DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa__ttH_Babies", "GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies", "GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies", "GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8__ttH_Babies"]

branches_hadronic = ["leadPt", "leadIDMVA", "subleadPt", "subleadIDMVA"]

ntuple = numpy.empty(shape=0)


