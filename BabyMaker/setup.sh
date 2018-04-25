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
  git checkout tth_dev

  cd ..
  source flashgg/setup_8_0_X.sh # ignore the "error: addinfo_cache" messages

  # Build
  cd $CMSSW_BASE/src
  scram b -j 20
  cd ../../
else
  cd ../../   
fi


pushd ~/ProjectMetis # or replace with your metis location
source setup.sh
popd 
