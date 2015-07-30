#pragma once

#include <fstream>	//Write in the ofstream file

#include "road.h"

void queueOverTime(std::vector<double> lengthOverTime, std::ofstream& file);

void queueLengths(std::map<int, Road> Roads, std::ofstream& file);