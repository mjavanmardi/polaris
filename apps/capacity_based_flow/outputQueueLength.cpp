#include "outputQueueLength.h"

using namespace std;

void queueOverTime(vector<double> lengthOverTime, ofstream& file) {
	for(vector<double>::iterator it = lengthOverTime.begin() ; it != lengthOverTime.end() ; it++) {
		file << "," << *it;
	}
}

void queueLengths(map<int, Road> Roads, ofstream& file) {
	for(map<int, Road>::iterator it = Roads.begin() ; it != Roads.end() ; it++) {
		file << endl << it->second.link() << "," << it->second.nodeA() << it->second.nodeB();
		queueOverTime(it->second.getLengthOverTime(), file);
	}
}