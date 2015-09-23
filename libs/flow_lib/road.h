#pragma once

#include "car.h"
#include "queue.h"




class Road {
public:
	//### Constructors & Destructors ###
	Road();
	Road(int ID, int link, int anode, int bnode, double _maximumSpeed, double _totalLength, double _distanceBetweenCars);
	Road(Json::Value value);
	~Road();
	void addQueue(int ID, double _maxLength, double _distanceBetweenCars, std::map<int,double> capacities, std::map<int,double> greenTime, std::map<int,double> cycle, std::map<int,double> offset);		// Add queue to the Road ; used only when importing the SQLite database


	//### Getters ###
	int ID() const;									// ID of the Road						
	int link() const;								// Link of the Road ; used only to import/convert the data structure from the SQLite database
	bool direction() const;							// Link of the Road ; used only to import/convert the data structure from the SQLite database
	int nodeA() const;								// Get the entering node of the road
	int nodeB() const;								// Get the exiting node of the road
	double speedMax() const;								// Get the maixmum speed of the road
	double getMaxIndivQueueLength() const;				// Get the maximum  queue length among all the individual queues
	int getMaxIndivQueueSize();						// Get the maximum number of cars among all the individual queues
	double getCommonQueueLength() const;					// Get the common queue length
	double getMaxCommonQueueLength();				// Get the maximum common queue length
	std::map<int, Queue> indivQueues();				// Get the individual queues
	std::map<int, Queue>::iterator IndivQueuesBegin();
	std::map<int, Queue>::iterator IndivQueuesEnd();
	Queue getQueue(int ID);							// Get the cars being in the queue having the id=ID
	std::vector<Car> getLastQueue();				// Get the cars being in the last queue
	std::vector<Car> getCommonQueue();				// Get the cars being in the common queue
	std::vector<Car>& getTA();						// Get the cars being in the traveling area
	std::vector<double> getLengthOverTime();		// Get the length of the maximum queue in the road
	double getNthQueueSize(int n) const;

	//### Dynamic Methods ###
	int selectIndividualQueue(int nextNode, bool& q) const;	// Select the queue where the car is going to go if its heading to nextNode ; q explicit if such a queue exist
	void commonToIndividualQueue();						// Release cars from common to individual queue
	void iterQueuesProg(int timestep);					// Iterate the queue lengh over time AND the cars progression
	double distanceToTravelInTA(Car C) const;					// Distance to travel for a car in a traveling Area to get to a queue
	
	void addCarToQueue(Car C);							// Release the car C from the traveling area into the queue (Either one individual queue or either the common queue when the indivudual ones are full)
	void removeCarFromQueue(int queueID, int timestep); // Remove the first car of the queue having the id=ID. The parameters "timestep" deals with the fake cars
	void addCarToTA(Car& C);							// Release the car C into the traveling area ; it means it just enters the road
	void removeCarsFromTA(std::vector<int> carNumbers); // Remove the cars from the Traveling Area ; it means it just entered a queue

	void moveFakeCars(int ID, int timestep);			// This method deals with the fake cars added in the system to model/simulate the reaction duration of real car when the one ahead moved.

	//### Serialization ###
	Json::Value toJson();
	bool operator==(const Road & R) const;

private:
	//### Constant parameters ###
	int roadID;						// ID of the car
	int roadLink;					// Link of the car (Only to import the Network from the SQLite database)
	int Anode;						// Entering node of the Road
	int Bnode;						// Exiting node of the Road
	double maximumSpeed;			// Maximum speed on the road
	double totalLength;				// Total length of the road (Including Traveling area, common queue and individual queue)
	double distanceBetweenCars;		// Distance between the cars in the common queue

	//### Common Queue, Individual Queue & Traveling Area ###
	std::vector<Car> travelingArea;	// Cars in the Traveling Area
	std::map<int, Queue> queues;	// Queue ID & Queue itself (Go to queue.h & queue.cpp)
	std::vector<Car> commonQueue;	// Cars in the Common Queue
	std::vector<Car> lastQueue;		// Cars in the Last queue. The last queue represents the queue of cars exiting the system on this road. They are stored in this lastQueue vector

	//### Outputs ###
	std::vector<double> lengthOverTime;		// Store the length of the road at each time-step
};

