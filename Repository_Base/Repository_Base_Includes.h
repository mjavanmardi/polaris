#pragma once
#include "Core\Core.h"
#include <iostream>
#include <fstream>




//================================================================================================================================================================
//================================= TEMPORARY CORE METASTRUCTURE UPDATES =========================================================================================
//================================================================================================================================================================
#pragma region TEMP_CORE
#define NAMESPACE(namespace_name, main_interface_name) \
	struct namespace_name_typedefs {typedef main_interface_name base_interface;}; \
	struct namespace_name : namespace_name_typedefs

#undef THIS
#define THIS ((base_interface*)this)
#define SUB_THIS(other_interface) ((other_interface*)this)

#define CHILD(child_interface,child_base) Polaris_Component<typename ThisType::child_interface,typename ThisType::child_base,ThisType::This_Type>

template<typename Return_Type, typename Param_Type=NULLTYPE, typename Param2_Type=NULLTYPE, typename Param3_Type=NULLTYPE, typename Param4_Type=NULLTYPE>
struct Target_Type
{
	typedef Return_Type ReturnType;
	typedef Param_Type ParamType;
	typedef Param2_Type Param2Type;
	typedef Param3_Type Param3Type;
	typedef Param4_Type Param4Type;
};

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
};

concept Is_Target_Type_Struct
{
	begin_requirements_list;
	requires_typename_defined(none, ReturnType,"Type does not model an HCM simple solution type.");
	requires_typename_defined(ReturnType, ParamType,"Type does not model an HCM simple solution type.");
	requires_typename_defined(ParamType, Param2Type,"Type does not model an HCM simple solution type.");
	end_requirements_list(Param2Type);
};
//================================================================================================================================================================
//================================================================================================================================================================
//================================================================================================================================================================
#pragma endregion