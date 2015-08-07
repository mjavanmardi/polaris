#pragma once

#include "road.h"

void TAToQueues(Road& R, int timestep);

void travelingAreaToQueues(std::map<int, Road>& Roads, int timestep);

double newCarSpeed(double carSpeed, double speedLimit, int timestep, double& length, double distanceLeftToTravel, double accMean, double deccMean);