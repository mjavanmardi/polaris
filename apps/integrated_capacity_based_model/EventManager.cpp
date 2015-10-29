#include "EventManager.h"

using namespace std;

typedef vector<Intersection*>::iterator interIt;
typedef vector<Car*>::iterator carIt;

EventManager::EventManager(double _dt, int _numberOfSteps) :
dt(_dt), time(0), numberOfSteps(_numberOfSteps)
{

}

EventManager::~EventManager()
{

}

void EventManager::setupTimeStep(RoadNetwork* rn, CarManager* cs)
{
	vector<Intersection*>* network = rn->getIntersections();
	for(interIt it = network->begin(); it != network->end();it++)
	{
		//### Loading all possible events ###
		vector<Event> eventsToAdd = (*it)->getPossibleEvents();
		for(int i=0;i<eventsToAdd.size();i++)
			if(eventsToAdd[i].getType() != nullEvent) //We don't add null events
				eventStack.push(eventsToAdd[i]);
		//### Setup capacities ###
		//TODO
	}
	//### Setup cars ###
	vector<Car*>* cars = cs->getCars();
	for(carIt it = cars->begin();it != cars->end();it++)
	{
		(*it)->initTimeStep(dt);
	}
}


void EventManager::runTimeStep(RoadNetwork* rn)
{
	while(!eventStack.empty())
	{
		//Load the event on the top of the stack
		Event currentEvent = eventStack.top();
		eventStack.pop();
		//Checking if it is a valid event
		Intersection* currentIntersection = currentEvent.getIntersection();
		if(currentIntersection != NULL)
		{
			//Running the event and getting the events to reschedule
			vector<Event> toReschedule = currentIntersection->runEvent(currentEvent,time,dt);
			//Rescheduling the events that needs to be
			for(vector<Event>::iterator it = toReschedule.begin();it != toReschedule.end();it++)
				if(it->getType() != nullEvent) //If returned event is not null
					eventStack.push(*it); //Rescheduling it
		}
	}
	time += dt; //When timestep is over, we go to the next time step
}

void EventManager::runSimulation(RoadNetwork* rn, CarManager* cs)
{
	rn->intersectionSpeak();
	cs->speak();
	for(int i=0;i<numberOfSteps;i++)
	{
		cout << "Timestep : " << i << endl;
		setupTimeStep(rn,cs);
		runTimeStep(rn);
		//rn->intersectionSpeak();
		//rn->roadSpeak();
		//cs->speak();
	}
}