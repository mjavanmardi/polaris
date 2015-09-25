#pragma once

#include "road.h"
#include "MovingCars.h"
#include <tuple>
#include <algorithm>

struct ComingCar
{
	int carID;
	int formerRoadID;
	int formerQueueID;
};

std::map<int,std::vector<ComingCar>> sortCars(std::vector<MovingCars> cars, const std::vector<std::vector<int>> &nodesToID);

bool tryToMoveFirstCar(std::map<int,ComingCar>::iterator roadIterator, std::map<int, Road>& Roads, int timeStep);

void queuesToTravelingAreas(std::map<int, Road>& Roads, std::vector<MovingCars> cars, int timestep, std::vector<std::vector<int>> &nodesToID);
