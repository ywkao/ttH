import sys, os
import numpy
import glob
import json

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--runII", help = "Use Run II legacy samples", action = "store_true")
parser.add_argument("--old_weights", action = "store_true")
args = parser.parse_args()

scale1fb = {}

if args.runII:
  samples_file = "nEvents_fromDBS/sum_of_weights_runII.json"
elif args.old_weights:
  samples_file = "nEvents_fromDBS/n_events.json"
else:
  samples_file = "nEvents_fromDBS/sum_of_weights.json"
with open(samples_file) as f_in:
  samples = json.load(f_in)

for sample, info in samples.iteritems():
  process = str(sample.split("/")[1])
  if not process in scale1fb:
    scale1fb[process] = {}
    scale1fb[process]["xs"] = 1
    scale1fb[process]["filter_eff"] = 1
    scale1fb[process]["br"] = 1
    scale1fb[process]["scale1fb_2016"] = 0
    scale1fb[process]["scale1fb_2017"] = 0
    scale1fb[process]["scale1fb_2018"] = 0
    scale1fb[process]["productions"] = {}
    scale1fb[process]["matched_xs"] = False

# Take cross sections from flashgg repository instead of Twiki
def copy_entry(dict1, dict2, key):
  if key in dict1.keys():
    dict2[key] = dict1[key]

with open("cross_sections_flashgg.json") as json_file:
  cross_sections = json.load(json_file)
  for sample, sample_info in scale1fb.iteritems():
    for key, info in cross_sections.iteritems():
      if key == sample and sample in cross_sections.keys():
	copy_entry(info, sample_info, "xs")
        copy_entry(info, sample_info, "br")
        copy_entry(info, sample_info, "filter_eff")
	sample_info["matched_xs"] = True

# Now calculate n_events and scale1fb
for sample, info in samples.iteritems():
  sample_split = sample.split("/")
  process = str(sample_split[1])
  production = str(sample_split[2])
  scale1fb[process]["productions"][production] = info

years = {	
		"2016" : "RunIISummer16MiniAODv3",
		"2017" : "RunIIFall17MiniAODv2",
		"2018" : "RunIIAutumn18MiniAOD",
	}
for year, year_info in years.iteritems():
  for sample, sample_info in scale1fb.iteritems():
    sum_of_weights = 0
    n_events_pos = 0
    n_events_neg = 0
    for production, production_info in sample_info["productions"].iteritems():
      if year_info not in production:
        continue
      if "BSandPUSummer16" in production and "ttHJetToGG" in sample: #skip this one, not sure what it is
	    production_info["comment"] = "unused in scale1fb calc"
	    continue
      if args.old_weights:
        n_events_pos += production_info["n_events_pos"]
        n_events_neg += production_info["n_events_neg"]
      else: 
        sum_of_weights += production_info["sum_of_weights"]
      #n_events_pos += production_info["n_events_pos"]
      #n_events_neg += production_info["n_events_neg"]
    if args.old_weights:
      if n_events_pos > 0:
        sample_info["scale1fb_" + year] = float(sample_info["xs"] * sample_info["br"] * sample_info["filter_eff"] * 1000) / float(n_events_pos - n_events_neg)
    else:
      if sum_of_weights > 0:
	    sample_info["scale1fb_" + year] = float(sample_info["xs"] * sample_info["br"] * sample_info["filter_eff"] * 1000) / float(sum_of_weights)
      else:
	    sample_info["scale1fb_" + year] = 0

with open("scale1fb.json", "w") as f_out:
  json.dump(scale1fb, f_out, indent=4, sort_keys=True)

for year in ["2016", "2017", "2018"]:
  with open("scale1fb_%s%s.h" % (year, "_RunII" if args.runII else ""), "w") as fout:
    fout.write("double scale1fb_%s%s(TString currentFileTitle) {\n" % (year, "_RunII" if args.runII else ""))
    fout.write("  std::map<TString, double> m = {\n")
    for key, info in scale1fb.iteritems():
      #for process in info["productions"].keys():
      fout.write('        {"%s", %.10f},\n' % (key.replace("/",""), info["scale1fb_" + year]))
    fout.write("  };")
    fout.write("\n")
    fout.write('  TObjArray *tx = currentFileTitle.Tokenize("/");\n')
    fout.write("  TString key = ((TObjString *)(tx->At(tx->GetEntries()-2)))->String();\n")
    fout.write('  std::map<TString, double>::iterator it;\n')
    fout.write('  for (it = m.begin(); it != m.end(); it++) {\n')
    fout.write('    if (key.Contains(it->first) && it->second > 0) {\n')
    fout.write('      return it->second;\n')
    fout.write('    }\n')
    fout.write('  }\n')
    fout.write('  cout << "Did not match " << key << " to a sample in the scale1fb map!" << endl;\n')
    #fout.write('  TString tag = "v1.0";\n')
    #fout.write('  TString to_replace = "__ttH_Babies_" + tag + "_%s";\n' % year)
    #fout.write('  TString replace_with = "";\n')
    #fout.write('  key = key.ReplaceAll(to_replace, replace_with);\n')
    #fout.write("  return m.find(key)->second;\n")
    fout.write("}")

