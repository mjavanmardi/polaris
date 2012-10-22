#pragma once
#include "Execution_Engine.h"
#include "Data_Engine.h"
#include "Interprocess_Engine.h"
#include "Generic.h"
#include "State_Checking.h"

///============================================================================
/// ObjectToSingleton - convert object type to appropriate singleton class
///============================================================================

template<typename ObjectType,typename ComponentType>
struct ObjectToSingleton{};

template<typename ComponentType>
struct ObjectToSingleton<Data_Object,ComponentType>
{
	typedef Typed_Data_Pages<ComponentType> type;
};

template<typename ComponentType>
struct ObjectToSingleton<Execution_Object,ComponentType>
{
	typedef Typed_Execution_Pages<ComponentType> type;
};

template<typename ComponentType>
struct ObjectToSingleton<Communication_Object,ComponentType>
{
	typedef ComponentType type;
};

///============================================================================
/// Polaris_Component - adds implementation, with parent
///============================================================================

template<template<class> class ImplementationTemplate=NULLTEMPLATE,typename MasterType=NULLTYPE,typename ParentType=NULLTYPE,typename GroupList=NULLTYPE,typename ObjectType=Data_Object>
class Polaris_Component:
	public Polaris_Component<NULLTEMPLATE,NULLTYPE,ParentType,NULLTYPE,ObjectType>,
	public ImplementationTemplate<MasterType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
#ifdef WINDOWS
	friend ImplementationTemplate<MasterType>;
#else
	friend class ImplementationTemplate<MasterType>;
#endif

	static const int component_index;
	typedef typename ObjectToSingleton<ObjectType,Polaris_Component>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;

	typedef Polaris_Component This_Type;
	typedef Polaris_Component Implementation_Type;
	typedef ObjectType Object_Type;
	typedef MasterType Master_Type;

	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;

	typedef ParentType Parent_Type;

	typedef typename ParentType::Entity_Type Entity_Type;
};

///============================================================================
/// Polaris_Component - adds implementation, no parent
///============================================================================

template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList,typename ObjectType>
class Polaris_Component<ImplementationTemplate,MasterType,NULLTYPE,GroupList,ObjectType>:
	public ObjectType,
	public ImplementationTemplate<MasterType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
#ifdef WINDOWS
	friend ImplementationTemplate<MasterType>;
#else
	friend class ImplementationTemplate<MasterType>;
#endif

	static const int component_index;
	typedef typename ObjectToSingleton<ObjectType,Polaris_Component>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;

	typedef Polaris_Component This_Type;
	typedef Polaris_Component Implementation_Type;
	typedef ObjectType Object_Type;
	typedef MasterType Master_Type;
	
	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;

	typedef NULLTYPE Parent_Type;
	
	Parent_Type* Parent()
	{
		return nullptr;
	}

	typedef Polaris_Component Entity_Type;
};

///============================================================================
/// Polaris_Component - adds object, with parent
///============================================================================

template<typename ParentType,typename ObjectType>
class Polaris_Component<NULLTEMPLATE,NULLTYPE,ParentType,NULLTYPE,ObjectType>:
	public ObjectType
{
public:
	ParentType* Parent()
	{
		return _parent;
	}

	void Parent(void* parent)
	{
		_parent=(ParentType*)parent;
	}

	ParentType* _parent;
};


///============================================================================
/// Polaris_Component type tracking information
///============================================================================

template<template<class> class ImplementationTemplate,typename MasterType,typename ParentType,typename GroupList,typename ObjectType>
const int Polaris_Component<ImplementationTemplate,MasterType,ParentType,GroupList,ObjectType>::component_index=component_counter++;

template<template<class> class ImplementationTemplate,typename MasterType,typename ParentType,typename GroupList,typename ObjectType>
vector<void*>* const Polaris_Component<ImplementationTemplate,MasterType,ParentType,GroupList,ObjectType>::all_components_reference=&all_components;

template<template<class> class ImplementationTemplate,typename MasterType,typename ParentType,typename GroupList,typename ObjectType>
typename Polaris_Component<ImplementationTemplate,MasterType,ParentType,GroupList,ObjectType>::Singleton_Type* const 
	Polaris_Component<ImplementationTemplate,MasterType,ParentType,GroupList,ObjectType>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component<ImplementationTemplate,MasterType,ParentType,GroupList,ObjectType>::Singleton_Type>
	(new Polaris_Component<ImplementationTemplate,MasterType,ParentType,GroupList,ObjectType>::Singleton_Type());

	
	
