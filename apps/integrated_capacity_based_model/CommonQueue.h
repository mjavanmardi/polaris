#pragma once

#include <list>
#include "Car.h"

class Car;

class CommonQueue : private std::list<Car*>
{
public:
	CommonQueue(int _numberOfLanes);

	void addCar(Car* car);
	bool moveCars(double dt);
	void speak() const;
	double getLength() const;
	int getSize() const;
	int getNumberOfLanes() const;
private:
	const int numberOfLanes;
	double length;
};