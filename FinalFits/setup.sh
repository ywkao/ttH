if [ ! -d CMSSW_7_4_7 ]; then
  cmsrel CMSSW_7_4_7
fi
cd CMSSW_7_4_7/src
cmsenv
git cms-init

# install combine
if [ ! -d HiggsAnalysis/CombinedLimit ]; then
  git clone git@github.com:cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
  cd ${CMSSW_BASE}/src/HiggsAnalysis/CombinedLimit
  git fetch origin
  git checkout origin/74x-root6
  git checkout -b mybranch
fi
cd ${CMSSW_BASE}/src

if [ ! -d HiggsAnalysis/GBRLikelihood ]; then
  git clone git@github.com:bendavid/GBRLikelihood.git HiggsAnalysis/GBRLikelihood
  cd ${CMSSW_BASE}/src/HiggsAnalysis
  cmsenv
  scram b -j 20
fi

# Download flashggFinalFit
cd ${CMSSW_BASE}/src/
if [ ! -d flashggFinalFit ]; then
  git clone git@github.com:sam-may/flashggFinalFit.git
  cd ${CMSSW_BASE}/src/flashggFinalFit/
  git checkout ttHOnlyFit

  # Build
  cd Signal
  make
  cd ../Background
  make
  cd ../..
fi

cd ../..

if [ ! -d CMSSW_8_0_28 ]; then
  export SCRAM_ARCH=slc6_amd64_gcc530
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
  git checkout finalfits_80x

  cd ..
  source flashgg/setup_8_0_X.sh # ignore the "error: addinfo_cache" messages

  # Build
  cd $CMSSW_BASE/src
  scram b -j 30
  cd ../../
else
  cd ../../
fi

