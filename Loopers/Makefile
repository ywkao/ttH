ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y

TARGET = ttHHadronicLooper ttHLeptonicLooper 

all: $(TARGET)

ttHHadronicLooper: ScanChain_ttHHadronic.C ttHHadronicLooper.cpp
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHHadronicLooper ttHHadronicLooper.cpp $(ROOTLIBS)

ttHLeptonicLooper: ScanChain_ttHLeptonic.C ttHLeptonicLooper.cpp
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHLeptonicLooper ttHLeptonicLooper.cpp $(ROOTLIBS)
