#pragma once

#include <vector>

// Contains the informations needed to move cars in a given queue
// Cars are only identified by the node they are heading at 
class MovingCars
{
public:
	MovingCars();
	MovingCars(int _nodeA, int _nodeB, int _queueID, int _roadID, std::vector<int> &_movingCarsIds);
	int getNodeA() const; //Road's nodeA
	int getNodeB() const; // Road's nodeB
	int getQueueID() const; // Current Queue
	int getRoadID() const; //Road where is the queue
	int getMovingCarsIds(int i) const; //Next node where moving car i is heading at
	int getMovingCarsIdsSize() const; //Number of moving cars in the current queue
	void removeOneCar(); //removing the first moving car

private:
	int nodeA;
	int nodeB;
	int queueID;
	int roadID;
	std::vector<int> movingCarsIds;
};