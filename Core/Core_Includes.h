#pragma once

#include <list>
#include <vector>
#include <string>
#include <Windows.h>
#include <WinBase.h>
#include <intrin.h>
#include <sstream>
#include <iostream>
#include <type_traits>
#include <assert.h>
#include <cstdarg>
#include "Run_Parameters.h"
using namespace std;

class NULLCLASS{};

#define nullptr NULL

struct NULLTYPE{};

struct UNKNOWN{};

class NC{};

typedef char Page[Page_Size];

static volatile long long* request_sum=new volatile long long();
static volatile long long* exec_sum=new volatile long long();

__declspec(thread) int thread_id;

typedef char Byte;

//unsigned int thread_stats[num_threads];

template<int Size>
struct Bytes{char bytes[Size];};

static unsigned int tick_tock=false;
static unsigned int sub_tick_tock=false;
static unsigned int sub_iteration=0;
static long iteration=0;

typedef void (*Event)(void*);

struct Conditional_Response
{
	Conditional_Response():next(INT_MAX),result(false){};

	int next;
	bool result;
};

union Revision
{
	inline void operator = (const long long val){revision=val;}
	inline operator long long&(){return revision;}

	struct
	{
		long sub_iteration_revision;
		long iteration_revision;
	};

	long long revision;
};
typedef void (*Conditional)(void*,Conditional_Response&);


//static void False(void*,Conditional_Response& rsp){rsp.result=false;rsp.next=INT_MAX;}
//
//template<int Delay>
//static void True(void*,Conditional_Response& rsp){rsp.result=true;rsp.next=iteration+Delay;}

#define typedef_template(TYPEDEF_NAME,TEMPLATE_NAME) template<typename T> struct TYPEDEF_NAME{typedef TEMPLATE_NAME<T> Type;}

#define template_type(TYPE,TYPEDEF_NAME) TYPEDEF_NAME::Type




