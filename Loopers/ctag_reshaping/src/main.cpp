#include <stdio.h>
#include <ctag_reshaping.h>
void test_run();

int main()
{
    printf("Hello World!\n");
    test_run();
    return 0;
}

void test_run()
{
    std::string file = "/wk_cms2/ykao/CMSSW_9_4_10/src/ttH/Loopers/ctag_reshaping/sfs_rootfiles/DeepJet_ctagSF_MiniAOD94X_2017_pTincl.root";
    retrieve_scale_factor sf(file);
    TRandom3 random(1234);

    TString type_flavour[3] = {"b", "c", "l"}; 
    TString type_uncertainty[14] = {"", "_Stat", "_EleIDSF", "_LHEScaleWeight_muF", "_LHEScaleWeight_muR", "_MuIDSF", "_PSWeightFSR", "_PSWeightISR", "_PUWeight", "_XSec_DYJets", "_XSec_ST", "_XSec_WJets", "_XSec_ttbar", "_jer"};

    double cvsl = random.Rndm();
    double cvsb = random.Rndm();

    sf.debug_mode();
    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<14; ++j)
        {
            if(!(i==0 and j==8)) continue; //check "_PUWeight" only
            TString name = "SF" + type_flavour[i] + "_hist" + type_uncertainty[j];
            double value = sf.get_scale_factor(name + "Up", cvsl, cvsb);
            printf("cvsl = %.2f, cvsb = %.2f, value = %.2f\n", cvsl, cvsb, value);
        }
    }
    std::cout << "This is the end of test run" << std::endl;
}
