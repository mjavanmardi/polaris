#pragma once

#include <map>

#include "Road.h"

class RoadNetwork
{
public:
	RoadNetwork();
	~RoadNetwork();
	void setupNodesToRoad();
	void roadSpeak();
	void intersectionSpeak();
	void insertCarInNetwork(Car* car);

	std::vector<Road*>* getNetwork();
	std::vector<Intersection*>* getIntersections();
private:
	std::vector<Road*> network;
	std::vector<Intersection*> intersections;
	std::vector< std::vector <Road*> > nodesToRoad;
};