subTag1="TTH"
#subTag1="FCNC"
subTag2="Hadronic"
#subTag2="Leptonic"
modelPath="testFCNC_v2"
binBounds=(0.95 0.99 0.996 0.99804 1)
savepath="/home/users/hmei/public_html/2019/20190605_testFCNC/"

#python makeSigBkgShape.py -l 0.9 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath  "/home/users/hmei/public_html/2019/20190605_testFCNC/" --doData -p TT_FCNC_hut &
#python makeSigBkgShape.py -l 0.9 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath  "/home/users/hmei/public_html/2019/20190605_testFCNC/" --doData -p TT_FCNC_hct &
#python makeSigBkgShape.py -l 0.998 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath ${savepath} --doData -p ttH_hgg & #--skipBkg &
#python makeSigBkgShape.py -l 0.998 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath ${savepath} --doData -p ggH_hgg --skipBkg &
#python makeSigBkgShape.py -l 0.998 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath ${savepath} --doData -p VBF_hgg --skipBkg &
#python makeSigBkgShape.py -l 0.9 --hi 1 -i 0 --tag ${subTag1}${subTag2}Tag --modelPath ${modelPath} --savepath  "/home/users/hmei/public_html/2019/20190605_testFCNC/" --doData -p VH_hgg  --skipBkg &

nBins=${#binBounds[@]}
echo $nBins
nBins=`expr $nBins - 1`
for (( i = 0; i < nBins; i++))
do
    lowCut=${binBounds[i]}
    highCut=${binBounds[i+1]}

    python makeSigBkgShape.py -l ${lowCut} --hi ${highCut} --tag ${subTag1}${subTag2}Tag_v${i} --modelPath ${modelPath} --savepath ${savepath} --doData -p ttH_hgg  #--skipBkg &

    for sig in ggH_hgg VBF_hgg
    do

	python makeSigBkgShape.py -l ${lowCut} --hi ${highCut} --tag ${subTag1}${subTag2}Tag_v${i} --modelPath ${modelPath} --savepath ${savepath} --doData -p ${sig} --skipBkg #&
	
    done
    
done
