#include "simulation.h"

using namespace std;



void simulation(int modelisationTime, map<int, Road>& Roads, vector<Car>& Cars, int timestep, vector<int> timestepsToPrint, int superior) {
	for(int t = 0 ; t < modelisationTime ; t++) {

		//clock_t a = clock();
	//### Add new cars into the system
		addNewCars(Cars, Roads, t, timestep);
		//clock_t b = clock();

	//### Move from common queue & Increase cars progression & Write the queue length at time = t &  Store cars that can exit a road to enter a new one BASED ON THE CAPACITY 
		vector<vector<int>> movingCars = preProcess(Roads, timestep);
		//clock_t c = clock();;

	//### Release cars from traveling areas into queues (Common or Individual)
		travelingAreaToQueues(Roads, timestep);
		//clock_t d = clock();

	//### Release cars from queues to traveling areas -> cars stored previously (Based on capacity) 
		queuesToTravelingAreas(Roads, movingCars, timestep);
		/*clock_t e = clock();
		if(e-a > 20)
			cout << endl << "Time : " << t << ". Total: " << e-a << ". 1:" << b-a << ". 2:" << c-b << ". 2:" << d-c << ". 3:" << e-d;*/

	//### Display the network state if needed
		printRoads(t, Roads, timestepsToPrint, superior);
	}
}