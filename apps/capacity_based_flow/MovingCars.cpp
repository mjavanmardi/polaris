#include "MovingCars.h"

using namespace std;

MovingCars::MovingCars():
nodeA(0), nodeB(0), queueID(0), roadID(0)
{
	nextNodes = vector<int>(0);
}
MovingCars::MovingCars(int _nodeA, int _nodeB, int _queueID, int _roadID, std::vector<int> &_nextNodes) :
nodeA(_nodeA), nodeB(_nodeB), queueID(_queueID), roadID(_roadID)
{
	nextNodes = vector<int>(0);
	for(int i=0;i < _nextNodes.size();i++)
		nextNodes.push_back(_nextNodes[i]);
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
}