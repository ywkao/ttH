#ifndef _PLOTHELPER_H_
#define _PLOTHELPER_H_

#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>

// ROOT
#include "TString.h"
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLine.h"
#include "TArrow.h"
#include "TPad.h"
#include "TLatex.h"
#include "TFrame.h"
#include "TGaxis.h"

using namespace std;

const vector<int> dataColors = {kBlack, kRed, kGreen, kBlue, kGray, kTeal};

class Comparison 
{
  public:
    Comparison(TCanvas* c1, TH1D* hData, TH1D* hMC);
    Comparison(TCanvas* c1, vector<TH1D*> hData, TH1D* hMC);
    Comparison(TCanvas* c1, TH1D* hData, vector<TH1D*> hMC);
    Comparison(TCanvas* c1, TH1D* hData, TH1D* hSignal, vector<TH1D*> hMC);
    Comparison(TCanvas* c1, vector<TH1D*> hData, TH1D* hSignal, vector<TH1D*> hMC);
    Comparison(TCanvas* c1, vector<TH1D*> hData, vector<TH1D*> hSignal, vector<TH1D*> hMC);
    Comparison(TCanvas* c1, vector<TH1D*> hData, vector<TH1D*> hSignal, vector<TH1D*> hMC, vector<TH1D*> hMCSyst_up, vector<TH1D*> hMCSyst_down);
    Comparison(TCanvas* c1, TH2D* hData, TH2D* hMC);
    Comparison(TCanvas* c1, vector<TH1D*> hData);
    ~Comparison();

    void plot(int idx, bool compare = true); 

    // Setters
    void set_filename(string filename) { mFilename = filename; TString f = filename; if (f.Contains("linear")) { mIndentCms = true; }}
    void set_x_bin_range(vector<int> xBinRange) { mCustomXRange = true; mXBinRange = xBinRange; }
    void set_y_bin_range(vector<int> yBinRange) { mCustomX2Range = true; mX2BinRange = yBinRange; }
    void set_y_lim_range(vector<double> yLimRange) { mCustomYRange = true; mYLimRange = yLimRange; }
    void set_rat_lim_range(vector<double> ratLimRange) { mCustomRatRange = true; mRatRange = ratLimRange; }
    void set_x_label(TString xLabel) {mXLabel = xLabel; }
    void set_y_label(TString yLabel) {mYLabel = yLabel; }
    void set_y_label(TString yLabel, double fontSize) { mYLabel = yLabel; mYLabelFontSize = fontSize; }
    void set_rat_label(TString ratLabel) {mRatLabel = ratLabel; }
    void set_info(vector<TString> vInfo) {mVInfo = vInfo;}
    void set_legend_labels(vector<TString> vLegendLabels) {mVLegendLabels = vLegendLabels;} 
    void set_scale(double scale) { mScale = scale; }
    void set_colors(int color1, int color2) { mColor1 = color1; mColor2 = color2; }
    void set_colors(vector<int> colorData) { mColorData = colorData; }
    void set_no_flow() { mFlow = false; }
    void set_no_underflow() {mUnderFlow = false; }
    void set_verbose() {mVerbose = true;}
    void set_no_log() { mLog = false; }
    void set_log_x() { mLogX = true; }
    void set_log_y() { mLogY = true; }
    void set_log_rat() { mLogRat = true; }
    void set_both_data() { mBothData = true; }
    void set_lumi(double lumi) {mLumi = lumi;}
    void set_no_lumi() {mLumi = -1;}
    void set_2DdrawOpt(TString drawOpt) {m2DDrawOpt = drawOpt;}
    void set_data_drawOpt(TString drawOpt) {mDataDrawOpt = drawOpt;}
    void set_stack_order(vector<int> vOrder) {mCustomStackOrder = true; mVOrder = vOrder;}
    void set_lower_lim(double value) {mCustomLowerLimit = true; mLowerLimit = value;}
    void set_no_ratio() { mRatio = false; }
    void add_paper_info(TString channel) {mPaperInfo = true; mChannel = channel;}
    void set_paper_style() { mPaperStyle = true; }
    void set_no_legend() { mDrawMainLegend = false; }

    void give_hlines(vector<double> vHLines) {mVHLines = vHLines;}
    void give_vlines(vector<double> vVLines) {mVVLines = vVLines;}
    void give_vlines_dotted(vector<double> vVLines) {mVVLines_dotted = vVLines;}
    void give_vshade(vector<double> vShade)  {mVShade = vShade;}
    void give_info(TString info) {mVInfo.push_back(info); }

    void set_pad_coords(double x1, double x2, double y1, double y2) { mCustomPad = true; mPadX1 = x1; mPadX2 = x2; mPadY1 = y1; mPadY2 = y2; }
    void suppress_x_label() { mSuppressXLabel = true; }
    void suppress_top() { mSuppressTop = true; }
    void skip_signal() { mSkipSignal = true; }
    void skip_cp() { mSkipCP = true; }

  private:
    void default_options(TCanvas* c1);
    void set_main_pad(TPad* mainPad, bool log, bool rat = true);
    void set_rat_pad(TPad* ratPad);
    void set_2d_pad(TPad* pad, bool log);

    void plotNoCompare(int idx);
    void plot1D(int idx);
    void plot2D(int idx);

    void calculate_systematics();

    void compute_limits(bool customXRange, bool customYRange);
    void compute_limits(bool customXRange, bool customYRange, bool customZRange);
    void compute_flow(vector<int> xBinRange);
    void set_histogram_options(int color1, int color2);
    void draw_histograms();
    void draw_main_histograms();
    void draw_2D_histograms(int idx);
    void annotate_plot();
    void make_rat_histogram(TH1D* hData, TH1D* hMC);
    void print(int idx);

    TCanvas* mCanvas;
    TPad* mMainPad;
    TPad* mRatPad;
    string mFilename;
    
    TH1D* mHData;
    vector<TH1D*> mVHData;
    TH1D* mHMC;
    vector<TH1D*> mVHMC;
    TH1D* mHRat;
    vector<TH1D*> mVHRat;
    vector<TH1D*> mVHSignal;

    bool mDoSystBand;
    vector<TH1D*> mVHMCSyst_up;
    vector<TH1D*> mVHMCSyst_down;
    TH1D* hMC_TotalSyst_up;
    TH1D* hMC_TotalSyst_down;
    TH1D* hMC_TotalSyst_up_noPlot;
    TH1D* hMC_TotalSyst_down_noPlot;
    TH1D* hRat_TotalSyst_up;
    TH1D* hRat_TotalSyst_down;
    TH1D* hMC_StatUnc_up;
    TH1D* hMC_StatUnc_down;
    TH1D* hRat_StatUnc_up;
    TH1D* hRat_StatUnc_down; 

    int mSystColor;
    int mStatColor;

    THStack* mStack;
    THStack* mStack2;

    TH2D* mH2DData;
    TH2D* mH2DMC;
    TH2D* mH2DRat;
    TString m2DDrawOpt;
    TString mDataDrawOpt;
    TString mSignalDrawOpt;

    bool mPaperInfo;
    TString mChannel;

    bool m2D;
    bool mMultipleData;

    bool mCustomXRange;
    vector<int> mXBinRange;
    bool mCustomYRange;
    vector<double> mYLimRange;
    bool mCustomX2Range;
    vector<int> mX2BinRange;
    bool mCustomRatRange;
    vector<double> mRatRange;
    bool mCustomStackOrder;
    vector<int> mVOrder;

    bool mCustomLowerLimit;
    double mLowerLimit;

    vector<double> mVHLines;
    vector<double> mVVLines;
    vector<double> mVVLines_dotted;
    vector<double> mVShade;

    TString mXLabel;
    TString mYLabel;
    double mYLabelFontSize;
    TString mRatLabel;
    vector<TString> mVInfo;
    vector<TString> mVLegendLabels;
    bool mDrawMainLegend;

    bool mVerbose;

    double mScale;
    bool mScaled;    
    double mLumi;

    bool mLog;
    bool mLogX;
    bool mLogY;
    bool mLogRat;
    bool mFlow;
    bool mUnderFlow;
    bool mBothData;
    bool mRatio;
    bool mRatUseData;

    int mColor1;
    int mColor2;
    vector<int> mColorData;

    const double topSpace = 0.45;
    const double botSpace = 0.05;
    const double fs = 0.0475;
    const double fs_paper = 0.055;

    bool mCustomPad;
    double mPadX1;
    double mPadX2;
    double mPadY1;
    double mPadY2;

    bool mSuppressXLabel;
    bool mSuppressTop;
    bool mPaperStyle;

    bool mIndentCms;

