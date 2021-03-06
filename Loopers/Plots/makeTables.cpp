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
	{"GammaJets_Madgraph", "$\\gamma$ + Jets (MadGraph)"},
	{"QCD_GammaJets_imputed", "($\\gamma$) + Jets (Data)"},
        {"QCD", "QCD"},
        {"TTGG", "$t\\bar{t}+\\gamma\\gamma$"},
        {"TTGJets", "$t\\bar{t}+\\gamma$ + Jets"},
        {"VG", "V + $\\gamma$"},
        {"WJets", "W + Jets"},
        {"TTJets", "$t\\bar{t}$ + Jets"},
	{"ttH", "ttH"},
	{"Data", "Data"},
	{"THQ", "tHq"},
	{"THW", "tHW"},
	{"ggH", "ggH"},
	{"VBF", "VBF"},
	{"VH", "VH"},
	{"TGamma", "$t + \\gamma$"},
	{"TTV", "$t\\bar{t}+V$"},
	{"VV", "$VV$"},
	{"tV", "$tV$"},
	{"ttH", "$t\\bar{t}H$"},
        {"TT_FCNC_hut", "$t\\bar{t}$ FCNC (Hut)"},
        {"TT_FCNC_hct", "$t\\bar{t}$ FCNC (Hct)"},
        {"ST_FCNC_hut", "$t$ FCNC (Hut)"},
        {"ST_FCNC_hct", "$t$ FCNC (Hct)"}
};

std::map<int, TString> mLeptons = {
        {0, "Hadronic"},
        {1, "$e$"},
        {2, "$\\mu$"},
        {3, "$e\\mu$"},
        {4, "$ee$"},
        {5, "$\\mu\\mu$"}
};

std::map<int, TString> mRecoLeptons = {
        {0, "Hadronic"},
        {1, "$e$"},
        {2, "$\\mu$"},
        {3, "$e\\mu$"},
        {4, "$ee$"},
        {5, "$\\mu\\mu$"},
        {6, "Multi Lepton"}
};

std::map<int, TString> mYears = {
	{0, "2016"},
	{1, "2017"},
	{2, "2018"}
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

std::map<int, TString> mPhotonLocations = {
	{0, "(Barrel/Barrel)"},
	{1, "(Barrel/Endcap)"},
	{2, "(Endcap/Endcap)"}
};

void make_table_std(TFile* file, TString hist_name, vector<TString> vBkgs, vector<TString> vSigs, TString label, TString ext = "") {
  TH1D* hData = (TH1D*)file->Get(hist_name + "_Data" + ext);
  TH1D* hBkg;
  vector<TH1D*> vHBkg;
  vector<TH1D*> vHSig;

  for (int i = 0; i < vBkgs.size(); i++) {
    vHBkg.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i] + ext));
    if (i == 0)
      hBkg = (TH1D*)file->Get(hist_name + "_" + vBkgs[i] + ext)->Clone("hBkg");
    else
      hBkg->Add((TH1D*)file->Get(hist_name + "_" + vBkgs[i] + ext));
  }

  for (int i = 0; i < vSigs.size(); i++) 
    vHSig.push_back((TH1D*)file->Get(hist_name + "_" + vSigs[i] + ext));

  int n_bins = hData->GetSize()-2;
  double yield_data_unc, yield_signal_unc, yield_bkg_unc;
  double yield_data = hData->IntegralAndError(0, n_bins+1, yield_data_unc);
  double yield_bkg = hBkg->IntegralAndError(0, n_bins+1, yield_bkg_unc);
  vector<double> yield_mc;
  vector<double> yield_mc_unc;
  for (int i = 0; i < vHBkg.size(); i++) {
    yield_mc_unc.push_back(0);
    yield_mc.push_back(vHBkg[i]->IntegralAndError(0, n_bins+1, yield_mc_unc[i]));
  }
  vector<double> yield_sig;
  vector<double> yield_sig_unc;
  for (int i = 0; i < vHSig.size(); i++) {
    yield_sig_unc.push_back(0);
    yield_sig.push_back(vHSig[i]->IntegralAndError(0, n_bins+1, yield_sig_unc[i]));
  }

  cout.setf(ios::fixed);
  cout << std::setprecision(2) << endl;

  cout << endl << "Standard table: " << label << endl;
  cout << "\\begin{center} \\Fontvi" << endl;
  cout << "\\begin{tabular}{| r || r | r|} \\hline" << endl;
  cout << "Process & Yield & $\\mathcal F$ of bkg \\\\ \\hline" << endl;
  for (int i = 0; i < vBkgs.size(); i++) {
    cout << mLatex.find(vBkgs[i])->second << " & " << yield_mc[i] << " $\\pm$ " << yield_mc_unc[i] << " & " << yield_mc[i]/yield_bkg << " \\\\ ";
    if (i == vBkgs.size() - 1)
      cout << " \\hline" << endl;
    else
      cout << endl;
  }
  cout << "All bkg. & " << yield_bkg << " $\\pm$ " << yield_bkg_unc << " & " << yield_bkg/yield_bkg << " \\\\ \\hline" << endl;
  cout << "Data & " << yield_data << " $\\pm$ " << yield_data_unc << " & " << yield_data/yield_bkg << " \\\\ \\hline \\hline" << endl;
  for (int i = 0; i < vSigs.size(); i++) {
    cout << mLatex.find(vSigs[i])->second << " & " << yield_sig[i] << " $\\pm$ " << yield_sig_unc[i] << " & " << yield_sig[i]/yield_bkg << " \\\\ ";
    if (i == vSigs.size() - 1)
      cout << " \\hline" << endl;
    else
      cout << endl;
  }

  cout << "\\end{tabular} \\end{center}" << endl;

  delete hData;
  //delete hSig;
  delete hBkg;
  for (int i = 0; i < vHBkg.size(); i++) {
    delete vHBkg[i];
  }
  for (int i = 0; i < vHSig.size(); i++) {
    delete vHSig[i];
  } 
}

