#include "JunctionArea.h"

using namespace std;

JunctionArea::JunctionArea(vector<vector<IndividualQueue>> indivQueues) :
nbLanes(indivQueues.size()), nbColumns(indivQueues[0].size())
{
	double maxLength = 0;
	//Copying the individual queue matrix
	for(int i = 0 ; i < indivQueues.size();i++)
	{
		vector<IndividualQueue> lane;
		double laneLength = 0;
		for(int j = 0 ; j < indivQueues[i].size();j++)
		{
			lane.push_back(indivQueues[i][j]);
			laneLength += indivQueues[i][j].getLength();
		}
		push_back(lane);
		if (laneLength > maxLength)
			maxLength = laneLength;
	}
	length = maxLength;
}

double JunctionArea::getTotalLengthLeft(int i, int j)
{
	return(at(i).at(j).getTotalLengthLeft());
}

double JunctionArea::getQueueLength(int i, int j)
{
	return(at(i).at(j).getLength());
}

double JunctionArea::getFreeFlowSectionLeft(std::pair<int,int> queueCoord)
{
	return(at(queueCoord.first).at(queueCoord.second).getFreeFlowSectionLength());
}

bool JunctionArea::isQueueEmpty(int i, int j)
{
	return(at(i).at(j).isEmpty());
}

bool JunctionArea::isStuckSectionEmpty(pair<int,int> queueCoord)
{
	return(at(queueCoord.first).at(queueCoord.second).isStuckSectionEmpty());
}

void JunctionArea::insertCar(Car* car, pair<int,int> queue)
{
	at(queue.first).at(queue.second).insertCar(car);
}

void JunctionArea::insertCarInStuckSection(Car* car, pair<int,int> queueCoord)
{
	at(queueCoord.first).at(queueCoord.second).insertCarInStuckSection(car);
}

pair<bool,double> JunctionArea::isPathFree(int nextRoad, int initI, int initJ)
{
	double freeFlowDistance = 0.;
	bool isFree = true;
	pair<int,int> position(initI,initJ); 
	for(int j=0;j<nbColumns;j++)
	{
		position = selectNextQueue(position,nextRoad); //We select the next queue to go to
		if(isQueueEmpty(position.first,position.second))
		{
			freeFlowDistance += getQueueLength(position.first,position.second);
		}
		else //If there are vehicles in the current individualQueue
		{
			freeFlowDistance += getTotalLengthLeft(position.first,position.second);
			isFree = false;
			break;
		}
	}
	return(pair<bool,double>(isFree,freeFlowDistance));
}

bool JunctionArea::moveCars(double dt)
{
	bool hasMoved = false;
	bool currentStepMoving = true;
	while(currentStepMoving) //While its possible to move cars during this timestep
	{
		currentStepMoving = false;
		//First we take car of the stuck cars in the last column (about to leave the road)
		bool stuckCarsMove = true;
		while(stuckCarsMove)
		{
			stuckCarsMove = false;
			for(int lane = 0 ; lane < nbLanes;lane++)
			{
				bool currentIterStuckCarsMove = at(lane).at(nbColumns-1).moveLastStuckCar(dt);
				stuckCarsMove = stuckCarsMove || currentIterStuckCarsMove;
			}
		}
		//Then we take care of the free flow cars in the last column
		bool freeFlowCarsMove = true;
		while(freeFlowCarsMove)
		{
			freeFlowCarsMove = false;
			for(int lane = 0 ; lane < nbLanes;lane++)
			{
				bool currentIterFreeFlowMove = at(lane).at(nbColumns-1).moveLastFreeFlowCars(dt);
				freeFlowCarsMove = freeFlowCarsMove || currentIterFreeFlowMove;\
			}
		}
		currentStepMoving = stuckCarsMove || freeFlowCarsMove; //We check if cars have moved during the current iteration
		hasMoved = hasMoved || currentStepMoving; //We check if cars are moving during the whole time steps
	}
	return hasMoved;
}

void JunctionArea::speak() 
{
	for(int i=0 ; i < size() ; i++)
	{
		for(int j=0 ; j < (*this)[i].size() ; j++)
		{
			cout << "Queue " << i << " " << j << endl;
			at(i).at(j).speak();
		}
	}
}

double JunctionArea::getNbLanes() const
{
	return nbLanes;
}

double JunctionArea::getLength() const
{
	return length;
}

pair<int,int> JunctionArea::selectNextQueue(pair<int,int> currentPosition,int nextRoad)
{
	int newLine = -1;
	double weight = DBL_MAX;
	int i = currentPosition.first;
	//We look for a queue in the next column than the current one :
	int newColumn = currentPosition.second + 1;
	if(currentPosition.second = nbColumns-1) //If the car is in the last column
	{
		return(pair<int,int>(-1,-1)); //We return a fake pair which means that the car is about to leave its current road
	}
	else
	{
		if(i==-1) //if the car is not yet in the junction area, it can choose any queue
		{
			for(int j=0;j<nbLanes;j++)
			{
				updateWeightAndLane(weight,newLine,j,newColumn,nextRoad);
			}
		}
		else //if the car is already in the junction are it can only go to a neighbor lane
		{
			//Left lane
			if(i-1>=0) 
				updateWeightAndLane(weight,newLine,i-1,newColumn,nextRoad);
			//StraightForard lane
			updateWeightAndLane(weight,newLine,i,newColumn,nextRoad);
			//Right lane
			if(i+1 < nbLanes)
				updateWeightAndLane(weight,newLine,i+1,newColumn,nextRoad);
		}
	}
	return(pair<int,int>(newLine,newColumn));
}

/*Checks if the queue (newLane,column) is a better choice than (currenetLane,column) which has weight currentWeight
for turning to nextRoad.*/
//It updates currentWeight and currentLane with the relevant values
void JunctionArea::updateWeightAndLane(double& currentWeight,int& currentLane,int newLane,int column, int nextRoad)
{
	if((*this)[newLane][column].isInTurningMovements(nextRoad)) //If road is accessible in the new queue
	{
		double newWeight = (double) at(newLane).at(column).getNumberOfCars();
		if(newWeight < currentWeight) //if the new queue is a better choice
		{
			currentWeight = newWeight; //we update currentWeight and currentLane
			currentLane = newLane;
		}
	}
}

vector<pair<int,TurningMovementType> > JunctionArea::getTurningMovements(int idLane)
{
	//Reminder for the pair ; first value = nextRoadId ; second value = turningMovementType
	return at(idLane).at(nbColumns-1).getTurningMovements();
}

vector<map<int,pair<double,double> > > JunctionArea::getCapacities()
{
	vector<map<int,pair<double,double> > > roadCapacities; //roadCapacities[i] = capacities of lane i 
	//Reminder : lanes are indexed top to bottom when direction of the road is left to right
	for(int i=0;i<nbLanes;i++)
	{
		roadCapacities.push_back(at(i).at(nbColumns-1).getStaticCapacities());
	}
	return roadCapacities;
}