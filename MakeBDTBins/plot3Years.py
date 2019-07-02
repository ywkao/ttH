from ROOT import *
from tdrStyle import *
setTDRStyle()
import numpy
import root_numpy
from array import array

def quantiles_to_mva_score(n_quantiles, mva_scores): # return mva_scores corresponding to each quantile in n_quantiles
  sorted_mva = numpy.flip(numpy.sort(mva_scores), 0)
  quantiles = []
  mva = []
  for i in range(n_quantiles):
    idx = int((float(i+1) / float(n_quantiles)) * len(sorted_mva)) - 1
    quantiles.append(float(i+1) / float(n_quantiles))
    mva.append(sorted_mva[idx])
#  return quantiles, mva
  print mva
  return mva

def MakePlot(config):

    filename = config["filename"]
    treename = config["treename"]
    plotvar = config["plotvar"]
    bins = config["bins"]
    cut = config["cut"]
    plotname = config["plotname"]
    weightname = config["weightname"]
    xTitle = config["xTitle"]
    savepath = config["savepath"]
    savename = config["savename"]

    f_ = TFile.Open(filename)
    t_ = f_.Get(treename)

    #mva_scores = (root_numpy.tree2array(t_, branches = [plotname], selection = cut + " && year == 2017"))
    #mva_score_b = quantiles_to_mva_score(10, mva_scores)
    #b_ = array('f')
    #for i in range(len(mva_score_b)):
    #    print (mva_score_b[len(mva_score_b)-1-i])[0]
    #    b_.append((mva_score_b[len(mva_score_b)-1-i])[0])

    h_2016 = TH1F(plotname + "_2016", "", bins[0], bins[1], bins[2])
    h_2017 = TH1F(plotname + "_2017", "", bins[0], bins[1], bins[2])
    h_2018 = TH1F(plotname + "_2018", "", bins[0], bins[1], bins[2])
    #h_2016 = TH1F(plotname + "_2016", "", len(mva_score_b)-1,b_)
    #h_2017 = TH1F(plotname + "_2017", "", len(mva_score_b)-1,b_)
    #h_2018 = TH1F(plotname + "_2018", "", len(mva_score_b)-1,b_)

    cut_2016 = cut + " && year == 2016"
    cut_2017 = cut + " && year == 2017"
    cut_2018 = cut + " && year == 2018"

    t_.Project(h_2016.GetName(), plotvar, weightname + "*(" + cut_2016 + ")")
    t_.Project(h_2017.GetName(), plotvar, weightname + "*(" + cut_2017 + ")")
    t_.Project(h_2018.GetName(), plotvar, weightname + "*(" + cut_2018 + ")")

    print h_2016.Integral()
    print h_2017.Integral()
    print h_2018.Integral()

    h_2016.Scale(1/h_2016.Integral())
    h_2017.Scale(1/h_2017.Integral())
    h_2018.Scale(1/h_2018.Integral())

    c1 = TCanvas("c1", "c1", 800, 800)
    dummy = TH1D("dummy","dummy",1,bins[1],bins[2])
    dummy.SetMinimum(0)
    yMax = max(h_2016.GetMaximum(), h_2017.GetMaximum(), h_2018.GetMaximum())*1.5
    dummy.SetMaximum(yMax)
    dummy.SetLineColor(0)
    dummy.SetMarkerColor(0)
    dummy.SetLineWidth(0)
    dummy.SetMarkerSize(0)
    dummy.GetYaxis().SetTitle("Events/" + str((bins[2]-bins[1])/float(bins[0])) )
    dummy.GetYaxis().SetTitleOffset(1.3)
    dummy.GetXaxis().SetTitle(xTitle)
    dummy.Draw()

    h_2016.SetLineColor(1)
    h_2017.SetLineColor(2)
    h_2018.SetLineColor(4)
    h_2016.SetLineWidth(3)
    h_2017.SetLineWidth(3)
    h_2018.SetLineWidth(3)


    h_2016.Draw("same HIST")
    h_2017.Draw("same HIST")
    h_2018.Draw("same HIST")

    legend = TLegend(0.65,0.75,0.90,0.9)
    legend.AddEntry(h_2016,"2016",'l')
    legend.AddEntry(h_2017,"2017",'l')
    legend.AddEntry(h_2018,"2018",'l')
    legend.SetTextSize(0.03)
    legend.SetLineWidth(1)
    legend.SetFillColor(0)
    legend.SetBorderSize(1)
    legend.Draw("same")

    c1.SaveAs(savepath + savename + ".png")
    c1.SaveAs(savepath + savename + ".pdf")

    import json
    with open(savepath + savename + ".txt", "w") as file:
        file.write(json.dumps(config, indent=4, sort_keys=True))

    from subprocess import call
    call('chmod -R 755 ' + savepath, shell = True)

configs = {}

