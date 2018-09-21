#include <iostream>
#include "TChain.h"

using namespace std;

int main() {
  TChain* ch = new TChain("Events");
  //ch->Add("/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016B-03Feb2017_ver2-v2/microAOD_1.root");

  TString version = "v2";
  //TString version = "v3_jetPt20";
  ch->Add("/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_" + version + "/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016B-03Feb2017_ver2-v2/*.root");
  ch->Add("/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_" + version + "/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016C-03Feb2017-v1/*.root");
  ch->Add("/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_" + version + "/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016D-03Feb2017-v1/*.root");
  ch->Add("/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_" + version + "/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016E-03Feb2017-v1/*.root");
  ch->Add("/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_" + version + "/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016F-03Feb2017-v1/*.root");
  ch->Add("/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_" + version + "/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016G-03Feb2017-v1/*.root");
  ch->Add("/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_" + version + "/DoubleEG/ReMiniAOD-03Feb2017-2_5_4-2_5_1-v0-Run2016H-03Feb2017_ver2-v1/*.root");

  if (version == "v2")
    cout << "Now running on old skims" << endl;
  cout << "This one should be there" << endl;
  cout << ch->GetEntries("EventAuxiliary.event() == 487401722") << endl;
  //cout << ch->GetEntries("EventAuxiliary.event() == 487401722 && Event.Auxiliary.run() == 279823") << endl;

  cout << "These ones might not" << endl;

  cout << ch->GetEntries("EventAuxiliary.event() == 173472515") << endl;
  cout << ch->GetEntries("EventAuxiliary.event() == 90705629") << endl;
  cout << ch->GetEntries("EventAuxiliary.event() == 1022917588") << endl;
}
