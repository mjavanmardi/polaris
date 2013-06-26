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
#include <map>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <iterator>

//random number
#include "../Repository/RngStream.h"

using namespace RNG_Components;

//algorithms
#include <math.h>
#include <algorithm>
#include <utility>
#include <functional>
#include <omp.h>

#ifdef __GNUC__
#include <sys/time.h>
#define ULONG_MAX 0xffffffffUL
#else
#include <windows.h>
#endif

#ifdef __GNUC__
// for map
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

void string_split(std::vector<std::string>& results, const std::string &source, const int fields);

void string_split(std::vector<std::string>& results, const std::string &source);

template<class Type>
Type convert_meter_to_foot(Type meter)
{
	Type foot = Type(3.28084 * meter);
	return foot;
};

template<class Type>
Type convert_meters_per_second_to_miles_per_hour(Type meters_per_second)
{
	Type miles_per_hour = Type(3.28084 * meters_per_second * 3600.0 / 5280.0);
	return miles_per_hour;
};

double get_current_cpu_time_in_seconds();

void calculate_mean_standard_deviation(const vector<float>& data_array, float& mean, float& standard_deviation);

void calculate_mean(const vector<float>& data_array, float& mean);

void mem_info(long long& vm_usage, long long& resident_set);

#endif