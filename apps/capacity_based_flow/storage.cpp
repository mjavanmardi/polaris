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
	double minNumberOfCars = capacMin * timestep /3600;

	//### Calculate how many cars are released ###
	bool q = true;
	int iter = 0;
	while(q) {
		Car C = Q.getQueue()[iter];
		double absCapacity = capac[C.nextNode()];
		double carWeight = capacMin/absCapacity;
		if(minNumberOfCars > carWeight) {
			iter += 1;
			minNumberOfCars -= carWeight;
			realNumberOfCars += 1;
		}
		else {
			realNumberOfCars += lastCarProba(minNumberOfCars, carWeight);
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