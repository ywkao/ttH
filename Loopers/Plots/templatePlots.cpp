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
  cPP->give_info("ttH Leptonic");
  cPP->give_info("t#bar{t}-enriched region + P.S. veto");
  cPP->give_info("Prompt/prompt");
  cPP->set_scale(-1);
  cPP->set_y_lim_range({0, 1.4*max_bin(vPP)});
  //cPP->set_y_lim_range({0, 0.75});
  //if (hist_name.Contains("Max"))
  //  cPP->set_y_lim_range({0, 1.4});
  //cPP->set_y_lim_range({0, 0.2});
  //cPP->set_y_lim_range({0.01, 1});
  cPP->set_no_lumi();
  cPP->set_no_log();
  cPP->set_lower_lim(0.0);
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
  cFP->give_info("t#bar{t}-enriched region + P.S. veto");
  cFP->give_info("Fake/prompt");
  cFP->set_scale(-1);
  cFP->set_y_lim_range({0, 1.4*max_bin(vFP)});
  //cFP->set_y_lim_range({0, 0.75});
  //if (hist_name.Contains("Max"))
    //cFP->set_y_lim_range({0, 1.4});
  //cFP->set_y_lim_range({0, 0.2});
  //cFP->set_y_lim_range({0.01, 1});
  cFP->set_no_lumi();
  cFP->set_no_log();
  cFP->set_lower_lim(0.0);
  cFP->plot(1, false);

  Comparison* cFF = new Comparison(c1, vFF); 
  cFF->set_filename(output_name);
  cFF->set_x_label(x_label);
  cFF->set_y_label("Events"); 
  cFF->set_legend_labels({"t#bar{t} + Jets", "t#bar{t}+#gamma+Jets", "t#bar{t} + #gamma#gamma"});
  //cFF->set_legend_labels({"t#bar{t} + #gamma#gamma", "t#bar{t}+#gamma+Jets", "t#bar{t} + Jets"});
  cFF->set_colors({kSpring, kBlue, kRed});
  cFF->give_info("ttH Leptonic");
  cFF->give_info("t#bar{t}-enriched region + P.S. veto");
  cFF->give_info("Fake/fake");
  cFF->set_y_lim_range({0, 1.4*max_bin(vFF)});
  //cFF->set_y_lim_range({0, 1.0});
  //if (hist_name.Contains("Max"))
  //  cFF->set_y_lim_range({0, 1.5});
  cFF->set_scale(-1);
  //cFF->set_y_lim_range({0, 0.4});
  //cFF->set_y_lim_range({0.01, 1});
  cFF->set_no_lumi();
  cFF->set_no_log();
  cFF->set_lower_lim(0.0);
  cFF->plot(2, false);


  delete cPP;
  delete cFP;
  delete cFF;
}

void compare_photon_origins(TCanvas* c1, TFile* file, string output_name, TString process, TString hist, TString x_label, vector<TString> vInfo, int idx) {
  TH1D *hPrompt, *hElec, *hFake;
  TString output = output_name;
  if (output.Contains("minID")) {
    hElec = (TH1D*)file->Get("hPhotonMinIDMVA_coarse_" + process + "GenPhotonDetail_4");
    hFake = (TH1D*)file->Get("hPhotonMinIDMVA_coarse_" + process + "GenPhotonDetail_3");
    hPrompt = (TH1D*)file->Get("hPhotonMinIDMVA_coarse_" + process + "GenPhotonDetail_5");
  }
  else {
    hPrompt = (TH1D*)file->Get(hist + "_prompt_" + process);
    hElec = (TH1D*)file->Get(hist + "_elec_" + process);
    hFake = (TH1D*)file->Get(hist + "_fake_" + process); 
  }

  vector<TH1D*> vH= {hElec, hFake, hPrompt}; 
  Comparison* c = new Comparison(c1, vH);
  c->set_filename(output_name);
  c->set_x_label(x_label);
  c->set_y_label("Fraction");
  if (output.Contains("minID"))
    c->set_legend_labels({"Elec/Prompt", "Fake/Prompt", "Prompt/Prompt"});
  else
    c->set_legend_labels({"Electron", "Fake", "Prompt"});
  c->give_info("ttH Leptonic Tag");
  c->set_scale(-1);
  c->set_no_lumi();
  c->set_no_log();
  c->set_y_lim_range({0, 1.4*max_bin(vH)});
  for (int i = 0; i < vInfo.size(); i++)
    c->give_info(vInfo[i]);
  c->plot(idx, false);
  
  delete c;
}

