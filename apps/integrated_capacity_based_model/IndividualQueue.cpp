#include "IndividualQueue.h"

using namespace std;

typedef vector<pair<int,TurningMovementType> >::const_iterator TurnMovIt ;

IndividualQueue::IndividualQueue(double _length, std::vector<std::pair<int,TurningMovementType> > _turningMovements, 
		double width, double baseSaturationFlowRate, double approachGrade, 
		double parkingFactor, double busFactor):
length(_length), turningMovements(_turningMovements), stuckSectionLength(0), 
freeFlowSectionLength(0)
{
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
		capacities[it->first] = baseCapacityFactor * areaFactor;
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
		stuckSection.push_back(car);
		stuckSectionLength += carLength;
	}
}

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

map<int,double> IndividualQueue::computeCapacities() const
{
	//Here we can add a traffic light model
	return capacities;
}

bool IndividualQueue::moveStuckCar()
{
	bool hasMoved = false;
	if(stuckSection.size() != 0)
	{
		Car* carMoving = *stuckSection.begin();
		carMoving->leaveRoad();
	}
	return hasMoved;
}