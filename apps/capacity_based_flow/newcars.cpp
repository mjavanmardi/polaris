#include "newcars.h"

using namespace std;

int selectEnteringRoad(map<int, Road>& Roads, int enterNodeA, int enterNodeB) {
	// The function the nodeA / nodeB data (coming from the SQLite database) to the road ID (Data structure used for the C++ this model)
			
	int roadID = 0;
	for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
		if(it->second.nodeA() == enterNodeA && it->second.nodeB() == enterNodeB) {
			roadID = it->first;
			break;
		}
	}
	return roadID;
}

void addNewCars(vector<Car>& Cars, map<int, Road>& Roads, int t, int timestep, vector<vector<int>> &nodesToID) {
	vector<int> toBeErased;
	int iter = 0;
	for(vector<Car>::iterator& it = Cars.begin() ; it != Cars.end() ; it++) {
		if(it->enteringTime() >= t && it->enteringTime() < t + timestep) {
			int roadID = selectEnteringRoad(Roads, it->enteringNodeA(), it->enteringNodeB());  // The function the nodeA / nodeB data (coming from the SQLite database) to the road ID (Data structure used for the C++ this model)
			//int roadID = nodesToID[it->enteringNodeA()][it->enteringNodeB()];
			if(roadID == 0)
				cout << endl << "There is a problem with the entering Road ; the queue ID shoudln't be 0";

			// "if" to check it there is room left on the road : Max Length > Common Queue + (Cars in the Traveling Area/# of lanes)
			if(Roads[roadID].getMaxCommonQueueLength() > Roads[roadID].getCommonQueueLength() + (Roads[roadID].getTA().size()/Roads[roadID].indivQueues().size())) {
				Roads[roadID].addCarToTA((*it));
				toBeErased.push_back(iter);
			}
			// "else" -> Not enought space to release the car in the network. The car entering time is being postponed
			else 
				it->postponedEnteringTime(timestep);
		}
		//The "else" statement is made possible because the cars have been sorted according to their entering time (see "carsPath.cpp"). 
		//If a car entering time is bigger that t+timestep, so all the next cars have a entering time bigger than that.
		else{
			break;
		}
		iter += 1;

	}
	for(int i = toBeErased.size() ; i>0 ; i--) {
		Cars.erase(Cars.begin() + toBeErased.at(i-1));
	} 
}