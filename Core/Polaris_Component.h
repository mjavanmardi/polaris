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

template<typename ComponentType>
struct ObjectToSingleton<NULLTYPE,ComponentType>
{
	typedef NULLTYPE type;
};

template<typename ComponentType,typename ParentType>
struct Entity_Extractor
{
	typedef typename ParentType::Entity_Type type;
};

template<typename ComponentType>
struct Entity_Extractor<ComponentType,NULLTYPE>
{
	typedef ComponentType type;
};

///============================================================================
/// Polaris_Component - adds implementation, without RTTI
///============================================================================

template<typename InheritanceList=NULLTYPELIST,typename MasterType=NULLTYPE,typename ObjectType=Data_Object,typename ParentType=NULLTYPE,bool RTTI=false,unsigned int Desired_Page_Size=_Default_Page_Size,typename GroupList=NULLTYPE>
class Polaris_Component:public ObjectType
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
	static const unsigned int page_factor;

	const int Identify() const{return -1;}

	typedef InheritanceList Inheritance_List;
	typedef typename TypeAt<InheritanceList,1>::Result Component_Type;
	typedef Component_Type ComponentType;

	static const int component_index;
	typedef typename ObjectToSingleton<ObjectType,Component_Type>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;

	typedef ObjectType Object_Type;
	typedef MasterType Master_Type;
	
	typedef false_type RTTI_Type;

	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;

	typedef ParentType Parent_Type;

	typedef typename Entity_Extractor<ComponentType,ParentType>::type Entity_Type;
};

///============================================================================
/// Polaris_Component - adds implementation, with RTTI
///============================================================================

template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,unsigned int Desired_Page_Size,typename GroupList>
class Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,true,Desired_Page_Size,GroupList>:public ObjectType
{
public:
#if STATE_CHECKS
	virtual void State_Check(){};
#endif
	static const unsigned int page_factor;
	
	Polaris_Component():_component_index(component_index){}

	const int _component_index;

	const int Identify() const{return _component_index;}

	typedef InheritanceList Inheritance_List;
	typedef typename TypeAt<InheritanceList,1>::Result Component_Type;
	typedef Component_Type ComponentType;

	static const int component_index;

	typedef true_type RTTI_Type;

	typedef typename ObjectToSingleton<ObjectType,Component_Type>::type Singleton_Type;
	static Singleton_Type* const singleton_reference;
	static vector<void*>* const all_components_reference;

	typedef ObjectType Object_Type;
	typedef MasterType Master_Type;
	
	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;
	
	typedef ParentType Parent_Type;

	typedef typename Entity_Extractor<ComponentType,ParentType>::type Entity_Type;
};

///============================================================================
/// Polaris_Component - adds implementation, non allocable variant
///============================================================================

//template<typename InheritanceList,typename MasterType,unsigned int Desired_Page_Size,typename GroupList>
//class Polaris_Component<InheritanceList,MasterType,NULLTYPE,NULLTYPE,Desired_Page_Size,GroupList>
//{
//public:
//#if STATE_CHECKS
//	virtual void State_Check(){};
//#endif
//	static const unsigned int page_factor;
//
//	typedef InheritanceList Inheritance_List;
//	typedef typename TypeAt<InheritanceList,1>::Result Component_Type;
//	typedef Component_Type ComponentType;
//
//	static const int component_index;
//	typedef typename ObjectToSingleton<NULLTYPE,Component_Type>::type Singleton_Type;
//	static Singleton_Type* const singleton_reference;
//	static vector<void*>* const all_components_reference;
//
//	typedef NULLTYPE Object_Type;
//	typedef MasterType Master_Type;
//	
//	typedef typename RemoveDuplicates<TypeList<GroupList,NULLTYPE>>::Result Group_List;
//
//	typedef NULLTYPE Parent_Type;
//	
//	Parent_Type* Parent()
//	{
//		return nullptr;
//	}
//
//	typedef Component_Type Entity_Type;
//};

////============================================================================
//// Polaris_Component - adds object, with identification
////============================================================================
//
//template<typename ObjectType>
//class Polaris_Component<NULLTYPELIST,NULLTYPE,ObjectType,NULLTYPE,true,0,NULLTYPE>:public ObjectType
//{
//public:
//	Polaris_Component(int component_index)
//	{
//		_component_index=component_index;
//	}
//
//	int _component_index;
//};
//
////============================================================================
//// Polaris_Component - adds object, no identification
////============================================================================
//
//template<typename ObjectType>
//class Polaris_Component<NULLTYPELIST,NULLTYPE,ObjectType,NULLTYPE,false,0,NULLTYPE>:public ObjectType
//{
//public:
//	Polaris_Component(int component_index){}
//};

template<int target,int number,int done = target%number>
struct Find_Closest_Greater_Divisor
{
	static const int result = Find_Closest_Greater_Divisor<target,number+1>::result;
};

template<int target,int number>
struct Find_Closest_Greater_Divisor<target,number,0>
{
	static const int result = number;
};


///============================================================================
/// Polaris_Component type tracking information
///============================================================================

//template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,bool RTTI,unsigned int Desired_Page_Size,typename GroupList>
//const unsigned int Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,RTTI,Desired_Page_Size,GroupList>::page_factor = _Page_Size / Find_Closest_Greater_Divisor<_Page_Size,Desired_Page_Size>::result;

template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,bool RTTI,unsigned int Desired_Page_Size,typename GroupList>
const unsigned int Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,RTTI,Desired_Page_Size,GroupList>::page_factor = _Page_Size / Desired_Page_Size;


