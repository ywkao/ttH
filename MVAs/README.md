1, train multiBDT from Baby:
python prep.py -f $PATHTOBaby -o $ROOTFILENAME
* rootfile will be saved at "rootfiles" dir

2, get maxZa as function of BDT cut:
python getZa.py -f rootfiles/$ROOTFILENAME.root -o TXTFILENAME
* txtfile will be saved at "txtfiles" dir

3, do ranking of variables:
./doRanking.sh
* then use "txtfiles/hadronicBDT_rankingVar_v0/getSortedMaxZa.sh" to get sorted max_mc_za