# ttH Hadronic and Leptonic Loopers

### Remaking class files
If you add new branches to the flat ntuples, you'll need to update the CMS3 class files:
1. Launch `root`, then `.L makeCMS3ClassFiles.C++`.
2. For ttHLeptonic `makeCMS3ClassFiles(<input_ntuple>, "ttHLeptonic")`
3. For ttHHadronic `makeCMS3ClassFiles(<input_ntuple>, "ttHHadronic")`
Note: This is a slightly modified version of `makeCMS3ClassFiles.C` that checks for duplicate branches in the target root file (flashgg dumpers store `nvtx` twice for some reason)
