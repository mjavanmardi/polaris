#pragma once

#include "road.h"


void printQueue(std::vector<Car> cars);

void printIndivQueues(std::map<int, Queue>& indivQueues);

void printLastQueue(std::vector<Car>& lastQueue);

void printCommonQueue(std::vector<Car>& commonQueue);

void printTravelingArea(std::vector<Car>& TA);

void printRoad(Road R);

void printRoads(int t, std::map<int, Road> Roads, std::vector<int> timestepsToPrint, bool q, int superior);