#include "MovingCars.h"

using namespace std;

MovingCars::MovingCars():
nodeA(0), nodeB(0), queueID(0), roadID(0)
{
	nextNodes = vector<int>(0);
	carIds = vector<int>(0);
}
MovingCars::MovingCars(int _nodeA, int _nodeB, int _queueID, int _roadID, std::vector<int> &_nextNodes, vector<int> &_carIds) :
nodeA(_nodeA), nodeB(_nodeB), queueID(_queueID), roadID(_roadID)
{
	nextNodes = vector<int>(0);
	carIds = vector<int>(0);
	for(int i=0;i < _nextNodes.size();i++)
	{
		nextNodes.push_back(_nextNodes[i]);
		carIds.push_back(_carIds[i]);
	}
}

int MovingCars::getNodeA()
{
	return nodeA;
}

int MovingCars::getNodeB()
{
	return nodeB;
}

int MovingCars::getQueueID()
{
	return queueID;
}

int MovingCars::getRoadID()
{
	return roadID;
}

int MovingCars::getNextNode(int i)
{
	return nextNodes[i];
}

int MovingCars::getNextNodesSize()
{
	return nextNodes.size();
}

void MovingCars::removeOneCar()
{
	nextNodes.pop_back();
	carIds.pop_back();
}

int MovingCars::getCarID(int i)
{
	return carIds[i];
}

int MovingCars::getCarIDsSize()
{
	return carIds.size();
}

bool MovingCars::isThere(int i) const
{
	bool result = false;
	for(vector<int>::const_iterator it = carIds.begin(); it != carIds.end() && !result;it++)
		result = (*it==i);
	return result;
}