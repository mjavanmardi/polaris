#pragma once

#include <fstream>

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
	void writeOutput(double time, std::ofstream& outputFlow);
private:
	std::vector<Car*> cars;
};