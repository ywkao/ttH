import sys, os
sys.path.append("../")

import parallel_utils
import workflow_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to denote with", type=str)
args = parser.parse_args()

bdt = "none"

os.chdir("../")
command_list = []
command_list.append('./ttHHadronicLooper "ttHHadronic_RunII_DiPhotonFits_Presel" "RunII" "%s" "%s" "none"' % (args.tag + "prefit", bdt))
command_list.append('./ttHHadronicLooper "ttHHadronic_RunII_MVA_Presel" "RunII" "%s" "%s" "none"' % (args.tag + "prefit", bdt))
parallel_utils.submit_jobs(command_list, 2)

os.chdir("tt_template_fit")
parallel_utils.run('python do_fits_qcd.py --input_file "../ttHHadronic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" --jet_bin "2+"' % (args.tag + "prefit"))
os.chdir("../")

input("You probably want to update ttHLooper.h with results of DiPhoton Fits. Press any key to continue")

command_list = []
command_list.append('./ttHHadronicLooper "ttHHadronic_RunII_DiPhotonFits_Presel" "RunII" "%s" "%s" "scale_diphoton"' % (args.tag, bdt))
command_list.append('./ttHHadronicLooper "ttHHadronic_RunII_MVA_Presel" "RunII" "%s" "%s" "scale_diphoton"' % (args.tag, bdt))
parallel_utils.submit_jobs(command_list, 2)

# Tables and Plots
os.chdir("Plots")

workflow_utils.make_tables_and_plots("../ttHHadronic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" % (args.tag + "prefit"), "ttH Hadronic|DiPhoton Fit Presel.|Pre-Fit")
workflow_utils.make_tables_and_plots("../ttHHadronic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" % (args.tag), "ttH Hadronic|DiPhoton Fit Presel.|Post-Fit")
workflow_utils.make_tables_and_plots("../ttHHadronic_RunII_MVA_Presel_Presel_%s_histogramsRunII.root" % (args.tag + "prefit"), "ttH Hadronic|Loose MVA Fit Presel.|Pre-Fit")
workflow_utils.make_tables_and_plots("../ttHHadronic_RunII_MVA_Presel_Presel_%s_histogramsRunII.root" % (args.tag), "ttH Hadronic|Loose MVA Fit Presel.|Post-Fit")

do_mva = False
if do_mva:
  parallel_utils.run('./ttHHadronicMVABabyMaker "ttHHadronic_RunII_MVA_Presel" "RunII" "%s_RunII_MVA_Presel_scale_diphoton" "scale_diphoton"' % (args.tag))

  os.chdir("../MVAs/")
  parallel_utils.run('python prep.py --input "../Loopers/MVABaby_ttHHadronic_%s_RunII_MVA_Presel_scale_diphoton" --channel "Hadronic"' % (args.tag))
  parallel_utils.run('python train.py --input "ttHHadronic_%s_RunII_MVA_Presel_scale_diphoton_features.hdf5" --channel "Hadronic" --tag "_%s_RunII_MVA_Presel_scale_diphoton" --ext ""' % (args.tag, args.tag))
