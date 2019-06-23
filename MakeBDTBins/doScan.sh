#tag = "FCNCHadronicTag"
#tag = "ttHLeptonic"
#sigName = "ttH_hgg"
#sigName = "TT_FCNC_hut"
#modelPath = "TT_FCNC_hut_scan_lep"
#modelPath = "TTHLep_Run2_step1"
#plotPath = "/home/users/hmei/public_html/2019/20190605_scan_ttHLepRun2_step1/"
#lowCut = 0
#highCut = 1
#fixBin2 = "n"
#useNCores = 10
#n_quantiles = 100

#tag="TTHHadronicTag"
#tag="TTHLeptonicTag"
#tag="FCNCHadronicTag"
tag="FCNCHUTLeptonicTag"
#sigName="ttH_hgg"
#for FCNC signal, only feed TT_FCNC, the doScan.py will take care of making signals for ST_FCNC and treat ST+TT as signals in datacard
sigName="TT_FCNC_hut"
modelPath="TTFCNC_hut_lep_20190623_step2_low"
plotPath="/home/users/hmei/public_html/2019/20190621_TTFCNC_hut_lep_20190623_step2_low/"
lowCut=0
highCut=0.86647624
fixBin2="y"

mkdir -p ${plotPath}
rm ${plotPath}/*
cp ~/public_html/tmpFile/index.php ${plotPath}

rm models/${modelPath}/*txt

if [ ${fixBin2} = "y" ]; then
python doScan.py --sigName ${sigName} --plotpath ${plotPath} --modelPath ${modelPath} --tag ${tag} --lowCut ${lowCut} --highCut ${highCut} --fixBin2
else
python doScan.py --sigName ${sigName} --plotpath ${plotPath} --modelPath ${modelPath} --tag ${tag} --lowCut ${lowCut} --highCut ${highCut}
fi

echo "tag: "${tag} > models/${modelPath}/command.log
echo "sigName: "${sigName} >> models/${modelPath}/command.log
echo "modelPath: "${modelPath} >> models/${modelPath}/command.log
echo "plotPath: "${plotPath} >> models/${modelPath}/command.log
echo "lowCut: "${lowCut} >> models/${modelPath}/command.log
echo "highCut: "${highCut} >> models/${modelPath}/command.log
echo "fixBin2: " ${fixBin2} >> models/${modelPath}/command.log

chmod -R 755 ~/public_html/

#bdtScoreIndex=`sort -nr -k 3 models/${modelPath}/scan2Bin.txt | head -1 | awk -F " " '{print $1}'`

# if in any of the two bdt bins the number of bkg events is smaller than 10, do not use it
bdtScoreIndex=`awk '{if ($4 >= 10 && $5 >= 10) {print} }' scan2Bin.txt | sort -n -k3 | head -1 | awk -F " " '{print $1}'`
if [[ ${sigName} = *FCNC*  ]]; then
    bdtScoreIndex=`awk '{if ($3 >= 10 && $4 >= 10) {print} }' scan2Bin.txt | sort -nr -k2 | head -1 | awk -F " " '{print $1}'`
fi

echo "python doScan.py --sigName ${sigName} --plotpath ${plotPath} --modelPath ${modelPath} --tag ${tag} --lowCut ${lowCut} --highCut ${highCut} --noScan --scoreIndex ${bdtScoreIndex}" >> models/${modelPath}/result_scan.txt
python doScan.py --sigName ${sigName} --plotpath ${plotPath} --modelPath ${modelPath} --tag ${tag} --lowCut ${lowCut} --highCut ${highCut} --noScan --scoreIndex ${bdtScoreIndex} >> models/${modelPath}/result_scan.txt


