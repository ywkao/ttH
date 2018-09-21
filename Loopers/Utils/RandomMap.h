#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include "TString.h"

class RandomMap {
  public:
    RandomMap(TString textFile);
    double retrieve_rand(unsigned long long event, unsigned int run, unsigned int lumi);

  private:
    std::map<TString, double> m;

};

inline
RandomMap::RandomMap(TString textFile)
{
  ifstream mapfile;
  mapfile.open(textFile);

  TString key;
  double rand;
  while (mapfile >> key >> rand) {
    m[key] = rand;
  }
}

inline
double RandomMap::retrieve_rand(unsigned long long event, unsigned int run, unsigned int lumi)
{
  TString key = to_string(event) + "_" + to_string(run) + "_" + to_string(lumi);
  return m.find(key)->second;
}
/*
std::map<TString, double> m = {

};

double retrieve_rand(TString evt_run_lumi) {
  return m.find(evt_run_lumi)->second;
}
*/

//class RandomMap
//  public:
//    RandomMap(TRandom* rand);
//    void 
