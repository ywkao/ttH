ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y

TARGET = makePlots templatePlots makeTables 

all: $(TARGET)

makePlots: makePlots.cpp PlotHelper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o makePlots makePlots.cpp $(ROOTLIBS)

templatePlots: templatePlots.cpp PlotHelper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o templatePlots templatePlots.cpp $(ROOTLIBS)

makeTables: makeTables.cpp PlotHelper.h
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o makeTables makeTables.cpp $(ROOTLIBS)
