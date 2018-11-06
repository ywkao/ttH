from subprocess import call
import multiprocessing

import argparse
def ParseOption():

    parser = argparse.ArgumentParser(description='submit all')
    parser.add_argument('-t', dest='tag', type=str, help='tag')

    args = parser.parse_args()
    return args

#inputBaby = "../Loopers/MVABaby_ttHHadronic_multiClass_promptness_toptag_forHggMeeting.root"
inputBaby = "../Loopers/MVABabies/MVABaby_ttHHadronic_MVABaby_ttHHadronic_multiClass_promptness_toptag_forRanking_v0.root"
outName = "testBaseBDT"

baseVars = ['njets_', 'nb_loose_', 'maxIDMVA_', 'minIDMVA_']
extraVars_diPho = ["dipho_delta_R", "diphoptom_", "dipho_cosphi_", "dipho_rapidity_"]
extraVars_perPho = ["leadptoM_", "subleadptoM_", "lead_eta_", "sublead_eta_", "leadPSV_", "subleadPSV_", "lead_phi_", "sublead_phi_"]
extraVars_jet1 = ["jet1_pt_", "jet1_eta_", "jet1_btag_", "jet1_phi_"]
extraVars_jet2 = ["jet2_pt_", "jet2_eta_", "jet2_btag_", "jet2_phi_"]
extraVars_jet3 = ["jet3_pt_", "jet3_eta_", "jet3_btag_", "jet3_phi_"]
extraVars_jet4 = ["jet4_pt_", "jet4_eta_", "jet4_btag_", "jet4_phi_"]
extraVars_jet5 = ["jet5_pt_", "jet5_eta_", "jet5_btag_", "jet5_phi_"] 
extraVars_jet6 = ["jet6_pt_", "jet6_eta_", "jet6_btag_", "jet6_phi_"]
extraVars_perTop = ["topTag_score_",  "topTag_pT_", "topTag_eta_", "topTag_phi_", "topTag_topMass_"]
extraVars_perEvent = ["ht_",  "met_", "max1_btag_", "max2_btag_"]

call("python prep.py -f " + inputBaby + " -o " + outName + " -l " + " ".join(baseVars), shell=True)
call("python getZa.py -f rootfiles/" + outName + ".root -o " + outName, shell=True)

extraVars_perJet = extraVars_jet1 + extraVars_jet2 + extraVars_jet3 + extraVars_jet4 + extraVars_jet5 + extraVars_jet6
allExtras = extraVars_diPho + extraVars_perPho + extraVars_perJet + extraVars_perTop + extraVars_perEvent

allExtras_dict = {}
allExtras_dict['extraVars_diPho'] = extraVars_diPho
allExtras_dict['extraVars_perPho'] = extraVars_perPho
allExtras_dict['extraVars_perTop'] = extraVars_perTop
allExtras_dict['extraVars_perEvent'] = extraVars_perEvent
allExtras_dict['extraVars_jet1'] = extraVars_jet1
allExtras_dict['extraVars_jet2'] = extraVars_jet2
allExtras_dict['extraVars_jet3'] = extraVars_jet3
allExtras_dict['extraVars_jet4'] = extraVars_jet4
allExtras_dict['extraVars_jet5'] = extraVars_jet5
allExtras_dict['extraVars_jet6'] = extraVars_jet6

def SeparateTrain(tag):
    #print extraVars_, tag
    for var_ in allExtras_dict[tag]:
        outName_ = "rankBDT_" + tag + "_" + var_
        trainVars_ = []
        for base in baseVars: 
            trainVars_.append(base)
        trainVars_.append(var_)
        cmd1 = "python prep.py -f " + inputBaby + " -o " + outName_ + " -l " + " ".join(trainVars_)
        cmd2 = "python getZa.py -f rootfiles/" + outName_ + ".root -o " + outName_
        print cmd1
        call(cmd1 + ";" + cmd2, shell=True)

args=ParseOption()
SeparateTrain(args.tag)
