#pragma once
#include "Car.h"
#include <list>

class Car;

class CommonQueue : private std::list<Car*>
{
public:
	CommonQueue(double _distanceBetweenCars, int _numberOfLanes);

	void addCar(Car* car);
	void removeCar(std::list<Car*>::iterator car);
	bool moveCars();
	void speak();
	double getLength() const;
	int getSize() const;
private:
	double length;
	const double distanceBetweenCars;
	const int numberOfLanes;
};