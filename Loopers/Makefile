ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y

TARGET = ttHHadronicLooper ttHLeptonicLooper ttHHadronicMVABabyMaker ttHLeptonicMVABabyMaker 

all: $(TARGET)

ttHHadronicLooper: ScanChain_ttHHadronic.h ScanChain_ttHHadronic.C ttHHadronicLooper.cpp scale1fb/scale1fb.h ttHLooper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHHadronicLooper ttHHadronicLooper.cpp $(ROOTLIBS)

ttHLeptonicLooper: ScanChain_ttHLeptonic.h ScanChain_ttHLeptonic.C ttHLeptonicLooper.cpp scale1fb/scale1fb.h ttHLooper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHLeptonicLooper ttHLeptonicLooper.cpp $(ROOTLIBS)

ttHHadronicMVABabyMaker: ttHHadronicMVABabyMaker.cpp MakeMVABabies_ttHHadronic.C MakeMVABabies_ttHHadronic.h scale1fb/scale1fb.h ttHLooper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHHadronicMVABabyMaker ttHHadronicMVABabyMaker.cpp $(ROOTLIBS)

ttHLeptonicMVABabyMaker: ttHLeptonicMVABabyMaker.cpp MakeMVABabies_ttHLeptonic.C MakeMVABabies_ttHLeptonic.h scale1fb/scale1fb.h ttHLooper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHLeptonicMVABabyMaker ttHLeptonicMVABabyMaker.cpp $(ROOTLIBS)
