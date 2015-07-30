#include "storage.h"

using namespace std;

int lastCarProba(double proba, double weight) {
	int lastCar = 0;

	srand(rand()*rand()*time(NULL));
	int possible = rand()%(int)(1000*weight);

	if(1000*proba > possible)
		lastCar = 1;

	return lastCar;
}

int numberOfAllowedCars(Queue Q, int timestep) {
	int realNumberOfCars = 0;
	map<int, double> capac = Q.getCapacities();
	
	//### Get the biggest capactiy ; so that it'll be used as reference capacity to release cars ###
	double capacMin = 0;
	for(map<int, double>::iterator it = capac.begin() ; it != capac.end() ; it++) {
		if((*it).second < capacMin || capacMin == 0)
			capacMin = (*it).second;
	}
	double minNumberOfCars = capacMin * timestep /3600; // the capac min is in cars per hour

	//### Calculate how many cars are released ###
	bool q = true;		// Boolean to know if the model should keep on making car cross the intersection
	int iter = 0;
	while(q && Q.getQueue().size() > iter) {	// While the model should look for another car
		Car C = Q.getQueue()[iter];					// Get the next car
		double absCapacity = capac[C.nextNode()];	// Calculate the capacity of the turning movement the car is looking for out of all that are allowed on this lane
		double carWeight = capacMin/absCapacity;	// Calculate the weight of the car ; included in ]0;1] ; equals to one for the turning movement with the smallest capacity
		if(minNumberOfCars > carWeight) {				// If the carweight is superior to the number of car that can cross the intersection, than the car is 
			iter += 1;						// To check in the next loop if the next car can cross the intersection
			minNumberOfCars -= carWeight;	// Decrease the total number of car still allowed to cross the intersection
			realNumberOfCars += 1;			// Increase the total number of car really crossing the network
		}
		else {		// For cars that have a weight superior to the minumum number of cars still allowed to crosse
			realNumberOfCars += lastCarProba(minNumberOfCars, carWeight); // There is a probability of having this car crossing the intersection. It depends on its weight and on the remainder of number of cars allowed to crosse
			q = false;
		}

	}

	realNumberOfCars =(realNumberOfCars < Q.getQueue().size()) ? realNumberOfCars : Q.getQueue().size();
	
	return realNumberOfCars;
}

void movingCars(vector<vector<int>>& cars, Road R, int timestep) {
	map<int, Queue> queues = R.indivQueues();
	for(map<int, Queue>::iterator it = queues.begin() ; it != queues.end() ; it++) {
		if(it->second.getQueue().size() != 0) {
			int numberOfCars = numberOfAllowedCars(it->second, timestep);
			if(numberOfCars > 0) {
				vector<int> newLine;
				newLine.push_back(R.nodeA());					// Node i
				newLine.push_back(R.nodeB());					// Node j
				newLine.push_back(it->first);					// Queue ID
				for(int i = 0 ; i < numberOfCars ; i++) {
					newLine.push_back(it->second.getQueue()[i].nextNode());// nextNode(Car[0]) - nextNode(Car[1]) - nextNode(Car[2]) ... for the cars allowed to move
				}
				cars.push_back(newLine);
			}
		}
	}
}

vector<vector<int>> preProcess(map<int, Road>& Roads, int timestep) {
	vector<vector<int>> capacityCars;
	for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
		//### Release cars from Common Queue
		it->second.commonToIndividualQueue();
		
		//### Write cars progression & Write queues length & Moving Fake Cars (In the individual queues && in the common queue)
		it->second.iterQueuesProg(timestep);

		//### Store cars that can exit the system based on the capacity
		movingCars(capacityCars, it->second, timestep);
	}
	return capacityCars;
}