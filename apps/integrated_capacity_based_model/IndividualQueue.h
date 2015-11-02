#pragma once

#include "Car.h"

#include <vector>
#include <list>
#include <map>

enum TurningMovementType {Thru,Uturn,Left,Right};

class IndividualQueue
{
public:
	IndividualQueue(double _length, std::vector<std::pair<int,TurningMovementType> > _turningMovements, 
		double width = 3.6576, double baseSaturationFlowRate = 19./36., double approachGrade = 0., 
		double parkingFactor = 1., double busFactor = 1.);
	IndividualQueue operator=(const IndividualQueue& iq);

	//### Dynamic methods ###
	int getNumberOfCars() const;
	double getTotalLengthLeft() const; //Return the length that is free in the queue
	double getStuckSectionLength() const;
	double getFreeFlowSectionLength() const;
	bool isEmpty() const;
	void insertCar(Car* car);
	std::map<int,double> IndividualQueue::computeCapacities() const;
	bool moveStuckCar(); //Try to move a single car

	//### Static methods ###
	double getLength() const;
	std::vector<std::pair<int,TurningMovementType> > getTurningMovements() const;
	void speak();
	bool isInTurningMovements(int i) const;

private:
	//Lane characteristics
	const double length;
	std::map<int,double> capacities; //first : nextRoad Id ; second : corresponding base capacity
	const std::vector<std::pair<int,TurningMovementType> > turningMovements;
	std::list<Car*> freeFlowSection;
	std::list<Car*> stuckSection;
	double stuckSectionLength;
	double freeFlowSectionLength;
};