template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList,typename ObjectType>
const int Polaris_Component<ImplementationTemplate,MasterType,NULLTYPE,GroupList,ObjectType>::component_index=component_counter++;

template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList,typename ObjectType>
vector<void*>* const Polaris_Component<ImplementationTemplate,MasterType,NULLTYPE,GroupList,ObjectType>::all_components_reference=&all_components;

template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList,typename ObjectType>
typename Polaris_Component<ImplementationTemplate,MasterType,NULLTYPE,GroupList,ObjectType>::Singleton_Type* const 
	Polaris_Component<ImplementationTemplate,MasterType,NULLTYPE,GroupList,ObjectType>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component<ImplementationTemplate,MasterType,NULLTYPE,GroupList,ObjectType>::Singleton_Type>
	(new Polaris_Component<ImplementationTemplate,MasterType,NULLTYPE,GroupList,ObjectType>::Singleton_Type());
	
	
/*
template<typename DataType,typename ObjectType=typename DataType::Object_Type>
struct type_singleton
{
	typedef Typed_Data_Pages<DataType> singleton_type;
	static Typed_Data_Pages<DataType>* ref;
};

template<typename DataType>
struct type_singleton<DataType,Execution_Object>
{
	typedef Typed_Execution_Pages<DataType> singleton_type;
	static Typed_Execution_Pages<DataType>* ref;
};

template<typename DataType,typename ObjectType>
Typed_Data_Pages<DataType>* type_singleton<DataType,ObjectType>::ref=new Typed_Data_Pages<DataType>();

template<typename DataType>
Typed_Execution_Pages<DataType>* type_singleton<DataType,Execution_Object>::ref=new Typed_Execution_Pages<DataType>(&Execution_Loop<(Page_Size-sizeof(Typed_Execution_Page<DataType>))/sizeof(DataType),sizeof(DataType)>);
*/

///============================================================================
/// Polaris_Component_Execution - adds implementation, with parent
///============================================================================

template<template<class> class ImplementationTemplate=NULLTEMPLATE,typename MasterType=NULLTYPE,typename ParentType=NULLTYPE,typename GroupList=NULLTYPE>
class Polaris_Component_Execution:
	public Polaris_Component_Execution<NULLTEMPLATE,NULLTYPE,ParentType,NULLTYPE>,
	public ImplementationTemplate<MasterType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
#ifdef WINDOWS
	friend ImplementationTemplate<MasterType>;
#else
	friend class ImplementationTemplate<MasterType>;
#endif
	
	static const int component_index;
	typedef typename ObjectToSingleton<Execution_Object,Polaris_Component_Execution>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;

	typedef Polaris_Component_Execution Implementation_Type;
	typedef Polaris_Component_Execution This_Type;
	typedef Execution_Object Object_Type;
	typedef MasterType Master_Type;
	
	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;

	typedef ParentType Parent_Type;

	typedef typename ParentType::Entity_Type Entity_Type;
};

///============================================================================
/// Polaris_Component_Execution - adds implementation, no parent
///============================================================================

template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList>
class Polaris_Component_Execution<ImplementationTemplate,MasterType,NULLTYPE,GroupList>:
	public Execution_Object,
	public ImplementationTemplate<MasterType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
#ifdef WINDOWS
	friend ImplementationTemplate<MasterType>;
#else
	friend class ImplementationTemplate<MasterType>;
#endif
	typedef Polaris_Component_Execution Implementation_Type;
	typedef Polaris_Component_Execution This_Type;
	typedef Execution_Object Object_Type;
	typedef MasterType Master_Type;
	
	static const int component_index;
	typedef typename ObjectToSingleton<Execution_Object,Polaris_Component_Execution>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;

	typedef NULLTYPE Parent_Type;
	
	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;

	Parent_Type* Parent()
	{
		return nullptr;
	}

	typedef Polaris_Component_Execution Entity_Type;
};

///============================================================================
/// Polaris_Component_Execution - adds object, with parent
///============================================================================

