CC = g++
CFLAGS = -g -std=c++0x -pthread 
LDFLAGS = -g -pthread -lstdc++
HEADERS = ../Core/*.h ../User_Space/*.h ../Repository/*.h
all: Application
clean: 
	rm Application
	rm Application.o
Application: Application.o 
	$(CC) -o Application $(LDFLAGS) Application.o -L../Scenario_Manager -ljson
Application.o: Application.cpp $(HEADERS)
	$(CC) -w -c $(CFLAGS) Application.cpp
