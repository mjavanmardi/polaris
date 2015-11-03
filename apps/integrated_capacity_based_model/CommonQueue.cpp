#include "CommonQueue.h"

using namespace std;

CommonQueue::CommonQueue(double _distanceBetweenCars, int _numberOfLanes) :
list<Car*>(), distanceBetweenCars(_distanceBetweenCars), numberOfLanes(_numberOfLanes), length(0)
{

}

void CommonQueue::addCar(Car* car)
{
	push_back(car);
	length += car->getLength() /double(numberOfLanes);
}

void CommonQueue::removeCar(std::list<Car*>::iterator car)
{
	length -= (*car)->getLength();
	erase(car);
}

bool CommonQueue::moveCars()
{
	bool hasMoved = false;
	//The set structure allows to move first the cars which are located at the end
	//of the traveling area
	list<Car*>::iterator carIt = begin();
	int numberOfMoves = numberOfLanes;
	for(int i=0;i<numberOfMoves && carIt != end();i++) //Only the cars in the first layer can move
	{
		MoveResult result = (*carIt)->travelingCommonQueue();
		hasMoved = hasMoved || result.getHasMoved();
		if(result.getHasChangedState()) //If a car has moved to a queue, we erase it and we move the iterator to the next car
		{
			list<Car*>::iterator carIt2 = next(carIt);
			numberOfMoves++; //If we delete a car, it means that a new one appears in the first layer
			for(list<Car*>::iterator carItMove = next(carIt);carItMove != end();carItMove++)
			{ //Each vehicle after the vehicle that has left moves
				(*carItMove)->addDistanceTraveled((*carIt)->getLength() / double(numberOfLanes));
			}
			removeCar(carIt);
			carIt = carIt2;
		}
		else //Otherwise, we just move the iterator to next car
			carIt++;
	}
	return hasMoved;
}

void CommonQueue::speak()
{
	cout << "Common Queue length : " << length << endl;
	cout << "Cars in  the common queue : " ;
	for(list<Car*>::iterator it = begin();it!=end();it++)
	{
		cout << (*it)->getLength() << " ; " ;
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