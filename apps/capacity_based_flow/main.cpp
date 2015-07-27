#pragma once

using namespace std;

#include "import.h"
#include "simulation.h"
#include "output.h"

const int timestep = 1;
const int modelisationTime = 3000 * timestep;
char *db_path = "C:\\Users\\ikoval\\Documents\\Polaris_network\\test";

int main() {

//### Import the SQLITE database and initialize the network ###
	map<int, Road> Roads = openRoad(db_path);
	
	vector<Car> Cars;
	Car C(false, 1, 5, 1, 1, 2);
	Cars.push_back(C);

//### Raise exeptions : Wrong pathes, wrong roads ...
	

//### Simulation
	bool q = true;
	int superior = 0;
	vector<int> timestepsToPrint;
	timestepsToPrint.push_back(modelisationTime-1);
	simulation(modelisationTime, Roads, Cars, timestep, timestepsToPrint, q, superior);

//### Convert the cars progressions (Raw Data) into "Entering Time - Duration - Exiting Time" & "Speed Profil"
	ofstream file1;
	file1.open("progressions.txt", ios::out | ios::trunc);
	progressions(Roads, file1, false);		//If TRUE, includes the speed profil in the output

//### Conver the queues length (Raw Data) into "Average, Std Dev, Length over time, ..."


	system("pause");
	return 0;
}