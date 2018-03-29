#include "PlotHelper.h"

const int nBkgCats = 6;

void make_plot(TCanvas* c1, TFile* f1, string output_name, TString hist_name, TString x_label, int idx) {
  TH1D* hSig = (TH1D*)f1->Get(hist_name + "_0");
  vector<TH1D*> hBkg;
  for (int i = 1; i <= nBkgCats; i++) {
    if (i == 1 || i == 6) continue;
    hBkg.push_back((TH1D*)f1->Get(hist_name + "_" + to_string(i)));
  }
  Comparison* c = new Comparison(c1, hSig, hBkg);
  c->set_filename(output_name);
  c->set_rat_label("#frac{Signal}{Background}");
  c->set_legend_labels({"ttH (M125)", "DiPhoton", "G+Jets", "QCD", "TT+G"});
  c->set_x_label(x_label);
  c->set_y_label("Events");
  c->set_data_drawOpt("HIST");
  c->set_lumi(1);

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
  make_plot(c1, f1, "ttHHadronic_plots.pdf", "hMass", "m_{#gamma#gamma} [GeV]", 0);
  make_plot(c1, f1, "ttHHadronic_plots.pdf", "hRapidity", "Y_{#gamma#gamma} [GeV^{1/2}]", 1);
  make_plot(c1, f1, "ttHHadronic_plots.pdf", "htthMVA", "tth MVA", 1);
  make_plot(c1, f1, "ttHHadronic_plots.pdf", "hMaxBTag", "max b-tag response", 2);

  TFile* f2 = new TFile("../ttHLeptonic_histograms.root");
  make_plot(c1, f2, "ttHLeptonic_plots.pdf", "hMass", "m_{#gamma#gamma} [GeV]", 0);
  make_plot(c1, f2, "ttHLeptonic_plots.pdf", "hRapidity", "Y_{#gamma#gamma} [GeV^{1/2}]", 2);

}
