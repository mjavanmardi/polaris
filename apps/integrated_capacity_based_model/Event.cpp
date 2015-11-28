#include "Event.h"

Event::Event() :
type(nullEvent), road(NULL), intersection(NULL)
{
	//default constructor builds a null event
}

Event::Event(EventType _type, Road* _road, Intersection* _intersection) :
	type(_type), road(_road), intersection(_intersection)
{

}

EventType Event::getType() const
{
	return type;
}

Road* Event::getRoad() const
{
	return road;
}

Intersection* Event::getIntersection() const
{
	return intersection;
}