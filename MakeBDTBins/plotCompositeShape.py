from ROOT import *
from tdrStyle import *
setTDRStyle()


class Model():

    def __init__(self, config):

        self.debug = False

        self.tag = config["tag"]
        ## absorb nth bdt bin in tag
        #self.nBins = config["nBins"]
        self.modelpath = config["modelpath"]
        self.savepath = config["savepath"]
        self.sigNames = config["sigNames"]
        self.bkgNames_res = config["bkgNames_res"]
        self.bkgNames_nonRes = config["bkgNames_nonRes"]
        self.savename = config["savename"]

        self.sigFiles = {}
        self.resBkgFiles = {}
        self.nonResBkgFiles = {}


        self.sigWorkspaces = {}
        self.resBkgWorkspaces = {}
        self.nonResBkgWorkspaces = {}

        self.sigYields = {}
        self.resBkgYields = {}
        self.nonResBkgYields = {}

        self.sigShapes = {}
        self.resBkgShapes = {}
        self.nonResBkgShapes = {}

        self.totalYield = 0

        self.var = RooRealVar("CMS_hgg_mass","",100,180)

        self.nGen = 100000
        #self.model = RooAbsPdf("model")

    def GetFiles(self):

        if self.debug:
            call("ls " + self.modelpath, shell=True)

        # signal
        for sig in self.sigNames:
            self.sigFiles[sig] = TFile.Open(self.modelpath + "CMS-HGG_sigfit_mva_" + sig + "_" + self.tag + ".root")
            self.sigWorkspaces[sig] = self.sigFiles[sig].Get("wsig_13TeV")
            self.sigShapes[sig] = self.sigWorkspaces[sig].pdf("hggpdfsmrel_" + sig + "_" + self.tag)
            self.sigYields[sig] = self.sigWorkspaces[sig].var("hggpdfsmrel_" + sig + "_" + self.tag + "_norm").getVal()
            self.totalYield += self.sigYields[sig]

        # resonant bkgground
        for resBkg in self.bkgNames_res:
            self.resBkgFiles[resBkg] = TFile.Open(self.modelpath + "CMS-HGG_sigfit_mva_" + resBkg + "_" + self.tag + ".root")
            self.resBkgWorkspaces[resBkg] = self.resBkgFiles[resBkg].Get("wsig_13TeV")
            self.resBkgShapes[resBkg] = self.resBkgWorkspaces[resBkg].pdf("hggpdfsmrel_" + resBkg + "_" + self.tag)
            self.resBkgYields[resBkg] = self.resBkgWorkspaces[resBkg].var("hggpdfsmrel_" + resBkg + "_" + self.tag + "_norm").getVal()
            self.totalYield += self.resBkgYields[resBkg]

        # non-resonant background, data sideband
        self.nonResBkgFiles[self.bkgNames_nonRes] = TFile.Open(self.modelpath + "CMS-HGG_bkg_" + self.tag + ".root")
        self.nonResBkgWorkspaces[self.bkgNames_nonRes] = self.nonResBkgFiles[self.bkgNames_nonRes].Get("wbkg_13TeV")
        self.nonResBkgShapes[self.bkgNames_nonRes]= self.nonResBkgWorkspaces[self.bkgNames_nonRes].pdf("CMS_hgg_bkgshape_" + self.tag)
        self.nonResBkgYields[self.bkgNames_nonRes] = self.nonResBkgWorkspaces[self.bkgNames_nonRes].var("CMS_hgg_bkgshape_" + self.tag + "_norm").getVal()

        self.totalYield += self.nonResBkgYields[self.bkgNames_nonRes]
        if self.debug:
            for sig in self.sigNames:
                self.sigFiles[sig].ls()
                self.sigWorkspaces[sig].Print()
            for resBkg in self.bkgNames_res:
                self.resBkgFiles[resBkg].ls()
                self.resBkgWorkspaces[resBkg].Print()
            self.nonResBkgFiles[self.bkgNames_nonRes].ls()
            self.nonResBkgWorkspaces[self.bkgNames_nonRes].Print()


    def AssembleModels(self):

        pdfs_al = RooArgList("pdfs")
        pdfs_bkg_as = RooArgSet("pdfs_resBkg")
        fracs_al = RooArgList("fracs")
        fracs_list = {}

        nsig = 0
        nbkg_res = 0
        nbkg_nonRes = 0

        npdfs = len(self.sigShapes) + len(self.resBkgShapes) + len(self.nonResBkgShapes)

        print "check"
        for sig in self.sigNames:
            pdfs_al.add(self.sigShapes[sig])
            fracs_list["frac_"+sig] = RooRealVar("frac_"+sig,"",self.sigYields[sig]/self.totalYield)
            fracs_list["frac_"+sig].Print()
            fracs_al.add(fracs_list["frac_"+sig])
            nsig += self.sigYields[sig]

        for resBkg in self.bkgNames_res:
            pdfs_al.add(self.resBkgShapes[resBkg])
            pdfs_bkg_as.add(self.resBkgShapes[resBkg])
            fracs_list["frac_"+resBkg] = RooRealVar("frac_"+resBkg,"",self.resBkgYields[resBkg]/self.totalYield)
            fracs_al.add(fracs_list["frac_"+resBkg])
            nbkg_res += self.resBkgYields[resBkg]

        pdfs_al.add(self.nonResBkgShapes[self.bkgNames_nonRes])
        pdfs_bkg_as.add(self.nonResBkgShapes[self.bkgNames_nonRes])
        nbkg_nonRes += self.nonResBkgYields[self.bkgNames_nonRes]

        pdfs_al.Print()
        fracs_al.Print()


        self.model = RooAddPdf("model","",pdfs_al,fracs_al)
        self.model.Print()

        print self.totalYield

        data_p = self.model.generate(RooArgSet(self.var),self.nGen)
        weight = RooRealVar("weight","",self.totalYield/self.nGen)
        weight_ = data_p.addColumn(weight)
        data_w = RooDataSet("data1D_w","",data_p,data_p.get(),"1","weight")

        nonResBkg_component = RooArgSet(self.nonResBkgShapes[self.bkgNames_nonRes])
        bkg_component = pdfs_bkg_as

