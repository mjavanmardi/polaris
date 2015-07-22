#pragma once

#include "road.h"


int selectEnteringRoad(std::map<int, Road>& Roads, int enterNode);

void addNewCars(std::vector<Car>& newCars, std::map<int, Road>& Roads, int t, int timestep);