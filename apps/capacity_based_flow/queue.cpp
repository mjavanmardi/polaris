#include "queue.h"

using namespace std;

//### Constructors & Destructors ###
Queue::Queue() {

}

//Standard constructor
Queue::Queue(int ID, double _maxLength, double _distanceBetweenCars, map<int,double> _capacities, map<int,double> _greenTime, map<int,double> _cycle, map<int,double> _offset) : 
greenTime(_greenTime), cycle(_cycle), offset(_offset)
{
	queueID = ID;
	maxLength = _maxLength;
	distanceBetweenCars = _distanceBetweenCars;
	capacities = _capacities;
	
	cars.clear();
	lengthOverTime.clear();
	nextNodes = vector<int>(0);
	for(map<int, double>::iterator it = _capacities.begin() ; it != _capacities.end() ; it++) {
		nextNodes.push_back(it->first);
	}
}

//consctructor from json value
Queue::Queue(Json::Value value)
{
	try
	{
		queueID = jsonToInt(value["queueID"],"queueID");
		maxLength = jsonToDouble(value["maxLength"],"maxLength");
		distanceBetweenCars = jsonToDouble(value["distanceBetweenCars"],"distanceBetweenCars");
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

int Queue::ID() const
{
	return queueID;
}

//return the length of the current queue
//TODO : compute it inside the queue, not when calling
double Queue::length() const
{
	double queueLength = 0;
	for(vector<Car>::const_iterator carIterator = cars.begin();carIterator!=cars.end();carIterator++) 
	{
		//Car curCar = cars[i];
		queueLength += (*carIterator).length() + distanceBetweenCars;
	}
	return queueLength;
}

vector<Car>& Queue::getQueue() {
	return cars;
}

vector<Car>::iterator Queue::getCarsBegin()
{
	return cars.begin();
}

vector<Car>::iterator Queue::getCarsEnd()
{
	return cars.end();
}

int Queue::getNumberOfCars() const
{
	return cars.size();
}

int Queue::getNextNode(int i) const
{
	return nextNodes[i];
}

int Queue::getNextNodeSize() const
{
	return nextNodes.size();
}

//### Dynamic methods ###

//Gives the weight of a turning movement 
int Queue::weight(int nextNode) const
{
	int weight=0;

	//We check if the nextNode (Where the car is going) can be reached from the current queue
	bool isReachable = false;
	for(int i=0;i<nextNodes.size();i++) {	
		if(nextNodes[i] == nextNode) {
			isReachable = true;
			break;
		}
	}

	//if there is room left in the queue
	//length() is the length of all the vehicles in the queue
	if(isReachable && length() < maxLength)	
		//We have a model to assign a car a queue 
		//The bigger the weight, the less is the driver likely to take it
		//2 and 5 are the linear coefficient affected to the number of cars and number of turning movement of the road
		weight = cars.size()*2 + nextNodes.size()*5; 
	else
		weight = 900000;

	return weight;
}

//Write cars' position in the queue at current step
//Writes the current queue length at current step
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

//This function gives the real (dynamic) capacity of each lane of current queue
//The dynamic capacity takes into account traffic lights
//If at time "time" the traffic light is red, the capacity is null
//otherwise, the static capacity is multiplied by cycleLength/greenTime
//isRed is true whenever there is a green light at current time "time"
map<int,double> Queue::getRealCapacity(const int& time, bool &isRed, double &min) 
{
	min = DBL_MAX;
	isRed = false;
	map<int,double> realCapacity;
	for(map<int,double>::const_iterator it = greenTime.begin();it!=greenTime.end();it++)
	{
		realCapacity[it->first] = 0;
		//if ( (time-offset) modulo cycle < greentime ) then light is green
		double t = fmod((time - offset[it->first]),cycle[it->first]);
		if(t<greenTime[it->first])
		{
			double factor = 1;
			if(greenTime[it->first] != 0)
				factor = cycle[it->first] / greenTime[it->first];
			realCapacity[it->first] = factor*capacities[it->first];
			if(realCapacity[it->first]<min)
				min = realCapacity[it->first];
		}
		else
		{
			isRed = true;
			break;
		}
	}
	return realCapacity;
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