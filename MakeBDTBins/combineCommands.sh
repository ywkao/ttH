# significance
combine -M ProfileLikelihood --significance CMS-HGG_mva_13TeV_datacard.txt -t -1 --expectSignal=1

# exclusion limit
combine -M Asymptotic -m 125 -n test CMS-HGG_mva_13TeV_datacard.txt  -t -1 --expectSignal=1