template<typename ParentType>
class Polaris_Component_Execution<NULLTEMPLATE,NULLTYPE,ParentType,NULLTYPE>:
	public Execution_Object
{
public:
	ParentType* Parent()
	{
		return _parent;
	}

	void Parent(void* parent)
	{
		_parent=(ParentType*)parent;
	}
	
	ParentType* _parent;
};

///============================================================================
/// Polaris_Component_Execution type tracking information
///============================================================================

template<template<class> class ImplementationTemplate,typename MasterType,typename ParentType,typename GroupList>
const int Polaris_Component_Execution<ImplementationTemplate,MasterType,ParentType,GroupList>::component_index=component_counter++;

template<template<class> class ImplementationTemplate,typename MasterType,typename ParentType,typename GroupList>
vector<void*>* const Polaris_Component_Execution<ImplementationTemplate,MasterType,ParentType,GroupList>::all_components_reference=&all_components;

template<template<class> class ImplementationTemplate,typename MasterType,typename ParentType,typename GroupList>
typename Polaris_Component_Execution<ImplementationTemplate,MasterType,ParentType,GroupList>::Singleton_Type* const 
	Polaris_Component_Execution<ImplementationTemplate,MasterType,ParentType,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component_Execution<ImplementationTemplate,MasterType,ParentType,GroupList>::Singleton_Type>
	(new Polaris_Component_Execution<ImplementationTemplate,MasterType,ParentType,GroupList>::Singleton_Type());

	
template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList>
const int Polaris_Component_Execution<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::component_index=component_counter++;

template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList>
vector<void*>* const Polaris_Component_Execution<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::all_components_reference=&all_components;

template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList>
typename Polaris_Component_Execution<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::Singleton_Type* const 
	Polaris_Component_Execution<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component_Execution<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::Singleton_Type>
	(new Polaris_Component_Execution<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::Singleton_Type());
	
	
	
	
///============================================================================
/// Polaris_Component_Communication - adds implementation, with parent
///============================================================================

template<template<class> class ImplementationTemplate=NULLTEMPLATE,typename MasterType=NULLTYPE,typename ParentType=NULLTYPE,typename GroupList=NULLTYPE>
class Polaris_Component_Communication:
	public Polaris_Component_Communication<NULLTEMPLATE,NULLTYPE,ParentType,NULLTYPE>,
	public ImplementationTemplate<MasterType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
#ifdef WINDOWS
	friend ImplementationTemplate<MasterType>;
#else
	friend class ImplementationTemplate<MasterType>;
#endif
	
	static const int component_index;
	typedef typename ObjectToSingleton<Communication_Object,Polaris_Component_Communication>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;

	typedef Polaris_Component_Communication Implementation_Type;
	typedef Polaris_Component_Communication This_Type;
	typedef Communication_Object Object_Type;
	typedef MasterType Master_Type;
	
	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;

	typedef ParentType Parent_Type;

	typedef typename ParentType::Entity_Type Entity_Type;
};

///============================================================================
/// Polaris_Component_Communication - adds implementation, no parent
///============================================================================

template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList>
class Polaris_Component_Communication<ImplementationTemplate,MasterType,NULLTYPE,GroupList>:
	public Communication_Object,
	public ImplementationTemplate<MasterType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
#ifdef WINDOWS
	friend ImplementationTemplate<MasterType>;
#else
	friend class ImplementationTemplate<MasterType>;
#endif
	typedef Polaris_Component_Communication Implementation_Type;
	typedef Polaris_Component_Communication This_Type;
	typedef Communication_Object Object_Type;
	typedef MasterType Master_Type;
	
	static const int component_index;
	typedef typename ObjectToSingleton<Communication_Object,Polaris_Component_Communication>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;

	typedef NULLTYPE Parent_Type;
	
	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;

	Parent_Type* Parent()
	{
		return nullptr;
	}

	typedef Polaris_Component_Communication Entity_Type;
};

///============================================================================
/// Polaris_Component_Communication - adds object, with parent
///============================================================================

template<typename ParentType>
class Polaris_Component_Communication<NULLTEMPLATE,NULLTYPE,ParentType,NULLTYPE>:
	public Communication_Object
{
public:
	ParentType* Parent()
	{
		return _parent;
	}

	void Parent(void* parent)
	{
		_parent=(ParentType*)parent;
	}
	
	ParentType* _parent;
};

