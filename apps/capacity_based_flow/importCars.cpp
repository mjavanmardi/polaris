#include "importCars.h"

using namespace std;


Car importCar(int enteringTime, int origin, int destination, int carNumber, vector<int> location, vector<int> nodeA, vector<int> nodeB) {
	bool fake = false;
	int number = carNumber;
	double carLength = 4;				// Default Value
	int reacDuration = 1;				// Default Value
	double maximumAcceleration = 2;		// Default Value
	int _enteringTime = enteringTime;
	int enteringNodeA;
	int enteringNodeB;
	int exitingNode;
	bool a = false;
	bool b = false;
	for(int i = 0 ; i < location.size() ; i++) {
		if(origin == location[i]) {
			enteringNodeA = nodeA[i];
			enteringNodeB = nodeB[i];
			a = true;
		}
		if(destination == location[i]) {
			exitingNode = nodeB[i];
			b = true;
		}
		if(a && b)
			break;
	}
	//### NEED TO ADD enteringNodeA && enteringNodeB
	Car C(fake, carNumber, carLength, reacDuration, enteringTime, maximumAcceleration);
	return C;
}

void preprocessLocations(result<polaris::io::Location> locations, vector<int>& location, vector<int>& nodeA, vector<int>& nodeB) {
	for(result<polaris::io::Location>::iterator it = locations.begin() ; it != locations.end() ; it++) {
		location.push_back(it->getLocation());
		Link L = *it->getLink();
		if(it->getDir() == 0) {
			nodeA.push_back(L.getNode_A()->getNode());
			nodeB.push_back(L.getNode_B()->getNode());
		}
		else {
			nodeA.push_back(L.getNode_B()->getNode());
			nodeB.push_back(L.getNode_B()->getNode());
		}
	}
}

std::vector<Car> openCars(char *db_path) {
//### Opening and transaction to the SQLite database ###	
	unique_ptr<odb::database> db;
	db = open_sqlite_database(db_path);
	odb::transaction t(db->begin());

//### Query to the SQLite database ###
	result<polaris::io::Trip> trips = db->template query<polaris::io::Trip>(query<polaris::io::Trip>::true_expr);
	result<polaris::io::Location> locations = db->template query<polaris::io::Location>(query<polaris::io::Location>::true_expr);
	vector<int> location, nodeA, nodeB;
	preprocessLocations(locations, location, nodeA, nodeB);

//### Cars initialization & implementation ###
	vector<Car> Cars;
	int number = 1;
	for(result<polaris::io::Trip>::iterator it = trips.begin() ; it != trips.end() ; it++) {
		int enteringTime = it->getStart();
		int origin = it->getOrigin();
		int destination = it->getDestination(); 
		Car C = importCar(enteringTime, origin, destination, number, location, nodeA, nodeB);
		Cars.push_back(C);
		number += 1;
	}
	
	return Cars;
}