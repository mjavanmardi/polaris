#pragma once

#include <map>

#include "Road.h"

class RoadNetwork
{
public:
	RoadNetwork();
	~RoadNetwork();
	void roadSpeak();
	void intersectionSpeak();
	void insertCarInNetwork(Car* car);

	Road* getRoad(int id);
	std::vector<Road*>* getNetwork();
	std::vector<Intersection*>* getIntersections();
private:
	std::vector<Road*> network;
	std::vector<Intersection*> intersections;
	
	std::map<int,Road*> idToRoad;
};