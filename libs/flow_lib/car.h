#pragma once

#include <vector>

class Car {
public:
	//### Constructors & Destructors ###
	Car();
	Car::Car(bool _fake, int _carNumber, double _carLength, int _reacDuration, int _enterTime, double _meanAcceleration, double _meanDecceleration);
	~Car();

	//### Getters ###
	double length();		// Length of the car
	int number();			// Number/ID of the car
	int enteringTime();		// Time when the car enters the system
	int enteringNodeA();	// A road is defined by two nodes (ahead and behind the car). enteringNodeA and enteringNodeB store this information when the SQLite database is read
	int enteringNodeB();	//  "	"	"	"
	int exitingNode();		// Node where is car is heading.
	int Node();				// The car is on the road (A,B) heading to (B,C). Returns B
	int nextNode();			// Returns C
	int prevNode();			// Returns A
	bool existence();		// There are fake cars in the networks that model the car reaction time. (Existence = true) => Not a fake car.
	double speed();
	int reactIter();		// Iterator to know how long the car has waited while he car ahead of it has moved
	int reactDuration();	// Reaction time of the vehicle/driver
	double distanceInTA();	// Distance already traveled by the car in the traveling area
	double accMean();		// Maximum acceleration of the car (!!! if the has a linear acceleration only !!! If it's polynomial, such as d
	double deccMean();
	std::vector<int> prog();// Store the progression state of the car (0: out of the queue /\ 1:first car in the queue /\ 2 ; second car in the queue /\ ...)
	double maxSpeed(int road);	//

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
	int exitNode;
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