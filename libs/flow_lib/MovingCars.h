#pragma once

#include <vector>

// Contains the informations needed to move cars in a given queue
class MovingCars
{
public:
	MovingCars(int _nodeA, int _nodeB, int _queueID, int _roadID, std::vector<int> &_nextNodes);
	int getNodeA(); //Road's nodeB
	int getNodeB(); // Road's nodeA
	int getQueueID(); // Current Queue
	int getRoadID(); //Road where is the queue
	int getNextNode(int i); //Next node where moving car i is heading at
	int getNextNodeSize(); //Number of moving cars in the current queue

private:
	int nodeA;
	int nodeB;
	int queueID;
	int roadID;
	std::vector<int> nextNodes;
};