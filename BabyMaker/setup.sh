export SCRAM_ARCH=slc6_amd64_gcc530

# Set up Shell Utilities
source /home/users/sjmay/Utilities/ShellFunctions.sh
export -f addHistos


if (( $# < 1 )); then
  echo "Please provide CMSSW version to setup ('80X' for 2016 or '94X' for 2017)"
  return
fi

CMSSW_VER=$1
echo "Setting up for $CMSSW_VER"

if [ "$CMSSW_VER" = "80X" ]; then
  # Set up CMMSW 80X
  export SCRAM_ARCH=slc6_amd64_gcc530
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
fi

if [ "$CMSSW_VER" = "94X" ]; then
  # Set up CMSSW 94X
  export SCRAM_ARCH=slc6_amd64_gcc630
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
fi

if [ "$CMSSW_VER" = "101X" ]; then
  # Set up CMSSW 101X
  export SCRAM_ARCH=slc6_amd64_gcc630
  if [ ! -d CMSSW_10_1_1 ]; then
    cmsrel CMSSW_10_1_1
    cd CMSSW_10_1_1/src
    cmsenv
    git cms-init
  else
    cd CMSSW_10_1_1/src
    cmsenv
  fi

  if [ ! -d flashgg ]; then
    cd $CMSSW_BASE/src
    git clone https://github.com/sam-may/flashgg
    cd flashgg
    git checkout tth_dev_101X

    cd ..
    source flashgg/setup_10_1_X.sh

    # Build
    cd $CMSSW_BASE/src
    scram b -j 30
    cd ../../
  else
    cd ../../
  fi
fi

if [ "$CMSSW_VER" = "102X" ]; then
  # Set up CMSSW 102X
  export SCRAM_ARCH=slc6_amd64_gcc700 
  if [ ! -d CMSSW_10_2_9 ]; then
    cmsrel CMSSW_10_2_9
    cd CMSSW_10_2_9/src
    cmsenv
    git cms-init
  else
    cd CMSSW_10_2_9/src
    cmsenv
  fi

  if [ ! -d flashgg ]; then
    cd $CMSSW_BASE/src
    git clone https://github.com/sam-may/flashgg
    cd flashgg
    git checkout tth_dev_runII

    cd ..
    source flashgg/setup_10_2_X.sh

    # Build
    cd $CMSSW_BASE/src
    scram b -j 30
    cd ../../
  else
    cd ../../
  fi
fi

if [ "$CMSSW_VER" = "105X" ]; then
  # Set up CMSSW 105X
  export SCRAM_ARCH=slc7_amd64_gcc700
  if [ ! -d CMSSW_10_5_0 ]; then
    cmsrel CMSSW_10_5_0
    cd CMSSW_10_5_0/src
    cmsenv
    git cms-init
  else
    cd CMSSW_10_5_0/src
    cmsenv
  fi

  if [ ! -d flashgg ]; then
    cd $CMSSW_BASE/src
    git clone https://github.com/sam-may/flashgg
    cd flashgg
    git checkout tth_dev_runII_v2

    cd ..
    source flashgg/setup_flashgg.sh

    # Build
    cd $CMSSW_BASE/src
    scram b -j 30
    cd ../../
  else
    cd ../../
  fi
fi

if [ "$CMSSW_VER" = "106X" ]; then
  # Set up CMSSW 105X
  export SCRAM_ARCH=slc7_amd64_gcc700
  if [ ! -d CMSSW_10_6_8 ]; then
    cmsrel CMSSW_10_6_8
    cd CMSSW_10_6_8/src
    cmsenv
    git cms-init
  else
    cd CMSSW_10_6_8/src
    cmsenv
  fi

  if [ ! -d flashgg ]; then
    cd $CMSSW_BASE/src
    git clone https://github.com/sam-may/flashgg
    cd flashgg
    git checkout tth_dev_runII_v2

    cd ..
    source flashgg/setup_flashgg.sh

    # Build
    cd $CMSSW_BASE/src
    scram b -j 30
    cd ../../
  else
    cd ../../
  fi
fi


if [ ! -d logs ]; then
  mkdir logs
fi

pushd /home/users/sjmay/Librarian/ProjectMetis # or replace with your metis location
source setup.sh
popd

export PYTHONPATH=$PYTHONPATH:$CMSSW_BASE/src/flashgg/Taggers/test 
