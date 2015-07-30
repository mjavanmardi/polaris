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

void preprocessCars(vector<Car>& Cars) { 
	sortCars(Cars);
}