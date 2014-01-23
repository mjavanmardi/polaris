#pragma once


#include "../Repository/Repository.h"
#include "User_Space_Forward_Declaration.h"


//#include "RngStream.h"
#include <time.h>
#include <iostream>
#include <fstream>

// use hashmap
//#include <unordered_map>
//#include <unordered_set>
#ifndef _MSC_VER
#include <tr1/boost::unordered::unordered_map>
#include <tr1/boost::unordered::unordered_set>
using namespace __gnu_cxx;
#endif


#define CHECK_CONDITIONAL if(response.next.iteration() < iteration() || response.next.iteration() == iteration() && response.next._subiteration() <= _subiteration())\
{\
	THROW_EXCEPTION("ERROR, can't go back in time.  Trying to schedule for iteration " << response.next.iteration() << "."<<response.next._subiteration() <<", at iteration "<<iteration() <<"."<<_subiteration());\
}

#define WHILE_PRINT(value)
//#define WHILE_PRINT(value) cout << value


#ifdef HIDE
//================================================================================================================================================================
//================================= TEMPORARY CORE METASTRUCTURE UPDATES =========================================================================================
//================================================================================================================================================================
#pragma region TEMP_CORE

typedef unsigned long ulong;



///======================================================================================
/// RTTI STUFF - in production

#define m_prototype_RTTI(Null_Prototype<FEATURE_NAME>, NUM_OVERLOADS, OVERLOAD_INDICATOR_NAME_1, OVERLOAD_MATCHING_CONCEPT_1, ...)\
	protected:\
	void* _##FEATURE_NAME;\
	public:\
	tag_getter_setter(FEATURE_NAME);\
	enum OVERLOAD_INDICATORS{ OVERLOAD_INDICATOR_NAME_1,\
	create_overload_names(NUM_OVERLOADS,__VA_ARGS__)};\
	OVERLOAD_INDICATORS overload_indicator;\
	_m_prototype_RTTI_add_setter(Null_Prototype<FEATURE_NAME>, OVERLOAD_INDICATOR_NAME_1, OVERLOAD_MATCHING_CONCEPT_1)\
	add_setter(NUM_OVERLOADS,FEATURE_NAME,__VA_ARGS__)\
	error_handler_RTTI(NUM_OVERLOADS,FEATURE_NAME,OVERLOAD_MATCHING_CONCEPT_1,__VA_ARGS__);\
	template<typename TargetType> OVERLOAD_INDICATORS FEATURE_NAME##_get_type(){return overload_indicator;}


#define _m_prototype_RTTI_add_setter(Null_Prototype<FEATURE_NAME>, OVERLOAD_INDICATOR_NAME, OVERLOAD_CONCEPT)\
	template<typename TargetType> void FEATURE_NAME(TargetType value,call_requirements(requires(TargetType,strip_modifiers(TargetType), Is_Polaris_Component) && OVERLOAD_CONCEPT))\
	{\
		_##FEATURE_NAME = (void*)value;\
		overload_indicator = OVERLOAD_INDICATOR_NAME;\
	}
