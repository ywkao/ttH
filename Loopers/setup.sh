export SCRAM_ARCH=slc6_amd64_gcc630   # or whatever scram_arch you need for your desired CMSSW release
export CMSSW_VERSION=CMSSW_9_4_0
source /cvmfs/cms.cern.ch/cmsset_default.sh
cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/cmssw/$CMSSW_VERSION/src
eval `scramv1 runtime -sh`
cd -

export PYTHONPATH=/home/users/namin/2017/fourtop/analysis/bdt/xgboost/python-package/lib/python2.7/site-packages/:$PYTHONPATH
export PYTHONPATH=/home/users/namin/2017/fourtop/analysis/bdt/xgboost/python-package/:$PYTHONPATH

if [ ! -d Optimization/weights ]; then
  mkdir Optimization/weights
fi
if [ ! -d Optimization/ROCs ]; then
  mkdir Optimization/ROCs
fi

