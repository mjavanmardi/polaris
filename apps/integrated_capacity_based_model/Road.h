#pragma once

#include <iostream>

#include "TravelingArea.h"
#include "CommonQueue.h"
#include "JunctionArea.h"
#include "Intersection.h"

class TravelingArea;
class CommonQueue;
class JunctionArea;
class Intersection;

class Road
{
public:
	Road(int _id, TravelingArea* _travArea, CommonQueue* _comQueue, JunctionArea* _juncArea, 
		Intersection* _intersectionA, Intersection* _intersectionB, double _length);
	~Road();
	TravelingArea* getTA() const;
	CommonQueue* getCQ() const;
	JunctionArea* getJA() const;
	int getId() const;
	void speak() const;

	//### Dynamic methods ###
	double getRoomLeftInTravelingArea();

private:
	const int id;
	const double length;
	TravelingArea* travArea;
	CommonQueue* comQueue;
	JunctionArea* juncArea;
	Intersection* intersectionA;
	Intersection* intersectionB;
};