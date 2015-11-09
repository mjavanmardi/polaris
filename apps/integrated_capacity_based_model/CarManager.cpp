#include "CarManager.h"

using namespace std;

CarManager::CarManager()
{
	// ### Building test cars ###
	CarType typeDodge(4.42,0.58034612,0.38614873,0.68841545,6.2254787,3.30777879);

	//path0
	vector<int> path0;
	path0.push_back(1);
	path0.push_back(0);
	path0.push_back(3);

	//path1
	vector<int> path1;
	path1.push_back(2);
	path1.push_back(0);
	path1.push_back(3);

	//path2
	vector<int> path2;
	path2.push_back(3);
	path2.push_back(0);
	path2.push_back(1);

	//path3
	vector<int> path3;
	path3.push_back(2);
	path3.push_back(0);
	path3.push_back(1);

	int id = 0;
	Car* a = new Car(id,typeDodge,path0,0);
	id++;
	Car* b = new Car(id,typeDodge,path1,0);
	id++;
	Car* c = new Car(id,typeDodge,path2,0);
	id++;
	
	cars.push_back(a);
	cars.push_back(b);
	cars.push_back(c);

	for(int i=0;i<30;i++)
	{
		Car* d = new Car(id,typeDodge,path0,rand()%10);
		cars.push_back(d);
		id++;
	}

	for(int i=0;i<30;i++)
	{
		Car* e = new Car(id,typeDodge,path1,rand()%10);
		cars.push_back(e);
		id++;
	}

	for(int i=0;i<30;i++)
	{
		Car* f = new Car(id,typeDodge,path2,rand()%10);
		cars.push_back(f);
		id++;
	}

	for(int i=0;i<30;i++)
	{
		Car* g = new Car(id,typeDodge,path3,rand()%10);
		cars.push_back(g);
		id++;
	}

}

CarManager::~CarManager()
{
	for(int i=0;i<cars.size();i++)
	{
		delete(cars[i]);
	}
}

void CarManager::addCarsToRoadNetWork(RoadNetwork* rn)
{
	for(vector<Car*>::iterator it = cars.begin() ; it != cars.end(); it++)
	{
		rn->insertCarInNetwork(*it);
	}
}

vector<Car*>* CarManager::getCars()
{
	return (&cars);
}

void CarManager::speak()
{
	for(vector<Car*>::iterator it = cars.begin();it != cars.end();it++)
	{
		(*it)->speak();
	}
}

//This function is not fixed for the moment. It can be customed to fit the needs
//outputVect is copied into an 'output.txt' file at the end of the algorithm
void CarManager::writeOutput(double time, vector< vector <double > >& outputVect)
{
	for(vector<Car*>::iterator it = cars.begin() ; it != cars.end(); it++)
	{
		if((*it)->getId()==91 || true)
		{
			vector<double> currentCar(0);
			currentCar.push_back((*it)->getId());
			currentCar.push_back(time);
			currentCar.push_back((*it)->getDistanceTraveled());
			currentCar.push_back((*it)->getState());
			currentCar.push_back((*it)->currentRoadId());
			currentCar.push_back((*it)->getCurrentLane());
			currentCar.push_back((*it)->getDistanceInCurrentRoad());
			outputVect.push_back(currentCar);
			//return;
		}
	}
}