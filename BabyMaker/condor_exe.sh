PACKAGE=package.tar.gz
OUTPUTDIR=$1
OUTPUTFILENAME=$2
INPUTFILENAMES=$3
INDEX=$4
CMSSW_VER=$5

ARGS=$7

echo "[wrapper] OUTPUTDIR	= " ${OUTPUTDIR}
echo "[wrapper] OUTPUTFILENAME	= " ${OUTPUTFILENAME}
echo "[wrapper] INPUTFILENAMES	= " ${INPUTFILENAMES}
echo "[wrapper] INDEX		= " ${INDEX}

echo "[wrapper] printing env"
printenv
echo 

echo "[wrapper] hostname  = " `hostname`
echo "[wrapper] date      = " `date`
echo "[wrapper] linux timestamp = " `date +%s`

######################
# Set up environment #
######################

export SCRAM_ARCH=slc7_amd64_gcc700
source /cvmfs/cms.cern.ch/cmsset_default.sh

# Untar
#xrdcp root://redirector.t2.ucsd.edu//store/user/smay/ttH/tarballs/FIXME package.tar.gz 
tar -xvf package.tar.gz
rm package.tar.gz

# Build
cd $CMSSW_VER/src/flashgg
echo "[wrapper] in directory: " ${PWD}

eval `scramv1 runtime -sh`
#echo "[wrapper] copying data from /eos"
#source MetaData/scripts/fggPopulateDataDir_auto.sh
#echo "[wrapper] done copying, ls-ing dirs now"
#ls -altrh Systematics/data
#ls -altrh Taggers/data
#ls -atlrh MicroAOD/data

echo "[wrapper] attempting to build"
cd $CMSSW_BASE
scramv1 b ProjectRename
scram b -j1
eval `scramv1 runtime -sh`
cd $CMSSW_BASE/src/flashgg
pip install --user htcondor
#export PYTHONPATH=$PYTHONPATH:$CMSSW_BASE/src/flashgg/Taggers/test/

# Create tag file
sed -i 's/MaxObjectWeightException = cms.double(10.),/MaxObjectWeightException = cms.double(13.),/g' Taggers/python/flashggTagSorter_cfi.py
echo "[wrapper `date +\"%Y%m%d %k:%M:%S\"`] running: cmsRun Taggers/test/ttH_TagAndDump.py ${INPUTFILENAMES} ${ARGS}"
cmsRun Taggers/test/ttH_TagAndDump.py ${INPUTFILENAMES} ${ARGS}

if [ "$?" != "0" ]; then
    echo "Removing output file because cmsRun crashed with exit code $?"
    rm *.root
fi

echo "[wrapper] output root files are currently: "
ls -lh *.root

# Copy output
eval `scram unsetenv -sh` # have to add this because CMSSW and gfal interfere with each other or something...
gfal-copy -p -f -t 4200 --verbose file://`pwd`/${OUTPUTFILENAME}.root gsiftp://gftp.t2.ucsd.edu/${OUTPUTDIR}/${OUTPUTFILENAME}_${INDEX}.root --checksum ADLER32
