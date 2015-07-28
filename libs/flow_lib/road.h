#pragma once

#include "car.h"
#include "queue.h"




class Road {
public:
	//### Constructors & Destructors ###
	Road();
	Road(int ID, int link, int anode, int bnode, double _maximumSpeed, double _totalLength, double _distanceBetweenCars);
	~Road();
	void addQueue(int ID, double _maxLength, double _distanceBetweenCars, std::map<int,double> capacities);


	//### Getters ###
	int ID();
	int link();
	bool direction();
	int nodeA();
	int nodeB();
	double speedMax();
	double getMaxIndivQueueLength();						// Get the queue length which is maximum among all the individual queues
	int getMaxIndivQueueSize();								// Get the number of cars which which is maximum among all the individual queues
	double getCommonQueueLength();							// Get the common queue length
	double getMaxCommonQueueLength();						// Get the maximum common queue length
	std::map<int, Queue> indivQueues();						// Get the individual queues
	Queue getQueue(int ID);
	std::vector<Car> getLastQueue();
	std::vector<Car> getCommonQueue();
	std::vector<Car>& getTA();
	std::vector<double> getLengthOverTime();

	//### Dynamic Methods ###
	int selectIndividualQueue(int nextNode, bool& q);		// Select the queue where the car is going to go if its heading to nextNode ; q explicit if such a queue exist
	void commonToIndividualQueue();							// Release cars from common to individual queue
	void iterQueuesProg(int timestep);						// Iterate the queue lengh over time AND the cars progression
	double distanceToTravelInTA(Car C);				// Distance to travel for a car in a traveling Area to get to a queue
	
	void addCarToQueue(Car C);
	void removeCarFromQueue(int queueID, int timestep);
	void addCarToTA(Car& C);
	void removeCarsFromTA(std::vector<int> carNumbers);

	void moveFakeCars(int ID, int timestep);



private:
	//### Constant parameters ###
	int roadID;
	int roadLink;
	int Anode;
	int Bnode;
	double maximumSpeed;
	double totalLength;
	double distanceBetweenCars;

	//### Common Queue, Individual Queue & Traveling Area ###
	std::vector<Car> travelingArea;
	std::map<int, Queue> queues;		//Queue ID & Queue itself
	std::vector<Car> commonQueue;
	std::vector<Car> lastQueue;

	//### Outputs ###
	std::vector<double> lengthOverTime;
};

