#include "ScanChain_ttHLeptonic.C"

int main(int argc, char* argv[]) {
  TString tag = argc <= 1 ? "ttHLeptonicLoose" : argv[1];
  cout << "Running ttH Leptonic Looper for tag: " << tag << endl;

  TString year = argc <= 2 ? "2016" : argv[2]; 
  cout << "Running for year: " << year << endl;

  TChain *ch = new TChain("tthLeptonicTagDumper/trees/tth_13TeV_all"); 
  add_samples(ch, year);

  ScanChain(ch, tag, year); 
}