    bool mSkipSignal;
    bool mSkipCP;
};


inline
Comparison::~Comparison()
{
  if (!m2D) {
    for (int i=0; i<mVHData.size(); i++){
      delete mVHData[i];
    }
    for (int i=0; i<mVHRat.size(); i++)
      delete mVHRat[i];
    if (mVHMC.size() > 0)
      delete mHMC;  
  }
  else {
    delete mH2DData;
    delete mH2DMC; 
  }
}

inline
Comparison::Comparison(TCanvas* c1, TH1D* hData, TH1D* hMC) 
{
  default_options(c1);
  mVHData.push_back((TH1D*)hData->Clone("mHData"));
  //mVHRat.push_back((TH1D*)hData->Clone("mHRat"));
  mHMC = (TH1D*)hMC->Clone("mHMC");
}

inline
Comparison::Comparison(TCanvas* c1, vector<TH1D*> hData, TH1D* hMC)
{
  default_options(c1);
  int nDataHists = hData.size();
  for (int i=0; i<nDataHists; i++) {
    TString idx = Form("%d", i);
    mVHData.push_back((TH1D*)hData[i]->Clone("mHData"+idx));
    //mVHRat.push_back((TH1D*)hData[i]->Clone("mHRat"+idx));
  }

  mHMC = (TH1D*)hMC->Clone("mHMC");
}

inline
Comparison::Comparison(TCanvas* c1, TH1D* hData, vector<TH1D*> hMC)
{
  default_options(c1);
  mVHData.push_back((TH1D*)hData->Clone("mHData"));

  int nMCHists = hMC.size();
  for (int i=0; i<nMCHists; i++) {
    TString idx = Form("%d", i);
    mVHMC.push_back((TH1D*)hMC[i]->Clone("mHMC"+idx));
    //mVHRat.push_back((TH1D*)hData[i]->Clone("mHRat"+idx));
  }

  mHMC = (TH1D*)hMC[0]->Clone("mHMC");
  for (int i=1; i<nMCHists; i++)
    mHMC->Add(hMC[i]);
}

inline
Comparison::Comparison(TCanvas* c1, TH1D* hData, TH1D* hSignal, vector<TH1D*> hMC)
{
  default_options(c1);
  mVHData.push_back((TH1D*)hData->Clone("mHData"));

  int nMCHists = hMC.size();
  for (int i=0; i<nMCHists; i++) {
    TString idx = Form("%d", i);
    mVHMC.push_back((TH1D*)hMC[i]->Clone("mHMC"+idx));
    //mVHRat.push_back((TH1D*)hData[i]->Clone("mHRat"+idx));
  }

  mHMC = (TH1D*)hMC[0]->Clone("mHMC");
  for (int i=1; i<nMCHists; i++)
    mHMC->Add(hMC[i]);

  mVHSignal.push_back((TH1D*)hSignal->Clone("mHSignal"));
}

inline
Comparison::Comparison(TCanvas* c1, vector<TH1D*> hData, TH1D* hSignal, vector<TH1D*> hMC)
{
  default_options(c1);
  //mVHData.push_back((TH1D*)hData->Clone("mHData"));
  for (int i = 0; i < hData.size(); i++) {
    TString name = "mHData" + to_string(i);
    mVHData.push_back((TH1D*)hData[i]->Clone(name));
  }
  
  int nMCHists = hMC.size();
  for (int i=0; i<nMCHists; i++) {
    TString idx = Form("%d", i);
    mVHMC.push_back((TH1D*)hMC[i]->Clone("mHMC"+idx));
    //mVHRat.push_back((TH1D*)hData[i]->Clone("mHRat"+idx));
  }

  mHMC = (TH1D*)hMC[0]->Clone("mHMC");
  for (int i=1; i<nMCHists; i++)
    mHMC->Add(hMC[i]);

  mVHSignal.push_back((TH1D*)hSignal->Clone("mHSignal"));
}

inline
Comparison::Comparison(TCanvas* c1, vector<TH1D*> hData, vector<TH1D*> hSignal, vector<TH1D*> hMC)
{
  default_options(c1);
  //mVHData.push_back((TH1D*)hData->Clone("mHData"));
  for (int i = 0; i < hData.size(); i++) {
    TString name = "mHData" + to_string(i);
    mVHData.push_back((TH1D*)hData[i]->Clone(name));
  }

  int nMCHists = hMC.size();
  for (int i=0; i<nMCHists; i++) {
    TString idx = Form("%d", i);
    mVHMC.push_back((TH1D*)hMC[i]->Clone("mHMC"+idx));
    //mVHRat.push_back((TH1D*)hData[i]->Clone("mHRat"+idx));
  }

  mHMC = (TH1D*)hMC[0]->Clone("mHMC");
  for (int i=1; i<nMCHists; i++)
    mHMC->Add(hMC[i]);

  for (int i = 0; i < hSignal.size(); i++) {
    TString idx = Form("%d", i);
     mVHSignal.push_back((TH1D*)hSignal[i]->Clone("mHSignal" + idx));
  }
}

inline
Comparison::Comparison(TCanvas* c1, vector<TH1D*> hData, vector<TH1D*> hSignal, vector<TH1D*> hMC, vector<TH1D*> hMCSyst_up, vector<TH1D*> hMCSyst_down)
{
  default_options(c1);
  //mVHData.push_back((TH1D*)hData->Clone("mHData"));
  for (int i = 0; i < hData.size(); i++) {
    TString name = "mHData" + to_string(i);
    mVHData.push_back((TH1D*)hData[i]->Clone(name));
  }

  int nMCHists = hMC.size();
  for (int i=0; i<nMCHists; i++) {
    TString idx = Form("%d", i);
    mVHMC.push_back((TH1D*)hMC[i]->Clone("mHMC"+idx));
    //mVHRat.push_back((TH1D*)hData[i]->Clone("mHRat"+idx));
  }

  mHMC = (TH1D*)hMC[0]->Clone("mHMC");
  for (int i=1; i<nMCHists; i++)
    mHMC->Add(hMC[i]);

  for (int i = 0; i < hSignal.size(); i++) {
    TString idx = Form("%d", i);
     mVHSignal.push_back((TH1D*)hSignal[i]->Clone("mHSignal" + idx));
  }

  for (int i = 0; i < hMCSyst_up.size(); i++) {
    TString idx = Form("%d", i);
    mVHMCSyst_up.push_back((TH1D*)hMCSyst_up[i]->Clone("mHMCSystUp" + idx));
    mVHMCSyst_down.push_back((TH1D*)hMCSyst_down[i]->Clone("mHMCSystDown" + idx)); 
  } 

  mDoSystBand = true; 

}

inline
Comparison::Comparison(TCanvas* c1, TH2D* hData, TH2D* hMC)
{
  default_options(c1);
  mH2DData = (TH2D*)hData->Clone("mH2DData");
  mH2DMC = (TH2D*)hMC->Clone("mH2DMC");
  m2D = true;

  mLogY = false;

  m2DDrawOpt = "COLZ2,TEXT";
}

inline
Comparison::Comparison(TCanvas* c1, vector<TH1D*> hData)
{
  default_options(c1);
  int nDataHists = hData.size();
  for (int i=0; i<nDataHists; i++) {
    TString idx = Form("%d", i);
    mVHData.push_back((TH1D*)hData[i]->Clone("mHData"+idx)); 
  }
}

