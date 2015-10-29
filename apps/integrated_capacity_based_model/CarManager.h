#pragma once

#include "Car.h"
#include "RoadNetwork.h"

class CarManager
{
public:
	CarManager();
	~CarManager();

	void addCarsToRoadNetWork(RoadNetwork* rn);

	std::vector<Car*>* getCars();

	void speak();
private:
	std::vector<Car*> cars;
};