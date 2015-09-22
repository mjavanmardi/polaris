#pragma once 


#include "queue.h"
#include "road.h"
#include "car.h"
#include <string>
#include "MovingCars.h"

int lastCarProba(double proba, double weight);

int numberOfAllowedCars(std::map<int, Queue>::iterator queueIterator, int timestep, int time); 

MovingCars movingCars(bool& q, std::map<int, Road>::iterator roadIterator, std::map<int, Queue>::iterator queueIterator, int timestep, int time);

std::vector<MovingCars> preProcess(std::map<int, Road>& Roads, int timestep, int time);
