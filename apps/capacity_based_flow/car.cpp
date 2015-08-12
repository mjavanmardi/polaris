#include "car.h"

using namespace std;

//### Constructors & Destructors ###
Car::Car() {		//Constructor used for fake cars in the queues
	fake = true;
	carNumber = -999;
	carLength = 5;
}

Car::Car(bool _fake, int _carNumber, double _carLength, int _reacDuration, int _enterTime, double _meanAcceleration, double _meanDecceleration, int entA, int entB, int exA, int exB) {
	fake = _fake;
	carNumber = _carNumber;
	carLength = _carLength;
	reacDuration = _reacDuration;
	enterTime = _enterTime;
	meanAcceleration = _meanAcceleration;
	meanDecceleration = _meanDecceleration;

	reacIter = 0;
	position = 0;
	distInTA = 0;
	carSpeed = 0;

	enterNodeA = entB;
	enterNodeB = entA;
	exitNodeA = exA;
	exitNodeB = exB;

//### THIS PART SHOULD BE DELETED ONCE THE DJIKSTRA OR WHATEVER MODEL WILL BE ADDED IN THE carsPath.cpp && carsPath.h FILES ###
	path.push_back(enterNodeA);
	path.push_back(enterNodeB);

	if(enterNodeA == 28 && _carNumber%3 == 0) {
		path.push_back(15);
		path.push_back(10);
	}
	else if(enterNodeA == 28 && _carNumber%3 == 1) {
		path.push_back(15);
		path.push_back(19);
	}
	else if(enterNodeA == 28 && _carNumber%3 == 2){
		path.push_back(22);
		path.push_back(19);
	}
	else if(enterNodeA == 26 && _carNumber%3 == 0) {
		path.push_back(20);
		path.push_back(14);
	}
	else if(enterNodeA == 26 && _carNumber%3 == 1) {
		path.push_back(20);
		path.push_back(19);
	}
	else if(enterNodeA == 26 && _carNumber%3 == 2) {
		path.push_back(22);
		path.push_back(19);
	}
	else if(enterNodeA == 27)
		path.push_back(19);
	else
		cout << endl << "Something's wrong with the manual writing of the paths";

	path.push_back(exitNodeA);
	path.push_back(exitNodeB);
}

Car::~Car() {

}


//### Getters ###

double Car::length() {return carLength;}

int Car::number() {	return carNumber;}

int Car::enteringTime() {return enterTime;}

int Car::enteringNodeA() { return enterNodeA;}

int Car::enteringNodeB() { return enterNodeB; }

int Car::exitingNode() { return exitNodeB; }

int Car::Node() { return path[position]; }

int Car::nextNode() { return path[position+1]; }

int Car::prevNode() { return path[position-1]; }

bool Car::existence() {
	if(fake)
		return false;
	else
		return true;
}

double Car::speed() { return carSpeed; }

int Car::reactIter() { return reacIter; }

int Car::reactDuration() { return reacDuration; }

double Car::distanceInTA() { return distInTA; }

double Car::accMean() { return meanAcceleration; }

double Car::deccMean() { return meanDecceleration; }

vector<int> Car::prog() { return progression; }

double Car::maxSpeed(int road) { return maxSpeeds[road]; }


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

void Car::initSpeed() {
	carSpeed = 0;
}
void Car::iterSpeed(double newSpeed) {
	carSpeed = newSpeed;
}

void Car::addSpeed(double speed) {
	maxSpeeds.push_back(speed);
}

void Car::postponedEnteringTime(int timestep){
	enterTime += timestep;
}