import sys, os
import json
import glob
import numpy

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("channel", help = "Hadronic or Leptonic", type=str)
parser.add_argument("selection", help = "e.g. ttHLeptonicLoose", type=str)
parser.add_argument("year", help = "e.g. 2016 or 2017", type=str)
parser.add_argument("n_trainings", help = "number of trainings to run", type=int)
parser.add_argument("n_hyperparameter_points", help = "number of hyperparameter points to take from json file", type=int)
parser.add_argument("--dry_run", help = "just check the workflow (don't actually train BDTs)", action="store_true")
args = parser.parse_args()

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def find_nearest(array,value):
    val = numpy.ones_like(array)*value
    idx = (numpy.abs(array-val)).argmin()
    return array[idx], idx

mean_data = 2.6140475044 
mean_mc = 2.45864720897
unc_data = 0.103457610201
unc_mc = 0.0857337372867

def calc_za_and_unc(file_pattern):
  files = glob.glob(file_pattern)
  max_za_data = []
  max_za_mc = []
  for file in files:
    data = numpy.load(file)
    value, start_idx = find_nearest(data["n_sig_mc"], 2.5)
    za_data = data["za_data"] # for data, we already require at least 4 data events in sideband to trust Z_A estimate
    za_mc = data["za_mc"][start_idx:] # for mc, require at least 2.5 signal events to trust Z_A estimate

    max_za_data.append(numpy.max(za_data))
    max_za_mc.append(numpy.max(za_mc))


  filter(lambda v: v==v, max_za_data)
  filter(lambda v: v==v, max_za_mc)

  mean_mc = numpy.mean(max_za_mc)
  mean_unc_mc = unc_mc / (float(len(files)) ** 0.5)
  std_dev_mc = numpy.std(max_za_mc)

  mean_data = numpy.mean(max_za_data)
  mean_unc_data = unc_data / (float(len(files)) ** 0.5)
  std_dev_data = numpy.std(max_za_data)  

  #print "[data] Mean of max Z_A: %.3f +/- %.3f " % (numpy.mean(max_za_data), unc_data / (float(len(files)) ** 0.5))
  #print "[mc] Mean of max Z_A: %.3f +/- %.3f " % (numpy.mean(max_za_mc), unc_mc / (float(len(files)) ** 0.5))

  #print "[data] Std. Dev of max Z_A: %.3f " % numpy.std(max_za_data)
  #print "[mc] Std. Dev of max Z_A: %.3f " % numpy.std(max_za_mc)

  vars_of_interest = {
        "max_za_data" : [max_za_data, "Max. Z_A (data)"],
        "max_za_mc" : [max_za_mc, "Max. Z_A (mc)"],
  }
  vars_of_interest = {}

  fig = plt.figure()
  bins = numpy.linspace(2, 4, 40)
  for var, info in vars_of_interest.iteritems():
    dist_to_plot = info[0]
    plt.hist(dist_to_plot, label = info[1], bins = bins)
    plt.ylabel("# of MVA Trainings")
    plt.xlabel(info[1])
    plt.xlim([2.0, 4.0])
    plt.ylim([0.0, 15.0])
    plt.legend(loc = 'upper right')
    plt.text(3.5, 10, "Mean       = %.2f" % numpy.mean(dist_to_plot))
    plt.text(3.5, 8,  "Std. Dev. = %.2f" % numpy.std(dist_to_plot))
    #print file_pattern.replace(".npz", "") + var +'.pdf'
    plt.savefig("n_minus_one_plots/" + (file_pattern.replace(".npz", "")).replace("Optimization/ZA_curves/", "") + var +'.pdf')
    plt.close('all')

  results = { 	"mean_mc" : mean_mc, "mean_unc_mc" : mean_unc_mc, "std_dev_mc" : std_dev_mc,
		"mean_data" : mean_data, "mean_unc_data" : mean_unc_data, "std_dev_data" : std_dev_data,
	}
  return results

