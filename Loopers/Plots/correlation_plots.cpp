#include "PlotHelper.h"
#include <iomanip>

void print(TCanvas* c1, string filename, int idx) {
  if (idx == 0)
    c1->Print((filename+"(").c_str());
  else if (idx == 1)
    c1->Print((filename).c_str());
  else if (idx == 2)
    c1->Print((filename+")").c_str());
  c1->Clear("D");
}

void make_correlation_plot(TCanvas* c1, string filename, TFile* f, TString process, TString hist_name, TString x_label, TString y_label, int idx) {
  TH2D* h = (TH2D*)f->Get(hist_name + "_" + process);
  h->Draw("COLZ");
  h->SetStats(0);
  h->GetXaxis()->SetTitle(x_label);
  h->GetYaxis()->SetTitle(y_label);

  TLatex* cms = new TLatex(0.14, 0.91, "CMS Preliminary");
  cms->SetTextSize(0.04);
  cms->SetNDC(kTRUE);
  cms->Draw("SAME");

  double corr = h->GetCorrelationFactor();
  TString corr_ = "r = " + to_string(corr);
  TLatex* correlation = new TLatex(0.16, 0.85, corr_); 
  correlation->SetTextSize(0.04);
  correlation->SetNDC(kTRUE);
  correlation->Draw("SAME");

  /*
  TLatex* mc = new TLatex(0.63, 0.85, "#gamma + Jets (MC)");
  mc->SetTextSize(0.04);
  mc->SetNDC(kTRUE);
  mc->Draw("SAME");
  */

  c1->SetLeftMargin(0.13);
  c1->SetRightMargin(0.13);
  c1->SetLogz();

  print(c1, filename, idx); 

  delete h;
  delete cms;
  delete correlation;
  //delete mc;

  return;
}

int main(int argc, char* argv[]) {

  if (argc < 2) {
    cout << "Please provide input file" << endl;  
    return 1;
  }

  TString file = argv[1];
  TFile* f = new TFile(file);
  TCanvas* c1 = new TCanvas("c1", "histos", 800, 800);
  string filename = "correlation_plots.pdf";

  /*
  make_correlation_plot(c1, filename, f, "GammaJets", "hFakePhotonIDMVA_NJets", "#gamma ID MVA (Fakes)", "N_{jets}", 0);
  make_correlation_plot(c1, filename, f, "GammaJets", "hFakePhotonIDMVA_MaxBTag", "#gamma ID MVA (Fakes)", "Max B-tag", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hFakePhotonIDMVA_2ndMaxBTag", "#gamma ID MVA (Fakes)", "2nd Max B-tag", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hFakePhotonIDMVA_DiPhotonPt", "#gamma ID MVA (Fakes)", "DiPhoton p_{T} [GeV]", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hFakePhotonIDMVA_Pt", "#gamma ID MVA (Fakes)", "Photon p_{T} [GeV]", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hFakePhotonIDMVA_Eta", "#gamma ID MVA (Fakes)", "Photon |#eta|", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hPromptPhotonIDMVA_NJets", "#gamma ID MVA (Prompts)", "N_{jets}", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hPromptPhotonIDMVA_MaxBTag", "#gamma ID MVA (Prompts)", "Max B-tag", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hPromptPhotonIDMVA_2ndMaxBTag", "#gamma ID MVA (Prompts)", "2nd Max B-tag", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hPromptPhotonIDMVA_DiPhotonPt", "#gamma ID MVA (Prompts)", "DiPhoton p_{T} [GeV]", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hPromptPhotonIDMVA_Pt", "#gamma ID MVA (Prompts)", "Photon p_{T} [GeV]", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hPromptPhotonIDMVA_Eta", "#gamma ID MVA (Prompts)", "Photon |#eta|", 1);

  make_correlation_plot(c1, filename, f, "GammaJets", "hPhotonLeadIDMVA_LeadPt", "Lead #gamma ID MVA", "Lead #gamma p_{T} [GeV]", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hPhotonSubleadIDMVA_SubleadPt", "Sublead #gamma ID MVA", "Sublead #gamma p_{T} [GeV]", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hPhotonLeadIDMVA_LeadEta", "Lead #gamma ID MVA", "Lead #gamma |#eta|", 1);
  make_correlation_plot(c1, filename, f, "GammaJets", "hPhotonSubleadIDMVA_SubleadEta", "Sublead #gamma ID MVA", "Sublead #gamma |#eta|", 2);
  */

  make_correlation_plot(c1, filename, f, "Data", "hBDT_LeadPtoM", "MVA Score", "Lead #gamma p_{T}/m_{#gamma#gamma}", 0);
  make_correlation_plot(c1, filename, f, "Data", "hBDT_SubleadPtoM", "MVA Score", "Sublead #gamma p_{T}/m_{#gamma#gamma}", 1);
  make_correlation_plot(c1, filename, f, "Data", "hMass_LeadPtoM", "m_{#gamma#gamma} [GeV]", "Lead #gamma p_{T}/m_{#gamma#gamma}", 1);
  make_correlation_plot(c1, filename, f, "Data", "hMass_SubleadPtoM", "m_{#gamma#gamma} [GeV]", "Sublead #gamma p_{T}/m_{#gamma#gamma}", 1); 
  make_correlation_plot(c1, filename, f, "Data", "hMass_LeadPtoM_afterBDTCut", "m_{#gamma#gamma} [GeV]", "Lead #gamma p_{T}/m_{#gamma#gamma}", 1);
  make_correlation_plot(c1, filename, f, "Data", "hMass_SubleadPtoM_afterBDTCut", "m_{#gamma#gamma} [GeV]", "Sublead #gamma p_{T}/m_{#gamma#gamma}", 2);
  return 0;
}
