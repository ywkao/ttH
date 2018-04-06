#include "ScanChain_ttHLeptonic.C"

int main(int argc, char* argv[]) {
  TChain *ch = new TChain("tthLeptonicTagDumper/trees/tth_13TeV_all"); 
  add_samples(ch);
  
  TString tag = "ttHLeptonicLoose";
  if (argc >=1 )
    tag = argv[1];
  cout << tag << endl;
  ScanChain(ch, tag); 
}
