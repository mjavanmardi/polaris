#include "TravelingArea.h"
#include <iostream>
using namespace std;

bool distInTAComp::operator()(const Car* leftCar, const Car* rightCar)
{
	return (leftCar->getDistanceInTA() > rightCar->getDistanceInTA());
}

TravelingArea::TravelingArea(int _numberOfLanes) : 
multiset<Car*,distInTAComp>(), numberOfLanes(_numberOfLanes), totalLength(0)
{

}

void TravelingArea::addCar(Car* car)
{
	insert(car);
	totalLength += car->getLength();
}

void TravelingArea::removeCar(multiset<Car*,distInTAComp>::iterator &car)
{
	totalLength -= (*car)->getLength();
	erase(car);
}

double TravelingArea::getMinLength() const
{
	return (totalLength/double(numberOfLanes));
}

bool TravelingArea::moveCars(double dt)
{
	bool hasMoved = false;
	//The set structure allows to move first the cars which are located at the end
	//of the traveling area
	multiset<Car*,distInTAComp>::iterator carIt = begin();
	while(carIt != end())
	{
		if((*carIt)->getId()==90)
			cout << endl;
		MoveResult result = (*carIt)->travelingAreaMove(dt);
		hasMoved = hasMoved || result.getHasMoved();
		if(result.getHasChangedState()) //If a car has moved to a queue, we erase it
		{
			multiset<Car*,distInTAComp>::iterator carIt2 = next(carIt);
			removeCar(carIt);
			carIt = carIt2;
		}
		else //Otherwise, we move the next car
			carIt++;
	}
	return hasMoved;
}

void TravelingArea::speak() const
{
	cout << "Length of Traveling Area : " << getMinLength() << endl;
	cout << "Cars in the Traveling Area : " ;
	for(std::multiset<Car*,distInTAComp>::iterator it = begin(); it != end() ; it++)
	{
		cout << (*it)->getId() << " ; " ;
	}
	cout << endl;
}