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

template<template<class,class> class ImplementationTemplate=NULLTEMPLATE_2,typename MasterType=NULLTYPE,typename ObjectType=Data_Object,typename ParentType=NULLTYPE,unsigned int PageFactor=1,typename GroupList=NULLTYPE>
class Polaris_Component:
	public Polaris_Component<NULLTEMPLATE_2,NULLTYPE,ObjectType,ParentType,0,NULLTYPE>,
	public ImplementationTemplate<MasterType,ParentType>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
#ifdef WINDOWS
	friend ImplementationTemplate<MasterType,ParentType>;
#else
	friend class ImplementationTemplate<MasterType>;
#endif

	static const unsigned int page_factor;

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

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
class Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>:
	public ObjectType,
	public ImplementationTemplate<MasterType,NULLTYPE>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
#ifdef WINDOWS
	friend ImplementationTemplate<MasterType,NULLTYPE>;
#else
	friend class ImplementationTemplate<MasterType>;
#endif
	static const unsigned int page_factor;

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
class Polaris_Component<NULLTEMPLATE_2,NULLTYPE,ObjectType,ParentType,0,NULLTYPE>:
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

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,unsigned int PageFactor,typename GroupList>
const unsigned int Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::page_factor=PageFactor;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,unsigned int PageFactor,typename GroupList>
const int Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::component_index=_component_counter++;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,unsigned int PageFactor,typename GroupList>
vector<void*>* const Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::all_components_reference=&_all_components;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,unsigned int PageFactor,typename GroupList>
typename Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::Singleton_Type* const 
	Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::Singleton_Type>
	(new Polaris_Component<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::Singleton_Type());
	

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
const unsigned int Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::page_factor=PageFactor;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
const int Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::component_index=_component_counter++;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
vector<void*>* const Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::all_components_reference=&_all_components;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
typename Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::Singleton_Type* const 
	Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::Singleton_Type>
	(new Polaris_Component<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::Singleton_Type());










///============================================================================
/// Polaris_Component_Class - adds implementation, with parent
///============================================================================

template<template<class,class> class ImplementationTemplate=NULLTEMPLATE_2,typename MasterType=NULLTYPE,typename ObjectType=Data_Object,typename ParentType=NULLTYPE,unsigned int PageFactor=1,typename GroupList=NULLTYPE>
class Polaris_Component_Class:public Polaris_Component_Class<NULLTEMPLATE_2,NULLTYPE,ObjectType,ParentType,0,NULLTYPE>
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
	static const unsigned int page_factor;

	static const int component_index;
	typedef typename ObjectToSingleton<ObjectType,ImplementationTemplate<MasterType,ParentType>>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;
	
	typedef ImplementationTemplate<MasterType,ParentType> This_Type;
	typedef ImplementationTemplate<MasterType,ParentType> Component_Type;
	typedef ImplementationTemplate<MasterType,ParentType> ComponentType;

	typedef ImplementationTemplate<MasterType,ParentType> Implementation_Type;
	typedef ObjectType Object_Type;
	typedef MasterType Master_Type;

	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;

	typedef ParentType Parent_Type;

	typedef typename ParentType::Entity_Type Entity_Type;
};

///============================================================================
/// Polaris_Component_Class - adds implementation, no parent
///============================================================================

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
class Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>:public ObjectType
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
	static const unsigned int page_factor;

	static const int component_index;
	typedef typename ObjectToSingleton<ObjectType,ImplementationTemplate<MasterType,NULLTYPE>>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;

	typedef ImplementationTemplate<MasterType,NULLTYPE> This_Type;
	typedef ImplementationTemplate<MasterType,NULLTYPE> Component_Type;
	typedef ImplementationTemplate<MasterType,NULLTYPE> ComponentType;

	typedef ImplementationTemplate<MasterType,NULLTYPE> Implementation_Type;
	typedef ObjectType Object_Type;
	typedef MasterType Master_Type;
	
	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;

	typedef NULLTYPE Parent_Type;
	
	Parent_Type* Parent()
	{
		return nullptr;
	}

	typedef ImplementationTemplate<MasterType,NULLTYPE> Entity_Type;
};

///============================================================================
/// Polaris_Component_Class - adds object, with parent
///============================================================================

template<typename ParentType,typename ObjectType>
class Polaris_Component_Class<NULLTEMPLATE_2,NULLTYPE,ObjectType,ParentType,0,NULLTYPE>:public ObjectType
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
/// Polaris_Component_Class type tracking information
///============================================================================

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,unsigned int PageFactor,typename GroupList>
const unsigned int Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::page_factor=PageFactor;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,unsigned int PageFactor,typename GroupList>
const int Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::component_index=_component_counter++;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,unsigned int PageFactor,typename GroupList>
vector<void*>* const Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::all_components_reference=&_all_components;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,typename ParentType,unsigned int PageFactor,typename GroupList>
typename Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::Singleton_Type* const 
	Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::Singleton_Type>
	(new Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,ParentType,PageFactor,GroupList>::Singleton_Type());
	

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
const unsigned int Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::page_factor=PageFactor;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
const int Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::component_index=_component_counter++;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
vector<void*>* const Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::all_components_reference=&_all_components;

template<template<class,class> class ImplementationTemplate,typename MasterType,typename ObjectType,unsigned int PageFactor,typename GroupList>
typename Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::Singleton_Type* const 
	Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::Singleton_Type>
	(new Polaris_Component_Class<ImplementationTemplate,MasterType,ObjectType,NULLTYPE,PageFactor,GroupList>::Singleton_Type());














///============================================================================
/// singleton access macros
///============================================================================

#define access_communication_singleton(COMPONENT_TYPE,PROTOTYPE,CALLER_TYPE) ((PROTOTYPE<COMPONENT_TYPE,CALLER_TYPE>*)COMPONENT_TYPE::singleton_reference)

///============================================================================
/// basic load macro
///============================================================================

#define load_event(COMPONENT_TYPE,LOCAL_CONDITIONAL_FUNCTION,LOCAL_EVENT_FUNCTION,FIRST_ITERATION, FIRST_SUB_ITERATION,TARGET_TYPE) \
       ((COMPONENT_TYPE*)this)->template Load_Register<COMPONENT_TYPE>((&LOCAL_CONDITIONAL_FUNCTION<NULLTYPE>),(&LOCAL_EVENT_FUNCTION<TARGET_TYPE>),FIRST_ITERATION, FIRST_SUB_ITERATION)

#define load_communication_handler(COMPONENT_TYPE,HANDLER_FUNCTION,TARGET_TYPE) \
       ((COMPONENT_TYPE*)this)->template Load_Register<COMPONENT_TYPE>((&HANDLER_FUNCTION<TARGET_TYPE>))

///============================================================================
/// Allocate
/// Multiple allocation calls to allocate and construct memory
///============================================================================

template<typename DataType,typename ReturnValueType>
ReturnValueType Allocate(void)
{
	static_assert(!(sizeof(DataType)>_Page_Size),"Allocation request too large, increase the _Page_Size value in Run_Parameters.h");

	return (ReturnValueType)((DataType::singleton_reference)->Allocate());
}

template<typename DataType>
DataType* Allocate(void)
{
	static_assert(!(sizeof(DataType)>_Page_Size),"Allocation request too large, increase the _Page_Size value in Run_Parameters.h");

	return (DataType*)((DataType::singleton_reference)->Allocate());
}

///============================================================================
/// Free
/// Call to free and destruct memory
///============================================================================

template<typename DataType>
inline void Free(DataType* object)
{
	(DataType::singleton_reference)->Queue_Free(object);
}