///============================================================================
/// Polaris_Component_Communication type tracking information
///============================================================================

template<template<class> class ImplementationTemplate,typename MasterType,typename ParentType,typename GroupList>
const int Polaris_Component_Communication<ImplementationTemplate,MasterType,ParentType,GroupList>::component_index=component_counter++;

template<template<class> class ImplementationTemplate,typename MasterType,typename ParentType,typename GroupList>
vector<void*>* const Polaris_Component_Communication<ImplementationTemplate,MasterType,ParentType,GroupList>::all_components_reference=&all_components;

template<template<class> class ImplementationTemplate,typename MasterType,typename ParentType,typename GroupList>
typename Polaris_Component_Communication<ImplementationTemplate,MasterType,ParentType,GroupList>::Singleton_Type* const 
	Polaris_Component_Communication<ImplementationTemplate,MasterType,ParentType,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component_Communication<ImplementationTemplate,MasterType,ParentType,GroupList>::Singleton_Type>
	(new Polaris_Component_Communication<ImplementationTemplate,MasterType,ParentType,GroupList>::Singleton_Type());

	
	
template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList>
const int Polaris_Component_Communication<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::component_index=component_counter++;

template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList>
vector<void*>* const Polaris_Component_Communication<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::all_components_reference=&all_components;

template<template<class> class ImplementationTemplate,typename MasterType,typename GroupList>
typename Polaris_Component_Communication<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::Singleton_Type* const 
	Polaris_Component_Communication<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component_Communication<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::Singleton_Type>
	(new Polaris_Component_Communication<ImplementationTemplate,MasterType,NULLTYPE,GroupList>::Singleton_Type());
	
	

///============================================================================
/// component and interface cast macros
///============================================================================

#define cast_self_to_component() ((ComponentType&)*this)
#define cast_target_to_component(TARGET_COMPONENT_TYPE,POINTER_TO_TARGET) ((TARGET_COMPONENT_TYPE&)*POINTER_TO_TARGET)
#define pthis ((ComponentType*)this)

#define cast_self_to_interface(INTERFACE_ALIAS) ((INTERFACE_ALIAS&)*this)
#define cast_target_to_interface(INTERFACE_ALIAS,POINTER_TO_TARGET) ((INTERFACE_ALIAS&)(*POINTER_TO_TARGET))
#define cast_iterator_to_interface(INTERFACE_ALIAS,ITERATOR_VARIABLE) ( (INTERFACE_ALIAS&) (*(*ITERATOR_VARIABLE)) )
#define cast_getter_to_interface(INTERFACE_ALIAS,FEATURE_FUNCTION) ( (INTERFACE_ALIAS&) FEATURE_FUNCTION<INTERFACE_ALIAS&>() )

///============================================================================
/// basic schedule macro
///============================================================================

#define schedule_event_local(SELF_AWARE_TYPE,CONDITIONAL_FUNCTION,LOCAL_EVENT_FUNCTION,FIRST_ITERATION,TARGET_TYPE) \
       ((typename SELF_AWARE_TYPE::This_Type*)this)->template Load_Register<typename SELF_AWARE_TYPE::This_Type>((&CONDITIONAL_FUNCTION<NULLTYPE>),(&LOCAL_EVENT_FUNCTION<TARGET_TYPE>),FIRST_ITERATION)

#define schedule_handler_local(SELF_AWARE_TYPE,HANDLER_FUNCTION,TARGET_TYPE) \
       ((typename SELF_AWARE_TYPE::This_Type*)this)->template Load_Register<typename SELF_AWARE_TYPE::This_Type>((&HANDLER_FUNCTION<TARGET_TYPE>))


///============================================================================
/// Allocate
/// Multiple allocation calls to allocate and construct memory
///============================================================================

template<typename DataType,typename TargetType>
TargetType Allocate(void)
{
	return (TargetType)((DataType::singleton_reference)->Allocate());
}

template<typename DataType>
DataType* Allocate(void)
{
	return (DataType*)((DataType::singleton_reference)->Allocate());
}


#define tag_polaris_prototype\
	typedef ComponentType Component_Type;\
	typedef CallerType Caller_Type;\
	typedef true_type Is_Prototype;
	
	
