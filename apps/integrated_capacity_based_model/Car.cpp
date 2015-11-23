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
	distanceInCurrentRoad = 0;
	maxDistanceInCurrentTimeStep = 0;
}

MoveResult Car::tryToEnterRoad(Road* road)
{
	if(state != in_WaitingForEntry)
		cout << "Problem with car : " << id << endl;
	bool hasMoved = false;
	bool hasChangedState = false;
	if(road->getRoomLeftInTravelingArea() > getLength()) //If there is room left in the targeted road
	{
		road->getTA()->addCar(this); //Add the car to the new traveling area
		currentRoad = road; //update current road
		state = in_TravelingArea; //updating state
		distanceInTA = 0.;
		distanceInTimeStep = maxDistanceInCurrentTimeStep; //We make sure that the that the car will not move anymore in the current time step
		distanceInCurrentRoad=0;
		hasMoved = true;
		hasChangedState = true;
	}
	MoveResult result(hasMoved,hasChangedState);
	return result;
}

MoveResult Car::travelingAreaMove(double dt)
{
	if(distanceInTimeStep >= maxDistanceInCurrentTimeStep) //If the car can't move anymore
		return MoveResult(false,false);

	const bool hasMoved = true; //The car necessarily moves at this point
	bool hasChangedState = false;
	JunctionArea* juncArea = currentRoad->getJA();
	CommonQueue* comQueue = currentRoad->getCQ();

	bool isCommonQueueEmpty = (comQueue->getSize()==0);
	if(!isCommonQueueEmpty) //if the common queue is not empty
	{
		double distToComQueue = currentRoad->getLength() - currentRoad->getJA()->getLength()  - currentRoad->getCQ()->getLength() - distanceInTA;
		bool hasReachedCommonQueue = distToComQueue <= 0.;
		if(hasReachedCommonQueue) //if this is the car has reached its common queue
		{
			//The car enters the common queue
			comQueue->addCar(this);
			state = in_CommonQueue;
			hasChangedState = true;
			return MoveResult(hasMoved,hasChangedState);
		}
		else
		{
			//The car moves in free-flow
			//Parameter : distance to the common queue
			double alpha = computeAlpha(dt,distToComQueue);
			updateSpeedAndDistance(dt,alpha); //Updates distanceInTA, distanceTraveled, distanceInTimeStep and speed
			hasChangedState = false;
			return MoveResult(hasMoved,hasChangedState);
		}
	}
	vector<int>::iterator itNodeB = next(nextNodeIterator);
	bool isLastRoad = itNodeB == path.end();
	bool hasReachedJunctionArea = currentRoad->getLength() - juncArea->getLength() <= distanceInTA;
	if(isLastRoad) //if this is the car's last road
	{
		if(hasReachedJunctionArea) //if the car has reached its junction area
		{
			//The car gets out of the network
			state = in_OutOfNetwork;
			hasChangedState = true;
			currentRoad = NULL;
			return MoveResult(hasMoved,hasChangedState);
		}
		else
		{
			//The car moves in free-flow
			//Parameter : distance to the junction area
			double distToJunctionArea = currentRoad->getLength()- juncArea->getLength() - distanceInTA;
			double alpha = computeAlpha(dt,distToJunctionArea);
			updateSpeedAndDistance(dt,alpha);
			hasChangedState = false;
			return MoveResult(hasMoved,hasChangedState);
		}
	}
	int& nextRoadNodeA = (*nextNodeIterator);
	int& nextRoadNodeB = (*itNodeB);
	int nextRoadId = currentRoad->getNodesToRoad()->at(nextRoadNodeA).at(nextRoadNodeB)->getId(); //We get the Id of the next road
	if(!hasReachedJunctionArea) //if the car has not reached its junction area
	{
		
		pair<bool,double> freePath = juncArea->isPathFree(nextRoadId,-1,-1); //freePath.second is the distance of free flow available in the junction area
		bool& isFreeFlowPathInJunctionArea = freePath.first;
		if(isFreeFlowPathInJunctionArea) //If there is a way to go to the next road without queueing
		{
			//The car moves in free-flow
			//Parameter : we consider that the stop is at the end of the road
			double distToStop = currentRoad->getLength() - distanceInTA; 
			double alpha = computeAlpha(dt,distToStop);
			updateSpeedAndDistance(dt,alpha);
			hasChangedState = false;
			return MoveResult(hasMoved,hasChangedState);
		}
		else
		{
			//The car moves in free-flow
			//Parameter : maximum free-flow distance avaiable
			double distLeftInTA = currentRoad->getLength() - distanceInTA - juncArea->getLength();
			double& freeDistanceInJA = freePath.second;
			double maxFreeFlowDistance = distLeftInTA + freeDistanceInJA;
			double alpha = computeAlpha(dt,maxFreeFlowDistance);
			updateSpeedAndDistance(dt,alpha);
			hasChangedState = false;
			return MoveResult(hasMoved,hasChangedState);
		}

	}
	pair<int,int> nextQueue = juncArea->selectNextQueue(pair<int,int>(-1,-1),nextRoadId);
	double freeRoom = juncArea->getTotalLengthLeft(nextQueue.first,nextQueue.second);
	bool isRoomInJunctionArea = (freeRoom >= type.getCarLength());
	if(isRoomInJunctionArea) //If there is room for the car to enter the individual queue 
	{
		//The car enters the junction area
		juncArea->insertCar(this,nextQueue); 
		state = in_JunctionArea;
		currentIndivQueue = nextQueue;
		distanceInTA = 0;
		hasChangedState = true;
		return MoveResult(hasMoved,hasChangedState);
	}
	else
	{
		//The car enters the common queue
		comQueue->addCar(this);
		state = in_CommonQueue;
		distanceInTA = 0;
		hasChangedState = true;
		return MoveResult(hasMoved,hasChangedState);
	}
}

