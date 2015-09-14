#pragma once

using namespace std;

#include "importNetwork.h"
#include "importCars.h"
#include "carsPath.h"
#include "simulation.h"
#include "outputProgression.h"
#include "outputQueueLength.h"
#include "testCapacity.h"
#include "JsonParser.h"
#include <fstream>


const int timestep = 1;
//const int modelisationTime = 178 * timestep;
const int modelisationTime = 6001 * timestep;
char *db_path = "C:\\Users\\planglois\\Documents\\Polaris_network\\network";


int main(int argc, char **argv) {

//### Import the SQLITE database and initialize the network ###
	map<int, Road> Roads = openRoad(db_path);
	vector<Car>& Cars = openCars(db_path);
	vector<int> enteringTimes = preprocessCars(Cars);
	
	
//### Simulation ###
	int superior = 0;
	vector<int> timestepsToPrint;
	timestepsToPrint.push_back(modelisationTime-1);
	simulation(modelisationTime, Roads, Cars, timestep, timestepsToPrint, superior);

//### Outputs ###
	ofstream file1, file2;
	file1.open("duration.txt", ios::out | ios::trunc);
	file2.open("queue_lengths.txt", ios::out | ios::trunc);
	progressions(Roads, file1, false);		// Convert the cars progressions (Raw Data) into "Entering Time - Duration - Exiting Time" & "Speed Profil". If true, include the speed profile in the output file
	queueLengths(Roads, file2);				// Convert the queues length (Raw Data) into "Average, Std Dev, Length over time, ..."
	
//### Writing road network to json file ###
	/*ofstream strRoad("TEST_network.json",ios::app);
	strRoad << roadsToJson(db_path);
	strRoad.close();*/

//### Writing cars demand to json file ###
	/*ofstream strCar("TEST_cars.json",ios::app);
	strCar << carsToJson(db_path);
	strCar.close();*/

	//map<int,Road> jRoads = jsonToRoads("TEST_network.json");
	//cout << "Road size : " << jRoads.size() << endl;

	//vector<Car> jCars = jsonToCars("TEST_cars.json");
	//cout << "Cars number : " << jCars.size() << endl;
	
	::testing::InitGoogleTest(&argc, argv);
	//RUN_ALL_TESTS();
	system("pause");
	return 0;
}