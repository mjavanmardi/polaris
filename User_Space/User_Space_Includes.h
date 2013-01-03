#pragma once
#include "Repository/Repository.h"
#include "RngStream.h"

#include <iostream>
#include <fstream>


//================================================================================================================================================================
//================================= TEMPORARY CORE METASTRUCTURE UPDATES =========================================================================================
//================================================================================================================================================================
#pragma region TEMP_CORE

typedef unsigned long long int ulong;





#define Basic_Data_Struct(DATA_STRUCT_NAME, DATA_VALUE_TYPE, TYPE_TRAIT_TAGS,...) \
struct DATA_STRUCT_NAME \
{													\
	typedef true_type TYPE_TRAIT_TAGS, __VA_ARGS__; \
	typedef DATA_VALUE_TYPE ValueType; \
	DATA_STRUCT_NAME(DATA_VALUE_TYPE val): Value(val){} \
	DATA_STRUCT_NAME(): Value(0){} \
	DATA_VALUE_TYPE Value; \
	operator DATA_VALUE_TYPE(){return Value;}; \
	DATA_STRUCT_NAME& operator=(DATA_VALUE_TYPE& obj){Value = obj; return *this;}  \
	DATA_STRUCT_NAME& operator+(DATA_VALUE_TYPE& obj){Value = Value + obj; return *this;}  \
	DATA_STRUCT_NAME& operator-(DATA_VALUE_TYPE& obj){Value = Value - obj; return *this;}  \
	DATA_STRUCT_NAME& operator*(DATA_VALUE_TYPE& obj){Value = Value * obj; return *this;}  \
	DATA_STRUCT_NAME& operator/(DATA_VALUE_TYPE& obj){Value = Value / obj; return *this;}  \
};

//concept Is_Target_Type_Struct
//{
//	begin_requirements_list (none);
//	requires_typename_defined(none, ReturnType,"Type does not model an HCM simple solution type.");
//	requires_typename_defined(ReturnType, ParamType,"Type does not model an HCM simple solution type.");
//	requires_typename_defined(ParamType, Param2Type,"Type does not model an HCM simple solution type.");
//	end_requirements_list(Param2Type);
//};



///======================================================================================
/// RTTI STUFF - in production

#define member_component_RTTI(FEATURE_NAME, NUM_OVERLOADS, OVERLOAD_INDICATOR_NAME_1, OVERLOAD_MATCHING_CONCEPT_1, ...)\
	protected:\
	void* _##FEATURE_NAME;\
	public:\
	tag_getter_setter(FEATURE_NAME);\
	enum OVERLOAD_INDICATORS{ OVERLOAD_INDICATOR_NAME_1,\
	create_overload_names(NUM_OVERLOADS,__VA_ARGS__)};\
	OVERLOAD_INDICATORS overload_indicator;\
	_member_component_RTTI_add_setter(FEATURE_NAME, OVERLOAD_INDICATOR_NAME_1, OVERLOAD_MATCHING_CONCEPT_1)\
	add_setter(NUM_OVERLOADS,FEATURE_NAME,__VA_ARGS__)\
	error_handler_RTTI(NUM_OVERLOADS,FEATURE_NAME,OVERLOAD_MATCHING_CONCEPT_1,__VA_ARGS__);\
	feature_implementation OVERLOAD_INDICATORS FEATURE_NAME##_get_type(){return overload_indicator;}


#define _member_component_RTTI_add_setter(FEATURE_NAME, OVERLOAD_INDICATOR_NAME, OVERLOAD_CONCEPT)\
	feature_implementation void FEATURE_NAME(TargetType value,call_requirements(requires(strip_modifiers(TargetType), Is_Polaris_Component) && OVERLOAD_CONCEPT))\
	{\
		_##FEATURE_NAME = (void*)value;\
		overload_indicator = OVERLOAD_INDICATOR_NAME;\
	}
