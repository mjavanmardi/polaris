#include "carsPath.h"

using namespace std;

// Undone part : This files (header and cpp) should take as input each car (ENTERING NODE + EXITING NODE) + Network
// An algorithm should find the closest way to go from the entering node to the exiting node with respect to the Network
//
// The easiest algorithm doing that may be Dijkstra
//



void sortCars(vector<Car>& Cars) {
	sort(Cars.begin(), Cars.end());
}

vector<int> preprocessCars(vector<Car>& Cars) { 
	vector<int> enteringTimes;
	for(vector<Car>::iterator it = Cars.begin() ; it != Cars.end() ; it++) {
		enteringTimes.push_back(it->enteringTime());
	}
	sortCars(Cars);
	return enteringTimes;
}