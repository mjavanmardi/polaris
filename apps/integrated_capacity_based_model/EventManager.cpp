#include "EventManager.h"

using namespace std;

typedef vector<Intersection*>::iterator interIt;
typedef vector<Car*>::iterator carIt;

EventManager::EventManager(double _dt, int _numberOfSteps, RoadNetwork* _rn, CarManager* _cs) :
dt(_dt), time(0), numberOfSteps(_numberOfSteps), rn(_rn), cs(_cs)
{
	outputResults = vector< vector <double> >(0, vector<double>(0));

	//Adding the cars to their initial intersection in the road network
	cs->addCarsToRoadNetWork(rn);
}

EventManager::~EventManager()
{

}

void EventManager::setupTimeStep()
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
		(*it)->setCapacities(rn->getNetworkCapacities());
	}
	//### Setup cars ###
	vector<Car*>* cars = cs->getCars();
	for(carIt it = cars->begin();it != cars->end();it++)
	{
		(*it)->initTimeStep(dt);
	}
	//rn->printCapacities();
}


void EventManager::runTimeStep()
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
}

void EventManager::run()
{
	for(int i=0;i<numberOfSteps;i++)
	{
		if(time == 6)
			cout << "debug : " << endl;
		if(!(i%10))
			cout << "Timestep : " << i << endl;
		setupTimeStep();
		runTimeStep();
		cs->writeOutput(dt*double(i),outputResults);
		time += dt; //When timestep is over, we go to the next time step
		//rn->intersectionSpeak();
		//rn->roadSpeak();
		//cs->speak();
	}
	//Writing output
	cout << "Writing output" << endl;
	ofstream outputFlow("output.txt");
	for(int i=0;i<outputResults.size();i++)
	{
		for(int j=0;j<outputResults[i].size();j++)
			outputFlow << outputResults[i][j] << " ; " ;
		outputFlow << endl;
	}
}