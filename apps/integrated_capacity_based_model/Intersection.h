#pragma once

#include "Event.h"

class Road;
class Event;

class Intersection
{
public:
	Intersection(int _id);
	void addEnteringRoad(Road* newRoad);
	void addOutgoingRoad(Road* newRoad);
	void addEnteringCar(Car* car, Road* enteringRoad);
	std::vector<Event> runEvent(Event ev, double time, double dt);

	std::vector<Event> getPossibleEvents(); //Get every event that can happen on the intersection

	void speak();
	int getId() const;
private:
	const int id;
	std::vector<Road*> enteringRoads;
	std::vector<Road*> outgoingRoads;
	std::list<std::pair<Car*,Road*>> enteringCars; //Cars which enter the network in this node
};