configs["sigMVA_ttHHad"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.6_28May2019_RunII_MVA_Presel_impute_addDNNs_addTopTag_FinalFitTree.root",
"treename":"t",
"plotvar":"mva_score",
"bins":[10,0,1],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 100 && mass > 180",
"plotname":"mva_score",
"weightname":"weight",
"xTitle":"mva score",
"savename":"mva_score_had_sig",
"savepath":"/home/users/hmei/public_html/2019/20190624_3yearplot/"
}

configs["bkgMCMVA_ttHHad"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.6_28May2019_RunII_MVA_Presel_impute_addDNNs_addTopTag_FinalFitTree.root",
"treename":"t",
"plotvar":"mva_score",
"bins":[10,0,1],
"cut":"sample_id == 0 && mass > 100 && mass > 180",
"plotname":"mva_score",
"weightname":"weight",
"xTitle":"mva score",
"savename":"mva_score_had_bkgMC",
"savepath":"/home/users/hmei/public_html/2019/20190624_3yearplot/"
}

configs["sigMVA_ttHLep"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"mva_score",
"bins":[10,0,1],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 100 && mass > 180",
"plotname":"mva_score",
"weightname":"weight",
"xTitle":"mva score",
"savename":"mva_score_lep_sig",
"savepath":"/home/users/hmei/public_html/2019/20190624_3yearplot/"
}

configs["bkgMCMVA_ttHLep"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"mva_score",
"bins":[10,0,1],
"cut":"sample_id == 0 && mass > 100 && mass > 180",
"plotname":"mva_score",
"weightname":"weight",
"xTitle":"mva score",
"savename":"mva_score_lep_bkgMC",
"savepath":"/home/users/hmei/public_html/2019/20190624_3yearplot/"
}

configs["mgg_ttHLep_sr"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"mass",
"bins":[50,105,140],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 105 && mass < 140 && mva_score > 0.96613055",
"plotname":"mgg",
"weightname":"weight",
"xTitle":"mgg_lep_sr",
"savename":"mgg_lep_sr",
"savepath":"/home/users/hmei/public_html/2019/20190624_3yearplot/"
}

configs["mgg_ttHHad_sr"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"mass",
"bins":[50,105,140],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 105 && mass < 140 && mva_score > 0.9955651 ",
"plotname":"mgg",
"weightname":"weight",
"xTitle":"mgg_had_sr",
"savename":"mgg_had_sr",
"savepath":"/home/users/hmei/public_html/2019/20190624_3yearplot/"
}

configs["maxIDMVA_sig_had"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"global_features[0]",
"bins":[50,-1,1],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 100 && mass < 180 && global_features[5] >= 3 ",
"plotname":"maxIDMVA",
"weightname":"weight",
"xTitle":"maxIDMVA",
"savename":"maxIDMVA_sig_had",
"savepath":"/home/users/hmei/public_html/2019/20190627_3yearplot/"
}

configs["minIDMVA_sig_had"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"global_features[1]",
"bins":[50,-1,1],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 100 && mass < 180 && global_features[5] >= 3 ",
"plotname":"minIDMVA",
"weightname":"weight",
"xTitle":"maxIDMVA",
"savename":"minIDMVA_sig_had",
"savepath":"/home/users/hmei/public_html/2019/20190627_3yearplot/"
}

configs["max2_btag_sig_had"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"global_features[2]",
"bins":[50,0,1],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 100 && mass < 180 && global_features[5] >= 3 ",
"plotname":"max2_btag",
"weightname":"weight",
"xTitle":"max2_btag",
"savename":"max2_btag_sig_had",
"savepath":"/home/users/hmei/public_html/2019/20190627_3yearplot/"
}

configs["max1_btag_sig_had"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"global_features[3]",
"bins":[50,0,1],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 100 && mass < 180 && global_features[5] >= 3 ",
"plotname":"max1_btag",
"weightname":"weight",
"xTitle":"max1_btag",
"savename":"max1_btag_sig_had",
"savepath":"/home/users/hmei/public_html/2019/20190627_3yearplot/"
}

configs["dipho_delta_R"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"global_features[4]",
"bins":[50,0,5],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 100 && mass < 180 && global_features[5] >= 3 ",
"plotname":"dipho_deltaR",
"weightname":"weight",
"xTitle":"dipho_deltaR",
"savename":"dipho_deltaR",
"savepath":"/home/users/hmei/public_html/2019/20190627_3yearplot/"
}

configs["njets"] = {\
"filename":"/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root",
"treename":"t",
"plotvar":"global_features[5]",
"bins":[10,0,10],
"cut":"signal_mass_label == 0 && process_id == 0 && mass > 100 && mass < 180 && global_features[5] >= 3 ",
"plotname":"njets",
"weightname":"weight",
"xTitle":"njets",
"savename":"njets",
"savepath":"/home/users/hmei/public_html/2019/20190627_3yearplot/"
}
import argparse
def ParseOption():

    parser = argparse.ArgumentParser(description='submit all')
    parser.add_argument('-n', dest='name', type=str, help='low bdt cut')
    args = parser.parse_args()
    return args

args=ParseOption()

MakePlot(configs[args.name])
#MakePlot(config2)
#MakePlot(config3)
#MakePlot(config4)
