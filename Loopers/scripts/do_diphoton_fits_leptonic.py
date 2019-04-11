import sys, os
sys.path.append("../")

import parallel_utils

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to denote with", type=str)
args = parser.parse_args()

bdt = "none"

os.chdir("../")
parallel_utils.run('./ttHLeptonicLooper "ttHLeptonic_RunII_MVA_Presel" "RunII" "%s" "%s" "none"' % (args.tag + "prefit", bdt))
os.chdir("tt_template_fit")
#parallel_utils.run('python do_fits_qcd.py --input_file "../ttHLeptonic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" --jet_bin "2+"' % (args.tag + "prefit"))
os.chdir("../")

#input("You probably want to update ttHLooper.h with results of DiPhoton Fits. Press any key to continue")

command_list = []
#command_list.append('./ttHLeptonicLooper "ttHLeptonic_RunII_DiPhotonFits_Presel" "RunII" "%s" "%s" "scale_diphoton"' % (args.tag, bdt))
#command_list.append('./ttHLeptonicLooper "ttHLeptonic_RunII_MVA_Presel" "RunII" "%s" "%s" "scale_diphoton"' % (args.tag, bdt))
parallel_utils.submit_jobs(command_list, 2)

os.chdir("Plots")
parallel_utils.run('./makeTables "../ttHLeptonic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" > ttHLeptonic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.txt' % (args.tag + "prefit", args.tag + "prefit"))
parallel_utils.run('./makeTables "../ttHLeptonic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" > ttHLeptonic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.txt' % (args.tag, args.tag))
parallel_utils.run('./makeTables "../ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" > ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.txt' % (args.tag, args.tag))

plot_label = "ttH Leptonic"
parallel_utils.run('./makePlots "std" "../ttHLeptonic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" "%s"' % (args.tag + "prefit", plot_label))
parallel_utils.run('./makePlots "std" "../ttHLeptonic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" "%s"' % (args.tag, plot_label))
parallel_utils.run('./makePlots "std" "../ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" "%s"' % (args.tag, plot_label))

parallel_utils.run('./makePlots "std_linear" "../ttHLeptonic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" "%s"' % (args.tag + "prefit", plot_label))
parallel_utils.run('./makePlots "std_linear" "../ttHLeptonic_RunII_DiPhotonFits_Presel_%s_histogramsRunII.root" "%s"' % (args.tag, plot_label))
parallel_utils.run('./makePlots "std_linear" "../ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" "%s"' % (args.tag, plot_label))

# Make MVA Baby
parallel_utils.run('./ttHLeptonicMVABabyMaker "ttHLeptonic_RunII_MVA_Presel" "RunII" "%s_RunII_MVA_Presel_scale_diphoton" "scale_diphoton"' % (args.tag))

os.chdir("../MVAs/")
parallel_utils.run('python prep.py --input "../Loopers/MVABaby_ttHLeptonic_%s_RunII_MVA_Presel_scale_diphoton" --channel "Leptonic"' % (args.tag))
parallel_utils.run('python train.py --input "ttHLeptonic_%s_RunII_MVA_Presel_scale_diphoton_features.hdf5" --channel "Leptonic" --tag "_%s_RunII_MVA_Presel_scale_diphoton" --ext ""' % (args.tag, args.tag))
