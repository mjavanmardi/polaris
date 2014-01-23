#pragma once
///----------------------------------------------------------------------------------------------------
/// Basic_Concepts.h - Fundamental Concept Checks
///----------------------------------------------------------------------------------------------------

#include "Concepts.h"

namespace polaris
{
	concept struct Is_Prototype
	{
		check_typedef_name(has_component_type,Component_Type);
		check_typedef_name(has_prototype_tag,Is_Prototype);
		define_default_check(has_component_type && has_prototype_tag);
	};

	concept struct Is_Polaris_Component
	{
		// Basic check to satisfy polaris component requirements
		check_typedef_name(has_object_type,Object_Type);
		check_typedef_name(has_master_type,Master_Type);

		// subchecks which can be used to identify object type
		check_typedef_type(is_data_object,Object_Type, Data_Object);
		check_typedef_type(is_execution_object,Object_Type, Execution_Object);
		
		// check to make sure it is not a debug version of prototype (i.e. debug prototypes will satisfy all of the above checks due to inheriting from COmponent_Type
		check_typedef_type(has_prototype_tag,Is_Prototype, true_type);

		// default check when concept is used
		define_default_check(!has_prototype_tag && has_parent_type && has_group_list && has_object_type && has_master_type && (is_data_object || is_execution_object));
	};

	concept struct Is_Target_Type_Struct
	{
		check_typedef_name(check1,ReturnType);
		check_typedef_name(check2,ParamType);
		check_typedef_name(check3,Param2Type);

		define_sub_check(has_ReturnType, check1);
		define_sub_check(has_ParamType, check2);
		define_sub_check(has_Param2Type, check3);
		define_default_check(check1 && check2 && check3);
	};
}
