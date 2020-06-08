import time
import itertools
import json
import glob

from metis.Sample import DBSSample
from metis.CMSSWTask import CMSSWTask
from metis.StatsParser import StatsParser

import os, sys

import ROOT as r
r.gROOT.SetBatch(True)

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--runII", help = "Use Run II legacy samples", action = "store_true")
args = parser.parse_args()


if args.runII:
  base_path = "/home/users/sjmay/ttH/FCNC_Workspaces/CMSSW_10_6_1_patch2/src/flashgg/MicroAOD/BatchSubmit/"
  #base_path = "/home/users/sjmay/ttH/BabyMaker/CMSSW_10_5_0_old/src/flashgg/MicroAOD/BatchSubmit/"
  input_jsons = [base_path + "datasets_RunIISummer16_22May2020.json", base_path + "datasets_RunIIFall17_22May2020.json", base_path + "datasets_RunIIAutumn18_22May2020.json"]
  output_json = "sum_of_weights_runII.json"
else:
  input_jsons = ["datasets_RunIISummer16.json", "datasets_RunIIFall17.json"] 
  output_json = "sum_of_weights.json"

with open(output_json, "r") as f_in:
  mc_samples = json.load(f_in)

def get_sum_of_weights(files, dbs=True):
  sum = 0
  evts = 0
  idx = 0
  for file in files:
    idx += 1
    print ("checking file %d / %d" % (idx, len(files)))
    sys.stdout.write("\033[F")
    if dbs:
        f = r.TFile.Open("root://cmsxrootd.fnal.gov//" + file.get_name())
    else:
        f = r.TFile.Open(file)
    if not f:
      print "bad file: %s" % file.get_name()
      return -(10**12)
    tree = f.Get("Events")
    tree.Draw("GenEventInfoProduct_generator__SIM.obj.weights_[0]")
    hist = r.gPad.GetPrimitive("htemp")
    mean = hist.GetMean()
    #mean = tree.GetMean("GenEventInfoProduct_generator__SIM.obj.weights_")
    total_evts = tree.GetEntries("GenEventInfoProduct_generator__SIM.obj.weights_[0]")
    print "Mean: %.6f, total entries: %d" % (mean, total_evts)
    sum += mean * total_evts
    evts += total_evts
    del hist
  return sum, evts

def get_negative_events(files):
  n_events_neg = 0
  n_events_pos = 0
  idx = 0
  for file in files:
    idx += 1
    print ("checking file %d / %d" % (idx, len(files)))
    sys.stdout.write("\033[F")
    f = r.TFile.Open("root://cmsxrootd.fnal.gov//" + file.get_name())
    if not f:
      print "bad file: %s" % file.get_name()
      return -(10**12), -(10**12)
    tree = f.Get("Events")
    evt_neg = tree.GetEntries("GenEventInfoProduct_generator__SIM.obj.weights_ < 0")
    evt_pos = tree.GetEntries("GenEventInfoProduct_generator__SIM.obj.weights_ > 0")
    n_events_neg += evt_neg
    n_events_pos += evt_pos
  return n_events_neg, n_events_pos

for input_json in input_jsons:
  with open(input_json, "r") as f_in:
    datasets = json.load(f_in)
    for key, dict in datasets["processes"].iteritems():
      if key == "data":
	    continue
      for sample in dict["datasets"]:
        if sample in mc_samples.keys():
          print "%s already found in samples list\n" % sample
          continue
        else:
          print "Adding %s to list of mc samples\n" % sample
          mc_samples[sample] = { "n_events_tot" : -1, "sum_of_weights" : 0}

for key, dict in mc_samples.iteritems():
  if not dict["sum_of_weights"] <= 0:
    continue
  #if "QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCUETP8M1" in key:
  #  continue # this sample doesn't work for some reason, but shouldn't matter since we never go below m_gg of ~100

  #if dict["n_events_neg"] + dict["n_events_pos"] == dict["n_events_tot"]:
    #print "%s has trustworthy n_events data, skipping\n\n" % key
    #continue
  #elif dict["n_events_tot"] > 0 and dict["n_events_pos"] > 0:
  #  print "%s has already had n_event info calculated, but n_neg + n_pos != n_total" % key
  #  print "Probably want to check it manually"
  #  continue
  print "Calculating n_events for %s\n\n" % key
  sample = DBSSample(dataset=key)
  files = sample.get_files()
  n_events = 0
  for file in files:
    n_events += file.get_nevents()
  dict["n_events_tot"] = n_events
  #dict["n_events_neg"], dict["n_events_pos"] = get_negative_events(files)
  dict["sum_of_weights"], dummy = get_sum_of_weights(files)
  #if dict["n_events_neg"] + dict["n_events_pos"] == dict["n_events_tot"]:
    #print "%s has: \n Total events: %d \n Positive Events: %d \n Negative Events: %d \n" % (key, dict["n_events_tot"], dict["n_events_pos"], dict["n_events_neg"])
  #else:
    #print "Some issue calculating n_events for %s: \n Total events: %d \n Positive Events: %d \n Negative Events: %d \n" % (key, dict["n_events_tot"], dict["n_events_pos"], dict["n_events_neg"])
  with open(output_json, "w") as f_out:
    json.dump(mc_samples, f_out, indent=4, sort_keys=True)

local_sets = [
        "/hadoop/cms/store/user/smay/miniaod_runII/TT_T2HJ_HAD_HUT_2018_20200522_v1_STEP4_v1/",
        "/hadoop/cms/store/user/smay/miniaod_runII/ST_HAD_HCT_2016_20200522_v1_STEP4_v1/",
        "/hadoop/cms/store/user/smay/miniaod_runII/ST_HAD_HUT_2016_20200522_v1_STEP4_v1/",
        "/hadoop/cms/store/user/hmei/miniaod_runII/TT_aT2HJ_HAD_HCT_2018_20200522_v1_STEP4_v1/",
        "/hadoop/cms/store/user/hmei/miniaod_runII/TT_T2HJ_HAD_HCT_2018_20200522_v1_STEP4_v1/",
        "/hadoop/cms/store/user/hmei/miniaod_runII/TT_aT2HJ_HAD_HUT_2018_20200522_v1_STEP4_v1/",
]

events_per_private_miniaod = 200

for ds in local_sets:
    dataset = ds.split("/")[-2]
    if dataset in mc_samples.keys():
        print "%s already found in samples list\n" % dataset
        continue
    else:
        print "Adding %s to list of mc samples\n" % dataset
        mc_samples[dataset] = { "n_events_tot" : -1, "sum_of_weights" : 0 }
        files = glob.glob(ds + "/*.root")
        #mc_samples[dataset]["n_events_tot"] = events_per_private_miniaod * len(files)
        mc_samples[dataset]["sum_of_weights"], mc_samples[dataset]["n_events_tot"] = get_sum_of_weights(files, False)

    with open(output_json, "w") as f_out:
        json.dump(mc_samples, f_out, indent=4, sort_keys=True)
