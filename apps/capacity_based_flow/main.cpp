#pragma once

using namespace std;

#include "importNetwork.h"
#include "importCars.h"
#include "carsPath.h"
#include "simulation.h"
#include "outputProgression.h"
#include "outputQueueLength.h"

const int timestep = 1;
const int modelisationTime = 3000 * timestep;
char *db_path = "C:\\Users\\ikoval\\Documents\\Polaris_network\\network";

int main() {
	
//### Import the SQLITE database and initialize the network ###
	map<int, Road> Roads = openRoad(db_path);
	vector<Car>& Cars = openCars(db_path);
	vector<int> enteringTimes = preprocessCars(Cars);
	
//### Simulation ###
	clock_t start = clock();
	int superior = 9000;
	vector<int> timestepsToPrint;
	timestepsToPrint.push_back(15000);
	timestepsToPrint.push_back(10000);
	timestepsToPrint.push_back(modelisationTime-1);
	simulation(modelisationTime, Roads, Cars, timestep, timestepsToPrint, superior);
	clock_t end = clock();

//### Outputs ###
	ofstream file1, file2;
	file1.open("duration.txt", ios::out | ios::trunc);
	file2.open("queue_lengths.txt", ios::out | ios::trunc);
	progressions(Roads, file1, false);		// Convert the cars progressions (Raw Data) into "Entering Time - Duration - Exiting Time" & "Speed Profil". If true, include the speed profile in the output file
	queueLengths(Roads, file2);				// Convert the queues length (Raw Data) into "Average, Std Dev, Length over time, ..."

	system("pause");
	return 0;
}