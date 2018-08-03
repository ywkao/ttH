import sys, os
import numpy
import glob
import json

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("year", type=str)
args = parser.parse_args()

if args.year == "2016":
  path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v3_jetPt20/"
elif args.year == "2017":
  path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2017_skim_v1/"
dirs = glob.glob(path+"*")


datasets = []
for dir in dirs:
  if "DoubleEG" in dir: #only do this for MC
    continue
  if args.year == "2016" and ("GluGluHToGG" in dir or "VBF" in dir or "VH" in dir or "bbH" in dir):
    continue
  dataset = dir.replace(path, "/")
  datasets.append(dataset)

scale1fb = {}
for dataset in datasets:
  scale1fb[dataset] = {"xs" : 1, "filter_eff": 1, "br" : 1, "kf" : 1, "nevents" : 1, "negative_weight_frac": 0, "scale1fb" : 0}

# Get number of events (calculated by scanning central microAOD in /eos on lxplus)
def grab_event_info(json_file_):
  with open(json_file_) as json_file:
    mc_events = json.load(json_file)
    for dataset in datasets:
      scale1fb[dataset]["negative_weight_frac"] = mc_events[dataset.replace("/","")][0]
      scale1fb[dataset]["nevents"] = mc_events[dataset.replace("/","")][1]

if args.year == "2016":
  grab_event_info("nEvents/mc_events_2016_80X.json")
elif args.year == "2017":
  grab_event_info("nEvents/mc_events_2017_94X.json")

# Take cross sections from flashgg repository instead of Twiki
with open("cross_sections_flashgg.json") as json_file:
  cross_sections = json.load(json_file)
  for dataset in datasets:
    ds = dataset.replace("/","")
    if ds in cross_sections.keys():
      scale1fb[dataset]["xs"] = cross_sections[ds]["xs"]
      if "br" in cross_sections[ds]:
        scale1fb[dataset]["br"] = cross_sections[ds]["br"]
      #if "kf" in cross_sections[ds]: #FIXME: should we do k-factor?
      #  scale1fb[dataset]["kf"] = cross_sections[ds]["kf"]

# Calculate scale1fb
for key, info in scale1fb.iteritems():
  if scale1fb[key]["nevents"] > 1:
    scale1fb[key]["scale1fb"] = (scale1fb[key]["xs"] * scale1fb[key]["filter_eff"] * scale1fb[key]["br"] * scale1fb[key]["kf"] * 1000) / (scale1fb[key]["nevents"] * ( 1 - 2*scale1fb[key]["negative_weight_frac"]))
  else:
    print("Did not match dataset %s" % key)
    print("Setting scale1fb to 0")
    scale1fb[key]["scale1fb"] = 0

# Check that things look reasonable
for key, info in scale1fb.iteritems():
  print key
  print info

with open("scale1fb_%s.h" % args.year, "w") as fout:
  fout.write("double scale1fb_%s(TString currentFileTitle) {\n" % args.year)
  fout.write("  std::map<TString, double> m = {\n")
  for key, info in scale1fb.iteritems():
    fout.write('  	{"%s", %.10f},\n' % (key.replace("/",""), info["scale1fb"]))
  fout.write("  };")
  fout.write("\n")
  fout.write('  TObjArray *tx = currentFileTitle.Tokenize("/");\n')
  fout.write("  TString key = ((TObjString *)(tx->At(tx->GetEntries()-2)))->String();\n")
  fout.write('  TString tag = "v3.11";\n')
  fout.write('  TString to_replace = "__ttH_Babies_" + tag + "_%s";\n' % args.year)
  fout.write('  TString replace_with = "";\n')
  fout.write('  key = key.ReplaceAll(to_replace, replace_with);\n')
  fout.write("  return m.find(key)->second;\n")
  fout.write("}")
  
