#include "outputProgression.h"

using namespace std;

vector<double> speedDistribInQueue(int nbPlace, int nbImplementation) {		//ADD THE TIMESTEP AS A PARAMETER
	vector<double> speedProfil;
	for(int i=0 ; i< nbImplementation ; i++) {
		double t = (double)i/(double)nbImplementation - 0.5;
		double speed = nbPlace*(60*t*t*t*t - 30*t*t +3.75);
		speedProfil.push_back(speed);
	}
	return speedProfil;
}

vector<double> speedDistribOutQueue(int nbImplementation, int duration, double vMax, double accMax) {
	vector<double> speedProfil;
	if(duration/2 < ceil(vMax/accMax)) { // The car does not reach Vmax
		for(int i = - duration/2 ; i< duration/2 * nbImplementation ; i++) {
			double z = (double) i/ (double)nbImplementation;
			double Vup = accMax * duration /2;
			double speed = 46 * Vup *z*z*z*z/(duration*duration*duration*duration) - 8*Vup *z*z / (duration*duration) + Vup;\
			speedProfil.push_back(speed);
		}
	}
	else { // The car reaches Vmax
		int accDuration = ceil(vMax/accMax);
		double t = vMax/accMax;
		for(int i = 0 ; i< accDuration*nbImplementation ; i++) {
			double z = (double) i/nbImplementation;
			double speed = - vMax *z*z*z*z / (t*t*t*t) +2*vMax / (t*t)*z*z;
			speedProfil.push_back(speed);
		}
		for(int i = 0 ; i< (duration - 2*accDuration)*nbImplementation ; i++) { //More than 0 loop due to the "else" condition
			speedProfil.push_back(vMax);
		}
		for(int i = accDuration*nbImplementation ; i > 0 ; i--) {
			double z = (double) i/nbImplementation;
			double speed = - vMax *z*z*z*z / (t*t*t*t) +2*vMax / (t*t)*z*z;
			speedProfil.push_back(speed);
		}
	}
	return speedProfil;
}

void moving_in_queue(ofstream& fichier, int nbImplementation, int nbPlace) { 
	vector<double> speed = speedDistribInQueue(nbPlace, nbImplementation);
	for(vector<double>::iterator it = speed.begin() ; it != speed.end() ; it++) {
		fichier << (*it) << ", ";
	}
}

void staying_in_queue(ofstream& fichier, int nbImplementation) {
	for(int i=0 ; i< nbImplementation ; i++) {
		fichier << 0 << ", ";
	}
}

void moving_out_of_queue(ofstream& fichier, int nbImplementation, int duration, int maxspeed, double accMax) {
	vector<double> speed = speedDistribOutQueue(nbImplementation, duration, maxspeed, accMax);
	for(vector<double>::iterator it = speed.begin() ; it != speed.end() ; it++) {
		fichier << (*it) << ",";
	}
}

void progressionOf(Car C, ofstream& fichier, bool q) {
	vector<int> prog = C.prog();
	fichier << C.number() << ", " << C.enteringTime() << ", " << prog.size() << ", ";
	if(q) {
		vector<int>::iterator it = prog.begin();
		int nbImplementation = 10;				//  TO BE INTEGRATED OUT OF THE FUNCTION
		int road = 0;
		if(prog.size() != 0) {
		while(it != prog.end()-1) {
			if((*it) > 1 && (*(it+1)) != (*it) ) {
				moving_in_queue(fichier, nbImplementation, (*it)-(*(it+1)));
				it += 1;
			}
			else if((it == prog.begin() && (*it)==0) || ((*it) == 1 && (*(it+1)) == 0)) {
				int duration=1;
				it += 1;
				while((*it) == 0 && it != prog.end()-1 ) {
					duration += 1;
					it += 1;
				}
				moving_out_of_queue(fichier, nbImplementation, duration, C.maxSpeed(road), C.accMax() );
				road += 1;
			}
			else {
				staying_in_queue(fichier, nbImplementation);
				it = it+1;
			}	
		}
		}
	}
	fichier << endl;
}

void progressionAt(Road R, ofstream& fichier, bool q) {
	map<int, Queue> queues = R.indivQueues();
	for(map<int, Queue>::iterator it = queues.begin() ; it != queues.end() ; it++) {
		vector<Car> cars = it->second.getQueue();
		for(vector<Car>::iterator it2 = cars.begin() ; it2 != cars.end() ; it2++) {
			progressionOf((*it2), fichier, q);
		}
	}
	vector<Car> lastCars = R.getTA();
	for(vector<Car>::iterator it = lastCars.begin() ; it != lastCars.end() ; it++) {
		progressionOf((*it), fichier, q);
	}
}

void progressions(map<int, Road> Roads,  ofstream& fichier, bool q) {
	for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {

		//Adding the progression for the last queues where the cars are stored
		int iter=1;
		for(vector<Car>::iterator it2 = it->second.getTA().begin() ; it2 != it->second.getTA().end() ; it2++) {
			it2->iterProg(iter);
			iter += 1;
		}

		//Turning the progression number into a speed profile
		progressionAt(it->second, fichier, q);
	}
}
