#include "IndividualQueue.h"

using namespace std;

IndividualQueue::IndividualQueue(double _length, std::vector<int> _turningMovements):
length(_length), turningMovements(_turningMovements)
{

}

IndividualQueue& IndividualQueue::operator=(const IndividualQueue& iq)
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