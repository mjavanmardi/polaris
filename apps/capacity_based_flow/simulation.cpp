#include "simulation.h"

using namespace std;



void simulation(int modelisationTime, map<int, Road>& Roads, vector<Car>& Cars, int timestep, vector<int> timestepsToPrint, bool q, int superior) {
	for(int t = 0 ; t < modelisationTime ; t++) {
	
		if(t>=10) {
				cout << endl << t;
				cout << endl;
		}

	//### Add new cars into the system
		addNewCars(Cars, Roads, t, timestep);

	//### Move from common queue & Increase cars progression & Write the queue length at time = t &  Store cars that can exit a road to enter a new one BASED ON THE CAPACITY 
		vector<vector<int>> movingCars = preProcess(Roads, timestep);

	//### Release cars from traveling areas into queues (Common or Individual)
		travelingAreaToQueues(Roads, timestep);

	//### Release cars from queues to traveling areas -> cars stored previously (Based on capacity) 
		queuesToTravelingAreas(Roads, movingCars, timestep);

	//### Display the network state if needed
		printRoads(t, Roads, timestepsToPrint, q, superior);
	}
}