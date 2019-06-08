#subTag1="TTH"
subTag1="FCNC"
#subTag2="Hadronic"
subTag2="Leptonic"
modelPath="FCNC_lep_v1"
#binBounds=(0.918675 1)
binBounds=(0.84568137 1)
savepath="/home/users/hmei/public_html/2019/20190605_FCNC_lep/"
mkdir -p ${savepath}
rm ${savepath}/*
cp ~/public_html/index.php ${savepath}

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

    tags=${tag}" "${subTag1}${subTag2}Tag_v${i}
    python makeSigBkgShape.py -l ${lowCut} --hi ${highCut} --tag ${subTag1}${subTag2}Tag_v${i} --modelPath ${modelPath} --savepath ${savepath} --doData -p ttH_hgg  #--skipBkg &

    for sig in ggH_hgg VBF_hgg TT_FCNC_hut TT_FCNC_hct ST_FCNC_hut ST_FCNC_hct
     do

	python makeSigBkgShape.py -l ${lowCut} --hi ${highCut} --tag ${subTag1}${subTag2}Tag_v${i} --modelPath ${modelPath} --savepath ${savepath} --doData -p ${sig} --skipBkg &
	
    done
    
done

python cardMaker.py --postFix "" --savepath "models/"${modelPath} --tags ${tags} --doFCNC --doMultiSig 

chmod -R 755 ~/public_html/    