void make_table_multiple(vector<TFile*> vF, TString hist_name, vector<TString> vBkgs, vector<TString> vSigs, vector<TString> vLabels) {  
  vector<TH1D*> hData;
  vector<TH1D*> hBkg;
  vector<vector<TH1D*>> vHBkg;
  vector<vector<TH1D*>> vHSig;

  for (int i = 0; i < vF.size(); i++) {
    hData.push_back((TH1D*)vF[i]->Get(hist_name + "_Data"));
    vector<TH1D*> vHTemp;
    for (int j = 0; j < vBkgs.size(); j++) {
      vHTemp.push_back((TH1D*)vF[i]->Get(hist_name + "_" + vBkgs[j]));
      if (j == 0) { 
	TString name = "hBkg" + to_string(i);
        TH1D* hTemp = (TH1D*)vF[i]->Get(hist_name + "_" + vBkgs[j])->Clone(name); 
	hBkg.push_back(hTemp);
      }
      else
	hBkg[i]->Add((TH1D*)vF[i]->Get(hist_name + "_" + vBkgs[j]));
    }
    vHBkg.push_back(vHTemp);
    vHTemp.clear();
    for (int j = 0; j < vSigs.size(); j++) {
      vHTemp.push_back((TH1D*)vF[i]->Get(hist_name + "_" + vSigs[j]));
    }
    vHSig.push_back(vHTemp);
  }

  vector<int> n_bins(vF.size());
  vector<double> yield_data_unc(vF.size());
  vector<double> yield_signal_unc(vF.size());
  vector<double> yield_bkg_unc(vF.size());
  vector<double> yield_data(vF.size());
  vector<double> yield_bkg(vF.size());
  vector<vector<double>> yield_mc(vF.size(), vector<double>(vBkgs.size()));
  vector<vector<double>> yield_mc_unc(vF.size(), vector<double>(vBkgs.size()));
  vector<vector<double>> yield_sig(vF.size(), vector<double>(vSigs.size()));
  vector<vector<double>> yield_sig_unc(vF.size(), vector<double>(vSigs.size()));

  for (int i = 0; i < vF.size(); i++) {
    n_bins[i] = hData[i]->GetSize()-2;
    yield_data[i] = hData[i]->IntegralAndError(0, n_bins[i]+1, yield_data_unc[i]);
    yield_bkg[i] = hBkg[i]->IntegralAndError(0, n_bins[i]+1, yield_bkg_unc[i]);
    for (int j = 0; j < vBkgs.size(); j++) {
      yield_mc[i][j] = vHBkg[i][j]->IntegralAndError(0, n_bins[i]+1, yield_mc_unc[i][j]);
    }
    for (int j = 0; j < vSigs.size(); j++) {
      yield_sig[i][j] = vHSig[i][j]->IntegralAndError(0, n_bins[i]+1, yield_sig_unc[i][j]);
    }
  }

  cout.setf(ios::fixed);
  cout << std::setprecision(2) << endl;

  cout << "\\begin{center} \\Fontvi" << endl;
  cout << "\\begin{tabular}{| r ||";
  for (int i = 0; i < vF.size(); i++)
    cout << " r | r | r ||";
  cout << "} \\hline" << endl;
  cout << "\\multirow{2}{*}{Process}";
  for (int i = 0; i < vF.size(); i++)
    cout << "& \\multicolumn{3}{c||}{" << vLabels[i] << "}";
  cout << " \\\\ \\cline{2-" << (vF.size()*3) + 1 << "}" << endl;
  for (int i = 0; i < vF.size(); i++)
    cout << "& Yield & $\\mathcal F$ of bkg. & $\\epsilon$"; 
  cout << "\\\\ \\hline \\hline" << endl;
  for (int i = 0; i < vBkgs.size(); i++) {
    cout << mLatex.find(vBkgs[i])->second;
    for (int j = 0; j < vF.size(); j++)
      cout << " & " << yield_mc[j][i] << " $\\pm$ " << yield_mc_unc[j][i] << " & " << yield_mc[j][i]/yield_bkg[j] << " & " << yield_mc[j][i] / yield_mc[0][i];
    cout << " \\\\ ";
    if (i == vBkgs.size() - 1)
      cout << " \\hline" << endl;
    else
      cout << endl;
  }

  cout << "All bkg.";
  for (int i = 0; i < vF.size(); i++)
    cout << " & " << yield_bkg[i]  << " $\\pm$ " << yield_bkg_unc[i] << " & " << yield_bkg[i]/yield_bkg[i] << " & " << yield_bkg[i]/yield_bkg[0];
  cout << " \\\\ \\hline" << endl;

  cout << "Data";
  for (int i = 0; i < vF.size(); i++)
    cout << " & " << yield_data[i]  << " $\\pm$ " << yield_data_unc[i] << " & " << yield_data[i]/yield_bkg[i] << " & " << yield_data[i]/yield_data[0];
  cout << " \\\\ \\hline" << endl;
  
  for (int i = 0; i < vSigs.size(); i++) {
    cout << mLatex.find(vSigs[i])->second;
    for (int j = 0; j < vF.size(); j++)
      cout << " & " << yield_sig[j][i] << " $\\pm$ " << yield_sig_unc[j][i] << " & " << yield_sig[j][i]/yield_bkg[j] << " & " << yield_sig[j][i] / yield_sig[0][i];
    cout << " \\\\ ";
    if (i == vSigs.size() - 1)
      cout << " \\hline" << endl;
    else
      cout << endl;
  }

  cout << " \\\\ \\hline" << endl;      
  cout << "\\end{tabular} \\end{center}" << endl;

  for (int i = 0; i < vF.size(); i++) {
    delete hData[i];
    //delete hSig[i];
    delete hBkg[i];
    for (int j = 0; j < vHBkg[i].size(); j++) 
      delete vHBkg[i][j];
    for (int j = 0; j < vHSig[i].size(); j++)
      delete vHSig[i][j];
  }
  
} 


