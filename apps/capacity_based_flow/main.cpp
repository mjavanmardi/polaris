#pragma once

using namespace std;


#include "io/Io.h"
#include "io/database.h"
#include "simulation.h"
#include "output.h"
//#include <sqlite3.h>		// To read the sqlite database
//#include <odb/sqlite/database.hxx>		// To read odb::sqlite::database

const int timestep = 1;
const int modelisationTime = 3000 * timestep;

int main() {
//### Initialization 
	//Transform the network input data (Graph) into a network
	//Transform the cars input data into pathes
	map<int, Road> Roads;
	Road R1(1, 2, 14, 150, 1), R2(2, 3, 14, 150, 1), R3(3, 4, 14, 150, 1), R4(4, 5, 14, 150, 1);
	
	map<int, float> capa1, capa2, capa3, capa4;
	capa1[3] = 1600;
	capa2[4] = 1600;
	capa3[5] = 1600;
	R1.addQueue(1, 30, 1, capa1);
	R2.addQueue(1, 30, 1, capa2);
	R3.addQueue(1, 30, 1, capa3);
	R4.addQueue(1, 30, 1, capa4);

	Roads[1] = R1;
	Roads[2] = R2;
	Roads[3] = R3;
	Roads[4] = R4;



	vector<Car> Cars;
	Car C(false, 1, 5, 1, 1, 1, 5, 2);
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