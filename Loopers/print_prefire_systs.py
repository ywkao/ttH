import os,sys
import ROOT

srs = ["0", "1", "2", "3"]
#channels = ["LEP"]
channels = ["HAD", "LEP"]
# ggH_hgg qqH_hgg ttH_hgg ZH_lep_hgg WH_lep_hgg WH_had_hgg ZH_had_hgg bbH_hgg tHq_hgg tHW_hgg bkg_mass
procs = ["ggH", "VBF", "ttH", "VH", "VH", "VH", "VH", "", "THQ", "THW", ""]

results = {}

for channel in channels:
    for sr in srs:
        key = "TTH" + channel + "_" + sr
        result = []
        
        file_base = "ttH" + ("Hadronic" if channel == "HAD" else "Leptonic") + "_RunII_SR_" + sr + "_v4.11_12Nov2019" + ("_impute" if channel == "HAD" else "")
        file_end = "_histogramsRunII.root"
        
        file_noPrefire = ROOT.TFile(file_base + "_undoL1Prefire" + file_end)
        file_nominalPrefire = ROOT.TFile(file_base + file_end)
        file_upPrefire = ROOT.TFile(file_base + "_L1PrefireUp" + file_end)
        file_downPrefire = ROOT.TFile(file_base + "_L1PrefireDown" + file_end)

        for proc in procs:
            if proc == "":
                result.append("-")
                continue

            hist_noPrefire = file_noPrefire.Get("hMass_v2_" + proc)
            hist_nominalPrefire = file_nominalPrefire.Get("hMass_v2_" + proc)
            hist_upPrefire = file_upPrefire.Get("hMass_v2_" + proc)
            hist_downPrefire = file_downPrefire.Get("hMass_v2_" + proc)

            if hist_noPrefire.GetEntries() < 10 or hist_noPrefire.Integral() < 0:
                result.append("-")
                continue

            yield_noPrefire = hist_noPrefire.Integral()
            yield_nominalPrefire = hist_nominalPrefire.Integral()
            yield_upPrefire = hist_upPrefire.Integral()
            yield_downPrefire = hist_downPrefire.Integral()

            print "%s, proc: %s" % (key, proc)
            print "Yield (no prefire): %.6f" % yield_noPrefire
            print "Yield (nominal prefire): %.6f" % yield_nominalPrefire
            print "Yield (up prefire): %.6f" % yield_upPrefire
            print "Yield (down prefire): %.6f" % yield_downPrefire


            A = yield_upPrefire / yield_nominalPrefire
            B = yield_downPrefire / yield_nominalPrefire
            print "Unc: %.6f" % (A/B)
            unc = A / B
            result.append(unc)

        results[key] = result

import json
with open("l1_prefire_syst_uncs.txt", "w") as f_out:
    for key in results.keys():
        f_out.write(key + ": " + str(results[key]) + '\n')
    #json.dump(results, f_out, indent=4, sort_keys=True)

