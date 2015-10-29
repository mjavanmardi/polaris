#include <iostream>
#include <list>
#include "EventManager.h"
using namespace std;

int main()
{

	RoadNetwork* testNetwork = new RoadNetwork();
	CarManager* testCars = new CarManager();
	testCars->addCarsToRoadNetWork(testNetwork);

	//testNetwork->roadSpeak();
	//testNetwork->intersectionSpeak();

	double dt = 0.5;
	int numberOfSteps = 18;

	EventManager simulation(dt,numberOfSteps);
	simulation.runSimulation(testNetwork,testCars);

	system("pause");
	return 0;
}