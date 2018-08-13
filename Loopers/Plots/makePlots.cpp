#include "PlotHelper.h"
#include <iomanip>

// histogram index to sample
// 0: ttH
// 1: Drell-Yan
// 2: Gamma Gamma + Jets
// 3: Gamma + Jets
// 4: QCD
// 5: TT + Gamma Gamma 
// 6: TT + Gamma + Jets
// 7: V + Gamma
// 8: W + Jets
// 9: TT + Jets

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

std::map<TString, int> mColors = {
        {"DY", kRed - 7},
        {"DiPhoton", kAzure+1},
        {"GammaJets", kViolet -4},
        {"QCD", kCyan-7},
        {"TTGG", kOrange+1},
        {"TTGJets", kTeal+10},
        {"VG", kGreen+3},
        {"WJets", kBlue+2},
        {"TTJets", kPink-2}
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


const int nGenPhotonCats = 3;
const int nGenLeptonCats = 6;

std::map<int, TString> mPhotons = {
	{0, "(F/F)"},
	{1, "(F/P)"},
	{2, "(P/P)"}
};

std::map<int, TString> mLeptons = {
	{0, "(Had.)"},
	{1, "(e)"},
	{2, "(#mu)"},
	{3, "(e#mu)"},
	{4, "(ee)"},
	{5, "(#mu#mu)"}
};

std::map<int, TString> mLeptonsLatex = {
        {0, "Hadronic"},
        {1, "$e$"},
        {2, "$\\mu$"},
        {3, "$e\\mu$"},
        {4, "$ee$"},
        {5, "$\\mu\\mu$"}
};


void make_plot(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, vector<TString> vBkgs, int idx, TString type = "std", TString year = "2016") {

  TH1D* hData = (TH1D*)file->Get(hist_name + "_Data");
  TH1D* hSig = (TH1D*)file->Get(hist_name + "_ttH");
  vector<TH1D*> hBkg;
  Comparison* c;
  
  vector<TString> vLegendLabels; 
  vector<int> vColors;

  if (type =="std") {
    vLegendLabels = {year + " Data", "ttH (M125)"};
    for (int i = 0; i < vBkgs.size(); i++) {
      hBkg.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i]));
      vLegendLabels.push_back(mLabels.find(vBkgs[i])->second);
      vColors.push_back(mColors.find(vBkgs[i])->second);
    }
 
    

    // print latex table
    if (hist_name == "hNVtx") {
      int n_bins = hData->GetSize()-2;
      double yield_data = hData->Integral(0, n_bins+1);
      double yield_signal = hSig->Integral(0, n_bins+1);
      double yield_bkg = 0;
      vector<double> yield_mc;
      for (int i = 0; i < hBkg.size(); i++) {
        yield_bkg += hBkg[i]->Integral(0, n_bins+1);
        yield_mc.push_back(hBkg[i]->Integral(0, n_bins+1));
      }
      cout.setf(ios::fixed);
      cout << std::setprecision(2) << endl;
      cout << "\\begin{center} \\Fontvi" << endl;
      cout << "\\begin{tabular}{| r || r | r|} \\hline" << endl;
      cout << "Process & Yield & Frac. of total bkg. \\\\ \\hline" << endl;
      for (int i = 0; i < vBkgs.size(); i++) {
        if (i == vBkgs.size() - 1)
          cout << mLatex.find(vBkgs[i])->second << " & " << yield_mc[i] << " & " << yield_mc[i]/yield_bkg << " \\\\ \\hline" << endl;
        else 
          cout << mLatex.find(vBkgs[i])->second << " & " << yield_mc[i] << " & " << yield_mc[i]/yield_bkg << " \\\\" << endl;
      }
      cout << "All bkg. & " << yield_bkg << " & " << yield_bkg/yield_bkg << " \\\\ \\hline" << endl;
      cout << "Data & " << yield_data << " & " << yield_data/yield_bkg << " \\\\ \\hline" << endl;
      cout << "\\end{tabular} \\end{center}" << endl;
    }
  }

  else if (type == "shape") {
    vLegendLabels = {"ttH (M125)"};
    for (int i = 0; i < vBkgs.size(); i++) {
      hBkg.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i]));
      vLegendLabels.push_back(mLabels.find(vBkgs[i])->second);
      vColors.push_back(mColors.find(vBkgs[i])->second);
    }
  }

  else if (type =="genPhoton") {
    vLegendLabels = {"ttH (M125)"};
    vColors = {kBlue+2, kAzure+1, kCyan-7, kYellow, kGreen -4, kTeal + 3, kRed+3, kRed, kMagenta-9};
    for (int i = 0; i < vBkgs.size(); i++) {
      for (int j = 0; j < nGenPhotonCats; j++) {
        hBkg.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i] + "GenPhoton_" + to_string(j)));
        vLegendLabels.push_back(mLabels.find(vBkgs[i])->second + mPhotons.find(j)->second);
      }   
    }
    // print latex table
    if (hist_name == "hMass") {
      int n_bins = hData->GetSize()-2;
      int start_bin;
      if (!(output_name == "ttHLeptonic_ttbarCR_plotsgenPhoton.pdf" || output_name == "ttHLeptonic_ttbarCR_plotsstd.pdf"))
        start_bin = 21; // start at m_gg of 80 GeV
      else
	start_bin = 1;
      double yield_data_unc(0), yield_signal_unc(0), yield_bkg_unc(0);
      vector<double> yield_mc_unc;
      double yield_data = hData->IntegralAndError(start_bin, n_bins+1, yield_data_unc);
      double yield_signal = hSig->IntegralAndError(start_bin, n_bins+1, yield_signal_unc);
      double yield_bkg = 0;
      vector<double> yield_mc;
    
      vector<TH1D*> hBkgNominal;
      for (int i = 0; i < vBkgs.size(); i++)
        hBkgNominal.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i]));

      for (int i = 0; i < hBkg.size(); i++) {
        yield_bkg += hBkg[i]->IntegralAndError(start_bin, n_bins+1, yield_bkg_unc);
        yield_mc_unc.push_back(0);
        yield_mc.push_back(hBkg[i]->IntegralAndError(start_bin, n_bins+1, yield_mc_unc[i]));
      }
      cout.setf(ios::fixed);
      cout << std::setprecision(2) << endl;
      cout << "\\begin{center} \\Fontvi" << endl;
      cout << "\\begin{tabular}{| c | r || r |} \\hline" << endl;
      cout << "Process & Component & Yield \\\\ \\hline" << endl;
      for (int i = 0; i < vBkgs.size(); i++) {
        double yield_process_unc;
        double yield_process = hBkgNominal[i]->IntegralAndError(start_bin, n_bins+1, yield_process_unc);
        for (int j = 0; j < nGenPhotonCats; j++) {
          double yield_component = yield_mc[(i*nGenPhotonCats)+j];
          if (j == 0)
            cout << "\\multirow{" << nGenPhotonCats+1 << "}{*}{" << mLatex.find(vBkgs[i])->second << "} & " << mPhotons.find(j)->second << " & " << yield_component << " $\\pm$ " << yield_mc_unc[(i*nGenPhotonCats)+j] << " \\\\" << endl;
          else if (j == nGenPhotonCats-1)
            cout << " & " << mPhotons.find(j)->second<< " & " << yield_component << " $\\pm$ " << yield_mc_unc[(i*nGenPhotonCats)+j] << " \\\\ \\cline{2-3}" << endl;
          else
            cout << " & " << mPhotons.find(j)->second<< " & " << yield_component << " $\\pm$ " << yield_mc_unc[(i*nGenPhotonCats)+j] << " \\\\" << endl;
        }
        cout << " & All Components & " << yield_process << " $\\pm$ " << yield_process_unc << " \\\\ \\hline" << endl;

      }
      //cout << "\\multicolumn{2}{|c||}{All background} & " << yield_bkg << " & " << yield_bkg/yield_bkg << " \\\\ \\hline" << endl;
      //cout << "\\multicolumn{2}{|c||}{Signal} & " << yield_signal << " & " << yield_signal/yield_bkg << " \\\\ \\hline" << endl;
      cout << "\\end{tabular} \\end{center}" << endl;

    }
  }

  else if (type =="genLepton") {
    vLegendLabels = {"ttH (M125)"};
    vColors = {kBlue+2, kAzure+1, kCyan-7, kYellow, kGreen -4, kTeal + 3, kRed+3, kRed, kRed - 7, kMagenta-9, kOrange, kViolet+1, kOrange-9};
    for (int i = 0; i < vBkgs.size(); i++) {
      for (int j = 0; j < nGenLeptonCats; j++) {
        hBkg.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i] + "GenLepton_" + to_string(j)));
        vLegendLabels.push_back(mLabels.find(vBkgs[i])->second + mLeptons.find(j)->second);
      }
    }

    if (hist_name == "hMass") {
      int n_bins = hData->GetSize()-2;
      int start_bin = 21; // start at m_gg of 80 GeV
      double yield_data_unc(0), yield_signal_unc(0), yield_bkg_unc(0);
      vector<double> yield_mc_unc;
      double yield_data = hData->IntegralAndError(start_bin, n_bins+1, yield_data_unc);
      double yield_signal = hSig->IntegralAndError(start_bin, n_bins+1, yield_signal_unc);
      double yield_bkg = 0;
      vector<double> yield_mc;

      vector<TH1D*> hBkgNominal;
      for (int i = 0; i < vBkgs.size(); i++)
        hBkgNominal.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i]));   
 
      for (int i = 0; i < hBkg.size(); i++) {
        yield_bkg += hBkg[i]->IntegralAndError(start_bin, n_bins+1, yield_bkg_unc);
	yield_mc_unc.push_back(0);
        yield_mc.push_back(hBkg[i]->IntegralAndError(start_bin, n_bins+1, yield_mc_unc[i]));
      }
      cout.setf(ios::fixed);
      cout << std::setprecision(2) << endl;
      cout << "\\begin{center} \\Fontvi" << endl;
      cout << "\\begin{tabular}{| c | r || r |} \\hline" << endl;
      cout << "Process & Component & Yield \\\\ \\hline" << endl;
      for (int i = 0; i < vBkgs.size(); i++) {
        double yield_process_unc;
        double yield_process = hBkgNominal[i]->IntegralAndError(start_bin, n_bins+1, yield_process_unc);
	for (int j = 0; j < nGenLeptonCats; j++) {
          double yield_component = yield_mc[(i*nGenLeptonCats)+j];
          if (j == 0)
            cout << "\\multirow{" << nGenLeptonCats+1 << "}{*}{" << mLatex.find(vBkgs[i])->second << "} & " << mLeptonsLatex.find(j)->second << " & " << yield_component << " $\\pm$ " << yield_mc_unc[(i*nGenLeptonCats)+j] << " \\\\" << endl;
          else if (j == nGenLeptonCats-1)
            cout << " & " << mLeptonsLatex.find(j)->second<< " & " << yield_component << " $\\pm$ " << yield_mc_unc[(i*nGenLeptonCats)+j] << " \\\\ \\cline{2-3}" << endl;
          else
            cout << " & " << mLeptonsLatex.find(j)->second<< " & " << yield_component << " $\\pm$ " << yield_mc_unc[(i*nGenLeptonCats)+j] << " \\\\" << endl;
        }
        cout << " & All Components & " << yield_process << " $\\pm$ " << yield_process_unc << " \\\\ \\hline" << endl;
      }
      cout << "\\end{tabular} \\end{center}" << endl;
    }
  }
 
  if (type == "std") {
    c = new Comparison(c1, hData, hSig, hBkg);
    c->set_data_drawOpt("E");
    c->set_rat_label("#frac{Data}{MC}");
  }
  else if (type == "shape") {
    c = new Comparison(c1, hSig, hBkg);
    c->set_data_drawOpt("HIST");
    c->set_rat_label("#frac{Signal}{Background}");
    c->set_scale(-1);
    c->set_log_rat();
    c->set_rat_lim_range({0.03333, 33.3});
  }
  else {
    c = new Comparison(c1, hSig, hBkg);
    c->set_data_drawOpt("HIST");
    c->set_rat_label("#frac{Signal}{Background}");
  }
  c->set_legend_labels(vLegendLabels);
  c->set_colors(vColors);

  c->set_filename(output_name);
  c->set_x_label(x_label);
  c->set_y_label("Events");
  TString output = output_name;
  double lumi = year == "2017" ? 41.5 : 35.9;
  c->set_lumi(lumi);
  if (hist_name == "hMassAN") {
    c->set_no_flow();
    c->set_no_log();
    if (output.Contains("SR3") && output.Contains("Hadronic"))
      c->set_y_lim_range({0, 5});
    if (output.Contains("SR2") && output.Contains("Hadronic"))
      c->set_y_lim_range({0, 8});
    if (output.Contains("SR1") && output.Contains("Hadronic"))
      c->set_y_lim_range({0, 25});
    if (output.Contains("SR1") && output.Contains("Leptonic"))
      c->set_y_lim_range({0, 10});
    if (output.Contains("SR2") && output.Contains("Leptonic"))
      c->set_y_lim_range({0, 3.5});
    if (output.Contains("dilep"))
      c->set_y_lim_range({0, 3.5});
    c->set_x_bin_range({1,80});
    cout << "Data yield in [100,120], [130,180]: " << hData->Integral() << endl;
    cout << "Data yield in [115, 120], [130, 135]: " << hData->Integral(16,20) + hData->Integral(31,35) << endl;
    cout << "Signal yield in [120, 130]: " << hSig->Integral(21,30) << endl;
  }
  

  if (hist_name == "hMass") {
   if (!(output_name == "ttHLeptonic_ttbarCR_plotsgenPhoton.pdf" || output_name == "ttHLeptonic_ttbarCR_plotsstd.pdf") ) {
    c->set_no_underflow();
    c->set_x_bin_range({21,50});
    }
    if (type == "std")
      c->set_verbose();
  }

  if (output.Contains("HadronicLoose"))
    c->give_info("ttH Hadronic Presel.");
  else if (output.Contains("Hadronic") && !output.Contains("Custom"))
    c->give_info("ttH Hadronic Tag");
  else if (output.Contains("Hadronic") && output.Contains("Custom")) {
    c->give_info("ttH Hadronic Presel.");
  }
  else if (output.Contains("LeptonicLoose"))
    c->give_info("ttH Leptonic Presel.");
  else if (output.Contains("ttbarCR")) {
    c->give_info("ttH Leptonic");
    c->give_info("t#bar{t}-Enriched Region");
  }
  else if (output.Contains("Leptonic") && !output.Contains("Custom"))
    c->give_info("ttH Leptonic Tag");
  else if (output.Contains("Leptonic") && output.Contains("Custom"))
    c->give_info("ttH Leptonic Presel.");
  if (output.Contains("PSV")) {
    c->give_info("Pixel Seed Veto");
    c->give_info("        Applied");
  }
  if (output.Contains("Custom"))
    c->give_info("Loose MVA cut applied");

  if (hist_name.Contains("SigmaIEtaIEta"))      c->set_x_bin_range({1,50});

  c->plot(idx);
  delete hData;
  delete hSig;
  for (int i = 0; i < hBkg.size(); i++)
    delete hBkg[i];
  delete c;
}
 




