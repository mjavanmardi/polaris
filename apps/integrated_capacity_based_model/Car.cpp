#include "Car.h"

using namespace std;

Car::Car(int _id, CarType _type, std::vector<int> _path, double _enteringTime) : 
	type(_type), path(_path), entryNode(*(path.begin())), exitingNode(path.back()), 
	enteringTime(_enteringTime), id(_id)
{
	state = in_WaitingForEntry;
	distanceInTA = 0;
	speed = 0;
	nextNodeIterator = (path.begin()+1);
	currentRoad = NULL;
	currentIndivQueue = pair<int,int>(-1,-1);
	distanceTraveled = 0;
	maxDistanceLeftInCurrentTimeStep = 0;
}

MoveResult Car::tryToEnterRoad(Road* road)
{
	if(state != in_WaitingForEntry)
		cout << "Problem with car : " << id << endl;
	double hasMoved = false;
	double hasChangedState = false;
	if(road->getRoomLeftInTravelingArea() > getLength()) //If there is room left in the targeted road
	{
		currentRoad = road; //update current road
		state = in_TravelingArea; //updating state
		distanceInTA = 0.;
		distanceInTimeStep = maxDistanceLeftInCurrentTimeStep; //We make sure that the that the car will not move anymore in the current time step
		hasMoved = true;
		hasChangedState = true;
	}
	MoveResult result(hasMoved,hasChangedState);
	return result;
}

MoveResult Car::travelingAreaMove(double dt)
{
	if(distanceInTimeStep >= maxDistanceLeftInCurrentTimeStep) //If the car can't move anymore
		return MoveResult(false,false);

	bool hasMoved = true;
	bool hasChangedState = false;
	JunctionArea* juncArea = currentRoad->getJA();
	CommonQueue* comQueue = currentRoad->getCQ();

	if(comQueue->getSize() == 0) //If the common queue is empty
	{
		int& nextRoadNodeA = (*nextNodeIterator);
		vector<int>::iterator itNodeB = next(nextNodeIterator);
		if(itNodeB ==path.end()) //If this is the car's last road
		{
			if(currentRoad->getLength() - juncArea->getLength() > distanceInTA) //If the car still needs to move
			{
				double distToStop = currentRoad->getLength()- juncArea->getLength() - comQueue->getLength() - distanceInTA;
				double alpha = computeAlpha(dt,distToStop);
				updateSpeedAndDistance(dt,alpha);
			}
			else //The car gets out of the network
			{
				distanceTraveled += getLength();
				state = in_OutOfNetwork;
				hasChangedState = true;
				currentRoad = NULL;
			}
		}
		else //If this is not the car's last road
		{
			int& nextRoadNodeB = (*itNodeB);
			int nextRoadId = currentRoad->getNodesToRoad()->at(nextRoadNodeA).at(nextRoadNodeB)->getId(); //We get the Id of the next road
			if(currentRoad->getLength() - juncArea->getLength() > distanceInTA) //If the car still needs to move
			{
				pair<bool,double> freePath = juncArea->isPathFree(nextRoadId,-1,-1); //freePath.second is the distance of free flow available in the junction area
				double distToStop = 0;
				if(freePath.first) //If there is a way to go to the next road without queueing
				{
					distToStop = currentRoad->getLength() - distanceInTA; // We consider that the stop is far away
				}
				else
				{
					double distLeftInTA = currentRoad->getLength() - distanceInTA - juncArea->getLength();
					double& freeDistanceInJA = freePath.second;
					distToStop = distLeftInTA + freeDistanceInJA;
				}
				double alpha = computeAlpha(dt,distToStop);
				updateSpeedAndDistance(dt,alpha);
			}
			else
			{
				//Insert in JA if room, in CQ if no room
				pair<int,int> nextQueue = juncArea->selectNextQueue(pair<int,int>(-1,-1),nextRoadId);
				double freeRoom = juncArea->getTotalLengthLeft(nextQueue.first,nextQueue.second);
				if(freeRoom >= type.getCarLength()) //If there is room for the car to enter the individual queue
				{
					distanceTraveled += 0; //Their can be overshoot, so we don't update the distanceTraveled
					juncArea->insertCar(this,nextQueue); //We insert the car in the junction area
					state = in_JunctionArea;
					currentIndivQueue = nextQueue;
					distanceInTA = 0;
				}
				else //No room : we insert in the CQ
				{
					distanceTraveled += 0; //Their can be overshoot, so we don't update the distanceTravled
					speed = 0;
					comQueue->addCar(this);
					state = in_CommonQueue;
					distanceInTA = 0;
				}
				hasChangedState = true;
			}
		}
	}
	else //If common queue is not empty
	{
		double distToComQueue = currentRoad->getLength() - currentRoad->getJA()->getLength()  - currentRoad->getCQ()->getLength() - distanceInTA;
		if(distToComQueue > 0) //If we still need to move
		{
			double alpha = computeAlpha(dt,distToComQueue);
			updateSpeedAndDistance(dt,alpha); //Updates distanceInTA, distanceTraveled, distanceInTimeStep and speed
		}
		else //If we don't need to move we enter the common queue
		{
			distanceTraveled += 0; //Their can be overshoot, so we don't update the distanceTravled
			speed = 0;
			comQueue->addCar(this);
			state = in_CommonQueue;
			hasChangedState = true;
		}
	}
	return MoveResult(hasMoved,hasChangedState);
}