void compare_vetos(TCanvas* c1, TFile* file, string output_name, TString process, TString hist, TString x_label, vector<TString> vInfo, int idx) {
  TH1D* hRemovedByEVeto = (TH1D*)file->Get(hist + "_failEVeto_" + process);
  TH1D* hRemovedByPSV = (TH1D*)file->Get(hist + "_failPSV_" + process);

  vector<TH1D*> vH = {hRemovedByEVeto, hRemovedByPSV};
  Comparison* c = new Comparison(c1, vH);
  c->set_filename(output_name);
  c->set_x_label(x_label);
  c->set_y_label("Fraction");
  c->set_legend_labels({"Removed by e Veto", "Removed by PS Veto"});
  c->give_info("ttH Leptonic Tag");
  c->set_scale(-1);
  c->set_no_lumi();
  c->set_no_log();
  c->set_y_lim_range({0, 1.4*max_bin(vH)});
  for (int i = 0; i < vInfo.size(); i++)
    c->give_info(vInfo[i]);
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
  //hOtherBkg->Add((TH1D*)file->Get(hist_name + "_QCD"));

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
 
  c->give_info("ttH Leptonic");
  c->give_info("t#bar{t}-Enriched Region");
  if (!postfit)
    c->give_info("Pre-Fit");
  else
    c->give_info("Post-Fit");

  c->set_no_log();
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
  TFile* f0 = new TFile("../ttbar_cr_histograms.root");
  //TFile* f1 = new TFile("../ttbar_cr_v2_histograms.root");
  TFile* f1 = new TFile("../ttHLeptonic_ttbarCR_4Mar2019_histogramsAll.root");

  TFile* f_veto_study = new TFile("../ttHLeptonic_veto_study_histograms.root");

  compare_templates(c1, f, "ttHLeptonic_template_plots_Min.pdf", "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", 1); 
  compare_templates(c1, f, "ttHLeptonic_template_plots_Max.pdf", "hPhotonMaxIDMVA_coarse", "Max #gamma ID MVA", 1);
  compare_templates(c1, f, "ttHLeptonic_template_plots_nJets.pdf", "hNJets", "N_{jets}", 1);
  compare_templates(c1, f, "ttHLeptonic_template_plots_nBJets.pdf", "hNbJets", "N_{b-jets (med)}", 1);

  compare_templates(c1, f1, "ttHLeptonic_ttbarCR_template_plots_Min.pdf", "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", 1);
  compare_templates(c1, f1, "ttHLeptonic_ttbarCR_template_plots_Max.pdf", "hPhotonMaxIDMVA_coarse", "Max #gamma ID MVA", 1);
  compare_templates(c1, f1, "ttHLeptonic_ttbarCR_template_plots_nJets.pdf", "hNJets", "N_{jets}", 1);
  compare_templates(c1, f1, "ttHLeptonic_ttbarCR_template_plots_nBJets.pdf", "hNbJets", "N_{b-jets (med)}", 1);
 
  //vector<TString> vBkgs = {"TTGGGenPhoton_2", "TTGGGenPhoton_1", "TTJetsGenPhoton_0", "DiPhoton", "GammaJets", "VG", "WJets", "QCD"};
  vector<TString> vBkgs = {"TTGGGenPhoton_2", "TTGJetsGenPhoton_1", "TTJetsGenPhoton_0"};
  string prefit_output = "ttHLeptonic_template_prefit.pdf";
  make_prefit_plot(c1, f1, prefit_output, "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", vBkgs);
  //make_prefit_plot(c1, f1, prefit_output, "hNJets", "N_{jets}", vBkgs, 1);
  //make_prefit_plot(c1, f1, prefit_output, "hNbJets", "N_{b-jets}", vBkgs, 2);

  //make_prefit_plot(c1, f1, "ttHLeptonic_template_postfit.pdf", "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", vBkgs, {3.5885351626194707e-07, 119.64862304327923, 0.5384692738360889, 3.826623401711424e-07});
  //make_prefit_plot(c1, f1, "ttHLeptonic_template_postfit.pdf", "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", vBkgs, {0.00014332822051296823, 160.80686724029272, 0.4331537473237817, 3.334613563392507e-06});

  make_prefit_plot(c1, f1, "ttHLeptonic_template_postfit.pdf", "hPhotonMinIDMVA_coarse", "Min #gamma ID MVA", vBkgs, {1.449702793941148, 1.0066722244162387, 0.9999999714826685, 1.0000000495976293});


  //compare_photon_origins(c1, f_veto_study, "ttHLeptonic_plots_minIDShapes.pdf", "TTGJets", "Minimum #gamma ID MVA", {"No Vetos Applied", "t#bar{t} + #gamma + Jets"}, 1);
  //compare_vetos(c1, f_veto_study, "ttHLeptonic_plots_minIDbyVeto.pdf", "TTGJets", "Mininum #gamma ID MVA", {"t#bar{t} + #gamma + Jets"}, 0);
  //compare_vetos(c1, f_veto_study, "ttHLeptonic_plots_minIDbyVeto.pdf", "Data", "Mininum #gamma ID MVA", {"Data"}, 2);

  /*
  compare_photon_origins(c1, f_veto_study, "ttHLeptonic_plots_IDShapes.pdf", "TTGJets", "hPhotonIDMVA", "#gamma ID MVA", {"No Vetos Applied", "t#bar{t} + #gamma + Jets"}, 0);
  compare_photon_origins(c1, f_veto_study, "ttHLeptonic_plots_IDShapes.pdf", "TTGJets", "hPhotonPt", "p_{T} [GeV]", {"No Vetos Applied", "t#bar{t} + #gamma + Jets"}, 1);
  compare_photon_origins(c1, f_veto_study, "ttHLeptonic_plots_IDShapes.pdf", "TTGJets", "hPhotonEta", "#eta", {"No Vetos Applied", "t#bar{t} + #gamma + Jets"}, 2);

  compare_vetos(c1, f_veto_study, "ttHLeptonic_plots_IDbyVeto.pdf", "TTGJets", "hPhotonIDMVA", "#gamma ID MVA", {"t#bar{t} + #gamma + Jets"}, 0);
  compare_vetos(c1, f_veto_study, "ttHLeptonic_plots_IDbyVeto.pdf", "Data", "hPhotonIDMVA", "#gamma ID MVA", {"Data"}, 1);
  compare_vetos(c1, f_veto_study, "ttHLeptonic_plots_IDbyVeto.pdf", "TTGJets", "hPhotonPt", "p_{T} [GeV]", {"t#bar{t} + #gamma + Jets"}, 1);
  compare_vetos(c1, f_veto_study, "ttHLeptonic_plots_IDbyVeto.pdf", "Data", "hPhotonPt", "p_{T} [GeV]", {"Data"}, 1);
  compare_vetos(c1, f_veto_study, "ttHLeptonic_plots_IDbyVeto.pdf", "TTGJets", "hPhotonEta", "#eta", {"t#bar{t} + #gamma + Jets"}, 1);
  compare_vetos(c1, f_veto_study, "ttHLeptonic_plots_IDbyVeto.pdf", "Data", "hPhotonEta", "#eta", {"Data"}, 2); 
  */
}
