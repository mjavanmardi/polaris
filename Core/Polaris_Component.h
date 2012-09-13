#pragma once
#include "Execution_Engine.h"
#include "Data_Engine.h"
#include "Generic.h"
#include "State_Checking.h"

///============================================================================
/// Polaris_Component - adds interface
///============================================================================

template<template<typename ThisType,typename CallerType> class InterfaceTemplate=NULLTEMPLATE_2,typename BaseType=NULLTYPE,typename ParentType=NULLTYPE,typename EntityType=NULLTYPE,typename ObjectType=Data_Object>
class Polaris_Component:
	public Polaris_Component<NULLTEMPLATE_2,BaseType,ParentType,EntityType,ObjectType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
	friend InterfaceTemplate<BaseType,BaseType>;
	friend InterfaceTemplate<Polaris_Component,Polaris_Component>;
	friend BaseType;

	typedef false_type Dispatched;

	template<typename ThisType=Polaris_Component,typename CallerType=NULLTYPE>
	struct Interface_Type{typedef InterfaceTemplate<ThisType,CallerType> type;};

	typedef Polaris_Component This_Type;
	typedef BaseType Base_Type;
	typedef ObjectType Object_Type;
	typedef EntityType Entity_Tag;
};

///============================================================================
/// Polaris_Component - adds object and entity tag, without parent
///============================================================================

template<typename EntityType,typename ObjectType>
class Polaris_Component<NULLTEMPLATE_2,NULLTYPE,NULLTYPE,EntityType,ObjectType>:
	public ObjectType
{
public:
	typedef NULLTYPE Parent_Type;
};

///============================================================================
/// Polaris_Component - adds object and entity tag, with parent
///============================================================================

template<typename ParentType,typename EntityType,typename ObjectType>
class Polaris_Component<NULLTEMPLATE_2,NULLTYPE,ParentType,EntityType,ObjectType>:
	public ObjectType
{
public:
	typedef ParentType Parent_Type;
	Parent_Type* _parent;
};

///============================================================================
/// Polaris_Component - adds base
///============================================================================

template<typename BaseType,typename ParentType,typename EntityType,typename ObjectType>
class Polaris_Component<NULLTEMPLATE_2,BaseType,ParentType,EntityType,ObjectType>:
	public Polaris_Component<NULLTEMPLATE_2,NULLTYPE,ParentType,EntityType,ObjectType>,
	public BaseType
{
public:

};

///============================================================================
/// Polaris_Component_Data - adds interface
///============================================================================

template<template<typename ThisType,typename CallerType> class InterfaceTemplate=NULLTEMPLATE_2,typename BaseType=NULLTYPE,typename ParentType=NULLTYPE,typename EntityType=NULLTYPE>
class Polaris_Component_Data:
	public Polaris_Component_Data<NULLTEMPLATE_2,BaseType,ParentType,EntityType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif

	friend InterfaceTemplate<BaseType,BaseType>;
	friend InterfaceTemplate<Polaris_Component_Data,Polaris_Component_Data>;
	friend BaseType;

	typedef false_type Dispatched;

	template<typename ThisType=Polaris_Component_Data,typename CallerType=NULLTYPE>
	struct Interface_Type{typedef InterfaceTemplate<ThisType,CallerType> type;};
	
	typedef Polaris_Component_Data This_Type;
	typedef BaseType Base_Type;
	typedef Data_Object Object_Type;
	typedef EntityType Entity_Tag;
};

///============================================================================
/// Polaris_Component_Data - adds object and entity tag, without parent
///============================================================================

template<typename EntityType>
class Polaris_Component_Data<NULLTEMPLATE_2,NULLTYPE,NULLTYPE,EntityType>:
	public Data_Object
{
public:
	typedef NULLTYPE Parent_Type;
};

///============================================================================
/// Polaris_Component_Data - adds object and entity tag, with parent
///============================================================================

template<typename ParentType,typename EntityType>
class Polaris_Component_Data<NULLTEMPLATE_2,NULLTYPE,ParentType,EntityType>:
	public Data_Object
{
public:
	typedef ParentType Parent_Type;
	Parent_Type* _parent;
};