double unc(double quantity, double value1, double unc1, double value2, double unc2) {
  return quantity * sqrt( pow(unc1/value1, 2) + pow(unc2/value2, 2));
}

void make_table_components(TFile* file, TString hist_name, vector<TString> vBkgs, vector<TString> vSigs, TString label, std::map<int, TString> mMap, TString mapTitle, bool include_data = false, bool verbose = false) {
  vector<TH1D*> hBkgIncl;
  vector<TH1D*> hBkg;

  vBkgs.push_back("ttH");
  if (include_data) vBkgs.push_back("Data");
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
    yield_bkg_incl.push_back(hBkgIncl[i]->IntegralAndError(0, n_bins+1, yield_bkg_incl_unc[i]));
    for (int j = 0; j < mMap.size(); j++) {
      yield_bkg_unc.push_back(0);
      int idx = (i*mMap.size())+j;
      yield_bkg.push_back(hBkg[idx]->IntegralAndError(0, n_bins+1, yield_bkg_unc[idx]));
    }
  }

  cout.setf(ios::fixed);
  cout << std::setprecision(2) << endl;


  if (!verbose) {
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
	  cout << " \\cline{2-3}";
	cout << endl;
      }
      cout << " & All Components & " << yield_bkg_incl[i] << " $\\pm$ " << yield_bkg_incl_unc[i] << " \\\\ \\hline" << endl;
    }
    cout << "\\end{tabular} \\end{center}" << endl; 
  }
  else {
    cout << endl << label << " " << mapTitle << " table: " << endl;
    cout << "\\begin{center} \\Fontvi" << endl;
    cout << "\\begin{tabular}{| c | r || r | r| r|} \\hline" << endl;
    cout << "Process & Component & Yield & Frac. of Total Sample & Fractional Error\\\\ \\hline" << endl;

    for (int i = 0; i < vBkgs.size(); i++) {
      for (int j = 0; j < mMap.size(); j++) {
        int idx = (i*mMap.size())+j;
        if (j == 0)
          cout << "\\multirow{" << mMap.size() + 1 << "}{*}{" << mLatex.find(vBkgs[i])->second << "}";
        cout << "& " << mMap.find(j)->second << " & " << yield_bkg[idx] << " $\\pm$ " << yield_bkg_unc[idx] << " & " << yield_bkg[idx] / yield_bkg_incl[i] << " $\\pm$ " << unc(yield_bkg[idx] / yield_bkg_incl[i], yield_bkg[idx], yield_bkg_unc[idx], yield_bkg_incl[i], yield_bkg_incl_unc[i]) << " & " << yield_bkg_unc[idx] / yield_bkg[idx] << " \\\\";
        if (j == mMap.size() - 1)
          cout << " \\cline{2-5}";
        cout << endl;
      }
      cout << " & All Components & " << yield_bkg_incl[i] << " $\\pm$ " << yield_bkg_incl_unc[i] << " & 1.00 $\\pm$ 0.00 & " << yield_bkg_incl_unc[i] / yield_bkg_incl[i]  << " \\\\ \\hline" << endl;
    }
    cout << "\\end{tabular} \\end{center}" << endl;

  }
 
}

