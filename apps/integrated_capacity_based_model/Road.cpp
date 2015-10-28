#include "Road.h"

using namespace std;

Road::Road(int _id, TravelingArea* _travArea, CommonQueue* _comQueue, JunctionArea* _juncArea,
	Intersection* _intersectionA, Intersection* _intersectionB, double _length) : 
	id(_id), travArea(_travArea), comQueue(_comQueue), juncArea(_juncArea), 
	intersectionA(_intersectionA), intersectionB(_intersectionB), length(_length)
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