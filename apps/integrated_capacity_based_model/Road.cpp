#include "Road.h"

using namespace std;

Road::Road(int _id, TravelingArea* _travArea, CommonQueue* _comQueue, JunctionArea* _juncArea,
	Intersection* _intersectionA, Intersection* _intersectionB, double _length, double _maxSpeed, 
	MatPointer _nodesToRoad) : 
	id(_id), travArea(_travArea), comQueue(_comQueue), juncArea(_juncArea), 
	intersectionA(_intersectionA), intersectionB(_intersectionB), length(_length),
	nodeA(_intersectionA->getId()),nodeB(_intersectionB->getId()), maxSpeed(_maxSpeed), 
	nodesToRoad(_nodesToRoad)
{

}

Road::~Road()
{
	delete travArea;
	delete comQueue;
	delete juncArea;
}

TravelingArea* Road::getTA() const
{
	return travArea;
}

CommonQueue* Road::getCQ() const
{
	return comQueue;
}

JunctionArea* Road::getJA() const
{ 
	return juncArea;
}

int Road::getId() const
{
	return id;
}

int Road::getNodeA() const
{
	return nodeA;
}

int Road::getNodeB() const
{
	return nodeB;
}

double Road::getMaxSpeed() const
{
	return maxSpeed;
}

MatPointer Road::getNodesToRoad() const
{
	return nodesToRoad;
}

Intersection* Road::getIntersectionA() const
{
	return intersectionA;
}

void Road::speak() const
{
	cout << "Road Id : " << id << " is going from intersection " <<
		intersectionA->getId() << " to intersection " << intersectionB->getId() << endl;
	travArea->speak();
	comQueue->speak();
	juncArea->speak();
	cout << endl << endl;
}

double Road::getRoomLeftInTravelingArea()
{
	return max(0.,(length - juncArea->getLength() - comQueue->getLength()));
}

bool Road::moveCarsInCQandTA(double dt)
{
	bool hasMoved = false;
	hasMoved = hasMoved || comQueue->moveCars();
	hasMoved = hasMoved || travArea->moveCars(dt);
	return hasMoved;
}

bool Road::moveCarsInJA()
{
	bool hasMoved = false;
	hasMoved = juncArea->moveCars();
	return hasMoved;
}

double Road::getLength() const
{
	return length;
}