inline
void Comparison::calculate_systematics()
{
    vector<double> stat_unc_up((mXBinRange[1] - mXBinRange[0]) + 3, 0);
    vector<double> stat_unc_down((mXBinRange[1] - mXBinRange[0]) + 3, 0);

    vector<double> total_unc_up((mXBinRange[1] - mXBinRange[0]) + 3, 0);
    vector<double> total_unc_down((mXBinRange[1] - mXBinRange[0]) + 3, 0);    

    // Calculate up/down variations for each systematic in each bin
    for (int i = 0; i < mVHMCSyst_up.size(); i++) {
        for (int j = 0; j < (mXBinRange[1] - mXBinRange[0]) + 3; j++) {
            int idx = mXBinRange[0] + (j - 1);
            double variation_up;

            if (mHMC->GetBinContent(idx) >= mVHMCSyst_up[i]->GetBinContent(idx) && mHMC->GetBinContent(idx) >= mVHMCSyst_down[i]->GetBinContent(idx))
                variation_up = 0;
            else {
                double difference_up = mVHMCSyst_up[i]->GetBinContent(idx) - mHMC->GetBinContent(idx);
                double difference_down = mVHMCSyst_down[i]->GetBinContent(idx) - mHMC->GetBinContent(idx);
                variation_up = difference_up > difference_down ? difference_up : difference_down;
            }
            
            double variation_down;
            if (mHMC->GetBinContent(idx) <= mVHMCSyst_up[i]->GetBinContent(idx) && mHMC->GetBinContent(idx) <= mVHMCSyst_down[i]->GetBinContent(idx))
                variation_down = 0;
            else {
                double difference_up = mVHMCSyst_up[i]->GetBinContent(idx) - mHMC->GetBinContent(idx);
                double difference_down = mVHMCSyst_down[i]->GetBinContent(idx) - mHMC->GetBinContent(idx);
                variation_down = difference_down < difference_up ? difference_down : difference_up; 
            }
            total_unc_up[j] += pow(variation_up, 2); // add all variations in quadrature
            total_unc_down[j] += pow(variation_down, 2);
            //cout << "Bin " << idx << "/" << mXBinRange[1] - mXBinRange[0] << ": " << variation_up << ", " << variation_down << endl;
        } 
    }

    // Add in stat unc.
    for (int i = 0; i < (mXBinRange[1] - mXBinRange[0]) + 3; i++) {
        int idx = mXBinRange[0] + (i - 1);
        double stat_unc = mHMC->GetBinError(idx);
        stat_unc_up[i] = stat_unc/2.;
        stat_unc_down[i] = stat_unc/2.;
        total_unc_up[i] += pow(stat_unc/2., 2);
        total_unc_down[i] += pow(stat_unc/2., 2);
    }

    for (int i = 0; i < total_unc_up.size(); i++) {
        total_unc_up[i] = pow(total_unc_up[i], 0.5); // take sqrt
        total_unc_down[i] = pow(total_unc_down[i], 0.5);
        //cout << "Bin " << i + 1 << "/" << total_unc_up.size() << ": " << total_unc_up[i] << ", " << total_unc_down[i] << endl;
    }

    hMC_TotalSyst_up = (TH1D*)mHMC->Clone("mHMC_syst_up");
    hMC_TotalSyst_up_noPlot = (TH1D*)mHMC->Clone("mHMC_syst_up_noPlot");
    hMC_TotalSyst_down = (TH1D*)mHMC->Clone("mHMC_syst_down");   
    hMC_TotalSyst_down_noPlot = (TH1D*)mHMC->Clone("mHMC_syst_down_noPlot");

    hMC_StatUnc_up = (TH1D*)mHMC->Clone("mHMC_stat_up");
    hMC_StatUnc_down = (TH1D*)mHMC->Clone("mHMC_stat_down"); 
    hRat_StatUnc_up = (TH1D*)mHMC->Clone("mHMC_rat_up");
    hRat_StatUnc_down = (TH1D*)mHMC->Clone("mHMC_rat_down");

    for (int i = 0; i < (mXBinRange[1] - mXBinRange[0]) + 3; i++) {
        int idx = mXBinRange[0] + (i - 1);

        hMC_StatUnc_up->SetBinContent(idx, mHMC->GetBinContent(idx) + (stat_unc_up[i]/2.));
        hMC_StatUnc_up->SetBinError(idx, stat_unc_up[i]/2.);

        hMC_StatUnc_down->SetBinContent(idx, mHMC->GetBinContent(idx) + (stat_unc_down[i]/2.));
        hMC_StatUnc_down->SetBinError(idx, stat_unc_down[i]/2.); 

        hMC_TotalSyst_up_noPlot->SetBinContent(idx, mHMC->GetBinContent(idx) + total_unc_up[i]);
        hMC_TotalSyst_up->SetBinContent(idx, mHMC->GetBinContent(idx) + (total_unc_up[i]/2.));
        hMC_TotalSyst_up->SetBinError(idx, total_unc_up[i]/2.);

        hMC_TotalSyst_down_noPlot->SetBinContent(idx, mHMC->GetBinContent(idx) - total_unc_down[i]);
        hMC_TotalSyst_down->SetBinContent(idx, mHMC->GetBinContent(idx) - (total_unc_down[i]/2.));
        hMC_TotalSyst_down->SetBinError(idx, total_unc_down[i]/2.);
    }

    hRat_TotalSyst_up = (TH1D*)mVHData[0]->Clone("mRatSyst_up");
    hRat_TotalSyst_up->Divide(hMC_TotalSyst_up_noPlot);

    hRat_TotalSyst_down = (TH1D*)mVHData[0]->Clone("mRatSyst_down");
    hRat_TotalSyst_down->Divide(hMC_TotalSyst_down_noPlot);

}

inline
void Comparison::default_options(TCanvas* c1)
{
  mCanvas = c1;
  m2D = false;
  mFilename = "plots.pdf";

  mLog = true;
  mLogX = false;
  mLogRat = false;
  mFlow = true;
  mUnderFlow = true;
  mBothData = false;
  mCustomXRange = false;
  mCustomYRange = false;
  mCustomRatRange = false;
  mCustomStackOrder = false;
  mCustomLowerLimit = false;
  mXBinRange.reserve(2);
  mYLimRange.reserve(2);
  mScale = 1;
  mScaled = false;
  mColor1 = kAzure+1;
  mColor2 = kBlack;

  mXLabel = "";
  mYLabel = "Events";
  mYLabelFontSize = 0.06;
  mRatLabel = "#frac{2017 Data}{2016 Data}";
  mRatUseData = true;

  mDataDrawOpt = "E";
  mSignalDrawOpt = "HIST";
  mVerbose = false;
  mDoSystBand = false;

  mSystColor = kRed;
  mStatColor = kGray+2;

  mRatio = true;
  mPaperInfo = false;

  mPaperStyle = false;

  mCustomPad = false;
  mSuppressXLabel = false;
  mSuppressTop = false;

  mDrawMainLegend = true;
  mSkipSignal = false;
  mSkipCP = false;

  mIndentCms = false;
}

inline
void Comparison::plot(int idx, bool compare)
{
  if (compare) {
    if (!m2D) plot1D(idx);
    else plot2D(idx);
  }
  else
    plotNoCompare(idx);
}

inline
void Comparison::plot1D(int idx)
{
  set_main_pad(mMainPad, mLog, mRatio); 
  compute_limits(mCustomXRange, mCustomYRange);
  compute_flow(mXBinRange);
  if (mDoSystBand)
      calculate_systematics();
  draw_main_histograms();
  set_histogram_options(mColor1, mColor2);
  annotate_plot();
  if (mRatio) {
    set_rat_pad(mRatPad);
    make_rat_histogram(mVHData[0], mHMC);
  }
  if (!mCustomPad)
     print(idx);
}

inline
void Comparison::plotNoCompare(int idx)
{
  set_main_pad(mMainPad, mLog, false);
  //compute_limits(mCustomXRange, mCustomYRange);
  //compute_flow(mXBinRange);
  draw_histograms();
  //set_histogram_options(mColor1, mColor2);
  annotate_plot();
  if (!mCustomPad)
      print(idx);

}


/*
inline
void Comparison::plot1DMultipleData(int idx)
{
  set_main_pad(mMainPad, mLog);
  compute_limits(mCustomXRange, mCustomYRange);
  compute_flow(mXBinRange);
  draw_main_histograms();
  set_histogram_options(mColor1, mColor2);
  annotate_plot();
  set_rat_pad(mRatPad);
  make_rat_histogram(mHData, mHMC);
  print(idx);
}
*/

inline
void Comparison::plot2D(int idx)
{
  set_2d_pad(mMainPad, mLog);
  compute_limits(mCustomXRange, mCustomYRange, mCustomX2Range);
  draw_2D_histograms(idx);
  //annotate_2D_plot();
  //print_2D(idx);

}

inline
void Comparison::set_main_pad(TPad* mainPad, bool log, bool rat)
{
  mCanvas->cd();
  if (mCustomPad) {
    cout << "Setting custom pad limits" << endl;
    mainPad = new TPad("p_main", "p_main", mPadX1, mPadY1, mPadX2, mPadY2);
    if (mSuppressTop)
        mainPad->SetBottomMargin(0.1);
    else
        mainPad->SetBottomMargin(0.02);
  }
  else if (rat) {
    mainPad = new TPad("p_main", "p_main", 0.0, 0.3, 1.0, 1.0);
    mainPad->SetBottomMargin(0.02);
  }
  else {
    mainPad = new TPad("p_main", "p_main", 0.0, 0.05, 1.0, 1.0);
    mainPad->SetBottomMargin(0.1);
  }
  mainPad->SetRightMargin(0.07);
  if (mSuppressTop) 
      mainPad->SetTopMargin(0.02);
  else
      mainPad->SetTopMargin(0.08);
  mainPad->SetLeftMargin(0.12);
  mainPad->Draw();
  mainPad->cd();
  if (log) mainPad->SetLogy();
  if (mLogX) mainPad->SetLogx();
}

