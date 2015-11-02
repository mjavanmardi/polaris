#pragma once

#include <map>

#include "Road.h"

class RoadNetwork
{
public:
	RoadNetwork();
	~RoadNetwork();
	void setupNodesToRoad();
	void setupCapacities();
	void roadSpeak();
	void intersectionSpeak();
	void insertCarInNetwork(Car* car);

	std::vector<Road*>* getNetwork();
	std::vector<Intersection*>* getIntersections();
	std::vector< std::vector < std::map <int,double> > >* getNetworkCapacities();
	void printCapacities();
private:
	std::vector<Road*> network;
	std::vector<Intersection*> intersections;
	std::vector< std::vector <Road*> > nodesToRoad;
	std::vector< std::vector < std::map <int,double> > > capacity;
};