#include "totravarea.h"

using namespace std;

//Each line of cars contains the cars authorized to move for a given queue
//the output of this function allows us given one roadID ->
//to have a vector containing the informations about the car coming to the road roadID at this timestep.
//nodesToID gives a road's ID from the road's nodeA and nodeB
map<int,vector<ComingCar>> sortCars(vector<MovingCars> cars, const vector<vector<int>> &nodesToID)
{
	map<int,vector<ComingCar>> roadToComingCars;
	ComingCar currentCar;
	for(vector<MovingCars>::iterator mcIterator = cars.begin();mcIterator != cars.end();mcIterator++)
	{
		for(int i=0;i<mcIterator->getNextNodesSize();i++)
		{
			currentCar.carID = mcIterator->getCarID(i);
			currentCar.formerRoadID = mcIterator->getRoadID();
			currentCar.formerQueueID = mcIterator->getQueueID();
			//We add the current car to the corresponding vector in the dictionary
			roadToComingCars[nodesToID[mcIterator->getNodeB()][mcIterator->getNextNode(i)]].push_back(currentCar); 
		}
	}
	return roadToComingCars;
}

//For a given road roadIterator->first, this function tries to move 
//the first car authorized to move (which is given by roadIterator->second[0])
//from its former road to the current road roadIterator->first
bool tryToMoveFirstCar(map<int,vector<ComingCar>>::iterator roadIterator, map<int, Road>& Roads, int timeStep)
{
	bool isMoving = false;
	//Renaming variables
	int formerRoadID = roadIterator->second[0].formerRoadID;
	int formerQueueID = roadIterator->second[0].formerQueueID;
	int currentRoadID = roadIterator->first;
	int carID = roadIterator->second[0].carID;
	double maxLengthLeft = Roads[currentRoadID].getMaxCommonQueueLength();
	double queueLengthAfterCarHasEntered =  Roads[currentRoadID].getCommonQueueLength() + (Roads[currentRoadID].getTA().size()/Roads[currentRoadID].getNumberOfIndivQueues());
	if(Roads[formerRoadID].getQueue(formerQueueID).getCarsBegin()->existence() == false) //If first car from former road is fake
	{
		Roads[formerRoadID].moveFakeCars(formerQueueID, timeStep); //We just move the fake cars
	}
	else if(maxLengthLeft > queueLengthAfterCarHasEntered) //If there is room left the car enters the road
	{
		isMoving = true; //There is at least one car which is moving at this step
		Car currentCar = Roads[formerRoadID].removeCarFromQueue(formerQueueID,carID,timeStep); //We remove the car from its former queue
		Roads[currentRoadID].addCarToTA(currentCar); //We add it to its new queue
		roadIterator->second.erase(roadIterator->second.begin()); //We remove the ComingCar information corresponding to the current car
	}
	else
		cout << "Car " << carID << " has no room to move now" << endl;
	return isMoving;
}

//This function moves the authorized cars from their queues to the new roads they are going to
void queuesToTravelingAreas(map<int, Road>& Roads, vector<MovingCars> cars, int timestep, vector<vector<int>> &nodesToID)
{
	map<int,vector<ComingCar>> roadToComingCars = sortCars(cars,nodesToID); //See documentation of this function
	bool isMoving = true; // this boolean is true as long as it's possible to move cars
	while(isMoving && (roadToComingCars.size() != 0)) //While it's possible to move cars
	{
		for(map<int,vector<ComingCar>>::iterator roadIterator = roadToComingCars.begin();roadIterator != roadToComingCars.end();roadIterator++)
		{
			isMoving = tryToMoveFirstCar(roadIterator,Roads,timestep); //We try to move one car to the raod roadIterator->first
			if(roadIterator->second.size()==0) //if there is no car to move anymore in roadIterator->first
				roadToComingCars.erase(roadIterator); //We remove this line from the dictionary roadToComingCars
		}
	}
}