inline
void Comparison::set_rat_pad(TPad* ratPad)
{
  mCanvas->cd();
  ratPad = new TPad( "p_main", "p_main", 0.0, 0.0, 1.0, 0.3);
  ratPad->SetBottomMargin(0.36);
  ratPad->SetRightMargin(0.07);
  ratPad->SetTopMargin(0.07);
  ratPad->SetLeftMargin(0.12);
  ratPad->Draw();
  ratPad->cd();
  ratPad->SetGridy();
  if (mLogX) ratPad->SetLogx();
  if (mLogRat) ratPad->SetLogy();
}

inline
void Comparison::set_2d_pad(TPad* pad, bool log)
{
  mCanvas->cd();
  pad = new TPad("p_main", "p_main", 0.0, 0.0, 1.0, 1.0);
  pad->SetBottomMargin(0.1);
  pad->SetRightMargin(0.15);
  pad->SetTopMargin(0.08);
  pad->SetLeftMargin(0.12);
  pad->Draw();
  pad->cd();
  if (log) mCanvas->SetLogz();
  if (mLogX) mCanvas->SetLogx();
  if (mLogY) mCanvas->SetLogy();
}

inline
void Comparison::compute_limits(bool customXRange, bool customYRange)
{
  if (!customXRange) { // calculate x limits
    int nBins = mVHData[0]->GetSize() - 2;
    mXBinRange[0] = 1;
    mXBinRange[1] = nBins;
    double area = mVHData[0]->Integral(0, nBins+1);
    for (int i=1; i<=nBins; i++) {
      double binContent = mVHData[0]->GetBinContent(i);
      if (binContent < (area/(pow(10,6))) )
        continue;
      else {
        mXBinRange[0] = i;
	break;
      } 
    } 
    for (int i=nBins; i>0; i--) {
      double binContent = mVHData[0]->GetBinContent(i);
      if (binContent < (area/(pow(10,6))) )
        continue;
      else {
        mXBinRange[1] = i;
        break;
      }
    }
  } 
  
  if (!customYRange) { // calculate y limits 
    double minContent(1.5*pow(10,308)), maxContent(0), avgContent(0);
    if (mVHData.size() > 0) {
      for (int i=mXBinRange[0]; i<=mXBinRange[1]; i++) {
	double binContent = mVHData[0]->GetBinContent(i);
	avgContent += binContent/float(mXBinRange[1] - mXBinRange[0]);
	if ((binContent < minContent) && binContent > mVHData[0]->Integral(mXBinRange[0], mXBinRange[1])/(pow(10,6)))  minContent = binContent;
	if (binContent > maxContent) maxContent = binContent;
        if (mScale != -1) {
	  binContent = (mHMC->GetBinContent(i)) * mScale;
	  if ((binContent < minContent) && binContent > mVHData[0]->Integral(mXBinRange[0], mXBinRange[1])/(pow(10,6)))  minContent = binContent;
	  if (binContent > maxContent) maxContent = binContent;
        }
      }
    }
    //if (minContent < avgContent/1000) minContent = avgContent/1000;
    if (mVHSignal.size() > 0) {
      for (int i=mXBinRange[0]; i<=mXBinRange[1]; i++) {
	double binContent = mVHSignal[0]->GetBinContent(i);
	avgContent += binContent/float(mXBinRange[1] - mXBinRange[0]);
	if ((binContent < minContent) && binContent > mVHSignal[0]->Integral(mXBinRange[0], mXBinRange[1])/(pow(10,6)))  minContent = binContent;
	if (binContent > maxContent) maxContent = binContent;
	binContent = (mHMC->GetBinContent(i))*mScale;
	if ((binContent < minContent) && binContent > mVHSignal[0]->Integral(mXBinRange[0], mXBinRange[1])/(pow(10,6)))  minContent = binContent;
	if (binContent > maxContent) maxContent = binContent;
      }
    }
    double range;
    double tSpace = topSpace + (0.05*mVInfo.size());
    if (mLog) {
      range = log10(maxContent) - log10(minContent);
      mYLimRange[1] = pow(10, log10(maxContent) + tSpace*range);
      mYLimRange[0] = pow(10, log10(minContent) - botSpace*range);
    }
    else {
      //cout << maxContent << endl;
      //cout << minContent << endl;
      range = maxContent - minContent;
      mYLimRange[1] = maxContent + tSpace*range;
      mYLimRange[0] = minContent - botSpace*range;
      //mYLimRange[1] = maxContent*1.25;
      //mYLimRange[0] = minContent*0.95;
    }
    if (mYLimRange[0] < 0) mYLimRange[0] = 0;
    if (mCustomLowerLimit) mYLimRange[0] = mLowerLimit;
  }

}

inline
void Comparison::compute_limits(bool customXRange, bool customYRange, bool customZRange) {  

} 

inline
void Comparison::compute_flow(vector<int> xBinRange)
{
  int nBins = mVHData[0]->GetSize() - 2;
  if (mFlow) {
    for (int i=0; i<mVHMC.size(); i++) {
      double overflowMC = mVHMC[i]->Integral(mXBinRange[1]+1, nBins+1);
      mVHMC[i]->AddBinContent(mXBinRange[1], overflowMC);
      if (!mUnderFlow) continue;
      double underflowMC = mVHMC[i]->Integral(0, mXBinRange[0]-1);
      mVHMC[i]->AddBinContent(mXBinRange[0], underflowMC);
    }
    for (int i=0; i<mVHSignal.size(); i++) {
      double overflowSignal = mVHSignal[i]->Integral(mXBinRange[1]+1, nBins+1);
      mVHSignal[i]->AddBinContent(mXBinRange[1], overflowSignal);
      if (!mUnderFlow) continue;
      double underflowSignal = mVHSignal[i]->Integral(0, mXBinRange[0]-1);
      mVHSignal[i]->AddBinContent(mXBinRange[0], underflowSignal);
    }

    if (mVHMC.size() > 0) {
      double overflowMC = mHMC->Integral(mXBinRange[1]+1, nBins+1);
      mHMC->AddBinContent(mXBinRange[1], overflowMC);
      double underflowMC = mHMC->Integral(0, mXBinRange[0]-1);
      if (mUnderFlow) mHMC->AddBinContent(mXBinRange[0], underflowMC);
      for (int i=0; i<mVHData.size(); i++) {
    	double overflowData = mVHData[i]->Integral(mXBinRange[1]+1, nBins+1);
	    mVHData[i]->AddBinContent(mXBinRange[1], overflowData);
	    if (!mUnderFlow) continue;
	    double underflowData = mVHData[i]->Integral(0, mXBinRange[0]-1);
	    mVHData[i]->AddBinContent(mXBinRange[0], underflowData);
      }
    }

    if (mVHMCSyst_up.size() > 0) {
      for (int i = 0; i < mVHMCSyst_up.size(); i++) {
        double overflow_up = mVHMCSyst_up[i]->Integral(mXBinRange[1]+1, nBins+1);
        double overflow_down = mVHMCSyst_down[i]->Integral(mXBinRange[1]+1, nBins+1);
        double underflow_up = mVHMCSyst_up[i]->Integral(0, mXBinRange[0]-1);
        double underflow_down = mVHMCSyst_down[i]->Integral(0, mXBinRange[0]-1);
        
        mVHMCSyst_up[i]->AddBinContent(mXBinRange[1], overflow_up);
        mVHMCSyst_down[i]->AddBinContent(mXBinRange[1], overflow_down);

        if (!mUnderFlow)    continue;

        mVHMCSyst_up[i]->AddBinContent(mXBinRange[0], underflow_up);
        mVHMCSyst_down[i]->AddBinContent(mXBinRange[0], underflow_down);
      }
    }

  }
  if (mVHMC.size() > 0)
    mHMC->GetXaxis()->SetRange(mXBinRange[0],mXBinRange[1]);
  for (int i=0; i<mVHMC.size(); i++)
    mVHMC[i]->GetXaxis()->SetRange(mXBinRange[0],mXBinRange[1]);
  for (int i=0; i<mVHSignal.size(); i++)
    mVHSignal[i]->GetXaxis()->SetRange(mXBinRange[0],mXBinRange[1]);
  for (int i=0; i<mVHData.size(); i++) 
    mVHData[i]->GetXaxis()->SetRange(mXBinRange[0],mXBinRange[1]);
}

