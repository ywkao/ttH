#subTag1="FCNCHCT"
#sigName="TT_FCNC_hct"

subTag1="TTH"

#subTag2="Hadronic"
#pathTag="TTH_had_preapproval"

subTag2="Leptonic"
pathTag="TTH_lep_preapproval"

date="20191015"
version="v1"
modelPath=${pathTag}"_"${date}"_"${version}
savepath="/home/users/hmei/public_html/2019/"${date}"_"${pathTag}"_"${version}"/"

# tth preapproval
# lep
binBounds_a="0.8997816,0.95635754,0.9725133,0.9870608,1"

#had
#binBounds_a="0.98602563,0.9948537,0.9983046,0.9990729,1"
#binBounds_a="0.9786375,0.9948537,0.99839586,0.9990729,1"
#binBounds_a="0.9786375,0.9908256,0.9948537,0.99839586,0.9990729,1"

echo $binbounds_a
IFS=', ' read -r -a binBounds <<< "$binBounds_a"

mkdir -p ${savepath}
rm ${savepath}/*
cp ~/public_html/backup/index.php ${savepath}

mkdir -p models/${modelPath}
rm models/${modelPath}/*

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
    python makeSigBkgShape.py -l ${lowCut} --hi ${highCut} --tag ${subTag1}${subTag2}Tag_v${i} --modelPath ${modelPath} --savepath ${savepath} --doData -p ttH_hgg & #--skipBkg &

    #for sig in ggH_hgg VBF_hgg THQ_hgg THW_hgg TT_FCNC_hut ST_FCNC_hut
    #for sig in ggH_hgg VBF_hgg THQ_hgg THW_hgg TT_FCNC_hct ST_FCNC_hct
    for sig in ggH_hgg
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
chmod -R 755 ${savepath}
# TTHLeptonicTag_v0 TTHLeptonicTag_v1 TTHLeptonicTag_v2 TTHHadronicTag_v0 TTHHadronicTag_v1 TTHHadronicTag_v2  TTHHadronicTag_v3
