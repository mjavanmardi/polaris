#include "storage.h"

using namespace std;

int lastCarProba(double proba, double weight) { //weight = fractional capacity
	int lastCar = 0;

	//srand(rand()*rand()*time(NULL));
	srand(0);
	int possible = rand()%(int)(1000*weight);

	if(1000*proba > possible)
		lastCar = 1;

	return lastCar;
}

map<int,bool> isGreen(double time, map<int,double> greenTime, map<int,double> cycle, map<int,double> offset)
{
	map<int,bool> green;
	for(map<int,double>::iterator it = greenTime.begin();it!=greenTime.end();it++)
	{
		green[it->first] = false;
		double t = fmod((time - offset[it->first]),cycle[it->first]);
		if(t<greenTime[it->first])
			green[it->first] = true;
		//cout << "green: " << green[i] << " time: " << time << " greentime: " << greenTime[i] << " cycle: " << cycle[i] << " offset: " << offset[i] << endl;
	}
	return green;
}

int numberOfAllowedCars(map<int, double>& capac, vector<Car> queue, int timestep, map<int,bool> green, map<int,double> factor) {
	//cout << " Size2 : " << capac.size() << endl;
	//cout << "capacSize : " << capac.size() << " greenSize : " << green.size() << endl;
	/*for(map<int, bool>::iterator it = green.begin();it != green.end();it++)
		cout << "Loop one : " << it->second << " " << capac[it->first] << endl;
	cout << endl;*/
	double capacMin = DBL_MAX;
	int size = queue.size();
	//for(map<int, double>::iterator it = capac.begin() ; it != capac.end() ; it++) 
	for(map<int, double>::iterator it = capac.begin();it != capac.end();it++)
	{
		//cout << "Loop two : " << green[it->first] << endl;
		if(green[it->first]) //if green light then the initial capacity is amplified by the factor
		{
			it->second *= factor[it->first];
			//cout << "factor : " << factor[i] << endl;
		}
		else
		{
			it->second = 0; // if red light, the capacity is null
			//cout << "No factor : " << factor[i] << endl;
		}
		if(it->second < capacMin) //To see : strange way to do a min
			capacMin = it->second;
		if(capacMin == 0)
			return 0;
	}
	//cout << endl;
	/*for(map<int, bool>::iterator it = green.begin();it != green.end();it++)
		cout << "Loop three : " << it->second << endl;
	cout << endl;*/
	//cout << "diff : " << capac.size() - green.size() << endl;
	/*while(true)
	{}*/
	double minNumberOfCars = capacMin * timestep / 3600;
	int realNumberOfCars = 0;

	bool q = true;
	int iter = 0;
	while(q && queue.size() >iter) {
		Car C = queue.at(iter);

		double absCapacity = capacMin;
		if(C.existence() == true)
			absCapacity = capac[C.nextNode()];	// Calculate the capacity of the turning movement the car is looking for out of all that are allowed on this lane
		double carWeight = capacMin/absCapacity;	// Calculate the weight of the car ; included in ]0;1] ; equals to one for the turning movement with the smallest capacity
		if(minNumberOfCars > carWeight) {				// If the carweight is superior to the number of car that can cross the intersection, than the car is 
			iter += 1;						// To check in the next loop if the next car can cross the intersection
			minNumberOfCars -= carWeight;	// Decrease the total number of car still allowed to cross the intersection
			realNumberOfCars += 1;			// Increase the total number of car really crossing the network
		}
		else {		// For cars that have a weight superior to the minumum number of cars still allowed to crosse
			realNumberOfCars += lastCarProba(minNumberOfCars, carWeight); // There is a probability of having this car crossing the intersection. It depends on its weight and on the remainder of number of cars allowed to crosse
			q = false;
		}
	}
	realNumberOfCars =(realNumberOfCars < size) ? realNumberOfCars : size;
	return realNumberOfCars;
}

vector<int> movingCars(bool& q, int nodeA, int nodeB, int queueID, map<int, double> capac, vector<Car> queue, int timestep, map<int,bool> green, map<int,double> factor) {
	int numberOfCars = numberOfAllowedCars(capac, queue, timestep, green, factor);
	vector<int> newLine;
	if(numberOfCars >0) {
		q = true;
		newLine.push_back(nodeA);
		newLine.push_back(nodeB);
		newLine.push_back(queueID);
		for(int i = 0 ; i < numberOfCars ; i++) {
			int nextNode = -999;
			if(queue.at(i).existence() == true)
				nextNode = queue.at(i).nextNode();
			newLine.push_back(nextNode);
		}
	}
	return newLine;
}

vector<vector<int>> preProcess(map<int, Road>& Roads, int timestep, int time) {
	vector<vector<int>> capacityCars;
	for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
		//### Release cars from Common Queue
		it->second.commonToIndividualQueue();

		//### Write cars progression & Write queues length & Moving Fake Cars (In the individual queues && in the common queue)
		it->second.iterQueuesProg(timestep);
		
		//### Store cars that can exit the system based on the capacity
		map<int, Queue> queues = it->second.indivQueues();
		for(map<int, Queue>::iterator it2 =  queues.begin() ; it2 !=  queues.end() ; it2++) {
			if(it2->second.getQueue().size() != 0) {
				bool q = false;
				map<int,double> greenTime = it2->second.getGreenTime();
				map<int,double> cycle = it2->second.getCycle();
				map<int,double> offset = it2->second.getOffset();
				map<int,bool> green = isGreen(time,greenTime,cycle,offset);
				map<int,double> factor;
				/*for(int i=0;i<green.size();i++)
				{
					cout << "Green : " << green[i] << endl;
				}*/
				for(map<int,double>::iterator it3 = greenTime.begin();it3 != greenTime.end();it3++)
				{
					factor[it3->first] = 1;
					if(greenTime[it3->first]!=0)
					{
						factor[it3->first] = cycle[it3->first] / greenTime[it3->first];
					}
				}
				//cout << "diff : " << it2->second.getCapacities().size() - green.size() << endl; 
				//cout << "Size1 : " << it2->second.getCapacities().size();
				vector<int>  newLine = movingCars(q, it->second.nodeA(), it->second.nodeB(), it2->first, it2->second.getCapacities(), it2->second.getQueue(), timestep, green, factor);
				if(q)
					capacityCars.push_back(newLine);
			}
		}
	
	}
	return capacityCars;
}