void make_table_vetos(TFile* file, TString process) {
  vector<TH1D*> vStart;
  vector<TH1D*> vPassEVeto;
  vector<TH1D*> vPassPSV;
  vector<TH1D*> vPassBoth;

  vector<TString> vHistNames = {"hNVtx", "hPhotonMinIDMVA_passEVeto", "hPhotonMinIDMVA_passPSV", "hPhotonMinIDMVA_passBothVeto"};
  vector<TString> vLabels = {"No Vetos Applied", "Conversion-Safe $e$ Veto", "Pixel Seed Veto", "Both Vetos"};
  vector<vector<TH1D*>> vVH = {vStart, vPassEVeto, vPassPSV, vPassBoth};

  for (int i = 0; i < vVH.size(); i++) {
    vVH[i].push_back((TH1D*)file->Get(vHistNames[i] + "_" + process + "PhotonLocations_0")); // barrel-barrel
    vVH[i].push_back((TH1D*)file->Get(vHistNames[i] + "_" + process + "PhotonLocations_1")); // barrel-endcap
    vVH[i].push_back((TH1D*)file->Get(vHistNames[i] + "_" + process + "PhotonLocations_2")); // endcap-endcap
    vVH[i].push_back((TH1D*)file->Get(vHistNames[i] + "_" + process)); // inclusive
  }

  cout.setf(ios::fixed);
  cout << std::setprecision(2) << endl;

  cout << "\\begin{center} \\Fontvi" << endl;
  cout << "\\begin{tabular}{|r|r|r|r|r|r|} \\hline" << endl;
  cout << "\\multirow{2}{*}{Vetos Applied} & \\multicolumn{4}{|c|}{Yield} & \\multirow{2}{*}{Efficiency} \\\\ \\cline{2-5}" << endl;
  cout << " & Barrel-Barrel & Barrel-Endcap & Endcap-Endcap & Total & \\\\ \\hline" << endl;

  for (int i = 0; i < vVH.size(); i++) {
    cout << vLabels[i];
    for (int j = 0; j < vVH[i].size(); j++) {
      double yield(0), unc(0);
      int n_bins = vVH[i][j]->GetSize()-2;  
      yield = vVH[i][j]->IntegralAndError(0, n_bins+1, unc);
      cout << " & " << yield << " $\\pm$ " << unc; //<< " & " << yield / vVH[i][vVH[i].size()-1]->Integral(0, n_bins+1) << " \\\\";
    } 
    cout << " & " << vVH[i][vVH[i].size()-1]->Integral(0, vVH[i][vVH[i].size()-1]->GetSize()-1) / vVH[0][vVH[0].size()-1]->Integral(0, vVH[0][vVH[i].size()-1]->GetSize()-1) << " \\\\ "; 
    if (i == vVH.size() - 1)
      cout << " \\hline";
    cout << endl;
  }
  cout << "\\end{tabular} \\end{center}" << endl; 
}

