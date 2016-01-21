#include "IndividualQueue.h"

using namespace std;

typedef vector<pair<int,TurningMovementType> >::const_iterator TurnMovIt ;

IndividualQueue::IndividualQueue(double _length, double _positionInRoad, std::vector<std::pair<int,TurningMovementType> > _turningMovements, 
		double width, double baseSaturationFlowRate, double approachGrade, 
		double parkingFactor, double busFactor):
length(_length), positionInRoad(_positionInRoad), turningMovements(_turningMovements), firstCarDistanceToNextRoad(0)
{
	//### Computing the capacities ###

	//Width factor model : 
	double widthFactor = 1.;
	if(width <3.048)
		widthFactor = 0.96;
	else
		if(width>3.93192)
			widthFactor = 1.04;

	//Approach grade model : 
	double gradeFactor = 1. - approachGrade / 200.;

	//Base factor model in vehicle/s :
	double baseCapacityFactor = baseSaturationFlowRate * widthFactor * 
		gradeFactor * parkingFactor * busFactor;

	for(TurnMovIt it = turningMovements.begin();it != turningMovements.end();it++)
	{
		double areaFactor = 1.;
		switch(it->second)
		{
			case Uturn :
				areaFactor = 0.9;
				break;
			case Left :
				areaFactor = 1./1.05;
				break;
			case Right :
				areaFactor = 1./1.18;
				break;
		}
		double realCapa = baseCapacityFactor * areaFactor;
		staticCapacities[it->first] = pair<double,double>(realCapa,realCapa);
	}

}

IndividualQueue IndividualQueue::operator=(const IndividualQueue& iq)
{
	IndividualQueue queue(iq.getLength(),iq.getPositionInRoad(),iq.getTurningMovements());
	return queue;
}

double IndividualQueue::getLength() const
{
	return length;
}

double IndividualQueue::getPositionInRoad() const
{
	return positionInRoad;
}

vector<std::pair<int,TurningMovementType> > IndividualQueue::getTurningMovements() const
{
	return turningMovements;
}

void IndividualQueue::speak()
{
	cout << "Vehicles in the queue : " ;
	for(list<Car*>::iterator it = carBuffer.begin();it != carBuffer.end();it++)
	{
		cout << (*it)->getId() << " ; " ;
	}
	cout << endl;
}

int IndividualQueue::getNumberOfCars() const
{
	return(carBuffer.size());
}

double IndividualQueue::getTotalLengthLeft() const
{
	return(length-firstCarDistanceToNextRoad);
}

bool IndividualQueue::isEmpty() const
{
	return(carBuffer.size() == 0);
}

void IndividualQueue::insertCar(Car* car)
{
	insertCarIncarBuffer(car);
}

void IndividualQueue::insertCarIncarBuffer(Car* car)
{
	carBuffer.push_back(car);
	if(car->getDistanceToNextRoad() > firstCarDistanceToNextRoad)
		firstCarDistanceToNextRoad = length + positionInRoad - carBuffer.back()->getDistanceInCurrentRoad();
}

//Check if road i is in the turning movements of the current individual queue
//Performance note : this can seem not to be performant. However, there is usually
//a maximum of 3 turning movements per individual queue
bool IndividualQueue::isInTurningMovements(int i) const
{
	bool isHere = false;
	for(TurnMovIt it = turningMovements.begin();it != turningMovements.end();it++)
	{
		if(it->first==i)
		{
			isHere = true;
			break;
		}
	}
	return isHere;
}

map<int,pair<double,double> > IndividualQueue::getStaticCapacities() const
{
	return staticCapacities;
}

//Move the cars located in the last column of the junction area
bool IndividualQueue::moveLastCars(double dt)
{
	bool hasMoved = false;
	if(carBuffer.size() != 0)
	{
		bool firstCarLeft = true;
		double positionFirstVehicle = -1.;
		while(firstCarLeft && carBuffer.size() != 0)
		{
			firstCarLeft = false;
			Car* carMoving = *carBuffer.begin();
			positionFirstVehicle = carMoving->getDistanceInCurrentRoad();
			MoveResult result = carMoving->leaveRoad(dt);
			firstCarLeft = result.getHasChangedState();
			if(result.getHasChangedState())
			{
				//We remove the Car
				carBuffer.erase(carBuffer.begin());
				if(carBuffer.size()==0)
					firstCarDistanceToNextRoad = 0;
				hasMoved = true; //We confirm that the car has actually moved
			}
		}
		//Every car after the removed car are moving
		list<Car*>::iterator it = carBuffer.begin();
		list<Car*>::iterator nextIt = it;
		if(it != carBuffer.end())
			nextIt = next(it);
		while(nextIt != carBuffer.end())
		{
			double frontDistanceAvailable = (*it)->getDistanceInCurrentRoad() - (*nextIt)->getDistanceInCurrentRoad();
			double frontSpeed = (*it)->getSpeed();
			bool hasCarMoved = (*nextIt)->moveQueuing(frontDistanceAvailable,frontSpeed,dt);
			hasMoved = hasMoved || hasCarMoved;
			it++;
			if(it != carBuffer.end())
			    nextIt++;
		}
		if(carBuffer.size() !=0)
			firstCarDistanceToNextRoad = carBuffer.back()->getDistanceToNextRoad();
	}
	return hasMoved;
}

//Move the cars located in the columns before the last column of the junction area
bool IndividualQueue::moveFirstCars(double dt)
{
	bool hasMoved = false;
	if(carBuffer.size() != 0)
	{
		bool firstCarLeft = true;
		double positionFirstVehicle = -1.;
		while(firstCarLeft && carBuffer.size() != 0)
		{
			firstCarLeft = false;
			Car* carMoving = *carBuffer.begin();
			positionFirstVehicle = carMoving->getDistanceInCurrentRoad();
			MoveResult result = carMoving->goToNextIndividualQueue(dt);
			//Car tries to leave queue
			if(result.getHasChangedState())
			{
				//We remove the Car
				carBuffer.erase(carBuffer.begin());
				if(carBuffer.size()==0)
					firstCarDistanceToNextRoad = 0;
			}
			hasMoved = result.getHasMoved(); //We confirm that the car has actually moved
		}
		//Every car after the removed car are moving
		list<Car*>::iterator it = carBuffer.begin();
		list<Car*>::iterator nextIt = it;
		if(it != carBuffer.end())
			nextIt = next(it);
		while(nextIt != carBuffer.end())
		{
			double frontDistanceAvailable = (*it)->getDistanceInCurrentRoad() - (*nextIt)->getDistanceInCurrentRoad();
			double frontSpeed = (*it)->getSpeed();
			bool hasCarMoved = (*nextIt)->moveQueuing(frontDistanceAvailable,frontSpeed,dt);
			hasMoved = hasMoved || hasCarMoved;
			it++;
			if(it != carBuffer.end())
			    nextIt++;
		}
		if(carBuffer.size() !=0)
			firstCarDistanceToNextRoad = length + positionInRoad - carBuffer.back()->getDistanceInCurrentRoad();
	}
	return hasMoved;
}