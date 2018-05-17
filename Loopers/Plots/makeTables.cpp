#include "PlotHelper.h"
#include <iomanip>

std::map<TString, TString> mLabels = {
        {"DY", "Drell-Yan"},
        {"DiPhoton", "#gamma#gamma + Jets"},
        {"GammaJets", "#gamma + Jets"},
        {"QCD", "QCD"},
        {"TTGG", "t#bar{t} + #gamma#gamma"},
        {"TTGJets", "t#bar{t}+#gamma+Jets"},
        {"VG", "V+#gamma"},
        {"WJets", "W+Jets"},
        {"TTJets", "t#bar{t} + Jets"}
};

std::map<TString, TString> mLatex = {
        {"DY", "Drell-Yan"},
        {"DiPhoton", "$\\gamma\\gamma$ + Jets"},
        {"GammaJets", "$\\gamma$ + Jets"},
        {"QCD", "QCD"},
        {"TTGG", "$t\\bar{t}+\\gamma\\gamma$"},
        {"TTGJets", "$t\\bar{t}+\\gamma$ + Jets"},
        {"VG", "V + $\\gamma$"},
        {"WJets", "W + Jets"},
        {"TTJets", "$t\\bar{t}$ + Jets"}
};

std::map<int, TString> mLeptons = {
        {0, "Hadronic"},
        {1, "$e$"},
        {2, "$\\mu$"},
        {3, "$e\\mu$"},
        {4, "$ee$"},
        {5, "$\\mu\\mu$"}
};

std::map<int, TString> mPhotons = {
        {0, "(Fake/Fake)"},
        {1, "(Fake/Prompt)"},
        {2, "(Prompt/Prompt)"}
};

std::map<int, TString> mPhotonsDetail = {
        {0, "(Fake/Fake)"},
	{1, "(Fake/Elec)"},
	{2, "(Elec/Elec)"},
        {3, "(Fake/Prompt)"},
	{4, "(Elec/Prompt)"},
        {5, "(Prompt/Prompt)"}
};

void make_table_std(TFile* file, TString hist_name, vector<TString> vBkgs, TString label) {
  TH1D* hData = (TH1D*)file->Get(hist_name + "_Data");
  TH1D* hSig = (TH1D*)file->Get(hist_name + "_ttH");
  TH1D* hBkg;
  vector<TH1D*> vHBkg;

  for (int i = 0; i < vBkgs.size(); i++) {
    vHBkg.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i]));
    if (i == 0)
      hBkg = (TH1D*)file->Get(hist_name + "_" + vBkgs[i])->Clone("hBkg");
    else
      hBkg->Add((TH1D*)file->Get(hist_name + "_" + vBkgs[i]));
  }

  int n_bins = hData->GetSize()-2;
  double yield_data_unc, yield_signal_unc, yield_bkg_unc;
  double yield_data = hData->IntegralAndError(0, n_bins+1, yield_data_unc);
  double yield_signal = hSig->IntegralAndError(0, n_bins+1, yield_signal_unc);
  double yield_bkg = hBkg->IntegralAndError(0, n_bins+1, yield_bkg_unc);
  vector<double> yield_mc;
  vector<double> yield_mc_unc;
  for (int i = 0; i < vHBkg.size(); i++) {
    yield_mc_unc.push_back(0);
    yield_mc.push_back(vHBkg[i]->IntegralAndError(0, n_bins+1, yield_mc_unc[i]));
  }
  cout.setf(ios::fixed);
  cout << std::setprecision(2) << endl;

  cout << endl << "Standard table: " << label << endl;
  cout << "\\begin{center} \\Fontvi" << endl;
  cout << "\\begin{tabular}{| r || r | r|} \\hline" << endl;
  cout << "Process & Yield & Frac. of total bkg. \\\\ \\hline" << endl;
  for (int i = 0; i < vBkgs.size(); i++) {
    cout << mLatex.find(vBkgs[i])->second << " & " << yield_mc[i] << " $\\pm$ " << yield_mc_unc[i] << " & " << yield_mc[i]/yield_bkg << " \\\\ ";
    if (i == vBkgs.size() - 1)
      cout << " \\hline" << endl;
    else
      cout << endl;
  }
  cout << "All bkg. & " << yield_bkg << " $\\pm$ " << yield_bkg_unc << " & " << yield_bkg/yield_bkg << " \\\\ \\hline" << endl;
  cout << "Data & " << yield_data << " $\\pm$ " << yield_data_unc << " & " << yield_data/yield_bkg << " \\\\ \\hline" << endl;
  cout << "\\end{tabular} \\end{center}" << endl;

  delete hData;
  delete hSig;
  delete hBkg;
  for (int i = 0; i < vHBkg.size(); i++) {
    delete vHBkg[i];
  }
}

