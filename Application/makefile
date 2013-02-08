CC = g++
CFLAGS = -g -std=c++0x -pthread -D LINUX
LDFLAGS = -g -pthread -lstdc++ -lrt
HEADERS = ../Core/*.h ../User_Space/*.h ../Repository/*.h ./*.h
all: NetworkModel
clean: 
	rm NetworkModel
	rm NetworkModel.o
NetworkModel: NetworkModel.o 
	$(CC) -o NetworkModel $(LDFLAGS) NetworkModel.o -L../Scenario_Manager -ljson -L../File_IO -lfileio
NetworkModel.o: NetworkModel.cpp $(HEADERS)
	$(CC) -w -c $(CFLAGS) NetworkModel.cpp
