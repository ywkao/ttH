#useNCores = 10
#n_quantiles = 100

#tag="TTHHadronicTag"
#tag="TTHLeptonicTag"
#tag="FCNCHadronicTag"
tag="FCNCHUTLeptonicTag"
#tag="FCNCHUTHadronicTag"
#sigName="ttH_hgg"
#for FCNC signal, only feed TT_FCNC, the doScan.py will take care of making signals for ST_FCNC and treat ST+TT as signals in datacard
sigName="TT_FCNC_hut"
#modelPath="TTFCNC_hut_had_20190623_step3_1"
modelPath="TTFCNC_hut_lep_20190626_step3_1"
#modelPath="TTHHadronic_20190624_step2_high"
plotPath="/home/users/hmei/public_html/2019/20190626_FCNCHUTLep_step3_1/"
highCut=0.86647624
lowCut=0.72113216
fixBin2="n"
useNCores="50"

mkdir -p ${plotPath}
rm ${plotPath}/*
cp ~/public_html/tmpFile/index.php ${plotPath}

rm models/${modelPath}/*txt

if [ ${fixBin2} = "y" ]; then
/bin/nice -n 10 python doScan.py --sigName ${sigName} --plotpath ${plotPath} --modelPath ${modelPath} --tag ${tag} --lowCut ${lowCut} --highCut ${highCut} --fixBin2 --useNCores ${useNCores}
else
/bin/nice -n 10 python doScan.py --sigName ${sigName} --plotpath ${plotPath} --modelPath ${modelPath} --tag ${tag} --lowCut ${lowCut} --highCut ${highCut} --useNCores ${useNCores}
fi

echo "tag: "${tag} > models/${modelPath}/command.log
echo "sigName: "${sigName} >> models/${modelPath}/command.log
echo "modelPath: "${modelPath} >> models/${modelPath}/command.log
echo "plotPath: "${plotPath} >> models/${modelPath}/command.log
echo "lowCut: "${lowCut} >> models/${modelPath}/command.log
echo "highCut: "${highCut} >> models/${modelPath}/command.log
echo "fixBin2: " ${fixBin2} >> models/${modelPath}/command.log

chmod -R 755 ~/public_html/

## get best sig according to scan boundary
# if in any of the two bdt bins the number of bkg events is smaller than 10, do not use it
echo "Scan results: " >> models/${modelPath}/result_scan.txt
bdtScoreIndex=""
if [[ ${sigName} = *ttH* ]]; then
    bdtScoreIndex=`awk '{if ($4 >= 10 && $5 >= 10) {print} }' models/${modelPath}/scan2Bin.txt | sort -nr -k3 | head -1 | awk -F " " '{print $1}'`
    bestSig=`awk '{if ($4 >= 10 && $5 >= 10) {print} }' models/${modelPath}/scan2Bin.txt | sort -nr -k3 | head -1`
    echo ${bestSig} >> models/${modelPath}/result_scan.txt
fi
if [[ ${sigName} = *FCNC*  ]]; then
    bdtScoreIndex=`awk '{if ($3 >= 10 && $4 >= 10) {print} }' models/${modelPath}/scan2Bin.txt | sort -n -k2 | head -1 | awk -F " " '{print $1}'`
    bestLimit=`awk '{if ($3 >= 10 && $4 >= 10) {print} }' models/${modelPath}/scan2Bin.txt | sort -n -k2 | head -1`
    echo ${bestLimit} >> models/${modelPath}/result_scan.txt

fi

echo "####" >> models/${modelPath}/result_scan.txt

# get the exact best cut on bdt score
echo "python doScan.py --sigName ${sigName} --plotpath ${plotPath} --modelPath ${modelPath} --tag ${tag} --lowCut ${lowCut} --highCut ${highCut} --noScan --scoreIndex ${bdtScoreIndex}" >> models/${modelPath}/result_scan.txt;
python doScan.py --sigName ${sigName} --plotpath ${plotPath} --modelPath ${modelPath} --tag ${tag} --lowCut ${lowCut} --highCut ${highCut} --noScan --scoreIndex ${bdtScoreIndex} >> models/${modelPath}/result_scan.txt

# save short version of summary
grep "Sig" models/${modelPath}/result_scan.txt > models/${modelPath}/result_summary.txt;
grep "python" models/${modelPath}/result_scan.txt >>  models/${modelPath}/result_summary.txt;
grep "bestCut" models/${modelPath}/result_scan.txt >> models/${modelPath}/result_summary.txt;
