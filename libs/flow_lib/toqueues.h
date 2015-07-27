#pragma once

#include "road.h"

void TAToQueues(Road& R, int timestep);

void travelingAreaToQueues(std::map<int, Road>& Roads, int timestep);

int timeToAdd(double speedMax, double accMax, double distanceInTA);
