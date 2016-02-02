#pragma once

#include "Road.h"
#include "Intersection.h"

class Road;
class Intersection;

//The 3 different types of events + the null event
enum EventType {nullEvent,enteringRoad,outgoingRoad,enteringNetwork};

//An event is an order given to an intersection to move certain cars.
//See documentation
class CMEvent
{
public:
	CMEvent();
	CMEvent(EventType _type, Road* _road, Intersection* _intersection);
	EventType getType() const;
	Road* getRoad() const;
	Intersection* getIntersection() const;
private:
	EventType type;
	Road* road;
	Intersection* intersection;
};