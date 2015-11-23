#include "CommonQueue.h"

using namespace std;

CommonQueue::CommonQueue(int _numberOfLanes) :
numberOfLanes(_numberOfLanes), length(0)
{

}

void CommonQueue::addCar(Car* car)
{
	push_back(car);
	length += car->getLength() /double(numberOfLanes);
}

bool CommonQueue::moveCars(double dt)
{
	bool hasMoved = false;
	double minDistanceInFrontLayer = DBL_MAX;
	double frontDistanceAvailable = 0;
	double averageSpeedFirstLayer = 0;
	list<Car*>::iterator carIt = begin();
	int it = 0;
	int maxIt = numberOfLanes;
	//We move the cars in the first layer
	while(it < maxIt && carIt != end())
	{
		MoveResult result = (*carIt)->travelingCommonQueue(dt);
		if(result.getHasChangedState())
		{
			list<Car*>::iterator nextIt = next(carIt);
			length -= (*carIt)->getLength() /double(numberOfLanes);
			erase(carIt);
			carIt = nextIt;
			maxIt++; //One car is out means that one appears in the front layer
		}
		else
		{
			if((*carIt)->getDistanceInCurrentRoad() < minDistanceInFrontLayer)
				minDistanceInFrontLayer = (*carIt)->getDistanceInCurrentRoad();
			averageSpeedFirstLayer += (*carIt)->getSpeed();
			carIt++;
		}
		it++;
	}
	list<Car*>::iterator firstLayerCarIt = begin();
	for(int i=0;i<numberOfLanes && firstLayerCarIt != end();i++)
	{
		frontDistanceAvailable += (*firstLayerCarIt)->getDistanceInCurrentRoad() - minDistanceInFrontLayer;
		firstLayerCarIt++;
	}
	double frontCarPosition = 0;
	double frontCarSpeed = 0;
	//We move the car right after the first layer
	if(carIt != end())
	{
		frontDistanceAvailable = frontDistanceAvailable / (double) numberOfLanes;
		averageSpeedFirstLayer = averageSpeedFirstLayer / (double) numberOfLanes;
		(*carIt)->moveQueuing(frontDistanceAvailable,averageSpeedFirstLayer,dt);
		frontCarPosition = (*carIt)->getDistanceInCurrentRoad();
		frontCarSpeed = (*carIt)->getSpeed();
		carIt++;
	}
	//We move the cars remaining
	for(carIt;carIt != end();carIt++)
	{
		(*carIt)->moveQueuing(max(0.,frontCarPosition - (*carIt)->getDistanceInCurrentRoad()),frontCarSpeed,dt);
		frontCarPosition = (*carIt)->getDistanceInCurrentRoad();
		frontCarSpeed = (*carIt)->getSpeed();
	}
	return hasMoved;
}

void CommonQueue::speak() const
{
	cout << "Common Queue length : " << length << endl;
	cout << "Cars in  the common queue : " ;
	for(list<Car*>::const_iterator it = begin();it!=end();it++)
	{
		cout << (*it)->getId() << " ; " ;
	}
	cout << endl;
}

double CommonQueue::getLength() const
{
	return length;
}

int CommonQueue::getSize() const
{
	return size();
}

int CommonQueue::getNumberOfLanes() const
{
	return numberOfLanes;
}