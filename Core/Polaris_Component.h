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

template<template<class> class ImplementationTemplate=NULLTEMPLATE,typename MasterType=NULLTYPE,typename ObjectType=Data_Object,typename ParentType=NULLTYPE,typename GroupList=NULLTYPE>
class Polaris_Component:
	public Polaris_Component<NULLTEMPLATE,NULLTYPE,ObjectType,ParentType,NULLTYPE>,
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

template<template<class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename GroupList>
class Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,GroupList>:
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
class Polaris_Component<NULLTEMPLATE,NULLTYPE,ObjectType,ParentType,NULLTYPE>:
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

template<template<class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,typename GroupList>
const int Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,GroupList>::component_index=_component_counter++;

template<template<class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,typename GroupList>
vector<void*>* const Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,GroupList>::all_components_reference=&_all_components;

template<template<class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,typename GroupList>
typename Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,GroupList>::Singleton_Type* const 
	Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,GroupList>::Singleton_Type>
	(new Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,GroupList>::Singleton_Type());

	
	
template<template<class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename GroupList>
const int Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,GroupList>::component_index=_component_counter++;

template<template<class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename GroupList>
vector<void*>* const Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,GroupList>::all_components_reference=&_all_components;

template<template<class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename GroupList>
typename Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,GroupList>::Singleton_Type* const 
	Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,GroupList>::Singleton_Type>
	(new Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,GroupList>::Singleton_Type());

///============================================================================
/// component and interface cast macros
///============================================================================

#define this_component() ((ComponentType&)*this)
#define target_to_component(TARGET_COMPONENT_TYPE,POINTER_TO_TARGET) ((TARGET_COMPONENT_TYPE&)*POINTER_TO_TARGET)
#define target_to_interface(INTERFACE_ALIAS,POINTER_TO_TARGET) ((INTERFACE_ALIAS&)(*POINTER_TO_TARGET))
#define iterator_to_interface(INTERFACE_ALIAS,ITERATOR_VARIABLE) ( (INTERFACE_ALIAS&) (*(*ITERATOR_VARIABLE)) )
#define get(INTERFACE_ALIAS,FEATURE_FUNCTION) ( (INTERFACE_ALIAS&) FEATURE_FUNCTION<INTERFACE_ALIAS&>() )
#define set(INTERFACE_ALIAS,FEATURE_FUNCTION,SET_VALUE) FEATURE_FUNCTION<INTERFACE_ALIAS&>(INTERFACE_ALIAS& SET_VALUE)

#define access_communication_singleton(COMPONENT_TYPE,PROTOTYPE,CALLER_TYPE,TARGET_TYPE) ((PROTOTYPE<COMPONENT_TYPE,CALLER_TYPE>*)COMPONENT_TYPE::singleton_reference)

///============================================================================
/// basic load macro
///============================================================================

#define load_event(COMPONENT_TYPE,LOCAL_CONDITIONAL_FUNCTION,LOCAL_EVENT_FUNCTION,FIRST_ITERATION,TARGET_TYPE) \
       ((COMPONENT_TYPE*)this)->template Load_Register<COMPONENT_TYPE>((&LOCAL_CONDITIONAL_FUNCTION<NULLTYPE>),(&LOCAL_EVENT_FUNCTION<TARGET_TYPE>),FIRST_ITERATION)

#define load_communication_handler(COMPONENT_TYPE,HANDLER_FUNCTION,TARGET_TYPE) \
       ((COMPONENT_TYPE*)this)->template Load_Register<COMPONENT_TYPE>((&HANDLER_FUNCTION<TARGET_TYPE>))


///============================================================================
/// Allocate
/// Multiple allocation calls to allocate and construct memory
///============================================================================

template<typename DataType,typename ReturnValueType>
ReturnValueType Allocate(void)
{
	return (ReturnValueType)((DataType::singleton_reference)->Allocate());
}

template<typename DataType>
DataType* Allocate(void)
{
	return (DataType*)((DataType::singleton_reference)->Allocate());
}


#define tag_as_prototype\
	typedef ComponentType ComponentType;\
	typedef CallerType CallerType;\
	typedef true_type Is_Prototype;
	
	
