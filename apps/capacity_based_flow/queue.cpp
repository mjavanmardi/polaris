#include "queue.h"

using namespace std;

//### Constructors & Destructors ###
Queue::Queue() {

}

Queue::Queue(int ID, double _maxLength, double _distanceBetweenCars, map<int,double> _capacities) {
	queueID = ID;
	maxLength = _maxLength;
	distanceBetweenCars = _distanceBetweenCars;
	capacities = _capacities;
	
	cars.clear();
	lengthOverTime.clear();
	nextNodes.clear();
	for(map<int, double>::iterator it = _capacities.begin() ; it != _capacities.end() ; it++) {
		nextNodes.push_back(it->first);
	}
}

Queue::~Queue() {

}

//### Getters ###

int Queue::ID() {
	return queueID;
}

double Queue::length() {
	double queueLength = 0;
	for(vector<Car>::iterator it = cars.begin() ; it != cars.end() ; it++) {
		queueLength += it->length() + distanceBetweenCars;
	}
	return queueLength;
}

vector<Car> Queue::getQueue() {
	return cars;
}

map<int, double> Queue::getCapacities() {
	return capacities;
}

//### Dynamic methods ###

int Queue::weight(int nextNode) {
	int weight;
	bool exist = false;
	for(vector<int>::iterator it = nextNodes.begin() ; it != nextNodes.end() ; it++) {
		if((*it) == nextNode) {
			exist = true;
			break;
		}
	}

	if(exist && length() < maxLength)
		weight = cars.size()*2 + nextNodes.size()*5;
	else
		weight = 900000;

	return weight;
}

void Queue::iterCarsProg() {
	int iter = 1;
	double queueLength = 0;
	for(vector<Car>::iterator it = cars.begin() ; it != cars.end() ; it++) {
		if(it->existence()) {
			it->iterProg(iter);
			iter += 1;
		}
		queueLength = it->length() + distanceBetweenCars;
	}

	if(queueLength < maxLength)
		lengthOverTime.push_back(queueLength);
	else
		lengthOverTime.push_back(maxLength);
}

void Queue::addCar(Car C) {
	cars.push_back(C);
}

void Queue::removeCar() {
	Car fakeCar;
	cars[0] = fakeCar;
}

void Queue::moveFakeCars(int timestep) {
	for(vector<Car>::iterator it = cars.begin() ; it != cars.end() ; it++) {
		if(it->existence() == false && it != cars.end() - 1) {		// The car is fake but not the last one
			if( (it+1)->existence() ) {									// The next car is not false 
				if( (it+1)->reactIter() < (it+1)->reactDuration() )	{		// The car has not already waited for enough time
					srand(rand()*rand()*time(NULL));
					int trand = rand()%100;
					int proba = 100*timestep/((it+1)->reactDuration() - (it+1)->reactIter());
					(it+1)->iterReactIter(timestep);

					if(proba > trand) {
						(it+1)->initReactIter();
						iter_swap(it,it+1);
					}
				}

				else {
					(it+1)->initReactIter();
					iter_swap(it,it+1);
				}
			}
		}		
	}
	while(cars.size() != 0 && cars.back().existence() == false) {
		cars.pop_back();
	}
}