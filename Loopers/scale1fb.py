import sys, os
import numpy
import glob
import dis_client
import json

path = "/hadoop/cms/store/user/bemarsh/flashgg/MicroAOD_skim/2016_skim_v2/"
dirs = glob.glob(path+"*")

datasets = []
for dir in dirs:
  if "DoubleEG" in dir: #only do this for MC
    continue
  dataset = dir.replace(path, "/") 
  datasets.append(dataset)

scale1fb = {}
for dataset in datasets:
  scale1fb[dataset] = {"xs" : 1, "br" : 1, "kf" : 1, "nevents" : 1, "scale1fb" : 0}


with open("../BabyMaker/CMSSW_8_0_28/src/flashgg/MetaData/work/campaigns/RunIISummer16-2_4_2-25ns_Moriond17.json") as json_file:
  das_datasets = json.load(json_file)
  for dataset in datasets:
    key = ""
    if "ttH" in dataset:
      key = "sig"
    else:
      key = "bkg"
    for das_dataset in das_datasets[key]:
      if dataset in das_dataset: # found a match
	query_result = dis_client.query(q=das_dataset, typ="basic")
        nEvents = ((query_result['response'])['payload'])['nevents']
        scale1fb[dataset]["nevents"] = nEvents

with open("../BabyMaker/CMSSW_8_0_28/src/flashgg/MetaData/data/cross_sections.json") as json_file:
  cross_sections = json.load(json_file)
  for dataset in datasets:
    ds = dataset.replace("/","")
    if ds in cross_sections.keys():
      if "xs" in cross_sections[ds].keys():
        scale1fb[dataset]["xs"] = cross_sections[ds]["xs"]
      if "br" in cross_sections[ds].keys():
        scale1fb[dataset]["br"] = cross_sections[ds]["br"]
      if "kf" in cross_sections[ds].keys():
        scale1fb[dataset]["kf"] = cross_sections[ds]["kf"]

for key, info in scale1fb.iteritems():
  if scale1fb[key]["nevents"] > 1:
    scale1fb[key]["scale1fb"] = (scale1fb[key]["xs"] * scale1fb[key]["br"] * scale1fb[key]["kf"] * 1000) / scale1fb[key]["nevents"]
  else:
    print("Did not match dataset %s in either flashgg catalog or on DAS" % key)
    print("Setting scale1fb to 0")
    scale1fb[key]["scale1fb"] = 0

for key, info in scale1fb.iteritems():
  print key
  print info

with open("scale1fb.h", "w") as fout:
  fout.write("double scale1fb(TString currentFileTitle) {\n")
  fout.write("  std::map<TString, double> m = {\n")
  for key, info in scale1fb.iteritems():
    fout.write('  	{"%s", %.10f},\n' % (key.replace("/",""), info["scale1fb"]))
  fout.write("  };")
  fout.write("\n")
  fout.write('  TObjArray *tx = currentFileTitle.Tokenize("/");\n')
  fout.write("  TString key = ((TObjString *)(tx->At(tx->GetEntries()-2)))->String();\n")
  fout.write('  TString to_replace = "__ttH_Babies_v1";\n')
  fout.write('  TString replace_with = "";\n')
  fout.write('  key = key.ReplaceAll(to_replace, replace_with);\n')
  fout.write("  return m.find(key)->second;\n")
  fout.write("}")
  
