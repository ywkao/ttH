import os
import sys
import glob
import fnmatch
import re
import json

sample_map = {}

dirs = glob.glob("/eos/cms//store/group/phys_higgs/cmshgg/sethzenz/flashgg/RunIISummer16-2_4_1-25ns_Moriond17/2_4_1/*")
for dir in dirs:
  microAOD_list = []
  for root, dirnames, filenames in os.walk(dir):
    for filename in fnmatch.filter(filenames, '*.root'):
      microAOD_list.append(os.path.join(root, filename))
  sample = dir.split("/")[-1]
  sample_map[sample] = microAOD_list 

with open("calc_neg_event_weights.cpp", "w") as fout:
  fout.write("#include <iostream>\n")
  fout.write('#include "TChain.h"\n')
  fout.write('\n')
  fout.write('using namespace std;\n')
  fout.write('\n')
  fout.write("int main() {\n")
  fout.write('  cout << " { " << endl;\n')
  with open ("cross_sections.json") as json_file:
    samples_list = json.load(json_file)
    for key, entries in sample_map.iteritems():
      if ".tmp" in key:
        continue 
      for sample, info in samples_list.iteritems():
        if sample == key:
	    fout.write('  TChain* ch_%s = new TChain("Events");\n' % key.replace("-", "_"))
	    fout.write('  ch_%s->Add("%s");\n' % (key.replace("-", "_"), re.sub(r"myMicroAODOutputFile_\d{1}", "myMicroAODOutputFile_*", entries[0])))
	    fout.write('  double pos_weights_%s = ch_%s->GetEntries("GenEventInfoProduct_generator__SIM.obj.weights_ > 0");\n' % (key.replace("-", "_"), key.replace("-", "_")))
	    fout.write('  double neg_weights_%s = ch_%s->GetEntries("GenEventInfoProduct_generator__SIM.obj.weights_ < 0");\n' % (key.replace("-", "_"), key.replace("-", "_")))
	    fout.write('  double frac_neg_weights_%s = neg_weights_%s / (pos_weights_%s + neg_weights_%s);\n' % (key.replace("-", "_"), key.replace("-", "_"), key.replace("-", "_"), key.replace("-", "_")))
	    fout.write('  cout << "{ \'%s\'" << " : " << frac_neg_weights_%s << " } , " << endl;\n' % (key, key.replace("-", "_")))
    fout.write('  cout << " } " << endl;\n')
  fout.write("}\n")
