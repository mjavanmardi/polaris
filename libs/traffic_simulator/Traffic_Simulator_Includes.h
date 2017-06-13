#pragma once


#include "../repository/Repository.h"
#include "Traffic_Simulator_Forward_Declaration.h"
#include "Traffic_Simulator_Concepts.h"
#include "Traffic_Simulator_Types.h"


//#include "RngStream.h"
#include <time.h>
#include <iostream>
#include <fstream>

// use hashmap
//#include <unordered_map>
//#include <unordered_set>
#ifndef _MSC_VER
#include <unordered_map>
#include <unordered_set>
using namespace __gnu_cxx;
#endif


/*
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib") 

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
#include <fcntl.h>
#include <procfs.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <stdio.h>

#endif

#else
#error "Cannot define getPeakRSS( ) or getCurrentRSS( ) for an unknown OS."
#endif


#define CHECK_CONDITIONAL if(response.next._iteration < _iteration || response.next._iteration == _iteration && response.next._sub_iteration <= _sub_iteration)\
{\
	THROW_EXCEPTION("ERROR, can't go back in time.  Trying to schedule for iteration " << response.next._iteration << "."<<response.next._sub_iteration <<", at iteration "<<_iteration <<"."<<_sub_iteration);\
}

#define WHILE_PRINT(value)
//#define WHILE_PRINT(value) cout << value


#ifdef _MSC_VER
struct Counter
{
private:
	double _freq;
	__int64 _start;
	LARGE_INTEGER _l;
	clock_t _t_start;
	clock_t _t_end;
public:
	Counter()
	{
		_freq = 0.0;
		_start = 0;
	}
	void Start()
	{
		if (!QueryPerformanceFrequency(&_l)) printf("QPF() failed with error %d\n", GetLastError());
		_freq = double(_l.QuadPart)/1000.0;
		if (!QueryPerformanceCounter(&_l)) printf("QPC() failed with error %d\n", GetLastError());
		_start = _l.QuadPart;
		_t_start = clock();
	}
	double Stop()
	{
		if (!QueryPerformanceCounter(&_l)) printf("QPC() failed with error %d\n", GetLastError());
		_t_end = clock();
		//cout << endl << "approx clock time (s) = "<< (double)(_t_end - _t_start) / (double)CLOCKS_PER_SEC<<endl;
		return (double)(_l.QuadPart - _start)/_freq;
	}
	//const __int64& get_start_value(){return _start;}
	//const double& get_freq_value(){return _freq;}
	//const long long& get_l_value(){return _l.QuadPart;}
};
#else
struct Counter
{
private:
	timespec _start;
	long long _l;
	//clock_t _t_start;
	//clock_t _t_end;
public:
	Counter()
	{
	}
	void Start()
	{
		_l=0;
		//TODO: I don't see where this is defined
		//start_timer(_start);
	}
	double Stop()
	{
		//TODO: I don't see where this is defined
		//end_timer(_start,_l);
		cout << endl << "approx clock time (ns) = "<< _l<<endl;
		return (double)(_l);
	}
	//const __int64& get_start_value(){return _start;}
	//const double& get_freq_value(){return _freq;}
	//const long long& get_l_value(){return _l.QuadPart;}
};
#endif


#ifndef _MSC_VER
//#define FLT_MAX	3.402823466e+38F	/* max value */
#include <cfloat>
#endif

#define INFINITY_FLOAT 9999999

//left substring
string left(string s, int pos)
{
	s = s.substr(0,pos);
	return s;
};

//right substring
string right(string s, int pos)
{
	s = s.substr(pos,(int)s.size());
	return s;
};

//extract substring
string substring(string s, int pos1, int pos2)
{
	s = s.substr(pos1, pos2);
	return s;
};