vars = { # dictionary of all potential variables to study
	"helicity_angle" : { "name" : "helic", "type" : "double", "function" : "helicity(lead_photon, sublead_photon)", "latex_name" : "Helicity Angle ($\\theta$)"},
	"lepton_pt" : { "name" : "lep_pt_", "type" : "double", "function" : "leps[0].Pt()", "latex_name" : "Lepton $p_T$"},
	"min_dr_lead_pho" : { "name" : "min_dr_lead_photon", "type" : "double", "function" : "min_dr(lead_photon, objects)", "latex_name" : "Min ($\\Delta R(\\gamma_{\\text{lead}}, \\text{leptons/jets})$)"},
	"min_dr_sublead_pho" : { "name" : "min_dr_sublead_photon", "type" : "double", "function" : "min_dr(sublead_photon, objects)", "latex_name" : "Min ($\\Delta R(\\gamma_{\\text{sublead}}, \\text{leptons/jets})$)"},
	"n_leps" : { "name" : "n_leps_", "type" : "int", "function" : "leps.size()", "latex_name" : "$N_{\\text{leps}}$"},
	"lepton_eta" : { "name" : "lep_eta_", "type" : "double", "function" : "leps[0].Eta()", "latex_name" : "Lepton $\eta$"},
	"nb_loose" : { "name" : "nb_loose_", "type" : "int", "function" : "nb_loose()", "latex_name" : "$N_{\\text{b-jets}}$ (loose)"},
	"max_btag" : { "name" : "max1_btag_", "type" : "double", "function" : "btag_scores_sorted[0].second", "latex_name" : "highest $b$-tag score"},
	"second_max_btag" : { "name" : "max2_btag_", "type" : "double", "function" : "btag_scores_sorted[1].second", "latex_name" : "2nd highest $b$-tag score"},
	"dR_higgs_lep" : { "name" : "dR_higgs_lep", "type" : "double", "function" : "diphoton.DeltaR(leps[0])", "latex_name" : "$\Delta R(p_{\gamma \gamma}, \\text{lep})$"},
	"dR_higgs_W" : { "name" : "dR_higgs_W", "type" : "double", "function" : "deltaR_Higgs_W(jets, diphoton)", "latex_name" : "$\Delta R(p_{\gamma \gamma}, W)$"},
	"pt_higgs" : { "name" : "pt_higgs", "type" : "double", "function" : "diphoton.Pt() / diphoton.M()", "latex_name" : "$p_T^{\gamma \gamma} / m_{\gamma \gamma}$"},
	"diphoton_dR" : { "name" : "dipho_deltaR", "type" : "double", "function" : "lead_photon.DeltaR(sublead_photon)", "latex_name" : "$\Delta R(\gamma_1, \gamma_2)$"},
	"max_phoIDMVA" : { "name" : "maxIDMVA_", "type" : "double", "function" : "leadIDMVA() > subleadIDMVA() ? leadIDMVA() : subleadIDMVA()", "latex_name" : "Max $\gamma$ ID MVA"},
	"min_phoIDMVA" : { "name" : "minIDMVA_", "type" : "double", "function" : "leadIDMVA() <= subleadIDMVA() ? leadIDMVA() : subleadIDMVA()", "latex_name" : "Min $\gamma$ ID MVA"},
	"ht" : { "name" : "ht_", "type" : "double", "function" : "get_ht(jets)", "latex_name" : "$H_T$"},
	"n_jets" : { "name" : "njets_", "type" : "int", "function" : "n_jets()", "latex_name" : "$N_{\\text{jets}}$"},
	"jet1_pt" : { "name" : "jet1_pt_", "type" : "double", "function" : "jet_pt1()", "latex_name" : "Jet 1 $p_T$"},
	"jet2_pt" : { "name" : "jet2_pt_", "type" : "double", "function" : "jet_pt2()", "latex_name" : "Jet 2 $p_T$"},
	"jet3_pt" : { "name" : "jet3_pt_", "type" : "double", "function" : "jet_pt3()", "latex_name" : "Jet 3 $p_T$"},
	"jet4_pt" : { "name" : "jet4_pt_", "type" : "double", "function" : "jet_pt4()", "latex_name" : "Jet 4 $p_T$"},
	"jet5_pt" : { "name" : "jet5_pt_", "type" : "double", "function" : "jet_pt5()", "latex_name" : "Jet 5 $p_T$"}, 
	"jet1_eta" : { "name" : "jet1_eta_", "type" : "double", "function" : "jet_eta1()", "latex_name" : "Jet 1 $\eta$"},
        "jet2_eta" : { "name" : "jet2_eta_", "type" : "double", "function" : "jet_eta2()", "latex_name" : "Jet 2 $\eta$"},
        "jet3_eta" : { "name" : "jet3_eta_", "type" : "double", "function" : "jet_eta3()", "latex_name" : "Jet 3 $\eta$"},
        "jet4_eta" : { "name" : "jet4_eta_", "type" : "double", "function" : "jet_eta4()", "latex_name" : "Jet 4 $\eta$"},
        "jet5_eta" : { "name" : "jet5_eta_", "type" : "double", "function" : "jet_eta5()", "latex_name" : "Jet 5 $\eta$"},
	"lead_pt_over_m" : { "name" : "leadptoM_", "type" : "double", "function" : "lead_ptoM()", "latex_name" : "Lead $p_T / m_{\gamma \gamma}$"},
	"sublead_pt_over_m" : { "name" : "subleadptoM_", "type" : "double", "function" : "sublead_ptoM()", "latex_name" : "Sublead $p_T / m_{\gamma \gamma}$"},
	"lead_eta" : { "name" : "lead_eta_", "type" : "double", "function" : "leadEta()", "latex_name" : "Lead Photon $\eta$"},
	"sublead_eta" : { "name" : "sublead_eta_", "type" : "double", "function" : "subleadEta()", "latex_name" : "Sublead Photon $\eta$"},
	"lead_psv" : { "name" : "leadPSV_", "type" : "int", "function" : "leadPixelSeed()", "latex_name" : "Lead PSV"},
	"sublead_psv" : { "name" : "subleadPSV_", "type" : "int", "function" : "subleadPixelSeed()", "latex_name" : "Sublead PSV"},
	"diphoton_cosine_delta_phi" : { "name" : "dipho_cosphi_", "type" : "double", "function" : "dipho_cosphi()", "latex_name" : "$|\cos(\Delta \phi (\gamma_1, \gamma_2))|$"},
	"diphoton_rapidity" : { "name" : "dipho_rapidity_", "type" : "double", "function" : "dipho_rapidity()", "latex_name" : "$Y_{\gamma \gamma}$"},
	"met" : { "name" : "met_", "type" : "double", "function" : "MetPt()", "latex_name" : "$E_T^{\\text{miss}}$"},
	"mt" : { "name" : "mt_", "type" : "double", "function" : "mT()", "latex_name" : "$m_T(l, E_T^{\\text{miss}})$"}	
}


