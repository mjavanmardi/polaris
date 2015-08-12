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

int numberOfAllowedCars(map<int, double>& capac, vector<Car> queue, int timestep) {
	double capacMin = 0;
	int size = queue.size();
	for(map<int, double>::iterator it = capac.begin() ; it != capac.end() ; it++) {
		if(it->second < capacMin || capacMin == 0)
			capacMin = it->second;
	}

	double minNumberOfCars = capacMin * timestep / 3600;
	int realNumberOfCars = 0;

	bool q = true;
	int iter = 0;
	while(q && queue.size() >iter) {
		Car C = queue.at(iter);

		double absCapacity = capacMin;
		if(C.existence() == true)
			absCapacity = capac[C.nextNode()];	// Calculate the capacity of the turning movement the car is looking for out of all that are allowed on this lane
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
	realNumberOfCars =(realNumberOfCars < size) ? realNumberOfCars : size;
	return realNumberOfCars;
}

vector<int> movingCars(bool& q, int nodeA, int nodeB, int queueID, map<int, double> capac, vector<Car> queue, int timestep) {
	int numberOfCars = numberOfAllowedCars(capac, queue, timestep);
	vector<int> newLine;
	if(numberOfCars >0) {
		q = true;
		newLine.push_back(nodeA);
		newLine.push_back(nodeB);
		newLine.push_back(queueID);
		for(int i = 0 ; i < numberOfCars ; i++) {
			int nextNode = -999;
			if(queue.at(i).existence() == true)
				nextNode = queue.at(i).nextNode();
			newLine.push_back(nextNode);
		}
	}
	return newLine;
}

vector<vector<int>> preProcess(map<int, Road>& Roads, int timestep) {
	vector<vector<int>> capacityCars;
	for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
		//### Release cars from Common Queue
		it->second.commonToIndividualQueue();

		//### Write cars progression & Write queues length & Moving Fake Cars (In the individual queues && in the common queue)
		it->second.iterQueuesProg(timestep);
		
		//### Store cars that can exit the system based on the capacity
		map<int, Queue> queues = it->second.indivQueues();
		for(map<int, Queue>::iterator it2 =  queues.begin() ; it2 !=  queues.end() ; it2++) {
			if(it2->second.getQueue().size() != 0) {
				bool q = false;
				vector<int>  newLine = movingCars(q, it->second.nodeA(), it->second.nodeB(), it2->first, it2->second.getCapacities(), it2->second.getQueue(), timestep);
				if(q)
					capacityCars.push_back(newLine);
			}
		}
	
	}
	return capacityCars;
}