#model.plotOn(xframe,ROOT.RooFit.Components(bkg_peak_component),ROOT.RooFit.LineStyle(2),ROOT.RooFit.LineColor(2))
        frame = self.var.frame()

        data_w.plotOn(frame, RooFit.Name("data"))
        self.model.plotOn(frame, RooFit.Name("s+b"))
        self.model.plotOn(frame, RooFit.Components(nonResBkg_component),RooFit.LineStyle(2), RooFit.Name("nonRes"))
        self.model.plotOn(frame, RooFit.Components(bkg_component),RooFit.LineStyle(2),RooFit.LineColor(2), RooFit.Name("bkg"))


        legend = TLegend(0.5,0.7,0.9,0.9)
        legend.AddEntry(frame.findObject("data"), 'pseudo data', 'pe')
        legend.AddEntry(frame.findObject("s+b"), 'signal + background', 'l')
        legend.AddEntry(frame.findObject("bkg"), 'background', 'l')
        legend.AddEntry(frame.findObject("nonRes"), 'non-resonant bkg', 'l')
        legend.SetTextSize(0.03)
        legend.SetLineWidth(2)
        legend.SetFillColor(0)
        legend.SetBorderSize(1)

        c = TCanvas("c","",800,800)

        frame.GetXaxis().SetTitle("m_{#gamma#gamma}")
        frame.Draw()
        legend.Draw('SAME')

        latex = TLatex()
        latex.SetNDC()
        latex.SetTextSize(0.45*c.GetTopMargin())
        latex.SetTextFont(42)
        latex.SetTextAlign(11)
        latex.DrawLatex(0.15, 0.2, self.tag)
        latex.DrawLatex(0.5, 0.5, "#non-resonant bkg: " + str(round(nbkg_nonRes,1)))
        latex.DrawLatex(0.5, 0.55, "#resonant bkg: " + str(round(nbkg_res,1)))
        latex.DrawLatex(0.5, 0.6, "#signal (ST+TT): " + str(round(nsig,1)))


        c.SaveAs("/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/" + self.savename + ".png")
        c.SaveAs("/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/" + self.savename + ".pdf")



