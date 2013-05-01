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
		ex_next_revision._iteration=INT_MAX;
		ex_next_revision._sub_iteration=0;
		ex_next_next_revision._iteration=INT_MAX;
		ex_next_next_revision._sub_iteration=0;

		ex_lock=0;

		ex_threads_counter_begin=0;
		ex_threads_counter_end=0;

#ifdef WITH_WAIT
		ex_threads_finished_event = CreateEvent(NULL,TRUE,FALSE,NULL);
#endif
	}

	void Activate_Type(Typed_Execution_Pages<>* ptr)
	{
		active_types.push_back(ptr);
	}
	
	///============================================================================
	/// Process - main entry point for discrete event engine
	///============================================================================

	void Process()
	{
		// log the current _revision

		Revision this_revision;
		this_revision._sub_iteration = _sub_iteration;
		this_revision._iteration = _iteration;

		Revision next_iteration;
		next_iteration._sub_iteration = 0;
		next_iteration._iteration = _iteration+1;

		// you are guaranteed that EX::next_revision will not change until the final thread has finished this EX
		// condition for success is whether the next_revision occurs before the start of the next full iteration

		while(/*ex_next_revision >= this_revision && */ex_next_revision < next_iteration)
		{
			Revision ex_response=ex_next_next_revision;

			list<Typed_Execution_Pages<>*>::iterator itr;

			for(itr=active_types.begin();itr!=active_types.end();itr++)
			{
				Typed_Execution_Pages<>* execution_type=(*itr);
				
				Revision tex_response=execution_type->tex_next_revision;

				// you are guaranteed that TEX::next_revision will not change until the final thread has finished this TEX

				if(tex_response == this_revision)
				{
					tex_response=execution_type->tex_next_next_revision;

					// process one slice of the TEX
					
					(execution_type->*(execution_type->type_process_directive))(tex_response);
					//itr->Process(tex_response);
					
					while(AtomicExchange(&execution_type->tex_lock,1)) SLEEP(0); // lock the type
						
						// TEX slice has revealed that it wishes to return some time in the future
						
						if(tex_response < execution_type->tex_next_next_revision)
						{
							// TEX slice wishes to return sooner in the future than already assumed
							execution_type->tex_next_next_revision=tex_response;
						}

						if(++execution_type->tex_threads_counter == _num_threads)
						{
							// final thread, in charge of getting ready for the next _revision, but only if something actually happened this _revision
							execution_type->tex_next_revision=execution_type->tex_next_next_revision;
							execution_type->tex_next_next_revision._iteration=INT_MAX;
							execution_type->tex_next_next_revision._sub_iteration=0;
							execution_type->tex_threads_counter=0;
							//execution_type->page_stride=max( (int)(( execution_type->active_pages.Size() )/( _Execution_Segments_Per_Thread * _num_threads )), 1 );
						}

					execution_type->tex_lock=0; // unlock the type
				}

				if(tex_response < ex_response)
				{
					ex_response=tex_response;
				}
			}

			while(AtomicExchange(&ex_lock,1)) SLEEP(0); // lock the execution engine
				
				// EX slice has revealed that it wishes to return some time in the future
				
				if(ex_response < ex_next_next_revision)
				{
					// EX slice wishes to return sooner in the future than already assumed
					ex_next_next_revision=ex_response;
				}
				
			ex_lock=0; // unlock the execution engine
			
			// collect all threads and advance the _sub_iteration
			
			if(AtomicIncrement(&ex_threads_counter_begin) == _num_threads)
			{
				ex_next_revision=ex_next_next_revision;
				ex_next_next_revision._iteration=INT_MAX;
				ex_next_next_revision._sub_iteration=0;

				//_sub_iteration++;
				_sub_iteration = ex_next_revision._sub_iteration;
#ifdef WITH_WAIT
				SetEvent(ex_threads_finished_event);
#endif
				ex_threads_counter_begin=0;
			}
			else
			{
#ifdef WITH_WAIT
				WaitForSingleObject(ex_threads_finished_event,INFINITE);
#else
				while(AtomicCompareExchange(&ex_threads_counter_begin,0,0)) SLEEP(0);
#endif
			}
			
			this_revision._sub_iteration = _sub_iteration;
			this_revision._iteration = _iteration;
			
			// condition for collection is whether the next_revision occurs before the start of the next full iteration
			if(/*ex_next_revision >= this_revision && */ex_next_revision < next_iteration)
			{
				// if the _iteration will be repeated, make sure all threads are collected before starting the new loop
				// this prevents threads from getting stuck in the previous AtomicCompareExchange

				if(AtomicIncrement(&ex_threads_counter_end) == _num_threads)
				{
#ifdef WITH_WAIT
					ResetEvent(ex_threads_finished_event);
#endif
					ex_threads_counter_end=0;
				}
				else
				{
					while(AtomicCompareExchange(&ex_threads_counter_end,0,0)) SLEEP(0);
				}
			}
		}
	}

	list<Typed_Execution_Pages<>*> active_types;

	Revision ex_next_next_revision;
	Revision ex_next_revision;

	_lock ex_lock;

	volatile unsigned int ex_threads_counter_begin;
	volatile unsigned int ex_threads_counter_end;

#ifdef WITH_WAIT
	HANDLE ex_threads_finished_event;
#endif
};