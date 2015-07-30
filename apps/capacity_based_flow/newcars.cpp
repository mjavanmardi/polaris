#include "newcars.h"

using namespace std;

int selectEnteringRoad(map<int, Road>& Roads, int enterNodeA, int enterNodeB) {
	int roadID = 0;
	map<int, Road> roads = Roads;
	for(map<int, Road>::iterator it = roads.begin() ; it != roads.end() ; it++) {
		if(it->second.nodeA() == enterNodeA && it->second.nodeB() == enterNodeB) {
			roadID = it->first;
			break;
		}
	}
	return roadID;
}

void addNewCars(vector<Car>& Cars, map<int, Road>& Roads, int t, int timestep) {
	vector<int> toBeErased;
	int iter = 0;
	for(vector<Car>::iterator& it = Cars.begin() ; it != Cars.end() ; it++) {
		if(it->enteringTime() > t && it->enteringTime() <= t + timestep) {
			int roadID = selectEnteringRoad(Roads, it->enteringNodeA(), it->enteringNodeB());
			if(roadID == 0)
				cout << endl << "There is a problem with the entering Road ; the queue ID shouln't be 0";
			Roads[roadID].addCarToTA((*it));
			toBeErased.push_back(iter);
		}
		else{
			
		}
		iter += 1;
	}

	for(int i = toBeErased.size() ; i>0 ; i--) {
		Cars.erase(Cars.begin() + toBeErased.at(i-1));
	}
}