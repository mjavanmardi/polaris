#include "Event.h"

CMEvent::CMEvent() :
type(nullEvent), road(NULL), intersection(NULL)
{
	//default constructor builds a null event
}

CMEvent::CMEvent(EventType _type, Road* _road, Intersection* _intersection) :
	type(_type), road(_road), intersection(_intersection)
{

}

EventType CMEvent::getType() const
{
	return type;
}

Road* CMEvent::getRoad() const
{
	return road;
}

Intersection* CMEvent::getIntersection() const
{
	return intersection;
}