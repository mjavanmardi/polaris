#pragma once 


#include "queue.h"
#include "road.h"
#include "car.h"

int lastCarProba(double proba, double weight);

int numberOfAllowedCars(Queue Q, int timestep);

void movingCars(std::vector<std::vector<int>>& cars, Road R, int timestep);

std::vector<std::vector<int>> preProcess(std::map<int, Road>& Roads, int timestep);