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

class World;

static World* world_ptr;

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
		if(next_iteration==iteration)
		{
			return Revision(sub_iteration+1,next_iteration);
		}
		else
		{
			return Revision(0,next_iteration);
		}
	}
	
	template<typename ThisType>
	inline Revision&& type_current_revision()
	{
		return Revision(allocator_template<ThisType>::allocator_reference.current_revision);
	}

	template<typename ThisType>
	inline Revision&& type_next_check()
	{
		return Revision(allocator_template<ThisType>::allocator_reference.next_revision);
	}

	//inline long object_current_revision()
	//{
	//	return current_revision;
	//}

	//inline long object_next_iteration()
	//{
	//	return next_iteration;
	//}

	//template<typename ThisType>
	//inline long type_current_revision()
	//{
	//	return allocator_template<ThisType>::allocator_reference.current_revision.iteration;
	//}

	//template<typename ThisType>
	//inline long type_next_revision()
	//{
	//	return allocator_template<ThisType>::allocator_reference.next_revision.iteration;
	//}	






	__forceinline void Swap_Event(Event new_event)
	{
		event_register=new_event;
	}

	template<typename ThisType>
	void Load_Register(Conditional conditional,Event p_event,int start)
	{
		Revision starting_iteration;
		starting_iteration.iteration_revision=start;
		starting_iteration.sub_iteration_revision=0;
		
		if(world_ptr->run)
		{
			if(starting_iteration.iteration_revision <= iteration) starting_iteration.iteration_revision=iteration+1;

			Revision this_revision;
			this_revision.iteration_revision=iteration;
			this_revision.sub_iteration_revision=sub_iteration;

			// Following makes EX aware, can catch EX in 2 states which are handled identically: A) Pre-Update B) Mid-Update
			
			while(_InterlockedExchange(&execution_root_ptr->ex_lock,1)) Sleep(0); // lock the execution engine
			// At this point we are assuming the following:
			// execution_root_ptr->ex_next_next_revision will not change

			if(starting_iteration < execution_root_ptr->ex_next_next_revision)
			{
				// This update is acceptable as it is indistinguishable from a different thread updating normally
				execution_root_ptr->ex_next_next_revision=starting_iteration;
			}

			execution_root_ptr->ex_lock=0; // unlock the execution engine


			// Following makes TEX aware, can catch TEX in 3 states: A) Pre-update, B) Mid-update, C) Post-update
			
			Typed_Execution_Pages<ThisType>* execution_type=&allocator_template<ThisType>::allocator_reference;

			while(_InterlockedExchange(&execution_type->tex_lock,1)) Sleep(0); // lock the TEX

			// At this point we are assuming the following:
			// execution_type->tex_threads_counter will not change
			// execution_type->next_revision will not change
			// execution_type->next_next_revision will not change

			if(execution_type->tex_threads_counter > 0)
			{
				// one possible case here, we are mid-update

				if(starting_iteration < execution_type->tex_next_next_revision)
				{
					// this can be safely updated as appears indistinguishable from a different thread updating normally
					execution_type->tex_next_next_revision=starting_iteration;
				}
			}
			else
			{
				// two possible cases here, either pre-update or post-update
				// distinguish by checking the next_revision

				if(execution_type->tex_next_revision == this_revision)
				{
					// we are slated to visit this iteration, cannot be post-update as this would be invalid
				
					if(starting_iteration < execution_type->tex_next_next_revision)
					{
						// this can be safely updated as it is assumed to be volatile

						execution_type->tex_next_next_revision=starting_iteration;
					}
				}
				else
				{
					// only remaining option is that we are post-update as we cannot be mid-update

					if(starting_iteration < execution_type->tex_next_revision)
					{
						// we will not otherwise be updated this revision

						execution_type->tex_next_revision=starting_iteration;
						execution_type->tex_next_next_revision=LLONG_MAX;
					}
				}
			}
			
			execution_type->tex_lock=0; // unlock the TEX

			// Following makes PTEX aware


			// Must first locate PTEX corresponding to self

			// we are assured to be at the top of the object, so cast to BYTEs
			Byte* pthis=(Byte*)this;
			
			// next, re-adjust to the start of the page
			
			int dist=(int)(pthis-(Byte*)memory_root_ptr->pages);

			pthis=((dist/Page_Size)*Page_Size+(Byte*)memory_root_ptr->pages);
			
			// finally, cast to PTEX
			Typed_Execution_Page<ThisType>* execution_page=(Typed_Execution_Page<ThisType>*)pthis;


			// Can catch PTEX in 2 states: A) Pre-update or B) Post-update Visited / Not Visited

			while(_InterlockedExchange(&execution_page->ptex_lock,1)) Sleep(0); // lock the page

			
			// At this point we are assuming the following:
			// execution_page->next_revision will not change
			// execution_page->next_next_revision will not change
			

			// two possible cases here, either pre-update or post-update
			// distinguish by checking the next_revision
			
			if(execution_page->ptex_next_revision == this_revision)
			{
				// we are slated to visit this iteration, cannot be post-update as this would be invalid
				
				if(starting_iteration < execution_page->ptex_next_next_revision)
				{
					// this can be safely updated as it is assumed to be volatile
					execution_page->ptex_next_next_revision=starting_iteration;
				}
			}
			else
			{
				// we will not be otherwise updated this revision

				if(starting_iteration < execution_page->ptex_next_revision)
				{
					execution_page->ptex_next_revision=starting_iteration;
					execution_page->ptex_next_next_revision=LLONG_MAX;
				}
			}

			execution_page->ptex_lock=0; // unlock the page

			// Following makes OPTEX aware
			// there should be no problem making this assignment as it doesn't matter whether it is acknowledged or not this revision
			
			next_iteration=starting_iteration.iteration_revision;
			

			//============================END==========================



		}
		else
		{

			if(starting_iteration <= execution_root_ptr->ex_next_revision)
			{
				execution_root_ptr->ex_next_revision=starting_iteration;
				execution_root_ptr->ex_next_next_revision=LLONG_MAX;
			}
			else if(starting_iteration < execution_root_ptr->ex_next_next_revision)
			{
				execution_root_ptr->ex_next_next_revision=starting_iteration;
			}


			
			Typed_Execution_Pages<ThisType>* execution_type=&allocator_template<ThisType>::allocator_reference;

			if(starting_iteration <= execution_type->tex_next_revision)
			{
				execution_type->tex_next_revision=starting_iteration;
				execution_type->tex_next_next_revision=LLONG_MAX;
			}
			else if(starting_iteration < execution_type->tex_next_next_revision)
			{
				execution_type->tex_next_next_revision=starting_iteration;
			}


			// Must first locate PTEX corresponding to self
			Byte* pthis=(Byte*)this;
			int dist=(int)(pthis-(Byte*)memory_root_ptr->pages);
			pthis=((dist/Page_Size)*Page_Size+(Byte*)memory_root_ptr->pages);
			Typed_Execution_Page<ThisType>* execution_page=(Typed_Execution_Page<ThisType>*)pthis;

			if(starting_iteration <= execution_page->ptex_next_revision)
			{
				execution_page->ptex_next_revision=starting_iteration;
				execution_page->ptex_next_next_revision=LLONG_MAX;
			}
			else if(starting_iteration < execution_page->ptex_next_next_revision)
			{
				execution_page->ptex_next_next_revision=starting_iteration;
			}


			next_iteration=starting_iteration.iteration_revision;
		}
		

		conditional_register=conditional;
		event_register=p_event;
	}
	
	//void Unload_Register()
	//{
	//	//conditional_register=&False;
	//	event_register=nullptr;
	//}

	template<typename T> struct allocator_template{static Typed_Execution_Pages<T> allocator_reference;};
	
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

template<typename T>
Typed_Execution_Pages<T> Execution_Object::allocator_template<T>::allocator_reference=Typed_Execution_Pages<T>();

///============================================================================
/// Tight loop function to process a given execution page
///============================================================================

template<const int iterations,const int stride>
static void Execution_Loop(Bytes<stride>* __restrict page, Revision& ptex_response)
{
	const Bytes<stride>* const __restrict end_page=page+iterations;
	
	const int this_iteration=iteration;
	
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

			((Execution_Object*)page)->current_iteration=this_iteration;
		}

		if(((Execution_Object*)page)->next_iteration < ptex_response.iteration_revision)
		{
			ptex_response.iteration_revision=((Execution_Object*)page)->next_iteration;
		}
	}
	while(++page<end_page);
}