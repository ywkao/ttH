#include "PlotHelper.h"
#include <iomanip>

const TString templatePP = "TTGG";
const TString templateFP = "TTGJets";
const TString templateFF = "TTJets";


double max_bin(vector<TH1D*> vH) {
  double max = 0;
  for (int i = 0; i < vH.size(); i++) {
    int n_bins = vH[i]->GetSize()-2;
    for (int j = 1; j <= n_bins; j++) {
      double bin_content = vH[i]->GetBinContent(j)/vH[i]->Integral(0, n_bins);
      if (bin_content > max)
        max = bin_content;
    } 
  }
  return max;
}

std::map<TString, TString> mLabels = {
        {"TTGG", "t#bar{t} + #gamma#gamma"},
        {"TTGGGenPhoton_1", "t#bar{t} + #gamma#gamma (F/P)"},
        {"TTGJets", "t#bar{t}+#gamma+Jets"},
        {"TTJets", "t#bar{t} + Jets"},
        {"DiPhoton", "#gamma#gamma + Jets"},
        {"GammaJets", "#gamma + Jets"},
        {"QCD", "QCD"},
        {"DY", "Drell-Yan"},
        {"VG", "V+#gamma"},
        {"WJets", "W+Jets"},
        {"QCDGenPhoton_0", "QCD (F/F)"},
        {"GammaJetsGenPhoton_1", "#gamma + Jets (F/P)"},
        {"DiPhotonGenPhoton_2", "#gamma #gamma + Jets (P/P)"}
};

std::map<TString, int> mColors = {
        {"DY", kRed - 7},
        {"DiPhotonGenPhoton_2", kAzure+1},
        {"GammaJetsGenPhoton_1", kViolet -4},
        {"QCDGenPhoton_0", kCyan-7},
        {"TTGG", kOrange+1},
        {"TTGGGenP", kTeal+10},
        {"TTGJets", kTeal+10},
        {"VG", kGreen+3},
        {"WJets", kBlue+2},
        {"TTJets", kPink-2}
};


void make_prefit_plot(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, vector<TString> vBkgs, vector<TString> vOtherBkgs, int idx, vector<double> scales = {}) {
  TH1D* hData = (TH1D*)file->Get(hist_name + "_Data");
  TH1D* hSig = (TH1D*)file->Get(hist_name + "_ttH");
  vector<TH1D*> hBkg;
  Comparison* c;

  vector<TString> vLegendLabels;
  vector<int> vColors;
  vLegendLabels = {"Data", "ttH (M125)"};
  for (int i = 0; i < vBkgs.size(); i++) {
    hBkg.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i]));
    vLegendLabels.push_back(mLabels.find(vBkgs[i])->second);
    vColors.push_back(mColors.find(vBkgs[i])->second);
  }


  TH1D* hOtherBkg = (TH1D*)file->Get(hist_name + "_" + vOtherBkgs[0]);
  for (int i = 1; i < vOtherBkgs.size(); i++) {
    hOtherBkg->Add((TH1D*)file->Get(hist_name + "_" + vOtherBkgs[i]));
  }

  hBkg.push_back(hOtherBkg);
  vLegendLabels.push_back("Other Bkgs");
  vColors.push_back(kGreen+3);

  bool postfit = scales.size() == hBkg.size();
  if (postfit) {
    for (int i = 0; i < hBkg.size(); i++) {
      hBkg[i]->Scale(scales[i]);
    }
  }

  c = new Comparison(c1, hData, hSig, hBkg);
  c->set_data_drawOpt("E");
  c->set_rat_label("#frac{Data}{MC}");

  c->set_legend_labels(vLegendLabels);
  c->set_colors(vColors);

  c->set_filename(output_name);
  c->set_x_label(x_label);
  c->set_y_label("Events");
  c->set_lumi(35.9);
  c->set_lower_lim(0.0);
 
  //c->give_info("ttH Leptonic");
  //c->give_info("t#bar{t}-Enriched Region");
  if (!postfit)
    c->give_info("Pre-Fit");
  else
    c->give_info("Post-Fit");

  c->set_no_log();
  c->plot(idx);
  delete hData;
  delete hSig;
  for (int i = 0; i < hBkg.size(); i++)
    delete hBkg[i];
  delete c; 
}

int main(int argc, char* argv[]) {
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

  TCanvas* c1 = new TCanvas("c1", "histos", 800, 800);
  TFile* f1 = new TFile("../ttHHadronicLoose_histograms.root");

  //make_prefit_plot(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, vector<TString> vBkgs, vector<TString> vOtherBkgs, int idx, vector<double> scales = {})
  TString ff_template = "QCDGenPhoton_0";
  TString fp_template = "GammaJetsGenPhoton_1";
  TString pp_template = "DiPhotonGenPhoton_2";
  vector<TString> vOtherBkgs = {"DY", "TTGG", "TTGJets", "TTJets", "VG"};
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, 0); 
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, 2, {0.36514281749063143, 8.522879651222972, 0.980241530998876, 0.9999994120958});
}
