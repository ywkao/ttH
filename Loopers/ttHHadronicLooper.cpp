
#include "ScanChain_ttHHadronic.C"

int main(int argc, char* argv[]) {
  TString tag = argc <= 1 ? "ttHHadronicLoose" : argv[1];
  cout << "Running ttH Hadronic Looper for tag: " << tag << endl;

  TString year = argc <= 2 ? "2016" : argv[2]; 
  cout << "Running for year: " << year << endl;

  TString ext = argc <= 3 ? "" : argv[3];
  cout << "Saving output files with tag: " << ext << endl;

  TString xml_file = argc <= 4 ? "none" : argv[4];
  if (xml_file != "none")
    cout << "Evaluating following BDT: " << xml_file << endl;
  else
    cout << "Not evaluating BDT" << endl;

  TChain *ch = new TChain("tthHadronicTagDumper/trees/tth_13TeV_all"); 
  if (year == "All") {
    add_samples(ch, "2016");
    add_samples(ch, "2017");
  }
  else
    add_samples(ch, year);

  ScanChain(ch, tag, year, ext, xml_file); 
}
