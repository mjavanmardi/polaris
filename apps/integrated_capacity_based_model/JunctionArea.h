#pragma once

#include "IndividualQueue.h"

class IndividualQueue;

class JunctionArea : private std::vector<std::vector<IndividualQueue>>
{
public:
	JunctionArea(std::vector<std::vector<IndividualQueue>> indivQueues);
	void speak();
	double getLength() const;
private:
	double length;
	const int nbLanes;
	const int nbColumns;
};