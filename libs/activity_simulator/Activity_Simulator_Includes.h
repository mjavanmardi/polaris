#pragma once

#include "traffic_simulator\User_Space_Includes.h"

#include "activity_simulator\activity_simulator_concepts.h"
#include "activity_simulator\activity_simulator_forward_declaration.h"

#include "population_synthesis\Population_Synthesis_Concepts.h"


//#include "../Repository/Repository.h"
//#include "Activity_Simulator_Forward_Declaration.h"
//
//
////#include "RngStream.h"
//#include <time.h>
//#include <iostream>
//#include <fstream>
//
//// use hashmap
////#include <unordered_map>
////#include <unordered_set>
//#ifndef _MSC_VER
//#include <tr1/boost::unordered::unordered_map>
//#include <tr1/boost::unordered::unordered_set>
//using namespace __gnu_cxx;
//#endif
//
//
//#define WHILE_PRINT(value)
////#define WHILE_PRINT(value) cout << value
//
//
//#ifdef _MSC_VER
//struct Counter
//{
//private:
//	double _freq;
//	__int64 _start;
//	LARGE_INTEGER _l;
//	clock_t _t_start;
//	clock_t _t_end;
//public:
//	Counter()
//	{
//		_freq = 0.0;
//		_start = 0;
//	}
//	void Start()
//	{
//		if (!QueryPerformanceFrequency(&_l)) printf("QPF() failed with error %d\n", GetLastError());
//		_freq = double(_l.QuadPart)/1000.0;
//		if (!QueryPerformanceCounter(&_l)) printf("QPC() failed with error %d\n", GetLastError());
//		_start = _l.QuadPart;
//		_t_start = clock();
//	}
//	double Stop()
//	{
//		if (!QueryPerformanceCounter(&_l)) printf("QPC() failed with error %d\n", GetLastError());
//		_t_end = clock();
//		cout << endl << "approx clock time (s) = "<< (double)(_t_end - _t_start) / (double)CLOCKS_PER_SEC<<endl;
//		return (double)(_l.QuadPart - _start)/_freq;
//	}
//	//const __int64& get_start_value(){return _start;}
//	//const double& get_freq_value(){return _freq;}
//	//const long long& get_l_value(){return _l.QuadPart;}
//};
//#else
//struct Counter
//{
//private:
//	timespec _start;
//	long long _l;
//	//clock_t _t_start;
//	//clock_t _t_end;
//public:
//	Counter()
//	{
//	}
//	void Start()
//	{
//		_l=0;
//		start_timer(_start);
//	}
//	double Stop()
//	{
//		end_timer(_start,_l);
//		cout << endl << "approx clock time (ns) = "<< _l<<endl;
//		return (double)(_l);
//	}
//	//const __int64& get_start_value(){return _start;}
//	//const double& get_freq_value(){return _freq;}
//	//const long long& get_l_value(){return _l.QuadPart;}
//};
//#endif
//
//
//#ifndef _MSC_VER
//#define FLT_MAX	3.402823466e+38F	/* max value */
//#endif
