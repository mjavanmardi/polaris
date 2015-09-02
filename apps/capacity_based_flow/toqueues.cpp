#include "toqueues.h"

using namespace std;


void TAToQueues(Road& R, int timestep) {
	vector<int> carsToErase;
	for(vector<Car>::iterator it = R.getTA().begin() ; it != R.getTA().end() ; it++) {
		it->iterProg(0);
		if(it->distanceInTA() < R.distanceToTravelInTA(*it)) {		// The car has to reach its queue (Individual or common)
			double length;
			double newSpeed = newCarSpeed(it->speed(), R.speedMax(), timestep, length, R.distanceToTravelInTA(*it) - it->distanceInTA(), it->accMean(), it->deccMean());
			it->iterSpeed(newSpeed);
			it->iterDistanceInTA(length);
		}
		else {								// The car has reached its queue (Individual or common)
			it->initDistanceInTA();					//The traveled distance is initialized
			it->initSpeed();
			carsToErase.push_back(it->number());
			R.addCarToQueue(*it);
		}
	}
	R.removeCarsFromTA(carsToErase);
}

void travelingAreaToQueues(map<int, Road>& Roads, int timestep) {
	for(map<int,Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
		TAToQueues(it->second,timestep);
	}
}

double newCarSpeed(double carSpeed, double speedLimit, int timestep, double& length, double distanceLeftToTravel, double accMean, double deccMean) {
	double newSpeed;
	// Be careful ! If the car speed is stored, it might decrease and increase a bit again before a traffic light
	// This is due to the fact that comparison are made each Delta(t), not continuously
	// Moreover, these acceleration and decceleration are MEAN value, so its understadable to have higher and lower value
	if(carSpeed>sqrt(2*deccMean*(distanceLeftToTravel-carSpeed*timestep))) {	// the "if"staetment compares the real car speed to the maximal speed the car can have 
		newSpeed = carSpeed - deccMean*timestep;								// in order to reach a speed equal to 0 when reaching the queue (with respect to deccMean, distanceLeftToTravel, carSpeed
		if(newSpeed<1)															// If carSpeed > maxSpeedToReach 0 after having traveled distanceLeftToTravel, then the speed decreases
			newSpeed = 1;
	}
	else if( carSpeed < speedLimit) {						// If the car didn't reach the moment to decrease its speed, it can increase it to the speedLimit
		newSpeed = carSpeed + timestep*accMean;
		if(newSpeed > speedLimit)
			newSpeed = speedLimit;
	}
	else
		newSpeed = carSpeed;

	length = timestep * (carSpeed + newSpeed) /2;		// The traveled distance is the duration * mean speed		(mean speed = (speed(t) + speed(t+1)) / 2)
	return newSpeed;
}