#pragma once 


#include "queue.h"
#include "road.h"
#include "car.h"

int lastCarProba(double proba, double weight);

int numberOfAllowedCars(std::map<int, double>& capac, std::vector<Car> queue, int timestep);

std::vector<int> movingCars(bool& q, int nodeA, int nodeB, int queueID, std::map<int, double> capac, std::vector<Car> queue, int timestep);

std::vector<std::vector<int>> preProcess(std::map<int, Road>& Roads, int timestep);