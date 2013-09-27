#pragma once
///----------------------------------------------------------------------------------------------------
/// Globals.h - Global Constructs not related to simulation
///----------------------------------------------------------------------------------------------------

#include "Memory_Allocation.h"

namespace polaris
{
	#define LOCK(LOCK_VARIABLE) while(AtomicExchange(&LOCK_VARIABLE,1)) SLEEP(0)
	#define UNLOCK(LOCK_VARIABLE) LOCK_VARIABLE=0

#ifdef _MSC_VER
	extern __declspec(thread) int __thread_id;
	typedef volatile long _lock;
	typedef volatile long _atomic_counter;
#else
	extern __thread int __thread_id;
	typedef volatile unsigned int _lock;
	typedef volatile unsigned int _atomic_counter;
#endif

	static int thread_id(){return __thread_id;}

#ifdef _MSC_VER
	#define SLEEP(Seconds) Sleep(Seconds*1000)
	#include <intrin.h>
	#define AtomicExchange(TARGET,VALUE) _InterlockedExchange(TARGET,VALUE)
	#define AtomicIncrement(TARGET) _InterlockedIncrement(TARGET)
	#define AtomicDecrement(TARGET) _InterlockedIncrement(TARGET)
	#define AtomicCompareExchange(TARGET,EXCHANGE_VALUE,COMPARE_VALUE) _InterlockedCompareExchange(TARGET,EXCHANGE_VALUE,COMPARE_VALUE)
#else
	#define SLEEP(Seconds) usleep(Seconds*1000000)
	extern const int inc_val;
	#define AtomicExchange(TARGET,VALUE) __sync_lock_test_and_set(TARGET,VALUE)
	#define AtomicIncrement(TARGET) (__sync_fetch_and_add(TARGET,inc_val)+inc_val)
	#define AtomicDecrement(TARGET) (__sync_fetch_and_sub(TARGET,inc_val)-inc_val)
	#define AtomicCompareExchange(TARGET,EXCHANGE_VALUE,COMPARE_VALUE) __sync_val_compare_and_swap(TARGET,EXCHANGE_VALUE,COMPARE_VALUE)
#endif
}