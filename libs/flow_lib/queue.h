#pragma once

#include <ctime>		//Include NULL
#include <iostream>		//Include rand()
#include <vector>
#include <map>
#include "car.h"

class Queue {
public:
	//### Constructors & Destructors ###
	Queue();
	Queue(int ID, double _maxLength, double _distanceBetweenCars, std::map<int,double> _capacities);
	~Queue();

	//### Getters ###
	int ID();
	double length();
	std::vector<Car> getQueue();
	std::map<int, double> getCapacities();

	//### Dynamic methods ###
	int weight(int nextNode);
	void iterCarsProg();
	void addCar(Car C);
	void removeCar();
	void moveFakeCars(int timestep);

	
private:
	//### Constant parameters ###
	int queueID;
	double maxLength;
	double distanceBetweenCars;
	std::vector<int> nextNodes;
	std::map<int, double> capacities;		//int of next node && capacity value to this next node

	//### Cars in queue ###
	std::vector<Car> cars;

	//### Outputs ###
	std::vector<double> lengthOverTime;


};	 