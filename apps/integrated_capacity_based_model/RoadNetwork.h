#pragma once

#include <map>

#include "Road.h"

//Reminder : capacity[roadId][laneNumber][nextRoadId] = (capacityAtTheBeginingOfTimeStep,capacityLeft)
typedef std::vector< std::vector < std::map <int,std::pair<double,double> > > > Capacity;

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
	Capacity* getNetworkCapacities();
	void printCapacities();
private:
	std::vector<Road*> network;
	std::vector<Intersection*> intersections;
	std::vector< std::vector <Road*> > nodesToRoad;
	Capacity capacity;
};