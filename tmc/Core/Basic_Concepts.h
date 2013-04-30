#pragma once
#include "Concepts.h"
#include "Polaris_Component.h"

///============================================================================
/// True_Concept - concept which always evaluates to true
///============================================================================

template<typename TYPE_A=NULLTYPE,typename TYPE_B=NULLTYPE,bool assert_requirements=false>
struct True_Concept
{
	typedef true_type type;
	static const bool value=true;
};

///============================================================================
/// False_Concept - concept which always evaluates to false
///============================================================================

template<typename TYPE_A=NULLTYPE,typename TYPE_B=NULLTYPE,bool assert_requirements=false>
struct False_Concept
{
	typedef false_type type;
	static const bool value=false;
};

///============================================================================
/// Is_Polaris_Component - detects whether type models a Polaris Component
///============================================================================


concept struct Is_Polaris_Component
{
	// Basic check to satisfy polaris component requirements
	//check_typename_defined(has_this_type,This_Type);
	check_typename_defined(has_parent_type,Parent_Type);
	check_typename_defined(has_group_list,Group_List);
	check_typename_defined(has_object_type,Object_Type);
	check_typename_defined(has_master_type,Master_Type);

	// subchecks which can be used to identify object type
	check_typename_state(is_data_object,Object_Type, Data_Object);
	check_typename_state(is_execution_object,Object_Type, Execution_Object);
	
	// check to make sure it is not a debug version of prototype (i.e. debug prototypes will satisfy all of the above checks due to inheriting from COmponent_Type
	check_typename_state(has_prototype_tag,Is_Prototype, true_type);

	// default check when concept is used
	define_default_check(!has_prototype_tag && has_parent_type && has_group_list && has_object_type && has_master_type && (is_data_object || is_execution_object));
};


concept struct Is_Polaris_Prototype
{
	check_typename_defined(has_component_type,Component_Type);
	check_typename_defined(has_caller_type,Caller_Type);
	check_typename_state(has_prototype_tag,Is_Prototype, true_type);
	define_default_check(has_component_type && has_caller_type && has_prototype_tag);
};


///============================================================================
/// Is_Same_Entity - check whether component is part of the same entity
///============================================================================

concept struct Is_Same_Entity
{
	check_concept(is_polaris_component_check,Is_Polaris_Component);
	check_typename_match(has_matching_entity_type,Entity_Type);
	define_default_check(is_polaris_component_check && has_matching_entity_type);
};

///============================================================================
/// Is_Same_Component - check whether component is itself
///============================================================================

concept struct Is_Same_Component
{
	check_concept(is_polaris_component_check,Is_Polaris_Component);
	check_typename_match(has_matching_this_type,This_Type);
	define_default_check(is_polaris_component_check && has_matching_this_type);
};


/////============================================================================
///// Is_Property - check whether component may be owned
/////============================================================================
//
//concept Is_Property
//{
//	begin_requirements_list;
//
//	requires_concept(none,Is_Polaris_Component);
//	requires_typename_defined(Is_Polaris_Component,Property_Tag,"Type is not property!");
//	
//	end_requirements_list(Property_Tag);
//};
//
/////============================================================================
///// Is_Retainable - check whether component is available for ownership
/////============================================================================
//
//concept Is_Retainable
//{
//	begin_requirements_list;
//
//	requires_concept(none,Is_Property);
//	requires_typename_state(Is_Property,Property_Tag,NULLTYPE,"Type may not be owned!");
//	
//	end_requirements_list(Property_Tag);
//};
//
/////============================================================================
///// Is_Retained_By - check whether component is retained by type
/////============================================================================
//
//concept Is_Retained_By
//{
//	begin_requirements_list;
//
//	requires_concept(none,Is_Retainable);
//	requires_typename_state(Is_Retainable,Property_Tag,TYPE_B,"Type is not owned by given type!");
//	
//	end_requirements_list(Property_Tag);
//};
//
/////============================================================================
///// Retain - wrapper class which "purchases" property
/////============================================================================
//
//template<typename New_Owner,typename Property>
//struct Retain:public Property
//{
//	assert_requirements(Property,Is_Retainable,"Type may not be retained!");
//	typedef New_Owner Property_Tag;
//};
//
/////============================================================================
///// Cede - wrapper class which "sells" property
/////============================================================================
//
//template<typename Prior_Owner,typename Property>
//struct Cede:public Property
//{
//	assert_requirements_2(Property,Prior_Owner,Is_Retained_By,"Type is not retained by given type!");
//	typedef NULLTYPE Property_Tag;
//};

///============================================================================
/// Is_Dispatchable - detects whether type can be dispatched
///============================================================================

//concept Is_Dispatchable
//{
//	begin_requirements_list(none);
//
//	requires_typename_state(none,Dispatched,false_type,"Type has already been Dispatched or Dispatched has not been defined");
//
//	end_requirements_list(Dispatched);
//};

///============================================================================
/// Is_Dispatched - detects whether type has been dispatched
///============================================================================

//concept Is_Dispatched
//{
//	begin_requirements_list(none);
//
//	requires_typename_state(none,Dispatched,true_type,"Type has not been Dispatched or Dispatched has not been defined");
//	
//	end_requirements_list(Dispatched);
//};

///============================================================================
/// Dispatch - wrapper class which acknowledges a dispatch
///============================================================================

//template<typename T>
//struct Dispatch:public T
//{
//	typedef true_type Dispatched;
//};