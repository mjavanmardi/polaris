#include "toqueues.h"

using namespace std;


void TAToQueues(Road& R, int timestep) {
	vector<int> carsToErase;
	for(vector<Car>::iterator it = R.getTA().begin() ; it != R.getTA().end() ; it++) {
		it->iterProg(0);
		if(it->distanceInTA() < R.distanceToTravelInTA((*it)))		// The car has to reach its queue (Individual or common)
			it->iterDistanceInTA(R.speedMax()*timestep);
		// !!! SOME TIME IS ADDED TO FAKE THE FACT THAT THE CAR WAS NOT TRAVELING AT VMAX 
		// !!! BUT, BESIDES THE TIME ADDED< THE CAR SHOULD REALLY WAIT THIS TIME AND NOT BE IN THE SYSTEM ANYMORE
		else {																// The car has reached its queue (Individual or common)
			it->initDistanceInTA();															//The traveled distance is initialized
			int addedTime = timeToAdd(R.speedMax(), it->accMax(), it->distanceInTA() );		//Some time is added to the cars 
			for(int i = 0; i<addedTime ; i++) { it->iterProg(0); }
			carsToErase.push_back(it->number());
			R.addCarToQueue((*it));
		}
	}
	R.removeCarsFromTA(carsToErase);
}

void travelingAreaToQueues(map<int, Road>& Roads, int timestep) {
	for(map<int,Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
		TAToQueues(it->second,timestep);
	}
}

int timeToAdd(double speedMax, double accMax, double distanceInTA) {
	double timeCar = speedMax / accMax;
	double timeVmax = distanceInTA / speedMax;

	int addedTime;
	if(timeCar<timeVmax)
		addedTime = ceil(speedMax / accMax);
	else
		addedTime = ceil(2*sqrt(distanceInTA/accMax) - distanceInTA/speedMax);

	return addedTime;
}