vector<TString> tokenize(TString input, TString delimiter) {
    vector<TString> v;
    if (!input.Contains(delimiter)) {
        v.push_back(input);
        return v;    
    }
    TObjArray *t = input.Tokenize(delimiter);
    for (int i = 0; i < t->GetEntries(); i++)
        v.push_back(((TObjString *)(t->At(i)))->String());
    return v;
}

int main(int argc, char* argv[])
{

  if (argc < 2) {
    cout << "Please provide one argument: input file (e.g. '../ttHHadronicLoose_histograms.root')" << endl;
    return 0;
  }

  vector<TString> files = tokenize(argv[1], "|"); 

  vector<TString> signals = {"ttH"};
  vector<TString> backgrounds = {"DiPhoton", "GammaJets", "QCD", "QCD_GammaJets_imputed", "TTGG", "TTGJets", "TTJets", "DY", "THQ", "THW", "TGamma", "VG", "ggH", "VH", "VBF", "GammaJets_Madgraph", "TTW", "TTZ", "VV", "tV"};
  if (argc > 2) {
      signals = tokenize(argv[2], "|");
  }
  if (argc > 3) {
      backgrounds = tokenize(argv[3], "|");
  }

  if (files.size() == 1) {
    TFile* f = new TFile(files[0]);
    TString label = (files[0].ReplaceAll("../", "")).ReplaceAll(".root", "");
    make_table_std(f, "hNVtx", backgrounds, signals, label);
    make_table_std(f, "hNVtx", backgrounds, signals, label, "Year_0");
    make_table_std(f, "hNVtx", backgrounds, signals, label, "Year_1");
    make_table_std(f, "hNVtx", backgrounds, signals, label, "Year_2");
    //make_table_components(f, "hNVtx", backgrounds, signals, label, mYears, "Year");
    //make_table_components(f, "hNVtx", backgrounds, signals, label, mLeptons, "GenLepton");
    //make_table_components(f, "hNVtx", backgrounds, signals, label, mPhotons, "GenPhoton"); 
    //make_table_components(f, "hNVtx", backgrounds, signals, label, mPhotonsDetail, "GenPhotonDetail");
    //make_table_components(f, "hNVtx", backgrounds, signals, label, mPhotonLocations, "PhotonLocations", true);
    //backgrounds.push_back("Data");
    //make_table_components(f, "hNVtx", backgrounds, signals, label, mRecoLeptons, "RecoLepton");
  } 

  else if (files.size() > 1) {
    vector<TFile*> vF;
    vector<TString> vLabels;
    for (int i = 0; i < files.size(); i++) {
      vF.push_back(new TFile(files[i]));
      vLabels.push_back((files[i].ReplaceAll("../", "")).ReplaceAll(".root", ""));
    }
    make_table_multiple(vF, "hNVtx", backgrounds, signals, vLabels); 
  }

}

