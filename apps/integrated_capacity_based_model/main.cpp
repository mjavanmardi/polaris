#include <iostream>
#include <list>
#include "EventManager.h"
#include "Polaris_PCH.h"
using namespace std;

int main(int argc,char** argv)
{
	srand(0);
	//RoadNetwork* testNetwork;
	//CarManager* testCars;
	//if (argc < 2) // build the hrd-coded test network 
	//{
	//	//Build the test network and the test cars (defalut constructors build default network and cars)
	//	testNetwork = new RoadNetwork();
	//	testCars = new CarManager();
	//}
	//else
	//{
	//	string  database_name = argv[1];
	//	unique_ptr<odb::database> db (open_sqlite_database<unique_ptr<odb::database> >(database_name));
	//				//odb::transaction t(db->begin());
	//}
	if (argc>=2)
	{
		string  database_name = argv[1];
		unique_ptr<odb::database> db (open_sqlite_database<unique_ptr<odb::database> >(database_name));
	}

	RoadNetwork* testNetwork = new RoadNetwork();
	CarManager* testCars = new CarManager();
	//Set the simulation parameters
	double dt = 0.25;
	int numberOfSteps = 500;

	//Create and run the simulation
	EventManager simulation(dt,numberOfSteps,testNetwork,testCars);
	simulation.run();

	//Delete the network and the cars
	delete testNetwork;
	delete testCars;

	system("pause");
	return 0;
}