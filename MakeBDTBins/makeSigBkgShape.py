from ROOT import *
from tdrStyle import *
setTDRStyle()
from subprocess import call
import filenameDict as nameDict

gSystem.AddIncludePath("-I$CMSSW_BASE/src/ ")
#gSystem.Load("$CMSSW_BASE/lib/slc6_amd64_gcc481/libHiggsAnalysisCombinedLimit.so")
gSystem.Load("$CMSSW_BASE/lib/slc6_amd64_gcc630/libHiggsAnalysisCombinedLimit.so")
gSystem.AddIncludePath("-I$ROOFITSYS/include")
gSystem.AddIncludePath("-Iinclude/")
RooMsgService.instance().getStream(1).removeTopic(ROOT.RooFit.DataHandling)
RooMsgService.instance().getStream(1).removeTopic(ROOT.RooFit.ObjectHandling)

def GetMultiDataset(t, cuts, datanameTag, debug=False):
    w = RooWorkspace("wbkg_13TeV")

    # for tree from train.py
    rv_mass = RooRealVar("mass","",100,180)
    rv_mva_score = RooRealVar("mva_score","",0,1)
    rv_sample_id = RooRealVar("sample_id","",0,2)
    rv_weight = RooRealVar("weight","",-999,999)

    h_mgg_hists = []
    d_mgg_unbin = []
    d_mgg_unbin_rename = []

    for i in range(len(cuts)):

        h_mgg_hists.append(TH1F("h_mgg_hist_" + str(i), "", 160, 100, 180))
        #t.Project(h_mgg_hists[-1].GetName(), "mass_", "evt_weight_*(" + cuts[i] + ")")
        #d_mgg_bin.append(RooDataHist(datanameTag + '_' + str(i), "", RooArgList(w.var("CMS_hgg_mass")), h_mgg_hists[-1], 1) )

        tmpdata = RooDataSet(datanameTag + '_' + str(i),"",t,RooArgSet(rv_mass, rv_sample_id, rv_weight, rv_mva_score), cuts[i])
        d_mgg_unbin.append( RooDataSet(tmpdata.GetName(), "", tmpdata, tmpdata.get(), "1", "weight" ) )

        d_mgg_unbin.append(tmpdata)
        #getattr(w,'import')(d_mgg_bin[-1], datanameTag + '_' + str(i), RooCmdArg())
        getattr(w,'import')(d_mgg_unbin[-1], RooCmdArg())

    w.writeToFile("allData.root")

