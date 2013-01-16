#pragma once
#include "Polaris_Variables.h"

static const long long _Megabytes_To_Allocate=2000;
static const unsigned int _num_iterations=3600*24*2; // 2 days
static const int _num_threads=1;

static const int _Max_Message_Size=5*1048576;

static const long long _Page_Size=4096;
static const long long _Max_Pages=(_Megabytes_To_Allocate*1048576)/_Page_Size;

typedef Basic_Units::Time_Variables::Time_Seconds Simulation_Timestep_Increment;


#define STATE_CHECKS 0

#define END (_num_iterations+1)

