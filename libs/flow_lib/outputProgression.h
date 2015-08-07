#pragma once

#include <iostream>
#include <fstream>
#include <iostream>

#include "car.h"
#include "road.h"

std::vector<double> speedDistribInQueue(int nbPlace, int nbImplementation);

std::vector<double> speedDistribOutQueue(int nbImplementation, int duration, double vMax, double accMax);

void moving_in_queue(std::ofstream& fichier, int nbImplementation, int nbPlace);

void staying_in_queue(std::ofstream& fichier, int nbImplementation);

void moving_out_of_queue(std::ofstream& fichier, int nbImplementation, int duration, int maxspeed, double accMax);

void progressionOf(Car C, std::ofstream& file, bool q);

void progressionAt(Road R, std::ofstream& fichier, bool q);

void progressions(std::map<int, Road> Roads,  std::ofstream& fichier, bool q);