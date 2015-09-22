#include "storage.h"

using namespace std;

//Each cars which aims at crossing an intersection has a weight
//At each step, there is a total weight allowed to cross
//When a car has a weight superior to the remaining allowed weight
//There is a probability that it crosses the intersection
//This function returns 1 if the car crosses, 0 otherwise
int lastCarProba(double remainingAllowedWeight, double lastCarWeight) 
{
	int canCross = 0;
	//srand(rand()*rand()*time(NULL));
	srand(0);
	int possible = rand()%(int)(1000*lastCarWeight);

	if(1000*remainingAllowedWeight > possible)
		canCross = 1;

	return canCross;
}

//For a given queue, returns the number of cars allowed to cross
int numberOfAllowedCars(map<int, Queue>::iterator queueIterator, int timestep, int time) 
{
	//Each cars which aims at crossing an intersection has a weight
	//At each step, there is a total weight allowed to cross
	//Here we compute this total weight
    double capacMin = queueIterator->second.getMinCapacity();
	double totalAllowedWeight = capacMin * timestep / 3600; 

	//Here we get the dynamic capacity of each turning movement
	//Which takes into account the traffic lights' state
	bool isRed = false;
	map<int,double> realCapacity = queueIterator->second.getRealCapacity(time, isRed);
	if(isRed) //If there is a red light, no car can cross
		return 0;

	int realNumberOfCars = 0;

	for(vector<Car>::iterator carIter = queueIterator->second.getCarsBegin(); carIter != queueIterator->second.getCarsEnd();carIter++) 
	{
		Car C = (*carIter);

		double absCapacity = capacMin;
		if(C.existence() == true)
			absCapacity = realCapacity[C.nextNode()];	// Calculate the capacity of the turning movement the car is looking for out of all that are allowed on this lane
		double carWeight = capacMin/absCapacity;	// Calculate the weight of the car ; included in ]0;1] ; equals to one for the turning movement with the smallest capacity
		if(totalAllowedWeight > carWeight) // If the carweight is superior to the number of car that can cross the intersection, than the car can cross
		{				
			totalAllowedWeight -= carWeight;	// Decrease the total number of car still allowed to cross the intersection
			realNumberOfCars++;			// Increase the total number of car really crossing the network
		}
		else 
		{
			// For the car that have a weight superior to the remainding total weight allowed
			// There is a probability of having this car crossing the intersection.
			// It depends on its weight and on the remainding total weight allowed
			realNumberOfCars += lastCarProba(totalAllowedWeight, carWeight);  
			break;
		}
	}
	int size = queueIterator->second.getNumberOfCars();
	realNumberOfCars =(realNumberOfCars < size) ? realNumberOfCars : size;
	return realNumberOfCars;
}

//For a given queue contained in the queueIterator
//This function returns a MovingCars, 
//which is a structure that represents the cars which move at this step in the given queue
MovingCars movingCars(bool& q, map<int, Road>::iterator roadIterator, map<int, Queue>::iterator queueIterator, int timestep, int time) {
	int numberOfCars = numberOfAllowedCars(queueIterator, timestep, time);
	vector<int> nextNodes;
	if(numberOfCars >0) {
		q = true;
		for(vector<Car>::iterator carIter = queueIterator->second.getCarsBegin(); carIter != queueIterator->second.getCarsEnd();carIter++)  
		{
			int nextNode = -999;
			if(carIter->existence() == true)
				nextNode = carIter->nextNode();
			nextNodes.push_back(nextNode);
		}
	}
	int roadID = roadIterator->first;
	int nodeA = roadIterator->second.nodeA();
	int nodeB = roadIterator->second.nodeB();
	int queueID = queueIterator->first;
	MovingCars carsMovingInCurrentQueue(nodeA,nodeB,queueID,roadID,nextNodes);
	return carsMovingInCurrentQueue;
}

//This function does 3 distinct actions on each road, which are specified inside
//It returns a vector which stores all the cars authoriwed to move at the current step
vector<MovingCars> preProcess(map<int, Road>& Roads, int timestep, int time) {
	vector<MovingCars> capacityCars(0);
	for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
		//### Release cars from Common Queue
		it->second.commonToIndividualQueue();

		//### Write cars progression & Write queues length & Moving Fake Cars (In the individual queues && in the common queue)
		it->second.iterQueuesProg(timestep);
		
		//### Store cars that can exit the system based on the capacity
		for(map<int, Queue>::iterator it2 =  it->second.IndivQueuesBegin() ; it2 !=  it->second.IndivQueuesEnd() ; it2++) {
			if(it2->second.getQueue().size() != 0) {
				bool q = false;
				MovingCars newLine = movingCars(q,it,it2,timestep,time);
				if(q)
					capacityCars.push_back(newLine);
			}
		}
	}
	return capacityCars;
}