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
	{"QCDGenPhoton_1", "QCD (F/P)"},
        {"GammaJetsGenPhoton_1", "#gamma + Jets (F/P)"},
        {"DiPhotonGenPhoton_2", "#gamma#gamma + Jets (P/P)"},
	{"DiPhotonGenPhoton_1", "#gamma#gamma + Jets (F/P)"}
};

std::map<TString, int> mColors = {
        {"DY", kRed - 7},
        {"DiPhotonGenPhoton_2", kAzure+1},
	{"DiPhotonGenPhoton_1", kRed},
        {"GammaJetsGenPhoton_1", kViolet -4},
        {"QCDGenPhoton_0", kCyan-7},
	{"QCDGenPhoton_1", kOrange},
        {"TTGG", kOrange+1},
        {"TTGGGenP", kTeal+10},
        {"TTGJets", kTeal+10},
        {"VG", kGreen+3},
        {"WJets", kBlue+2},
        {"TTJets", kPink-2}
};


void make_prefit_plot(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, vector<TString> vBkgs, vector<TString> vOtherBkgs, vector<TString> vInfo, int idx, vector<double> scales = {}) {
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
 
  c->give_info("ttH Hadronic Loose");
  for (int i = 0; i < vInfo.size(); i++)
    c->give_info(vInfo[i]);
  //c->give_info("t#bar{t}-Enriched Region");
  if (!postfit)
    c->give_info("Pre-Fit");
  else
    c->give_info("Post-Fit");

  if (!hist_name.Contains("Jets")) {
    c->set_no_log();
    c->set_lower_lim(0.0);
  }
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

  TCanvas* c1 = new TCanvas("c1", "histos", 600, 800);
  TFile* f1 = new TFile("../ttHHadronicLoose_histograms.root");

  //make_prefit_plot(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, vector<TString> vBkgs, vector<TString> vOtherBkgs, int idx, vector<double> scales = {})
  TString ff_template = "QCDGenPhoton_0";
  //TString fp_template = "QCDGenPhoton_1";
  TString fp_template = "GammaJetsGenPhoton_1";
  //TString fp_template = "DiPhotonGenPhoton_1";
  TString pp_template = "DiPhotonGenPhoton_2";
  vector<TString> vOtherBkgs = {"DY", "TTGG", "TTGJets", "TTJets", "VG"};

  // Diphoton PP, Gamma Jets FP, QCD FF
  vector<double> scales_1 = {1.641040148450026, 8.112919043144519, 0.36046715009367847, 1.0000003023824828};
  vector<double> scales_2 = {1.3496496760286325, 9.296097204033499, 0.40000000009581543, 1.0000001758387995};

  // Diphoton PP, Diphoton FP, QCD FF
  //vector<double> scales_1 = {0.29559462500900724, 45.726195718631274, 0.6806121442633202, 1.000000318702647};
  //vector<double> scales_2 = {1,1,1,1};

  // Diphoton PP, QCD FP, QCD FF
  //vector<double> scales_3 = {1.6284709515128826, 0.8243114508164996, 0.36392374575587044, 1.0000003186830242};
  vector<double> scales_3 = {1.5230841089657836, 0.95609767216492, 0.31881376589991667, 1.000000317540502};


  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 0); 
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {"Constrain fit by hand"}, 1, scales_2); 
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {"No constraints on fit"}, 1, scales_1); 

  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMaxIDMVA", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);  
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMaxIDMVA", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {"Constrain fit by hand"}, 1, scales_2); 
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMaxIDMVA", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {"No constraints on fit"}, 1, scales_1);


  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, "QCDGenPhoton_1", pp_template}, vOtherBkgs, {}, 1);
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, "QCDGenPhoton_1", pp_template}, vOtherBkgs, {"No constraints on fit"}, 1, scales_3); 
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMaxIDMVA", "Max. #gamma ID MVA", {ff_template, "QCDGenPhoton_1", pp_template}, vOtherBkgs, {}, 1); 
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMaxIDMVA", "Max. #gamma ID MVA", {ff_template, "QCDGenPhoton_1", pp_template}, vOtherBkgs, {"No constraints on fit"}, 1, scales_3); 
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hNJets", "N_{jets}", {ff_template, "QCDGenPhoton_1", pp_template}, vOtherBkgs, {}, 1);
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hNJets", "N_{jets}", {ff_template, "QCDGenPhoton_1", pp_template}, vOtherBkgs, {"No constraints on fit"}, 1, scales_3);
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hNbJets", "N_{b-jets} (loose)", {ff_template, "QCDGenPhoton_1", pp_template}, vOtherBkgs, {}, 1);
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hNbJets", "N_{b-jets} (loose)", {ff_template, "QCDGenPhoton_1", pp_template}, vOtherBkgs, {"No constraints on fit"}, 1, scales_3);

  vector<double> scales_4 = {1.6284709515128826, 7.99453, 0.36392374575587044, 1.0000003186830242};
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_4);
  make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMaxIDMVA", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 2, scales_4);
}
