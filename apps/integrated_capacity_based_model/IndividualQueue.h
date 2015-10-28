#pragma once

#include "Car.h"

#include <vector>
#include <list>

class IndividualQueue
{
public:
	IndividualQueue(double _length, std::vector<int> _turningMovements);
	IndividualQueue& operator=(const IndividualQueue& iq);
	double getLength() const;
	std::vector<int> getTurningMovements() const;
	void speak();
private:
	const double length;
	const std::vector<int> turningMovements;
	std::list<Car*> freeFlowSection;
	std::list<Car*> stuckSection;
};