def GetDataset(t, cut, tag, savepath, doUnbin, debug=False):

    w = RooWorkspace("w")
    w.factory("CMS_hgg_mass[100,180]")
    w.factory("MH[125]")

    h_mgg = TH1F("h_mgg", "h_mgg", 320, 100, 180)
    h_mgg.Sumw2()
    t.Project(h_mgg.GetName(), "mass", "weight*(" + cut + ")")
    #t.Project(h_mgg.GetName(), "mass", "1*(" + cut + ")")
    print "integral", h_mgg.Integral()

    #d_mgg_bin = RooDataHist("roohist_data_mass_TTHHadronicTag", "", RooArgList(w.var("CMS_hgg_mass")), h_mgg, 1)
    d_mgg_bin = RooDataHist("roohist_data_mass_" + tag, "", RooArgList(w.var("CMS_hgg_mass")), h_mgg, 1)
    print "bin dataset", d_mgg_bin.sumEntries(), d_mgg_bin.numEntries()

    rv_sample_id = RooRealVar("sample_id","",0,2)
    rv_mass = RooRealVar("mass","",100,180)
    rv_weight = RooRealVar("weight","",-999,999)
    rv_mva_score = RooRealVar("mva_score","",0,1)
    rv_signal_mass_label = RooRealVar("signal_mass_label","",0,2)
    rv_process_id = RooRealVar("process_id","",0,100)


    #d_mgg_unbin = RooDataSet("roo_data_mass_" + tag, "", t, RooArgSet(rv_mass, rv_sample_id, rv_weight, rv_mva_score, rv_signal_mass_label, rv_process_id), cut)
    #d_mgg_unbin_w = RooDataSet(d_mgg_unbin.GetName(), "", d_mgg_unbin, d_mgg_unbin.get(), "1", "weight")
    #print "unbin dataset", d_mgg_unbin_w.sumEntries(), d_mgg_unbin_w.numEntries()

    #rv_mass_hgg = RooRealVar("CMS_hgg_mass","",100,180)
    #d_mgg_unbin_w.addColumn(rv_mass_hgg)
    #d_mgg_unbin_w.Print()
    ##for i in range(d_mgg_unbin_w.numEntries()):
    #    #print d_mgg_unbin_w.get(i).getRealValue("mass")#, d_mgg_unbin_w.get(i).getRealValue("CMS_hgg_mass")
    ##    rv_mass_hgg.setVal(d_mgg_unbin_w.get(i).getRealValue("mass"))
    ##    d_mgg_unbin_w.add(RooArgSet(rv_mass_hgg))


    if debug:

        h_mgg_sig = TH1F("h_mgg_sig", "h_mgg_sig", 160, 100, 180)
        h_mgg_bkg = TH1F("h_mgg_bkg", "h_mgg_bkg", 160, 100, 180)
        h_mgg_data = TH1F("h_mgg_data", "h_mgg_data", 160, 100, 180)
        t.Project(h_mgg_sig.GetName(), "mass", "weight*( sample_id == 1 && mass > 100 && mass < 180 && signal_mass_label== 0)")
        t.Project(h_mgg_bkg.GetName(), "mass", "weight*( sample_id == 0 && ((mass > 100 && mass < 120) || ( mass > 130 &&mass < 180 )))")
        t.Project(h_mgg_data.GetName(), "mass", "weight*( sample_id == 2 && mass > 100 && mass < 180)")

        c1 = TCanvas("c1", "c1", 800, 800)
        dummy = TH1D("dummy","dummy",1,100,180)
        dummy.SetMinimum(0)
        yMax = h_mgg_data.GetMaximum()*1.5
        #yMax = h_mgg.GetMaximum()*1.5
        dummy.SetMaximum(yMax)
        dummy.SetLineColor(0)
        dummy.SetMarkerColor(0)
        dummy.SetLineWidth(0)
        dummy.SetMarkerSize(0)
        dummy.GetYaxis().SetTitle("Events")
        dummy.GetYaxis().SetTitleOffset(1.3)
        dummy.GetXaxis().SetTitle("m_{#gamma#gamma}")
        dummy.Draw()

        h_mgg_sig.SetFillColor(2)
        h_mgg_bkg.SetFillColor(4)
        mc = THStack("mc","mc")
        mc.Add(h_mgg_bkg)
        mc.Add(h_mgg_sig)
        mc.Draw("same hist")

        h_mgg_data.SetMarkerStyle(20)
        h_mgg_data.Draw("same pe")

        h_mgg.Draw("same pe")

        c1.SaveAs(savepath+"/" + process + "_" + tag + "_data_mc.png")
        c1.SaveAs(savepath+"/" + process + "_" + tag + "_data_mc.pdf")

    if doUnbin:
        return d_mgg_unbin_w
    else:
        return d_mgg_bin, h_mgg.GetMaximum()

