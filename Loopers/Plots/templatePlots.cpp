#include "PlotHelper.h"
#include <iomanip>

const TString templatePP = "TTGG";
const TString templateFP = "TTGJets";
const TString templateFF = "TTJets";


void compare_templates(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, int idx) {
  TH1D* hPP_TTGG = (TH1D*)file->Get(hist_name + "_TTGGGenPhoton_2");
  TH1D* hPP_TTGJets = (TH1D*)file->Get(hist_name + "_TTGJetsGenPhoton_2");
  TH1D* hPP_TTJets = (TH1D*)file->Get(hist_name + "_TTJetsGenPhoton_2");
  
  TH1D* hFP_TTGG = (TH1D*)file->Get(hist_name + "_TTGGGenPhoton_1");
  TH1D* hFP_TTGJets = (TH1D*)file->Get(hist_name + "_TTGJetsGenPhoton_1");
  TH1D* hFP_TTJets = (TH1D*)file->Get(hist_name + "_TTJetsGenPhoton_1");

  TH1D* hFF_TTGG = (TH1D*)file->Get(hist_name + "_TTGGGenPhoton_0");
  TH1D* hFF_TTGJets = (TH1D*)file->Get(hist_name + "_TTGJetsGenPhoton_0");
  TH1D* hFF_TTJets = (TH1D*)file->Get(hist_name + "_TTJetsGenPhoton_0");

  //vector<TH1D*> vPP = {hPP_TTGG, hPP_TTGJets, hPP_TTJets};
  //vector<TH1D*> vFP = {hFP_TTGG, hFP_TTGJets, hFP_TTJets};
  //vector<TH1D*> vFF = {hFF_TTGG, hFF_TTGJets, hFF_TTJets};

  vector<TH1D*> vPP = {hPP_TTJets, hPP_TTGJets, hPP_TTGG};
  vector<TH1D*> vFP = {hFP_TTJets, hFP_TTGJets, hFP_TTGG};
  vector<TH1D*> vFF = {hFF_TTJets, hFF_TTGJets, hFF_TTGG};

  /*
  for (int i = 0; i < vPP.size(); i++) {
    vPP[i]->Scale(1/vPP[i]->Integral());
    if (i == 0)
      vPP[i]->Draw("E");
    else
      vPP[i]->Draw("SAME,E");
  }*/

  Comparison* cPP = new Comparison(c1, vPP);
  cPP->set_filename(output_name);
  cPP->set_x_label(x_label);
  cPP->set_y_label("Events");  
  cPP->set_legend_labels({"t#bar{t} + Jets", "t#bar{t}+#gamma+Jets", "t#bar{t} + #gamma#gamma"});
  //cPP->set_legend_labels({"t#bar{t} + #gamma#gamma", "t#bar{t}+#gamma+Jets", "t#bar{t} + Jets"});
  cPP->set_colors({kSpring, kBlue, kRed});
  cPP->give_info("ttH Leptonic Tag");
  cPP->give_info("Prompt/prompt");
  cPP->set_scale(-1);
  cPP->set_y_lim_range({0, 0.75});
  if (hist_name.Contains("Max"))
    cPP->set_y_lim_range({0, 1.25});
  //cPP->set_y_lim_range({0, 0.2});
  //cPP->set_y_lim_range({0.01, 1});
  cPP->set_no_lumi();
  cPP->set_no_log();
  cPP->plot(0, false);
  //cPP->plot(idx == 0 ? 0 : 1);

  Comparison* cFP = new Comparison(c1, vFP); 
  cFP->set_filename(output_name);
  cFP->set_x_label(x_label);
  cFP->set_y_label("Events"); 
  cFP->set_legend_labels({"t#bar{t} + Jets", "t#bar{t}+#gamma+Jets", "t#bar{t} + #gamma#gamma"});
  //cFP->set_legend_labels({"t#bar{t} + #gamma#gamma", "t#bar{t}+#gamma+Jets", "t#bar{t} + Jets"});
  cFP->set_colors({kSpring, kBlue, kRed});
  cFP->give_info("ttH Leptonic Tag");
  cFP->give_info("Fake/prompt");
  cFP->set_scale(-1);
  cFP->set_y_lim_range({0, 0.75});
  if (hist_name.Contains("Max"))
    cFP->set_y_lim_range({0, 1.25});
  //cFP->set_y_lim_range({0, 0.2});
  //cFP->set_y_lim_range({0.01, 1});
  cFP->set_no_lumi();
  cFP->set_no_log();;
  cFP->plot(1, false);

  Comparison* cFF = new Comparison(c1, vFF); 
  cFF->set_filename(output_name);
  cFF->set_x_label(x_label);
  cFF->set_y_label("Events"); 
  cFF->set_legend_labels({"t#bar{t} + Jets", "t#bar{t}+#gamma+Jets", "t#bar{t} + #gamma#gamma"});
  //cFF->set_legend_labels({"t#bar{t} + #gamma#gamma", "t#bar{t}+#gamma+Jets", "t#bar{t} + Jets"});
  cFF->set_colors({kSpring, kBlue, kRed});
  cFF->give_info("ttH Leptonic Tag");
  cFF->give_info("Fake/fake");
  cFF->set_y_lim_range({0, 1.0});
  if (hist_name.Contains("Max"))
    cFF->set_y_lim_range({0, 1.5});
  cFF->set_scale(-1);
  //cFF->set_y_lim_range({0, 0.4});
  //cFF->set_y_lim_range({0.01, 1});
  cFF->set_no_lumi();
  cFF->set_no_log();
  cFF->plot(2, false);


  //delete cPP;
  //delete cFP;
  //delete cFF;
}


