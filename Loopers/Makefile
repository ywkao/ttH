ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y

TARGET = ttHHadronicLooper ttHLeptonicLooper

all: $(TARGET)

ttHHadronicLooper: ScanChain_ttHHadronic.C ttHHadronicLooper.cpp scale1fb/scale1fb.h ttHLooper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHHadronicLooper ttHHadronicLooper.cpp $(ROOTLIBS)

ttHLeptonicLooper: ScanChain_ttHLeptonic.C ttHLeptonicLooper.cpp scale1fb/scale1fb.h ttHLooper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o ttHLeptonicLooper ttHLeptonicLooper.cpp $(ROOTLIBS)

find_zombies: find_zombies.cpp
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o find_zombies find_zombies.cpp $(ROOTLIBS)
