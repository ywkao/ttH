#subTag1="TTH"
subTag1="FCNCHCT"
sigName="TT_FCNC_hct"
subTag2="Hadronic"
#subTag2="Leptonic"
#modelPath="TTH_had_final_20190612_v2"
modelPath="FCNC_hct_had_final_20190624_v1"
savepath="/home/users/hmei/public_html/2019/20190624_FCNC_hct_had_final_v1/"
#binBounds=(0.38 0.48 0.56 1)
#binBounds=(0.4 0.6 1)

#binBounds=(0.8434914 0.9345798 0.96248275 0.9898756 1) #
#binBounds=(0.9858139 0.99374276 0.9971033 0.999148 1)

#lep
#binBounds=(0.8434914 0.9345798 0.96248275 0.989 1) #0.9898756 1) #
#had
#binBounds=(0.9858139 0.99374276 0.9971033 0.99905 1) #0.999148 1)
#binBounds=(0.9675 0.99374276 0.9971033 0.99905 1) #0.999148 1)
# fcnc hut had
#binBounds=(0.9457 0.9616 0.9750 0.9907 1)
# fcnc hct had
binBounds=(0.9458 0.9646 0.9763 0.9893 1)

#binBounds=(0.8852191 0.93 0.96613055 0.98956394 1)
#binBounds=(0.9675139 0.98843944 0.9955651 0.9975579 1)
## oldBins
#binBounds=(0.93 0.96945995 0.9869719 1)
#binBounds=(0.95 0.99 0.996 0.99804 1)
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

    #for sig in ggH_hgg TT_FCNC_hut ST_FCNC_hut
    for sig in ggH_hgg TT_FCNC_hct ST_FCNC_hct
    do

        python makeSigBkgShape.py -l ${lowCut} --hi ${highCut} --tag ${subTag1}${subTag2}Tag_v${i} --modelPath ${modelPath} --savepath ${savepath} --doData -p ${sig} --skipBkg &
#
    done

done

echo ${tags}

if [[ ${subTag1} = "FCNC" ]]; then
    python cardMaker.py --postFix "" --savepath "models/"${modelPath} --tags ${tags} --doFCNC --doMultiSig --FCNCSig ${sigName}
fi
if [[ ${subTag1} = "ttH" ]]; then
    python cardMaker.py --postFix "" --savepath "models/"${modelPath} --tags ${tags}
fi
chmod -R 755 ~/public_html/

# TTHLeptonicTag_v0 TTHLeptonicTag_v1 TTHLeptonicTag_v2 TTHHadronicTag_v0 TTHHadronicTag_v1 TTHHadronicTag_v2  TTHHadronicTag_v3
