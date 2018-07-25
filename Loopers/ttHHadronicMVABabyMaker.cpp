#include "MakeMVABabies_ttHHadronic.C"

int main(int argc, char* argv[]) {
  TChain *ch = new TChain("tthHadronicTagDumper/trees/tth_13TeV_all"); 
  add_samples(ch);

  TString tag = "ttHHadronicLoose";
  if (argc >=1 )
    tag = argv[1];
  cout << tag << endl;

  BabyMaker *looper = new BabyMaker();
  looper->ScanChain(ch, tag);
  return 0; 
}