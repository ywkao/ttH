ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y

TARGET = check_eventRunLumi 

all: $(TARGET)

check_eventRunLumi: check_eventRunLumi.cpp
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o check_eventRunLumi check_eventRunLumi.cpp $(ROOTLIBS)
