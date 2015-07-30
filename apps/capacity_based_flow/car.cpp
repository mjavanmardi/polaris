#include "car.h"

using namespace std;

//### Constructors & Destructors ###
Car::Car() {		//Constructor used for fake cars in the queues
	fake = true;
	carLength = 5;
}

Car::Car(bool _fake, int _carNumber, double _carLength, int _reacDuration, int _enterTime, double _maximumAcceleration) {
	fake = _fake;
	carNumber = _carNumber;
	carLength = _carLength;
	reacDuration = _reacDuration;
	enterTime = _enterTime;
	maximumAcceleration = _maximumAcceleration;

	progression.clear();
	reacIter = 0;
	position = 0;
	distInTA = 0;

	path.clear();
	path.push_back(83938);
	path.push_back(83939);
	path.push_back(83941);
	path.push_back(83937);
	path.push_back(83936);
	path.push_back(83935);
	path.push_back(83943);

	enterNodeA = 83938;
	enterNodeB = 83939;
	exitNode = 83943;
}

Car::~Car() {

}


//### Getters ###

double Car::length() {
	return carLength;
}

int Car::number() {
	return carNumber;
}

int Car::enteringTime() {
	return enterTime;
}

int Car::enteringNodeA() {
	return enterNodeA;
}
int Car::enteringNodeB() {
	return enterNodeB;
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

double Car::distanceInTA() {
	return distInTA;
}

double Car::accMax() {
	return maximumAcceleration;
}

vector<int> Car::prog() {
	return progression;
}

double Car::maxSpeed(int road) {
	return maxSpeeds[road];
}


//### Setters ###

void Car::iterDistanceInTA(double length) {
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

void Car::addSpeed(double speed) {
	maxSpeeds.push_back(speed);
}