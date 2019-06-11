subTag1="TTH"
#subTag1="FCNC"
subTag2="Hadronic"
#subTag2="Leptonic"
modelPath="TTH_had_20190610_v2"
#binBounds=(0.8852191 0.96613055 0.98956394 1)
binBounds=(0.9675139 0.98843944 0.9955651 0.9975579 1)
savepath="/home/users/hmei/public_html/2019/20190610_TTH_had_v2/"
mkdir -p ${savepath}
rm ${savepath}/*
cp ~/public_html/tmpFile/index.php ${savepath}

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

#    for sig in ggH_hgg #VBF_hgg TT_FCNC_hut TT_FCNC_hct ST_FCNC_hut ST_FCNC_hct
#     do
#
#	python makeSigBkgShape.py -l ${lowCut} --hi ${highCut} --tag ${subTag1}${subTag2}Tag_v${i} --modelPath ${modelPath} --savepath ${savepath} --doData -p ${sig} --skipBkg &
##	
#    done
#    
done

echo ${tags}
#python cardMaker.py --postFix "" --savepath "models/"${modelPath} --tags ${tags} --doFCNC --doMultiSig 
python cardMaker.py --postFix "" --savepath "models/"${modelPath} --tags ${tags}  
# need add mkdir for modelPath if not created

chmod -R 755 ~/public_html/    