cats = { # sort all of these into categories
  	"b_tagging" : {"features" : [vars["nb_loose"], vars["max_btag"], vars["second_max_btag"]], "latex_name" : "b-tagging Features"},
	"jet_kinematics" : { "features" : [vars["n_jets"], vars["ht"], vars["jet1_pt"], vars["jet1_eta"], vars["jet2_pt"], vars["jet2_eta"], vars["jet3_pt"], vars["jet3_eta"], vars["jet4_pt"], vars["jet4_eta"], vars["jet5_pt"], vars["jet5_eta"]], "latex_name" : "Jet Kinematic Features"},
	"diphoton" : { "features" : [vars["helicity_angle"], vars["dR_higgs_lep"], vars["dR_higgs_W"], vars["pt_higgs"], vars["diphoton_dR"], vars["diphoton_cosine_delta_phi"], vars["diphoton_rapidity"]], "latex_name" : "DiPhoton Features"},
	"individual_photon" : { "features" : [vars["lead_pt_over_m"], vars["sublead_pt_over_m"], vars["lead_eta"], vars["sublead_eta"], vars["lead_psv"], vars["sublead_psv"], vars["max_phoIDMVA"], vars["min_phoIDMVA"], vars["min_dr_lead_pho"], vars["min_dr_sublead_pho"]], "latex_name" : "Individual Photon Features"},
	"lepton" : { "features" : [vars["lepton_pt"], vars["lepton_eta"], vars["mt"], vars["n_leps"]], "latex_name" : "Lepton Features"},
	"other" : { "features" : [vars["met"]], "latex_name" : "Other Features"},
}


