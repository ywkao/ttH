for ((i=0;i<100;i++));
do
    sig=`grep Significance sig_${i}.txt`
    nbkg0=`sed -n '1p' CMS-HGG_bkg_TTHLeptonicTag_0_${i}_nbkg.txt`
    nbkg1=`sed -n '1p' CMS-HGG_bkg_TTHLeptonicTag_1_${i}_nbkg.txt`
    mva=`grep mva mvaScore_${i}.txt`
    echo $sig" "${nbkg0}" "${nbkg1}" "${mva} >> final.txt
done
sort -n -k2 final.txt > final_sort.txt
grep Sig sig_100.txt >> final_sort.txt
tail final_sort.txt
#rm sig_*txt
#rm CMS-HGG_bkg_TTHLeptonicTag_*_nbkg.txt
#rm mvaScore*txt
