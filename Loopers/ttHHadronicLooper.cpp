#include "ScanChain_ttHHadronic.C"

int main(int argc, char* argv[]) {
  TChain *ch = new TChain("tthHadronicTagDumper/trees/tth_13TeV_all"); 
  add_samples(ch);
  ScanChain(ch, "ttHHadronic_histograms.root"); 
}
