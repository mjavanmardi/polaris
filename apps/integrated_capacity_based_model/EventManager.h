#pragma once

#include <stack>

#include "RoadNetwork.h"
#include "CarManager.h"

class EventManager
{
public:
	EventManager(double _dt);
	~EventManager();

	void setupTimeStep(RoadNetwork* rn, CarManager* cs);
private:
	std::stack<Event> eventStack;
	const double dt; //timestep
};