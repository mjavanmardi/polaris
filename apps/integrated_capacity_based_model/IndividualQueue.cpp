#include "IndividualQueue.h"

using namespace std;

IndividualQueue::IndividualQueue(double _length, std::vector<int> _turningMovements):
length(_length), turningMovements(_turningMovements), stuckSectionLength(0), 
freeFlowSectionLength(0)
{

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

vector<int> IndividualQueue::getTurningMovements() const
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
	for(vector<int>::const_iterator it = turningMovements.begin();it != turningMovements.end();it++)
	{
		if(*it==i)
		{
			isHere = true;
			break;
		}
	}
	return isHere;
}