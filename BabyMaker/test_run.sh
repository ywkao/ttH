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

fi

if [ "$CMSSW_VER" = "94X" ]; then
  export SCRAM_ARCH=slc6_amd64_gcc530
  if [ ! -d CMSSW_9_4_6 ] ; then
    echo "Need to run setup.sh first!"
    return
  fi

  cd CMSSW_9_4_6/src/flashgg
  cmsenv

  INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD/forHualin_2017/TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2_MINIAODSIM_forHualin_2017/test_skim_1.root'
#  INPUTFILENAMES='/home/users/hmei/test/CMSSW_9_4_9/src/flashgg/myMicroAODOutputFile.root'
#  INPUTFILENAMES='/home/users/hmei/CMSSW_9_4_6/src/flashgg/myMicroAODOutputFile_ttGG.root'
#  INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2017_skim_v1/TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8/RunIIFall17-3_1_0-3_1_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/microAOD_1.root' # or change to your favorite file
#  INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/RunIIFall17-3_2_0_skim_v1/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17-3_2_0-3_2_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/microAOD_1.root'
#  INPUTFILENAMES='/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-3_2_0/3_2_0/GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8/RunIIFall17-3_2_0-3_2_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/180817_102411/0000/myMicroAODOutputFile_7.root'
#  INPUTFILENAMES='/store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIIFall17-3_2_0/3_2_0/TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8/RunIIFall17-3_2_0-3_2_0-v0-RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/180817_112700/0000/myMicroAODOutputFile_37.root'
fi

# Actually run the test
cmsRun Taggers/test/ttH_TagAndDump.py ${INPUTFILENAMES} 1000
