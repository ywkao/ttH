#include "PlotHelper.h"
#include <iomanip>

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

void compare_templates(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, vector<TString> vBkgs, vector<TString> vBkgLabels, vector<int> vColors, vector<TString> vInfo, int idx, bool log = false) {
  vector<TH1D*> vH;
  for (int i = 0; i < vBkgs.size(); i++)
    vH.push_back((TH1D*)file->Get(hist_name + "_" + vBkgs[i]));

  Comparison* c = new Comparison(c1, vH);
  c->set_filename(output_name);
  c->set_x_label(x_label);
  c->set_y_label("Fraction of Events");
  c->set_legend_labels(vBkgLabels);
  c->set_colors(vColors);
  c->set_info(vInfo);
  c->set_scale(-1); 
  c->set_no_lumi();
  if (!log)
    c->set_no_log();
  if (!log)
    c->set_y_lim_range({0.0, max_bin(vH)*1.4});
  else if (hist_name.Contains("PtRatio") || hist_name.Contains("DeltaRGen"))
    c->set_y_lim_range({0.0000001, max_bin(vH)*2});
  c->set_data_drawOpt("HIST");
  c->plot(idx, false);

  delete c;
}

std::map<TString, TString> mLabels = {
        {"TTGGGenPhoton_2", "t#bar{t} + #gamma#gamma (P/P)"},
	{"TTGGGenPhoton_1", "t#bar{t} + #gamma#gamma (F/P)"},
	{"TTGJetsGenPhoton_1", "t#bar{t}+#gamma+Jets(F/P)"},
        {"TTJetsGenPhoton_0", "t#bar{t} + Jets (F/F)"},
	{"DiPhoton", "#gamma#gamma + Jets"},
        {"GammaJets", "#gamma + Jets"},
	{"QCD", "QCD"},
	{"DY", "Drell-Yan"},
        {"VG", "V+#gamma"},
        {"WJets", "W+Jets"},
};

std::map<TString, int> mColors = {
        {"DY", kRed - 7},
        {"DiPhoton", kAzure+1},
        {"GammaJets", kViolet -4},
        {"QCD", kCyan-7},
        {"TTGGGenPhoton_2", kOrange+1},
        {"TTGGGenPhoton_1", kTeal+10},
	{"TTGJetsGenPhoton_1", kTeal+10},
        {"VG", kGreen+3},
        {"WJets", kBlue+2},
        {"TTJetsGenPhoton_0", kPink-2}
};



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
  TFile* f = new TFile("../ttHHadronicLoose_histograms.root");
  string output_name = "qcd_templates.pdf";

  compare_templates(c1, f, output_name, "hPhotonMinIDMVA", "Min #gamma ID MVA", {"QCDGenPhoton_0", "GammaJetsGenPhoton_0", "DiPhotonGenPhoton_0"}, {"QCD (F/F)", "#gamma + Jets (F/F)", "#gamma#gamma + Jets (F/F)"}, {kAzure+1, kViolet-4, kCyan-7}, {"ttH Hadronic Loose","Fake/fake templates"}, 0);
  compare_templates(c1, f, output_name, "hPhotonMinIDMVA", "Min #gamma ID MVA", {"QCDGenPhoton_1", "GammaJetsGenPhoton_1", "DiPhotonGenPhoton_1"}, {"QCD (F/P)", "#gamma + Jets (F/P)", "#gamma#gamma + Jets (F/P)"}, {kAzure+1, kViolet-4, kCyan-7}, {"ttH Hadronic Loose","Fake/prompt templates"}, 1);
  compare_templates(c1, f, output_name, "hPhotonMinIDMVA", "Min #gamma ID MVA", {"QCDGenPhoton_2", "GammaJetsGenPhoton_2", "DiPhotonGenPhoton_2"}, {"QCD (P/P)", "#gamma + Jets (P/P)", "#gamma#gamma + Jets (P/P)"}, {kAzure+1, kViolet-4, kCyan-7}, {"ttH Hadronic Loose","Prompt/prompt templates"}, 1);

  compare_templates(c1, f, output_name, "hPhotonMaxIDMVA", "Max #gamma ID MVA", {"QCDGenPhoton_0", "GammaJetsGenPhoton_0", "DiPhotonGenPhoton_0"}, {"QCD (F/F)", "#gamma + Jets (F/F)", "#gamma#gamma + Jets (F/F)"}, {kAzure+1, kViolet-4, kCyan-7}, {"ttH Hadronic Loose","Fake/fake templates"}, 1);
  compare_templates(c1, f, output_name, "hPhotonMaxIDMVA", "Max #gamma ID MVA", {"QCDGenPhoton_1", "GammaJetsGenPhoton_1", "DiPhotonGenPhoton_1"}, {"QCD (F/P)", "#gamma + Jets (F/P)", "#gamma#gamma + Jets (F/P)"}, {kAzure+1, kViolet-4, kCyan-7}, {"ttH Hadronic Loose","Fake/prompt templates"}, 1);
  compare_templates(c1, f, output_name, "hPhotonMaxIDMVA", "Max #gamma ID MVA", {"QCDGenPhoton_2", "GammaJetsGenPhoton_2", "DiPhotonGenPhoton_2"}, {"QCD (P/P)", "#gamma + Jets (P/P)", "#gamma#gamma + Jets (P/P)"}, {kAzure+1, kViolet-4, kCyan-7}, {"ttH Hadronic Loose","Prompt/prompt templates"}, 1);

  compare_templates(c1, f, output_name, "hPhotonPtRatio", "#gamma p_{T} (reco) / p_{T} (gen)", {"QCD", "GammaJets", "DiPhoton"}, {"QCD", "#gamma + Jets", "#gamma#gamma + Jets"}, {kAzure+1, kViolet-4, kCyan-7}, {"ttH Hadronic Loose"}, 1, true);

  compare_templates(c1, f, output_name, "hPhotonDeltaRGen", "#Delta R(reco #gamma, gen #gamma)", {"QCD", "GammaJets", "DiPhoton"}, {"QCD", "#gamma + Jets", "#gamma#gamma + Jets"}, {kAzure+1, kViolet-4, kCyan-7}, {"ttH Hadronic Loose"}, 1, true);

  compare_templates(c1, f, output_name, "hPhotonMinIDMVA", "Min #gamma ID MVA", {"QCDGenPhotonDetail_3", "GammaJetsGenPhotonDetail_3", "DiPhotonGenPhotonDetail_3", "TTGJetsGenPhotonDetail_3", "TTGGGenPhotonDetail_3", "TTJetsGenPhotonDetail_3"}, {"QCD (F/P)", "#gamma + Jets (F/P)", "#gamma#gamma + Jets (F/P)", "t#bar{t}+#gamma + jets (F/P)", "t#bar{t}+#gamma#gamma (F/P)", "t#bar{t} + jets (F/P)"}, {kAzure+1, kViolet-4, kCyan-7, 1, 1}, {"ttH Hadronic Loose","Fake/prompt templates"}, 2);

}