inline
void Comparison::set_histogram_options(int color1, int color2)
{
  if (!mPaperStyle)
    mYLabelFontSize = 0.05;

  for (int i=0; i<mVHData.size(); i++) {
    mVHData[i]->SetMarkerStyle(20);
    mVHData[i]->SetMarkerColor(dataColors[i]);
    mVHData[i]->SetLineColor(dataColors[i]); 
    mVHData[i]->SetLineWidth(2);
    mVHData[i]->SetMarkerSize(1.25);
    mVHData[i]->GetYaxis()->SetTitle(mYLabel);
    mVHData[i]->GetYaxis()->SetTitleSize(mYLabelFontSize);
    mVHData[i]->GetYaxis()->SetTitleOffset(1.015 * mVHData[i]->GetYaxis()->GetTitleOffset());
    if (mPaperStyle)
      mVHData[i]->GetYaxis()->SetLabelSize(0.05);
    if ((!mVHMC.empty() && mRatio) || mSuppressXLabel) {
      mVHData[i]->GetXaxis()->SetLabelOffset(999);
      mVHData[i]->GetXaxis()->SetLabelSize(0);
    }
    else if (!mRatio) {
      mVHData[i]->GetXaxis()->SetLabelSize(0.05);
      mVHData[i]->GetXaxis()->SetTitle(mXLabel);
      mVHData[i]->GetXaxis()->SetTitleOffset(0.88);
      mVHData[i]->GetXaxis()->SetTitleSize(0.053);
      //mVHData[i]->GetXaxis()->SetLabelOffset();
    }
  }

  if (mBothData) {
    mHMC->SetMarkerStyle(20);
    mHMC->SetMarkerColor(dataColors[1]);
    mHMC->SetLineColor(dataColors[1]);
    mHMC->SetLineWidth(2);
    mHMC->SetMarkerSize(1.25);
    mHMC->GetYaxis()->SetTitle(mYLabel);
    mHMC->GetYaxis()->SetTitleSize(mYLabelFontSize);
    mHMC->GetYaxis()->SetTitleOffset(1.2);
  }

  vector<int> vDefaultColors = {kRed - 7, kAzure+1, kViolet -4, kCyan-7, kOrange+6, kGreen, kYellow, kBlue+2, kPink+9};
  if (mColorData.size() > 0)
    vDefaultColors = mColorData;
  //vector<int> vDefaultColors = {kRed - 7, kAzure+1, kViolet -4, kCyan-7, kOrange+6, kOrange+7, kOrange+4, kGreen-9, kGreen, kGreen+3,kYellow, kBlue+2, kPink+1, kPink+9, kPink+4};
  for (int i=0; i<mVHMC.size(); i++) {
    mVHMC[i]->SetFillColor(vDefaultColors[i]);
    mVHMC[i]->SetLineColor(vDefaultColors[i]);
    mVHMC[i]->SetMarkerColor(vDefaultColors[i]);
    if (mBothData) mVHMC[i]->SetMarkerStyle(20);
    mVHMC[i]->GetYaxis()->SetTitle(mYLabel);
    mVHMC[i]->GetYaxis()->SetTitleSize(mYLabelFontSize);
    mVHMC[i]->GetYaxis()->SetTitleOffset(1.2);
    if (!mLog) mVHMC[i]->GetYaxis()->SetTitleOffset(1.6);
    if (mRatio) {
      mVHMC[i]->GetXaxis()->SetLabelOffset(999);
      mVHMC[i]->GetXaxis()->SetLabelSize(0);
    }
    else {
      mVHMC[i]->GetXaxis()->SetTitle(mXLabel);
      mVHMC[i]->GetXaxis()->SetTitleOffset();
      mVHMC[i]->GetXaxis()->SetTitleSize(0.13);
      mVHMC[i]->GetXaxis()->SetLabelOffset();
    }
  }
  if (mVHMC.size() > 0 && mRatio) {
    mStack->GetXaxis()->SetLabelOffset(999);
    mStack->GetXaxis()->SetLabelSize(0);
  }
  else if (!mRatio) {
    mStack->GetXaxis()->SetTitle(mXLabel);
    mStack->GetXaxis()->SetTitleOffset();
    mStack->GetXaxis()->SetTitleSize(0.13);
    mStack->GetXaxis()->SetLabelOffset();
  }


  vector<int> vColorsSignal = {kBlack, kRed, kOrange, kMagenta, kMagenta+10}; 
  for (int i=0; i<mVHSignal.size(); i++) {
    //mVHSignal[i]->SetFillColor(kBlack);
    mVHSignal[i]->SetLineColor(vColorsSignal[i]);
    mVHSignal[i]->SetMarkerColor(vColorsSignal[i]);
    if (mBothData) mVHSignal[i]->SetMarkerStyle(20);
    mVHSignal[i]->GetYaxis()->SetTitle(mYLabel);
    mVHSignal[i]->GetYaxis()->SetTitleSize(mYLabelFontSize);
    mVHSignal[i]->GetYaxis()->SetTitleOffset(1.2);
    if (!mLog) mVHSignal[i]->GetYaxis()->SetTitleOffset(1.6);
    mVHSignal[i]->SetLineWidth(3);
    if (mRatio) {
      mVHSignal[i]->GetXaxis()->SetLabelOffset(999);
      mVHSignal[i]->GetXaxis()->SetLabelSize(0);
    }
    else {
      mVHSignal[i]->GetXaxis()->SetTitle(mXLabel);
      mVHSignal[i]->GetXaxis()->SetTitleOffset();
      mVHSignal[i]->GetXaxis()->SetTitleSize(0.13);
      mVHSignal[i]->GetXaxis()->SetLabelOffset();
    }
  }


}

bool sortByValue(const std::pair<int,float>& pair1, const std::pair<int,float>& pair2 ) {
  return pair1.second < pair2.second;
}

inline
void Comparison::draw_histograms()
{
  for (int i=0; i<mVHData.size(); i++) {
    if (mScale == -1) {
      double scale = 1/mVHData[i]->Integral();
      mVHData[i]->Scale(scale);
      mVHData[i]->SetLineColor(mColorData[i]);
      mVHData[i]->SetMarkerColor(mColorData[i]);
      mVHData[i]->SetLineWidth(3);
    }
    if (i == 0) {
     mVHData[i]->Draw(mDataDrawOpt);
     mVHData[i]->SetMinimum(mYLimRange[0]);
     mVHData[i]->SetMaximum(mYLimRange[1]);
    }
    else
     mVHData[i]->Draw("SAME, " + mDataDrawOpt);
  }
  mVHData[0]->GetXaxis()->SetTitle(mXLabel);
  mVHData[0]->GetYaxis()->SetTitle(mYLabel);
  mVHData[0]->GetXaxis()->SetTitleOffset(1.1);
  mVHData[0]->GetXaxis()->SetTitleSize(0.13); 
}

