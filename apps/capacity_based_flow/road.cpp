#include "road.h"

using namespace std;

//### Constructors & Destructors ###
Road::Road(){

}

Road::Road(int ID, int link,  int anode, int bnode, double _maximumSpeed, double _totalLength, double _distanceBetweenCars) {
	roadID = ID;
	roadLink = link;
	Anode = anode;
	Bnode = bnode;
	maximumSpeed = _maximumSpeed;
	totalLength = _totalLength;
	distanceBetweenCars = _distanceBetweenCars;

	travelingArea.clear();
	queues.clear();
	commonQueue.clear();
	lastQueue.clear();
	lengthOverTime.clear();
}

Road::~Road() {

}

void Road::addQueue(int ID, double _maxLength, double _distanceBetweenCars, map<int,double> capacities) {
	Queue Q(ID, _maxLength, _distanceBetweenCars, capacities);
	queues[ID] = Q;
}


//### Getters ###

int Road::ID() {return roadID;}

int Road::link() {return roadLink;}

int Road::nodeA(){return Anode;}

int Road::nodeB(){return Bnode;}

double Road::speedMax() {return maximumSpeed;}

double Road::getMaxIndivQueueLength(){	// Length in meters
	double max = 0;		
	for(map<int, Queue>::iterator it = queues.begin() ; it != queues.end() ; it++) {	// Loop over the queues to find the maximal length
		if(it->second.length() > max)
			max = it->second.length();
	}
	return max;
}

int Road::getMaxIndivQueueSize() {		// Size in number of cars
	int max = 0;
	for(map<int, Queue>::iterator it = queues.begin() ; it != queues.end() ; it++) {	// Loop over the queues to find the one with the larger number of cars
		if(it->second.getQueue().size() > max)
			max = it->second.getQueue().size();
	}
	return max;
}

double Road::getCommonQueueLength() {
	double commonQueueLength = 0;
	for(vector<Car>::iterator it = commonQueue.begin() ; it != commonQueue.end() ; it ++) {		// Loop over the common queue to count its length, adding "distanceBetweenCars" to have a space between them
		commonQueueLength = it->length() + distanceBetweenCars;
	}
	return commonQueueLength/(double)queues.size();			// The total length is divided by the number of queues = number of lanes on the road. The output is in meter;
}

double Road::getMaxCommonQueueLength() {return totalLength - getMaxIndivQueueLength();}

map<int, Queue> Road::indivQueues() {return queues;}

Queue Road::getQueue(int ID) {return queues[ID];}

vector<Car> Road::getLastQueue(){return lastQueue;}

vector<Car> Road::getCommonQueue() {return commonQueue;}

vector<Car>& Road::getTA() {return travelingArea;}

vector<double> Road::getLengthOverTime() {return lengthOverTime;}


//### Dynamic Methods ###

int Road::selectIndividualQueue(int nextNode, bool& q){
	// nextNode is the node where the car is going. It the car is on Road(NodeA,NodeB) and goes to Road(B,C), it represents C
	// The boolean returns true if a road has been found (Not being ealready full of cars)
	int queueID = 0;
	int minWeight = 10000;		// Initialization to get a queue
	q = false;
	for(map<int, Queue>::iterator it = queues.begin() ; it != queues.end() ; it++) {
		if(it->second.weight(nextNode) < minWeight) {
			minWeight = it->second.weight(nextNode);	// Iteration on the minimum weight to compare to the next one
			queueID = it->first;						// Get the ID of the queue
			q = true;									// At least, there is room for a car in one of the queues going to nextNode
		}
	}
	return queueID;
}

void Road::commonToIndividualQueue() {
	int iter = 0;		// Iterator which stops when it gets to the second layer of cars in the common queue
	while(commonQueue.size() != 0) {
		if(iter == queues.size() || iter == commonQueue.size() )	// If the first layer has already been screened (iter == queues.size() ) or it got to the end (commonQueue.size()), the loop is broken/stopped
			break;

		Car C = commonQueue[0+iter];
		bool q = false;
		int ID = selectIndividualQueue(C.nextNode(), q);		// Returns  q = false if at least one queue having a turn on C.nextNode() is empty
		if (q) {
			queues[ID].getQueue().push_back(C);
			commonQueue.erase(commonQueue.begin()+iter);
		}
		else
			iter += 1;
	}
}

void Road::iterQueuesProg(int timestep) {
	
	//### Move Fake Cars in the individual Queues  &&  Iterate Queues length & Cars progression ###
	for(map<int, Queue>::iterator it = queues.begin() ; it != queues.end() ; it++) {
		moveFakeCars(it->first, timestep);
		it->second.iterCarsProg();		// Iterate Queues length & Cars progression

	}


	//### Iterate cars progression in the Common Queue ###
	int numberOfCars = getMaxIndivQueueSize();
	int iter = 0;
	for(vector<Car>::iterator it = commonQueue.begin() ; it != commonQueue.end() ; it++) {
		if(it->existence()) {
			double div = (double) iter / (double) queues.size();
			it->iterProg(numberOfCars + (int)floor(div) );
			iter += 1;
		}
	} 

	//### Iterate Common Queue Length ###
	lengthOverTime.push_back( getMaxIndivQueueLength() + getCommonQueueLength() );
}

double Road::distanceToTravelInTA(Car C) {
	double distance;
	if(C.Node() != C.exitingNode()) {	// It is not the last road where the car is traveling on		
		if(commonQueue.size() != 0)			// There are cars in the common queue
			distance = totalLength - getCommonQueueLength() - getMaxIndivQueueLength();
		else {								// There is no cars in the common queue
			bool q = false;
			int queueID = selectIndividualQueue(C.nextNode(), q);
			if(q)	// If q = true, it means that there is at least one lane that can host the car (Meaning having the turning movement the car is looking for and not being full)
				distance = totalLength - queues[queueID].length();
			else	// If q = false, it means that there all the queues having the dedicated turning movement 
				distance = totalLength - getMaxIndivQueueLength();
		}
	}
	else {				// The car is traveling on its last road. It's going to exit the network
		distance = totalLength; // If the car enters its last road, it has to go through all the road (Independantly of cars being on the road -> It assumes that the exiting roads are quite the same for every cars so they are just stored in the last queue)
	}
	return distance;
}

void Road::addCarToQueue(Car C){
	if(C.Node() == C.lastNode())	//Last Road of the car
		lastQueue.push_back(C);
	else {							//Not last road of the car
		bool q = false;
		int queueID = selectIndividualQueue(C.nextNode(), q);
		if(q)							//The car can go into some queue
			queues[queueID].addCar(C);
		else							//The car cannot fit any individual queue
			commonQueue.push_back(C);
	}
}

void Road::removeCarFromQueue(int queueID, int timestep) {
	queues[queueID].removeCar();
	moveFakeCars(queueID, timestep);
}

void Road::addCarToTA(Car& C) {
	C.iterPosition();
	C.addSpeed(maximumSpeed);
	travelingArea.push_back(C);
}

void Road::removeCarsFromTA(std::vector<int> carNumbers) {
	vector<Car> newTA;
	while(carNumbers.size() != 0) {
		for(vector<Car>::iterator it = travelingArea.begin() ; it != travelingArea.end() ; it++) {
			if(it->number() != carNumbers[0])
				newTA.push_back((*it));
		}
		carNumbers.erase(carNumbers.begin());
		travelingArea = newTA;
	}
}

void Road::moveFakeCars(int ID, int timestep) {
	queues[ID].moveFakeCars(timestep);
}

