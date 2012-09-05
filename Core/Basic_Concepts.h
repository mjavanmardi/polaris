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

concept Is_Polaris_Component
{
	begin_requirements_list;

	requires_typename_defined(none,This_Type,"Type is not self-aware");
	requires_typename_defined(This_Type,Parent_Type,"Type is not aware of parent type");
	requires_typename_defined(Parent_Type,Object_Type,"Type is not aware of object type");
	requires_typename_defined(Object_Type,Base_Type,"Type is not aware of base type");
	requires_typename_defined(Base_Type,Interface_Type,"Type is not aware of interface type");
	requires_typename_defined(Interface_Type,Dispatched,"Type is not aware of dispatch capability");
	requires_typename_defined(Dispatched,Entity_Tag,"Type is not aware of entity");

	end_requirements_list(Entity_Tag);
};

///============================================================================
/// Is_Dispatchable - detects whether type can be dispatched
///============================================================================

concept Is_Dispatchable
{
	begin_requirements_list;

	requires_concept(none,Is_Polaris_Component);
	requires_typename_state(Is_Polaris_Component,Dispatched,false_type,"Type has already been Dispatched or Dispatched has not been defined");

	end_requirements_list(Dispatched);
};

///============================================================================
/// Is_Dispatched - detects whether type has been dispatched
///============================================================================

concept Is_Dispatched
{
	begin_requirements_list;

	requires_concept(none,Is_Polaris_Component);
	requires_typename_state(Is_Polaris_Component,Dispatched,true_type,"Type has not been Dispatched or Dispatched has not been defined");
	
	end_requirements_list(Dispatched);
};

///============================================================================
/// Dispatch - wrapper class which acknowledges a dispatch
///============================================================================

template<typename T>
struct Dispatch:public T
{
	typedef true_type Dispatched;
};

///============================================================================
/// Is_Same_Entity - check whether component is part of the same entity
///============================================================================

concept Is_Same_Entity
{
	begin_requirements_list;

	requires_concept(none,Is_Polaris_Component);
	requires_typename_match(Is_Polaris_Component,Entity_Tag,"Entity tags do not match!");
	
	end_requirements_list(Entity_Tag);
};

///============================================================================
/// Is_Same_Component - check whether component is itself
///============================================================================

concept Is_Same_Component
{
	begin_requirements_list;

	requires_concept(none,Is_Polaris_Component);
	requires_typename_match(Is_Polaris_Component,This_Type,"Type tags do not match!");
	
	end_requirements_list(This_Type);
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