void Car::updateSpeedAndDistance(double dt, double alpha)
{
	double newSpeed = max(0.,min(currentRoad->getMaxSpeed(),speed + alpha * dt));
	double dx = 0.5 * dt * (speed + newSpeed);
	distanceInTA += dx;
	distanceTraveled += dx;
	distanceInTimeStep += dx;
	distanceInCurrentRoad +=dx;
	speed = newSpeed;
}

//See documentation : free flow algorithm feature
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

//Used only for the first layer of cars in the Common Queue
MoveResult Car::travelingCommonQueue(double dt)
{
	if(distanceInTimeStep >= maxDistanceInCurrentTimeStep) //If the car can't move anymore
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

		bool isJunctionAreaReached = getDistanceToJunctionArea() <= 0;
		if(isJunctionAreaReached) //If junction area is reached
		{
			bool isRoomLeftInJunctionArea = freeRoom >= getLength();
			if(isRoomLeftInJunctionArea) //If there is room for the car to enter the individual queue
			{
				hasChangedState = true;
				hasMoved = true;
				juncArea->insertCar(this,nextQueue); //We insert the car in the junction area
				state = in_JunctionArea;
				currentIndivQueue = nextQueue;
				double dx = min(maxDistanceInCurrentTimeStep-distanceInTimeStep, getLength()/currentRoad->getCQ()->getNumberOfLanes());
				distanceInCurrentRoad += dx;
				distanceTraveled += dx;
				distanceInTimeStep += dx;
			}
		}
		else
		{
			double alpha = computeAlpha(dt,freeRoom+getDistanceToJunctionArea());
			updateSpeedAndDistance(dt,alpha);
		}
	}
	return MoveResult(hasMoved,hasChangedState);
}

