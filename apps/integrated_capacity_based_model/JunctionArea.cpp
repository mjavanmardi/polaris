#include "JunctionArea.h"

using namespace std;

JunctionArea::JunctionArea(vector<vector<IndividualQueue>> indivQueues) :
nbLanes(indivQueues.size()), nbColumns(indivQueues[0].size())
{
	double maxLength = 0;
	for(int i = 0 ; i < indivQueues.size();i++)
	{
		vector<IndividualQueue> lane;
		double laneLength = 0;
		for(int j = 0 ; j < indivQueues[i].size();j++)
		{
			lane.push_back(indivQueues[i][j]);
			laneLength += indivQueues[i][j].getLength();
		}
		push_back(lane);
		if (laneLength > maxLength)
			maxLength = laneLength;
	}
	length = maxLength;
}

void JunctionArea::speak() 
{
	for(int i=0 ; i < size() ; i++)
	{
		for(int j=0 ; j < (*this)[i].size() ; j++)
		{
			cout << "Queue " << i << " " << j << endl;
			at(i).at(j).speak();
		}
	}
}

double JunctionArea::getLength() const
{
	return length;
}