#pragma once
#include "Polaris_IO\Polaris_IO.h"
#include "Core/Core.h"


//#define member_feature_accessor(FEATURE_ALIAS, MEMBER_COMPONENT_NAME, MEMBER_COMPONENT_FEATURE, MEMBER_COMPONENT_PROTOTYPE)\
//	define_get_set_exists_check(MEMBER_COMPONENT_NAME, MEMBER_COMPONENT_NAME##_get_exists, MEMBER_COMPONENT_NAME##_set_exists);\
//	feature_prototype TargetType FEATURE_ALIAS(requires(check(ComponentType,MEMBER_COMPONENT_NAME##_get_exists)))\
//	{\
//		define_component_interface(MEMBER_COMPONENT_NAME##_itf,MEMBER_COMPONENT_NAME,MEMBER_COMPONENT_PROTOTYPE,ComponentType);\
//		MEMBER_COMPONENT_NAME##_itf* itf = this_component().MEMBER_COMPONENT_NAME<ComponentType,CallerType,MEMBER_COMPONENT_NAME##_itf*>();\
//		return itf->template MEMBER_COMPONENT_FEATURE<TargetType>();\
//	}\
//	feature_prototype TargetType FEATURE_ALIAS(requires(check(ComponentType,!MEMBER_COMPONENT_NAME##_get_exists)))\
//	{\
//		assert_check(ComponentType, MEMBER_COMPONENT_NAME##_get_exists, "The member component specified does not exists.  Did you tag with the tag_as_getter macro?");\
//	}\
//	feature_prototype void FEATURE_ALIAS(TargetType value, requires(check(ComponentType,MEMBER_COMPONENT_NAME##_set_exists)))\
//	{\
//		define_component_interface_local(MEMBER_COMPONENT_NAME##_itf,MEMBER_COMPONENT_PROTOTYPE,MEMBER_COMPONENT_NAME,ComponentType);\
//		MEMBER_COMPONENT_NAME##_itf* itf = this_component().MEMBER_COMPONENT_NAME<ComponentType,CallerType,MEMBER_COMPONENT_NAME##_itf*>();\
//		itf->template MEMBER_COMPONENT_FEATURE<TargetType>(value);\
//	}\
//	feature_prototype void FEATURE_ALIAS(TargetType value, requires(check(ComponentType,!MEMBER_COMPONENT_NAME##_set_exists)))\
//	{\
//		assert_check(ComponentType, MEMBER_COMPONENT_NAME##_set_exists, "The member component specified does not exists.  Did you tag with the tag_as_setter macro?");\
//	}


//template<typename Return_Type=NULLTYPE, typename Param_Type=NULLTYPE, typename Param2_Type=NULLTYPE, typename Param3_Type=NULLTYPE, typename Param4_Type=NULLTYPE, typename Param5_Type=NULLTYPE, typename Param6_Type=NULLTYPE, typename Param7_Type=NULLTYPE, typename Param8_Type=NULLTYPE>
//struct Target_Type
//{
//	typedef Return_Type ReturnType;
//	typedef Param_Type ParamType;
//	typedef Param2_Type Param2Type;
//	typedef Param3_Type Param3Type;
//	typedef Param4_Type Param4Type;
//	typedef Param5_Type Param5Type;
//	typedef Param6_Type Param6Type;
//	typedef Param7_Type Param7Type;
//	typedef Param8_Type Param8Type;
//};
//
//concept struct Is_Target_Type_Struct
//{
//	check_typename_defined(check1,ReturnType);
//	check_typename_defined(check2,ParamType);
//	check_typename_defined(check3,Param2Type);
//
//	define_default_check(check1 && check2 && check3);
//};