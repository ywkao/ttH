from ROOT import *
from tdrStyle import *
setTDRStyle()

savepath = "/home/users/hmei/public_html/2019/20190529_FCNC/"
filename = "../ttHHadronic_RunII_MVA_Presel_veryLoose_testFCNC_20190529_v2_impute_histogramsRunII.root"
f = TFile.Open(filename)

names = ["hMassTop1", "hMassTop2", "hMass", "hPhotonLeadPToM", "hPhotonSubleadPToM", "hPhotonLeadEta", "hPhotonSubleadEta", "hDiphotonCosPhi", "hNJets", "hNbTight"]
xmin = [0,0,100,0,0,-3,-3,0,0,-0.5]
xmax = [500,500,180,3,3,3,3,1,10,5]
ymax = [0.12,0.06,1,0.3,0.4,0.2,0.2,1,1,1]
xTitle = ["m_{#gamma#gammaj} (GeV)", "m_{jjj} (GeV)", "m_{#gamma#gamma} (GeV)", "lead p_T/m_{#gamma#gamma}", "sublead p_T/m_{#gamma#gamma}", "lead #eta", "sublead #eta", "cos(#Delta#phi)", "nJets", "nTightBJets"]

processes = ["ttH", "FCNC_hut", "FCNC_hct", "TTGG", "DiPhoton"]
colors = [1,2,4,8,6]


for i in range(len(names)):
    hists = []
    c = TCanvas("c","",800,800)
    legend = TLegend(0.55,0.65,0.85,0.9)
    dummy = TH1D("dummy","dummy",1,xmin[i],xmax[i])
    dummy.SetMinimum(0)
    dummy.SetMaximum(ymax[i])
    dummy.SetLineColor(0)
    dummy.SetMarkerColor(0)
    dummy.SetLineWidth(0)
    dummy.SetMarkerSize(0)
    dummy.GetXaxis().SetTitle(xTitle[i])
    #dummy.GetYaxis().SetTitleOffset(1.3)
    #dummy.GetYaxis().SetTitle("Measured #sigma_")
    dummy.Draw()
    for j in range(len(processes)):
        hists.append(f.Get(names[i] + "_" + processes[j]))
        hists[-1].Scale(1/hists[-1].Integral())
        hists[-1].SetLineWidth(2)
        hists[-1].SetLineColor(colors[j])
        hists[-1].Draw("hist same")
        legend.AddEntry(hists[-1],processes[j],'l')
    legend.SetTextSize(0.03)
    legend.SetLineWidth(1)
    legend.SetFillColor(0)
    legend.SetBorderSize(1)
    legend.Draw("same")

    c.SaveAs(savepath+names[i]+".png")
    c.SaveAs(savepath+names[i]+".pdf")

from subprocess import call
call("chmod -R 755 ~/public_html", shell=True)
