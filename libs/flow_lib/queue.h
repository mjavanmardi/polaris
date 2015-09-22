#pragma once

#include <ctime>		//Include NULL
#include <iostream>		//Include rand()
#include <vector>
#include <map>
#include<sstream>
#include "car.h"

class Queue {
public:
	//### Constructors & Destructors ###
	Queue();
	Queue(int ID, double _maxLength, double _distanceBetweenCars, std::map<int,double> _capacities, std::map<int,double> _greenTime, std::map<int,double> _cycle, std::map<int,double> _offset);
	Queue(Json::Value value);
	~Queue();

	//### Getters ###
	int ID();
	double length();
	std::vector<Car>& getQueue();
	std::vector<Car>::iterator getCarsBegin();
	std::vector<Car>::iterator getCarsEnd();
	int getNumberOfCars();

	//### Dynamic methods ###
	int weight(int nextNode);
	void iterCarsProg();
	void addCar(Car C);
	void removeCar();
	void moveFakeCars(int timestep);
	std::map<int,double> getRealCapacity(int time, bool &isRed, double &min);

	//### Serialization ###

	Json::Value toJson();
	bool operator==(const Queue & q) const;

	
private:
	//### Constant parameters ###
	int queueID;
	double maxLength;
	double distanceBetweenCars;
	std::vector<int> nextNodes;

	//For one queue, there can be several turning movement
	//key = next node 
	//value = capacity for turning movement to this next node
	std::map<int, double> capacities; //static capactity 
	std::map<int,double> greenTime; //green time for each turining movement
	std::map<int,double> cycle; //traffic light cycle for each turining movement
	std::map<int,double> offset; //traffic light offset for each turining movement

	//### Cars in queue ###
	std::vector<Car> cars;

	//### Outputs ###
	std::vector<double> lengthOverTime;


};	 