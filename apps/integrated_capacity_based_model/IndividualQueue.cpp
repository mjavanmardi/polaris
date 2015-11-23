#include "IndividualQueue.h"

using namespace std;

typedef vector<pair<int,TurningMovementType> >::const_iterator TurnMovIt ;

IndividualQueue::IndividualQueue(double _length, std::vector<std::pair<int,TurningMovementType> > _turningMovements, 
		double width, double baseSaturationFlowRate, double approachGrade, 
		double parkingFactor, double busFactor):
length(_length), turningMovements(_turningMovements), firstCarDistanceToNextRoad(0)
{
	//### Computing the capacities ###

	//Width factor model : 
	double widthFactor = 1.;
	if(width <3.048)
		widthFactor = 0.96;
	else
		if(width>3.93192)
			widthFactor = 1.04;

	//Approach grade model : 
	double gradeFactor = 1. - approachGrade / 200.;

	//Base factor model in vehicle/s :
	double baseCapacityFactor = baseSaturationFlowRate * widthFactor * 
		gradeFactor * parkingFactor * busFactor;

	for(TurnMovIt it = turningMovements.begin();it != turningMovements.end();it++)
	{
		double areaFactor = 1.;
		switch(it->second)
		{
			case Uturn :
				areaFactor = 0.9;
				break;
			case Left :
				areaFactor = 1./1.05;
				break;
			case Right :
				areaFactor = 1./1.18;
				break;
		}
		double realCapa = baseCapacityFactor * areaFactor;
		staticCapacities[it->first] = pair<double,double>(realCapa,realCapa);
	}

}

IndividualQueue IndividualQueue::operator=(const IndividualQueue& iq)
{
	IndividualQueue queue(iq.getLength(),iq.getTurningMovements());
	return queue;
}

double IndividualQueue::getLength() const
{
	return length;
}

vector<std::pair<int,TurningMovementType> > IndividualQueue::getTurningMovements() const
{
	return turningMovements;
}

void IndividualQueue::speak()
{
	cout << "Vehicles in stuck section : " ;
	for(list<Car*>::iterator it = stuckSection.begin();it != stuckSection.end();it++)
	{
		cout << (*it)->getId() << " ; " ;
	}
	cout << endl;
}

int IndividualQueue::getNumberOfCars() const
{
	return(stuckSection.size());
}

double IndividualQueue::getTotalLengthLeft() const
{
	return(length-firstCarDistanceToNextRoad);
}

bool IndividualQueue::isEmpty() const
{
	return(stuckSection.size() == 0);
}

void IndividualQueue::insertCar(Car* car)
{
	insertCarInStuckSection(car);
}

void IndividualQueue::insertCarInStuckSection(Car* car)
{
	stuckSection.push_back(car);
	if(car->getDistanceToNextRoad() > firstCarDistanceToNextRoad)
		firstCarDistanceToNextRoad = car->getDistanceToNextRoad();
}

//Check if road i is in the turning movements of the current individual queue
//Performance note : this can seem not to be performant. However, there is usually
//a maximum of 3 turning movements per individual queue
bool IndividualQueue::isInTurningMovements(int i) const
{
	bool isHere = false;
	for(TurnMovIt it = turningMovements.begin();it != turningMovements.end();it++)
	{
		if(it->first==i)
		{
			isHere = true;
			break;
		}
	}
	return isHere;
}

map<int,pair<double,double> > IndividualQueue::getStaticCapacities() const
{
	return staticCapacities;
}

//Move the cars in a stuck section located in the last column of the junction area
bool IndividualQueue::moveLastStuckCar(double dt)
{
	bool hasMoved = false;
	if(stuckSection.size() != 0)
	{
		bool firstCarLeft = true;
		double positionFirstVehicle = -1.;
		while(firstCarLeft && stuckSection.size() != 0)
		{
			firstCarLeft = false;
			Car* carMoving = *stuckSection.begin();
			positionFirstVehicle = carMoving->getDistanceInCurrentRoad();
			MoveResult result = carMoving->leaveRoad(dt);
			firstCarLeft = result.getHasChangedState();
			if(result.getHasChangedState())
			{
				//We remove the Car
				stuckSection.erase(stuckSection.begin());
				if(stuckSection.size()==0)
					firstCarDistanceToNextRoad = 0;
				hasMoved = true; //We confirm that the car has actually moved
			}
		}
		//Every car after the removed car are moving
		list<Car*>::iterator it = stuckSection.begin();
		list<Car*>::iterator nextIt = it;
		if(it != stuckSection.end())
			nextIt = next(it);
		while(nextIt != stuckSection.end())
		{
			double frontDistanceAvailable = (*it)->getDistanceInCurrentRoad() - (*nextIt)->getDistanceInCurrentRoad();
			double frontSpeed = (*it)->getSpeed();
			bool hasCarMoved = (*nextIt)->moveQueuing(frontDistanceAvailable,frontSpeed,dt);
			hasMoved = hasMoved || hasCarMoved;
			it++;
			if(it != stuckSection.end())
			    nextIt++;
		}
		if(stuckSection.size() !=0)
			firstCarDistanceToNextRoad = stuckSection.back()->getDistanceToNextRoad();
	}
	return hasMoved;
}