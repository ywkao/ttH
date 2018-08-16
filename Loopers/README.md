# ttH Hadronic and Leptonic Loopers

### Remaking class files
If you add new branches to the flat ntuples, you'll need to update the CMS3 class files:
1. Launch `root`, then `.L makeCMS3ClassFiles.C++`.
2. For ttHLeptonic `makeCMS3ClassFiles(<input_ntuple>, "tthLeptonicTagDumper/trees/tth_13TeV_all", "ttHLeptonic")`
3. For ttHHadronic `makeCMS3ClassFiles(<input_ntuple>, "tthHadronicTagDumper/trees/tth_13TeV_all", "ttHHadronic")`
Note: This is a slightly modified version of `makeCMS3ClassFiles.C` that checks for duplicate branches in the target root file (flashgg dumpers store `nvtx` twice for some reason)

### Running looper
1. `source setup.sh ; make`
2. Specify the tag of babies that you want to run on in `ttHLooper.h` (https://github.com/sam-may/ttH/blob/master/Loopers/ttHLooper.h#L287) and in `scale1fb/scale1fb_*.` (https://github.com/sam-may/ttH/blob/master/Loopers/scale1fb/scale1fb_2016.h#L40). There is probably a smarter way to do this :)
3. To loop through events and fill histograms, run `./ttHHadronicLooper <selection> <year>` or `./ttHLeptonicLooper <selection> <year>`. The supported options for `<selection>` are listed in `ScanChain_ttHHadronic.C` and `ScanChain_ttHLeptonic.C` (you can add your own selections, too). `<year>` should be either 2016 or 2017.

### Making Plots/Tables
1. `source setup.sh ; cd Plots ; make`
2. To make data/MC plots: `./makePlots <plot_type> <input_file>`. The supported options for `<plot_type>` are "std" (makes regular data/MC plots), "shape" (makes normalized signal/MC shape comparisons), "genPhoton" (makes signal/MC plots with MC split by gen photon type), and "genLepton" (makes signal/MC plots with MC split by gen lepton type). You can change the list of background MC that are plotted by changing `vBkgs` in `makePlots.cpp`.
3. To make data/signal/MC tables: `./makeTables <input_file>`. This will make yield tables for data/signal/background MC including inclusive yields for each sample, yields split by gen photon type, gen lepton type, and diphoton pair location.

### Training BDTs + Evaluating Performance
1. `source setup.sh ; make`
2. The full BDT workflow can be run with `python run_bdt_workflow.py <channel> <selection> <year> <training_tag>` This is a wrapper to the following steps (shown below for Hadronic)
    1. Make root baby with all needed branches: `./ttHHadronicMVABabyMaker Hadronic <selection> <year>`
    2. Convert `.root` file to `.hdf5` file for `xgboost`: `cd ../MVAs ; python prep.py Hadronic`
    3. Train BDT: `python train.py Hadronic <training_tag>`
    4. Evaluate BDT, fill histograms, and create `.root` file for optimization studies: `cd ../Loopers ; ./ttHHadronicLooper Hadronic <selection> <year> <xml_file>`
    5. Evaluate significance based on BDT: `cd Optmization ; python estimate_significance.py <optimization_baby>`
3. To add a variable to the BDT: `python add_bdt_variable.py <var_name> <var_type> <var_definition> <channel>`
    * Note: if you try adding a variable twice, the script is not yet smart enough to recognize and protect against redeclaration of variables, etc.
4. To remove a variable from the BDT: `python add_bdt_variable.py <var_name> <var_type> <var_definition> <channel> --remove`

