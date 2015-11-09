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

	double dt = 0.25;
	int numberOfSteps = 500;

	EventManager simulation(dt,numberOfSteps,testNetwork,testCars);
	simulation.run();

	delete testNetwork;
	delete testCars;

	system("pause");
	return 0;
}