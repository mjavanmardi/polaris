#pragma once
#include "Typed_Execution_Page.h"

///============================================================================
/// Execution_Root
/// A singleton object responsible for allocating typed execution pages
/// and issuing the process command over all types
///============================================================================

class Execution_Root
{
public:
	Execution_Root()
	{
		ex_current_revision=LLONG_MAX;
		ex_next_revision=LLONG_MAX;

		ex_lock=0;

		ex_threads_counter_begin=0;
		ex_threads_counter_end=0;
	}

	struct Execution_Type
	{
		Execution_Type(Typed_Execution_Pages<>* ptr):typed_execution_pages(ptr){};
		Typed_Execution_Pages<>* typed_execution_pages;
	};

	void Activate_Type(Typed_Execution_Pages<>* ptr)
	{
		active_types.push_back(Execution_Type(ptr));
	}

	void Process()
	{
		// log the current revision

		Revision this_revision;
		this_revision.sub_iteration_revision = sub_iteration;
		this_revision.iteration_revision = iteration;

		// you are guaranteed that EX::current_revision will not change until the final thread has finished this EX

		while(ex_current_revision == this_revision)
		{
			


			Revision ex_response=ex_next_revision;

			list<Execution_Type>::iterator itr;

			for(itr=active_types.begin();itr!=active_types.end();itr++)
			{
				Typed_Execution_Pages<>* execution_type=itr->typed_execution_pages;
				
				Revision tex_response=execution_type->tex_current_revision;

				// you are guaranteed that TEX::current_revision will not change until the final thread has finished this TEX

				if(tex_response == this_revision)
				{
					tex_response=execution_type->tex_next_revision;

					// process one slice of the TEX

					itr->typed_execution_pages->Process(tex_response);

					while(_InterlockedExchange(&execution_type->tex_lock,1)) Sleep(0); // lock the type

					// TEX slice has revealed that it wishes to return some time in the future

					if(tex_response < execution_type->tex_next_revision)
					{
						// TEX slice wishes to return sooner in the future than already assumed
						execution_type->tex_next_revision=tex_response;
					}

					if(++execution_type->tex_threads_counter == num_threads)
					{
						// final thread, in charge of getting ready for the next revision, but only if something actually happend this revision
						execution_type->tex_current_revision=execution_type->tex_next_revision;
						execution_type->tex_next_revision=LLONG_MAX;
						execution_type->tex_threads_counter=0;
					}

					execution_type->tex_lock=0; // unlock the type
				}

				if(tex_response < ex_response)
				{
					ex_response=tex_response;
				}
			}

			while(_InterlockedExchange(&ex_lock,1)) Sleep(0); // lock the execution engine

			// EX slice has revealed that it wishes to return some time in the future
			
			if(ex_response < ex_next_revision)
			{
				// EX slice wishes to return sooner in the future than already assumed
				ex_next_revision=ex_response;
			}

			ex_lock=0; // unlock the execution engine



			
			// collect all threads and advance the sub_iteration

			if(_InterlockedIncrement(&ex_threads_counter_begin) == num_threads)
			{
				ex_current_revision=ex_next_revision;
				ex_next_revision=LLONG_MAX;

				sub_tick_tock=!sub_tick_tock;
				sub_iteration++;

				ex_threads_counter_begin=0;
			}
			else
			{
				while(_InterlockedCompareExchange(&ex_threads_counter_begin,0,0)) Sleep(0);
			}

			this_revision.sub_iteration_revision = sub_iteration;
			this_revision.iteration_revision = iteration;

			
			
			if(ex_current_revision == this_revision)
			{
				// if the iteration will be repeated, make sure all threads are collected before starting the new loop
				// this prevents threads from getting stuck in the previous _InterlockedCompareExchange

				if(_InterlockedIncrement(&ex_threads_counter_end) == num_threads)
				{
					ex_threads_counter_end=0;
				}
				else
				{
					while(_InterlockedCompareExchange(&ex_threads_counter_end,0,0)) Sleep(0);
				}
			}
		}
	}

	list<Execution_Type> active_types;

	Revision ex_next_revision;
	Revision ex_current_revision;

	volatile long ex_lock;

	volatile long ex_threads_counter_begin;
	volatile long ex_threads_counter_end;
};
