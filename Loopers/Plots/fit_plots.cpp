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
	{"GammaJets_MadgraphGenPhoton_1", "#gamma + Jets (F/P) (Madgraph)"},
        {"DiPhotonGenPhoton_2", "#gamma#gamma + Jets (P/P)"},
	{"DiPhotonGenPhoton_1", "#gamma#gamma + Jets (F/P)"},
	{"QCD_GammaJets_imputed", "(#gamma) + Jets (Data)"}
};

std::map<TString, int> mColors = {
        {"DY", kRed - 7},
        {"DiPhotonGenPhoton_2", kAzure+1},
	{"DiPhoton", kAzure+1},
	{"DiPhotonGenPhoton_1", kRed},
        {"GammaJetsGenPhoton_1", kViolet -4},
	{"GammaJets_MadgraphGenPhoton_1", kViolet-4},
        {"QCDGenPhoton_0", kCyan-7},
	{"QCDGenPhoton_1", kOrange},
        {"QCD", kCyan-7},
        {"TTGG", kOrange+1},
        {"TTGGGenP", kTeal+10},
        {"TTGJets", kTeal+10},
        {"VG", kGreen+3},
        {"WJets", kBlue+2},
        {"TTJets", kPink-2},
	{"QCD_GammaJets_imputed", kGray},
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
  c->set_lumi(77.4);
 
  c->give_info("ttH Leptonic");
  c->give_info("Signal Region");
  //c->give_info("2017 Preselection"
  for (int i = 0; i < vInfo.size(); i++)
    c->give_info(vInfo[i]);
  //c->give_info("t#bar{t}-Enriched Region");
  if (!postfit)
    c->give_info("Pre-Fit");
  else
    c->give_info("Post-Fit");

  c->set_no_log();
  c->set_lower_lim(0.0);
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

  bool hadronic = false;
  bool impute = false;

  if (impute) {
    //TFile* f1 = new TFile("../ttHHadronicLoose_impute_impute_with_kinematics_28Feb2019_histogramsAll.root");
    TFile* f1 = new TFile("../ttHHadronicLoose_impute_21Feb2019_histogramsAll.root");
    TString ff_template = "QCD";
    TString fp_template = "QCD_GammaJets_imputed";
    TString pp_template = "DiPhoton";
    vector<TString> vOtherBkgs = {"DY", "TTGG", "TTGJets", "TTJets", "VG"};
    //vector<double> scales_1 = {0.0, 1.0277753515953971, 1.2003632049757942, 1.000000352557614}; // imputing with kinematics
    vector<double> scales_1 = {0.0, 0.9604070590596785, 1.1882196044384912, 1.0000003527469943}; // imputing without kinematics
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hPhotonMinIDMVA_fine", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 0); 
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hPhotonMaxIDMVA_fine", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hNJets", "N_{jets}", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hNbMedium", "N_{b-jets} (medium)", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hMinIDPhotonPt", "Min #gamma ID p_{T} [GeV]", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1); 
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hMaxIDPhotonPt", "Max #gamma ID p_{T} [GeV]", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hMinIDPhotonEta", "Min #gamma ID |#eta|", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hMaxIDPhotonEta", "Max #gamma ID |#eta|", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);

    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hPhotonMinIDMVA_fine", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1); 
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hPhotonMaxIDMVA_fine", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hNJets", "N_{jets}", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hNbMedium", "N_{b-jets} (medium)", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hMinIDPhotonPt", "Min #gamma ID p_{T} [GeV]", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hMaxIDPhotonPt", "Max #gamma ID p_{T} [GeV]", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hMinIDPhotonEta", "Min #gamma ID |#eta|", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "qcd_impute_fits.pdf", "hMaxIDPhotonEta", "Max #gamma ID |#eta|", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 2, scales_1);



  }
  else if (hadronic) {
    TFile* f1 = new TFile("../ttHHadronic_QCDFits_Presel__histograms2017.root");

    TString ff_template = "QCDGenPhoton_0";
    TString fp_template = "GammaJetsGenPhoton_1";
    TString pp_template = "DiPhotonGenPhoton_2";
    vector<TString> vOtherBkgs = {"DY", "TTGG", "TTGJets", "TTJets", "VG"};

    vector<double> scales_1 = {2.5870951952922523, 1.8534316644570374, 1.0721464900550817, 1.0000011136519826}; // fine binning, Pythia

    make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA_fine", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 0); 
    make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMaxIDMVA_fine", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "qcd_fits.pdf", "hNJets", "N_{jets}", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "qcd_fits.pdf", "hNbMedium", "N_{b-jets} (medium)", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
   

    make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMinIDMVA_fine", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1); 
    make_prefit_plot(c1, f1, "qcd_fits.pdf", "hPhotonMaxIDMVA_fine", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "qcd_fits.pdf", "hNJets", "N_{jets}", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "qcd_fits.pdf", "hNbMedium", "N_{b-jets} (medium)", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 2, scales_1);
  }
  else {
    //TFile* f1 = new TFile("../ttHLeptonic_ttbarCR_4Mar2019_histogramsAll.root");
    TFile* f1 = new TFile("../ttHLeptonic_baseline_maxZA_5Mar2019_histogramsAll.root");    

    TString ff_template = "TTJets";
    TString fp_template = "TTGJets";
    TString pp_template = "TTGG";
    vector<TString> vOtherBkgs = {"DY", "VG", "TGamma", "DiPhoton", "GammaJets"}; 

    vector<double> scales_1 = {2.3203374836532866, 1.1934635223213481, 1.4134660734426867, 0.9999999246642024};

    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 0);
    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hPhotonMaxIDMVA", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hNJets", "N_{jets}", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hNbMedium", "N_{b-jets} (medium)", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1);
    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hPixelSeed", "Pixel Seed Veto", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1); 
 

    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hPhotonMinIDMVA", "Min. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hPhotonMaxIDMVA", "Max. #gamma ID MVA", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hNJets", "N_{jets}", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hNbMedium", "N_{b-jets} (medium)", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 1, scales_1);
    make_prefit_plot(c1, f1, "ttX_fits.pdf", "hPixelSeed", "Pixel Seed Veto", {ff_template, fp_template, pp_template}, vOtherBkgs, {}, 2, scales_1);
  }

}
