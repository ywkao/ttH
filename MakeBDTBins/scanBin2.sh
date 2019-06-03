start=$1

for ((i=1;i<=10;i++));
do

    echo $i
    index=`expr $i + $start`
    echo $index
    
    cp models/testStep2/CMS-HGG_sigfit_mva_TTH_TTHLeptonicTag_0_v${index}.root /home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/flashggFinalFit/Plots/FinalResults/test1D_2bin/models/
    cp models/testStep2/CMS-HGG_bkg_TTH_TTHLeptonicTag_0_v${index}.root /home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/flashggFinalFit/Plots/FinalResults/test1D_2bin/models/

    cp models/testStep2/CMS-HGG_sigfit_mva_TTH_TTHLeptonicTag_1_v${index}.root /home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/flashggFinalFit/Plots/FinalResults/test1D_2bin/models/
    cp models/testStep2/CMS-HGG_bkg_TTH_TTHLeptonicTag_1_v${index}.root /home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/flashggFinalFit/Plots/FinalResults/test1D_2bin/models/
      
    cd /home/users/hmei/flashggFinalFit/CMSSW_7_4_7/src/flashggFinalFit/Plots/FinalResults/test1D_2bin/models/

    cp ../CMS-HGG_mva_13TeV_datacard.txt CMS-HGG_mva_13TeV_datacard_${index}.txt
    sed -i "s/_0.root/_0_v"${index}".root/g" CMS-HGG_mva_13TeV_datacard_${index}.txt
    sed -i "s/_1.root/_1_v"${index}".root/g" CMS-HGG_mva_13TeV_datacard_${index}.txt
   
    pwd
    cmsenv
    combine -M ProfileLikelihood --significance CMS-HGG_mva_13TeV_datacard_${index}.txt -t -1 --expectSignal=1 > sig_${index}.txt;
    sig=`grep Significance sig_${index}.txt`
    echo ${index}" "${sig} >> /home/users/hmei/ttH_new/ttH/MakeBDTBins/2bin_.txt
    echo $sig
    cd -
    
done