void make_table_components(TFile* file, TString hist_name, vector<TString> vBkgs, TString label, std::map<int, TString> mMap, TString mapTitle) {
  vector<TH1D*> hBkgIncl;
  vector<TH1D*> hBkg;

  for (int i = 0; i < vBkgs.size(); i++) {
    hBkgIncl.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i]));
    for (int j = 0; j < mMap.size(); j++) {
      hBkg.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i] + mapTitle + "_" + to_string(j)));  
    }
  }

  int n_bins = hBkg[0]->GetSize()-2;
  vector<double> yield_bkg;
  vector<double> yield_bkg_unc;
  vector<double> yield_bkg_incl;
  vector<double> yield_bkg_incl_unc;

  for (int i = 0; i < vBkgs.size(); i++) {
    yield_bkg_incl_unc.push_back(0);
    yield_bkg_incl.push_back(hBkgIncl[i]->IntegralAndError(0, n_bins, yield_bkg_incl_unc[i]));
    for (int j = 0; j < mMap.size(); j++) {
      yield_bkg_unc.push_back(0);
      int idx = (i*mMap.size())+j;
      yield_bkg.push_back(hBkg[idx]->IntegralAndError(0, n_bins, yield_bkg_unc[idx]));
    }
  }

  cout.setf(ios::fixed);
  cout << std::setprecision(2) << endl;

  cout << endl << label << " " << mapTitle << " table: " << endl;
  cout << "\\begin{center} \\Fontvi" << endl;
  cout << "\\begin{tabular}{| c | r || r |} \\hline" << endl;
  cout << "Process & Component & Yield \\\\ \\hline" << endl;

  for (int i = 0; i < vBkgs.size(); i++) {
    for (int j = 0; j < mMap.size(); j++) {
      int idx = (i*mMap.size())+j;
      if (j == 0)
        cout << "\\multirow{" << mMap.size() + 1 << "}{*}{" << mLatex.find(vBkgs[i])->second << "}";
      cout << "& " << mMap.find(j)->second << " & " << yield_bkg[idx] << " $\\pm$ " << yield_bkg_unc[idx] << " \\\\";
      if (j == mMap.size() - 1)
        cout << " \\hline";
      cout << endl;
    }
    cout << " & All Components & " << yield_bkg_incl[i] << " $\\pm$ " << yield_bkg_incl_unc[i] << " \\\\ \\hline" << endl;
  }
  cout << "\\end{tabular} \\end{center}" << endl;  
}

int main(int argc, char* argv[])
{
  TFile* f1 = new TFile("../ttHHadronic_histograms.root");
  TFile* f2 = new TFile("../ttHLeptonic_histograms.root");
  TFile* f3 = new TFile("../ttHHadronicLoose_histograms.root");
  TFile* f4 = new TFile("../ttHLeptonicLoose_histograms.root");
  TFile* f5 = new TFile("../ttbar_cr_histograms.root");
  TFile* f6 = new TFile("../ttbar_cr_v2_histograms.root");

  vector<TFile*> vFiles = {f1, f2, f3, f4, f5, f6};
  vector<TString> vLabels = {"Hadronic", "Leptonic", "Hadronic Loose", "Leptonic Loose", "ttbar cr", "ttbar cr v2"};

  vector<TString> vBkgs = {"DiPhoton", "GammaJets", "QCD", "TTGG", "TTGJets", "TTJets", "VG", "WJets", "DY"};

  for (int i = 0; i < vFiles.size(); i++) {
    make_table_std(vFiles[i], "hNVtx", vBkgs, vLabels[i]);
    make_table_components(vFiles[i], "hNVtx", vBkgs, vLabels[i], mLeptons, "GenLepton");
    make_table_components(vFiles[i], "hNVtx", vBkgs, vLabels[i], mPhotons, "GenPhoton");
  }
}

