#include "totravarea.h"

using namespace std;


vector<MovingCars> selectCars(int j, int k, vector<MovingCars> Cars) {
	//### REMINDER ### vector<vector<int>>Cars : Node i . Node j . Queue ID . nextNode(Car[0]) . nextNode(Car[1]) . nextNode(Car[2]) ...
	vector<MovingCars> selectedCars;
	for(vector<MovingCars>::iterator it = Cars.begin() ; it != Cars.end() ; it++) {
			if((it->getNodeB() == j && it->getNextNode(0) == k) || it->getNextNode(0) == -999) {
			//if(it->at(1) == j && it->at(3) == k) {		//If the car comes from R(.,j) && goes to k
				//tuple<int,int,int> newLine(it->at(0),it->at(2),it->at(3));// Node where the car comes from i [R(i,.)], Queue ID of the Road(i,j), Road ID
				selectedCars.push_back(*it);
			}
	}
	random_shuffle(selectedCars.begin(),selectedCars.end()); //sample Cars
	return selectedCars;
	//Returns the cars going to Road(j,k) within the vector selectedCars <Node i, Queue ID>
}

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

//Has a problem : Roads size changes at some point in this function !
void releaseCars(Road& R, map<int, Road>& Roads, vector<MovingCars> sampledCars, vector<pair<int,int>>& stuckCars, vector<pair<int,int>>& movingCars, int timestep, vector<vector<int > > &nodesToID) {
	//### REMINDER sampledCars < Node i , Queue ID >
	for(vector<MovingCars>::iterator it = sampledCars.begin() ; it != sampledCars.end() ; it ++) {
		int nodei = it->getNodeA(); //Node where the car comes from
		int nodej = it->getNodeB();
		int queueID = it->getQueueID();
		int roadID = it->getRoadID();

		if (!Roads.count(roadID))
			cout << "pb : " << "nodei " << nodei << "    R.nodeA " << R.nodeA() << endl;
		// RELEASE
		pair<int,int> currentCar(nodei,queueID);
		if(Roads[roadID].getQueue(queueID).getQueue().size() != 0) 
		{
			if(Roads[roadID].getQueue(queueID).getQueue()[0].existence() == false)	// First Car is fake
			{
				Roads[roadID].moveFakeCars(queueID, timestep);			// Move Fake Car
				movingCars.push_back(currentCar);
			}
			else 
			{																		// First Car is not fake
				if(R.getMaxCommonQueueLength() > R.getCommonQueueLength() + (R.getTA().size()/R.indivQueues().size())) // Check if there is room left
				{
					Car movingCar = Roads[roadID].getQueue(queueID).getQueue()[0];				// Get the first car of the previous road at the right queue
					R.addCarToTA(movingCar);													// Add the car to the next Traveling Area
					Roads[roadID].removeCarFromQueue(queueID, timestep); 						// Remove the car from the previous road ; Already includes fake car moving
					movingCars.push_back(currentCar);
				}
				else
				{
					stuckCars.push_back(currentCar);
				}
			}
		}
		else
			stuckCars.push_back(currentCar);
	}
}

void deleteStuckCars(MovingCars& car, vector<pair<int,int>> stuckCars, bool& stuck) {
	for(vector<pair<int,int>>::iterator it = stuckCars.begin() ; it != stuckCars.end() ; it++) {
		if(it->first == car.getNodeA() && it->second == car.getQueueID() && car.getNextNodesSize() > 0) {	//Node i and Queue ID of car correspond to the moving car
			stuck = true;
			break;
		}
	}
}

void deleteMovingCars(MovingCars& car, vector<pair<int,int>> movingCars, bool& moving) {
	for(vector<pair<int,int>>::iterator it = movingCars.begin() ; it != movingCars.end() ; it++) {
		if(it->first == car.getNodeA() && it->second == car.getQueueID() && car.getNextNodesSize() > 0) {	//Node i and Queue ID of car correspond to the moving car
			car.removeOneCar();
			moving = true;
			break;
		}
	}
}

void deleteCars(vector<MovingCars>& Cars,	vector<pair<int, int>> movingCars, vector< pair< int, int>> stuckCars, int nodej) {
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
					if(it->getNextNodesSize() == 4) {
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
		//cout << "Cars size : " << Cars.size() << endl;
 		for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
			//### REMINDER ### vector<vector<int>>Cars : Only the cars that are allowed to cross an intersection based on the capacity.
			//### REMINDER ### vector<vector<int>>Cars : Node i . Node j . Queue ID . Road ID . nextNode(Car[0]) . nextNode(Car[1]) . nextNode(Car[2]) ...

			//### Select first cars going to R(j,k) ; selectedCars < Node i . Queue ID >
			vector<MovingCars> selectedCars = selectCars(it->second.nodeA(), it->second.nodeB(), Cars);
			//cout << "Cars 1 size : " << Cars.size() << endl << "Selected cars size : " << selectedCars.size() << endl;

			/*for(int i=0;i<Cars.size();i++)
			{
				for(int j=0;j<Cars[i].size();j++)
					cout << Cars[i][j] << " " ;
				cout << endl;
			}
			cout << endl;*/

			//### Sample the cars selected previously
			//sampleCars(selectedCars);

			//### Release cars && Store the moving & non-moving cars
			vector<pair<int,int>> stuckCars;	//Cars that didn't moved because of the storage
			vector<pair<int,int>> movingCars;	//Cars that dit moved thanks to the storage
			//start = clock();
			releaseCars(it->second, Roads, selectedCars, stuckCars, movingCars, timestep, nodesToID);
			/*if(test)
			{
				cout << "Second number of roads : " << Roads.size() << endl;
				test = false;
			}*/
			//timeSpent["Release cars"] += (clock()-start) / (double)(CLOCKS_PER_SEC);

			//### From vector<vector<int>> Cars, delete lines where the first car cannot cross 
			//###								&&
			//###		Decrease NumberOfCarsFromThisQueue -> If 0 -> delete the line
			deleteCars(Cars, movingCars, stuckCars, it->second.nodeA());
			//cout << "Cars 2 size : " << Cars.size() << endl;
		}
	}
}