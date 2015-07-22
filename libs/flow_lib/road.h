#pragma once

#include "car.h"
#include "queue.h"




class Road {
public:
	//### Constructors & Destructors ###
	Road();
	Road(int anode, int bnode, float _maximumSpeed, float _totalLength, float _distanceBetweenCars);
	~Road();
	void addQueue(int ID, float _maxLength, float _distanceBetweenCars, std::map<int,float> capacities);


	//### Getters ###
	int nodeA();
	int nodeB();
	float speedMax();
	float getMaxIndivQueueLength();							// Get the queue length which is maximum among all the individual queues
	int getMaxIndivQueueSize();								// Get the number of cars which which is maximum among all the individual queues
	float getCommonQueueLength();							// Get the common queue length
	float getMaxCommonQueueLength();						// Get the maximum common queue length
	std::map<int, Queue> indivQueues();						// Get the individual queues
	Queue getQueue(int ID);
	std::vector<Car> getLastQueue();
	std::vector<Car> getCommonQueue();
	std::vector<Car>& getTA();

	//### Dynamic Methods ###
	int selectIndividualQueue(int nextNode, bool& q);		// Select the queue where the car is going to go if its heading to nextNode ; q explicit if such a queue exist
	void commonToIndividualQueue();							// Release cars from common to individual queue
	void iterQueuesProg(int timestep);						// Iterate the queue lengh over time AND the cars progression
	float distanceToTravelInTA(Car C);				// Distance to travel for a car in a traveling Area to get to a queue
	
	void addCarToQueue(Car C);
	void removeCarFromQueue(int queueID, int timestep);
	void addCarToTA(Car& C);
	void removeCarsFromTA(std::vector<int> carNumbers);

	void moveFakeCars(int ID, int timestep);



private:
	//### Constant parameters ###
	int Anode;
	int Bnode;
	float maximumSpeed;
	float totalLength;
	float distanceBetweenCars;

	//### Common Queue, Individual Queue & Traveling Area ###
	std::vector<Car> travelingArea;
	std::map<int, Queue> queues;		//Queue ID & Queue itself
	std::vector<Car> commonQueue;
	std::vector<Car> lastQueue;

	//### Outputs ###
	std::vector<float> lengthOverTime;
};

