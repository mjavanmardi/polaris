#pragma once

#include <iostream>	//Only for the debugging : cout
#include "json.h"
#include "reader.h"
#include "JsonParser.h"
#include <vector>

class Car {
public:
	//### Constructors & Destructors ###
	Car();
	Car::Car(bool _fake, int _carNumber, double _carLength, int _reacDuration, int _enterTime, double _meanAcceleration, double _meanDecceleration, int entA, int entB, int exA, int exB);
	Car(Json::Value value); //Creates a car from a json value
	~Car();

	//### Getters ###
	double length() const;		// Length of the car
	int number() const;			// Number/ID of the car
	int enteringTime() const;		// Time when the car enters the system
	int enteringNodeA() const;	// A road is defined by two nodes (ahead and behind the car). enteringNodeA and enteringNodeB store this information when the SQLite database is read
	int enteringNodeB() const;	//  "	"	"	"
	int exitingNode() const;		// Node where is car is heading.
	int Node() const;				// The car is on the road (A,B) heading to (B,C). Returns B
	int nextNode() const;			// Returns C
	int prevNode() const;			// Returns A
	bool existence() const;		// There are fake cars in the networks that model the car reaction time. (Existence = true) => Not a fake car.
	double speed() const;
	int reactIter() const;		// Iterator to know how long the car has waited while the car ahead of it has moved
	int reactDuration() const;	// Reaction time of the vehicle/driver
	double distanceInTA() const;	// Distance already traveled by the car in the traveling area
	double accMean() const;		// Maximum acceleration of the car (!!! if the has a linear acceleration only !!! If it's polynomial, such as d
	double deccMean() const;
	std::vector<int> prog() const;// Store the progression state of the car (0: out of the queue /\ 1:first car in the queue /\ 2 ; second car in the queue /\ ...)
	double maxSpeed(int road) const;	//

	//### Setters ###
	void iterDistanceInTA(double length);	// Iterated traveled distance in Traveling Area by length (= Speed * Timestep)
	void initDistanceInTA();				// Initialize traveled distance in Traveling Area to 0

	void iterProg(int iter);				// Progression of the car : State(in OR out [of the queue]) ; if(in the queue), gives the position
	void iterPosition();
	void initReactIter();					// Reaction time when the car upfront moves
	void iterReactIter(int k);
	void initSpeed();
	void iterSpeed(double newSpeed);

	void addSpeed(double speed);
	void postponedEnteringTime(int timestep);

	//### Serialization ###

	Json::Value toJson(); //Turns current car into a json value
	bool operator==(const Car & C1) const;
	
	friend bool operator< (Car & C1, Car & C2) { // Operator to compare the vehicles based on their entering time. Help sorting the vehicles that enter the system
		if(C1.enterTime < C2.enterTime)
			return true;
		else
			return false;
	}

private:
	//### Constant parameters ###
	bool fake;
	int carNumber;
	double carLength;
	int reacDuration;
	int enterTime;
	int enterNodeA;
	int enterNodeB;
	int exitNodeA;
	int exitNodeB;
	double meanAcceleration;
	double meanDecceleration;
	double carSpeed;
	std::vector<int> path;

	//### Iterators ###
	int reacIter;
	int position;
	double distInTA;

	//### Outputs ###
	std::vector<int> progression;
	std::vector<double> maxSpeeds;	//Max speeds depending on roads => needed for the outputs


};