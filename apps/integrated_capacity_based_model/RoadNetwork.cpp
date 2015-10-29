#include "RoadNetwork.h"

using namespace std;

typedef vector<Intersection*>::iterator interIt;
typedef vector<Road*>::iterator roadIt;

RoadNetwork::RoadNetwork()
{
	// ### TEST NETWORK ### //

	// ### Building TravelingAreas ###
	TravelingArea* ta0 = new TravelingArea(2);
	TravelingArea* ta1 = new TravelingArea(1);
	TravelingArea* ta2 = new TravelingArea(2);
	TravelingArea* ta3 = new TravelingArea(1);
	TravelingArea* ta4 = new TravelingArea(2);
	TravelingArea* ta5 = new TravelingArea(1);


	// ### Building CommonQueue ###
	CommonQueue* cq0 = new CommonQueue(0.1,2);
	CommonQueue* cq1 = new CommonQueue(0.1,1);
	CommonQueue* cq2 = new CommonQueue(0.1,2);
	CommonQueue* cq3 = new CommonQueue(0.1,1);
	CommonQueue* cq4 = new CommonQueue(0.1,2);
	CommonQueue* cq5 = new CommonQueue(0.1,1);

	// ### Building JunctionArea ###

	//ja0
	vector<int> tm_0_0_0;
	tm_0_0_0.push_back(3);
	IndividualQueue iq00(10., tm_0_0_0);
	vector<int> tm_0_1_0;
	tm_0_1_0.push_back(5);
	IndividualQueue iq01(10., tm_0_1_0);
	vector<vector<IndividualQueue>> iq;
	vector<IndividualQueue> temp;
	temp.push_back(iq00);
	iq.push_back(temp);
	temp.erase(temp.begin());
	temp.push_back(iq01);
	iq.push_back(temp);
	JunctionArea* ja0 = new JunctionArea(iq);

	//ja1
	vector<int> tm_1_0_0;
	tm_1_0_0.push_back(-1);
	IndividualQueue iq10(10.,tm_1_0_0);
	iq = vector<vector<IndividualQueue>>();
	temp = vector<IndividualQueue>();
	temp.push_back(iq10);
	iq.push_back(temp);
	JunctionArea* ja1 = new JunctionArea(iq);

	//ja2
	vector<int> tm_2_0_0;
	tm_2_0_0.push_back(5);
	IndividualQueue iq20(10., tm_2_0_0);
	vector<int> tm_2_1_0;
	tm_2_1_0.push_back(1);
	IndividualQueue iq21(10., tm_2_1_0);
	iq = vector<vector<IndividualQueue>>();
	temp = vector<IndividualQueue>();
	temp.push_back(iq20);
	iq.push_back(temp);
	temp.erase(temp.begin());
	temp.push_back(iq21);
	iq.push_back(temp);
	JunctionArea* ja2 = new JunctionArea(iq);

	//ja3
	vector<int> tm_3_0_0;
	tm_3_0_0.push_back(-1);
	IndividualQueue iq30(10., tm_3_0_0);
	iq = vector<vector<IndividualQueue>>();
	temp = vector<IndividualQueue>();
	temp.push_back(iq30);
	iq.push_back(temp);
	JunctionArea* ja3 = new JunctionArea(iq);

	//ja4
	vector<int> tm_4_0_0;
	tm_4_0_0.push_back(1);
	IndividualQueue iq40(10., tm_4_0_0);
	vector<int> tm_4_1_0;
	tm_4_1_0.push_back(1);
	tm_4_1_0.push_back(3);
	IndividualQueue iq41(10., tm_4_1_0);
	iq = vector<vector<IndividualQueue>>();
	temp = vector<IndividualQueue>();
	temp.push_back(iq40);
	iq.push_back(temp);
	temp.erase(temp.begin());
	temp.push_back(iq41);
	iq.push_back(temp);
	JunctionArea* ja4 = new JunctionArea(iq);

	//ja5
	vector<int> tm_5_0_0;
	tm_5_0_0.push_back(-1);
	IndividualQueue iq50(10., tm_5_0_0);
	iq = vector<vector<IndividualQueue>>();
	temp = vector<IndividualQueue>();
	temp.push_back(iq50);
	iq.push_back(temp);
	JunctionArea* ja5 = new JunctionArea(iq);

	//### Building the intersections ###
	Intersection* i0 = new Intersection(0);
	Intersection* i1 = new Intersection(1);
	Intersection* i2 = new Intersection(2);
	Intersection* i3 = new Intersection(3);
	intersections.push_back(i0);
	intersections.push_back(i1);
	intersections.push_back(i2);
	intersections.push_back(i3);

	//### Building the roads ###
	Road* r0 = new Road(0,ta0,cq0,ja0,i1,i0,100.,13.9,&nodesToRoad);
	Road* r1 = new Road(1,ta1,cq1,ja1,i0,i1,100.,13.9,&nodesToRoad);
	Road* r2 = new Road(2,ta2,cq2,ja2,i2,i0,100.,13.9,&nodesToRoad);
	Road* r3 = new Road(3,ta3,cq3,ja3,i0,i2,100.,13.9,&nodesToRoad);
	Road* r4 = new Road(4,ta4,cq4,ja4,i3,i0,100.,13.9,&nodesToRoad);
	Road* r5 = new Road(5,ta5,cq5,ja5,i0,i3,100.,13.9,&nodesToRoad);
	network.push_back(r0);
	network.push_back(r1);
	network.push_back(r2);
	network.push_back(r3);
	network.push_back(r4);
	network.push_back(r5);

	//### Building the links ###

	i0->addEnteringRoad(r0);
	i1->addOutgoingRoad(r0);
	i0->addOutgoingRoad(r1);
	i1->addEnteringRoad(r1);
	i0->addEnteringRoad(r2);
	i2->addOutgoingRoad(r2);
	i0->addOutgoingRoad(r3);
	i2->addEnteringRoad(r3);
	i0->addEnteringRoad(r4);
	i3->addOutgoingRoad(r4);
	i0->addOutgoingRoad(r5);
	i3->addEnteringRoad(r5);

	//### END OF TEST NETWORK ###

	//### Building the nodesToRoad matrix ###
	setupNodesToRoad();
}

