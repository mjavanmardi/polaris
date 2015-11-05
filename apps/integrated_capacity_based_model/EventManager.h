#pragma once

#include <stack>
#include <cstdlib>

#include "RoadNetwork.h"
#include "CarManager.h"

class EventManager
{
public:
	EventManager(double _dt, int _numberOfSteps, RoadNetwork* _rn, CarManager* _cs);
	~EventManager();

	void setupTimeStep();
	void runTimeStep();
	void run();
private:
	std::stack<Event> eventStack;
	const double dt; //timestep
	const int numberOfSteps;
	double time; // Current simulation time
	RoadNetwork* rn;
	CarManager* cs;
	std::vector< std::vector <double > > outputResults;
};