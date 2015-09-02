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
	for(int t = 0 ; t < modelisationTime ; t++) {

	//### Add new cars into the system
		addNewCars(Cars, Roads, t, timestep);

	//### Move from common queue & Increase cars progression & Write the queue length at time = t &  Store cars that can exit a road to enter a new one BASED ON THE CAPACITY 
		vector<vector<int>> movingCars = preProcess(Roads, timestep, t);

	//### Release cars from traveling areas into queues (Common or Individual)
		travelingAreaToQueues(Roads, timestep);

	//### Release cars from queues to traveling areas -> cars stored previously (Based on capacity) 
		queuesToTravelingAreas(Roads, movingCars, timestep);
		
	//### Display the network state if needed => This part has not been optimized (Computational part) and it takes part even if it doesn't print anything
		//printRoads(t, Roads, timestepsToPrint, superior);

		/*if(!checkFor(t,73,Roads))
			t=t;
		cout << "Car size : " << Cars.size() << endl;*/

		if(t%10==0)
			std::cout << "t : " << t << std::endl ; 
	}
}