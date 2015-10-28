#pragma once

#include "Car.h"
#include "RoadNetwork.h"

class CarManager
{
public:
	CarManager();
	~CarManager();

	void addCarsToRoadNetWork(RoadNetwork* rn);
private:
	std::vector<Car*> cars;
};