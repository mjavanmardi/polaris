#pragma once

#include "road.h"


std::vector<std::pair<int, int>> selectCars(int j, int k, std::vector<std::vector<int>> Cars);

void sampleCars(std::vector<std::pair<int,int>>& selectedCars, std::vector<std::vector<int>>& storedCars);

int selectPreviousRoad(std::map<int, Road> Roads, int i, int j);

void releaseCars(Road& R, std::map<int, Road>& Roads, std::vector<std::pair<int,int>> sampledCars, std::vector<std::pair<int,int>>& stuckCars, std::vector<std::pair<int,int>>& movingCars, int timestep);

void deleteStuckCars(std::vector<int>& car, std::vector<std::pair<int,int>> stuckCars, bool& stuck);

void deleteMovingCars(std::vector<int>& car, std::vector<std::pair<int,int>> movingCars, bool& moving);

void deleteCars(std::vector<std::vector<int>>& Cars, std::vector<std::pair<int, int>> movingCars, std::vector< std::pair< int, int>> stuckCars, int nodej); 

void queuesToTravelingAreas(std::map<int, Road>& Roads, std::vector<std::vector<int>> Cars, int timestep);