int main(int argc, char* argv[])
{
  // Parse args
  if (argc != 3) {
    cout << "Please provide two arguments: type of plot to make (e.g. 'std') and input file (e.g. '../ttHHadronicLoose_histograms.root')" << endl;
    return 0;
  }

  TString type = argv[1];
  TString type_s = argv[1];

  TString file_path = argv[2];
  TString year = file_path.Contains("2017") ? "2017" : "2016";
  TString tag = file_path.Contains("Hadronic") ? "Hadronic" : "Leptonic";

  TFile* f = new TFile(file_path); 
  vector<TFile*> vFiles = {f};
  string output = (file_path.ReplaceAll("../", "")).ReplaceAll(".root", type + ".pdf").Data();
  
  vector<string> vNames = {output};

  // Decide which backgrounds you want to plot
  vector<TString> vBkgs;
  if (type == "std" || type == "shape") { 
    //vBkgs = {"DY", "DiPhoton", "GammaJets", "TTGG", "TTGJets", "VG", "WJets"}; 
    //vBkgs = {"DY", "DiPhoton", "GammaJets", "QCD", "TTGG", "TTGJets", "VG", "WJets"}; 
    //vBkgs = {"DiPhoton", "GammaJets", "QCD", "TTGG", "TTGJets", "TTJets", "VG", "WJets"}; 
    if (year == "2016")
      vBkgs = {"DiPhoton", "GammaJets", "QCD", "TTGG", "TTGJets", "TTJets", "VG", "DY"};
    if (year == "2017")
      vBkgs = {"DiPhoton", "GammaJets", "QCD", "TTGG", "TTGJets", "TTJets", "DY"};
  }
  else if (type == "genPhoton") {
    //vBkgs = {"DiPhoton", "GammaJets", "QCD"};
    //vBkgs = {"TTGG", "TTGJets", "TTJets"};
    vBkgs = {"TTGG", "TTGJets"};
  } 
  else if (type == "genLepton") {
    //vBkgs = {"TTGG", "TTGJets", "TTJets"};
    //vBkgs = {"TTGJets"};
    //vBkgs = {"TTGG"};
    vBkgs = {"TTJets"};
    //vBkgs = {"TTGG", "TTGJets"};
  }

  // Style options
  gStyle->SetOptStat(0);
  gStyle->SetPalette(kRainBow,0);
  gStyle->SetPaintTextFormat(".2f");
  gStyle->SetTickLength(0.01);
  gStyle->SetErrorX(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTitleAlign(33);
  gStyle->SetTitleX(.93);
  gStyle->SetTitleFontSize(0.03); 

  TCanvas* c1 = new TCanvas("c1", "histos", 600, 800);
  TCanvas* c2 = new TCanvas("c2", "histos2", 900, 800);

  for (int i = 0; i < vFiles.size(); i++) {
    make_plot(c1, vFiles[i], vNames[i], "hMass", "m_{#gamma#gamma} [GeV]", vBkgs, 0,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hRapidity", "Y_{#gamma#gamma} [GeV^{1/2}]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hDiphotonSumPt", "p_{T}(#gamma_{1}) + p_{T}(#gamma_{2}) [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hDiphotonCosPhi", "|cos(#Delta #phi_{#gamma 1, #gamma 2})|", vBkgs, 1,type, year);

    make_plot(c1, vFiles[i], vNames[i], "hNJets", "N_{jets}", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hNbLoose", "N_{b-jets} (Loose)", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hNbMedium", "N_{b-jets} (Medium)", vBkgs, 1,type, year);   
    make_plot(c1, vFiles[i], vNames[i], "hNbTight", "N_{b-jets} (Tight)", vBkgs, 1,type, year);

    make_plot(c1, vFiles[i], vNames[i], "hJet1pT", "Jet1 p_{T} [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hJet2pT", "Jet2 p_{T} [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hJet3pT", "Jet3 p_{T} [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hJet4pT", "Jet4 p_{T} [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hbJet1pT", "bJet1 p_{T} [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hbJet2pT", "bJet2 p_{T} [GeV]", vBkgs, 1,type, year);

    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadPt", "p_{T}(#gamma_{1}) [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadEta", "#eta(#gamma_{1})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadPhi", "#phi(#gamma_{1})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadSigmaIEtaIEta", "#sigma_{i#eta,i#eta}(#gamma_{1})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadHOverE", "h/E(#gamma_{1})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadR9", "R9(#gamma_{1})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadIDMVA", "Photon ID MVA(#gamma_{1})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadPToM", "p_{T}/m_{#gamma#gamma} (#gamma_{1})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadSigmaEOverE", "#sigma_{E}/E (#gamma_{1})", vBkgs, 1,type, year);

    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadPt", "p_{T}(#gamma_{2}) [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadEta", "#eta(#gamma_{2})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadPhi", "#phi(#gamma_{2})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadSigmaIEtaIEta", "#sigma_{i#eta,i#eta}(#gamma_{2})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadHOverE", "h/E(#gamma_{2})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadR9", "R9(#gamma_{2})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadIDMVA", "Photon ID MVA(#gamma_{2})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadPToM", "p_{T}/m_{#gamma#gamma} (#gamma_{2})", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadSigmaEOverE", "#sigma_{E}/E (#gamma_{2})", vBkgs, 1,type, year);

    make_plot(c1, vFiles[i], vNames[i], "htthMVA", "tth MVA", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hMaxBTag", "max b-tag response", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hSecondMaxBTag", "2nd max b-tag response", vBkgs, 1,type, year);

    make_plot(c1, vFiles[i], vNames[i], "hJet1Eta", "Jet1 #eta", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hJet2Eta", "Jet2 #eta", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hJet3Eta", "Jet3 #eta", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hJet4Eta", "Jet4 #eta", vBkgs, 1,type, year);

    make_plot(c1, vFiles[i], vNames[i], "hHT", "HT [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hMetPt", "E_{T}^{miss} [GeV]", vBkgs, 1,type, year);

    if (vNames[i] == "ttHLeptonic_ttbarCR_plots" + type_s + ".pdf") {
      make_plot(c1, vFiles[i], vNames[i], "hPhotonMaxIDMVA", "Max #gamma ID", vBkgs, 1, type, year);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonMinIDMVA", "Min #gamma ID", vBkgs, 1, type, year);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonMinIDMVA_coarse", "Min #gamma ID", vBkgs, 1, type, year);
      make_plot(c1, vFiles[i], vNames[i], "hDiphoMVA", "Diphoton MVA", vBkgs, 1, type, year);
    }
    make_plot(c2, vFiles[i], vNames[i], "hMassAN", "m_{#gamma#gamma} [GeV]", vBkgs, 1,type, year);
    make_plot(c1, vFiles[i], vNames[i], "hMassAN", "m_{#gamma#gamma} [GeV]", vBkgs, 1,type, year);
    //make_plot(c1, vFiles[i], vNames[i], "hPhotonIDMVA_prompt", "#gamma ID (Prompt)", vBkgs, 1, type, year);
    //make_plot(c1, vFiles[i], vNames[i], "hPhotonIDMVA_elec", "#gamma ID (Elec)", vBkgs, 1, type, year);
    //make_plot(c1, vFiles[i], vNames[i], "hPhotonIDMVA_fake", "#gamma ID (Fake)", vBkgs, 1, type, year);
    make_plot(c1, vFiles[i], vNames[i], "hMT", "m_{T}(E_{T}^{miss}, lep) [GeV]", vBkgs, 1, type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonMaxIDMVA", "Max #gamma ID", vBkgs, 1, type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonMinIDMVA", "Min #gamma ID", vBkgs, 1, type, year);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonMinIDMVA_coarse", "Min #gamma ID", vBkgs, 1, type, year);
    make_plot(c1, vFiles[i], vNames[i], "hDiphoMVA", "Diphoton MVA", vBkgs, 1, type, year);

    make_plot(c1, vFiles[i], vNames[i], "h" + tag + "MVA", tag + " MVA", vBkgs, 1, type, year);
    if (tag == "Leptonic") {
      make_plot(c1, vFiles[i], vNames[i], "hLeptonPt", "Lepton p_{T} [GeV]", vBkgs, 1, type, year);
      make_plot(c1, vFiles[i], vNames[i], "hLeptonEta", "Lepton #eta", vBkgs, 1, type, year);
    }

    if (year == "2017") {
      make_plot(c1, vFiles[i], vNames[i], "hPtHiggs", "DiPhoton p_{T} [GeV]", vBkgs, 1, type, year);
      make_plot(c1, vFiles[i], vNames[i], "hMinDrDiphoJet", "Min #Delta R(p_{#gamma#gamma}, jet)", vBkgs, 1, type, year);
      make_plot(c1, vFiles[i], vNames[i], "hDijetClosestWMass", "Min |m_{jj} - m_{W}| [GeV]", vBkgs, 1, type, year);
      make_plot(c1, vFiles[i], vNames[i], "hDijetMass", "m_{jj} (all pairs)", vBkgs, 1, type, year);
      make_plot(c1, vFiles[i], vNames[i], "hDeltaRDiphoW", "#Delta R(p_{#gamma#gamma}, p_{W})", vBkgs, 1, type, year);
    }

    //if (vNames[i] == "ttHHadronicLoose_plots_" + type_s + ".pdf" || vNames[i] == "ttHHadronicCustom_plots_" + type_s + ".pdf")
    //  make_plot(c1, vFiles[i], vNames[i], "hHadronicMVA", "Hadronic MVA Score", vBkgs, 1, type, year); 

    //if (vNames[i] == "ttHLeptonicLoose_plots_" + type_s + ".pdf" || vNames[i] == "ttHLeptonicCustom_plots_" + type_s + ".pdf")
    //  make_plot(c1, vFiles[i], vNames[i], "hLeptonicMVA", "Leptonic MVA Score", vBkgs, 1, type, year);

    make_plot(c1, vFiles[i], vNames[i], "hNVtx", "# Vertices", vBkgs, 2,type, year);
  }
}
