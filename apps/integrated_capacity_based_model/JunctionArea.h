#pragma once

#include "IndividualQueue.h"

class IndividualQueue;

class JunctionArea : private std::vector<std::vector<IndividualQueue>>
{
public:
	JunctionArea(std::vector<std::vector<IndividualQueue>> indivQueues);

	//### Dynamic methods ###

	double getTotalLengthLeft(int i, int j); //Get the total length left in a given individual queue 
	bool isQueueEmpty(int i, int j); //Check if a given individual queue is empty
	void insertCar(Car* car, std::pair<int,int> queue);

	//Return true if there is a path free of cars that leads to nextRoad from individualQueue (initI,initJ)
	//if false, second variable of the pair is the free flow distance
	std::pair<bool,double> isPathFree(int nextRoad, int initI, int initJ); 
	std::pair<int,int> JunctionArea::selectNextQueue(std::pair<int,int> currentPosition,int nextRoad);
	bool moveCars();

	//### Static methods ###
	double getQueueLength(int i, int j); //Get the length of a given individual queue
	void speak();
	double getLength() const;
	void updateWeightAndLane(double& currentWeight,int& currentLane,int newLane,int column, int nextRoad);
private:
	double length;
	const int nbLanes;
	const int nbColumns;
};