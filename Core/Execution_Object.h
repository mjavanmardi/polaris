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
	Execution_Object():next_iteration(INT_MAX){};

	permit_state_checking;
	


	
	inline Revision&& object_current_revision()
	{
		return Revision(0,current_revision);
	}

	inline Revision&& object_next_check()
	{
		if(next_iteration==_iteration)
		{
			return Revision(_sub_iteration+1,next_iteration);
		}
		else
		{
			return Revision(0,next_iteration);
		}
	}
	

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
	
	__forceinline void Swap_Event(Event new_event)
	{
		event_register=new_event;
	}
	
	template<typename ComponentType>
	void Load_Register(Conditional conditional,Event p_event,int start);
	
	//void Unload_Register()
	//{
	//	//conditional_register=&False;
	//	event_register=nullptr;
	//}
	

	Conditional conditional_register;

	int next_iteration;
	int current_revision;

	union
	{
		Event event_register;

		struct
		{
			Execution_Object* next_free_cell;
		};
	};
};

typedef void (*Execution_Directive)(void*,Revision&);

///============================================================================
/// Tight loop function to process a given execution page
///============================================================================

template<const int iterations,const int stride>
void Execution_Loop(void* page_in, Revision& ptex_response)
{
	Bytes<stride>* page=(Bytes<stride>*)page_in;
	
	const Bytes<stride>* const __restrict end_page=page+iterations;
	
	const int this_iteration=_iteration;
	
	Conditional_Response optex_response;
	
	do
	{
		//not necessary if developer doesn't compare response initial values
		//response.next=INT_MAX;
		//response.result=false;

		if(((Execution_Object*)page)->next_iteration==this_iteration)
		{
			((Execution_Object*)page)->conditional_register(page,optex_response);
			
			((Execution_Object*)page)->next_iteration=optex_response.next;

			if(optex_response.result) ((Execution_Object*)page)->event_register(page);
					
			((Execution_Object*)page)->current_revision=this_iteration;
		}

		if(((Execution_Object*)page)->next_iteration < ptex_response._iteration)
		{
			ptex_response._iteration=((Execution_Object*)page)->next_iteration;
		}
	}
	while(++page<end_page);
}