#pragma once

#include "car.h"
#include "io/database.h"
#include "io/Io.h"

using namespace odb;
using namespace polaris::io;


Car importCar(int enteringTime, int origin, int destination, int carNumber, std::vector<int> location, std::vector<int> nodeA, std::vector<int> nodeB);

void preprocessLocations(result<polaris::io::Location> locations, std::vector<int>& location, std::vector<int>& nodeA, std::vector<int>&nodeB);

std::vector<Car> openCars(char *db_path);