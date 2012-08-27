#pragma once
#include "Concepts.h"
#include "Polaris_Component.h"

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
	typedef T ThisType;
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