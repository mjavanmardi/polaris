#pragma once
#include <vector>
#include <cmath>
#include "CarType.h"
#include "Road.h"

class Road;

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
	//Here we declare all the procedure to move the car depending on the car state
	MoveResult tryToEnterRoad(Road* road);
	MoveResult travelingAreaMove(double dt);
	MoveResult travelingCommonQueue();

	void addDistanceTraveled(double distance);
	void postponeEnteringTime(double time);
	void initTimeStep(double dt); //Set up the car at the beginning of a time step

	//### Dynamic model ###

	void updateSpeedAndDistance(double dt, double alpha);
	double computeAlpha(double dt, double distToStop);
	double dBraking(double dt, double acceleration);

	//### getters ###

	double getDistanceInTA() const;
	double getLength() const;
	int getId() const;
	int getEntryNode() const;
	int getNextNode() const;
	double getEnteringTime() const;

	void speak();

private:
	CarState state; //Main state variable

	//### Secondary state variables ###
	double enteringTime;
	double distanceInTA; //Distance traveled in the current traveling area
	double speed; // current speed
	std::vector<int>::iterator nextNodeIterator; //iterator on the next node the car is heading at
	Road* currentRoad; // Pointer to the current Road
	std::pair<int,int> currentIndivQueue; // Coordinate of the current individual queue ((-1,-1) if out of the junction area)
	double maxDistanceLeftInCurrentTimeStep; // Maximum distance the car can move during the current time step
	double distanceInTimeStep; //Distance moved during the current time step

	double distanceTraveled; //Distance travelled by the car since its entry in the network

	//### Car caracteristics ###
	const int id;
	const CarType type;
	std::vector<int> path; //Successive nodes the car has scheduled to follow
	const int entryNode; //Node where the car enters the network
	const int exitingNode;

};
