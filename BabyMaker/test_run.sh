export SCRAM_ARCH=slc6_amd64_gcc530
source /cvmfs/cms.cern.ch/cmsset_default.sh

source setup.sh
cd CMSSW_8_0_28/src/flashgg
cmsenv

INPUTFILENAMES='/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016B-03Feb2017_ver2-v2/microAOD_1.root'

cmsRun Taggers/test/ttH_Tag.py ${INPUTFILENAMES}

cmsRun Validation/test/ttHTagsDumper_cfg.py
