#pragma once
#include "Core\Core.h"
#include <iostream>
#include <fstream>




//================================================================================================================================================================
//================================= TEMPORARY CORE METASTRUCTURE UPDATES =========================================================================================
//================================================================================================================================================================
#pragma region TEMP_CORE



template<typename Return_Type=NULLTYPE, typename Param_Type=NULLTYPE, typename Param2_Type=NULLTYPE, typename Param3_Type=NULLTYPE, typename Param4_Type=NULLTYPE, typename Param5_Type=NULLTYPE, typename Param6_Type=NULLTYPE, typename Param7_Type=NULLTYPE, typename Param8_Type=NULLTYPE>
struct Target_Type
{
	typedef Return_Type ReturnType;
	typedef Param_Type ParamType;
	typedef Param2_Type Param2Type;
	typedef Param3_Type Param3Type;
	typedef Param4_Type Param4Type;
	typedef Param5_Type Param5Type;
	typedef Param6_Type Param6Type;
	typedef Param7_Type Param7Type;
	typedef Param8_Type Param8Type;
};



#define Basic_Data_Struct(DATA_STRUCT_NAME, DATA_VALUE_TYPE, TYPE_TRAIT_TAGS,...) \
struct DATA_STRUCT_NAME \
{													\
	typedef true_type TYPE_TRAIT_TAGS, __VA_ARGS__; \
	typedef DATA_VALUE_TYPE ValueType; \
	DATA_STRUCT_NAME(DATA_VALUE_TYPE val,call_requires(DATA_VALUE_TYPE,is_arithmetic)): Value(val){} \
	DATA_STRUCT_NAME(call_requires(DATA_VALUE_TYPE,is_arithmetic)): Value(0){} \
	DATA_VALUE_TYPE Value; \
	operator DATA_VALUE_TYPE(){return Value;}; \
	DATA_STRUCT_NAME& operator=(DATA_VALUE_TYPE& obj){Value = obj; return *this;}  \
	DATA_STRUCT_NAME& operator+(DATA_VALUE_TYPE& obj){Value = Value + obj; return *this;}  \
	DATA_STRUCT_NAME& operator-(DATA_VALUE_TYPE& obj){Value = Value - obj; return *this;}  \
	DATA_STRUCT_NAME& operator*(DATA_VALUE_TYPE& obj){Value = Value * obj; return *this;}  \
};

concept Is_Target_Type_Struct
{
	begin_requirements_list (none);
	requires_typename_defined(none, ReturnType,"Type does not model an HCM simple solution type.");
	requires_typename_defined(ReturnType, ParamType,"Type does not model an HCM simple solution type.");
	requires_typename_defined(ParamType, Param2Type,"Type does not model an HCM simple solution type.");
	end_requirements_list(Param2Type);
};



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



//================================================================================================================================================================
//================================================================================================================================================================
//================================================================================================================================================================
#pragma endregion