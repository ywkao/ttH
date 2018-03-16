# Set up CMMSW 80X
cmsrel CMSSW_8_0_28
cd CMSSW_8_0_28/src
cmsenv
git cms-init

# Set up flashgg
cd $CMSSW_BASE/src
#git clone https://github.com/sam-may/flashgg 

# flashgg does not build with most recent commit, so skipping using the fork that I made for now and instead using the original flashg, reverting to a commit that does build successfully, and then manually copying over scripts for creating flat ntuples

git clone https://github.com/cms-analysis/flashgg flashgg
cd flashgg
git reset --hard 074d2fa8fbdde8e58e422f80d559d76bb699355b 
cd ..
source flashgg/setup_8_0_X.sh

# Build
cd $CMSSW_BASE/src
scram b -j 20

# Copy over scripts
cp scripts/ttH_Tag.py CMSSW_8_0_28/src/flashgg/Taggers/test/
cp scripts/ttHTagsDumper_cfg.py CMSSW_8_0_28/src/flashgg/Validation/test/
