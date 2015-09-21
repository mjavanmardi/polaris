#pragma once 


#include "queue.h"
#include "road.h"
#include "car.h"
#include <string>
#include "MovingCars.h"

int lastCarProba(double proba, double weight);

std::map<int,bool> isGreen(double time, std::map<int,double> greenTime, std::map<int,double> cycle, std::map<int,double> offset);

int numberOfAllowedCars(std::map<int, double>& capac, std::vector<Car> queue, int timestep, std::map<int,bool> green, std::map<int,double> factor);

std::vector<int> movingCars(bool& q, int roadID, int nodeA, int nodeB, int queueID, std::map<int, double> capac, std::vector<Car> queue, int timestep, bool green, double factor);

std::vector<std::vector<int>> preProcess(std::map<int, Road>& Roads, int timestep, int time);