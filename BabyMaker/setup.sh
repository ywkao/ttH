export SCRAM_ARCH=slc6_amd64_gcc530

# Set up Shell Utilities
source ~/Utilities/ShellFunctions.sh
export -f addHistos

# Set up CMMSW 80X
if [ ! -d CMSSW_8_0_28 ]; then
  cmsrel CMSSW_8_0_28
  cd CMSSW_8_0_28/src
  cmsenv
  git cms-init
else
  cd CMSSW_8_0_28/src
  cmsenv
fi

if [ ! -d flashgg ]; then
  cd $CMSSW_BASE/src
  git clone https://github.com/sam-may/flashgg 
  cd flashgg
  git checkout tth_dev_80X

  cd ..
  source flashgg/setup_8_0_X.sh # ignore the "error: addinfo_cache" messages

  # Build
  cd $CMSSW_BASE/src
  scram b -j 30
  cd ../../
else
  cd ../../   
fi

if [ ! -d CMSSW_9_4_6 ]; then
  cmsrel CMSSW_9_4_6
  cd CMSSW_9_4_6/src
  cmsenv
  git cms-init
else
  cd CMSSW_9_4_6/src
  cmsenv
fi

if [ ! -d flashgg ]; then
  cd $CMSSW_BASE/src
  git clone https://github.com/sam-may/flashgg
  cd flashgg
  git checkout tth_dev_94X

  cd ..
  source flashgg/setup_9_4_X.sh 

  # Build
  cd $CMSSW_BASE/src
  scram b -j 30
  source flashgg/afterbuild_9_4_X.sh
  cd ../../
else
  cd ../../
fi



if [ ! -d logs ]; then
  mkdir logs
fi

pushd ~/ProjectMetis # or replace with your metis location
source setup.sh
popd 
