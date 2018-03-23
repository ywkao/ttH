#include "ScanChain_ttHLeptonic.C"

int main(int argc, char* argv[]) {
  TChain *ch = new TChain("tthLeptonicTagDumper/trees/tth_13TeV_all"); 
  add_samples(ch);
  ScanChain(ch, "ttHLeptonic_histograms.root"); 
}
