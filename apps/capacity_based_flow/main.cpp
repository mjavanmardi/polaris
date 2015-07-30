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
char *db_path = "C:\\Users\\ikoval\\Documents\\Polaris_network\\test";

int main() {

//### Import the SQLITE database and initialize the network ###
	map<int, Road> Roads = openRoad(db_path);
	
	vector<Car>& Cars = openCars(db_path);
	preprocessCars(Cars);
	// CALCULATE_THE_PATH_OF_CARS(Cars); SHOULD BE DONE in the carsPath.h/cpp files

	//### THIS NEXT PART IS HERE TILL THERE IS NO PROCESS IN "CARSPATH" TO CALCULATE THE PATH OUT OF THE ENTERING AND EXITING NODE ###
	//vector<Car> Cars;
	//Car C(false, 1, 5, 1, 1, 2);
	//Cars.push_back(C);
	//### END OF THE TEMPORARY SECTION

//### Simulation
	bool q = true;
	int superior = 0;
	vector<int> timestepsToPrint;
	timestepsToPrint.push_back(modelisationTime-1);
	simulation(modelisationTime, Roads, Cars, timestep, timestepsToPrint, q, superior);


//### Outputs ###
	ofstream file1, file2;
	file1.open("progressions.txt", ios::out | ios::trunc);
	file2.open("queue_lengths.txt", ios::out | ios::trunc);
	progressions(Roads, file1, false);		// Convert the cars progressions (Raw Data) into "Entering Time - Duration - Exiting Time" & "Speed Profil". If true, include the speed profile in the output file
	queueLengths(Roads, file2);				// Convert the queues length (Raw Data) into "Average, Std Dev, Length over time, ..."

	system("pause");
	return 0;
}