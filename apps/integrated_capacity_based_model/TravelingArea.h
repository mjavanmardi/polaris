#pragma once
#include "Car.h"
#include <set>

class Car;

//Class to implement the distance which is low when a car is at the end of the traveling area
class distInTAComp
{
public:
	bool operator()(const Car* leftCar, const Car* rightCar);
};

//Traveling Area inherits from set<Car*,distInTAComp> so that the storing order
// is the distance to the end of the TravelingArea
class TravelingArea : private std::multiset<Car*,distInTAComp>
{
public:
	TravelingArea(int _numberOfLanes);
	void addCar(Car* car); //add a car to the TA with logarithmic complexity
	void removeCar(multiset<Car*,distInTAComp>::iterator car); //remove a car from the TA with a logarithmic complexity
	double getMinLength() const; // returns the minimum length of the traveling car so that the cars do not touch each other
	bool moveCars(); // returns true if any car has moved
	void speak() const;
private:
	const int numberOfLanes;
	double totalLength; //Sum of the cars length
};