#include <iostream>
#include <list>
#include "EventManager.h"
using namespace std;

int main()
{
	srand(0);
	RoadNetwork* testNetwork = new RoadNetwork();
	CarManager* testCars = new CarManager();
	testCars->addCarsToRoadNetWork(testNetwork);

	//testNetwork->roadSpeak();
	//testNetwork->intersectionSpeak();

	double dt = 0.5;
	int numberOfSteps = 35;

	EventManager simulation(dt,numberOfSteps,testNetwork,testCars);
	simulation.run();

	system("pause");
	return 0;
}