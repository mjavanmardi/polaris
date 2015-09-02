#pragma once

#include "gtest/gtest.h"

#include "storage.h"

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
	capac[0] = 1;
	factor[0] = 1;
	std::vector<Car> queue(5);
	int timestep = 1;
	std::map<int,bool> green;
	green[0] = false;
	int testedValue = numberOfAllowedCars(capac,queue,timestep,green,factor);
	ASSERT_EQ(0,testedValue);
}