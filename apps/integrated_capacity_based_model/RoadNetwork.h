#pragma once

#include <map>
#include "Polaris_PCH.h"
#include "Road.h"

//Reminder : capacity[roadId][laneNumber][nextRoadId] = (capacityAtTheBeginingOfTimeStep,capacityLeft)
typedef std::vector< std::vector < std::map <int,std::pair<double,double> > > > Capacity;

class RoadNetwork
{
public:
	RoadNetwork();
	RoadNetwork(std::unique_ptr<odb::database>);
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
	std::vector<Road*> network; //contain links
	std::vector<Intersection*> intersections; //contain intersection
	//this is an ajencency matrix that maps node ids (i,j) to the link pointer
	std::vector< std::vector <Road*> > nodesToRoad; //this structure, need to be changed, we cannot support support, for a 30K node road networ, we get close to memory capacity
	Capacity capacity;
};