#include <iostream>
#include <list>
#include "CarManager.h"
#include "RoadNetwork.h"
using namespace std;

int main()
{

	RoadNetwork* testNetwork = new RoadNetwork();
	CarManager* testCars = new CarManager();
	testCars->addCarsToRoadNetWork(testNetwork);
	testNetwork->roadSpeak();
	testNetwork->intersectionSpeak();

	system("pause");
	return 0;
}