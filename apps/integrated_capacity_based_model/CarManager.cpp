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

	Car* a = new Car(0,typeDodge,path0,0);
	Car* b = new Car(1,typeDodge,path1,0);
	Car* c = new Car(2,typeDodge,path2,0);
	
	cars.push_back(a);
	cars.push_back(b);
	cars.push_back(c);

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