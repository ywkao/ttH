#include "PlotHelper.h"

const int nBkgCats = 18;

// histogram index to sample
// 0: ttH
// 1: Drell-Yan
// 2: Gamma Gamma + Jets
// 3: Gamma + Jets
// 4: QCD
// 5: TT + Gamma Gamma (hadronic)
// 6: TT + Gamma Gamma (semileptonic)
// 7: TT + Gamma Gamma (dileptonic)
// 8: TT + Gamma + Jets (hadronic)
// 9: TT + Gamma + Jets (semileptonic, e)
// 10: TT + Gamma + Jets (semileptonic, mu)
// 11: TT + Gamma + Jets (dileptonic, emu)
// 12: TT + Gamma + Jets (dileptonic, ee)
// 13: TT + Gamma + Jets (dileptonic, mumu)
// 14: V + Gamma
// 15: W + Jets
// 16: TT + Jets (hadronic)
// 17: TT + Jets (semileptonic)
// 18: TT + Jets (dileptonic)

void make_plot(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, int idx, TString type = "std") {
  TH1D* hData = (TH1D*)file->Get(hist_name + "_" + to_string(nBkgCats+1));
  TH1D* hSig = (TH1D*)file->Get(hist_name + "_0");
  vector<TH1D*> hBkg;
  Comparison* c;
  if (type == "std") {
    for (int i = 1; i <= nBkgCats; i++) {
      if (i == 5) {// this is a ttbar sample
	TH1D* h = (TH1D*)file->Get(hist_name + "_" + to_string(i));
	h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+1)));
	h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+2))); // add up hadronic/semi/dilepton contributions
        hBkg.push_back(h);
      }
      else if (i == 8) {
        TH1D* h = (TH1D*)file->Get(hist_name + "_" + to_string(i));
        h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+1)));
        h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+2))); // add up hadronic/semi/dilepton contributions
	h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+3)));
	h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+4)));
	h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+5)));
        hBkg.push_back(h);
      }
      else if (i == 6 || i == 7 || i == 9 || i == 10 || i == 11 || i == 12 || i == 13 || i == 16 || i == 17 || i == 18)
	continue;
      else
	hBkg.push_back((TH1D*)file->Get(hist_name + "_" + to_string(i)));
    }
    c = new Comparison(c1, hData, hSig, hBkg);
    c->set_legend_labels({"Data", "ttH (M125)", "Drell-Yan", "#gamma#gamma + Jets", "#gamma+Jets", "QCD", "TT+#gamma#gamma", "TT+#gamma+Jets", "V+#gamma", "W+Jets"});
    c->set_data_drawOpt("E");
    c->set_rat_label("#frac{Data}{MC}");
    c->set_colors({kRed - 7, kAzure+1, kViolet -4, kCyan-7, kOrange+1, kTeal+10, kGreen+3, kBlue+2});
  }
  else if (type == "ttbar") {
    for (int i = 1; i <= nBkgCats; i++) {
      if (!(i >= 5 && i <= 13))
        continue;
      hBkg.push_back((TH1D*)file->Get(hist_name + "_" + to_string(i)));  
    }
    c = new Comparison(c1, hSig, hBkg);
    c->set_legend_labels({"ttH (M125)", "TT+#gamma#gamma (hadronic)", "TT+#gamma#gamma (semileptonic)", "TT+#gamma#gamma (dileptonic)", "TT+#gamma+Jets (hadronic)", "TT+#gamma+Jets (semi, e)", "TT+#gamma+Jets (semi, mu)", "TT+#gamma+Jets (dilep, e+mu)", "TT+#gamma+Jets (dilep, e+e)", "TT+#gamma+Jets (dilep, mu+mu)"});
    c->set_data_drawOpt("HIST");
    c->set_rat_label("#frac{Signal}{Background}");
    c->set_rat_lim_range({0.0,1.0});
    c->set_colors({kBlue+2, kAzure+1, kCyan-7, kYellow, kGreen -4, kTeal + 3, kRed+3, kRed, kMagenta-9});
  }
  else if (type == "ttbar_vanilla") {
    for (int i = 1; i <= nBkgCats; i++) {
      if (!(i >= 13 && i <= 15))
        continue;
      hBkg.push_back((TH1D*)file->Get(hist_name + "_" + to_string(i)));
    }
    c = new Comparison(c1, hSig, hBkg);
    c->set_legend_labels({"ttH (M125)", "TT + Jets (hadronic)", "TT + Jets (semileptonic)", "TT + Jets (dileptonic)"}); 
    c->set_data_drawOpt("HIST");
    c->set_rat_label("#frac{Signal}{Background}");
    c->set_rat_lim_range({0.0,0.25});
    c->set_colors({kGreen+4, kGreen, kGreen-10});
  }
  else if (type == "linear") { // normalize signal to bkg, no data no QCD
    for (int i = 1; i <= nBkgCats; i++) {
      if (i == 5) {// this is a ttbar sample
        TH1D* h = (TH1D*)file->Get(hist_name + "_" + to_string(i));
        h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+1)));
        h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+2))); // add up hadronic/semi/dilepton contributions
        hBkg.push_back(h);
      }
      else if (i == 8) {
        TH1D* h = (TH1D*)file->Get(hist_name + "_" + to_string(i));
        h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+1)));
        h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+2))); // add up hadronic/semi/dilepton contributions
        h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+3)));
        h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+4)));
        h->Add((TH1D*)file->Get(hist_name + "_" + to_string(i+5)));
        hBkg.push_back(h);
      }
      else if (i==4 || i == 6 || i == 7 || i == 9 || i == 10 || i == 11 || i == 12 || i == 13 || i == 16 || i == 17 || i == 18)
        continue;
      else
        hBkg.push_back((TH1D*)file->Get(hist_name + "_" + to_string(i)));
    }
    c = new Comparison(c1, hSig, hBkg);
    c->set_legend_labels({"ttH (M125)", "Drell-Yan", "#gamma#gamma + Jets", "#gamma+Jets", "TT+#gamma#gamma", "TT+#gamma+Jets", "V+#gamma", "W+Jets"});
    c->set_rat_label("#frac{Signal}{Background}");
    c->set_colors({kRed - 7, kAzure+1, kViolet -4, kOrange+1, kTeal+10, kGreen+3, kBlue+2});
    c->set_scale(-1);  
    c->set_no_log(); 
  }

  c->set_filename(output_name);
  //c->set_legend_labels({"Data", "ttH (M125)", "Drell-Yan", "#gamma#gamma + Jets", "#gamma+Jets", "QCD", "TT+#gamma#gamma (Hadronic)", "TT+#gamma#gamma (SemiLep)", "TT+#gamma#gamma (Dilep)","TT+#gamma+Jets (Hadronic)", "TT+#gamma+Jets (SemiLep)", "TT+#gamma+Jets (Dilep)", "V+#gamma", "W + Jets", "TT + Jets (Hadronic)", "TT + Jets (SemiLep)", "TT + Jets (DiLep)"});
  c->set_x_label(x_label);
  c->set_y_label("Events");
  c->set_lumi(35.9);
  TString output = output_name;
  if (hist_name == "hMass") {
    c->set_no_underflow();
    c->set_x_bin_range({17,50});
    if (type == "std")
      c->set_verbose();
  }

  if (output.Contains("HadronicLoose"))
    c->give_info("ttH Hadronic Loose Tag");
  else if (output.Contains("Hadronic"))
    c->give_info("ttH Hadronic Tag");
  else if (output.Contains("LeptonicLoose"))
    c->give_info("ttH Leptonic Loose Tag");
  else if (output.Contains("Leptonic"))
    c->give_info("ttH Leptonic Tag");
  //c->set_stack_order({4,5,1,6,0,2,3});
  //c->set_scale(-1);
  //c->set_no_log();
  //c->set_rat_lim_range({0.0,2.0});
  // Manually set options for specific plots
  if (hist_name.Contains("SigmaIEtaIEta"))	c->set_x_bin_range({1,50});
     
  c->plot(idx);
  delete c;
}


