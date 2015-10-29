#pragma once

#include <stack>

#include "RoadNetwork.h"
#include "CarManager.h"

class EventManager
{
public:
	EventManager(double _dt, int _numberOfSteps);
	~EventManager();

	void setupTimeStep(RoadNetwork* rn, CarManager* cs);
	void runTimeStep(RoadNetwork* rn);
	void runSimulation(RoadNetwork* rn, CarManager* cs);
private:
	std::stack<Event> eventStack;
	const double dt; //timestep
	const int numberOfSteps;
	double time; // Current simulation time
};