# ttH Hadronic and Leptonic Loopers

### Remaking class files
1. Launch `root`, then `.L makeCMS3ClassFiles.C++`.
2. For ttHLeptonic `makeCMS3ClassFiles("/hadoop/cms/store/user/smay/ttH/ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8__ttH_Babies_v1/merged_ntuple_1.root", "tthLeptonicTagDumper/trees/tth_13TeV_all", "ttHLeptonic")`
3. For ttHHadronic `makeCMS3ClassFiles("/hadoop/cms/store/user/smay/ttH/ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8__ttH_Babies_v1/merged_ntuple_1.root", "tthHadronicTagDumper/trees/tth_13TeV_all", "ttHHadronic")`
Note: This is a slightly modified version of `makeCMS3ClassFiles.C` that checks for duplicate branches in the target root file (flashgg dumpers store `nvtx` twice for some reason)
