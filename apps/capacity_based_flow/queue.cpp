#include "queue.h"

using namespace std;

//### Constructors & Destructors ###
Queue::Queue() {

}

Queue::Queue(int ID, double _maxLength, double _distanceBetweenCars, map<int,double> _capacities, map<int,double> _greenTime, map<int,double> _cycle, map<int,double> _offset) : 
greenTime(_greenTime), cycle(_cycle), offset(_offset)
{
	queueID = ID;
	maxLength = _maxLength;
	distanceBetweenCars = _distanceBetweenCars;
	capacities = _capacities;
	
	cars.clear();
	lengthOverTime.clear();
	nextNodes.clear();
	for(map<int, double>::iterator it = _capacities.begin() ; it != _capacities.end() ; it++) {
		nextNodes.push_back(it->first);
	}
}

Queue::Queue(Json::Value value)
{
	try
	{
		if(value["queueID"].isInt())
			queueID = value["queueID"].asInt();
		else
			throw string("queueID information missing");
		if(value["maxLength"].isDouble())
			maxLength = value["maxLength"].asDouble();
		else
			throw string("maxLength information missing");
		if(value["distanceBetweenCars"].isDouble())
			distanceBetweenCars = value["distanceBetweenCars"].asDouble();
		else
			throw string("distanceBetweenCars information missing");
		nextNodes = jsonToVectInt(value["nextNodes"],"nextNodes");
		capacities = jsonToMapIntDouble(value["capacities"],"capacities");
		greenTime = jsonToMapIntDouble(value["greenTime"],"greenTime");
		cycle = jsonToMapIntDouble(value["cycle"],"cycle");
		offset = jsonToMapIntDouble(value["offset"],"offset");
		jsonToVector(value["cars"],"cars",cars);
		lengthOverTime = jsonToVectorDouble(value["lengthOverTime"],"lengthOverTime");
	}
	catch(string &const st)
	{
		cerr << st << endl;
	}
}

Queue::~Queue() {

}

//### Getters ###

int Queue::ID() {
	return queueID;
}

double Queue::length() {
	double queueLength = 0;
	for(vector<Car>::iterator it = cars.begin() ; it != cars.end() ; it++) {
		queueLength += it->length() + distanceBetweenCars;
	}
	return queueLength;
}

vector<Car>& Queue::getQueue() {
	return cars;
}

map<int, double> Queue::getCapacities() {
	return capacities;
}

map<int,double> Queue::getGreenTime()
{
	return greenTime;
}

map<int,double> Queue::getCycle()
{
	return cycle;
}

map<int,double> Queue::getOffset()
{
	return offset;
}

//### Dynamic methods ###

int Queue::weight(int nextNode) {
	int weight;
	bool exist = false;
	for(vector<int>::iterator it = nextNodes.begin() ; it != nextNodes.end() ; it++) {	// To verify is the nextNode (Where the car is going) is part of the turning movement of this queue
		if((*it) == nextNode) {
			exist = true;
			break;
		}
	}

	if(exist && length() < maxLength)	//(exist == true) -> means that the nextNode is part of the turning movement of the present queue && length() is the length of all the vehicles in the queue
		weight = cars.size()*2 + nextNodes.size()*5; // This is the model comparing the queues. 2 and 5 are the linear coefficient affected to the number of cars and number of turning movement of the road
	else
		weight = 900000;

	return weight;
}

void Queue::iterCarsProg() {
	int iter = 1;
	double queueLength = 0;
	for(vector<Car>::iterator it = cars.begin() ; it != cars.end() ; it++) {
		if(it->existence()) {
			it->iterProg(iter);
			iter += 1;
		}
		queueLength += it->length() + distanceBetweenCars;
	}

	if(queueLength < maxLength)
		lengthOverTime.push_back(queueLength);
	else
		lengthOverTime.push_back(maxLength);
}

void Queue::addCar(Car C) {
	cars.push_back(C);
}

void Queue::removeCar() {
	Car fakeCar;
	cars[0] = fakeCar;
}

void Queue::moveFakeCars(int timestep) {
	for(vector<Car>::iterator it = cars.begin() ; it != cars.end() ; it++) {
		if(it->existence() == false && it != cars.end() - 1) {		// The car is fake but not the last one
			if( (it+1)->existence() ) {									// The next car is not false 
				if( (it+1)->reactIter() < (it+1)->reactDuration() )	{		// The car has not already waited for enough time
					//srand(rand()*rand()*time(NULL));
					srand(0);
					int trand = rand()%100;
					int proba = 100*timestep/((it+1)->reactDuration() - (it+1)->reactIter());
					(it+1)->iterReactIter(timestep);
					
					if(proba > trand) {
						(it+1)->initReactIter();
						iter_swap(it,it+1);
					}
				}

				else {
					(it+1)->initReactIter();
					iter_swap(it,it+1);
				}
			}
		}		
	}
	while(cars.size() != 0 && cars.back().existence() == false) {
		cars.pop_back();
	}
}

Json::Value Queue::toJson()
{
	Json::Value queueValue;
	queueValue["queueID"] = Json::Value(queueID);
	queueValue["maxLength"] = Json::Value(maxLength);
	queueValue["distanceBetweenCars"] = Json::Value(distanceBetweenCars);
	queueValue["nextNodes"] = Json::Value(Json::arrayValue);
	for(int i=0;i<nextNodes.size();i++)
		queueValue["nextNodes"].append(Json::Value(nextNodes[i]));
	Json::Value JCapacities;
	for(map<int,double>::iterator it = capacities.begin();it!=capacities.end();it++)
	{
		int key = it->first;
		string sKey = static_cast<ostringstream*>( &(ostringstream() << key) )->str(); //convert int to string
		JCapacities[sKey] = Json::Value(it->second);
	}
	queueValue["capacities"] = JCapacities; 
	Json::Value JGreenTime;
	for(map<int,double>::iterator it = greenTime.begin();it!=greenTime.end();it++)
	{
		JGreenTime[std::to_string(long double(it->first))] = Json::Value(it->second);
	}
	queueValue["greenTime"] = JGreenTime; 
	Json::Value JCycle;
	for(map<int,double>::iterator it = cycle.begin();it!=cycle.end();it++)
	{
		JCycle[std::to_string(long double(it->first))] = Json::Value(it->second);
	}
	queueValue["cycle"] = JCycle; 
	Json::Value JOffset;
	for(map<int,double>::iterator it = offset.begin();it!=offset.end();it++)
	{
		JOffset[std::to_string(long double(it->first))] = Json::Value(it->second);
	}
	queueValue["offset"] = JOffset; 
	Json::Value JCars(Json::arrayValue);
	for(int i=0;i<cars.size();i++)
	{
		JCars.append(cars[i].toJson());
	}
	queueValue["cars"] = JCars;
	Json::Value JLengthOverTime(Json::arrayValue);
	for(int i=0;i<lengthOverTime.size();i++)
	{
		JLengthOverTime.append(Json::Value(lengthOverTime[i]));
	}
	queueValue["lengthOverTime"] = JLengthOverTime;
	return queueValue;
}

bool Queue::operator==(const Queue & q) const
{
	bool result = true;
	result = result && (q.queueID == queueID);
	result = result && (q.maxLength == maxLength);
	result = result && (q.distanceBetweenCars == distanceBetweenCars);
	result = result && (q.nextNodes == nextNodes);
	result = result && (q.capacities == capacities);
	result = result && (q.greenTime == greenTime);
	result = result && (q.cycle == cycle);
	result = result && (q.offset == offset);
	result = result && (q.cars == cars);
	result = result && (q.lengthOverTime == lengthOverTime);
	return result;
}