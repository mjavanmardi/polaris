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

struct NULLTYPE{};

template<typename A> struct NULLTEMPLATE{};
template<typename A,typename B> struct NULLTEMPLATE_2{};

struct UNKNOWN{};

class NC{};

typedef char Page[Page_Size];

static volatile long long* request_sum=new volatile long long();
static volatile long long* exec_sum=new volatile long long();

#ifdef WINDOWS
__declspec(thread) int thread_id;
#else
int thread_id;
#endif

static int num_partitions=0;
static int host_rank=0;

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
	Revision():revision(0){};
	Revision(long sub_revision,long revision):sub_iteration(sub_revision),iteration(revision){};
	Revision(Revision& copy):revision(copy.revision){};

	inline void operator = (const long long val){revision=val;}
	inline operator long long&(){return revision;}
#ifdef WINDOWS
	struct
	{
		long sub_iteration;
		long iteration;
	};
#else
	struct
	{
		long iteration;
		long sub_iteration;
	};
#endif
	long long revision;
};

static Revision revision;
static long& sub_iteration=revision.sub_iteration;
static long& iteration=revision.iteration;

typedef void (*Conditional)(void*,Conditional_Response&);

#define typedef_template(TYPEDEF_NAME,TEMPLATE_NAME) template<typename T> struct TYPEDEF_NAME{typedef TEMPLATE_NAME<T> Type;}

#define template_type(TYPE,TYPEDEF_NAME) TYPEDEF_NAME::Type

//static ofstream logger=ofstream("log.txt");

#define PRINT(X) cout << X << endl;


///============================================================================
/// all_components - type singletons for all compiled components
///============================================================================

static vector<void*> all_components;

static int component_counter;

template<typename SingletonType>
static SingletonType* Add_Component_Singleton(SingletonType* val)
{
	all_components.push_back(val);
	return val;
}