void Car::updateSpeedAndDistance(double dt, double alpha)
{
	double newSpeed = max(0.,min(currentRoad->getMaxSpeed(),speed + alpha * dt));
	double dx = 0.5 * dt * (speed + newSpeed);
	distanceInTA += dx;
	distanceTraveled += dx;
	distanceInTimeStep += dx;
	speed = newSpeed;
}

double Car::computeAlpha(double dt, double distToStop)
{
	double alpha = 1.;
	double maxSpeed = currentRoad->getMaxSpeed();
	if(distToStop <= dBraking(dt,type.getBeta() * type.getMaxAcc()) )
	{
		if(distToStop <= dBraking(dt,type.getMaxAcc()))
		{
			alpha = - max(0.,min(type.getMaxDec(),speed*speed/(2.*distToStop)));
		}
		else
		{
			alpha = - type.getGamma() * type.getBeta() * type.getMaxDec();
		}
	}
	else
	{
		if(maxSpeed <= speed)
		{
			alpha = 0.;
		}
		else
		{
			alpha = type.getMaxAcc() * (1 - speed/maxSpeed * (1 - type.getDelta()));

		}
	}
	return alpha;
}

double Car::dBraking(double dt, double acceleration)
{
	int nb = (int) floor(speed/(acceleration * dt));
	return (nb * dt * (speed - 0.5 * acceleration * nb * dt));
}

MoveResult Car::travelingCommonQueue()
{
	if(distanceInTimeStep >= maxDistanceLeftInCurrentTimeStep) //If the car can't move anymore
		return MoveResult(false,false);
	bool hasMoved = false;
	bool hasChangedState = false;
	JunctionArea* juncArea = currentRoad->getJA();

	vector<int>::iterator itNodeB = next(nextNodeIterator);
	if(itNodeB == path.end()) //If this is the car's last road
	{
		distanceTraveled += getLength();
		state = in_OutOfNetwork; //The car gets out of the network
		hasChangedState = true;
		hasMoved = true;
		currentRoad = NULL;
	}
	else
	{
		int& nextRoadNodeA = (*nextNodeIterator);
		int& nextRoadNodeB = (*itNodeB);
		int nextRoadId = currentRoad->getNodesToRoad()->at(nextRoadNodeA).at(nextRoadNodeB)->getId(); //We get the Id of the next road
		pair<int,int> nextQueue = juncArea->selectNextQueue(pair<int,int>(-1,-1),nextRoadId);
		double freeRoom = juncArea->getTotalLengthLeft(nextQueue.first,nextQueue.second);
		if(freeRoom >= type.getCarLength()) //If there is room for the car to enter the individual queue
		{
			hasChangedState = true;
			hasMoved = true;
			juncArea->insertCar(this,nextQueue); //We insert the car in the junction area
			state = in_JunctionArea;
			currentIndivQueue = nextQueue;
			double dx = min(maxDistanceLeftInCurrentTimeStep-distanceInTimeStep, getLength()/currentRoad->getCQ()->getNumberOfLanes());
			distanceTraveled += dx;
			distanceInTimeStep += dx;
		}
	}

	return MoveResult(hasMoved,hasChangedState);
}

MoveResult Car::leaveRoad()
{
	if(distanceInTimeStep >= maxDistanceLeftInCurrentTimeStep) //If the car can't move anymore
		return MoveResult(false,false);
	bool hasMoved = false;
	bool hasChangedState = false;
	vector<int>::iterator nextNodeB = next(nextNodeIterator);
	if (nextNodeB == path.end()) //If this is the car's last road
	{
		hasChangedState = true;
		hasMoved = true;
		state = in_OutOfNetwork;
	}
	else //This is not the car's last road
	{
		//Get the next road ID
		Road* targetRoad = currentRoad->getNodesToRoad()->at(*nextNodeIterator).at(*nextNodeB);
		int targetRoadId = targetRoad->getId();
		//Get the capacity left for the targeted turning movement
		map<int,pair<double,double>> currentQueueCapacities = currentRoad->getCapacity()->at(currentRoad->getId()).at(currentIndivQueue.first);
		double& leftCapacity = currentQueueCapacities.at(targetRoadId).second;
		//Computing the car's capacity
		double minCapacity = DBL_MAX;
		for(map<int,pair<double,double>>::iterator it = currentQueueCapacities.begin() ; it != currentQueueCapacities.end() ; it++)
		{
			if(it->second.first < minCapacity)
				minCapacity = it->second.first;
		}
		double carCapacity = minCapacity / currentQueueCapacities.at(targetRoadId).first ;
		if(carCapacity <= leftCapacity || lastCarProba(leftCapacity,carCapacity)) //If there is enough capacity left
		{
			if(targetRoad->getRoomLeftInTravelingArea() > getLength()) //If there is room left in the targeted road
			{
				//Then we can enter the next Road
				targetRoad->getTA()->addCar(this);
				//The turning movement capacity decreasess (leftCapacity is a reference)
				leftCapacity = max(0.,(double) (leftCapacity-carCapacity));
				//We update the state variables
				hasChangedState = true;
				hasMoved = true;
				state = in_TravelingArea;
				currentRoad = targetRoad;
				distanceInTA = 0;
				nextNodeIterator++;
				currentIndivQueue = pair<int,int>(-1,-1);
				distanceInTimeStep += getLength();
				distanceTraveled += getLength();
			}
		}
	}
	return MoveResult(hasMoved,hasChangedState);
}