MoveResult Car::leaveRoad(double dt)
{
	double distanceToNextRoad = currentRoad->getLength() - distanceInCurrentRoad;
	if(distanceInTimeStep >= maxDistanceInCurrentTimeStep) //If the car can't move anymore
		return MoveResult(false,false);
	bool hasMoved = false;
	bool hasChangedState = false;
	vector<int>::iterator nextNodeB = next(nextNodeIterator);
	if (nextNodeB == path.end()) //If this is the car's last road
	{
		hasChangedState = true;
		hasMoved = true;
		state = in_OutOfNetwork;
		return MoveResult(hasMoved,hasChangedState);
	}
	//This is not the car's last road

	distanceToNextRoad = currentRoad->getLength() - distanceInCurrentRoad;
	if(distanceToNextRoad >0)
	{
		double alpha = computeAlpha(dt,distanceToNextRoad);
		updateSpeedAndDistance(dt,alpha); //Updates distanceInTA, distanceTraveled, distanceInTimeStep and speed
	}
	distanceToNextRoad = currentRoad->getLength() - distanceInCurrentRoad;
	if(distanceToNextRoad <= 0)
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
				nextNodeIterator++;
				currentIndivQueue = pair<int,int>(-1,-1);
				
				double dx = min(-distanceToNextRoad,distanceInTimeStep - maxDistanceInCurrentTimeStep);

				distanceInTA = 0;
				distanceInTimeStep += 0;
				distanceTraveled += 0;

				distanceInCurrentRoad = 0;
			}
			else
				speed = 0;
		}
		else
			speed = 0;
	}
	return MoveResult(hasMoved,hasChangedState);
}

//Each cars which aims at crossing an intersection has a weight
//At each step, there is a total weight allowed to cross
//When a car has a weight superior to the remaining allowed weight
//There is a probability that it crosses the intersection
//This function returns 1 if the car crosses, 0 otherwise
bool Car::lastCarProba(double remainingAllowedWeight, double lastCarWeight) const
{
	bool canCross = false;
	int possible = rand()%(int)(1000*lastCarWeight);

	if(1000*remainingAllowedWeight > possible)
		canCross = true;
	//cout << "Can cross : " << canCross << endl;
	return canCross;
}

bool Car::moveQueuing(double frontDistanceAvailable,double frontSpeed, double dt)
{
	if(distanceInTimeStep >= maxDistanceInCurrentTimeStep) //If the car can't move anymore
		return false;
	if(frontDistanceAvailable<0)
	{
		speed = max(0.,speed - type.getMaxDec() * dt);
		return false;
	}
	double safetyDistance = getSafetyDistance(frontSpeed,currentRoad->getMaxSpeed());
	
	double error = (frontDistanceAvailable) - safetyDistance;
	double lowBound = speed - type.getMaxDec() * dt;
	double highBound = speed + type.getMaxAcc() * dt;
	speed = min(min(currentRoad->getMaxSpeed(),highBound),max(max(0.,lowBound),error/dt));
	double dx = speed*dt;
	distanceInTimeStep += dx;
	distanceTraveled += dx;
	distanceInCurrentRoad += dx;
	return (dx >= 1E-14); //To avoid numerical error
}

double Car::getSafetyDistance(double frontCarSpeed, double maxSpeed) const
{
	return type.getTau() * frontCarSpeed + maxSpeed/(2*type.getMaxDec()) * (speed - frontCarSpeed) + type.getDz(); 
}

void Car::setSpeedZero()
{
	speed = 0.;
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
	maxDistanceInCurrentTimeStep = 0.5 * type.getMaxAcc() * dt * dt; 
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

int Car::getCurrentLane() const
{
	return currentIndivQueue.first;
}

int Car::currentRoadId() const
{
	if(currentRoad != NULL)
		return currentRoad->getId();
	else
		return -1;
}

double Car::getDistanceInCurrentRoad() const
{
	return distanceInCurrentRoad;
}

double Car::getDistanceToJunctionArea() const
{
	return currentRoad->getLength()-currentRoad->getJA()->getLength() - distanceInCurrentRoad;
}

double Car::getSpeed() const
{
	return speed;
}

double Car::getDistanceToNextRoad() const
{
	return currentRoad->getLength()-distanceInCurrentRoad;
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