std::map<TString, TString> mLabels = {
        {"TTGGGenPhoton_2", "t#bar{t} + #gamma#gamma (P/P)"},
	{"TTGGGenPhoton_1", "t#bar{t} + #gamma#gamma (F/P)"},
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
        {"VG", kGreen+3},
        {"WJets", kBlue+2},
        {"TTJetsGenPhoton_0", kPink-2}
};


void make_prefit_plot(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, vector<TString> vBkgs, vector<double> scales = {}) {
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

  TH1D* hOtherBkg = (TH1D*)file->Get(hist_name + "_DiPhoton");
  hOtherBkg->Add((TH1D*)file->Get(hist_name + "_GammaJets"));
  hOtherBkg->Add((TH1D*)file->Get(hist_name + "_VG"));
  hOtherBkg->Add((TH1D*)file->Get(hist_name + "_WJets"));
  hOtherBkg->Add((TH1D*)file->Get(hist_name + "_QCD"));

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
 
  c->give_info("ttH Leptonic");
  c->give_info("t#bar{t}-Enriched Region");
  if (!postfit)
    c->give_info("Pre-Fit");
  else
    c->give_info("Post-Fit");

  c->plot(1);
  delete hData;
  delete hSig;
  for (int i = 0; i < hBkg.size(); i++)
    delete hBkg[i];
  delete c; 
}
void make_template_plot(TCanvas* c1, TFile* file, string output_name, TString hist_name, TString x_label, int idx) {
  //TH1D* hPP = (TH1D*)file->Get(hist_name + "_" + templatePP + "_GenPhoton_2";
  //TH1D* hFP = (TH1D*)file->Get(hist_name + "_" + templateFP + "_GenPhoton_1";
  //TH1D* hFF = (TH1D*)file->Get(hist_name + "_" + templateFF + "_GenPhoton_0";
  //TH1D* hBkg = (TH1D*)file->Get(hist_name + "_GammaJets_"

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
  TFile* f = new TFile("../ttHLeptonic_histograms.root");
  TFile* f1 = new TFile("../ttbar_cr_histograms.root");

  compare_templates(c1, f, "ttHLeptonic_template_plots_Min.pdf", "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", 1); 
  compare_templates(c1, f, "ttHLeptonic_template_plots_Max.pdf", "hPhotonMaxIDMVA_coarse", "Max #gamma ID MVA", 1);
 
  //vector<TString> vBkgs = {"TTGGGenPhoton_2", "TTGGGenPhoton_1", "TTJetsGenPhoton_0", "DiPhoton", "GammaJets", "VG", "WJets", "QCD"};
  vector<TString> vBkgs = {"TTGGGenPhoton_2", "TTGGGenPhoton_1", "TTJetsGenPhoton_0"};
  make_prefit_plot(c1, f1, "ttHLeptonic_template_prefit.pdf", "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", vBkgs);
  make_prefit_plot(c1, f1, "ttHLeptonic_template_postfit.pdf", "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", vBkgs, {3.5885351626194707e-07, 119.64862304327923, 0.5384692738360889, 3.826623401711424e-07});
  //make_prefit_plot(c1, f1, "ttHLeptonic_template_postfit.pdf", "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", vBkgs, {0.00014332822051296823, 160.80686724029272, 0.4331537473237817, 3.334613563392507e-06});
}
