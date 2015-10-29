#pragma once

#include "Car.h"

#include <vector>
#include <list>

class IndividualQueue
{
public:
	IndividualQueue(double _length, std::vector<int> _turningMovements);
	IndividualQueue operator=(const IndividualQueue& iq);

	//### Dynamic methods ###
	int getNumberOfCars() const;
	double getTotalLengthLeft() const; //Return the length that is free in the queue
	double getStuckSectionLength() const;
	double getFreeFlowSectionLength() const;
	bool isEmpty() const;
	void insertCar(Car* car);

	//### Static methods ###
	double getLength() const;
	std::vector<int> getTurningMovements() const;
	void speak();
	bool isInTurningMovements(int i) const;

private:
	const double length;
	const std::vector<int> turningMovements;
	std::list<Car*> freeFlowSection;
	std::list<Car*> stuckSection;
	double stuckSectionLength;
	double freeFlowSectionLength;
};