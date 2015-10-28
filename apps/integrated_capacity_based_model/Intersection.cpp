#include "Intersection.h"

using namespace std;

typedef list<pair<Car*,Road*>>::iterator EnterCarIt;

Intersection::Intersection(int _id) :
id(_id)
{

}

void Intersection::addEnteringRoad(Road* newRoad)
{
	enteringRoads.push_back(newRoad);
}

void Intersection::addOutgoingRoad(Road* newRoad)
{
	outgoingRoads.push_back(newRoad);
}

Event Intersection::runEvent(Event ev, double time, double dt)
{
	Event eventToReschedule(nullEvent,NULL,NULL); //sample
	EventType type = ev.getType();
	switch(type)
	{
		case enteringRoad:
		 {
			 Road* road = ev.getRoad();
			 //TODO : add function to execute in road
			 break;
		 }
		case outgoingRoad:
		 {
			 Road* road = ev.getRoad();
			 //TODO : add function to execute in road
			 break;
		 }
		case enteringNetwork:
		 {
			 EnterCarIt it = enteringCars.begin();
			 while(it != enteringCars.end())
			 {
				 EnterCarIt it2 = next(it);
				 Car* currentCar = it->first;
				 Road* targetRoad = it->second;
				 if(currentCar->getEnteringTime() <= time)
				 {
					 if(targetRoad->getRoomLeftInTravelingArea() >= currentCar->getLength())
					 {
						 MoveResult result = currentCar->move(); //Run the moving function in the car
						 targetRoad->getTA()->addCar(currentCar); //Add the car to the new traveling area
						 enteringCars.erase(it); //erasing the car
						 it = it2; //updating the iterator
					 }
					 else
					 {
						 currentCar->postponeEnteringTime(dt); //We postpone the car's entering time
						 it++; //updating the iterator
					 }
				 }
				 else
					 it++;
			 }
			 break;
		 }
	}
	return eventToReschedule;
}

std::vector<Event>& Intersection::getPossibleEvents()
{
	vector<Event> possibleEvents;
	//Vehicle entering the network insertion
	if(enteringCars.size() != 0) //If there are vehicle waiting for entrance
	{
		Event enterCar(enteringNetwork,NULL,this);
		possibleEvents.push_back(enterCar);
	}
	//Entering roads
	for(vector<Road*>::iterator it = enteringRoads.begin();it != enteringRoads.end();it++)
	{
		Event moveCarsInEnteringRoad(enteringRoad,(*it),this);
		possibleEvents.push_back(moveCarsInEnteringRoad);
	}
	//Outgoing roads
	for(vector<Road*>::iterator it = outgoingRoads.begin();it != outgoingRoads.end();it++)
	{
		Event moveCarsInOutgoingRoad(outgoingRoad,(*it),this);
		possibleEvents.push_back(moveCarsInOutgoingRoad);
	}
	return possibleEvents;
}

int Intersection::getId() const
{
	return id;
}

void Intersection::speak()
{
	cout << "Intersection number " << id << endl;
	cout << "Cars waiting for entry : " ;
	for(EnterCarIt it = enteringCars.begin();it != enteringCars.end() ; it++)
	{
		cout << (*it).first->getId() << " ; " ;
	}
	cout << endl;
}

void Intersection::addEnteringCar(Car* car, Road* enteringRoad)
{
	enteringCars.push_back(pair<Car*,Road*>(car,enteringRoad));
}