def GetSigPdf(events, norm, tag, savename, savepath, modelpath, maxInHist):

    # input is a RooDataSet
    # tag is TTHHadronic_n
    call("mkdir -p models/" + modelpath, shell=True)

    w = RooWorkspace("wsig_13TeV")
    w.factory("CMS_hgg_mass[100,180]")
    w.factory("MH[125]")

    #norm = events.sumEntries()
    print "sumEtries: ", norm

    # change pdf name, parameters name
    w.factory("DoubleCB:"+tag+"(CMS_hgg_mass, mean_"+tag+"[125,120,130], sigma_"+tag+"[1,0,5], a1_"+tag+"[1,0,10], n1_"+tag+"[1,0,10], a2_"+tag+"[1,0,10], n2_"+tag+"[1,0,10])")

    w.pdf(tag).fitTo(events, RooFit.PrintLevel(-1))

    #rv_norm = RooRealVar("rv_norm_"+tag, "", norm)
    if ("FCNC" in tag) and ("Leptonic" in tag):
        norm = norm*1/1.527

    if norm <= 0:
        norm = 1e-09

    rv_norm = RooRealVar(tag+"_norm", "", norm)

    exPdf = RooExtendPdf("extend" + tag, "", w.pdf(tag), rv_norm)
    getattr(w,'import')(rv_norm)
    getattr(w,'import')(exPdf)

    frame = w.var("CMS_hgg_mass").frame()
    events.plotOn(frame)
    w.pdf(tag).plotOn(frame)
    #w.pdf(tag).paramOn(frame)

    c1 = TCanvas("c1", "c1", 800, 800)
    dummy = TH1D("dummy","dummy",1,100,180)
    dummy.SetMinimum(0)
    yMax = maxInHist*1.2
    dummy.SetMaximum(yMax)
    dummy.SetLineColor(0)
    dummy.SetMarkerColor(0)
    dummy.SetLineWidth(0)
    dummy.SetMarkerSize(0)
    dummy.GetYaxis().SetTitle("Events")
    dummy.GetYaxis().SetTitleOffset(1.3)
    dummy.GetXaxis().SetTitle("m_{#gamma#gamma} (GeV)")
    dummy.Draw()

    latex = TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.6*c1.GetTopMargin())
    latex.SetTextFont(42)
    latex.SetTextAlign(11)
    latex.SetTextColor(1)

    latex.DrawLatex(0.5, 0.85, (tag.split("_"))[-2] + "_" + (tag.split("_"))[-1])
    latex.DrawLatex(0.5, 0.78, "mean = " + str(round(w.var("mean_"+tag).getVal(), 3) ) + " #pm " + str(round(w.var("mean_"+tag).getError(), 3) ))
    latex.DrawLatex(0.5, 0.71, "sigma = " + str(round(w.var("sigma_"+tag).getVal(), 3) ) + " #pm " + str(round(w.var("sigma_"+tag).getError(), 3) ))
    latex.DrawLatex(0.5, 0.64, "a1 = " + str(round(w.var("a1_"+tag).getVal(), 3) ) + " #pm " + str(round(w.var("a1_"+tag).getError(), 3) ))
    latex.DrawLatex(0.5, 0.57, "a2 = " + str(round(w.var("a2_"+tag).getVal(), 3) ) + " #pm " + str(round(w.var("a2_"+tag).getError(), 3) ))
    latex.DrawLatex(0.5, 0.50, "n1 = " + str(round(w.var("n1_"+tag).getVal(), 3) ) + " #pm " + str(round(w.var("n1_"+tag).getError(), 3) ))
    latex.DrawLatex(0.5, 0.43, "n2 = " + str(round(w.var("n2_"+tag).getVal(), 3) ) + " #pm " + str(round(w.var("n2_"+tag).getError(), 3) ))

    frame.Draw("same")
    c1.SaveAs(savepath + "/fit_sig_" + savename + ".png")
    c1.SaveAs(savepath + "/fit_sig_" + savename + ".pdf")

    w.var("mean_"+tag).setConstant()
    w.var("sigma_"+tag).setConstant()
    w.var("a1_"+tag).setConstant()
    w.var("a2_"+tag).setConstant()
    w.var("n1_"+tag).setConstant()
    w.var("n2_"+tag).setConstant()

    w.writeToFile("models/" + modelpath + "/" + savename + ".root")