///============================================================================
/// Polaris_Component_Data - adds base
///============================================================================

template<typename BaseType,typename ParentType,typename EntityType>
class Polaris_Component_Data<NULLTEMPLATE_2,BaseType,ParentType,EntityType>:
	public Polaris_Component_Data<NULLTEMPLATE_2,NULLTYPE,ParentType,EntityType>,
	public BaseType
{
public:
};


///============================================================================
/// Polaris_Component_Execution - adds interface
///============================================================================

template<template<typename ThisType,typename CallerType> class InterfaceTemplate=NULLTEMPLATE_2,typename BaseType=NULLTYPE,typename ParentType=NULLTYPE,typename EntityType=NULLTYPE>
class Polaris_Component_Execution:
	public Polaris_Component_Execution<NULLTEMPLATE_2,BaseType,ParentType,EntityType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
	friend InterfaceTemplate<BaseType,BaseType>;
	friend InterfaceTemplate<Polaris_Component_Execution,Polaris_Component_Execution>;
	friend BaseType;

	typedef false_type Dispatched;
	template<typename ThisType=Polaris_Component_Execution,typename CallerType=NULLTYPE>
	struct Interface_Type{typedef InterfaceTemplate<ThisType,CallerType> type;};

	typedef Polaris_Component_Execution This_Type;
	typedef BaseType Base_Type;
	typedef Execution_Object Object_Type;
	typedef EntityType Entity_Tag;
};

///============================================================================
/// Polaris_Component_Execution - adds object and entity tag, without parent
///============================================================================

template<typename EntityType>
class Polaris_Component_Execution<NULLTEMPLATE_2,NULLTYPE,NULLTYPE,EntityType>:
	public Execution_Object
{
public:
	typedef NULLTYPE Parent_Type;
};

///============================================================================
/// Polaris_Component_Execution - adds object and entity tag, with parent
///============================================================================

template<typename ParentType,typename EntityType>
class Polaris_Component_Execution<NULLTEMPLATE_2,NULLTYPE,ParentType,EntityType>:
	public Execution_Object
{
public:
	typedef ParentType Parent_Type;
	Parent_Type* _parent;
};

///============================================================================
/// Polaris_Component_Execution - adds base
///============================================================================

template<typename BaseType,typename ParentType,typename EntityType>
class Polaris_Component_Execution<NULLTEMPLATE_2,BaseType,ParentType,EntityType>:
	public Polaris_Component_Execution<NULLTEMPLATE_2,NULLTYPE,ParentType,EntityType>,
	public BaseType
{
public:
};



///============================================================================
/// basic schedule macro
///============================================================================

#define schedule_event_local(SELF_AWARE_TYPE,CONDITIONAL_FUNCTION,LOCAL_EVENT_FUNCTION,FIRST_ITERATION,TARGET_TYPES,...) \
       ((SELF_AWARE_TYPE::This_Type*)this)->Load_Register<SELF_AWARE_TYPE::This_Type>((Conditional)(&CONDITIONAL_FUNCTION<NULLTYPE>),(Event)(&LOCAL_EVENT_FUNCTION<TARGET_TYPES,__VA_ARGS__>),FIRST_ITERATION)

//#define schedule_event_plocal(SELF_AWARE_TYPE,CONDITIONAL_FUNCTION,LOCAL_EVENT_FUNCTION,FIRST_ITERATION,TARGET_TYPES,...) \
//       ((SELF_AWARE_TYPE::This_Type*)pthis)->Load_Register<SELF_AWARE_TYPE::This_Type>((Conditional)(&CONDITIONAL_FUNCTION<SELF_AWARE_TYPE::This_Type,SELF_AWARE_TYPE::This_Type,NULLTYPE>),(Event)(&SELF_AWARE_TYPE::This_Type::LOCAL_EVENT_FUNCTION<SELF_AWARE_TYPE::This_Type,SELF_AWARE_TYPE::This_Type,TARGET_TYPES,__VA_ARGS__>),FIRST_ITERATION)

///============================================================================
/// pthis macro
///============================================================================

#define PTHIS(SELF_AWARE_TYPE) ((SELF_AWARE_TYPE::This_Type*)this)
#define _pthis ((ThisType*)this)
