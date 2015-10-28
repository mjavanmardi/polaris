#include "EventManager.h"

using namespace std;

typedef vector<Intersection*>::iterator interIt;

EventManager::EventManager(double _dt) :
dt(_dt)
{

}

EventManager::~EventManager()
{

}

void EventManager::setupTimeStep(RoadNetwork* rn, CarManager* cs)
{
	vector<Intersection*>* network = rn->getIntersections();
	//### Loading all possible events ###
	for(interIt it = network->begin(); it != network->end();it++)
	{
		vector<Event> eventsToAdd = (*it)->getPossibleEvents();
		for(int i=0;i<eventsToAdd.size();i++)
			if(eventsToAdd[i].getType() != nullEvent) //We don't add null events
				eventStack.push(eventsToAdd[i]);
	}
	//### Setup cars ###
	//TODO
}