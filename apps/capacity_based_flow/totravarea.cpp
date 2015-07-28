#include "totravarea.h"

using namespace std;


vector<pair<int,int>> selectCars(int j, int k, vector<vector<int>> Cars) {
	vector<pair<int,int>> selectedCars;
	for(vector<vector<int>>::iterator it = Cars.begin() ; it != Cars.end() ; it++) {
		if(it->at(1) == j && it->at(3) == k) {		//If the car comes from R(.,j) && goes to k
			pair<int, int> newLine;
			newLine.first = it->at(0);	// Node where the car come from i [R(i,.)]
			newLine.second = it->at(2);	// Queue ID of the Road(i,j)
			selectedCars.push_back(newLine);
		}
	}
	return selectedCars;
}

void sampleCars(vector<pair<int,int>>& selectedCars) {
	vector<pair<int, int>> sampledCars;
	int size = selectedCars.size();
	int iter = 0;
	while(sampledCars.size() != size) {
		int number = rand()%(size-iter);
		sampledCars.push_back(selectedCars[number]);
		selectedCars.erase(selectedCars.begin() + number);
		iter += 1;
	}
	selectedCars = sampledCars;
}

int selectPreviousRoad(map<int, Road> Roads, int i, int j) {
	for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
		if(it->second.nodeA() == i && it->second.nodeB() == j) 
			return it->first;
	}
}

void releaseCars(Road& R, map<int, Road>& Roads, vector<pair<int,int>> sampledCars, vector<pair<int,int>>& stuckCars, vector<pair<int,int>>& movingCars, int timestep) {
	//### REMINDER sampledCars < Node i , Queue ID >
	for(vector<pair<int, int>>::iterator it = sampledCars.begin() ; it != sampledCars.end() ; it ++) {
		const int nodei = it ->first;
		const int queueID = it->second;
		int roadID = selectPreviousRoad(Roads, nodei, R.nodeA());
		if(Roads[roadID].getQueue(queueID).getQueue().size() != 0) {
			if(Roads[roadID].getQueue(queueID).getQueue()[0].existence() == false)	{	// First Car is fake
				Roads[roadID].moveFakeCars(queueID, timestep);			// Move Fake Car
				movingCars.push_back((*it));
			}
			else {																		// First Car is not fake
				if(R.getMaxCommonQueueLength() > R.getCommonQueueLength()) {				// Check if there is room left
					Car movingCar = Roads[roadID].getQueue(queueID).getQueue()[0];				// Get the first car of the previous road at the right queue
					R.addCarToTA(movingCar);													// Add the car to the next Traveling Area
					Roads[roadID].removeCarFromQueue(queueID, timestep); 						// Remove the car from the previous road ; Already includes fake car moving
					movingCars.push_back((*it));
				}
				else
					stuckCars.push_back((*it));
			}
		}
	}
}

void deleteStuckCars(vector<int>& car, vector<pair<int,int>> stuckCars, bool& stuck) {
	for(vector<pair<int,int>>::iterator it = stuckCars.begin() ; it != stuckCars.end() ; it++) {
		if(it->first == car[0] && it->second == car[1]) {	//Node i and Queue ID of car correspond to the moving car
			stuck = true;
			break;
		}
	}
}

void deleteMovingCars(vector<int>& car, vector<pair<int,int>> movingCars, bool& moving) {
	for(vector<pair<int,int>>::iterator it = movingCars.begin() ; it != movingCars.end() ; it++) {
		if(it->first == car[0] && it->second == car[2]) {	//Node i and Queue ID of car correspond to the moving car
			car.erase(car.begin() + 3);
			moving = true;
			break;
		}
	}


}

void deleteCars(vector<vector<int>>& Cars,	vector<pair<int, int>> movingCars, vector< pair< int, int>> stuckCars, int nodej) {
	for(vector< vector<int> >::iterator it = Cars.begin() ; it != Cars.end() ;) {
		if(Cars.size() != 0) {
			if(it->at(1) == nodej) {
				bool moving = false;
				bool stuck = false;
				deleteMovingCars((*it), movingCars, moving);
				if(moving == false) 
					deleteStuckCars((*it), stuckCars, stuck);
			
				//### Increase iterator
				if(moving == true) {
					if(it->size() == 3)
						Cars.erase(it);
					else
						it++;
				}
				else if(stuck == true) {
					Cars.erase(it);
				}
				else {
					it++;
				}
				//###
			}
			else
				it++;
		}
		if(Cars.size() == 0)
			break;
	}
}

void queuesToTravelingAreas(map<int, Road>& Roads, vector<vector<int>> Cars, int timestep) {
	while(Cars.size() != 0) {
		for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
			//### REMINDER ### vector<vector<int>>Cars : Node i . Node j . Queue ID . nextNode(Car[0]) . nextNode(Car[1]) . nextNode(Car[2]) ...
		
			//### Select first cars going to R(j,k) ; selectedCars < Node i . Queue ID >
			vector<pair<int, int>> selectedCars = selectCars(it->second.nodeA(), it->second.nodeB(), Cars);

			//### Sample the cars selected previously
			sampleCars(selectedCars);

			//### Release cars && Store the moving & non-moving cars
			vector<pair<int,int>> stuckCars;	//Cars that didn't moved because of the storage
			vector<pair<int,int>> movingCars;	//Cars that dit moved thanks to the storage
			releaseCars(it->second, Roads, selectedCars, stuckCars, movingCars, timestep);

			//### From vector<vector<int>> Cars, delete lines where the first car cannot cross 
			//###								&&
			//###		Decrease NumberOfCarsFromThisQueue -> If 0 -> delete the line
			deleteCars(Cars, movingCars, stuckCars, it->second.nodeA());
		}
	}
}