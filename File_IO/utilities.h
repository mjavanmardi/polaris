#ifndef UTILITIES_H
#define UTILITIES_H

#define INFINITY_FLOAT 9999999

//io
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

//container
#include <string>
#include <hash_map>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <iterator>

//random number
#include "RngStream.h"

//algorithms
#include <math.h>
#include <algorithm>
#include <utility>
#include <functional>
#include <omp.h>

#ifdef LINUX
#include <sys/time.h>
#else
#include <windows.h>
#endif

#ifdef LINUX
// for hash_map
using namespace __gnu_cxx;
#endif

using namespace std;

//template<class T>
//string to_string(T t);

string left(string s, int pos);

string right(string s, int pos);

string substring(string s, int pos1, int pos2);

int convert_hhmmss_to_seconds(string hhmmss);

int convert_hhmm_to_seconds(string hhmm);

string convert_seconds_to_hhmmss(int time_in_seconds);

string convert_seconds_to_hhmm(int time_in_seconds);

void string_split(std::vector<std::string>& results, const std::string &source, const int fields, string delimiter);

double get_current_cpu_time_in_seconds();

#endif