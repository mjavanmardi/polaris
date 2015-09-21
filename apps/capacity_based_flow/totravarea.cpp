#include "totravarea.h"

using namespace std;


vector<MovingCars> selectCars(int j, int k, vector<MovingCars> Cars) {
	//### REMINDER ### vector<vector<int>>Cars : Node i . Node j . Queue ID . nextNode(Car[0]) . nextNode(Car[1]) . nextNode(Car[2]) ...
	vector<MovingCars> selectedCars;
	for(vector<MovingCars>::iterator it = Cars.begin() ; it != Cars.end() ; it++) {
			if((it->getNodeB() == j && it->getNextNode(0) == k) || it->getNextNode(0) == -999) { //If the car comes from R(.,j) && goes to k
				selectedCars.push_back(*it);
			}
	}
	random_shuffle(selectedCars.begin(),selectedCars.end()); //sample Cars ; replaces function void sampleCars(vector<tuple<int,int,int>>& selectedCars)
	return selectedCars;
	//Returns the cars going to Road(j,k) within the vector selectedCars <Node i, Queue ID>
}

//Former function not used anymore. Kept just in case
void sampleCars(vector<tuple<int,int,int>>& selectedCars) {
	vector<tuple<int,int,int>> sampledCars;
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

void releaseCars(Road& R, map<int, Road>& Roads, vector<MovingCars> sampledCars, vector<MovingCars>& stuckCars, vector<MovingCars>& movingCars, int timestep, vector<vector<int > > &nodesToID) {
	//### REMINDER sampledCars < Node i , Queue ID >
	for(vector<MovingCars>::iterator it = sampledCars.begin() ; it != sampledCars.end() ; it ++) {
		int nodei = it->getNodeA(); //Node where the car comes from
		int nodej = it->getNodeB();
		int queueID = it->getQueueID();
		int roadID = it->getRoadID();

		if (!Roads.count(roadID))
			cout << "pb : " << "nodei " << nodei << "    R.nodeA " << R.nodeA() << endl;
		// RELEASE
		if(Roads[roadID].getQueue(queueID).getQueue().size() != 0) 
		{
			if(Roads[roadID].getQueue(queueID).getQueue()[0].existence() == false)	// First Car is fake
			{
				Roads[roadID].moveFakeCars(queueID, timestep);			// Move Fake Car
				movingCars.push_back(*it);
			}
			else 
			{																		// First Car is not fake
				if(R.getMaxCommonQueueLength() > R.getCommonQueueLength() + (R.getTA().size()/R.indivQueues().size())) // Check if there is room left
				{
					Car movingCar = Roads[roadID].getQueue(queueID).getQueue()[0];				// Get the first car of the previous road at the right queue
					R.addCarToTA(movingCar);													// Add the car to the next Traveling Area
					Roads[roadID].removeCarFromQueue(queueID, timestep); 						// Remove the car from the previous road ; Already includes fake car moving
					movingCars.push_back(*it);
				}
				else
				{
					stuckCars.push_back(*it);
				}
			}
		}
		else
			stuckCars.push_back(*it);
	}
}

void deleteStuckCars(MovingCars& car, vector<MovingCars> stuckCars, bool& stuck) {
	for(vector<MovingCars>::iterator it = stuckCars.begin() ; it != stuckCars.end() ; it++) {
		if(it->getNodeA() == car.getNodeA() && it->getQueueID() == car.getQueueID() && car.getNextNodesSize() > 0) {	//Node i and Queue ID of car correspond to the moving car
			stuck = true;
			break;
		}
	}
}

void deleteMovingCars(MovingCars& car, vector<MovingCars> movingCars, bool& moving) {
	for(vector<MovingCars>::iterator it = movingCars.begin() ; it != movingCars.end() ; it++) {
		if(it->getNodeA() == car.getNodeA() && it->getQueueID() == car.getQueueID() && car.getNextNodesSize() > 0) {	//Node i and Queue ID of car correspond to the moving car
			car.removeOneCar();
			moving = true;
			break;
		}
	}
}

void deleteCars(vector<MovingCars>& Cars,	vector<MovingCars> movingCars, vector<MovingCars> stuckCars, int nodej) {
	for(vector< MovingCars >::iterator it = Cars.begin() ; it != Cars.end() ;) {
		if(Cars.size() != 0) {
			if(it->getNodeB() == nodej) {
				bool moving = false;
				bool stuck = false;
				deleteMovingCars((*it), movingCars, moving);
				if(moving == false) 
					deleteStuckCars((*it), stuckCars, stuck);
				if(it->getNextNodesSize() == 0) 
				{
						Cars.erase(it);
						break;
				}
				//### Increase iterator
				if(moving == true) {
					if(it->getNextNodesSize() == 0) {
						Cars.erase(it);
						break;
					}
					else
						it++;
				}
				else if(stuck == true) {
					Cars.erase(it);
					break;
				}
				else 
					it++;
				//###
			}
			else 
				it++;
		}
		if(Cars.size() == 0)
			break;
	}
}

void queuesToTravelingAreas(map<int, Road>& Roads, vector<MovingCars> Cars, int timestep, vector<vector<int>> &nodesToID) {
	std::clock_t start;
	bool test = false;
	if(Roads.size()==48)
		test = true;
	while(Cars.size() != 0) 
	{
 		for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
			//### REMINDER ### vector<vector<int>>Cars : Only the cars that are allowed to cross an intersection based on the capacity.
			//### REMINDER ### vector<vector<int>>Cars : Node i . Node j . Queue ID . Road ID . nextNode(Car[0]) . nextNode(Car[1]) . nextNode(Car[2]) ...

			//### Select first cars going to R(j,k) ; selectedCars < Node i . Queue ID >
			vector<MovingCars> selectedCars = selectCars(it->second.nodeA(), it->second.nodeB(), Cars);

			//### Release cars && Store the moving & non-moving cars
			vector<MovingCars> stuckCars;	//Cars that didn't moved because of the storage
			vector<MovingCars> movingCars;	//Cars that dit moved thanks to the storage
			releaseCars(it->second, Roads, selectedCars, stuckCars, movingCars, timestep, nodesToID);

			//### From vector<vector<int>> Cars, delete lines where the first car cannot cross 
			//###								&&
			//###		Decrease NumberOfCarsFromThisQueue -> If 0 -> delete the line
			deleteCars(Cars, movingCars, stuckCars, it->second.nodeA());
		}
	}
}