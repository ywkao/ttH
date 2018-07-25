export SCRAM_ARCH=slc6_amd64_gcc530
source /cvmfs/cms.cern.ch/cmsset_default.sh

if [ ! -d CMSSW_8_0_28 ] ; then
  echo "Need to run setup.sh first!"
  exit 1
fi

cd CMSSW_8_0_28/src/flashgg
cmsenv

#INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016B-03Feb2017_ver2-v2/microAOD_1.root'


#INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/microAOD_1.root'
INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2017_skim_v1/TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8/RunIIFall17-3_1_0-3_1_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/microAOD_1.root'

#INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCUETP8M1_13TeV_Pythia8/RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/microAOD_1.root'
#INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8_v2/RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-BS2016_BSandPUSummer16_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/microAOD_1.root'

cmsRun Taggers/test/ttH_TagAndDump.py ${INPUTFILENAMES} 1000
