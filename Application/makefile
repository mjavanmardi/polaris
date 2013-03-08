CC = g++
CFLAGS = -O2 -std=c++0x -pthread -D LINUX
LDFLAGS = -pthread -lstdc++ -lrt
HEADERS = ../Core/*.h ../User_Space/*.h ../Repository/*.h ./*.h
all: NetworkModel
clean: 
	rm -f NetworkModel
	rm -f NetworkModel.o
	rm -f ../Test_Data/Chicago_Network/NetworkModel
	rm -f ../Test_Data/Network_Model/NetworkModel
NetworkModel: NetworkModel.o 
	$(CC) -o NetworkModel $(LDFLAGS) NetworkModel.o -L../Scenario_Manager -ljson -L../File_IO -lfileio
	cp -f NetworkModel ../Test_Data/Chicago_Network
	cp -f NetworkModel ../Test_Data/Network_Model
NetworkModel.o: NetworkModel.cpp $(HEADERS)
	$(CC) -w -c $(CFLAGS) NetworkModel.cpp