def GetBkgPdf(events, tag, savename, savepath, modelpath, maxInHist):

    call("mkdir -p models/" + modelpath, shell=True)
    nBins = 80

    # input is a RooDataSet
    # tag is TTHHadronic_n or TTHLeptonic_n
    w = RooWorkspace("wbkg_13TeV")
    w.factory("CMS_hgg_mass[100,180]")

    norm = events.sumEntries()
    print "norm: ", norm

    w.var("CMS_hgg_mass").setRange("SL", 100.0, 120.0)
    w.var("CMS_hgg_mass").setRange("SU", 130.0, 180.0)
    w.var("CMS_hgg_mass").setRange("full", 100, 180)
    w.var("CMS_hgg_mass").setRange("blind",120,130)

    w.factory("Exponential:"+tag+"(CMS_hgg_mass, tau[-2,-10,0])")
    w.factory("ExtendPdf:"+tag+"_ext("+tag+", nevt[100,0,10000000], 'full')")

    #w.pdf(tag).fitTo(events, RooFit.Range("SL,SU"), RooFit.Minos())
    w.pdf(tag+"_ext").fitTo(events, RooFit.Range("SL,SU"),RooFit.Extended(True), RooFit.PrintLevel(-1))

    frame = w.var("CMS_hgg_mass").frame()

    #events.Print()
    #events.plotOn(frame, RooFit.Cut("mRegion==mRegion::Sideband"), RooFit.Binning(80))
    events.plotOn(frame, RooFit.Binning(nBins))

    #w.pdf(tag).plotOn(frame)#,RooFit.Range(100,180))
    #w.pdf(tag).paramOn(frame)
    w.pdf(tag+"_ext").plotOn(frame)#,RooFit.Range(100,180))
    #w.pdf(tag+"_ext").paramOn(frame)

    c1 = TCanvas("c1", "c1", 800, 800)
    dummy = TH1D("dummy","dummy",1,100,180)
    dummy.SetMinimum(0)
    yMax = maxInHist*1.2*(320.0/nBins)
    dummy.SetMaximum(yMax)
    dummy.SetLineColor(0)
    dummy.SetMarkerColor(0)
    dummy.SetLineWidth(0)
    dummy.SetMarkerSize(0)
    dummy.GetYaxis().SetTitle("Events")
    dummy.GetYaxis().SetTitleOffset(1.3)
    dummy.GetXaxis().SetTitle("m_{#gamma#gamma} (GeV)")
    dummy.Draw()

    frame.Draw("same")

    latex = TLatex()
    latex.SetNDC()
    latex.SetTextSize(0.6*c1.GetTopMargin())
    latex.SetTextFont(42)
    latex.SetTextAlign(11)
    latex.SetTextColor(1)

    latex.DrawLatex(0.4, 0.85, (tag.split("_"))[-2] + "_" + (tag.split("_"))[-1])
    latex.DrawLatex(0.4, 0.78, "nEvents = " + str(round(w.var("nevt").getVal(), 3) ) + " #pm " + str(round(w.var("nevt").getError(), 3) ))
    latex.DrawLatex(0.4, 0.71, "#tau = " + str(round(w.var("tau").getVal(), 3) ) + " #pm " + str(round(w.var("tau").getError(), 3) ))

    c1.SaveAs(savepath + "/fit_bkg_" + savename + ".png")
    c1.SaveAs(savepath + "/fit_bkg_" + savename + ".pdf")

    #l=ROOT.RooArgSet(w.var("CMS_hgg_mass"))
    #frac = w.pdf(tag).createIntegral(l,l,"blind")
    #print "frac", frac.getVal()
    #norm = norm/(1-frac.getVal())
    nEvt = w.var("nevt").getVal()
    w.factory(tag+"_norm["+str(nEvt)+",0,"+str(3*nEvt)+"]")
    print nEvt

    #events.SetName(events.GetName() + "_" + args.nbin)
    getattr(w,'import')(events, RooCmdArg())
    w.writeToFile("models/" + modelpath + "/" + savename + ".root")

    call("echo " + str(nEvt) + " > models/" + modelpath + "/" + savename + ".txt" , shell=True)