inline
void Comparison::draw_main_histograms()
{
  mStack = new THStack("hStack", "");

  if (mScale == -1) {
    double eventsData = mVHData[0]->Integral();
    double eventsMC = mHMC->Integral();
    mScale = eventsData/eventsMC;
  }
  if (mVerbose) {
    cout << "Overall normalization: " << mVHData[0]->Integral()/mHMC->Integral() << endl;
    //cout << "m_gg 120-130 sig/bkg: " << mVHSignal[0]->Integral(25,26)/mHMC->Integral(25,26) << endl;
    //cout << mVHSignal[0]->Integral(25,26) << endl;
    //cout << mHMC->Integral(25,26) << endl; 
    //cout << mHMC->GetBinContent(25) << endl; 
    //cout << mHMC->GetBinContent(26) << endl;
    double nEventsSigMgg = mVHSignal[0]->Integral(25,26);
    double nEventsBkgMgg = 0;
    for (int i = 0; i < mVHMC.size(); i++) {
      if (mVHMC[i]->GetBinContent(25) > 0) nEventsBkgMgg += mVHMC[i]->GetBinContent(25);
      if (mVHMC[i]->GetBinContent(26) > 0) nEventsBkgMgg += mVHMC[i]->GetBinContent(26);
    }
    //cout << "m_gg 120-130 sig/bkg: " << nEventsSigMgg/nEventsBkgMgg << endl;
  }
  if (!mScaled) {
    mHMC->Scale(mScale);
    std::vector<std::pair<int, float> > hist_ordering;
    if (!mCustomStackOrder) {
      for (unsigned int i=0; i<mVHMC.size(); i++) {
	mVHMC[i]->Scale(mScale);
	hist_ordering.push_back(std::pair<int, float>(i, mVHMC[i]->Integral()));
      }
      // Sort MC histograms by area
      std::sort(hist_ordering.begin(), hist_ordering.end(), sortByValue);
      for (std::vector<std::pair<int, float> >::iterator it = hist_ordering.begin(); it != hist_ordering.end(); ++it)
	mStack->Add(mVHMC[it->first]);
    }
    else {
      for (unsigned int i=0; i<mVHMC.size(); i++) {
        mVHMC[i]->Scale(mScale);
        mStack->Add(mVHMC[mVOrder[i]]);
      }
    }
    mScaled = true;
  }

  for (int i=0; i<mVHData.size(); i++) { 
    if (i == 0) {
      mVHData[i]->Draw(mDataDrawOpt);
      mVHData[i]->SetMinimum(mYLimRange[0]);
      mVHData[i]->SetMaximum(mYLimRange[1]);
    }
    else
     mVHData[i]->Draw("SAME, " + mDataDrawOpt);
  }

  if (!mBothData) mStack->Draw("SAME, HIST");
  else mHMC->Draw("SAME, E");

  if (mDoSystBand) {
    hMC_TotalSyst_up->SetFillStyle(3144);
    hMC_TotalSyst_up->SetFillColorAlpha(mSystColor, 0.5);
    hMC_TotalSyst_up->SetMarkerColor(mSystColor);
    hMC_TotalSyst_up->SetMarkerSize(0.);
    hMC_TotalSyst_up->SetLineColor(mSystColor);
    hMC_TotalSyst_up->Draw("E2, SAME");
    hMC_TotalSyst_down->SetFillStyle(3144);
    hMC_TotalSyst_down->SetFillColorAlpha(mSystColor, 0.5);
    hMC_TotalSyst_down->SetMarkerColor(mSystColor);
    hMC_TotalSyst_down->SetMarkerSize(0.);
    hMC_TotalSyst_down->SetLineColor(mSystColor);
    hMC_TotalSyst_down->Draw("E2, SAME");

    hMC_StatUnc_up->SetFillStyle(3144);
    hMC_StatUnc_up->SetFillColorAlpha(mStatColor, 0.5);
    hMC_StatUnc_up->SetMarkerColor(mStatColor);
    hMC_StatUnc_up->SetMarkerSize(0.);
    hMC_StatUnc_up->SetLineColor(mStatColor);
    hMC_StatUnc_up->Draw("E2, SAME");
    hMC_StatUnc_down->SetFillStyle(3144);
    hMC_StatUnc_down->SetFillColorAlpha(mStatColor, 0.5);
    hMC_StatUnc_down->SetMarkerColor(mStatColor);
    hMC_StatUnc_down->SetMarkerSize(0.);
    hMC_StatUnc_down->SetLineColor(mStatColor);
    hMC_StatUnc_down->Draw("E2, SAME");
  }

  if (mVShade.size() > 0) {
    TH1D* hShade = new TH1D("hShade", "", 1000, 0, 10);//(TH1D*)mHMC->Clone("shade");
    hShade->SetFillColorAlpha(kGray+1, 0.25);
    hShade->SetMarkerSize(0.);
    hShade->SetMarkerColor(kGray+1);
    hShade->SetLineColor(kGray+1);
    hShade->SetLineWidth(0.);
    //hShade->SetFillStyle(3004);
    for (unsigned int i = 0; i < hShade->GetNbinsX(); i++) {
        if (hShade->GetXaxis()->GetBinCenter(i) > mVShade[0] && hShade->GetXaxis()->GetBinCenter(i) < mVShade[1]) {
            hShade->SetBinContent(i, mYLimRange[0]);
            hShade->SetBinError(i, 99999999999.);
        }
        else {
            hShade->SetBinContent(i, 0.00000001);
            hShade->SetBinError(i, 0.);
        }
    }
    hShade->Draw("E2,SAME");
  }

  //mStack->GetXaxis()->SetRange(mXBinRange[0],mXBinRange[1]);
  //mStack->SetMinimum(mYLimRange[0]);
  //mStack->SetMinimum(mYLimRange[0]);
  //mHMC->SetMaximum(mYLimRange[1]);
  //mHMC->SetMaximum(mYLimRange[1]); 
  for (int i=0; i<mVHData.size(); i++)
    mVHData[i]->Draw("SAME, " + mDataDrawOpt);
  for (int i=0; i<mVHSignal.size(); i++)
    mVHSignal[i]->Draw("SAME, " + mSignalDrawOpt);

  gPad->RedrawAxis();
}

inline
void Comparison::draw_2D_histograms(int idx)
{
  if (mScale == -1) {
    double eventsData = mH2DData->Integral();
    double eventsMC = mH2DMC->Integral();
    mScale = eventsData/eventsMC;
  }
  if (!mScaled) {
    mH2DMC->Scale(mScale);
    mScaled = true;
  }

  mCanvas->Clear();
  mH2DRat = (TH2D*)mH2DData->Clone("mH2DRat");
  mH2DData->Draw(m2DDrawOpt);
  if (mCustomYRange) {
    mH2DData->SetMinimum(mYLimRange[0]);
    mH2DData->SetMaximum(mYLimRange[1]);
  }

  mH2DData->GetXaxis()->SetTitle(mXLabel);
  mH2DData->GetYaxis()->SetTitle(mYLabel);
  mH2DData->GetYaxis()->SetTitleOffset(1.3);


  TLatex* cms = new TLatex(0.11, 0.91, "CMS Preliminary");
  cms->SetTextSize(fs*0.75);
  cms->SetNDC(kTRUE);
  cms->Draw("SAME");

  TLatex* lumi;
  TString lumiText = Form("%.2f",mLumi);
  lumiText += " fb^{-1} (13 TeV)";
  lumi = new TLatex(0.70, 0.91, lumiText.Data());
  lumi->SetTextSize(fs*0.75);
  lumi->SetNDC(kTRUE);
  lumi->Draw("SAME");

  vector<TLatex*> vLabs;
  bool drawLabs = false;
  if (mVLegendLabels.size() >= 2) {
    drawLabs = true;
    vLabs.push_back(new TLatex(0.40, 0.91, mVLegendLabels[0]));
    vLabs.push_back(new TLatex(0.40, 0.91, mVLegendLabels[1]));
    vLabs.push_back(new TLatex(0.40, 0.91, mRatLabel));
  }
  for (int i=0; i<vLabs.size(); i++) {
    vLabs[i]->SetTextSize(fs*0.75);
    vLabs[i]->SetNDC(kTRUE);
  }

  vector<TLatex*> t(mVInfo.size());
  for(int i=0; i<mVInfo.size(); i++) {
    double j = i;
    j *= 0.05;
    t[i] = new TLatex(0.20, 0.85-j, mVInfo[i]);
    t[i]->SetTextSize(fs*0.75);
    t[i]->SetNDC(kTRUE);
    t[i]->Draw("SAME");
  }
  if (drawLabs) vLabs[0]->Draw("SAME");

  if (idx == 0)
    mCanvas->Print((mFilename+"(").c_str());
  else
    mCanvas->Print((mFilename).c_str());  
  mCanvas->Clear();

  mH2DMC->Draw(m2DDrawOpt);
  mH2DMC->GetXaxis()->SetTitle(mXLabel);
  mH2DMC->GetYaxis()->SetTitle(mYLabel);
  mH2DMC->GetYaxis()->SetTitleOffset(1.3);
  if (mCustomYRange) {
    mH2DMC->SetMinimum(mYLimRange[0]);
    mH2DMC->SetMaximum(mYLimRange[1]);
  }

  cms->Draw("SAME");
  lumi->Draw("SAME");
  //vector<TLatex*> t(mVInfo.size());
  for(int i=0; i<mVInfo.size(); i++) {
    double j = i;
    j *= 0.05; 
    //t[i] = new TLatex(0.20, 0.85-j, mVInfo[i]);
    //t[i]->SetTextSize(fs);
    //t[i]->SetNDC(kTRUE);
    t[i]->Draw("SAME");
  }
  if (drawLabs) vLabs[1]->Draw("SAME");

  mCanvas->Print((mFilename).c_str());
  mCanvas->Clear();
  mCanvas->SetLogz(0); 
  //mH2DRat = (TH2D*)mH2DData->Clone("mH2DRat");
  mH2DRat->Divide(mH2DMC);
  mH2DRat->Draw(m2DDrawOpt);
  mH2DRat->SetMinimum(0);
  mH2DRat->SetMaximum(2);

  mH2DRat->GetXaxis()->SetTitle(mXLabel);
  mH2DRat->GetYaxis()->SetTitle(mYLabel);
  mH2DRat->GetYaxis()->SetTitleOffset(1.3);
  
  cms->Draw("SAME");
  lumi->Draw("SAME");
  //vector<TLatex*> t(mVInfo.size());
  for(int i=0; i<mVInfo.size(); i++) {
    double j = i;
    j *= 0.05;
    //t[i] = new TLatex(0.20, 0.85-j, mVInfo[i]);
    //t[i]->SetTextSize(fs);
    //t[i]->SetNDC(kTRUE);
    t[i]->Draw("SAME");
  }
  if (drawLabs) vLabs[2]->Draw("SAME");
  
  if (idx == 2)
    mCanvas->Print((mFilename+")").c_str());
  else
    mCanvas->Print((mFilename).c_str());
  mCanvas->Clear("D"); 
  mCanvas->SetLogx(0);
  mCanvas->SetLogy(0);
}

