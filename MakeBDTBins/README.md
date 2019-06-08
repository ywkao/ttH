# how to use this code

- filenames are stored in filenameDict.py

- makeSigBkgShape.py makes signal and background models
  - basic usage: python makeSigBkgShape.py -l lowCutOnBDT --hi highCutOnBDT -i index --tag TTHHadronicTag --modelPath dirToSaveModels --savepath dirToSavePlots --doData -p ggH_hgg --skipBkg
	- tag can be: TTHHardonicTag*, TTHLeptonicTag*, FCNCHadronicTag*, FCNCLeptonicTag*
	- if using --doData, the code will determine background model from data sideband, else using MC
	- -p is to sepecify which signal process to run on, could be ggH_hgg/VBF_hgg/VH_hgg/ttH_hgg/TT_FCNC_hut/TT_FCNC_hct/ST_FCNC_hut/ST_FCNC_hct
	- if using --skipBkg, then only signal model will be made

- makeModels.sh makes models for finalFit for given BDT bins, see options at the beginning of script
  - this part is mainly for final result, e.g. 3 bins for TTHHadronic or 2 bins for FCNCHadronic, currently has a lot of hardcode, probably fine

- cardMaker makes datacards for ttH/FCNC analysis
  - sig/bkg shape roofiles and modelnames consistent with what is produced in makeSigBkgShape.py
  - have options to include one or multiple signals for datacard for FCNC study
  - currently doesn't have systematic uncertainties built in datacard
  
- doScan.py take same input file as makeSigBkgShape.py, from filenameDict.py
  - start with some range of events depending on BDT cuts
  - there are many hardcode in doScan_oneCore function, needs improvement; one reason is makeSigBkgShape.py works under CMSSW_9_4_9, while combine in doScan.py works under CMSSW_7_4_7