template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,bool RTTI,unsigned int Desired_Page_Size,typename GroupList>
const int Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,RTTI,Desired_Page_Size,GroupList>::component_index=_component_counter++;

template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,bool RTTI,unsigned int Desired_Page_Size,typename GroupList>
vector<void*>* const Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,RTTI,Desired_Page_Size,GroupList>::all_components_reference=&_all_components;

template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,bool RTTI,unsigned int Desired_Page_Size,typename GroupList>
typename Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,RTTI,Desired_Page_Size,GroupList>::Singleton_Type* const 
	Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,RTTI,Desired_Page_Size,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,RTTI,Desired_Page_Size,GroupList>::Singleton_Type>
	(new Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,RTTI,Desired_Page_Size,GroupList>::Singleton_Type());
	

template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,unsigned int Desired_Page_Size,typename GroupList>
const unsigned int Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,true,Desired_Page_Size,GroupList>::page_factor=_Page_Size / Desired_Page_Size;

template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,unsigned int Desired_Page_Size,typename GroupList>
const int Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,true,Desired_Page_Size,GroupList>::component_index=_component_counter++;

template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,unsigned int Desired_Page_Size,typename GroupList>
vector<void*>* const Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,true,Desired_Page_Size,GroupList>::all_components_reference=&_all_components;

template<typename InheritanceList,typename MasterType,typename ObjectType,typename ParentType,unsigned int Desired_Page_Size,typename GroupList>
typename Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,true,Desired_Page_Size,GroupList>::Singleton_Type* const 
	Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,true,Desired_Page_Size,GroupList>::singleton_reference
	= Add_Component_Singleton<typename Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,true,Desired_Page_Size,GroupList>::Singleton_Type>
	(new Polaris_Component<InheritanceList,MasterType,ObjectType,ParentType,true,Desired_Page_Size,GroupList>::Singleton_Type());


//template<typename InheritanceList,typename MasterType,unsigned int Desired_Page_Size,typename GroupList>
//const unsigned int Polaris_Component<InheritanceList,MasterType,NULLTYPE,NULLTYPE,Desired_Page_Size,GroupList>::page_factor=Desired_Page_Size;
//
//template<typename InheritanceList,typename MasterType,unsigned int Desired_Page_Size,typename GroupList>
//const int Polaris_Component<InheritanceList,MasterType,NULLTYPE,NULLTYPE,Desired_Page_Size,GroupList>::component_index=_component_counter++;
//
//template<typename InheritanceList,typename MasterType,unsigned int Desired_Page_Size,typename GroupList>
//vector<void*>* const Polaris_Component<InheritanceList,MasterType,NULLTYPE,NULLTYPE,Desired_Page_Size,GroupList>::all_components_reference=&_all_components;
//
//template<typename InheritanceList,typename MasterType,unsigned int Desired_Page_Size,typename GroupList>
//typename Polaris_Component<InheritanceList,MasterType,NULLTYPE,NULLTYPE,Desired_Page_Size,GroupList>::Singleton_Type* const 
//	Polaris_Component<InheritanceList,MasterType,NULLTYPE,NULLTYPE,Desired_Page_Size,GroupList>::singleton_reference
//	= Add_Component_Singleton<typename Polaris_Component<InheritanceList,MasterType,NULLTYPE,NULLTYPE,Desired_Page_Size,GroupList>::Singleton_Type>
//	(new Polaris_Component<InheritanceList,MasterType,NULLTYPE,NULLTYPE,Desired_Page_Size,GroupList>::Singleton_Type());












///============================================================================
/// singleton access macros
///============================================================================

#define access_communication_singleton(COMPONENT_TYPE,PROTOTYPE,CALLER_TYPE) ((PROTOTYPE<COMPONENT_TYPE,CALLER_TYPE>*)COMPONENT_TYPE::singleton_reference)

///============================================================================
/// basic load macro
///============================================================================

#define load_event(COMPONENT_TYPE,LOCAL_CONDITIONAL_FUNCTION,LOCAL_EVENT_FUNCTION,FIRST_ITERATION, FIRST_SUB_ITERATION,TARGET_TYPE) \
       ((COMPONENT_TYPE*)this)->template Load_Event<COMPONENT_TYPE>((&LOCAL_CONDITIONAL_FUNCTION<NULLTYPE>),(&LOCAL_EVENT_FUNCTION<TARGET_TYPE>),FIRST_ITERATION, FIRST_SUB_ITERATION)

#define load_event_implementation(COMPONENT_TYPE,LOCAL_CONDITIONAL_FUNCTION,LOCAL_EVENT_FUNCTION,FIRST_ITERATION, FIRST_SUB_ITERATION,TARGET_TYPE) \
       ((COMPONENT_TYPE*)this)->template Load_Event<COMPONENT_TYPE>((&LOCAL_CONDITIONAL_FUNCTION<COMPONENT_TYPE,NULLTYPE,TARGET_TYPE>),(&LOCAL_EVENT_FUNCTION<COMPONENT_TYPE,NULLTYPE,TARGET_TYPE>),FIRST_ITERATION, FIRST_SUB_ITERATION)

#define load_communication_handler(COMPONENT_TYPE,HANDLER_FUNCTION,TARGET_TYPE) \
       ((COMPONENT_TYPE*)this)->template Load_Event<COMPONENT_TYPE>((&HANDLER_FUNCTION<TARGET_TYPE>))

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
