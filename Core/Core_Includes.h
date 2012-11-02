#pragma once

#define WINDOWS

#include <list>
#include <vector>
#include <set>
#include <deque>
#include <queue>
#include <string>

#ifdef WINDOWS
#include <Windows.h>
#include <WinBase.h>
#include <type_traits>
#include <random>
#else
#include <tr1/unordered_map>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#define __forceinline inline
#define unordered_map tr1::unordered_map
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <sstream>
#include <iostream>

#include <assert.h>
#include <cstdarg>
#include <set>

#include <fstream>
#include "Run_Parameters.h"
using namespace std;


#ifdef WINDOWS
#include <intrin.h>
#define AtomicExchange(TARGET,VALUE) _InterlockedExchange(TARGET,VALUE)
#define AtomicIncrement(TARGET) _InterlockedIncrement(TARGET)
#define AtomicDecrement(TARGET) _InterlockedIncrement(TARGET)
#define AtomicCompareExchange(TARGET,EXCHANGE_VALUE,COMPARE_VALUE) _InterlockedCompareExchange(TARGET,EXCHANGE_VALUE,COMPARE_VALUE)
#else
static const int inc_val=1;
#define AtomicExchange(TARGET,VALUE) __sync_val_compare_and_swap(TARGET,VALUE,true)
#define AtomicIncrement(TARGET) (__sync_fetch_and_add(TARGET,inc_val)+inc_val)
#define AtomicDecrement(TARGET) (__sync_fetch_and_sub(TARGET,inc_val)-inc_val)
#define AtomicCompareExchange(TARGET,EXCHANGE_VALUE,COMPARE_VALUE) __sync_val_compare_and_swap(TARGET,EXCHANGE_VALUE,COMPARE_VALUE)
#endif

#ifdef WINDOWS
#define SLEEP(Seconds) Sleep(Seconds*1000)
#else
#define SLEEP(Seconds) usleep(Seconds*1000000)
#endif
class NULLCLASS{};

#define nullptr NULL

#define none true

struct NULLTYPE{};

template<typename A> struct NULLTEMPLATE{};
template<typename A,typename B> struct NULLTEMPLATE_2{};

struct UNKNOWN{};

class NC{};

typedef char Page[_Page_Size];

static volatile long long* request_sum=new volatile long long();
static volatile long long* exec_sum=new volatile long long();

#ifdef WINDOWS
__declspec(thread) int _thread_id;
#else
int _thread_id;
#endif

static int _num_partitions=0;
static int _host_rank=0;

typedef char Byte;

template<int Size>
struct Bytes{char bytes[Size];};

static unsigned int tick_tock=false;
static unsigned int sub_tick_tock=false;


typedef void (*Event)(void*);

struct Conditional_Response
{
	Conditional_Response():next(INT_MAX),result(false){};

	int next;
	bool result;
};


union Revision
{
	Revision():_revision(0){};
	Revision(long sub_revision,long revision):_sub_iteration(sub_revision),_iteration(revision){};
	Revision(Revision& copy):_revision(copy._revision){};

	inline void operator = (const long long val){_revision=val;}
	inline operator long long&(){return _revision;}
#ifdef WINDOWS
	struct
	{
		long _sub_iteration;
		long _iteration;
	};
#else
	struct
	{
		long _iteration;
		long _sub_iteration;
	};
#endif
	long long _revision;
};

static Revision _revision;
static long& _sub_iteration=_revision._sub_iteration;
static long& _iteration=_revision._iteration;

typedef void (*Conditional)(void*,Conditional_Response&);

#define typedef_template(TYPEDEF_NAME,TEMPLATE_NAME) template<typename T> struct TYPEDEF_NAME{typedef TEMPLATE_NAME<T> Type;}

#define template_type(TYPE,TYPEDEF_NAME) TYPEDEF_NAME::Type

//static ofstream logger=ofstream("log.txt");

#define PRINT(X) cout << X << endl;

#define START() world->Start_Turning()

///============================================================================
/// _all_components - type singletons for all compiled components
///============================================================================

static vector<void*> _all_components;

static int _component_counter;

template<typename SingletonType>
static SingletonType* Add_Component_Singleton(SingletonType* val)
{
	_all_components.push_back(val);
	return val;
}




#ifdef DEBUG_1
#define debug_1(MESSAGE) cout << MESSAGE
#else
#define debug_1(MESSAGE) 
#endif

#ifdef DEBUG_2
#undef debug_1
#define debug_1(MESSAGE) cout << MESSAGE
#define debug_2(MESSAGE) cout << MESSAGE
#else
#define debug_2(MESSAGE) 
#endif

#ifdef DEBUG_3
#undef debug_1
#undef debug_2
#define debug_1(MESSAGE) cout << MESSAGE
#define debug_2(MESSAGE) cout << MESSAGE
#define debug_3(MESSAGE) cout << MESSAGE
#else
#define debug_3(MESSAGE) 
#endif