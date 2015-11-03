#pragma once

#include "road.h"

void travelingAreaToQueues(std::map<int, Road>& Roads, int timestep);

std::pair<double,double> newCarDistanceAndSpeed(double carSpeed, double speedLimit, int timestep, double distanceLeftToTravel, double accMean, double deccMean);