int main(int argc, char* argv[])
{
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

  TString type;
  string type_s;
  if (argc >= 1) {
    type = argv[1];
    type_s = argv[1];
  }
  else {
    type = "std";
    type_s = "std";
  }
  TCanvas* c1 = new TCanvas("c1", "histos", 600, 800);
  TFile* f1 = new TFile("../ttHHadronic_histograms.root");
  string output_name_hadronic = "ttHHadronic_plots_" + type_s + ".pdf";

  TFile* f2 = new TFile("../ttHLeptonic_histograms.root");
  string output_name_leptonic = "ttHLeptonic_plots_" + type_s + ".pdf";

  TFile* f3 = new TFile("../ttHHadronicLoose_histograms.root");
  string output_name_hadronic_loose = "ttHHadronicLoose_plots_" + type_s + ".pdf";

  TFile* f4 = new TFile("../ttHLeptonicLoose_histograms.root");
  string output_name_leptonic_loose = "ttHLeptonicLoose_plots_" + type_s + ".pdf";

  vector<TFile*> vFiles = {f1, f2, f3, f4};
  vector<string> vNames = {output_name_hadronic, output_name_leptonic, output_name_hadronic_loose, output_name_leptonic_loose};

  //vector<TString> vTypes = {"std","linear", "ttbar", "ttbar_vanilla"};
  //vector<TString> vTypes = {"ttbar"};

  for (int i = 0; i < vFiles.size(); i++) {
      make_plot(c1, vFiles[i], vNames[i], "hMass", "m_{#gamma#gamma} [GeV]", 0,type);
      make_plot(c1, vFiles[i], vNames[i], "hRapidity", "Y_{#gamma#gamma} [GeV^{1/2}]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hDiphotonSumPt", "p_{T}(#gamma_{1}) + p_{T}(#gamma_{2}) [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hDiphotonCosPhi", "|cos(#Delta #phi_{#gamma 1, #gamma 2})|", 1,type);

      make_plot(c1, vFiles[i], vNames[i], "hNJets", "N_{jets}", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hNbJets", "N_{b-jets} (medium)", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hJet1pT", "Jet1 p_{T} [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hJet2pT", "Jet2 p_{T} [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hJet3pT", "Jet3 p_{T} [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hJet4pT", "Jet4 p_{T} [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hbJet1pT", "bJet1 p_{T} [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hbJet2pT", "bJet2 p_{T} [GeV]", 1,type);

      make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadPt", "p_{T}(#gamma_{1}) [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadEta", "#eta(#gamma_{1})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadPhi", "#phi(#gamma_{1})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadSigmaIEtaIEta", "#sigma_{i#eta,i#eta}(#gamma_{1})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadHOverE", "h/E(#gamma_{1})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadR9", "R9(#gamma_{1})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadIDMVA", "Photon ID MVA(#gamma_{1})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadPToM", "p_{T}/m_{#gamma#gamma} (#gamma_{1})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadSigmaEOverE", "#sigma_{E}/E (#gamma_{1})", 1,type);

      make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadPt", "p_{T}(#gamma_{2}) [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadEta", "#eta(#gamma_{2})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadPhi", "#phi(#gamma_{2})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadSigmaIEtaIEta", "#sigma_{i#eta,i#eta}(#gamma_{2})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadHOverE", "h/E(#gamma_{2})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadR9", "R9(#gamma_{2})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadIDMVA", "Photon ID MVA(#gamma_{2})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadPToM", "p_{T}/m_{#gamma#gamma} (#gamma_{2})", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadSigmaEOverE", "#sigma_{E}/E (#gamma_{2})", 1,type);

      make_plot(c1, vFiles[i], vNames[i], "htthMVA", "tth MVA", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hMaxBTag", "max b-tag response", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hSecondMaxBTag", "2nd max b-tag response", 1,type);

      make_plot(c1, vFiles[i], vNames[i], "hJet1Eta", "Jet1 #eta", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hJet2Eta", "Jet2 #eta", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hJet3Eta", "Jet3 #eta", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hJet4Eta", "Jet4 #eta", 1,type);

      make_plot(c1, vFiles[i], vNames[i], "hHT", "HT [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hMetPt", "E_{T}^{miss} [GeV]", 1,type);
      make_plot(c1, vFiles[i], vNames[i], "hNVtx", "# Vertices", 2,type);
  }
}
