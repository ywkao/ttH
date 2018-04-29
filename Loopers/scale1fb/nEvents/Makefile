ROOTLIBS = $(shell root-config --libs) -lMinuit -lMLP -lXMLIO -lTMVA -lGenVector

CC = g++
CFLAGS = -g -std=c++1y

TARGET = calc_neg_event_weights count_n_events 

all: $(TARGET)

calc_neg_event_weights: calc_neg_event_weights.cpp 
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o calc_neg_event_weights calc_neg_event_weights.cpp $(ROOTLIBS)

count_n_events: count_n_events.cpp
	$(CC) $(CFLAGS) -I${ROOTSYS}/include -o count_n_events count_n_events.cpp $(ROOTLIBS)
