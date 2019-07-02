#subTag1="TTH"
subTag1="FCNCHCT"
sigName="TT_FCNC_hct"
subTag2="Hadronic"
#subTag2="Leptonic"
#modelPath="TTH_lep_2017bdt_20190627_v2"
modelPath="FCNC_hct_had_final_20190702_v1"
savepath="/home/users/hmei/public_html/2019/20190702_FCNC_hct_had_final_v1/"

# fcnc hut had
#binBounds=(0.9457 0.9616 0.9750 0.9907 1)
# fcnc hct had
binBounds=(0.9458 0.9646 0.9763 0.9893 1)
# fcnc lep hut
#binBounds=(0.72113216 0.8145363 0.86647624 0.93589205 1)
#binBounds=(0.86647624 0.93589205 1)
# fcnc lep hct
#binBounds=(0.5791701 0.73848784 0.8422407 0.92392653 1)

# 2017 bdt at Run2 bdt sig eff
# lep
#binBounds=(0.3075364 0.4747876 0.5746820 0.7840243 1)
#binBounds=(0.4 0.6 1)
# had
#binBounds=(0.340765 0.4555961 0.507464 0.584346 1)
#binBounds=(0.38 0.48 0.56 1)

mkdir -p ${savepath}
rm ${savepath}/*
cp ~/public_html/tmpFile/index.php ${savepath}

mkdir -p models/${modelPath}
rm models/${modelPath}/*

#python makeSigBkgShape.py -l 0.9 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath  "/home/users/hmei/public_html/2019/20190605_testFCNC/" --doData -p TT_FCNC_hut &
#python makeSigBkgShape.py -l 0.9 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath  "/home/users/hmei/public_html/2019/20190605_testFCNC/" --doData -p TT_FCNC_hct &
#python makeSigBkgShape.py -l 0.998 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath ${savepath} --doData -p ttH_hgg & #--skipBkg &
#python makeSigBkgShape.py -l 0.998 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath ${savepath} --doData -p ggH_hgg --skipBkg &
#python makeSigBkgShape.py -l 0.998 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath ${savepath} --doData -p VBF_hgg --skipBkg &
#python makeSigBkgShape.py -l 0.9 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath  "/home/users/hmei/public_html/2019/20190605_testFCNC/" --doData -p VH_hgg  --skipBkg &

tags=""
nBins=${#binBounds[@]}
echo $nBins
nBins=`expr $nBins - 1`
for (( i = 0; i < nBins; i++))
do
    lowCut=${binBounds[i]}
    highCut=${binBounds[i+1]}

    echo $i
    echo ${tags}
    tags=${tags}" "${subTag1}${subTag2}Tag_v${i}
    python makeSigBkgShape.py -l ${lowCut} --hi ${highCut} --tag ${subTag1}${subTag2}Tag_v${i} --modelPath ${modelPath} --savepath ${savepath} --doData -p ttH_hgg  #--skipBkg &

    #for sig in ggH_hgg VBF_hgg THQ_hgg THW_hgg TT_FCNC_hut ST_FCNC_hut
    for sig in ggH_hgg VBF_hgg THQ_hgg THW_hgg TT_FCNC_hct ST_FCNC_hct
    do

        python makeSigBkgShape.py -l ${lowCut} --hi ${highCut} --tag ${subTag1}${subTag2}Tag_v${i} --modelPath ${modelPath} --savepath ${savepath} --doData -p ${sig} --skipBkg &
#
    done

done

echo ${tags}

if [[ ${subTag1} = *FCNC* ]]; then
    python cardMaker.py --postFix "" --savepath "models/"${modelPath} --tags ${tags} --doFCNC --doMultiSig --FCNCSig ${sigName}
    echo "python cardMaker.py --postFix \"\" --savepath \"models/\"${modelPath} --tags ${tags} --doFCNC --doMultiSig --FCNCSig ${sigName}"

fi
if [[ ${subTag1} = *TTH* ]]; then
    python cardMaker.py --postFix "" --savepath "models/"${modelPath} --tags ${tags}
fi
chmod -R 755 ~/public_html/

# TTHLeptonicTag_v0 TTHLeptonicTag_v1 TTHLeptonicTag_v2 TTHHadronicTag_v0 TTHHadronicTag_v1 TTHHadronicTag_v2  TTHHadronicTag_v3