//Each cars which aims at crossing an intersection has a weight
//At each step, there is a total weight allowed to cross
//When a car has a weight superior to the remaining allowed weight
//There is a probability that it crosses the intersection
//This function returns 1 if the car crosses, 0 otherwise
bool Car::lastCarProba(double remainingAllowedWeight, double lastCarWeight) 
{
	bool canCross = false;
	srand(0);
	int possible = rand()%(int)(1000*lastCarWeight);

	if(1000*remainingAllowedWeight > possible)
		canCross = true;
	//cout << "Can cross : " << canCross << endl;
	return canCross;
}

MoveResult Car::moveFromLastFreeFlowArea(double dt)
{
	if(distanceInTimeStep >= maxDistanceLeftInCurrentTimeStep) //If the car can't move anymore
		return MoveResult(false,false);
	bool hasMoved = false;
	bool hasChangedState = false;
	double distanceLeft = currentRoad->getJA()->getFreeFlowSectionLeft(currentIndivQueue)-distanceInTA;
	if(distanceLeft > 0) //If the car still has to move
	{
		double alpha = computeAlpha(dt,distanceLeft);
		updateSpeedAndDistance(dt,alpha);
	}
	else
	{
		if(currentRoad->getJA()->isStuckSectionEmpty(currentIndivQueue)) //If the stuck section is empty
		{
			MoveResult tryToLeaveRoad = leaveRoad(); //Updates the state attributes if the car has managed to change road
			if(!tryToLeaveRoad.getHasChangedState()) //If the car couldn't leave the road
			{//We add it to the stuck section and we indicate it has changed state
				currentRoad->getJA()->insertCarInStuckSection(this,currentIndivQueue);
				hasMoved = true;
				hasChangedState = true;
				//Update the state variables
				distanceInTA = 0;
				speed = 0;
				distanceInTimeStep += 0; //There can be overshooting
				distanceTraveled += 0; //There can be overshooting
			}
			else
			{
				hasMoved = true;
				hasChangedState = true;
			}
		}
		else //If we arrived in the stuck section, we get into it
		{
			currentRoad->getJA()->insertCarInStuckSection(this,currentIndivQueue);
			hasMoved = true;
			hasChangedState = true;
			//Update the state variables
			distanceInTA = 0;
			speed = 0;
			distanceInTimeStep += 0; //There can be overshooting
			distanceTraveled += 0; //There can be overshooting
		}
	}
	return MoveResult(hasMoved,hasChangedState);
}

void Car::setSpeedZero()
{
	speed = 0.;
}

void Car::addDistanceTraveled(double distance)
{
	distanceTraveled += distance;
	distanceInTimeStep += min(distance,maxDistanceLeftInCurrentTimeStep-distanceInTimeStep);
}

void Car::postponeEnteringTime(double time)
{
	enteringTime += time;
}

//Initializes a car at the beginning of a timestep
//The max distance the car can travel during the timestep has to be set
void Car::initTimeStep(double dt)
{
	//To be improved
	maxDistanceLeftInCurrentTimeStep = 0.5 * type.getMaxAcc() * dt * dt; 
	distanceInTimeStep = 0;
}

double Car::getDistanceInTA() const
{
	return distanceInTA;
}

double Car::getDistanceTraveled() const
{
	return distanceTraveled;
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

CarState Car::getState() const
{
	return state;
}

void Car::speak()
{
	cout << "Car number : " << id << endl;
	cout << "  I have traveled a total of " << distanceTraveled << "m" << endl;
	switch(state)
	{
		case in_WaitingForEntry:
		{
			cout << "  I am waiting for road entry" << endl;
			break;
		}
		case in_TravelingArea:
		{
			cout << "  I am in the TA of road : " << currentRoad->getId() << endl;
			cout << "  I have traveled " << distanceInTA << "m in this road." << endl;
			cout << "  My speed is : " << speed << endl;
			break;
		}
		case in_CommonQueue:
		{
			cout << "  I am in the CQ of road : " << currentRoad->getId() << endl;
			break;
		}
		case in_JunctionArea:
		{
			cout << "  I am in the JA of road : " << currentRoad->getId() << endl;
			break;
		}
		case in_OutOfNetwork:
		{
			cout << "  I am out of the network" << endl;
			break;
		}
	}
	cout << endl;
}