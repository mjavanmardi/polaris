#pragma once

#include "road.h"
#include "MovingCars.h"
#include <tuple>
#include <algorithm>

std::vector<MovingCars> selectCars(int j, int k, std::vector<MovingCars> Cars);

void sampleCars(std::vector<std::tuple<int,int,int>>& selectedCars, std::vector<std::vector<int>>& storedCars);

void releaseCars(Road& R, std::map<int, Road>& Roads, std::vector<MovingCars> sampledCars, std::vector<MovingCars>& stuckCars, std::vector<MovingCars>& movingCars, int timestep, std::vector<std::vector<int>> &nodesToID);

void deleteStuckCars(MovingCars& car, std::vector<std::pair<int,int>> stuckCars, bool& stuck);

void deleteMovingCars(MovingCars& car, std::vector<std::pair<int,int>> movingCars, bool& moving);

void deleteCars(std::vector<MovingCars>& Cars, std::vector<MovingCars> movingCars, std::vector<MovingCars> stuckCars, int nodej); 

void queuesToTravelingAreas(std::map<int, Road>& Roads, std::vector<MovingCars> Cars, int timestep, std::vector<std::vector<int>> &nodesToID);