inline
void Comparison::annotate_plot()
{

  TLatex* cms;
  if (mPaperStyle) {
    cms  = new TLatex(0.12, 0.935, "CMS");
    cms->SetTextSize(0.075);
  }
  else if (mIndentCms) {
    cms = new TLatex(0.2, 0.935, "CMS #bf{#it{Preliminary}}");
    cms->SetTextSize(0.05);
  }
  else {
    //cms  = new TLatex(0.12, 0.935, "CMS");
    //cms = new TLatex(0.12, 0.935, "CMS #bf{#it{Supplementary}}");
    //cms = new TLatex(0.12, 0.935, "CMS #bf{#it{Preliminary}}");
    cms = new TLatex(0.12, 0.935, "CMS");
    cms->SetTextSize(0.05);
  }
  cms->SetNDC();
  if (!mSuppressTop)
      cms->Draw("SAME");

  TLatex* lumi;
  if (mLumi != -1) {
    TString lumiText = Form("#bf{%.0f fb^{-1} (13#scale[0.75]{ }TeV)}", mLumi);
    double xlumi = mPaperStyle ? 0.61 : 0.63;
    lumi = new TLatex(xlumi, 0.935, lumiText.Data());
    if (mPaperStyle)
        lumi->SetTextSize(0.0725);
    else
        lumi->SetTextSize(0.05);
    lumi->SetNDC();
    if (!mSuppressTop)
        lumi->Draw("SAME");
  }  
  else {
    lumi = new TLatex(0.73, 0.93, "(13 TeV)");
    lumi->SetTextSize(0.055);
    lumi->SetNDC(kTRUE);
    lumi->Draw("SAME");
  }

  vector<TLatex*> t(mVInfo.size());
  for(int i=0; i<mVInfo.size(); i++) {
    double j = i;
    j *= 0.06;
    t[i] = new TLatex(0.15, 0.85-j, mVInfo[i]);
    t[i]->SetTextSize(fs);
    t[i]->SetNDC(kTRUE);
    t[i]->Draw("SAME");
  }

  //TLatex* xLabel = new TLatex(0.7, 0.01, mXLabel);
  //xLabel->SetTextSize(fs);
  //xLabel->SetNDC();
  //if (!mRatio)
  //  xLabel->Draw("SAME");

  TLegend* l1;
  if (mVLegendLabels.size() > 0) {
    double j = mVHData.size()*0.05;
    if (mVHMC.empty())
      l1 = new TLegend(0.6, 0.75, 0.92, 0.89);
    else if (mVLegendLabels.size() > 5) {
      l1 = new TLegend(0.55, 0.65-(j/2), 0.92, 0.91);
      l1->SetNColumns(2);
    }
    else 
      l1 = new TLegend(0.6, 0.75-j, 0.92, 0.89);
    for (int i=0; i<mVHData.size(); i++)
      l1->AddEntry(mVHData[i], mVLegendLabels[i], "lep");
    if (!mSkipSignal) {
      for (int i=0; i<mVHSignal.size(); i++)
        l1->AddEntry(mVHSignal[i], mVLegendLabels[i+mVHData.size()], "f");
    }
    int idxMC = mVHData.size() + mVHSignal.size();
    //int idxMC = mSkipSignal ? mVHData.size() : mVHData.size() + mVHSignal.size();
    for (int i=0; i<mVHMC.size(); i++) {
      if (!mBothData) l1->AddEntry(mVHMC[i], mVLegendLabels[idxMC+i], "f");
      else l1->AddEntry(mVHMC[i], mVLegendLabels[idxMC], "lep");
    }
    if (mVHMC.size() == 0 && mBothData) {
      l1->AddEntry(mHMC, mVLegendLabels[1], "lep");
    }
    if (mVLegendLabels.size() > 7)
      l1->SetNColumns(2);
    l1->SetBorderSize(0);
    //if (mDrawMainLegend)
    //  l1->Draw("SAME");
  }

  vector<TLine*> vLH(mVHLines.size());
  for (int i=0; i<mVHLines.size(); i++) {
    //cout << mXBinRange[0] << " " << mXBinRange[1] << endl;
    //cout << mHMC->GetBinLowEdge(mXBinRange[0]) << " " << mHMC->GetBinLowEdge(mXBinRange[1]+1) << endl;
    vLH[i] = new TLine(mHMC->GetBinLowEdge(mXBinRange[0]), mVHLines[i], mHMC->GetBinLowEdge(mXBinRange[1]+1), mVHLines[i]);
    vLH[i]->SetLineWidth(2);
    vLH[i]->SetLineStyle(2);
    vLH[i]->Draw("SAME");
  }
  vector<TLine*> vLV(mVVLines.size());
  vector<TLatex*> vPaperInfo;
  vector<double> vPaperCoords_had = { 0.59, 0.6925, 0.805, 0.865 }; 
  vector<double> vPaperCoords_lep = { 0.525, 0.655, 0.735, 0.8475 }; 
  vector<double> vPaperCoords = mChannel == "Had" ? vPaperCoords_had : vPaperCoords_lep;
  double yCoord_had = 0.49;
  double yCoord_lep = mPaperStyle ? 0.76 : 0.50;
  double yCoord = mChannel == "Had" ? yCoord_had : yCoord_lep;
  for (int i=0; i<mVVLines.size(); i++) {
    vLV[i] = new TLine(mVVLines[i], mYLimRange[0], mVVLines[i], exp(log(mYLimRange[1]))); //exp(log(mYLimRange[1])/2.0));
    vLV[i]->SetLineWidth(2);
    vLV[i]->SetLineStyle(2);
    vLV[i]->Draw("SAME");
    if (mPaperInfo) {
      vPaperInfo.push_back(new TLatex(vPaperCoords[i], yCoord, mChannel + " " + to_string(mVVLines.size() - i)));
      vPaperInfo[i]->SetTextSize(fs*1.0);
      vPaperInfo[i]->SetNDC(kTRUE);
      vPaperInfo[i]->SetTextAngle(90);
      vPaperInfo[i]->Draw("SAME");
    }

    if (mPaperInfo && !mSkipCP) {
      double x, y;
      int cp_bin1(1), cp_bin2(2);
      if (mChannel != "Had") {
        cp_bin1 += 2;
        cp_bin2 += 2;
      }
      TString cp1 = "#color[1]{CP " + to_string(cp_bin1) + "}";
      TString cp2 = "#color[1]{CP " + to_string(cp_bin2) + "}";
      if (mChannel == "Had") {
        x = 0.74;
        y = 0.35;
      }
      else {
        x = 0.655;
        y = mPaperStyle ? 0.57 : 0.37;
      }
      double offset = mChannel == "Had" ? 0.04 : 0.055;
      TLatex* lcp1 = new TLatex(x - offset, y, cp2);
      TLatex* lcp2 = new TLatex(x + 0.015, y, cp1);
      lcp1->SetTextSize(fs*1.0);
      lcp1->SetNDC(kTRUE);
      
      lcp1->SetTextAngle(90);
      lcp1->Draw("SAME");

      lcp2->SetTextSize(fs*1.0); 
      lcp2->SetNDC(kTRUE);
      
      lcp2->SetTextAngle(90);
      lcp2->Draw("SAME");

    }

  }

  vector<TLine*> vLV_dotted(mVVLines_dotted.size());
  for (int i = 0; i < mVVLines_dotted.size(); i++) {
    if (mSkipCP)
        continue;
    if (mChannel == "Had")
        vLV_dotted[i] = new TLine(mVVLines_dotted[i], mYLimRange[0], mVVLines_dotted[i], exp(log(mYLimRange[1])/2.15));
    else if (mChannel == "Lep" && mPaperStyle)
        vLV_dotted[i] = new TLine(mVVLines_dotted[i], mYLimRange[0], mVVLines_dotted[i], exp(log(mYLimRange[1])/1.6));
    else
        vLV_dotted[i] = new TLine(mVVLines_dotted[i], mYLimRange[0], mVVLines_dotted[i], exp(log(mYLimRange[1])/2.15));
    vLV_dotted[i]->SetLineWidth(3);
    vLV_dotted[i]->SetLineStyle(9);
    //vLV_dotted[i]->SetLineColor(4);
    vLV_dotted[i]->Draw("SAME");

    double y_h = mChannel == "Had" ? exp(log(mYLimRange[1])/2.15) : (mChannel == "Lep" && mPaperStyle ? exp(log(mYLimRange[1])/1.6) : exp(log(mYLimRange[1])/2.15));
    double factor = mChannel == "Had" ? 8./5. : 1;
    TArrow* hline = new TArrow(mVVLines_dotted[i] - (0.8*factor), y_h, mVVLines_dotted[i] + (0.8*factor*1.1), y_h, 0.02, "<|>");
    hline->SetLineWidth(3);
    hline->SetLineStyle(9);
    hline->Draw();

  }

  if (mDrawMainLegend)
      l1->Draw("SAME");
}

