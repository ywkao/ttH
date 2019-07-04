from ROOT import *

def MakeTables(filename, processes, bins, baseCut, bugTree=False):

    processIDMap = {"ttH_hgg":0, "ggH_hgg":14, "VBF_hgg":15, "VH_hgg":16, "THQ_hgg":11, "THW_hgg":12,
                    "TT_FCNC_hut":22, "TT_FCNC_hct":23, "ST_FCNC_hut":24, "ST_FCNC_hct":25}

    f = TFile.Open(filename)
    t = f.Get("t")

    nEvents = []
    processNames = [] # this var needs to be optimized
    for i in range(len(bins)-1):

        nEvt = "Bin" + str(i+1)
        process_ = ""
        for process in processes:
            processID = processIDMap[process]
            cut = baseCut + " && mva_score > " + str(bins[i]) + " && mva_score < " + str(bins[i+1])
            cut = cut + " && process_id == " + str(processID)
            h = TH1F("h_mgg_" + process + "_bin" + str(i+1), "", 160,100,180)
            t.Project(h.GetName(), "mass", "weight*(" + cut + ")")
            #numEvents = t.GetEntries("weight*(" + cut + ")")

            print "bin:", i+1, process, ": ", h.Integral()
#            process_ += " " + process.replace("_hgg","")
            process_ += " " + process.replace("_FCNC_","\_").replace("_hgg","")
            nSig = round(h.Integral(),2)
            if bugTree:
                nSig = nSig/1.527

            if nSig < 0: nSig = 0
#            if ("THQ" in process) or ("THW" in process):
#                nSig *= 140.1/(36.1+41.5)
            nEvt += " " + str(round(nSig,2))

        cut_data = "process_id == 10 && ((mass > 100 && mass < 120) || (mass > 130 && mass < 180))"
        cut_data = cut_data + " && mva_score > " + str(bins[i]) + " && mva_score < " + str(bins[i+1])
        h_data = TH1F("h_mgg_data_bin" + str(i+1), "", 160,100,180)
        t.Project(h_data.GetName(), "mass", "1*(" + cut_data + ")")

        w = RooWorkspace("w")
        w.factory("CMS_hgg_mass[100,180]")

        d_mgg_bin = RooDataHist("roohist_data_mass_" + str(i), "", RooArgList(w.var("CMS_hgg_mass")), h_data, 1)

        w.var("CMS_hgg_mass").setRange("SL", 100.0, 120.0)
        w.var("CMS_hgg_mass").setRange("SU", 130.0, 180.0)
        w.var("CMS_hgg_mass").setRange("full", 100, 180)
        w.var("CMS_hgg_mass").setRange("blind",120,130)
        w.factory("Exponential:bkgShape(CMS_hgg_mass, tau[-2,-10,0])")
        w.factory("ExtendPdf:bkgShape_ext(bkgShape, nevt[100,0,10000000], 'full')")
        w.pdf("bkgShape_ext").fitTo(d_mgg_bin, RooFit.Range("SL,SU"),RooFit.Extended(True))

        l = RooArgSet(w.var("CMS_hgg_mass"))
        frac = w.pdf("bkgShape_ext").createIntegral(l,l,"blind")
        print "data: ", frac.getVal()*h_data.Integral()

        process_ += " data"
        nData = round(frac.getVal()*h_data.Integral(), 2)
        nEvt += " " + str(nData)

        print process_
        print nEvt
#        import sys
#        sys.exit()

        nEvents.append(nEvt)
        processNames.append(process_) # this var needs to be optimized

    return nEvents,processNames


def WriteTex(nEvents, processNames, nProcess):
    print "\\begin{center}"
    line1 = "\\begin{tabular}{|c||c|"
    #line2 = ""
    for i in range(nProcess):
        line1 += "c|"
        #line2 += "& " + processNames[i]
    line1 += "} \hline"
    #line2 += "\\\\ \hline"
    print line1
    print processNames[0].replace(" ", " & ") + "\\\\ \hline \hline"
    lines = []
    for i in range(len(nEvents)):
        print nEvents[i].replace(" ", " & ") + "\\\\ \hline"

    print "\end{tabular} \end{center}"

#processes = ["ttH_hgg", "ggH_hgg", "VBF_hgg", "THQ_hgg", "THW_hgg"]

#ttHBaseCut = "signal_mass_label == 0 && mass > 120 && mass < 130"
FCNCBaseCut = "signal_mass_label == 0 && mass > 120 && mass < 130"

def WriteTex2(nEvents, processNames, nProcess, sigTag):

    print nEvents
    print processNames
    print nProcess
    print sigTag

    nBDTBins = len(nEvents)

    processes = processNames[0].split(' ')
    nBkgToT = [0]*nBDTBins
    for i in range(nBDTBins):
        # loop over nBDT bins
        nEventsPerBin = nEvents[i].split(' ')
        for j in range(1,len(processes)):
            # loop over n processes
            if sigTag not in processes[j]:
                nBkgToT[i] += float(nEventsPerBin[j])
        print nBkgToT[i]

    print "\\begin{center}"
    line1 = "\\begin{tabular}{|c||c|"
    #line2 = ""
    for i in range(nProcess):
        line1 += "c|"
        #line2 += "& " + processNames[i]
    line1 += "c|"
    line1 += "} \hline"
    #line2 += "\\\\ \hline"
    print line1
    processName = processNames[0].replace(" ", " & ")
    processName += " & total background"
    print  processName + "\\\\ \hline \hline"
    #lines = []

    for i in range(nBDTBins):
        nEvents_bin = "Bin" + str(i+1) + "&" #nEvents[i].replace(" ", " & ")
        nEventsPerBin = nEvents[i].split(' ')
        for j in range(1, len(processes)):
            if sigTag not in processes[j]:
                nEvents_bin += str( round(100*float(nEventsPerBin[j])/nBkgToT[i],2) ) + '\% & '
            else:
                nEvents_bin += str( nEventsPerBin[j] ) + ' & '
        nEvents_bin += str(nBkgToT[i])
        print  nEvents_bin + "\\\\ \hline"

    print "\end{tabular} \end{center}"


