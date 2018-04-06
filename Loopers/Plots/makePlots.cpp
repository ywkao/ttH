#include "PlotHelper.h"

const int nBkgCats = 7;

void make_plot(TCanvas* c1, TFile* f1, string output_name, TString hist_name, TString x_label, int idx) {
  TH1D* hSig = (TH1D*)f1->Get(hist_name + "_0");
  vector<TH1D*> hBkg;
  for (int i = 1; i <= nBkgCats; i++) {
    //if (i == 1 || i == 7) continue;
    hBkg.push_back((TH1D*)f1->Get(hist_name + "_" + to_string(i)));
  }
  Comparison* c = new Comparison(c1, hSig, hBkg);
  c->set_filename(output_name);
  c->set_rat_label("#frac{Signal}{Background}");
  c->set_legend_labels({"ttH (M125)", "Drell-Yan", "#gamma#gamma + Jets", "#gamma+Jets", "QCD", "TT+#gamma#gamma", "TT+#gamma+Jets", "V+#gamma"});
  c->set_x_label(x_label);
  c->set_y_label("Events");
  c->set_data_drawOpt("HIST");
  c->set_lumi(1);
  TString output = output_name;
  if (output.Contains("Hadronic"))	c->set_rat_lim_range({0.0,0.04});
  else if (output.Contains("Leptonic"))	c->set_rat_lim_range({0.0,0.1});

  if (output.Contains("HadronicLoose"))
    c->give_info("ttH Hadronic Loose Tag");
  else if (output.Contains("Hadronic"))
    c->give_info("ttH Hadronic Tag");
  else if (output.Contains("LeptonicLoose"))
    c->give_info("ttH Leptonic Loose Tag");
  else if (output.Contains("Leptonic"))
    c->give_info("ttH Leptonic Tag");
  c->set_scale(1);

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

  TCanvas* c1 = new TCanvas("c1", "histos", 600, 800);
  TFile* f1 = new TFile("../ttHHadronic_histograms.root");
  string output_name_hadronic = "ttHHadronic_plots.pdf";

  TFile* f2 = new TFile("../ttHLeptonic_histograms.root");
  string output_name_leptonic = "ttHLeptonic_plots.pdf";

  TFile* f3 = new TFile("../ttHHadronicLoose_histograms.root");
  string output_name_hadronic_loose = "ttHHadronicLoose_plots.pdf";

  TFile* f4 = new TFile("../ttHLeptonicLoose_histograms.root");
  string output_name_leptonic_loose = "ttHLeptonicLoose_plots.pdf";

  vector<TFile*> vFiles = {f1, f2, f3, f4};
  vector<string> vNames = {output_name_hadronic, output_name_leptonic, output_name_hadronic_loose, output_name_leptonic_loose};

  for (int i = 0; i < vFiles.size(); i++) {
    make_plot(c1, vFiles[i], vNames[i], "hMass", "m_{#gamma#gamma} [GeV]", 0);
    make_plot(c1, vFiles[i], vNames[i], "hRapidity", "Y_{#gamma#gamma} [GeV^{1/2}]", 1);
    make_plot(c1, vFiles[i], vNames[i], "hDiphotonSumPt", "p_{T}(#gamma_{1}) + p_{T}(#gamma_{2}) [GeV]", 1);
    make_plot(c1, vFiles[i], vNames[i], "hDiphotonCosPhi", "|cos(#Delta #phi_{#gamma 1, #gamma 2})|", 1);

    make_plot(c1, vFiles[i], vNames[i], "hNJets", "N_{jets}", 1);
    make_plot(c1, vFiles[i], vNames[i], "hNbJets", "N_{b-jets} (medium)", 1);
    make_plot(c1, vFiles[i], vNames[i], "hJet1pT", "Jet1 p_{T} [GeV]", 1);
    make_plot(c1, vFiles[i], vNames[i], "hJet2pT", "Jet2 p_{T} [GeV]", 1);
    make_plot(c1, vFiles[i], vNames[i], "hJet3pT", "Jet3 p_{T} [GeV]", 1);
    make_plot(c1, vFiles[i], vNames[i], "hJet4pT", "Jet4 p_{T} [GeV]", 1);
    make_plot(c1, vFiles[i], vNames[i], "hbJet1pT", "bJet1 p_{T} [GeV]", 1);
    make_plot(c1, vFiles[i], vNames[i], "hbJet2pT", "bJet2 p_{T} [GeV]", 1);

    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadPt", "p_{T}(#gamma_{1}) [GeV]", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadEta", "#eta(#gamma_{1})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadPhi", "#phi(#gamma_{1})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadSigmaIEtaIEta", "#sigma_{i#eta,i#eta}(#gamma_{1})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadHOverE", "h/E(#gamma_{1})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadR9", "R9(#gamma_{1})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadIDMVA", "Photon ID MVA(#gamma_{1})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadPToM", "p_{T}/m_{#gamma#gamma} (#gamma_{1})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonLeadSigmaEOverE", "#sigma_{E}/E (#gamma_{1})", 1);

    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadPt", "p_{T}(#gamma_{2}) [GeV]", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadEta", "#eta(#gamma_{2})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadPhi", "#phi(#gamma_{2})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadSigmaIEtaIEta", "#sigma_{i#eta,i#eta}(#gamma_{2})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadHOverE", "h/E(#gamma_{2})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadR9", "R9(#gamma_{2})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadIDMVA", "Photon ID MVA(#gamma_{2})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadPToM", "p_{T}/m_{#gamma#gamma} (#gamma_{2})", 1);
    make_plot(c1, vFiles[i], vNames[i], "hPhotonSubleadSigmaEOverE", "#sigma_{E}/E (#gamma_{2})", 1);

    make_plot(c1, vFiles[i], vNames[i], "htthMVA", "tth MVA", 1);
    make_plot(c1, vFiles[i], vNames[i], "hMaxBTag", "max b-tag response", 1);
    make_plot(c1, vFiles[i], vNames[i], "hSecondMaxBTag", "2nd max b-tag response", 1);

    make_plot(c1, vFiles[i], vNames[i], "hJet1Eta", "Jet1 #eta", 1);
    make_plot(c1, vFiles[i], vNames[i], "hJet2Eta", "Jet2 #eta", 1);
    make_plot(c1, vFiles[i], vNames[i], "hJet3Eta", "Jet3 #eta", 1);
    make_plot(c1, vFiles[i], vNames[i], "hJet4Eta", "Jet4 #eta", 1);

    make_plot(c1, vFiles[i], vNames[i], "hHT", "HT [GeV]", 2);
  }
}
