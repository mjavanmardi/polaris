#pragma once

#include "Road.h"
#include "Intersection.h"

class Road;
class Intersection;

enum EventType {nullEvent,enteringRoad,outgoingRoad,enteringNetwork};

class Event
{
public:
	Event();
	Event(EventType _type, Road* _road, Intersection* _intersection);
	EventType getType() const;
	Road* getRoad() const;
	Intersection* getIntersection() const;
private:
	EventType type;
	Road* road;
	Intersection* intersection;
};