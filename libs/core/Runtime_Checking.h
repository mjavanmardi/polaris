#pragma once
///----------------------------------------------------------------------------------------------------
/// Runtime_Checking.h - Macros and Subroutines Targetted at Assisting Run Time Debugging
///----------------------------------------------------------------------------------------------------

#include "Simulation_Control.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Enables the memory log to be constructed
	///----------------------------------------------------------------------------------------------------
	//#define ENABLE_MEMORY_LOGGING

	///----------------------------------------------------------------------------------------------------
	/// MESSAGES - Enable / Disable various categories of runtime messages
	///----------------------------------------------------------------------------------------------------

#ifdef _DEBUG
	#define ENABLE_MESSAGES
	//#define ENABLE_WARNINGS
	#define ENABLE_DEBUG_MESSAGES
#endif
	#define ENABLE_EXCEPTIONS
	///----------------------------------------------------------------------------------------------------
	/// SAFE_MODE - Enables all general purpose safeties during run
	///----------------------------------------------------------------------------------------------------
	//#define SAFE_MODE
	//#define ENABLE_WARNINGS
	#define ENABLE_MESSAGES
	///----------------------------------------------------------------------------------------------------
	/// FORWARD_SIMULATION_MODE - Enforces monotonically forward stepping simulation
	///----------------------------------------------------------------------------------------------------
	//#define FORWARD_SIMULATION_MODE

	
	///----------------------------------------------------------------------------------------------------
	/// PERFORMANCE_MODE - Disables all safeties during run to maximize performance
	///----------------------------------------------------------------------------------------------------
	//#define PERFORMANCE_MODE
	
	#define P_ASSERT_FALSE \
		struct assert_struct\
		{\
			void do_stuff(){x++;}\
			int x;\
		};\
		((assert_struct*)nullptr)->do_stuff()
	

	#ifdef SAFE_MODE
		#define ENABLE_WARNINGS
		#define ENABLE_EXCEPTIONS
	#endif

	#ifdef FORWARD_SIMULATION_MODE
		#define ENABLE_EXCEPTIONS	
	#endif

	#ifdef ENABLE_MESSAGES
		#define MESSAGE(...) cout << __VA_ARGS__ << endl;
	#else
		#define MESSAGE(...) ;
	#endif

	#ifdef ENABLE_DEBUG_MESSAGES
		#define DEBUG_MESSAGE(...) cout << "\n" << __FILE__ << " at " << __LINE__ << "\n\t" << __VA_ARGS__ << endl;
	#else
		#define DEBUG_MESSAGE(...) ;
	#endif

	#ifdef ENABLE_WARNINGS
		#define THROW_WARNING(...) cout << "\n" << "WARNING: "<< __FILE__ << " at " << __LINE__ << "\n\t" << __VA_ARGS__ << endl;
	#else 
		#define THROW_WARNING(...) int RANDOM_THING_WHICH_IS_OPTIMIZED_OUT=0;
	#endif

	#ifdef ENABLE_EXCEPTIONS
		#define THROW_EXCEPTION(...) \
			{stringstream s;\
			s << "\nRUNTIME_ERROR: " << __FILE__ << " at " << __LINE__<< "\n\t" << __VA_ARGS__ << endl;\
			cerr << s.str();\
			P_ASSERT_FALSE/*throw new runtime_error(s.str().c_str())*/;}
	#else
		#define THROW_EXCEPTION(...) ;
	#endif

	#ifdef PERFORMANCE_MODE
		#undef ENABLE_MESSAGES
		#undef ENABLE_WARNINGS
		#undef ENABLE_EXCEPTIONS
		#undef ENABLE_DEBUG_MESSAGES
		#undef SAFE_MODE
		#undef FORWARD_SIMULATION_MODE
	#endif
}