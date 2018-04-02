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
  c->set_rat_lim_range({0.0,0.01});

  TString output = output_name;
  if (output.Contains("Hadronic"))
    c->give_info("ttH Hadronic Tag");
  else if (output.Contains("Leptonic"))
    c->give_info("ttH Leptonic Tag");
  c->set_scale(1);
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


  // Make ttH Hadronic plots //
  make_plot(c1, f1, output_name_hadronic, "hMass", "m_{#gamma#gamma} [GeV]", 0);
  make_plot(c1, f1, output_name_hadronic, "hRapidity", "Y_{#gamma#gamma} [GeV^{1/2}]", 1);
  make_plot(c1, f1, output_name_hadronic, "hDiphotonSumPt", "p_{T}(#gamma_{1}) + p_{T}(#gamma_{2}) [GeV]", 1);
  make_plot(c1, f1, output_name_hadronic, "hDiphotonCosPhi", "|cos(#Delta #phi_{#gamma 1, #gamma 2})|", 1);

  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadPt", "p_{T}(#gamma_{1}) [GeV]", 1);
  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadEt", "E_{T}(#gamma_{1}) [GeV]", 1);  
  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadEta", "#eta(#gamma_{1})", 1);  
  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadPhi", "#phi(#gamma_{1})", 1);    
  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadSigmaIEtaIEta", "#sigma_{i#eta,i#eta}(#gamma_{1})", 1); 
  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadHOverE", "h/E(#gamma_{1})", 1);
  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadR9", "R9(#gamma_{1})", 1);
  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadIDMVA", "Photon ID MVA(#gamma_{1})", 1);
  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadPToM", "p_{T}/m_{#gamma#gamma} (#gamma_{1})", 1);
  make_plot(c1, f1, output_name_hadronic, "hPhotonLeadSigmaEOverE", "#sigma_{E}/E (#gamma_{1})", 1);

  make_plot(c1, f1, output_name_hadronic, "htthMVA", "tth MVA", 1);
  make_plot(c1, f1, output_name_hadronic, "hMaxBTag", "max b-tag response", 1);
  make_plot(c1, f1, output_name_hadronic, "hSecondMaxBTag", "2nd max b-tag response", 2);

  // Make ttH Leptonic plots //
  make_plot(c1, f2, output_name_leptonic, "hMass", "m_{#gamma#gamma} [GeV]", 0);
  make_plot(c1, f2, output_name_leptonic, "hRapidity", "Y_{#gamma#gamma} [GeV^{1/2}]", 1);
  make_plot(c1, f2, output_name_leptonic, "hDiphotonSumPt", "p_{T}(#gamma_{1}) + p_{T}(#gamma_{2}) [GeV]", 1);
  make_plot(c1, f2, output_name_leptonic, "hDiphotonCosPhi", "|cos(#Delta #phi_{#gamma 1, #gamma 2})|", 1);

  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadPt", "p_{T}(#gamma_{1}) [GeV]", 1);
  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadEt", "E_{T}(#gamma_{1}) [GeV]", 1);
  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadEta", "#eta(#gamma_{1})", 1);
  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadPhi", "#phi(#gamma_{1})", 1);
  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadSigmaIEtaIEta", "#sigma_{i#eta,i#eta}(#gamma_{1})", 1);
  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadHOverE", "h/E(#gamma_{1})", 1);
  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadR9", "R9(#gamma_{1})", 1);
  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadIDMVA", "Photon ID MVA(#gamma_{1})", 1);
  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadPToM", "p_{T}/m_{#gamma#gamma} (#gamma_{1})", 1);
  make_plot(c1, f2, output_name_leptonic, "hPhotonLeadSigmaEOverE", "#sigma_{E}/E (#gamma_{1})", 2);
}
