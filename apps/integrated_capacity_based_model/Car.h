#pragma once
#include <vector>
#include <cmath>
#include "CarType.h"
#include "Road.h"

class Road;

//This class just allows the cars to inform their parent road structure 
//if they have been able to move or not
class MoveResult 
{
public:
	MoveResult(bool _hasMoved, bool _hasChangedState) :
	  hasMoved(_hasMoved), hasChangedState(_hasChangedState){}
	bool getHasMoved() const {return hasMoved;}
	bool getHasChangedState() const {return hasChangedState;}
private:
	const bool hasMoved;
	const bool hasChangedState;
};

//Enumeration of all possible states for a car
enum CarState {in_WaitingForEntry,in_TravelingArea,in_CommonQueue,in_JunctionArea,in_OutOfNetwork};

class Car
{
public:
	Car(int _id, CarType _type, std::vector<int> _path, double _enteringTime);
	//MoveResult move(double dt); //Moves the car during current time step.
	void writeOutput(); //Todo : choose a format for the output

	//### Moving procedures ###
	//Here we declare all the procedure to move the car 
	//depending on the car's situation in the network
	MoveResult tryToEnterRoad(Road* road);
	MoveResult travelingAreaMove(double dt);
	MoveResult travelingCommonQueue(double dt);
	MoveResult leaveRoad(double dt); 
	MoveResult moveFromLastFreeFlowArea(double dt);
	MoveResult goToNextIndividualQueue(double dt);

	void postponeEnteringTime(double time);
	void initTimeStep(double dt); //Set up the car at the beginning of a time step
	bool moveQueuing(double frontDistanceAvailable,double frontSpeed, double dt);

	bool lastCarProba(double remainingAllowedWeight, double lastCarWeight) const;
	void setSpeedZero(); //Used when a car enters a queue

	//### Dynamic travel model ###
	//Free flow model. See documentation

	void updateSpeedAndDistance(double dt, double alpha);
	double computeAlpha(double dt, double distToStop);
	double dBraking(double dt, double acceleration);

	//### Getters ###

	double getDistanceInTA() const;
	double getDistanceTraveled() const;
	double getLength() const;
	int getId() const;
	int getEntryNode() const;
	int getNextNode() const;
	double getEnteringTime() const;
	CarState getState() const;
	int getCurrentLane() const;
	int currentRoadId() const;
	double getDistanceInCurrentRoad() const;
	double getDistanceToJunctionArea() const;
	double getSpeed() const;
	double getSafetyDistance(double frontCarSpeed, double maxSpeed) const;
	double getDistanceToNextRoad() const;

	void speak();

private:
	CarState state; //Main state variable

	//### Secondary state variables ###
	double enteringTime;
	double speed; // current speed
	std::vector<int>::iterator nextNodeIterator; //iterator on the next node the car is heading at
	Road* currentRoad; // Pointer to the current Road
	std::pair<int,int> currentIndivQueue; // Coordinate of the current individual queue ((-1,-1) if out of the junction area)
	double maxDistanceInCurrentTimeStep; // Maximum distance the car can move during the current time step
	double distanceInTimeStep; //Distance moved during the current time step
	double distanceInTA; //Distance traveled in the current traveling area
	double distanceInCurrentRoad; //Distance traveled by the car in the current road

	double distanceTraveled; //Distance travelled by the car since its entry in the network

	//### Car caracteristics ###
	const int id; //Id of the car
	const CarType type; //Physical characteristics of the car
	std::vector<int> path; //Successive nodes the car has scheduled to follow
	const int entryNode; //Node where the car enters the network
	const int exitingNode; //Node where the car exits the network

};
