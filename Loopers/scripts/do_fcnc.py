# vim: set fdm=marker:
# imports, parser{{{
import sys, os
sys.path.append("../")
import itertools
import parallel_utils
import workflow_utils
import argparse
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

from subprocess import *
os.chdir("../")
do_looping = True
if do_looping:
    bash_command('echo Hello World! "(from do_fcnc.py)"; ' + 'pwd; ')
    bash_command("make")

    # BabyMaker{{{
    #parallel_utils.run('python looper_wrapper.py --babymaker --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017"' % (args.baby_version, args.tag + "_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --babymaker --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017"' % (args.baby_version, args.tag + "_hct_BDT"))
    #parallel_utils.run('python looper_wrapper.py --babymaker --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2017"' % (args.baby_version, args.tag + "_impute_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --babymaker --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2017"' % (args.baby_version, args.tag + "_impute_hct_BDT"))
    #}}}
    # Loopers{{{
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017"' % (args.baby_version, args.tag + "_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2017"' % (args.baby_version, args.tag + "_impute_hut_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Leptonic" --baby_version "%s" --tag "%s" --selection "ttHLeptonic_RunII_MVA_Presel" --bkg_options "none" --years "2017"' % (args.baby_version, args.tag + "_hct_BDT"))
    #parallel_utils.run('python looper_wrapper.py --fcnc --channel "Hadronic" --baby_version "%s" --tag "%s" --selection "ttHHadronic_RunII_MVA_Presel" --bkg_options "impute" --years "2017"' % (args.baby_version, args.tag + "_impute_hct_BDT"))
    #}}}
    # Data/MC Plots{{{
    #os.chdir("Plots")
    #parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std" --plot_labels "FCNC Leptonic|Loose MVA Presel." --signals "TT_FCNC_hut|ST_FCNC_hut" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG"' % ("ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_hut_BDT_FCNC")))
    #parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std" --plot_labels "FCNC Leptonic|Loose MVA Presel." --signals "TT_FCNC_hct|ST_FCNC_hct" --backgrounds "DiPhoton|GammaJets|TTGG|TTGJets|TTJets|VG"' % ("ttHLeptonic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_hct_BDT_FCNC")))
    #parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std" --plot_labels "FCNC Hadronic|Loose MVA Presel." --signals "TT_FCNC_hut|ST_FCNC_hut" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG"' % ("ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_impute_hut_BDT_FCNC")))
    #parallel_utils.run('python plot_wrapper.py --input_file "../%s" --plot_type "std" --plot_labels "FCNC Hadronic|Loose MVA Presel." --signals "TT_FCNC_hct|ST_FCNC_hct" --backgrounds "DiPhoton|QCD_GammaJets_imputed|TTGG|TTGJets|TTJets|VG"' % ("ttHHadronic_RunII_MVA_Presel_%s_histogramsRunII.root" % (args.tag + "_impute_hct_BDT_FCNC")))
    #os.chdir("../")
    #}}}





