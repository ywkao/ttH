# flashgg babymaker
Makes flat ntuples from microAOD. Each ntuple contains a tree for (very loose) (1) ttH leptonic and (2) ttH hadronic preselections

# Instructions
1. `git clone https://github.com/sam-may/ttH ttH`
2. `cd BabyMaker`
3. For making flat ntuples from 2016 80X microAOD data/MC: `source setup.sh 80X`. Alternatively, for making flat ntuples from 2017 94X microAOD data/MC `source setup.sh 94X`
4. To run a local test job on a single file, `source test_run.sh 80X` or `source test_run.sh 94X`.
5. To submit full set of jobs to Condor, `python ducks.py <job_tag> <year>`. `<job_tag>` can be whatever you want, and will be appended to your output files. `<year>` should be either 2016 (for 80X) or 2017 (for 94X). The default option for `ducks.py` is to submit jobs only for the M125 ttH signal sample, data, and relevant background MC processes. You can launch jobs for ALL microAOD a la `python ducks.py <job_tag> <year> --do_all`
6. Once your jobs finish, merge the outputs so that loopers are faster: `python merge_ducks.py <job_tag> <year>`.

# Development
1. For 2016 (80X) development, start from `tth_dev_80X` branch of `flashgg`. For 2017 (94X) development start from `tth_dev_94X` branch of `flashgg`.
2. To add branches to the flat ntuples, append to the dumper section in `Taggers/test/ttH_TagAndDump.py`. The dumper section calls member functions from the `ttHHadronicTag` or `ttHLeptonicTag` classes. If the variable you want to add is not already in the classes, you'll need to modify the class defintions at `DataFormats/interface/TTH*Tag.h` and rebuild `flashgg` with `scram b -j <n_cores>`. 
3. The actual analysis code is contained in `Taggers/plugins/TTH*TagProducer.cc`.

As an example, suppose you wanted to add gen-lepton information to the flat ntuples. First, you would modify the the class def file to contain a member variable and setter/getter functions (e.g. L34,61,94 of https://github.com/sam-may/flashgg/blob/tth_dev_80X/DataFormats/interface/TTHHadronicTag.h). Then, add your analysis code to `Taggers/plugins/TTH*TagProducer.cc` (e.g. L523-558, 643 of https://github.com/sam-may/flashgg/blob/tth_dev_80X/Taggers/plugins/TTHHadronicTagProducer.cc). Finally, modify the dumper to include this in the flat ntuples (e.g. L197 of https://github.com/sam-may/flashgg/blob/tth_dev_80X/Taggers/test/ttH_TagAndDump.py).  
