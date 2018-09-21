export SCRAM_ARCH=slc6_amd64_gcc530
source /cvmfs/cms.cern.ch/cmsset_default.sh

if (( $# < 1 )); then
  echo "Please provide CMSSW version to setup ('80X' for 2016 or '94X' for 2017)"
  return
fi

CMSSW_VER=$1
echo "Running babymaker for $CMSSW_VER"

INPUTFILENAMES=''

if [ "$CMSSW_VER" = "80X" ]; then
  export SCRAM_ARCH=slc6_amd64_gcc530
  if [ ! -d CMSSW_8_0_28 ] ; then
    echo "Need to run setup.sh first!"
    return
  fi

  cd CMSSW_8_0_28/src/flashgg
  cmsenv

  #INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v3_jetPt20/TTGG_0Jets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16-2_4_1-25ns_Moriond17-2_4_1-v0-RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/microAOD_1.root' # or change to your favorite file
  INPUTFILENAMES='file:/home/users/bemarsh/analysis/tth/test/CMSSW_8_0_28/src/flashgg/myMicroAODOutputFile.root'

fi

if [ "$CMSSW_VER" = "94X" ]; then
  export SCRAM_ARCH=slc6_amd64_gcc530
  if [ ! -d CMSSW_9_4_6 ] ; then
    echo "Need to run setup.sh first!"
    return
  fi

  cd CMSSW_9_4_6/src/flashgg
  cmsenv

  #INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2017_skim_v1/TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8/RunIIFall17-3_1_0-3_1_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/microAOD_1.root' # or change to your favorite file
  INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/RunIIFall17-3_2_0_skim_v1/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17-3_2_0-3_2_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/microAOD_1.root'

fi

if [ "$CMSSW_VER" = "101X" ]; then
  export SCRAM_ARCH=slc6_amd64_gcc530
  if [ ! -d CMSSW_10_1_1 ] ; then
    echo "Need to run setup.sh first!"
    return
  fi

  cd CMSSW_10_1_1/src/flashgg
  cmsenv

  #INPUTFILENAMES='file:/home/users/sjmay/ttH/BabyMaker/test_10X/CMSSW_10_1_1/src/flashgg/myMicroAODOutputFile.root'
  INPUTFILENAMES='file:/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/test/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIISummer18MiniAOD-101X_upgrade2018_realistic_v7-v1_MINIAODSIM_test/test_skim_1.root'

fi


# Actually run the test
cmsRun Taggers/test/ttH_TagAndDump.py ${INPUTFILENAMES} 1000
