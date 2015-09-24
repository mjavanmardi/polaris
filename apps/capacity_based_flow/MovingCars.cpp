#include "MovingCars.h"

using namespace std;

MovingCars::MovingCars():
nodeA(0), nodeB(0), queueID(0), roadID(0)
{
	movingCarsIds = vector<int>(0);
}
MovingCars::MovingCars(int _nodeA, int _nodeB, int _queueID, int _roadID, std::vector<int> &_movingCarsIds) :
nodeA(_nodeA), nodeB(_nodeB), queueID(_queueID), roadID(_roadID)
{
	movingCarsIds = vector<int>(0);
	for(int i=0;i < movingCarsIds.size();i++)
		movingCarsIds.push_back(_movingCarsIds[i]);
}

int MovingCars::getNodeA() const
{
	return nodeA;
}

int MovingCars::getNodeB() const
{
	return nodeB;
}

int MovingCars::getQueueID() const
{
	return queueID;
}

int MovingCars::getRoadID() const
{
	return roadID;
}

int MovingCars::getMovingCarsIds(int i) const
{
	return movingCarsIds[i];
}

int MovingCars::getMovingCarsIdsSize() const
{
	return movingCarsIds.size();
}

void MovingCars::removeOneCar()
{
	movingCarsIds.pop_back();
}