config_hct_lep_v0 = {\
"tag":"FCNCHCTLeptonicTag_v0",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hct_lep_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hct", "TT_FCNC_hct"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HCTLep_v0"
}
config_hct_lep_v1 = {\
"tag":"FCNCHCTLeptonicTag_v1",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hct_lep_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hct", "TT_FCNC_hct"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HCTLep_v1"
}
config_hct_lep_v2 = {\
"tag":"FCNCHCTLeptonicTag_v2",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hct_lep_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hct", "TT_FCNC_hct"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HCTLep_v2"
}
config_hct_lep_v3 = {\
"tag":"FCNCHCTLeptonicTag_v3",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hct_lep_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hct", "TT_FCNC_hct"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HCTLep_v3"
}
config_hut_lep_v0 = {\
"tag":"FCNCHUTLeptonicTag_v0",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hut_lep_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hut", "TT_FCNC_hut"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HUTLep_v0"
}
config_hut_lep_v1 = {\
"tag":"FCNCHUTLeptonicTag_v1",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hut_lep_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hut", "TT_FCNC_hut"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HUTLep_v1"
}
config_hut_lep_v2 = {\
"tag":"FCNCHUTLeptonicTag_v2",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hut_lep_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hut", "TT_FCNC_hut"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HUTLep_v2"
}
config_hut_lep_v3 = {\
"tag":"FCNCHUTLeptonicTag_v3",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hut_lep_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hut", "TT_FCNC_hut"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HUTLep_v3"
}
config_hct_had_v0 = {\
"tag":"FCNCHCTHadronicTag_v0",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hct_had_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hct", "TT_FCNC_hct"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HCTHad_v0"
}
config_hct_had_v1 = {\
"tag":"FCNCHCTHadronicTag_v1",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hct_had_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hct", "TT_FCNC_hct"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HCTHad_v1"
}
config_hct_had_v2 = {\
"tag":"FCNCHCTHadronicTag_v2",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hct_had_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hct", "TT_FCNC_hct"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HCTHad_v2"
}
config_hct_had_v3 = {\
"tag":"FCNCHCTHadronicTag_v3",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hct_had_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hct", "TT_FCNC_hct"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HCTHad_v3"
}
config_hut_had_v0 = {\
"tag":"FCNCHUTHadronicTag_v0",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hut_had_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hut", "TT_FCNC_hut"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HUTHad_v0"
}
config_hut_had_v1 = {\
"tag":"FCNCHUTHadronicTag_v1",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hut_had_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hut", "TT_FCNC_hut"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HUTHad_v1"
}
config_hut_had_v2 = {\
"tag":"FCNCHUTHadronicTag_v2",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hut_had_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hut", "TT_FCNC_hut"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HUTHad_v2"
}
config_hut_had_v3 = {\
"tag":"FCNCHUTHadronicTag_v3",
"modelpath":"/home/users/hmei/ttH/MakeBDTBins/models/FCNC_hut_had_final_20190702_v1/",
"savepath":"/home/users/hmei/public_html/2019/20190702_checkShape_FCNC/",
"sigNames":["ST_FCNC_hut", "TT_FCNC_hut"],
"bkgNames_res":["ggH_hgg", "VBF_hgg", "ttH_hgg", "THQ_hgg", "THW_hgg"],
"bkgNames_nonRes":"data",
"savename":"HUTHad_v3"
}

model_hut_lep_v0 = Model(config_hut_lep_v0)
model_hut_lep_v0.GetFiles()
model_hut_lep_v0.AssembleModels()
model_hut_lep_v1 = Model(config_hut_lep_v1)
model_hut_lep_v1.GetFiles()
model_hut_lep_v1.AssembleModels()
model_hut_lep_v2 = Model(config_hut_lep_v2)
model_hut_lep_v2.GetFiles()
model_hut_lep_v2.AssembleModels()
model_hut_lep_v3 = Model(config_hut_lep_v3)
model_hut_lep_v3.GetFiles()
model_hut_lep_v3.AssembleModels()

model_hct_lep_v0 = Model(config_hct_lep_v0)
model_hct_lep_v0.GetFiles()
model_hct_lep_v0.AssembleModels()
model_hct_lep_v1 = Model(config_hct_lep_v1)
model_hct_lep_v1.GetFiles()
model_hct_lep_v1.AssembleModels()
model_hct_lep_v2 = Model(config_hct_lep_v2)
model_hct_lep_v2.GetFiles()
model_hct_lep_v2.AssembleModels()
model_hct_lep_v3 = Model(config_hct_lep_v3)
model_hct_lep_v3.GetFiles()
model_hct_lep_v3.AssembleModels()



model_hut_had_v0 = Model(config_hut_had_v0)
model_hut_had_v0.GetFiles()
model_hut_had_v0.AssembleModels()
model_hut_had_v1 = Model(config_hut_had_v1)
model_hut_had_v1.GetFiles()
model_hut_had_v1.AssembleModels()
model_hut_had_v2 = Model(config_hut_had_v2)
model_hut_had_v2.GetFiles()
model_hut_had_v2.AssembleModels()
model_hut_had_v3 = Model(config_hut_had_v3)
model_hut_had_v3.GetFiles()
model_hut_had_v3.AssembleModels()

model_hct_had_v0 = Model(config_hct_had_v0)
model_hct_had_v0.GetFiles()
model_hct_had_v0.AssembleModels()
model_hct_had_v1 = Model(config_hct_had_v1)
model_hct_had_v1.GetFiles()
model_hct_had_v1.AssembleModels()
model_hct_had_v2 = Model(config_hct_had_v2)
model_hct_had_v2.GetFiles()
model_hct_had_v2.AssembleModels()
model_hct_had_v3 = Model(config_hct_had_v3)
model_hct_had_v3.GetFiles()
model_hct_had_v3.AssembleModels()

from subprocess import call
call("chmod -R 755 /home/users/hmei/public_html/", shell=True)