#define _error_handler_RTTI(FEATURE_NAME, CONCEPTS)\
	feature_implementation void FEATURE_NAME(TargetType value,call_requirements(!(requires(strip_modifiers(TargetType), Is_Polaris_Component) && (CONCEPTS))))\
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
#define add_setter_2(FEATURE_NAME, NAME, CONCEPT) _member_component_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT)
#define add_setter_3_(ARGS) add_setter_3 ARGS
#define add_setter_3(FEATURE_NAME, NAME,CONCEPT,NAME2, CONCEPT2) _member_component_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT) _member_component_RTTI_add_setter(FEATURE_NAME, NAME2, CONCEPT2)
#define add_setter_4_(ARGS) add_setter_4 ARGS
#define add_setter_4(FEATURE_NAME, NAME,CONCEPT,NAME2,CONCEPT2,NAME3, CONCEPT3) _member_component_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT) _member_component_RTTI_add_setter(FEATURE_NAME, NAME2, CONCEPT2) _member_component_RTTI_add_setter(FEATURE_NAME, NAME3, CONCEPT3)
#define add_setter_5_(ARGS) add_setter_5 ARGS
#define add_setter_5(FEATURE_NAME, NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3,NAME4, CONCEPT4) _member_component_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT) _member_component_RTTI_add_setter(FEATURE_NAME, NAME2, CONCEPT2) _member_component_RTTI_add_setter(FEATURE_NAME, NAME3, CONCEPT3) _member_component_RTTI_add_setter(FEATURE_NAME, NAME4, CONCEPT4)
#define add_setter_6_(ARGS) add_setter_6 ARGS
#define add_setter_6(FEATURE_NAME, NAME,CONCEPT,NAME2,CONCEPT2,NAME3,CONCEPT3,NAME4,CONCEPT4,NAME5, CONCEPT5) _member_component_RTTI_add_setter(FEATURE_NAME, NAME, CONCEPT) _member_component_RTTI_add_setter(FEATURE_NAME, NAME2, CONCEPT2) _member_component_RTTI_add_setter(FEATURE_NAME, NAME3, CONCEPT3) _member_component_RTTI_add_setter(FEATURE_NAME, NAME4, CONCEPT4) _member_component_RTTI_add_setter(FEATURE_NAME, NAME5, CONCEPT5)

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



#define requires_typename_state_interface(LINKED_CONCEPT,REQUIREMENT_NAME,TYPENAME_STATE,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_typename(typename U::Component_Type::REQUIREMENT_NAME*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool member_exists=sizeof(has_matching_typename<T>(0))==success;\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename is_same<typename U::Component_Type::REQUIREMENT_NAME,TYPENAME_STATE>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;

#define requires_typed_member_interface(LINKED_CONCEPT,REQUIREMENT_NAME,DESIRED_TYPE,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_named_member(decltype(&U::Component_Type::REQUIREMENT_NAME));\
		template<typename U> static large_type has_matching_named_member(...);\
		static const bool member_exists=sizeof(has_matching_named_member<T>(0))==success;\
		template<class U,bool B> struct p_conditional{typedef false_type type;};\
		template<class U> struct p_conditional<U,true>{typedef typename is_same<decltype(&U::REQUIREMENT_NAME),DESIRED_TYPE U::*>::type type;};\
		static const bool value=(member_exists && p_conditional<T,member_exists>::type::value);\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;


#define requires_typename_defined_interface(LINKED_CONCEPT,REQUIREMENT_NAME,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_typename(typename U::Component_Type::REQUIREMENT_NAME*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;


#define requires_named_member_interface(LINKED_CONCEPT,REQUIREMENT_NAME,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<typename U> static small_type has_matching_named_member(decltype(&U::Component_Type::REQUIREMENT_NAME));\
		template<typename U> static large_type has_matching_named_member(...);\
		static const bool value=sizeof(has_matching_named_member<T>(0))==success;\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;

