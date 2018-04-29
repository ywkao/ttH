#ifndef _TTH_PROCESS_H_
#define _TTH_PROCESS_H_

#include "TFile.h"

using namespace std;

const int nGenLeptonCats = 6;
const int nGenPhotonCats = 3;

class Process
{
  public:
    Process(TFile* f, TString name); 
    ~Process();  

    void add_histogram(TString name, int nBins, double xLow, double xHigh);
    void fill_histogram(TString name, double value, double weight, int genLeptonId = -1, int genPhotonId = -1);

  private:
    TFile* mFile;
    TString mName;

    vector<TH1D*> mH;

    bool mGenLepton;
    vector<vector<TH1D*>> mHGenLeptonComp;
    bool mGenPhoton;
    vector<vector<TH1D*>> mHGenPhotonComp;
    
    std::map<TString, int> mMap;
};

inline Process::~Process()
{
  for (int i = 0; i < mH.size(); i++)
    delete mH[i];
  for (int i = 0; i < mHGenLeptonComp.size(); i ++ ) {
    for (int j = 0; j < mHGenLeptonComp[i].size(); j++)
      delete mHGenLeptonComp[i][j];
  }
  for (int i = 0; i < mHGenPhotonComp.size(); i ++ ) {
    for (int j = 0; j < mHGenPhotonComp[i].size(); j++)
      delete mHGenPhotonComp[i][j];
  }
}

inline Process::Process(TFile* f, TString name) 
{
  mFile = f;
  f->cd();
  mName = name;
  mGenLepton = true;
  mGenPhoton = true;
}

inline void Process::add_histogram(TString name, int nBins, double xLow, double xHigh)
{
  mH.push_back(new TH1D(name + "_" + mName, "", nBins, xLow, xHigh));
  mH[mH.size() - 1]->Sumw2();

  if (mGenLepton) {
    vector<TH1D*> vTemp;
    for (int i = 0; i < nGenLeptonCats; i++) {
      vTemp.push_back(new TH1D(name + "_" + mName + "GenLepton_" + to_string(i), "", nBins, xLow, xHigh));
      vTemp[i]->Sumw2();
    }       
    mHGenLeptonComp.push_back(vTemp);
  }

  if (mGenPhoton) {
    vector<TH1D*> vTemp;
    for (int i = 0; i < nGenPhotonCats; i++) {
      vTemp.push_back(new TH1D(name + "_" + mName + "GenPhoton_" + to_string(i), "", nBins, xLow, xHigh));
      vTemp[i]->Sumw2();
    }       
    mHGenPhotonComp.push_back(vTemp);
  }

  mMap[name] = mH.size() - 1;
}

inline void Process::fill_histogram(TString name, double value, double weight, int genLeptonId, int genPhotonId)
{
  int idx = mMap.find(name)->second;
  mH[idx]->Fill(value, weight);

  if (genLeptonId >= 0)
    mHGenLeptonComp[idx][genLeptonId]->Fill(value, weight);
  if (genPhotonId >= 0)
    mHGenPhotonComp[idx][genPhotonId]->Fill(value, weight);
}

#endif // _TTH_PROCESS_H_
