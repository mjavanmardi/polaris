#pragma once

#include <iostream>

#include "TravelingArea.h"
#include "CommonQueue.h"
#include "JunctionArea.h"
#include "Intersection.h"

typedef const std::vector< std::vector <Road*> >* MatPointer;
typedef const std::vector< std::vector < std::map <int,std::pair<double,double> > > >* CapaPointer;

class TravelingArea;
class CommonQueue;
class JunctionArea;
class Intersection;

class Road
{
public:
	Road(int _id, TravelingArea* _travArea, CommonQueue* _comQueue, JunctionArea* _juncArea, 
		Intersection* _intersectionA, Intersection* _intersectionB, double _length, double _maxSpeed, 
		MatPointer _nodesToRoad, CapaPointer _capacity);
	~Road();

	//### Static methods ###
	TravelingArea* getTA() const;
	CommonQueue* getCQ() const;
	JunctionArea* getJA() const;
	int getId() const;
	int getNodeA() const;
	int getNodeB() const;
	void speak() const;
	double getLength() const;
	double getMaxSpeed() const;
	MatPointer getNodesToRoad() const;
	CapaPointer getCapacity() const;
	Intersection* getIntersectionA() const;

	//### Dynamic methods ###
	double getRoomLeftInTravelingArea();

	bool moveCarsInCQandTA(double dt); //Move cars in commonQueue and TravelingArea
	bool moveCarsInJA(double dt); //Move cars in JunctionArea

private:
	const int id;
	const double length;
	TravelingArea* travArea;
	CommonQueue* comQueue;
	JunctionArea* juncArea;
	Intersection* intersectionA;
	Intersection* intersectionB;
	const int nodeA;
	const int nodeB;
	const double maxSpeed;
	MatPointer nodesToRoad;
	CapaPointer capacity;
};