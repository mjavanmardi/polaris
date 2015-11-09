#include "IndividualQueue.h"

using namespace std;

typedef vector<pair<int,TurningMovementType> >::const_iterator TurnMovIt ;

IndividualQueue::IndividualQueue(double _length, std::vector<std::pair<int,TurningMovementType> > _turningMovements, 
		double width, double baseSaturationFlowRate, double approachGrade, 
		double parkingFactor, double busFactor):
length(_length), turningMovements(_turningMovements), stuckSectionLength(0), 
freeFlowSectionLength(0)
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
	cout << "Vehicles in free flow section : " ;
	for(list<Car*>::iterator it = freeFlowSection.begin();it != freeFlowSection.end();it++)
	{
		cout << (*it)->getId() << " ; " ;
	}
	cout << endl;
	cout << "Vehicles in stuck section : " ;
	for(list<Car*>::iterator it = stuckSection.begin();it != stuckSection.end();it++)
	{
		cout << (*it)->getId() << " ; " ;
	}
	cout << endl;
}

int IndividualQueue::getNumberOfCars() const
{
	return(freeFlowSection.size()+stuckSection.size());
}

double IndividualQueue::getTotalLengthLeft() const
{
	return(length-stuckSectionLength-freeFlowSectionLength);
}

double IndividualQueue::getStuckSectionLength() const
{
	return(stuckSectionLength);
}

double IndividualQueue::getFreeFlowSectionLength() const
{
	return(length-stuckSectionLength);
}

bool IndividualQueue::isEmpty() const
{
	return(freeFlowSection.size() == 0 && stuckSection.size() == 0);
}

bool IndividualQueue::isStuckSectionEmpty() const
{
	return(stuckSection.size() == 0);
}

void IndividualQueue::insertCar(Car* car)
{
	double carLength = car->getLength();
	if(length-stuckSectionLength >= carLength) //if there is room in the free flow section
	{
		freeFlowSection.push_back(car);
		freeFlowSectionLength += carLength;
	}
	else // add in the stuck section
	{
		insertCarInStuckSection(car);
	}
}

void IndividualQueue::insertCarInStuckSection(Car* car)
{
	stuckSection.push_back(car);
	car->setSpeedZero();
	stuckSectionLength += car->getLength();
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
		Car* carMoving = *stuckSection.begin();
		MoveResult result = carMoving->leaveRoad(dt);
		if(result.getHasChangedState())
		{
			//We remove the Car
			stuckSectionLength -= carMoving->getLength();
			stuckSection.erase(stuckSection.begin());
			//Every car after the removed car are moving
			for(list<Car*>::iterator it = stuckSection.begin() ; it != stuckSection.end() ; it++ )
			{
				(*it)->addDistanceTraveled(carMoving->getLength());
			}
			hasMoved = true; //We confirm that the car has actually moved
		}
	}
	return hasMoved;
}

//Move the cars in a free flow section located in the last column of the junction area
bool IndividualQueue::moveLastFreeFlowCars(double dt)
{
	bool hasMoved = false;
	list<Car*>::iterator carIt = freeFlowSection.begin();
	while(carIt != freeFlowSection.end()) //We go through the free flow cars
	{
		MoveResult result = (*carIt)->moveFromLastFreeFlowArea(dt);
		hasMoved = hasMoved || result.getHasMoved();
		if(result.getHasChangedState()) //If a car has left the free flow section, we erase it
		{
			freeFlowSectionLength-= (*carIt)->getLength();
			list<Car*>::iterator carIt2 = next(carIt);
			freeFlowSection.erase(carIt);
			carIt = carIt2;
		}
		else //Otherwise, we move the next car
			carIt++;
	}
	return hasMoved;
}