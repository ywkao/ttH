#!/usr/bin/env python

import sys
import ROOT
fin=ROOT.TFile.Open(sys.argv[1])
pu = fin.Get("pileup")
## pileup = map( pu.GetBinContent, xrange(1,pu.GetNbinsX()+1) )
pileup = [pu.GetBinContent(x) / pu.Integral() for x in range(1,pu.GetNbinsX()+1)]
print(",".join(["%1.4g"%x for x in pileup]))