#filename = "ttHHadronic_1617_FinalFitTree.root"
#filename = "ttHHadronic_v05.08_FinalFitTree.root"
#filename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.5_8May2019_forHualin_2017_Presel_impute_FinalFitTree.root"
#filename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.5_1May2019_RunII_MVA_Presel_impute_cutPtoM_FinalFitTree.root"
#filename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.5_1May2019_RunII_MVA_Presel_impute_FinalFitTree.root"
#filename = "/home/users/sjmay/ttH/Loopers/MVABaby_ttHHadronic_v1.5_8May2019_forHualin_2017_Presel_impute.root"
#filename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__unblinded_forHualin_15May2019_2017_Presel_impute_FinalFitTree.root"
#filename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__unblinded_forHualin_15May2019_RunII_MVA_Presel_impute_cutPtoM_FinalFitTree.root"
#hadfilename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__unblinded_forHualin_15May2019_RunII_MVA_Presel_impute_FinalFitTree.root"
#filename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.5_forHualin_16May2019_RunII_MVA_Presel_impute_cutPtoM_FinalFitTree.root"
#filename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.5_forHualin_16May2019_RunII_MVA_Presel_impute_FinalFitTree.root"

## had
#hadfilename_tth = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.6_28May2019_RunII_MVA_Presel_impute_addDNNs_addTopTag_FinalFitTree.root"
#hadfilename_fcnc = "/home/users/sjmay/ttH/MVAs/ttHHadronic_4June2019_v1.7_impute_FinalFitTree.root"
##hadfilename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__unblinded_forHualin_15May2019_2017_Presel_impute_FinalFitTree.root"
## lep
#lepfilename_tth = "/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.6_28May2019_RunII_MVA_Presel_addDNN_FinalFitTree.root"
#lepfilename_fcnc = "/home/users/sjmay/ttH/MVAs/ttHLeptonic_4June2019_v1.7_FinalFitTree.root"
#lepfilename = "/home/users/hmei/ttH/MVAs/ttHLeptonic_test2017_FinalFitTree.root"



## do scan
import argparse
def ParseOption():

    parser = argparse.ArgumentParser(description='submit all')
    parser.add_argument('-l', dest='low', type=str, help='low bdt cut')
    parser.add_argument('--hi', dest='high', type=str, help='high bdt cut')
    #parser.add_argument('-b', dest='nbin', type=str, help='bin number')
    parser.add_argument('-i', dest='index', type=str, help='for version')
    parser.add_argument('--tag', dest='tag', type=str, help='lep or had')
    parser.add_argument('--modelPath', dest='modelPath', type=str, help='model path')
    parser.add_argument('--savepath', dest='savepath', type=str, help='model path')
    parser.add_argument("--doData", help = "get bkg shape from data sideband instead of mc", default = False, action="store_true")
    parser.add_argument("--skipBkg", help = "skip making bkg model", default = False, action="store_true")
    parser.add_argument("-p", dest='process', type=str)
    args = parser.parse_args()
    return args

args=ParseOption()

tag = args.tag
filename = ""

if "TTHHadronicTag" in tag:
    filename = nameDict.namedict["TTHHadronicTag"] #hadfilename_tth
if "TTHLeptonicTag" in tag:
    filename = nameDict.namedict["TTHLeptonicTag"] #lepfilename_tth
if "FCNCHadronicTag" in tag:
    filename = nameDict.namedict["FCNCHadronicTag"] #hadfilename_fcnc
if "FCNCLeptonicTag" in tag:
    filename = nameDict.namedict["FCNCLeptonicTag"] #lepfilename_fcnc
if "FCNCHUTHadronicTag" in tag:
    filename = nameDict.namedict["FCNCHUTHadronicTag"] #hadfilename_fcnc
if "FCNCHUTLeptonicTag" in tag:
    filename = nameDict.namedict["FCNCHUTLeptonicTag"] #lepfilename_fcnc
if "FCNCHCTHadronicTag" in tag:
    filename = nameDict.namedict["FCNCHCTHadronicTag"] #hadfilename_fcnc
if "FCNCHCTLeptonicTag" in tag:
    filename = nameDict.namedict["FCNCHCTLeptonicTag"] #lepfilename_fcnc

process = args.process
savepath = args.savepath

f = TFile.Open(filename)
t = f.Get("t")

processIDMap = {"ttH_hgg":0, "ggH_hgg":14, "VBF_hgg":15, "VH_hgg":16, "THQ_hgg":11, "THW_hgg":12,
                "TT_FCNC_hut":22, "TT_FCNC_hct":23, "ST_FCNC_hut":24, "ST_FCNC_hct":25}

