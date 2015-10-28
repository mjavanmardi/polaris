#include "Car.h"

using namespace std;

Car::Car(int _id, CarType _type, std::vector<int> _path, double _enteringTime) : 
	type(_type), path(_path), entryNode(*(path.begin())), exitingNode(path.back()), 
	enteringTime(_enteringTime), id(_id)
{
	state = in_WaitingForEntry;
	distanceInTA = 0;
	speed = 0;
	nextNodeIterator = path.begin()++;
	currentRoad = NULL;
	currentIndivQueue = pair<int,int>(-1,-1);
	distanceTraveled = 0;
	maxDistanceLeftInCurrentTimeStep = 0;
}

MoveResult Car::move()
{
	MoveResult result(false,false);
	//Each moving procedure has to update distanceInTimeStep !
	switch(state) //The moving procedure depends on the current state
	{
		case in_WaitingForEntry:
		{
			return tryToEnterRoad();
			break;
		}
		case in_TravelingArea:
		{
			return travelingAreaMove();
			break;
		}
		case in_CommonQueue:
		{
			return travelingCommonQueue();
			break;
		}
		case in_JunctionArea:
		{
			break;
		}
		case in_OutOfNetwork:
		{
			break;
		}
	}
	return result;
}

MoveResult Car::tryToEnterRoad()
{
	MoveResult result(true,true);
	//For the moment, this method is executed only if their is room for the car to enter the road
	//As a consequence, the move result is always (true,true)
	distanceInTimeStep = maxDistanceLeftInCurrentTimeStep; //We make sure that the that the car will not move anymore in the current time step
	return result;
}

MoveResult Car::travelingAreaMove()
{
	bool hasMoved = true;
	bool hasChangedState = true;
	/*if(maxDistanceLeftInCurrentTimeStep<=0)
		return MoveResult(false,false);*/
	//TODO
	return MoveResult(hasMoved,hasChangedState);
}

MoveResult Car::travelingCommonQueue()
{
	bool hasMoved = true;
	bool hasChangedState = true;
	/*if(maxDistanceLeftInCurrentTimeStep<=0)
		return MoveResult(false,false);*/
	//TODO
	return MoveResult(hasMoved,hasChangedState);
}

void Car::addDistanceTraveled(double distance)
{
	distanceTraveled += distance;
}

void Car::postponeEnteringTime(double time)
{
	enteringTime += time;
}

//Initializes a car at the beginning of a timestep
//The max distance the car can travel during the timestep has to be set
void Car::initTimeStep(double dt)
{
	maxDistanceLeftInCurrentTimeStep = dt * speed;
	distanceInTimeStep = 0;
}

double Car::getDistanceInTA() const
{
	return distanceInTA;
}

double Car::getLength() const
{
	return type.getCarLength();
}

int Car::getId() const
{
	return id;
}

int Car::getEntryNode() const
{
	return entryNode;
}

int Car::getNextNode() const
{
	return (*nextNodeIterator);
}

double Car::getEnteringTime() const
{
	return enteringTime;
}