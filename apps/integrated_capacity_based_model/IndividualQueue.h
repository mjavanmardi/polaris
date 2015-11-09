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
	bool isStuckSectionEmpty() const;
	void insertCar(Car* car);
	void insertCarInStuckSection(Car* car);
	std::map<int,std::pair<double,double> > IndividualQueue::getStaticCapacities() const;
	bool moveLastStuckCar(double dt); //Try to move a single stuck car to the next road
	bool moveLastFreeFlowCars(double dt); //Try to move the cars in free flow area to the next road

	//### Static methods ###
	double getLength() const;
	std::vector<std::pair<int,TurningMovementType> > getTurningMovements() const;
	void speak();
	bool isInTurningMovements(int i) const;

private:
	//Lane characteristics
	const double length;
	std::map<int,std::pair<double,double> > staticCapacities; //first : nextRoad Id ; second : (capacity at the beginning timestep, capacity left)
	const std::vector<std::pair<int,TurningMovementType> > turningMovements; //First value of the pair : id of the next road
	std::list<Car*> freeFlowSection;
	std::list<Car*> stuckSection;
	double stuckSectionLength;
	double freeFlowSectionLength;
};