//convert hh:mm:ss to seconds
int convert_hhmmss_to_seconds(string hhmmss)
{
	string hh,mm,ss;
	//int pos = (int) hhmmss.find(":");
	int pos = 2;
	hh = left(hhmmss,pos);
	hhmmss = right(hhmmss,pos+1);

	//pos = (int) hhmmss.find(":");
	pos = 2;
	mm = left(hhmmss,pos);
	ss = right(hhmmss,pos+1);
	int h = stoi(hh);
	int m = stoi(mm);
	int s = stoi(ss);

	int time_in_seconds = h*3600 + m*60 + s;
	
	return time_in_seconds;
};

//convert hh:mm:ss to seconds
int convert_hhmm_to_seconds(string hhmm)
{
	string hh,mm;
	int pos = 2;

	hh = left(hhmm,pos);
	mm = right(hhmm,pos+1);
	
	int h = stoi(hh);
	int m = stoi(mm);

	int time_in_seconds = h*3600 + m*60;
	
	return time_in_seconds;
};

void string_split(std::vector<std::string>& results, const std::string &source, const int fields)
{
	results.clear();
	results.resize(fields);

	const char* s = source.c_str();
	const char* s2;
	int count;
	int pos = 0;
	while (pos < fields)
	{
		count = 0;
		while(*s != '\0' && (*s == '\t' || *s == ' ' || *s == '\r' || *s == ',')) s++;
		if (*s == '\0')
		{
			break;
		}
		s2 = s;
		while(*s2 != '\0' && *s2 != '\t' && *s2 != ' ' && *s2 != '\r' && *s2 != ',')
		{
			s2++;
			count++;
		}
		results[pos].assign(s, count);
		s = s2;
		pos++;
	}
};

void string_split(std::vector<std::string>& results, const std::string &source)
{
	results.clear();

	const char* s = source.c_str();
	const char* s2;
	int count;
	while (true)
	{
		count = 0;
		while(*s != '\0' && (*s == '\t' || *s == ' ' || *s == '\r' || *s == ',')) s++;
		if (*s == '\0')
		{
			break;
		}
		s2 = s;
		while(*s2 != '\0' && *s2 != '\t' && *s2 != ' ' && *s2 != '\r' && *s2 != ',')
		{
			s2++;
			count++;
		}
		string str;
		str.assign(s, count);
		results.push_back(str);
		s = s2;
	}
};

//convert seconds to hh:mm:ss
string convert_seconds_to_hhmmss(int time_in_seconds)
{
	long long h = time_in_seconds/3600;
	time_in_seconds = time_in_seconds%3600;
	long long m = time_in_seconds/60;
	time_in_seconds = time_in_seconds%60;
	long long s = time_in_seconds;
	string hh = to_string(h);
	if (hh.size()==1)
	{
		hh = "0" + hh;
	}
	string mm = to_string(m);
	if (mm.size()==1)
	{
		mm = "0" + mm;
	}

	string ss = to_string(s);
	if (ss.size()==1)
	{
		ss = "0" + ss;
	}

	string hhmmss = hh + ":" + mm + ":" + ss;
	return hhmmss;
};

string convert_seconds_to_hhmm(int time_in_seconds)
{
	long long h = time_in_seconds/3600;
	time_in_seconds = time_in_seconds%3600;
	long long m = time_in_seconds/60;
	time_in_seconds = time_in_seconds%60;
	long long s = time_in_seconds;
	string hh = to_string(h);
	if (hh.size()==1)
	{
		hh = "0" + hh;
	}
	string mm = to_string(m);
	if (mm.size()==1)
	{
		mm = "0" + mm;
	}

	string ss = to_string(s);
	if (ss.size()==1)
	{
		ss = "0" + ss;
	}

	string hhmm = hh + ":" + mm;
	return hhmm;
};

void calculate_mean_standard_deviation(const std::vector<float>& data_array, float& mean, float& standard_deviation)
{
	int array_size = int(data_array.size());
	if (array_size>1)
	{
		mean = 0.0f;
		for (int i=0;i<array_size;i++)
		{
			mean += data_array[i];
		}
		mean = float (mean/(float(array_size)));
		standard_deviation = 0.0f;
		for (int i=0;i<array_size;i++)
		{
			standard_deviation += (data_array[i] - mean)*(data_array[i] - mean);
		}

		standard_deviation = float(sqrt(double(standard_deviation/float(array_size-1))));
	}
	else
	{
		if (array_size ==1)
		{
			mean = data_array[0];
			standard_deviation = 0.0f;
		}
		else
		{
			mean = 0.0f;
			standard_deviation = 0.0f;
		}
	}
};

