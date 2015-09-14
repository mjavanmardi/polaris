#include "simulation.h"

using namespace std;

bool checkFor(int modelisationTime, int n, map<int, Road> Roads)
{
	for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++)
	{
		Road R = it->second;
		map<int, Queue> queues = R.indivQueues();
		for(map<int, Queue>::iterator it = queues.begin() ; it != queues.end() ; it++) {
			vector<Car> cars = it->second.getQueue();
			for(vector<Car>::iterator it2 = cars.begin() ; it2 != cars.end() ; it2++) {
				if(it2->number()==n)
				{
					cout << "Car " << n << " found at time : " << modelisationTime << " in individual queues." << endl;
					return true;
				}
			}
		}
		vector<Car> travelingCars = R.getTA();
		for(vector<Car>::iterator it = travelingCars.begin() ; it != travelingCars.end() ; it++) {
			if(it->number()==n)
				{
					cout << "Car " << n << " found at time : " << modelisationTime << " in traveling areas." << endl;
					return true;
				}
		}
		vector<Car> lastQueueCars = R.getLastQueue();
		for(vector<Car>::iterator it = lastQueueCars.begin() ; it != lastQueueCars.end() ; it++) {
			if(it->number()==n)
				{
					cout << "Car " << n << " found at time : " << modelisationTime << " in last queues." << endl;
					return true;
				}
		}
		for(vector<Car>::iterator it = R.getCommonQueue().begin();it != R.getCommonQueue().end();it++)
		{
			if(it->number()==n)
			{
				cout << "Car " << n << " found at time : " << modelisationTime << " in common queues." << endl;
				return true;
			}
		}
	}
	cout << "Car " << n << " not found at time : " << modelisationTime << endl;
	return false;
}

void simulation(int modelisationTime, map<int, Road>& Roads, vector<Car>& Cars, int timestep, vector<int> timestepsToPrint, int superior) {
	//### Time measuring ###
	std::map<std::string,double> timeSpent;
	std::clock_t start;
	timeSpent["Add cars"] = 0;
	timeSpent["TA to queue"]  = 0;
	timeSpent["Queue to TA"] = 0;
	timeSpent["Common to individual queue"] = 0;
	timeSpent["Iter Queue Progress"] = 0;
	timeSpent["Store exiting cars"] = 0;
	timeSpent["Green time loop"] = 0;
	timeSpent["isGreen"] = 0;
	timeSpent["start2"] = 0;
	for(int t = 0 ; t < modelisationTime ; t++) {

	

	//### Add new cars into the system
		start = clock();
		addNewCars(Cars, Roads, t, timestep);
		timeSpent["Add cars"] += (clock()-start) / (double)(CLOCKS_PER_SEC);


	//### Move from common queue & Increase cars progression & Write the queue length at time = t &  Store cars that can exit a road to enter a new one BASED ON THE CAPACITY 
		vector<vector<int>> movingCars = preProcess(Roads, timestep, t, timeSpent);

	//### Release cars from traveling areas into queues (Common or Individual)
		start = clock();
		travelingAreaToQueues(Roads, timestep);
		timeSpent["TA to queue"] += (clock()-start) / (double)(CLOCKS_PER_SEC);

	//### Release cars from queues to traveling areas -> cars stored previously (Based on capacity) 
		start = clock();
		queuesToTravelingAreas(Roads, movingCars, timestep);
		timeSpent["Queue to TA"] += (clock()-start) / (double)(CLOCKS_PER_SEC);
		
	//### Display the network state if needed => This part has not been optimized (Computational part) and it takes part even if it doesn't print anything
		//printRoads(t, Roads, timestepsToPrint, superior);

		/*if(!checkFor(t,73,Roads))
			t=t;
		cout << "Car size : " << Cars.size() << endl;*/

		if(t%50==0)
			std::cout << "t : " << t << std::endl ; 
	}
	for(std::map<std::string,double>::iterator it = timeSpent.begin();it!=timeSpent.end();it++)
	{
		cout << "Time spent in " << it->first << " = " << it->second << " s" << endl;
	}
}