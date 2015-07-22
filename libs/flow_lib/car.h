#pragma once

#include <vector>

class Car {
public:
	//### Constructors & Destructors ###
	Car();
	Car::Car(bool _fake, int _carNumber, float _carLength, int _reacDuration, int _enterTime, int _enterNode, int _exitNode, float _maximumAcceleration);
	~Car();

	//### Getters ###
	float length();
	int number();
	int enteringTime();
	int enteringNode();
	int exitingNode();
	int lastNode();
	int Node();
	int nextNode();
	int prevNode();
	bool existence();
	int reactIter();
	int reactDuration();
	float distanceInTA();
	float accMax();
	std::vector<int> prog();
	float maxSpeed(int road);

	//### Setters ###
	void iterDistanceInTA(float length);			// Traveled distance in Traveling Area
	void initDistanceInTA();

	void iterProg(int iter);					// Progression of the car : State(in OR out [of the queue]) ; if(in the queue), gives the position
	void iterPosition();
	void initReactIter();						// Reaction time when the car upfront moves
	void iterReactIter(int k);

	void addSpeed(float speed);

private:
	//### Constant parameters ###
	bool fake;
	int carNumber;
	float carLength;
	int reacDuration;
	int enterTime;
	int enterNode;
	int exitNode;
	float maximumAcceleration;
	std::vector<int> path;

	//### Iterators ###
	int reacIter;
	int position;
	float distInTA;

	//### Outputs ###
	std::vector<int> progression;
	std::vector<float> maxSpeeds;	//Max speeds depending on roads => needed for the outputs


};