void calculate_mean(const std::vector<float>& data_array, float& mean)
{
	int array_size = int(data_array.size());
	if (array_size>1)
	{
		mean = 0.0f;
		for (int i=0;i<array_size;i++)
		{
			mean += data_array[i];
		}
		mean = float (mean/(float(array_size)));
	}
	else
	{
		if (array_size ==1)
		{
			mean = data_array[0];
		}
		else
		{
			mean = 0.0f;
		}
	}
};

#ifdef __GNUC__
	double get_current_cpu_time_in_seconds()
	{
		struct timespec current_cpu_time;
		double current_cpu_time_in_seconds;
		clock_gettime(CLOCK_REALTIME, &current_cpu_time);

		current_cpu_time_in_seconds = (double)((current_cpu_time.tv_sec * 1000000000 + current_cpu_time.tv_nsec)) / 1000000000.0;
		return current_cpu_time_in_seconds;
	};
#else
	double get_current_cpu_time_in_seconds()
	{
		LARGE_INTEGER current_cpu_time;
		LARGE_INTEGER query_performance_frequency;
		double current_cpu_time_in_seconds;

		QueryPerformanceFrequency(&query_performance_frequency);
		QueryPerformanceCounter(&current_cpu_time);

		current_cpu_time_in_seconds =(double) (current_cpu_time.QuadPart/((double)query_performance_frequency.QuadPart));	
		return current_cpu_time_in_seconds;
	};
#endif


/**
 * Returns the peak (maximum so far) resident set size (physical
 * memory use) measured in bytes, or zero if the value cannot be
 * determined on this OS.
 */
size_t getPeakRSS( )
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (size_t)info.PeakWorkingSetSize;

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
    /* AIX and Solaris ------------------------------------------ */
    struct psinfo psinfo;
    int fd = -1;
    if ( (fd = open( "/proc/self/psinfo", O_RDONLY )) == -1 )
        return (size_t)0L;      /* Can't open? */
    if ( read( fd, &psinfo, sizeof(psinfo) ) != sizeof(psinfo) )
    {
        close( fd );
        return (size_t)0L;      /* Can't read? */
    }
    close( fd );
    return (size_t)(psinfo.pr_rssize * 1024L);

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* BSD, Linux, and OSX -------------------------------------- */
    struct rusage rusage;
    getrusage( RUSAGE_SELF, &rusage );
#if defined(__APPLE__) && defined(__MACH__)
    return (size_t)rusage.ru_maxrss;
#else
    return (size_t)(rusage.ru_maxrss * 1024L);
#endif

#else
    /* Unknown OS ----------------------------------------------- */
    return (size_t)0L;          /* Unsupported. */
#endif
}





/**
 * Returns the current resident set size (physical memory use) measured
 * in bytes, or zero if the value cannot be determined on this OS.
 */
size_t getCurrentRSS( )
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (size_t)info.WorkingSetSize;

#elif defined(__APPLE__) && defined(__MACH__)
    /* OSX ------------------------------------------------------ */
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
        (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
        return (size_t)0L;      /* Can't access? */
    return (size_t)info.resident_size;

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    /* Linux ---------------------------------------------------- */
    long rss = 0L;
    FILE* fp = NULL;
    if ( (fp = fopen( "/proc/self/statm", "r" )) == NULL )
        return (size_t)0L;      /* Can't open? */
    if ( fscanf( fp, "%*s%ld", &rss ) != 1 )
    {
        fclose( fp );
        return (size_t)0L;      /* Can't read? */
    }
    fclose( fp );
    return (size_t)rss * (size_t)sysconf( _SC_PAGESIZE);

#else
    /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    return (size_t)0L;          /* Unsupported. */
#endif
}