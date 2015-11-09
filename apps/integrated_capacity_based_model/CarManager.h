#pragma once

#include <fstream>

#include "Car.h"
#include "RoadNetwork.h"

//This class holds all the cars in the network.
//No car should be created outside of this class
class CarManager
{
public:
	CarManager();
	~CarManager();

	void addCarsToRoadNetWork(RoadNetwork* rn);

	std::vector<Car*>* getCars();

	void speak();
	void writeOutput(double time, std::vector< std::vector <double > >& outputVect);
private:
	std::vector<Car*> cars;
};