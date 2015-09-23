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

Car::Car(Json::Value value)
{
	try
	{
		fake = jsonToBool(value["fake"],"fake");
		carNumber = jsonToInt(value["carNumber"],"carNumber");
		carLength = jsonToDouble(value["carLength"],"carLength");
		reacDuration = jsonToInt(value["reacDuration"],"reacDuration");
		enterTime = jsonToInt(value["enterTime"],"enterTime");
		enterNodeA = jsonToInt(value["enterNodeA"],"enterNodeA");
		enterNodeB = jsonToInt(value["enterNodeB"],"enterNodeB");
		exitNodeA = jsonToInt(value["exitNodeA"],"exitNodeA");
		exitNodeB = jsonToInt(value["exitNodeB"],"exitNodeB");
		meanAcceleration = jsonToDouble(value["meanAcceleration"],"meanAcceleration");
		meanDecceleration = jsonToDouble(value["meanDecceleration"],"meanDecceleration");
		carSpeed = jsonToDouble(value["carSpeed"],"carSpeed");
		path = jsonToVectorInt(value["path"],"path");
		reacIter = jsonToInt(value["reacIter"],"reacIter");
		position = jsonToInt(value["position"],"position");
		distInTA = jsonToDouble(value["distInTA"],"distInTA");
		progression = jsonToVectorInt(value["progression"],"progression");
		maxSpeeds = jsonToVectorDouble(value["maxSpeeds"],"maxSpeeds");
	}
	catch(string &const st)
	{
		cerr << st << endl;
	}
}

Car::~Car() {

}


//### Getters ###

double Car::length() const {return carLength;}

int Car::number() const {	return carNumber;}

int Car::enteringTime() const {return enterTime;}

int Car::enteringNodeA() const { return enterNodeA;}

int Car::enteringNodeB() const { return enterNodeB; }

int Car::exitingNode() const { return exitNodeB; }

int Car::Node() const { return path[position]; }

int Car::nextNode() const { return path[position+1]; }

int Car::prevNode() const { return path[position-1]; }

bool Car::existence() const {return (!fake);}

double Car::speed() const { return carSpeed; }

int Car::reactIter() const { return reacIter; }

int Car::reactDuration() const { return reacDuration; }

double Car::distanceInTA() const { return distInTA; }

double Car::accMean() const { return meanAcceleration; }

double Car::deccMean() const { return meanDecceleration; }

vector<int> Car::prog() const { return progression; }

double Car::maxSpeed(int road) const { return maxSpeeds[road]; }


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

Json::Value Car::toJson()
{
	Json::Value carValue;
	carValue["fake"] = Json::Value(fake);
	carValue["carNumber"] = Json::Value(carNumber);
	carValue["carLength"] = Json::Value(carLength);
	carValue["reacDuration"] = Json::Value(reacDuration);
	carValue["enterTime"] = Json::Value(enterTime);
	carValue["enterNodeA"] = Json::Value(enterNodeA);
	carValue["enterNodeB"] = Json::Value(enterNodeB);
	carValue["exitNodeA"] = Json::Value(exitNodeA);
	carValue["exitNodeB"] = Json::Value(exitNodeB);
	carValue["meanAcceleration"] = Json::Value(meanAcceleration);
	carValue["meanDecceleration"] = Json::Value(meanDecceleration);
	carValue["carSpeed"] = Json::Value(carSpeed);
	Json::Value jPath(Json::arrayValue);
	for(int i=0;i<path.size();i++)
		jPath.append(path[i]);
	carValue["path"] = Json::Value(jPath);
	carValue["reacIter"] = Json::Value(reacIter);
	carValue["position"] = Json::Value(position);
	carValue["distInTA"] = Json::Value(distInTA);
	Json::Value jProgression(Json::arrayValue);
	for(int i=0;i<progression.size();i++)
		jProgression.append(progression[i]);
	carValue["progression"] = Json::Value(jProgression);
	Json::Value jMaxSpeeds(Json::arrayValue);
	for(int i=0;i<maxSpeeds.size();i++)
		jMaxSpeeds.append(maxSpeeds[i]);
	carValue["maxSpeeds"] = Json::Value(jMaxSpeeds);
	return carValue;
}

bool Car::operator==(const Car & C1) const
{
	bool result = true;
	result = result && (C1.fake==fake);
	result = result && (C1.carNumber==carNumber);
	result = result && (C1.carLength==carLength);
	result = result && (C1.reacDuration==reacDuration);
	result = result && (C1.enterTime==enterTime);
	result = result && (C1.enterNodeA==enterNodeA);
	result = result && (C1.enterNodeB==enterNodeB);
	result = result && (C1.exitNodeA==exitNodeA);
	result = result && (C1.exitNodeB==exitNodeB);
	result = result && (C1.meanAcceleration==meanAcceleration);
	result = result && (C1.meanDecceleration==meanDecceleration);
	result = result && (C1.carSpeed==carSpeed);
	result = result && (C1.path==path);
	result = result && (C1.reacIter==reacIter);
	result = result && (C1.position==position);
	result = result && (C1.distInTA==distInTA);
	result = result && (C1.progression==progression);
	result = result && (C1.maxSpeeds==maxSpeeds);
	return result;
}