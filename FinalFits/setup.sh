if [ ! -d CMSSW_7_4_7 ]; then
  cmsrel CMSSW_7_4_7
fi
cd CMSSW_7_4_7/src
cmsenv
git cms-init

# install combine
if [ ! -d HiggsAnalysis/CombinedLimit ]; then
  git clone git@github.com:cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
fi
cd ${CMSSW_BASE}/src/HiggsAnalysis/CombinedLimit
git fetch origin
git checkout origin/74x-root6
git checkout -b mybranch
cd ${CMSSW_BASE}/src

if [ ! -d HiggsAnalysis/GBRLikelihood ]; then
  git clone git@github.com:bendavid/GBRLikelihood.git HiggsAnalysis/GBRLikelihood
fi

cd ${CMSSW_BASE}/src/HiggsAnalysis
cmsenv
scram b -j 20

# Download flashggFinalFit
cd ${CMSSW_BASE}/src/
if [ ! -d flashggFinalFit ]; then
  git clone git@github.com:sam-may/flashggFinalFit.git
fi

cd ${CMSSW_BASE}/src/flashggFinalFit/
git checkout ttHOnlyFit

# Build
cd Signal
make
cd ../Background
make
cd ..
