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
		define_default_check(!has_prototype_tag && has_object_type && has_master_type && (is_data_object || is_execution_object));
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

//	namespace Container_Concepts
//	{
//		concept struct Is_Iterable
//		{
//			check_concept(is_prototype, polaris::Is_Prototype, T, V);
//			check_typedef_name(has_iterator,iterator);
//			check_accessor_name(has_begin, begin);
//			check_accessor_name(has_end, end);
//			check_accessor_name(has_insert, insert);
//			define_default_check(is_prototype && has_iterator && has_begin && has_end && has_insert);
//		};
//		concept struct Is_Back_Insertion_Sequence
//		{
//			check_concept(is_iterable, Is_Iterable,T,V);
//			check_accessor_name(has_front, front);
//			check_accessor_name(has_back, back);
//			check_accessor_name(has_push_back, push_back);
//			check_accessor_name(has_pop_back, pop_back);
//			define_default_check(is_iterable && has_front && has_back && has_push_back && has_pop_back);
//		};
//		concept struct Is_Random_Access_Sequence
//		{
//			check_concept(is_back_insertion_sequence, Is_Back_Insertion_Sequence,T,V);
//			check_accessor_name(has_at, at);
//			define_default_check(is_back_insertion_sequence && has_at);
//		};
//		concept struct Is_Multidimensional_Random_Access_Sequence
//		{
//			check_concept(is_random_access, Is_Random_Access_Sequence,T,V);
//			check_accessor_name(has_dimensions, dimensions);
//			check_typedef_name(has_index_type, index_type);
//			define_default_check(is_random_access && has_dimensions && has_index_type);
//		};
//		concept struct Is_Associative
//		{
//			/*check_concept(is_iterable, Is_Iterable,T,V);
//			check_accessor_name(has_find, Component_Type::find);
//			check_accessor_name(has_equal_range, Component_Type::equal_range);
//			define_sub_check(Is_Associative_Prototype, has_find_p && has_equal_range_p);
//
//			check_method_name(has_find, find);
//			check_method_name(has_equal_range, equal_range);
//			define_sub_check(Is_Associative_Component, has_find_p && has_equal_range_p);
//
//			define_default_check(is_iterable && has_find && has_equal_range);*/
//			check_typedef_name(has_associative_type, Is_Associative_Type);
//			define_default_check(has_associative_type);
//		};
//	}
}
