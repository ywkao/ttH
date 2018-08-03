# flashgg babymaker
Makes flat ntuples from microAOD. Each ntuple contains a tree for (very loose) (1) ttH leptonic and (2) ttH hadronic preselections

# Instructions
1. `git clone https://github.com/sam-may/ttH ttH`
2. `cd BabyMaker`
3. For making flat ntuples from 2016 80X microAOD data/MC: `source setup.sh 80X`. Alternatively, for making flat ntuples from 2017 94X microAOD data/MC `source setup.sh 94X`
4. To run a local test job on a single file, `source test_run.sh 80X` or `source test_run.sh 94X`.
5. To submit full set of jobs to Condor, `python ducks.py <job_tag> <year>`. `<job_tag>` can be whatever you want, and will be appended to your output files. `<year>` should be either 2016 (for 80X) or 2017 (for 94X). The default option for `ducks.py` is to submit jobs only for the M125 ttH signal sample, data, and relevant background MC processes. You can launch jobs for ALL microAOD a la `python ducks.py <job_tag> <year> --do_all`
6. Once your jobs finish, merge the outputs so that loopers are faster: `python merge_ducks.py <job_tag> <year>`.
