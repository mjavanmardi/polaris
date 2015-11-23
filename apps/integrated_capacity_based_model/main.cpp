#include <iostream>
#include <list>
#include "EventManager.h"
using namespace std;

int main()
{
	srand(0);
	
	//Build the test network and the test cars
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