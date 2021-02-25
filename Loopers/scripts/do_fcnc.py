# vim: set fdm=marker:
# imports, parser{{{
import sys, os
sys.path.append("../")
import itertools
import parallel_utils
import workflow_utils
import argparse
import subprocess
from func_make_za_plot import make_za_plot
from func_make_za_plot import make_data_mc_plots

parser = argparse.ArgumentParser()
parser.add_argument("--tag", help = "tag to denote with", type=str)
parser.add_argument("--baby_version", help = "which version of babies to use", type=str)
args = parser.parse_args()
#}}}
# bdt.xml files{{{
#bdt_lep = "../MVAs/Leptonic_4June2019_v1.7_FCNC_bdt.xml"
#bdt_had = "../MVAs/Hadronic_12June2019_v1.7_impute_FCNC_bdt.xml"

#bdt_lep_hut = "../MVAs/Leptonic_v1.7_27Jun2019_FCNC_SingleBDT_hut__bdt.xml"
#bdt_lep_hct = "../MVAs/Leptonic_v1.7_27Jun2019_FCNC_SingleBDT_hct__bdt.xml"
#bdt_had_hut = "../MVAs/Hadronic_v1.7_27Jun2019_FCNC_SingleBDT_impute_hut__bdt.xml"
#bdt_had_hct = "../MVAs/Hadronic_v1.7_27Jun2019_FCNC_SingleBDT_impute_hct__bdt.xml"

bdt_lep_hut = "../MVAs/Leptonic_v4.11_14Jan2020_hut__bdt.xml"
bdt_lep_hct = "../MVAs/Leptonic_v4.11_14Jan2020_hct__bdt.xml"
bdt_had_hut = "../MVAs/Hadronic_v4.11_14Jan2020_impute_hut__bdt.xml"
bdt_had_hct = "../MVAs/Hadronic_v4.11_14Jan2020_impute_hct__bdt.xml"
#}}}
#def bash_command(command):{{{
def bash_command(command):
    p = Popen('%s' % (command), shell=True)
    result = os.waitpid(p.pid, 0)
#}}}

import glob

from subprocess import *
os.chdir("../")
do_looping = True
if do_looping:
    bash_command('echo Hello World! "(from do_fcnc.py)"; ' + 'pwd; ')
    bash_command("make")
    ## NOTE: to process 16+17+18, just remove the --years option for BabyMaker and Loopers; For Plots, just assign "std" instead of "std_2017" ##

    ##--------------- BabyMaker ---------------##
    #parallel_utils.run('python looper_wrapper.py --babymaker --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017"' % (args.baby_version, args.tag + "_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --babymaker --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017"' % (args.baby_version, args.tag + "_hct_BDT"))
    #parallel_utils.run('python looper_wrapper.py --babymaker --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2017"' % (args.baby_version, args.tag + "_impute_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --babymaker --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2017"' % (args.baby_version, args.tag + "_impute_hct_BDT"))

    ##--------------- Loopers ---------------##
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2016"' % (args.baby_version, args.tag + "_impute_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2016"' % (args.baby_version, args.tag + "_impute_hct_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2017"' % (args.baby_version, args.tag + "_impute_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2017"' % (args.baby_version, args.tag + "_impute_hct_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2018"' % (args.baby_version, args.tag + "_impute_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2018"' % (args.baby_version, args.tag + "_impute_hct_BDT"))

    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2016"' % (args.baby_version, args.tag + "_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2016"' % (args.baby_version, args.tag + "_hct_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017"' % (args.baby_version, args.tag + "_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017"' % (args.baby_version, args.tag + "_hct_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2018"' % (args.baby_version, args.tag + "_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2018"' % (args.baby_version, args.tag + "_hct_BDT"))

    ##--------------- Data-MC Plots ---------------##
    #os.chdir("Plots")
    #make_data_mc_plots("ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_impute_hut_BDT_FCNC"), "std_2016", "FCNC Hadronic|Loose MVA Presel.", "TT_FCNC_hut|ST_FCNC_hut", "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG")
    #make_data_mc_plots("ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_impute_hct_BDT_FCNC"), "std_2016", "FCNC Hadronic|Loose MVA Presel.", "TT_FCNC_hct|ST_FCNC_hct", "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG")
    #make_data_mc_plots("ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_hut_BDT_FCNC"), "std_2016", "FCNC Leptonic|Loose MVA Presel.", "TT_FCNC_hut|ST_FCNC_hut", "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG")
    #make_data_mc_plots("ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_hct_BDT_FCNC"), "std_2016", "FCNC Leptonic|Loose MVA Presel.", "TT_FCNC_hct|ST_FCNC_hct", "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG")
    ##os.chdir("../")

    #os.chdir("scripts/")
    #subprocess.call("time ./quick.sh", shell=True)
    #os.chdir("../")

    ##--------------- Data-MC Plots ---------------##
    os.chdir("Plots")
    make_data_mc_plots("ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_impute_hut_BDT_FCNC"), "std", "FCNC Hadronic|Loose MVA Presel.", "TT_FCNC_hut|ST_FCNC_hut", "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG")
    make_data_mc_plots("ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_impute_hct_BDT_FCNC"), "std", "FCNC Hadronic|Loose MVA Presel.", "TT_FCNC_hct|ST_FCNC_hct", "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG")
    make_data_mc_plots("ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_hut_BDT_FCNC"), "std", "FCNC Leptonic|Loose MVA Presel.", "TT_FCNC_hut|ST_FCNC_hut", "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG")
    make_data_mc_plots("ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_hct_BDT_FCNC"), "std", "FCNC Leptonic|Loose MVA Presel.", "TT_FCNC_hct|ST_FCNC_hct", "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG")
    #os.chdir("../")

