#include "car.h"

using namespace std;

//### Constructors & Destructors ###
Car::Car() {		//Constructor used for fake cars in the queues
	fake = true;
	carLength = 5;
}

Car::Car(bool _fake, int _carNumber, float _carLength, int _reacDuration, int _enterTime, int _enterNode, int _exitNode, float _maximumAcceleration) {
	fake = _fake;
	carNumber = _carNumber;
	carLength = _carLength;
	reacDuration = _reacDuration;
	enterTime = _enterTime;
	enterNode = _enterNode;
	exitNode = _exitNode;
	maximumAcceleration = _maximumAcceleration;

	progression.clear();
	reacIter = 0;
	position = 0;
	distInTA = 0;

	path.clear();
	path.push_back(1);
	path.push_back(2);
	path.push_back(3);
	path.push_back(4);
	path.push_back(5);

}

Car::~Car() {

}


//### Getters ###

float Car::length() {
	return carLength;
}

int Car::number() {
	return carNumber;
}

int Car::enteringTime() {
	return enterTime;
}

int Car::enteringNode() {
	return enterNode;
}

int Car::exitingNode() {
	return exitNode;
}

int Car::lastNode() {
	return exitNode;
}

int Car::Node() {
	return path[position];
}

int Car::nextNode() {
	return path[position+1];
}

int Car::prevNode() {
	return path[position-1];
}

bool Car::existence() {
	if(fake)
		return false;
	else
		return true;
}

int Car::reactIter() {
	return reacIter;
}

int Car::reactDuration() {
	return reacDuration;
}

float Car::distanceInTA() {
	return distInTA;
}

float Car::accMax() {
	return maximumAcceleration;
}

vector<int> Car::prog() {
	return progression;
}

float Car::maxSpeed(int road) {
	return maxSpeeds[road];
}


//### Setters ###

void Car::iterDistanceInTA(float length) {
	distInTA += length;
}

void Car::initDistanceInTA() {
	distInTA = 0;
}

void Car::iterProg(int iter) {
	progression.push_back(iter);
}

void Car::iterPosition() {
	position += 1;
}

void Car::initReactIter() {
	reacIter = 0;
}

void Car::iterReactIter(int k) {
	reacIter += k;
}

void Car::addSpeed(float speed) {
	maxSpeeds.push_back(speed);
}