inline
void Comparison::make_rat_histogram(TH1D* hData, TH1D* hMC)
{
  TGaxis::SetMaxDigits(4);
  if (mRatUseData)
    mVHRat.push_back((TH1D*)mVHData[0]->Clone("mVHRat0"));
  else
    mVHRat.push_back((TH1D*)mVHSignal[0]->Clone("mVHRat0"));
  mVHRat[0]->SetTitle("");
  mVHRat[0]->Divide(hMC);
  if (mCustomRatRange)
    mVHRat[0]->GetYaxis()->SetRangeUser(mRatRange[0],mRatRange[1]);
  else
    mVHRat[0]->GetYaxis()->SetRangeUser(0.0,2.0);
  mVHRat[0]->GetYaxis()->SetLabelSize(0.08);
  mVHRat[0]->GetYaxis()->SetNdivisions(5);
  mVHRat[0]->GetYaxis()->SetNdivisions(5);

  mVHRat[0]->GetYaxis()->SetTitle(mRatLabel);
  mVHRat[0]->GetYaxis()->SetTitleSize(0.08);
  mVHRat[0]->GetYaxis()->SetTitleOffset(0.65);
  mVHRat[0]->GetYaxis()->CenterTitle();

  mVHRat[0]->GetXaxis()->SetTitle(mXLabel);
  mVHRat[0]->GetXaxis()->SetTitleOffset(1.1);
  mVHRat[0]->GetXaxis()->SetTitleSize(0.13);

  mVHRat[0]->SetMarkerStyle(20);
  //mVHRat[0]->SetMarkerSize(1);
  mVHRat[0]->SetStats(0);
  mVHRat[0]->GetXaxis()->SetRange(mXBinRange[0], mXBinRange[1]);
  mVHRat[0]->SetMarkerColor(dataColors[0]);
  mVHRat[0]->SetLineColor(dataColors[0]);
  mVHRat[0]->SetFillColor(dataColors[0]);

  mVHRat[0]->Draw("e1");
  //for (int i = 0; i < mVHRat[0]->GetNbinsX(); i++)
  //  cout << "Bin " << i + 1 << ": " << mVHRat[0]->GetBinContent(i+1) << endl; 
  mVHRat[0]->GetXaxis()->SetLabelOffset();
  mVHRat[0]->GetXaxis()->SetLabelSize(0.08);

  for (int i=1; i<mVHData.size(); i++) {
    TString idx = Form("%d", i);
    mVHRat.push_back((TH1D*)mVHData[i]->Clone("mVHRat"+idx));
    mVHRat[i]->Divide(hMC);
    mVHRat[i]->SetMarkerStyle(20);
    mVHRat[i]->SetMarkerColor(dataColors[i]);
    mVHRat[i]->SetFillColor(dataColors[i]);
    mVHRat[i]->SetLineColor(dataColors[i]);
    mVHRat[i]->SetStats(0);
    mVHRat[i]->Draw("e1, same");
  }

  if (mDoSystBand) {
    for (int i = 0; i < (mXBinRange[1] - mXBinRange[0]) + 3; i++) {
      int idx = mXBinRange[0] + (i - 1);

      double central_value_up = (mVHRat[0]->GetBinContent(idx) + hRat_TotalSyst_up->GetBinContent(idx)) / 2.;
      double unc_up = (abs(hMC_TotalSyst_up_noPlot->GetBinContent(idx) - mHMC->GetBinContent(idx)) / mHMC->GetBinContent(idx)) / 2.;

      if (mHMC->GetBinContent(idx) == 0.)
          unc_up = 0.;

      hRat_TotalSyst_up->SetBinContent(idx, 1 + (unc_up));
      hRat_TotalSyst_up->SetBinError(idx, unc_up);

      double central_value_down = (mVHRat[0]->GetBinContent(idx) + hRat_TotalSyst_down->GetBinContent(idx)) / 2.;
      double unc_down = (abs(hMC_TotalSyst_down_noPlot->GetBinContent(idx) - mHMC->GetBinContent(idx)) / mHMC->GetBinContent(idx)) / 2.;

      if (mHMC->GetBinContent(idx) == 0.)
          unc_down = 0.; 

      hRat_TotalSyst_down->SetBinContent(idx, 1 - (unc_down));
      hRat_TotalSyst_down->SetBinError(idx, unc_down);

      double stat_unc_up = (abs(hMC_StatUnc_up->GetBinContent(idx) - mHMC->GetBinContent(idx)) / mHMC->GetBinContent(idx));
      double stat_unc_down = (abs(hMC_StatUnc_down->GetBinContent(idx) - mHMC->GetBinContent(idx)) / mHMC->GetBinContent(idx));

      if (mHMC->GetBinContent(idx) == 0.) {
        stat_unc_up = 0.;
        stat_unc_down = 0.;
      }

      hRat_StatUnc_up->SetBinContent(idx, 1 + (stat_unc_up));
      hRat_StatUnc_up->SetBinError(idx, stat_unc_up);

      hRat_StatUnc_down->SetBinContent(idx, 1 - (stat_unc_down));
      hRat_StatUnc_down->SetBinError(idx, stat_unc_down);

    }


    //TLegend* legend = new TLegend(0.14, 0.37, 0.44, 0.49);
    TLegend* legend = new TLegend(0.14, 0.37, 0.64, 0.49);
    legend->AddEntry(hRat_StatUnc_up, "Stat. Unc.", "f");
    legend->AddEntry(hRat_TotalSyst_up, "Stat. + Syst. Unc.", "f");
    legend->SetNColumns(2);
    legend->SetBorderSize(0);
    legend->Draw("SAME"); 

    hRat_TotalSyst_up->SetFillStyle(3144);
    hRat_TotalSyst_up->SetFillColorAlpha(mSystColor, 0.5);
    hRat_TotalSyst_up->SetMarkerColor(mSystColor);
    hRat_TotalSyst_up->SetMarkerSize(0.);
    hRat_TotalSyst_up->SetLineColor(mSystColor);
    hRat_TotalSyst_up->Draw("E2, SAME");

    hRat_TotalSyst_down->SetFillStyle(3144);
    hRat_TotalSyst_down->SetFillColorAlpha(mSystColor, 0.5);
    hRat_TotalSyst_down->SetMarkerColor(mSystColor);
    hRat_TotalSyst_down->SetMarkerSize(0.);
    hRat_TotalSyst_down->SetLineColor(mSystColor);
    hRat_TotalSyst_down->Draw("E2, SAME");

    hRat_StatUnc_up->SetFillStyle(3144);
    hRat_StatUnc_up->SetFillColorAlpha(mStatColor, 0.5);
    hRat_StatUnc_up->SetMarkerColor(mStatColor);
    hRat_StatUnc_up->SetMarkerSize(0.);
    hRat_StatUnc_up->SetLineColor(mStatColor);
    hRat_StatUnc_up->Draw("E2, SAME");

    hRat_StatUnc_down->SetFillStyle(3144);
    hRat_StatUnc_down->SetFillColorAlpha(mStatColor, 0.5);
    hRat_StatUnc_down->SetMarkerColor(mStatColor);
    hRat_StatUnc_down->SetMarkerSize(0.);
    hRat_StatUnc_down->SetLineColor(mStatColor);
    hRat_StatUnc_down->Draw("E2, SAME");

  }
  
  for (int i=0; i<mVHData.size(); i++) 
    mVHRat[i]->Draw("e1, same");


}

inline
void Comparison::print(int idx)
{
  if (idx == 0)
    mCanvas->Print((mFilename+"(").c_str());
  else if (idx == 1)
    mCanvas->Print((mFilename).c_str());
  else if (idx == 2)
    mCanvas->Print((mFilename+")").c_str());
  mCanvas->Clear("D");
}

#endif // _PLOTHELPER_H_