modelpath = args.modelPath

bkgSampleCut = "sample_id == 0 &&  process_id != 0 "
if args.doData:
    bkgSampleCut = "sample_id == 2"

# ------------------------------------------------------------------------#
## This part is prepare inputs for binning scan
### sample_id: 0 bkg, 1 sig, 2 data
### signal_mass_label: 0 mH125

sigCut = "mva_score > " + str(args.low) + " && mva_score < " + str(args.high) + " && signal_mass_label == 0 && process_id == " + str(processIDMap[process])
#sigCut = "tth_2017_reference_mva > " + str(args.low) + " && tth_2017_reference_mva < " + str(args.high) + " && signal_mass_label == 0 && process_id == " + str(processIDMap[process])

bkgCut = "mva_score > " + str(args.low) + " && mva_score < " + str(args.high) + " && " + bkgSampleCut + " && ((mass > 100 && mass < 120) || ( mass > 130 && mass < 180)) "
#bkgCut = "tth_2017_reference_mva > " + str(args.low) + " && tth_2017_reference_mva < " + str(args.high) + " && " + bkgSampleCut + " && ((mass > 100 && mass < 120) || ( mass > 130 && mass < 180)) "

d_mgg_sig, maxInHist = GetDataset(t, sigCut, tag, savepath, False, False) # mH125
if (process == "ggH_hgg") or (process == "VBF_hgg"):
    sigCut2 = "mva_score > " + str(args.low) + " && mva_score < " + str(args.high) + " && signal_mass_label == 0 && process_id == 0"
#    sigCut2 = "tth_2017_reference_mva > " + str(args.low) + " && tth_2017_reference_mva < " + str(args.high) + " && signal_mass_label == 0 && process_id == 0"
    d_mgg_sig_2, maxInHist_2 = GetDataset(t, sigCut2, tag, savepath, False, False)
    GetSigPdf(d_mgg_sig_2, d_mgg_sig.sumEntries(), "hggpdfsmrel_" + process + "_" + tag, "CMS-HGG_sigfit_mva_" + process + "_" + tag, savepath, modelpath, maxInHist_2)
else:
    GetSigPdf(d_mgg_sig, d_mgg_sig.sumEntries(), "hggpdfsmrel_" + process + "_" + tag, "CMS-HGG_sigfit_mva_" + process + "_" + tag, savepath, modelpath, maxInHist)

if not args.skipBkg:
    d_mgg_bkg, maxInHist = GetDataset(t, bkgCut, tag, savepath,False, False)
    print "d_mgg_bkg: ", d_mgg_bkg.numEntries()
    GetBkgPdf(d_mgg_bkg,"CMS_hgg_bkgshape_" + tag, "CMS-HGG_bkg_" + tag, savepath, modelpath, maxInHist)

# ------------------------------------------------------------------------#
## This part is to provide inputs for making RooMultiPdf -> envelope method
'''
datanameTag = "test_13TeV_TTHHadronicTag"
cut1 = 0.2
cut2 = 0.3
# cut on pt/mgg before bdt
cut3 = 0.9585
#filename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.5_forHualin_16May2019_RunII_MVA_Presel_impute_cutPtoM_FinalFitTree.root"
# no cut on pt/mgg before bdt
#cut3 = 0.98
#filename = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.5_forHualin_16May2019_RunII_MVA_Presel_impute_FinalFitTree.root"

### using tree from train.py
cutbase = "sample_id == 2 && ((mass > 100 && mass < 180)) && "
cutBins = ["mva_score > " + str(cut3), "mva_score > " + str(cut2) + " && mva_score < " + str(cut3), "mva_score > " + str(cut1) + " && mva_score < " + str(cut2)]

cuts = []
for i in range(len(cutBins)):
    cuts.append(cutbase + cutBins[i])
#GetMultiDataset(t, cuts, datanameTag, debug=False)
'''
call('chmod -R 755 ~/public_html', shell=True)