do_mvas = False
if do_mvas:
    os.chdir("../MVAs/")
    #my_own_tag = "_three_methods" # reminder: "prep" + ${suffix} + ".py"
    #my_own_tag = "_chi2_updated"

    ###--------------- MVA Prep ---------------#
    #command_list = []
    #command_list.append('python prep%s.py --dont_train_with_dnn --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --fcnc_hut --tag "%s"' % (my_own_tag, args.tag + "_hut_BDT", my_own_tag))
    #command_list.append('python prep%s.py --dont_train_with_dnn --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --fcnc_hct --tag "%s"' % (my_own_tag, args.tag + "_hct_BDT", my_own_tag))
    #command_list.append('python prep%s.py --dont_train_with_dnn --input "../Loopers/MVABaby_ttHHadronic_%s_FCNC.root" --channel "Hadronic" --fcnc_hut --tag "%s"' % (my_own_tag, args.tag + "_impute_hut_BDT", my_own_tag)) 
    #command_list.append('python prep%s.py --dont_train_with_dnn --input "../Loopers/MVABaby_ttHHadronic_%s_FCNC.root" --channel "Hadronic" --fcnc_hct --tag "%s"' % (my_own_tag, args.tag + "_impute_hct_BDT", my_own_tag))
    #parallel_utils.submit_jobs(command_list, 4)
    #print "after prep..."

    ####--------------- MVA Training ---------------#
    #parallel_utils.run('python train.py --input "ttHLeptonic_%s_FCNC_features%s.hdf5" --channel "Leptonic" --tag "%s" --ext ""' % (args.tag + "_hut_BDT", my_own_tag, my_own_tag + "_hut"))
    #parallel_utils.run('python train.py --input "ttHLeptonic_%s_FCNC_features%s.hdf5" --channel "Leptonic" --tag "%s" --ext ""' % (args.tag + "_hct_BDT", my_own_tag, my_own_tag + "_hct"))
    #parallel_utils.run('python train.py --input "ttHHadronic_%s_FCNC_features%s.hdf5" --channel "Hadronic" --tag "%s" --ext ""' % (args.tag + "_impute_hut_BDT", my_own_tag, my_own_tag + "_impute_hut"))
    #parallel_utils.run('python train.py --input "ttHHadronic_%s_FCNC_features%s.hdf5" --channel "Hadronic" --tag "%s" --ext ""' % (args.tag + "_impute_hct_BDT", my_own_tag, my_own_tag + "_impute_hct")) 
    #print "after training..."

    ###--------------- ZA Plots ---------------#
    dir = "depository_symbolicLink"
    bash_command("ls -ld " + dir)
    make_za_plot(dir, "Leptonic", "hut")
    make_za_plot(dir, "Leptonic", "hct")
    make_za_plot(dir, "Hadronic", "hut")
    make_za_plot(dir, "Hadronic", "hct")
