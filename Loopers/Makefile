ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y

TARGET = ttHHadronicLooper ttHHadronicMVABabyMaker 

all: $(TARGET)

ttHHadronicLooper: ScanChain_ttHHadronic.h ScanChain_ttHHadronic.C ttHHadronicLooper.cpp scale1fb/scale1fb_2016.h scale1fb/scale1fb_2017.h ttHLooper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHHadronicLooper ttHHadronicLooper.cpp $(ROOTLIBS)

ttHHadronicMVABabyMaker: ttHHadronicMVABabyMaker.cpp MakeMVABabies_ttHHadronic.C MakeMVABabies_ttHHadronic.h scale1fb/scale1fb_2016.h scale1fb/scale1fb_2017.h ttHLooper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHHadronicMVABabyMaker ttHHadronicMVABabyMaker.cpp $(ROOTLIBS)

#tprimetHHadronicLooper_signal: src/tprimetHHadronicLooper.cpp src/ScanChain_tprimetHHadronic.C include/ScanChain.h include/chi2_helper.h include/truth_matching.h include/sorting.h
#	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHHadronicMVABabyMaker ttHHadronicMVABabyMaker.cpp $(ROOTLIBS)
