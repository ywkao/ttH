import sys, os
sys.path.append("../")

import parallel_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to denote with", type=str)
args = parser.parse_args()

bdt = "none"

os.chdir("../")
#parallel_utils.run('./ttHHadronicLooper "ttHHadronic_RunII_DiPhotonFits_Presel" "RunII" "%s" "%s" "none"' % (args.tag + "prefit", bdt))
os.chdir("PhotonID_Sideband/")
#parallel_utils.run('python derive_shape_fake.py --input "../ttHHadronic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" --hist_name "hFakePhotonIDMVA"' % (args.tag + "prefit"))

os.chdir("../")
parallel_utils.run('./ttHHadronicLooper "ttHHadronic_RunII_MVA_Presel" "RunII" "%s" "%s" "impute_no_scale"' % (args.tag + "_impute_prefit", bdt))

os.chdir("Plots")
parallel_utils.run('./makeTables "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" > ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.txt' % (args.tag + "_impute_prefit", args.tag + "_impute_prefit"))

plot_label = "ttH Hadronic"
for plot_type in ["std", "std_linear"]:
  parallel_utils.run('./makePlots "%s" "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" "%s"' % (plot_type, args.tag + "_impute_prefit", plot_label + "|Loose Preselection"))

os.chdir("../tt_template_fit/")
parallel_utils.run('python do_fits_impute.py --input_file "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" --jet_bin "2+"' % (args.tag + "_impute_prefit"))

input("You probably want to update ttHLooper.h with results of DiPhoton Fits. Press any key to continue")

os.chdir("../")
parallel_utils.run('./ttHHadronicLooper "ttHHadronic_RunII_MVA_Presel" "RunII" "%s" "%s" "impute"' % (args.tag + "_impute", bdt))

plot_label = "ttH Hadronic"
parallel_utils.run('./makeTables "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" > ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.txt' % (args.tag + "_impute", args.tag + "_impute"))

for plot_type in ["std", "std_linear"]:
  parallel_utils.run('./makePlots "%s" "../ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" "%s"' % (plot_type, args.tag + "_impute", plot_label))

os.chdir("../")
parallel_utils.run('./ttHHadronicMVABabyMaker "ttHHadronic_RunII_MVA_Presel" "RunII" "%s_RunII_MVA_Presel_impute" "impute"' % (args.tag))

os.chdir("../MVAs/")
parallel_utils.run('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_impute" --channel "Hadronic"' % (args.tag))
parallel_utils.run('python train.py --input "ttHHadronic_%s_RunII_MVA_Presel_impute_features.hdf5" --channel "Hadronic" --tag "_%s_RunII_MVA_Presel_impute" --ext ""' % (args.tag, args.tag))

