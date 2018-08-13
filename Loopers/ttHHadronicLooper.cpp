#include "ScanChain_ttHHadronic.C"

int main(int argc, char* argv[]) {
  TString tag = argc <= 1 ? "ttHHadronicLoose" : argv[1];
  cout << "Running ttH Hadronic Looper for tag: " << tag << endl;

  TString year = argc <= 2 ? "2016" : argv[2]; 
  cout << "Running for year: " << year << endl;

  TChain *ch = new TChain("tthHadronicTagDumper/trees/tth_13TeV_all"); 
  add_samples(ch, year);

  ScanChain(ch, tag, year); 
}