#define requires_feature_interface(LINKED_CONCEPT,FEATURE_NAME,FEATURE_TYPE,ERROR_MESSAGE)\
	struct FEATURE_NAME\
	{\
	template<typename U> static small_type has_matching_typename(typename U::Component_Type::FEATURE_NAME##_FEATURE_TYPE*);\
		template<typename U> static large_type has_matching_typename(...);\
		static const bool value=sizeof(has_matching_typename<T>(0))==success;\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,FEATURE_NAME>::Result auto_check_list_##FEATURE_NAME;

#define requires_method_interface(LINKED_CONCEPT,REQUIREMENT_NAME,METHOD_FORM,ERROR_MESSAGE)\
	struct REQUIREMENT_NAME\
	{\
		template<METHOD_FORM> struct tester{};\
		template<typename U> static small_type has_matching_function_member(tester<&U::Component_Type::REQUIREMENT_NAME>*);\
		template<typename U> static large_type has_matching_function_member(...);\
		static const bool value=sizeof(has_matching_function_member<T>(0))==success;\
		static_assert(value || !assert_requirements,"\n\n\n[--------- "#ERROR_MESSAGE" ---------]\n\n");\
	};\
	typedef typename Append<auto_check_list_##LINKED_CONCEPT,REQUIREMENT_NAME>::Result auto_check_list_##REQUIREMENT_NAME;


//#define member_feature_accessor(FEATURE_ALIAS, MEMBER_COMPONENT_NAME, MEMBER_COMPONENT_FEATURE, MEMBER_COMPONENT_PROTOTYPE)\
//	define_get_set_exists_check(MEMBER_COMPONENT_NAME, MEMBER_COMPONENT_NAME##_get_exists, MEMBER_COMPONENT_NAME##_set_exists);\
//	feature_prototype TargetType FEATURE_ALIAS(requires(check(ComponentType,MEMBER_COMPONENT_NAME##_get_exists)))\
//	{\
//		define_component_interface_local(MEMBER_COMPONENT_NAME##_itf,MEMBER_COMPONENT_PROTOTYPE,MEMBER_COMPONENT_NAME,ComponentType);\
//		MEMBER_COMPONENT_NAME##_itf* itf = cast_self_to_component().MEMBER_COMPONENT_NAME<ComponentType,CallerType,MEMBER_COMPONENT_NAME##_itf*>();\
//		return itf->template MEMBER_COMPONENT_FEATURE<TargetType>();\
//	}\
//	feature_prototype TargetType FEATURE_ALIAS(requires(check(ComponentType,!MEMBER_COMPONENT_NAME##_get_exists)))\
//	{\
//		assert_check(ComponentType, MEMBER_COMPONENT_NAME##_get_exists, "The member component specified does not exists.  Did you tag with the tag_as_getter/setter macros?");\
//	}\
//	feature_prototype void FEATURE_ALIAS(TargetType value, requires(check(ComponentType,MEMBER_COMPONENT_NAME##_set_exists)))\
//	{\
//		define_component_interface_local(MEMBER_COMPONENT_NAME##_itf,MEMBER_COMPONENT_PROTOTYPE,MEMBER_COMPONENT_NAME,ComponentType);\
//		MEMBER_COMPONENT_NAME##_itf* itf = cast_self_to_component().MEMBER_COMPONENT_NAME<ComponentType,CallerType,MEMBER_COMPONENT_NAME##_itf*>();\
//		itf->template MEMBER_COMPONENT_FEATURE<TargetType>(value);\
//	}

#define get_exists_check(FEATURE_NAME) FEATURE_NAME##_get_exists
#define set_exists_check(FEATURE_NAME) FEATURE_NAME##_set_exists

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
	const __int64& get_start_value(){return _start;}
	const double& get_freq_value(){return _freq;}
	const long long& get_l_value(){return _l.QuadPart;}
};

//================================================================================================================================================================
//================================================================================================================================================================
//================================================================================================================================================================
#pragma endregion

#ifndef WINDOWS
#define FLT_MAX	3.402823466e+38F	/* max value */
#endif