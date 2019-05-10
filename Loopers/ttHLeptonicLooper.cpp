#include "ScanChain_ttHLeptonic.C"

int main(int argc, char* argv[]) {
  TString tag = argc <= 1 ? "ttHLeptonicLoose" : argv[1];
  cout << "Running ttH Leptonic Looper for tag: " << tag << endl;

  TString year = argc <= 2 ? "2016" : argv[2]; 
  cout << "Running for year: " << year << endl;

  TString ext = argc <= 3 ? "" : argv[3];
  cout << "Saving output files with tag: " << ext << endl;

  TString xml_file = argc <= 4 ? "none" : argv[4];
  if (xml_file != "none")
    cout << "Evaluating following BDT: " << xml_file << endl;
  else
    cout << "Not evaluating BDT" << endl;

  TString bkg_options = argc <= 5 ? "none" : argv[5];
  if (bkg_options == "none")
    cout << "No MC scaling/bkg imputing applied to bkg" << endl;
  else if (bkg_options == "scale_mc")
    cout << "Scaling QCD/GammaJets/DiPhoton MC" << endl;
  else if (bkg_options == "impute")
    cout << "Using data-driven QCD/GammaJets description. Scaling normalization along with DiPhoton MC" << endl;
  else if (bkg_options == "impute_no_scale")
    cout << "Using data-driven QCD/GammaJets description. NOT scaling normalization" << endl;
  else if (bkg_options == "old_vgamma")
    cout << "Using DY sample for PF/PP events (instead of V+gamma sample)" << endl;
  else
    cout << "Did not recognize background treatment option" << endl;

  TChain *ch = new TChain("tthLeptonicTagDumper/trees/tth_13TeV_all"); 

  TString file = argc <= 6 ? "all" : argv[6];
  TString mYear = argc <= 7 ? "" : argv[7];
  TString idx = argc <= 8 ? "" : argv[8];

  if (file == "all") {
    if (year.Contains("RunII")) {
      if (year == "RunII") {
	add_samples(ch, "2016_RunII");
	add_samples(ch, "2017_RunII");
	add_samples(ch, "2018_RunII");
      }
      else {
	if (year.Contains("2016"))
	  add_samples(ch, "2016_RunII");
	if (year.Contains("2017"))
	  add_samples(ch, "2017_RunII");
	if (year.Contains("2018"))
	  add_samples(ch, "2018_RunII");
      }
    }

    else if (year == "All") {
      add_samples(ch, "2016");
      add_samples(ch, "2017");
    }
    else
      add_samples(ch, year);
  }

  else
    ch->Add(file);

  ScanChain(ch, tag, year, ext, xml_file, bkg_options, mYear, idx);
}