#processes = ["ttH_hgg", "ggH_hgg", "VBF_hgg", "THQ_hgg", "THW_hgg"]
#ttHBaseCut = "signal_mass_label == 0 && mass > 120 && mass < 130"
FCNCBaseCut = "signal_mass_label == 0 && mass > 120 && mass < 130"

#ttHHadBins = [0.9675,0.9937,0.9971,0.9991,1]
#ttHHadTree = "/home/users/sjmay/ttH/MVAs/ttHHadronic__v1.7_19Jun2019_RunII_MVA_Presel_impute_addDNNs_addTopTag_add_ttH_vs_tH_score_FinalFitTree.root"
#
#ttHLepBins = [0.8435,0.9346,0.9625,0.9890,1]
#ttHLepTree = "/home/users/sjmay/ttH/MVAs/ttHLeptonic__v1.7_19Jun2019_RunII_MVA_Presel_addDNN_add_ttH_vs_tH_score_FinalFitTree.root"

FCNCHUTLepTree="/home/users/sjmay/ttH/MVAs/ttHLeptonic_v1.7_19Jun2019_FCNC_SingleBDTBaseline_hut_FinalFitTree.root"
FCNCHUTHadTree="/home/users/sjmay/ttH/MVAs/ttHHadronic_v1.7_19Jun2019_FCNC_SingleBDTBaseline_impute_hut_FinalFitTree.root"
FCNCHCTLepTree="/home/users/sjmay/ttH/MVAs/ttHLeptonic_v1.7_19Jun2019_FCNC_SingleBDTBaseline_hct_FinalFitTree.root"
FCNCHCTHadTree="/home/users/sjmay/ttH/MVAs/ttHHadronic_v1.7_19Jun2019_FCNC_SingleBDTBaseline_impute_hct_FinalFitTree.root"

# fcnc hut had
FCNCHUTHadBins = [0.9457, 0.9616, 0.9750, 0.9907, 1]
# fcnc hct had
FCNCHCTHadBins = [0.9458, 0.9646, 0.9763, 0.9893, 1]
# fcnc lep hut
FCNCHUTLepBins = [0.72113216, 0.8145363, 0.86647624, 0.93589205, 1]
# fcnc lep hct
FCNCHCTLepBins = [0.5791701, 0.73848784, 0.8422407, 0.92392653, 1]


#FCNCHadBins = [0.95, 1]
#FCNCHadTree = "/home/users/sjmay/ttH/MVAs/ttHHadronic_v1.7_19Jun2019_FCNC_SingleBDTBaseline_impute_FinalFitTree.root"

#FCNCLepBins = [0.86, 1]
#FCNCLepTree = "/home/users/sjmay/ttH/MVAs/ttHLeptonic_v1.7_19Jun2019_FCNC_SingleBDTBaseline_FinalFitTree.root"

#nEvents_ttHHad,processNames_ttHHad = MakeTables(ttHHadTree, processes, ttHHadBins, ttHBaseCut)
#WriteTex(nEvents_ttHHad, processNames_ttHHad, len(processes) + 1)

#nEvents_ttHLep,processNames_ttHLep = MakeTables(ttHLepTree, processes, ttHLepBins, ttHBaseCut)
#WriteTex(nEvents_ttHLep, processNames_ttHLep, len(processes) + 1)

processes_hut = ["TT_FCNC_hut", "ST_FCNC_hut", "ttH_hgg", "ggH_hgg", "VBF_hgg", "THQ_hgg", "THW_hgg"]
processes_hct = ["TT_FCNC_hct", "ST_FCNC_hct", "ttH_hgg", "ggH_hgg", "VBF_hgg", "THQ_hgg", "THW_hgg"]

print "% FCNC HUT Had"
#nEvents_FCNCHUTHad,processNames_FCNCHUTHad = MakeTables(FCNCHUTHadTree, processes_hut, FCNCHUTHadBins, FCNCBaseCut)
#WriteTex(nEvents_FCNCHUTHad, processNames_FCNCHUTHad, len(processes_hut) + 1)
#WriteTex2(nEvents_FCNCHUTHad, processNames_FCNCHUTHad, len(processes_hut) + 1, "hut")

#print "% FCNC HCT Had"
#nEvents_FCNCHCTHad,processNames_FCNCHCTHad = MakeTables(FCNCHCTHadTree, processes_hct, FCNCHCTHadBins, FCNCBaseCut)
#WriteTex2(nEvents_FCNCHCTHad, processNames_FCNCHCTHad, len(processes_hct) + 1, "hct")
#
#print "% FCNC HUT Lep"
#nEvents_FCNCHUTLep,processNames_FCNCHUTLep = MakeTables(FCNCHUTLepTree, processes_hut, FCNCHUTLepBins, FCNCBaseCut)
#WriteTex2(nEvents_FCNCHUTLep, processNames_FCNCHUTLep, len(processes_hut) + 1, "hut")
#
#print "% FCNC HCT Lep"
nEvents_FCNCHCTLep,processNames_FCNCHCTLep = MakeTables(FCNCHCTLepTree, processes_hct, FCNCHCTLepBins, FCNCBaseCut)
WriteTex2(nEvents_FCNCHCTLep, processNames_FCNCHCTLep, len(processes_hct) + 1, "hct")