def make_za_plot(dir, channel, coupling):
  pre = "za_" + channel + "__"
  if channel == "Leptonic":
      suf = coupling + ".npz"
  else:
      suf = "impute_" + coupling + ".npz"

  ### Remark: none, top tagger, chi-2/quad, meng-cheng, three together, combined + ctag, combined + CvsL CvsB
  #labels = "No top taggers, top tagger, chi-2/quad.-eq., new tagger, three methods together, all + c-tag, all + cvsl and cvsb"
  #files = '%s/%s_none_%s, %s_topTagger_%s, %s_chi2_%s, %s_mengcheng_only_%s, %s_three_methods_%s, %s_ctagMengCheng_%s, %s_combined_cvslcvsB_%s' % (dir, pre, suf, pre, suf, pre, suf, pre, suf, pre, suf, pre, suf, pre, suf)

  ### Remark: none, top tagger, chi-2/quad, meng-cheng, three together
  #labels = "No top taggers, top tagger, chi-2/quad.-eq., new tagger, three methods together"
  #files = '%s/%s_none_%s, %s_topTagger_%s, %s_chi2_%s, %s_mengcheng_only_%s, %s_three_methods_%s' % (dir, pre, suf, pre, suf, pre, suf, pre, suf, pre, suf)

  ### Remark: none, top tagger + chi-2/quad, both + ctag, both + ctag + meng-cheng
  #labels = "No top taggers, top tagger + chi-2/quad.-eq., both + c-tag, both + c-tag + new taggers"
  #files = '%s/%s_none_%s, %s_all_%s, %s_ctagAll_%s, %s_ctagMengCheng_%s' % (dir, pre, suf, pre, suf, pre, suf, pre, suf)

  ### Remark: three, three + ctag, three + cvsl and cvsb, three + all DeepCSV
  #labels = "Three together, three together + c-tag, three together + cvsl and cvsb, three together + all DeepCSV discriminants"
  #files = '%s_three_methods_%s, %s_ctagMengCheng_%s, %s_combined_cvslcvsB_%s, %s_mengcheng_%s' % (pre, suf, pre, suf, pre, suf, pre, suf)

  ### Remark: none, 1+2, 2+3, 3+1, three together
  labels = "No top taggers, top tagger + chi-2/quad.-eq., chi-2/quad.-eq. + new tagger, new tagger + top tagger three methods together"
  files = '%s/%s_none_%s, %s_all_%s, %s_chi2_mengcheng_%s, %s_mengcheng_toptagger_%s, %s_three_methods_%s' % (dir, pre, suf, pre, suf, pre, suf, pre, suf, pre, suf)

  parallel_utils.run('python make_za_comparisons.py --inputs "%s" --labels "%s" --tag "%s" --ylim "0.5, 3.0" --colors black blue green red orange' % (files, labels, channel + "_" + coupling))





do_mvas = True
if do_mvas:
  os.chdir("../MVAs/")
  my_own_tag = "_three_methods" # reminder: "prep" + ${suffix} + ".py"

  ##--------------- MVA Prep ---------------#
  #command_list = []
  #command_list.append('python prep%s.py --dont_train_with_dnn --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --fcnc_hut --tag "%s"' % (my_own_tag, args.tag + "_hut_BDT", my_own_tag))
  #command_list.append('python prep%s.py --dont_train_with_dnn --input "../Loopers/MVABaby_ttHLeptonic_%s_FCNC.root" --channel "Leptonic" --fcnc_hct --tag "%s"' % (my_own_tag, args.tag + "_hct_BDT", my_own_tag))
  #command_list.append('python prep%s.py --dont_train_with_dnn --input "../Loopers/MVABaby_ttHHadronic_%s_FCNC.root" --channel "Hadronic" --fcnc_hut --tag "%s"' % (my_own_tag, args.tag + "_impute_hut_BDT", my_own_tag)) 
  #command_list.append('python prep%s.py --dont_train_with_dnn --input "../Loopers/MVABaby_ttHHadronic_%s_FCNC.root" --channel "Hadronic" --fcnc_hct --tag "%s"' % (my_own_tag, args.tag + "_impute_hct_BDT", my_own_tag))
  #parallel_utils.submit_jobs(command_list, 4)
  #print "after prep..."

  ##--------------- MVA Training ---------------#
  #parallel_utils.run('python train.py --input "ttHLeptonic_%s_FCNC_features%s.hdf5" --channel "Leptonic" --tag "%s" --ext ""' % (args.tag + "_hut_BDT", my_own_tag, my_own_tag + "_hut"))
  #parallel_utils.run('python train.py --input "ttHLeptonic_%s_FCNC_features%s.hdf5" --channel "Leptonic" --tag "%s" --ext ""' % (args.tag + "_hct_BDT", my_own_tag, my_own_tag + "_hct"))
  #parallel_utils.run('python train.py --input "ttHHadronic_%s_FCNC_features%s.hdf5" --channel "Hadronic" --tag "%s" --ext ""' % (args.tag + "_impute_hut_BDT", my_own_tag, my_own_tag + "_impute_hut"))
  #parallel_utils.run('python train.py --input "ttHHadronic_%s_FCNC_features%s.hdf5" --channel "Hadronic" --tag "%s" --ext ""' % (args.tag + "_impute_hct_BDT", my_own_tag, my_own_tag + "_impute_hct")) 
  #print "after training..."

  ##--------------- ZA Plots ---------------#
  dir = "depository_symbolicLink"
  bash_command("ls -ld " + dir)

  make_za_plot(dir, "Leptonic", "hut")
  make_za_plot(dir, "Leptonic", "hct")
  make_za_plot(dir, "Hadronic", "hut")
  make_za_plot(dir, "Hadronic", "hct")