baseline_vars = [ # variables to store in the baseline BDT that we use as a starting point to build up from
	"n_jets",
	"nb_loose",
	"lead_psv",
	"sublead_psv",
	"max_phoIDMVA",
	"min_phoIDMVA",
	"lepton_pt",
]

do_results = True
do_hyperparameter_scan = False

# First, remove all variables 

if do_hyperparameter_scan:
  baseline_results = {}
  for var, info in vars.iteritems():
      print "python add_bdt_variable.py '%s' '%s' '%s' '%s' --remove" % (info["name"], info["type"], info["function"], args.channel)
      os.system("python add_bdt_variable.py '%s' '%s' '%s' '%s' --remove" % (info["name"], info["type"], info["function"], args.channel))

  # Now, add in variables for baseline BDT
  for var in baseline_vars:
    info = vars[var]
    print "python add_bdt_variable.py '%s' '%s' '%s' '%s'" % (info["name"], info["type"], info["function"], args.channel)
    os.system("python add_bdt_variable.py '%s' '%s' '%s' '%s'" % (info["name"], info["type"], info["function"], args.channel))

  build_success = os.system("make")
  if build_success != 0:
    print "Errors building, will not run rest of workflow"
    os.system("exit(1)")

  if not args.dry_run:
    for i in range(args.n_hyperparameter_points):
      print "python bdt_ducks.py '%s' '%s' '%s' '%s' '%s'" % (args.channel, args.selection, args.year, "hyperparameter_grid_search_%s" % i, args.n_trainings)
      os.system("python bdt_ducks.py '%s' '%s' '%s' '%s' '%s'" % (args.channel, args.selection, args.year, "hyperparameter_grid_search_%s" % i, args.n_trainings))

if do_results:
  scan_results = {}
  max_za_mc = numpy.empty(args.n_hyperparameter_points)
  max_za_data = numpy.empty(args.n_hyperparameter_points)
  with open("../MVAs/hyperparameter_points.json") as f_in:
    all_hyperparams = json.load(f_in)
  for i in range(args.n_hyperparameter_points):
    print "Optimization/ZA_curves/MVAOptimizationBaby_*_%s_hyperparameter_grid_search_%s_*_bdt.npz" % (args.channel, str(i))
    hyperparam_score = calc_za_and_unc("Optimization/ZA_curves/MVAOptimizationBaby_*_%s_hyperparameter_grid_search_%s_*_bdt.npz" % (args.channel, str(i)))
    max_za_mc[i] = hyperparam_score["mean_mc"]
    max_za_data[i] = hyperparam_score["mean_data"]
    scan_results[str(i)] = { "mean_mc" : hyperparam_score["mean_mc"], "mean_data" : hyperparam_score["mean_data"], "hyperparameters" : all_hyperparams[str(i)] } 
  
  sort_indices = numpy.argsort(max_za_mc)

  with open("hyperparameter_scan_results.txt", "w") as f_out:
    for i in range(args.n_hyperparameter_points):
      f_out.write(json.dumps(scan_results[str(sort_indices[i])]) + '\n')
 
