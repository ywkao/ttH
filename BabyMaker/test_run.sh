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
  #INPUTFILENAMES='file:/home/users/bemarsh/analysis/tth/test/CMSSW_8_0_28/src/flashgg/myMicroAODOutputFile.root'
  INPUTFILENAMES='file:/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/2016_topTag_overlapRemoval/test_ttgg.root'

fi

if [ "$CMSSW_VER" = "94X" ]; then
  export SCRAM_ARCH=slc6_amd64_gcc530
  if [ ! -d CMSSW_9_4_6 ] ; then
    echo "Need to run setup.sh first!"
    return
  fi

  cd CMSSW_9_4_6/src/flashgg
  cmsenv

  #INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/forHualin_2017/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_MINIAODSIM_forHualin_2017/test_skim_1.root'
  #INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/forHualin_2017/TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2_MINIAODSIM_forHualin_2017/test_skim_1.root'
  INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/forHualin_2017/DoubleEG_Run2017C-31Mar2018-v1_MINIAOD_forHualin_2017/test_skim_433.root'

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
  #INPUTFILENAMES='file:/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/test/GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8_RunIISummer18MiniAOD-101X_upgrade2018_realistic_v7-v1_MINIAODSIM_test/test_skim_1.root'

fi


if [ "$CMSSW_VER" = "102X" ]; then
  export SCRAM_ARCH=slc6_amd64_gcc700
  if [ ! -d CMSSW_10_2_1 ] ; then
    echo "Need to run setup.sh first!"
    return
  fi

  cd CMSSW_10_2_1/src/flashgg
  cmsenv

  INPUTFILENAMES='file:/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/RunIIFall18-4_0_0/EGamma/RunIIFall18-4_0_0-RunIIFall18-4_0_0-3-g040ebcb0-v0-Run2018B-PromptReco-v1/microAOD_1.root'
fi


# Actually run the test
cmsRun Taggers/test/ttH_TagAndDump.py ${INPUTFILENAMES}
#cmsRun Taggers/test/ttH_TagAndDump.py ${INPUTFILENAMES} 1000
#cmsRun Taggers/test/ttH_TagAndDump.py ${INPUTFILENAMES} 1000 puTarget=6.246e-06,2.63e-05,4.92e-05,9.085e-05,9.855e-05,0.0001426,0.0001557,0.0001656,0.0002269,0.0005395,0.001076,0.002034,0.00322,0.004616,0.006528,0.009202,0.01283,0.01707,0.02125,0.0251,0.02847,0.03118,0.03325,0.03486,0.03625,0.03757,0.03869,0.03937,0.03946,0.03892,0.03782,0.03627,0.03435,0.03211,0.02967,0.0272,0.02483,0.02264,0.0207,0.01907,0.01784,0.01709,0.01686,0.0171,0.01771,0.01849,0.01916,0.01945,0.01911,0.01804,0.01627,0.01399,0.01147,0.008977,0.006728,0.004849,0.003375,0.002281,0.001504,0.0009715,0.0006179,0.0003883,0.000242,0.0001501,9.294e-05,5.768e-05,3.599e-05,2.263e-05,1.438e-05,9.234e-06,5.996e-06,3.933e-06,2.602e-06,1.731e-06,1.157e-06,7.744e-07,5.184e-07,3.466e-07,2.311e-07,1.535e-07,1.015e-07,6.677e-08,4.366e-08,2.836e-08,1.829e-08,1.171e-08,7.438e-09,4.686e-09,2.927e-09,1.812e-09,1.111e-09,6.754e-10,4.066e-10,2.424e-10,1.431e-10,8.363e-11,4.839e-11,2.771e-11,1.571e-11
