#pragma once
#include "Memory_Detail.h"


template<typename ComponentType>
void Execution_Object::Load_Register(Conditional conditional,Event p_event,int start_iteration,int start_sub_iteration)	
{
	Revision starting_iteration;
	starting_iteration._iteration=start_iteration;
	starting_iteration._sub_iteration=start_sub_iteration;
	
	if(world_ptr->run)
	{
		// Maybe should be an assert
		if(starting_iteration._iteration <= _iteration) starting_iteration._iteration=_iteration+1;

		Revision this_revision;
		this_revision._iteration=_iteration;
		this_revision._sub_iteration=_sub_iteration;

		// Following makes EX aware, can catch EX in 2 states which are handled identically: A) Pre-Update B) Mid-Update
		
		while(AtomicExchange(&execution_root_ptr->ex_lock,1)) SLEEP(0); // lock the execution engine
		// At this point we are assuming the following:
		// execution_root_ptr->ex_next_next_revision will not change

		if(starting_iteration < execution_root_ptr->ex_next_next_revision)
		{
			// This update is acceptable as it is indistinguishable from a different thread updating normally
			execution_root_ptr->ex_next_next_revision=starting_iteration;
		}

		execution_root_ptr->ex_lock=0; // unlock the execution engine
		

		// Following makes TEX aware, can catch TEX in 3 states: A) Pre-update, B) Mid-update, C) Post-update
		
		Typed_Execution_Pages<ComponentType>* execution_type=(Typed_Execution_Pages<ComponentType>*)ComponentType::singleton_reference;

		while(AtomicExchange(&execution_type->tex_lock,1)) SLEEP(0); // lock the TEX

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
				// we are slated to visit this _iteration, cannot be post-update as this would be invalid
			
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
					// we will not otherwise be updated this _revision

					execution_type->tex_next_revision=starting_iteration;
					execution_type->tex_next_next_revision._iteration=LONG_MAX;
					execution_type->tex_next_next_revision._sub_iteration=0;
				}
			}
		}
		
		execution_type->tex_lock=0; // unlock the TEX

		// Following makes PTEX aware


		// Must first locate PTEX corresponding to self

		// we are assured to be at the top of the object, so cast to BYTEs
		Byte* _this=(Byte*)this;
		
		// next, re-adjust to the start of the page
		
		long long dist=(long long)(_this-(Byte*)memory_root_ptr->pages);

		_this=((dist/_Page_Size)*_Page_Size+(Byte*)memory_root_ptr->pages);
		
		// finally, cast to PTEX
		Typed_Execution_Page<ComponentType>* execution_page=(Typed_Execution_Page<ComponentType>*)_this;


		// Can catch PTEX in 2 states: A) Pre-update or B) Post-update Visited / Not Visited

		while(AtomicExchange(&execution_page->ptex_lock,1)) SLEEP(0); // lock the page

		
		// At this point we are assuming the following:
		// execution_page->next_revision will not change
		// execution_page->next_next_revision will not change
		

		// two possible cases here, either pre-update or post-update
		// distinguish by checking the next_revision
		
		if(execution_page->ptex_next_revision == this_revision)
		{
			// we are slated to visit this _iteration, cannot be post-update as this would be invalid
			
			if(starting_iteration < execution_page->ptex_next_next_revision)
			{
				// this can be safely updated as it is assumed to be volatile
				execution_page->ptex_next_next_revision=starting_iteration;
			}
		}
		else
		{
			// we will not be otherwise updated this _revision

			if(starting_iteration < execution_page->ptex_next_revision)
			{
				execution_page->ptex_next_revision=starting_iteration;
				execution_page->ptex_next_next_revision._iteration=LONG_MAX;
				execution_page->ptex_next_next_revision._sub_iteration=0;
			}
		}

		execution_page->ptex_lock=0; // unlock the page

		// Following makes OPTEX aware
		// there should be no problem making this assignment as it doesn't matter whether it is acknowledged or not this _revision
		
		next_revision=starting_iteration;
		

		//============================END==========================
	}
	else
	{

		if(starting_iteration <= execution_root_ptr->ex_next_revision)
		{
			execution_root_ptr->ex_next_revision=starting_iteration;
			execution_root_ptr->ex_next_next_revision._iteration=LONG_MAX;
			execution_root_ptr->ex_next_next_revision._sub_iteration=0;
		}
		else if(starting_iteration < execution_root_ptr->ex_next_next_revision)
		{
			execution_root_ptr->ex_next_next_revision=starting_iteration;
		}


		
		Typed_Execution_Pages<ComponentType>* execution_type=(Typed_Execution_Pages<ComponentType>*)ComponentType::singleton_reference;

		if(starting_iteration <= execution_type->tex_next_revision)
		{
			execution_type->tex_next_revision=starting_iteration;
			execution_type->tex_next_next_revision._iteration=LONG_MAX;
			execution_type->tex_next_next_revision._sub_iteration=0;
		}
		else if(starting_iteration < execution_type->tex_next_next_revision)
		{
			execution_type->tex_next_next_revision=starting_iteration;
		}


		// Must first locate PTEX corresponding to self
		Byte* _this=(Byte*)this;
		long long dist=(long long)(_this-(Byte*)memory_root_ptr->pages);
		_this=((dist/_Page_Size)*_Page_Size+(Byte*)memory_root_ptr->pages);
		Typed_Execution_Page<ComponentType>* execution_page=(Typed_Execution_Page<ComponentType>*)_this;

		if(starting_iteration <= execution_page->ptex_next_revision)
		{
			execution_page->ptex_next_revision=starting_iteration;
			execution_page->ptex_next_next_revision._iteration=LONG_MAX;
			execution_page->ptex_next_next_revision._sub_iteration=0;
		}
		else if(starting_iteration < execution_page->ptex_next_next_revision)
		{
			execution_page->ptex_next_next_revision=starting_iteration;
		}


		next_revision=starting_iteration;
	}
	

	conditional_register=conditional;
	event_register=p_event;
}