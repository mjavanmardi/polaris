#pragma once

#include "Event.h"

class Road;
class CMEvent;

class Intersection
{
public:
	Intersection(int _id);
	void addEnteringRoad(Road* newRoad);
	void addOutgoingRoad(Road* newRoad);
	void addEnteringCar(Car* car, Road* enteringRoad);
	std::vector<CMEvent> runEvent(CMEvent ev, double time, double dt);
	void setCapacities(std::vector< std::vector < std::map <int,std::pair<double,double> > > >* networkCapacities);

	std::vector<CMEvent> getPossibleEvents(); //Get every event that can happen on the intersection

	void speak();
	int getId() const;
private:
	const int id;
	std::vector<Road*> enteringRoads;
	std::vector<Road*> outgoingRoads;
	std::list<std::pair<Car*,Road*>> enteringCars; //Cars which enter the network in this node
};