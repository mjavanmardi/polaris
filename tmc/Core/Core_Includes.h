#pragma once

#define WINDOWS

#include <list>
#include <vector>
#include <set>
#include <unordered_set>
#include <deque>
#include <queue>
#include <string>
#include <stdexcept>
#include <random>
#include <sstream>
#include <cmath>

#ifdef WINDOWS
#include <Windows.h>
#include <WinBase.h>
#include <unordered_map>
#include <type_traits>

#else
#define nullptr NULL
#include <time.h>
#include <tr1/unordered_map>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#define __forceinline inline
#define unordered_map tr1::unordered_map
#define unordered_set tr1::unordered_set
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
#define WITH_WAIT

#else
static const int inc_val=1;

#define AtomicExchange(TARGET,VALUE) __sync_lock_test_and_set(TARGET,VALUE)
#define AtomicIncrement(TARGET) (__sync_fetch_and_add(TARGET,inc_val)+inc_val)
#define AtomicDecrement(TARGET) (__sync_fetch_and_sub(TARGET,inc_val)-inc_val)
#define AtomicCompareExchange(TARGET,EXCHANGE_VALUE,COMPARE_VALUE) __sync_val_compare_and_swap(TARGET,EXCHANGE_VALUE,COMPARE_VALUE)
#endif

#ifdef WINDOWS
#define SLEEP(Seconds) Sleep(Seconds*1000)
#else
#define SLEEP(Seconds) usleep(Seconds*1000000)
#endif
struct NULLCLASS{static const int page_factor=1;};
const int NULLCLASS::page_factor;


typedef volatile unsigned int _lock;
#define LOCK(LOCK_VARIABLE) while(AtomicExchange(&LOCK_VARIABLE,1)) SLEEP(0)
#define UNLOCK(LOCK_VARIABLE) LOCK_VARIABLE=0

#define none true
#define not_available false

struct NULLTYPE{};

typedef NULLTYPE NT;

template<typename A> struct NULLTEMPLATE{};
template<typename A,typename B> struct NULLTEMPLATE_2{};
template<typename A,typename B,typename C> struct NULLTEMPLATE_3{};
template<typename A,typename B,typename C,typename D> struct NULLTEMPLATE_4{};

typedef char Page[_Page_Size];

static volatile long long* request_sum=new volatile long long();
static volatile long long* exec_sum=new volatile long long();
static volatile unsigned int stdout_lock=0;

#ifdef WINDOWS
static __declspec(thread) int _thread_id;
#else
static __thread int _thread_id;
#endif

static int _num_partitions=0;
static int _host_rank=0;

typedef char Byte;

template<int Size>
struct Bytes{char bytes[Size];};


typedef void (*Event)(void*);



union Revision
{
	Revision():_revision(0){};
	Revision(int sub_revision,int revision):_sub_iteration(sub_revision),_iteration(revision){};
	Revision(Revision& copy):_revision(copy._revision){};

	inline void operator = (const long long val){_revision=val;}
	inline operator long long&(){return _revision;}

	struct
	{
		int _sub_iteration;
		int _iteration;
	};

	long long _revision;
};

static Revision _revision;
static int& _sub_iteration=_revision._sub_iteration;
static int& _iteration=_revision._iteration;


struct Conditional_Response
{
	Conditional_Response():result(false){next._iteration=INT_MAX;next._sub_iteration=0;};

	Revision next;
	bool result;
};

typedef void (*Conditional)(void*,Conditional_Response&);

static void False_Condition(void*,Conditional_Response& resp)
{
	resp.result=false;
	resp.next=END;
}

typedef void (*selection_callback_type)(const list<void*>&,const list<void*>&,const list<void*>&,vector<pair<string,string>>&);
typedef void (*double_click_callback_type)(const list<void*>&,vector<pair<string,string>>&,vector<vector<string>>&);
typedef bool (*submission_callback_type)(const list<void*>&,const vector<string>&,const vector<string>&);

#define typedef_template(TYPEDEF_NAME,TEMPLATE_NAME) template<typename T> struct TYPEDEF_NAME{typedef TEMPLATE_NAME<T> Type;}

#define template_type(TYPE,TYPEDEF_NAME) TYPEDEF_NAME::Type

//static ofstream logger=ofstream("log.txt");

#define PRINT(X) cout << X << endl;

#define START() world->Start_Turning()

#ifdef WINDOWS
//static LARGE_INTEGER thread_start_timers[_num_threads];
//static LARGE_INTEGER thread_stop_timers[_num_threads];
//static LARGE_INTEGER thread_start_timers_B[_num_threads];
//static LARGE_INTEGER thread_stop_timers_B[_num_threads];
#endif


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


#ifdef WINDOWS
#else
static timespec process_timer[_num_threads];
static long long process_time[_num_threads];
static timespec sub_process_timer_a[_num_threads];
static long long sub_process_time_a[_num_threads];
static timespec sub_process_timer_b[_num_threads];
static long long sub_process_time_b[_num_threads];
static timespec sub_process_timer_c[_num_threads];
static long long sub_process_time_c[_num_threads];
static timespec exchange_timer[_num_threads];
static long long exchange_time[_num_threads];

void start_timer(timespec& start)
{
	clock_gettime(CLOCK_REALTIME, &start);
}

void end_timer(timespec& start,long long& total)
{
	timespec end;
	clock_gettime(CLOCK_REALTIME, &end);
	
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0)
	{
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	}
	else
	{
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	
	total=total+temp.tv_nsec+1000000000*temp.tv_sec;
}

#endif
#ifdef SHOW_WARNINGS
#define THROW_WARNING(MESSAGE_STREAM) \
{std::stringstream s;\
s << endl << "WARNING at "<<__FILE__<<", line: "<<__LINE__<<endl;\
s << MESSAGE_STREAM;\
std::cout << s.str();}
#else 
#define THROW_WARNING(MESSAGE_STREAM)
#endif

#define THROW_EXCEPTION(MESSAGE_STREAM) \
{std::stringstream s;\
s << endl << "RUNTIME_ERROR at "<<__FILE__<<", line: "<<__LINE__<<endl;\
s << MESSAGE_STREAM;\
std::cerr << s.str();\
throw new std::runtime_error(s.str().c_str());}


#ifdef _DEBUG
#define ADD_DEBUG_INFO : ComponentType
#else
#define ADD_DEBUG_INFO
#endif



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


//---------------------------------------------------------
//	define numeric check (tr1::isnan is not available on vc++
//---------------------------------------------------------
#ifdef WINDOWS
#define ISNAN(x) _isnan(x)
#else
#define ISNAN(x) std::tr1::isnan(x)
#endif

//---------------------------------------------------------
//	antares - build global antares singleton
//---------------------------------------------------------

static void* antares;
static void* canvas;
static void* information_panel;