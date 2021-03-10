import sys, os
sys.path.append("../")
import itertools
import parallel_utils
import workflow_utils
import argparse
import subprocess
import hadd_helper
from func_make_za_plot import make_za_plot
from func_make_za_plot import make_data_mc_plots

parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to denote with", type=str)
parser.add_argument("--baby_version", help = "which version of babies to use", type=str)
args = parser.parse_args()

do_looping = True
perform_BabyMaker = False
perform_Loopers = False 
perform_plotWraper = False 
do_mvas = True

#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#

os.chdir("../")
if do_looping:
    ## NOTE: to process 16+17+18, just remove the --years option for BabyMaker and Loopers; For Plots, just assign "std" instead of "std_2017" ##
    subprocess.call('make', shell=True)

    script = "looper_wrapper.py"
    version = args.baby_version
    tag = args.tag + "_impute_hct_BDT"
    selection = "ttHHadronic_RunII_MVA_Presel"

    if perform_BabyMaker:
        parallel_utils.run('python %s --fcnc --babymaker --channel "Hadronic" --baby_version "%s" --tag "%s" --selection %s --bkg_options "impute" --years "2016"' % (script, version, tag, selection))
        parallel_utils.run('python %s --fcnc --babymaker --channel "Hadronic" --baby_version "%s" --tag "%s" --selection %s --bkg_options "impute" --years "2017"' % (script, version, tag, selection))
        parallel_utils.run('python %s --fcnc --babymaker --channel "Hadronic" --baby_version "%s" --tag "%s" --selection %s --bkg_options "impute" --years "2018"' % (script, version, tag, selection))

        hadd_helper.merge_mva_babies(tag, True) # only 2018 + signals for the moment

    if perform_Loopers:
        parallel_utils.run('python %s --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection %s --bkg_options "impute" --years "2016"' % (script, version, tag, selection))
        parallel_utils.run('python %s --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection %s --bkg_options "impute" --years "2017"' % (script, version, tag, selection))
        parallel_utils.run('python %s --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection %s --bkg_options "impute" --years "2018"' % (script, version, tag, selection))
        hadd_helper.merge(tag, True)

    if perform_plotWraper: # Data-MC comparison
        os.chdir("Plots")
        signals = "TT_FCNC_hct"
        signals = "TprimeBToTH_M-600|TprimeBToTH_M-1000|ttH"
        signals = "TprimeBToTH_M-600|TprimeBToTH_M-900|TprimeBToTH_M-1200|ttH"
        backgrounds = "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG"
        labels = "" # "FCNC Hadronic|Loose MVA Presel."
        make_data_mc_plots("ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_impute_hct_BDT_FCNC"), "std", labels, signals, backgrounds)

#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#

if do_mvas:
    os.chdir("../MVAs/")
    my_own_tag = "_tprime" # reminder: "prep" + ${suffix} + ".py"

    ##------------------------- MVA Prep -------------------------#
    #command_list = []
    #inputfile = "../Loopers/MVABaby_ttHHadronic_%s_FCNC.root" % (args.tag + "_impute_hct_BDT")
    #command_list.append('python prep%s.py --dont_train_with_dnn --input %s --channel "Hadronic" --fcnc_hct --tag "%s"' % (my_own_tag, inputfile, my_own_tag))
    #parallel_utils.submit_jobs(command_list, 1)
    #print "after prep..."

    ###------------------------- MVA Training -------------------------#
    inputfile = "ttHHadronic_%s_FCNC_features%s.hdf5" % (args.tag + "_impute_hct_BDT", my_own_tag)
    parallel_utils.run('python train.py --input %s --channel "Hadronic" --tag "%s" --ext ""' % (inputfile, my_own_tag + "_impute_hct")) 
    print "after training..."

    ###------------------------- ZA Plots -------------------------#
    #dir = "depository_symbolicLink"
    #subprocess.call("ls -ld " + dir, shell=True)
    #make_za_plot(dir, "Hadronic", "hct")
