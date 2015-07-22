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
	Queue(int ID, float _maxLength, float _distanceBetweenCars, std::map<int,float> _capacities);
	~Queue();

	//### Getters ###
	int ID();
	float length();
	std::vector<Car> getQueue();
	std::map<int, float> getCapacities();

	//### Setters ###
	
	//### Dynamic methods ###
	int weight(int nextNode);
	void iterCarsProg();
	void addCar(Car C);
	void removeCar();

	void moveFakeCars(int timestep);


private:
	//### Constant parameters ###
	int queueID;
	float maxLength;
	float distanceBetweenCars;
	std::vector<int> nextNodes;
	std::map<int, float> capacities;		//int of next node && capacity value to this next node

	//### Cars in queue ###
	std::vector<Car> cars;

	//### Outputs ###
	std::vector<float> lengthOverTime;


};	 