RoadNetwork::~RoadNetwork()
{
	for(int i=0;i<network.size();i++)
		delete network[i];
	for(int j=0;j<intersections.size();j++)
		delete intersections[j];
}

void RoadNetwork::setupNodesToRoad()
{
	int maxRoadIndex = -1;
	for(interIt it = intersections.begin(); it != intersections.end();it++)
	{
		if ((*it)->getId() > maxRoadIndex)
			maxRoadIndex = (*it)->getId();
	}
	nodesToRoad = vector < vector <Road*> >(maxRoadIndex+1,vector<Road*>(maxRoadIndex+1,NULL));
	for(roadIt it = network.begin(); it != network.end();it++)
	{
		nodesToRoad[(*it)->getNodeA()][(*it)->getNodeB()] = (*it);
	}

}

void RoadNetwork::roadSpeak()
{
	for(int i=0;i<network.size();i++)
	{
		network[i]->speak();
	}
	cout << endl << endl;
}

void RoadNetwork::intersectionSpeak()
{
	for(int i=0;i<intersections.size();i++)
	{
		intersections[i]->speak();
	}
	cout << endl << endl;
}

void RoadNetwork::insertCarInNetwork(Car* car)
{
	for(vector<Intersection*>::iterator it = intersections.begin(); it != intersections.end();it++)
	{
		if((*it)->getId()==car->getEntryNode())
		{
			Road* enteringRoad =nodesToRoad[car->getEntryNode()][car->getNextNode()];
			(*it)->addEnteringCar(car,enteringRoad);
		}
	}
}

vector<Road*>* RoadNetwork::getNetwork()
{
	return (&network);
}

vector<Intersection*>* RoadNetwork::getIntersections()
{
	return (&intersections);
}