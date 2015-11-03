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
	vector<pair<int,TurningMovementType>> tm_0_0_0;
	tm_0_0_0.push_back(pair<int,TurningMovementType>(3,Left));
	IndividualQueue iq00(10., tm_0_0_0);
	vector<pair<int,TurningMovementType>> tm_0_1_0;
	tm_0_1_0.push_back(pair<int,TurningMovementType>(5,Thru));
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
	vector<pair<int,TurningMovementType>> tm_1_0_0;
	tm_1_0_0.push_back(pair<int,TurningMovementType>(-1,Thru));
	IndividualQueue iq10(10.,tm_1_0_0);
	iq = vector<vector<IndividualQueue>>();
	temp = vector<IndividualQueue>();
	temp.push_back(iq10);
	iq.push_back(temp);
	JunctionArea* ja1 = new JunctionArea(iq);

	//ja2
	vector<pair<int,TurningMovementType>> tm_2_0_0;
	tm_2_0_0.push_back(pair<int,TurningMovementType>(5,Left));
	IndividualQueue iq20(10., tm_2_0_0);
	vector<pair<int,TurningMovementType>> tm_2_1_0;
	tm_2_1_0.push_back(pair<int,TurningMovementType>(-1,Right));
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
	vector<pair<int,TurningMovementType>> tm_3_0_0;
	tm_3_0_0.push_back(pair<int,TurningMovementType>(-1,Thru));
	IndividualQueue iq30(10., tm_3_0_0);
	iq = vector<vector<IndividualQueue>>();
	temp = vector<IndividualQueue>();
	temp.push_back(iq30);
	iq.push_back(temp);
	JunctionArea* ja3 = new JunctionArea(iq);

	//ja4
	vector<pair<int,TurningMovementType>> tm_4_0_0;
	tm_4_0_0.push_back(pair<int,TurningMovementType>(1,Thru));
	IndividualQueue iq40(10., tm_4_0_0);
	vector<pair<int,TurningMovementType>> tm_4_1_0;
	tm_4_1_0.push_back(pair<int,TurningMovementType>(1,Thru));
	tm_4_1_0.push_back(pair<int,TurningMovementType>(3,Right));
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
	vector<pair<int,TurningMovementType>> tm_5_0_0;
	tm_5_0_0.push_back(pair<int,TurningMovementType>(-1,Thru));
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
	Road* r0 = new Road(0,ta0,cq0,ja0,i1,i0,100.,13.9,&nodesToRoad,&capacity);
	Road* r1 = new Road(1,ta1,cq1,ja1,i0,i1,100.,13.9,&nodesToRoad,&capacity);
	Road* r2 = new Road(2,ta2,cq2,ja2,i2,i0,100.,13.9,&nodesToRoad,&capacity);
	Road* r3 = new Road(3,ta3,cq3,ja3,i0,i2,100.,13.9,&nodesToRoad,&capacity);
	Road* r4 = new Road(4,ta4,cq4,ja4,i3,i0,100.,13.9,&nodesToRoad,&capacity);
	Road* r5 = new Road(5,ta5,cq5,ja5,i0,i3,100.,13.9,&nodesToRoad,&capacity);
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

	//### Building the capacity structure
	setupCapacities();
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

void RoadNetwork::setupCapacities()
{
	//We get the maximum road id
	int maxRoadId = 0;
	for(roadIt it = network.begin();it != network.end();it++)
	{
		const int& currentRoadId = (*it)->getId();
		if(currentRoadId > maxRoadId)
			maxRoadId = currentRoadId;
	}
	//We create the corresponding capacity structure :
	//capacity[road][lane][turningMovement].first = capacity at the beginning timestep
	//capacity[road][lane][turningMovement].second = capacity left
	capacity = Capacity(maxRoadId+1, vector < map<int,pair<double,double> > >(0));
	
	for(roadIt it = network.begin();it != network.end();it++)
	{
		vector < map<int,pair<double,double> > > currentRoad = vector < map<int,pair<double,double> > >(0);
		JunctionArea* currentJA = (*it)->getJA();
		for(int i=0;i<currentJA->getNbLanes();i++)
		{
			vector <pair<int,TurningMovementType> > currentTurningMovements = currentJA->getTurningMovements(i);
			map<int,pair<double,double> > currentLane = map<int,pair<double,double> >();
			for(int j=0;j<currentTurningMovements.size();j++)
			{
				currentLane[currentTurningMovements[j].first] = pair<double,double>(0,0); //capacity initiated at 0
			}
			currentRoad.push_back(currentLane);
		}
		capacity[(*it)->getId()] = currentRoad;
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

void RoadNetwork::printCapacities()
{
	cout << "Printing the capacities" << endl ;
	for(int i=0;i<capacity.size();i++)
	{
		cout << "  Road : " << i << endl;
		for(int j=0;j<capacity[i].size();j++)
		{
			cout << "    Lane : " << j << endl;
			for(map<int,pair<double,double> >::iterator it = capacity[i][j].begin() ; it != capacity[i][j].end();it++)
			{
				cout << "      Turning movement to road : " << it->first << " has capacity left " << it->second.second << endl;
			}
			cout << endl;
		}
		cout << endl << endl;
	}
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

vector< vector < map <int,pair<double,double> > > >* RoadNetwork::getNetworkCapacities()
{
	return &capacity;
}