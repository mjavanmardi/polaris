#pragma once
#include "Core_Includes.h"

///============================================================================
/// General Definitions
///============================================================================

#define EXPAND(x) x
#define COUNT_ARGUMENTS_IMPL(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,N,...) N
#define count_arguments(...) EXPAND(COUNT_ARGUMENTS_IMPL(__VA_ARGS__,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))

#define define_state_check(CHECK_NAME,...) template<typename ThisType,typename CallerType,typename TargetType> inline static void CHECK_NAME(void* tthis,__VA_ARGS__)

#define begin_state_checks if(STATE_CHECKS){__assume(STATE_CHECKS);ThisType* pthis=(ThisType*)tthis; bool no_error=true

#define end_state_checks assert(no_error);}

///============================================================================
/// State Checks
///============================================================================

//#define requires_local_var_equals(VAR_NAME,VALUE,ERROR_MESSAGE) if(pthis->VAR_NAME!=VALUE){cout << ERROR_MESSAGE << endl << endl; no_error=false;}

#define requires_condition(CONDITION,ERROR_MESSAGE,EXIT,LOGSTREAM) \
	try{ if(!CONDITION){LOGSTREAM << ERROR_MESSAGE << endl << endl; no_error=!EXIT;} } \
	catch(exception& e){LOGSTREAM << ERROR_MESSAGE << " and Standard Exception: " << e.what() << endl; no_error=!EXIT;}

#define requires_thistype_match \
	try{if(typeid(ThisType)!=typeid(*pthis)){cout << "Type Mismatch: " << endl << endl << "Actual Type: " << typeid(*pthis).name() << endl << endl << "Stated Type: " << typeid(ThisType).name() << endl << endl; no_error=false;} }\
	catch(exception& e){cout << "Standard Exception: " << e.what() << endl; no_error=false; }

///============================================================================
/// Enable State Checking
///============================================================================

#if STATE_CHECKS
#define permit_state_checking virtual void State_Check()=0; typedef true_type state_checking_permitted
#else
#define permit_state_checking typedef true_type state_checking_permitted
#endif

#define state_check(CHECK_NAME) CHECK_NAME<ThisType,CallerType,TargetType>