#define _error_handler_RTTI(FEATURE_NAME, CONCEPTS)\
	template<typename TargetType> void FEATURE_NAME(TargetType value,call_requirements(!(requires(TargetType,strip_modifiers(TargetType), Is_Polaris_Component) && (CONCEPTS))))\
	{\
		assert_requirements(strip_modifiers(TargetType),Is_Polaris_Component,"TargetType is not a Polaris Component");\
		static_assert(false,"\n\n\n[--------- None of " #FEATURE_NAME" setter requirements from {"#CONCEPTS"} were matched---------]\n\n");\
	}


#define create_overload_names(N,...) create_overload_names_##N##_((__VA_ARGS__))
#define create_overload_names_2_(ARGS) create_overload_names_2 ARGS
#define create_overload_names_2(NAME) NAME
#define create_overload_names_3_(ARGS) create_overload_names_3 ARGS
#define create_overload_names_3(NAME,CONCEPT,NAME2) NAME, NAME2
#define create_overload_names_4_(ARGS) create_overload_names_4 ARGS
#define create_overload_names_4(NAME,CONCEPT,NAME2,CONCEPT2,NAME3) NAME, NAME2, NAME3
#define create_overload_names_5_(ARGS) create_overload_names_5 ARGS
#define create_overload_names_5(NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3,NAME4) NAME,NAME2,NAME3,NAME4
#define create_overload_names_6_(ARGS) create_overload_names_6 ARGS
#define create_overload_names_6(NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3,NAME4,CONCEPT4,NAME5) NAME,NAME2,NAME3,NAME4,NAME5


#define add_setter(N,...) add_setter_##N##_((__VA_ARGS__))
#define add_setter_2_(ARGS) add_setter_2 ARGS
#define add_setter_2(Null_Prototype<FEATURE_NAME>, NAME, CONCEPT) _m_prototype_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT)
#define add_setter_3_(ARGS) add_setter_3 ARGS
#define add_setter_3(Null_Prototype<FEATURE_NAME>, NAME,CONCEPT,NAME2, CONCEPT2) _m_prototype_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT) _member_component_RTTI_add_setter(FEATURE_NAME, NAME2, CONCEPT2)
#define add_setter_4_(ARGS) add_setter_4 ARGS
#define add_setter_4(Null_Prototype<FEATURE_NAME>, NAME,CONCEPT,NAME2,CONCEPT2,NAME3, CONCEPT3) _m_prototype_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT) _member_component_RTTI_add_setter(FEATURE_NAME, NAME2, CONCEPT2) _member_component_RTTI_add_setter(FEATURE_NAME, NAME3, CONCEPT3)
#define add_setter_5_(ARGS) add_setter_5 ARGS
#define add_setter_5(Null_Prototype<FEATURE_NAME>, NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3,NAME4, CONCEPT4) _m_prototype_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT) _member_component_RTTI_add_setter(FEATURE_NAME, NAME2, CONCEPT2) _member_component_RTTI_add_setter(FEATURE_NAME, NAME3, CONCEPT3) _member_component_RTTI_add_setter(FEATURE_NAME, NAME4, CONCEPT4)
#define add_setter_6_(ARGS) add_setter_6 ARGS
#define add_setter_6(Null_Prototype<FEATURE_NAME>, NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3,NAME4,CONCEPT4,NAME5, CONCEPT5) _m_prototype_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT) _member_component_RTTI_add_setter(FEATURE_NAME, NAME2, CONCEPT2) _member_component_RTTI_add_setter(FEATURE_NAME, NAME3, CONCEPT3) _member_component_RTTI_add_setter(FEATURE_NAME, NAME4, CONCEPT4) _member_component_RTTI_add_setter(FEATURE_NAME, NAME5, CONCEPT5)

#define error_handler_RTTI(N,...) error_handler_RTTI_##N##_((__VA_ARGS__))
#define error_handler_RTTI_2_(ARGS) error_handler_RTTI_2 ARGS
#define error_handler_RTTI_2(FEATURE_NAME,CONCEPT,NAME2,CONCEPT2) _error_handler_RTTI(FEATURE_NAME, (CONCEPT) || (CONCEPT2))
#define error_handler_RTTI_3_(ARGS) error_handler_RTTI_3 ARGS
#define error_handler_RTTI_3(FEATURE_NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3) _error_handler_RTTI(FEATURE_NAME, (CONCEPT) || (CONCEPT2) || (CONCEPT3))
#define error_handler_RTTI_4_(ARGS) error_handler_RTTI_4 ARGS
#define error_handler_RTTI_4(FEATURE_NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3,NAME4,CONCEPT4) _error_handler_RTTI(FEATURE_NAME, (CONCEPT) || (CONCEPT2)|| (CONCEPT3)|| (CONCEPT4))
#define error_handler_RTTI_5_(ARGS) error_handler_RTTI_5 ARGS
#define error_handler_RTTI_5(FEATURE_NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3,NAME4,CONCEPT4,NAME5,CONCEPT5) _error_handler_RTTI(FEATURE_NAME, (CONCEPT) || (CONCEPT2)|| (CONCEPT3)|| (CONCEPT4)|| (CONCEPT5))
#define error_handler_RTTI_6_(ARGS) error_handler_RTTI_6 ARGS
#define error_handler_RTTI_6(FEATURE_NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3,NAME4,CONCEPT4,NAME5,CONCEPT5,NAME6,CONCEPT6) _error_handler_RTTI(FEATURE_NAME, (CONCEPT) || (CONCEPT2)|| (CONCEPT3)|| (CONCEPT4)|| (CONCEPT5)|| (CONCEPT6))

//================================================================================================================================================================
//================================================================================================================================================================
//================================================================================================================================================================
#pragma endregion
#endif


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
		cout << endl << "approx clock time (s) = "<< (double)(_t_end - _t_start) / (double)CLOCKS_PER_SEC<<endl;
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
		start_timer(_start);
	}
	double Stop()
	{
		end_timer(_start,_l);
		cout << endl << "approx clock time (ns) = "<< _l<<endl;
		return (double)(_l);
	}
	//const __int64& get_start_value(){return _start;}
	//const double& get_freq_value(){return _freq;}
	//const long long& get_l_value(){return _l.QuadPart;}
};
#endif


#ifndef _MSC_VER
#define FLT_MAX	3.402823466e+38F	/* max value */
#endif
