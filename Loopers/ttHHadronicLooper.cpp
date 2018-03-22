#include "ScanChain_ttHHadronic.C"

int main(int argc, char* argv[]) {
  TChain *ch = new TChain("tthHadronicTagDumper/trees/tth_13TeV_all"); 
  ch->Add("/hadoop/cms/store/user/smay/ttH/ttHJetToGG_M100_13TeV_amcatnloFXFX_madspin_pythia8__ttH_Babies_v1/merged_ntuple_1.root");
  ScanChain(ch, "sample_output.root"); 
}
