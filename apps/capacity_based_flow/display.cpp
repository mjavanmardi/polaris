#include "display.h"

using namespace std;


void printQueue(vector<Car> cars) {
	for(vector<Car>::iterator it = cars.begin() ; it != cars.end() ; it++) {
		cout << "Car#" << it->number() << ". ";
	}
}

void printIndivQueues(map<int, Queue>& indivQueues) {
	for(map<int, Queue>::iterator it = indivQueues.begin() ; it != indivQueues.end() ; it++) {
		if(it->second.getQueue().size() != 0) {
			cout << endl << "Queue (ID=" << it->first << ") : ";
			printQueue(it->second.getQueue());
		}
	}
}

void printLastQueue(vector<Car>& lastQueue){ 
	if(lastQueue.size() != 0) {
		cout << endl << "Last Queue : ";
		printQueue(lastQueue);
	}
}

void printCommonQueue(vector<Car>& commonQueue){
	if(commonQueue.size() != 0) {
		cout << endl << "Common Queue : ";
		printQueue(commonQueue);
	}
}

void printTravelingArea(vector<Car>& TA) {
	if(TA.size() != 0) {
		cout << endl << "Traveling Area : ";
		printQueue(TA);
	}
}

void printRoad(Road R) {
	map<int, Queue> queues = R.indivQueues();
	for(map<int, Queue>::iterator it = queues.begin() ; it != queues.end() ; it++) {
		if(it->second.getQueue().size() != 0) {
			cout << endl << "Road #" << R.link() << " : <" << R.nodeA() << "," << R.nodeB() << ">";
			break;
		}
		if(R.getCommonQueue().size() != 0) {
			cout << endl << "Road #" << R.link() << " : <" << R.nodeA() << "," << R.nodeB() << ">";
			break;
		}
		if(R.getTA().size() != 0) {
			cout << endl << "Road #" << R.link() << " : <" << R.nodeA() << "," << R.nodeB() << ">";
			break;
		}
		if(R.getLastQueue().size() != 0) {
			cout << endl << "Road #" << R.link() << " : <" << R.nodeA() << "," << R.nodeB() << ">";
			break;
		}
	}

	printIndivQueues(R.indivQueues());
	printLastQueue(R.getLastQueue());
	printCommonQueue(R.getCommonQueue());
	printTravelingArea(R.getTA());

}

void printRoads(int t, map<int, Road> Roads, vector<int> timestepsToPrint, bool q, int superior) {
	for(vector<int>::iterator it = timestepsToPrint.begin() ; it != timestepsToPrint.end() ; it++) {
		if((*it) == t || q || t <= superior) {
			cout << endl << endl << " TIME " << t;
			for(map<int, Road>::iterator it2 = Roads.begin() ; it2 != Roads.end() ; it2++) {
				printRoad(it2->second);
			}
			break;
		}
	}
}