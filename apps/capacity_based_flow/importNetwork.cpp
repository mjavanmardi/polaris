#include "importNetwork.h"

using namespace std;

double getCapacity(string type, double grade) {

/// ### This capacity calculation is given by the Highway Capacity Manual 2010 - Chapter 18 (Signalized Intersection), page 35 ###
	double effectiveGreenTime = 0.5;				// Default Value				
	double BSFR = 1900;								// Default Value
	double laneWidth = 1;							// Default Value
	double heavyVehicles = 1;						// Default Value			
	double approachGrade = 1-grade/200;				// !!!! WHAT IS THE VALUE/METRIC ??? ANGLE/PERCENT/... !!!
	double parkingActivity = 1;						// Default Value						
	double blockingEffect = 1;						// Default Value			
	double areaType = 1;							// Default Value	
	double laneUtilization = 1;						// Default Value		
	double leftTurn = 1;							// Default Value
	double rightTurn = 1;							// Default Value		
	double pedestrianLeftTurn = 1;					// Default Value
	double pedestrianBicyclePedestrianRightTurn = 1;// Default Value

	double capacity = effectiveGreenTime * BSFR * laneWidth * heavyVehicles * approachGrade * parkingActivity * blockingEffect * areaType * laneUtilization * rightTurn * leftTurn * pedestrianLeftTurn * pedestrianBicyclePedestrianRightTurn;

	return capacity;
}

bool verifyLane(int lane, string lanes) {
	bool q = false;
	char mini = lanes.at(0);
	int min;
	if(mini == 'R')
		min = 1;
	else
		min = lanes.at(0) - '0';
	/*cout << endl << lanes;
	cout << endl << lanes.at(0);
	cout << endl << min;*/
	int max = (int) lanes.at(lanes.size()-1);
	if(lane >= min && lane <= max)
		q = true;
	return q;
}

void importLanes(Road& R, bool q, int lane, double grade, vector<int> link, vector<int> toNodeA, vector<int> toNodeB, vector<bool> dir, vector<string> lanes, vector<string> type) {
	for(int i = 1; i < lane+1; i++) {
		map<int, double> capacities;
		
		for(int j = 0 ; j < dir.size() ; j++) {
			bool laneOK = verifyLane(i, lanes[j]);
			if(R.link() == link[j] && q == dir[j] && laneOK) {	//Verify if the same link, same direction and same lanes inside
				int toNode;
				if(toNodeA[j] == R.nodeB())
					toNode = toNodeB[j];
				else
					toNode = toNodeA[j];
				capacities[toNode] = getCapacity(type[j], grade);

			}
		}
		
		double maxLength = 30;				// Default Value
		double distanceBetweenCars = 1;		// Default Value
		R.addQueue(lane, 30, 1, capacities);
	}
}

pair<Road,Road> importRoad(polaris::io::Link db_itr, int ID, vector<int> link, vector<int> toNodeA, vector<int> toNodeB, vector<bool> dir, vector<string> lanes, vector<string> type) {
	
//### Get roads attributes ###
	int roadLink = db_itr.getLink();
	int anode = db_itr.getNode_A()->getNode();
	int bnode = db_itr.getNode_B()->getNode();
	double maxSpeed_AB = db_itr.getSpeed_Ab();
	double maxSpeed_BA = db_itr.getSpeed_Ba();
	double totalLength = db_itr.getLength() + db_itr.getSetback_A() + db_itr.getSetback_B();
	double distanceBetweenCars = (double) 1;		//Default value for the distance between cars
	
// ### Road Initialization  ###
// !!! Figure out how to return only one pair if lane = 0 !!!
	Road R1(ID, roadLink, anode, bnode, maxSpeed_AB, totalLength, distanceBetweenCars);
	Road R2(ID+1, roadLink, bnode, anode, maxSpeed_BA, totalLength, distanceBetweenCars);
	

//### Road Lanes Initialization ###
	int lanes_AB = db_itr.getLanes_Ab();
	int lanes_BA = db_itr.getLanes_Ba();
	
	importLanes(R1, 0, lanes_AB, db_itr.getGrade(), link, toNodeA, toNodeB, dir, lanes, type);		//The boolean is for the "direction" in the database. 0 = nodeA->B. 1 = nodeB->A
	importLanes(R2, 1, lanes_BA, db_itr.getGrade(), link, toNodeA, toNodeB, dir, lanes, type);



//### Return Roads ###
	pair<Road, Road> Roads(R1,R2);
	return Roads;
}

void preprocessConnections(result<polaris::io::Connect> connections, vector<int>& link, vector<int>& toNodeA, vector<int>& toNodeB, vector<bool>& dir, vector<string>& lanes, vector<string>& type) {
	for(result<polaris::io::Connect>::iterator it = connections.begin() ; it != connections.end() ; it++) {
		link.push_back((*it->getLink()).getLink());
		Link L = *it->getTo_Link();
		toNodeA.push_back(L.getNode_A()->getNode());
		toNodeB.push_back(L.getNode_B()->getNode());
		dir.push_back(it->getDir());
		lanes.push_back(it->getLanes());
		type.push_back(it->getType());
	}
}

map<int, Road> openRoad(char *db_path) {
//### Opening and transaction to the SQLite database ###
	unique_ptr<odb::database> db;
	db = open_sqlite_database(db_path);
	odb::transaction t(db->begin());


//### Query to the SQLite database ###
	result<polaris::io::Link> links = db->template query<polaris::io::Link>(query<polaris::io::Link>::true_expr);
	result<polaris::io::Connect> connections = db->template query<polaris::io::Connect>(query<polaris::io::Connect>::true_expr);
	vector<int> link, toNodeA, toNodeB;
	vector<bool> dir;
	vector<string> lanes, type;
	preprocessConnections(connections, link, toNodeA, toNodeB, dir, lanes, type);

//### Data Structure initialization & implementation ###
	std::map<int, Road> newRoads;
	int ID = 1;
	for(result<polaris::io::Link>::iterator db_itr = links.begin (); db_itr != links.end (); ++db_itr) {
		pair<Road, Road>  twoRoads = importRoad(*db_itr, ID, link, toNodeA, toNodeB, dir, lanes, type);
		newRoads[twoRoads.first.ID()] = twoRoads.first;
		newRoads[twoRoads.second.ID()] = twoRoads.second;
		ID +=2;
	}
		
	return newRoads;
}

