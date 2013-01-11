#pragma once
#include "Memory_Engine.h"

///============================================================================
/// Forward declarations
///============================================================================

template<typename DataType=NULLCLASS>
class Typed_Execution_Pages;

class Execution_Root;

static Execution_Root* execution_root_ptr;

template<typename DataType=NULLCLASS>
struct Typed_Execution_Page;

///============================================================================
/// Execution_Object
/// An object chunk with associated type, which is available for allocation
/// Contains 2 registers for event handling:
/// One activated every step, another activated should the first return true
///============================================================================

struct Execution_Object
{
	permit_state_checking;

	
	//inline long object_current_revision()
	//{
	//	return current_revision;
	//}

	//inline long object_next_check()
	//{
	//	return next_iteration;
	//}
	

	//template<typename ComponentType>
	//inline Revision&& type_current_revision()
	//{
	//	return Revision(allocator_template<ComponentType>::allocator_reference.tex_current_revision);
	//}

	//template<typename ComponentType>
	//inline Revision&& type_next_check()
	//{
	//	return Revision(allocator_template<ComponentType>::allocator_reference.tex_next_revision);
	//}
	
	//inline long object_current_revision()
	//{
	//	return current_revision;
	//}

	//inline long object_next_iteration()
	//{
	//	return next_iteration;
	//}

	//template<typename ComponentType>
	//inline long type_current_revision()
	//{
	//	return allocator_template<ComponentType>::allocator_reference.current_revision._iteration;
	//}

	//template<typename ComponentType>
	//inline long type_next_revision()
	//{
	//	return allocator_template<ComponentType>::allocator_reference.next_revision._iteration;
	//}	
	
	//void Unload_Register()
	//{
	//	//conditional_register=&False;
	//	event_register=nullptr;
	//}


	__forceinline void Swap_Event(Event new_event)
	{
		event_register=new_event;
	}
	
	template<typename ComponentType>
	void Load_Register(Conditional conditional,Event p_event,int start);
	
	Conditional conditional_register;

	///============================================================================
	/// packed_iteration - strips out 4th bit for internal purposes
	///============================================================================

	//struct packed_iteration
	//{
	//	inline int operator=(int value)
	//	{
	//		_value=value;
	//		return _value&0x7fffffff;
	//	}

	//	inline bool operator==(int value)
	//	{
	//		return (_value&0x7fffffff)==value;
	//	}

	//	inline bool operator<(int value)
	//	{
	//		return (_value&0x7fffffff)<value;
	//	}

	//	inline bool operator<=(int value)
	//	{
	//		return (_value&0x7fffffff)<=value;
	//	}

	//	inline bool operator>=(int value)
	//	{
	//		return (_value&0x7fffffff)>=value;
	//	}

	//	inline bool operator>(int value)
	//	{
	//		return (_value&0x7fffffff)>value;
	//	}

	//	inline operator int()
	//	{
	//		return (_value&0x7fffffff);
	//	}

	//	inline void queue_free()
	//	{
	//		_value=0x80000000;
	//	}
	//	
	//	inline void set_free()
	//	{
	//		_value=0x7fffffff;
	//	}

	//	int _value;
	//} next_iteration;

	//union Packed_Revision
	//{
	//	Packed_Revision():_revision(0){};
	//	Packed_Revision(int sub_revision,int revision):_sub_iteration(sub_revision){_iteration=revision;};
	//	Packed_Revision(Packed_Revision& copy):_revision(copy._revision){};

	//	inline void operator = (const long long val){_revision=val;}
	//	inline operator long long&(){return _revision;}
	//#ifdef WINDOWS
	//	struct
	//	{
	//		int _sub_iteration;
	//		packed_iteration _iteration;
	//	};
	//#else
	//	struct
	//	{
	//		packed_iteration _iteration;
	//		int _sub_iteration;
	//	};
	//#endif
	//	long long _revision;
	//} next_iteration;

	Revision next_revision;

	//int current_revision;
	
	union
	{
		Event event_register;

		Execution_Object* next_free_cell;
	};
};

typedef void (*Execution_Directive)(void*,Revision&);

///============================================================================
/// Tight loop function to process a given execution page
///============================================================================

template<typename DataType,const int iterations,const int stride>
void Execution_Loop(void* page_in, Revision& ptex_response)
{
	Bytes<stride>* page=(Bytes<stride>*)page_in;
	
	const Bytes<stride>* const __restrict end_page=page+iterations;
	
	const Revision this_revision=_revision;
	
	Conditional_Response optex_response;
	
	do
	{
		//not necessary if developer doesn't compare response initial values
		//response.next=INT_MAX;
		//response.result=false;

		if( ((Execution_Object*)page)->next_revision._revision == this_revision._revision )
		{
			((Execution_Object*)page)->conditional_register(page,optex_response);

			((Execution_Object*)page)->next_revision=optex_response.next;

			if(optex_response.result) ((Execution_Object*)page)->event_register(page);
		}

		if(((Execution_Object*)page)->next_revision >= ptex_response)
		{
			continue;
		}
		else
		{
			if(((Execution_Object*)page)->next_revision >= 0)
			{
				ptex_response=((Execution_Object*)page)->next_revision;
			}
			else
			{
				(DataType::singleton_reference)->Free((DataType*)page);
			}
		}
	}
	while(++page < end_page);
}