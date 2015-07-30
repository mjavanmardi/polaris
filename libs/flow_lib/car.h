#pragma once

#include <vector>

class Car {
public:
	//### Constructors & Destructors ###
	Car();
	Car::Car(bool _fake, int _carNumber, double _carLength, int _reacDuration, int _enterTime, double _maximumAcceleration);
	~Car();

	//### Getters ###
	double length();
	int number();
	int enteringTime();
	int enteringNodeA();
	int enteringNodeB();
	int exitingNode();
	int lastNode();
	int Node();
	int nextNode();
	int prevNode();
	bool existence();
	int reactIter();
	int reactDuration();
	double distanceInTA();
	double accMax();
	std::vector<int> prog();
	double maxSpeed(int road);

	//### Setters ###
	void iterDistanceInTA(double length);		// Traveled distance in Traveling Area
	void initDistanceInTA();

	void iterProg(int iter);					// Progression of the car : State(in OR out [of the queue]) ; if(in the queue), gives the position
	void iterPosition();
	void initReactIter();						// Reaction time when the car upfront moves
	void iterReactIter(int k);

	void addSpeed(double speed);


	friend bool operator< (Car & C1, Car & C2) {
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
	double maximumAcceleration;
	std::vector<int> path;

	//### Iterators ###
	int reacIter;
	int position;
	double distInTA;

	//### Outputs ###
	std::vector<int> progression;
	std::vector<double> maxSpeeds;	//Max speeds depending on roads => needed for the outputs


};