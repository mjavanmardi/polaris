#pragma once

#include "gtest/gtest.h"
#include <string>
#include <fstream>
#include "storage.h"
#include "importNetwork.h"

bool infEq(double a, double b)
{
	return (a<b);
}

//Tests known values for the isGreen function
TEST (isGreen,knownValue)
{
	std::map<int, double> greenTime, cycle, offset;
	double time = 3.5;
	greenTime[0] = 2;
	cycle[0] = 3;
	offset[0] = 1;
	std::map<int,bool> result = isGreen(time,greenTime,cycle,offset);
	ASSERT_EQ(false,result[0]);
	time = 4.2;
	greenTime[0] = 2;
	cycle[0] = 3;
	offset[0] = 1;
	result = isGreen(time,greenTime,cycle,offset);
	ASSERT_EQ(true,result[0]);
}

//At a red light, no car should pass
TEST (numberOfAllowedCars,redLightCase)
{
	std::map<int,double> capac, factor;
	capac[0] = 7200;
	factor[0] = 1;
	std::vector<Car> queue(5);
	int timestep = 1;
	std::map<int,bool> green;
	green[0] = false;
	int testedValue = numberOfAllowedCars(capac,queue,timestep,green,factor);
	ASSERT_EQ(0,testedValue);
}

//At a green light, number of car allowed should increase with factor
TEST (numberOfAllowedCars,ShouldIncreaseWithFactor)
{
	std::map<int,double> capac, factor;
	capac[0] = 7200;
	factor[0] = 1;
	std::vector<Car> queue;
	Car a;
	for(int i=0;i<7;i++)
	{
		a = Car(true,i,1,1,1,1,27,27,27,27,27); //fake cars
		queue.push_back(a);
	}
	int timestep = 1;
	std::map<int,bool> green;
	green[0] = true;
	int firstNumberofAllowedCars = numberOfAllowedCars(capac,queue,timestep,green,factor);
	capac[0] = 7200;
	factor[0] = 3;
	green[0] = true;
	int secondNumberofAllowedCars = numberOfAllowedCars(capac,queue,timestep,green,factor);
	ASSERT_PRED2(infEq,firstNumberofAllowedCars,secondNumberofAllowedCars);
}

//Check that a car to json to a car is identity
TEST (Car,CarJsonCarIdentity)
{
	Car a = Car(true,0,1,1,1,1,27,27,27,27,27);
	Car b = Car(a.toJson());
	ASSERT_EQ(a,b);
}

//Check that a queue to json to a queue is identity
TEST (Queue,QueueJsonQueueIdentity)
{
	int ID = 0;
	double maxLength = 10;
	double distanceBetweenCars = 1;
	std::map<int,double> capacities;
	std::map<int,double> greenTime;
	std::map<int,double> cycle;
	std::map<int,double> offset;
	capacities[0]=7200;
	greenTime[0]=5;
	cycle[0]=10;
	offset[0]=1;
	Queue qOne(ID,maxLength,distanceBetweenCars,capacities,greenTime,cycle,offset);
	Car a = Car(true,0,1,1,1,1,27,27,27,27,27);
	qOne.addCar(a);
	Queue qTwo(qOne.toJson());
	//std::cout << std::endl << std::endl << qOne.toJson() << std::endl << std::endl; //To check the Json code
	ASSERT_EQ(qOne,qTwo);
}

//Check that a road to json to a road is identity
TEST (Road,RoadJsonRoadIdentity)
{
	int ID = 0;
	int link = 1;
	int anode = 1;
	int bnode = 2;
	double maxSpeed = 80;
	double totalLength = 2000;
	double distanceBC = 1;
	Road rOne(ID,link,anode,bnode,maxSpeed,totalLength,distanceBC);
	std::map<int,double> capacities;
	std::map<int,double> greenTime;
	std::map<int,double> cycle;
	std::map<int,double> offset;
	capacities[0]=7200;
	greenTime[0]=5;
	cycle[0]=10;
	offset[0]=1;
	rOne.addQueue(0,500,1,capacities,greenTime,cycle,offset);
	Road rTwo(rOne.toJson());
	ASSERT_EQ(rOne,rTwo);
}

//Check if the test network can be imported
TEST (RoadNetworkJson,CanBeImported) 
{
	std::string test = fileToString("TEST_network.json"); 
	Json::Value root;
	Json::Reader reader;
	reader.parse(test,root);
	//std::cout << root << std::endl;
	ASSERT_EQ (root.isNull(),false);
}

//Check if the test car demand can be imported
TEST (CarsDemandJson,CanBeImported)
{
	std::string test = fileToString("TEST_cars.json"); 
	Json::Value root;
	Json::Reader reader;
	reader.parse(test,root);
	ASSERT_EQ (root.isArray(),true);
}