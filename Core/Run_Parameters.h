#pragma once
#include "Polaris_Variables.h"

static const long long _Megabytes_To_Allocate=6000;
static unsigned int _num_iterations=60*60*24*2; // 2 days
static const int _num_threads=10;

static const int _num_antares_threads = _num_threads + 1;
static const int _Max_Message_Size=5*1048576;

static const long long _Page_Size=4096*1000;
static const long long _Default_Page_Size=4096*10;
static const long long _Max_Pages=(_Megabytes_To_Allocate*1048576)/_Page_Size;

typedef Basic_Units::Time_Variables::Time_Seconds Simulation_Timestep_Increment;


#define STATE_CHECKS 0

#define END (_num_iterations+1)

