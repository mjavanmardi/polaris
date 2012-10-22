#pragma once
#include "Polaris_Component.h"

#ifdef WINDOWS
DWORD WINAPI Thread_Loop(LPVOID package_ptr);
#else
void* Thread_Loop(void* package_ptr);
#endif

class World;

struct Thread_Package
{
	World* world_ptr;
	int id;
};

class World
{
public:
	World();
	
	void Start_Turning()
	{
		run=true;

		// instruct threads to leave the finished queue
		threads_finished_counter=0;

		while(true)
		{
			// wait until all threads have entered the finished queue

			while(threads_finished_counter!=num_threads) SLEEP(0);
		
			// can safely say no threads are running at this point
			threads_running_counter=0;

			tick_tock=!tick_tock;

			iteration++;

			sub_iteration=0;

			if(iteration>=num_iterations)
			{
				AtomicDecrement(&run);

				// instruct threads to leave the finished queue (and the simulation)
				threads_finished_counter=0;

				break;
			}
			else
			{
				// instruct threads to leave the finished queue
				threads_finished_counter=0;
			}
		}
	}
	
	volatile long run;

	volatile long threads_finished_counter;
	volatile long threads_running_counter;

	//LARGE_INTEGER frequency;
	//LARGE_INTEGER start;
	//LARGE_INTEGER end;

	Thread_Package packages[num_threads];
#ifdef WINDOWS
	void* threads[num_threads];
#else
	pthread_t threads[num_threads];
#endif
};

World* world_ptr=nullptr;

World::World()
{
	memory_root_ptr=new Memory_Root();
	execution_root_ptr=new Execution_Root();
	data_root_ptr=new Data_Root();
	interprocess_root_ptr=new Interprocess_Engine();
	world_ptr=this;
	run=false;

	// no threads have work yet
	threads_finished_counter=num_threads;
	
	// no threads running
	threads_running_counter=0;

	for(int i=0;i<num_threads;i++)
	{
		packages[i].id=i;
		packages[i].world_ptr=this;
#ifdef WINDOWS
		threads[i]=CreateThread(NULL,0,Thread_Loop,&packages[i],0,NULL);
#else
		pthread_create(&threads[i],NULL,Thread_Loop,&packages[i]);
#endif
	}
}


static World* world=new World();

#ifdef WINDOWS
DWORD WINAPI Thread_Loop(LPVOID package_ptr)
#else
void* Thread_Loop(void* package_ptr)
#endif
{
	World* world=((Thread_Package*)package_ptr)->world_ptr;
	int id=((Thread_Package*)package_ptr)->id;

	thread_id=id;

	Execution_Root* const exe=execution_root_ptr;
	Interprocess_Engine* const iexe=interprocess_root_ptr;

	// thread enters in the finished queue
	while(world->threads_finished_counter!=0) SLEEP(0);

	// thread indicates that it has left the finished queue
	AtomicIncrement(&world->threads_running_counter);

	// thread waits until all threads have left the finished queue to begin actual operation
	while(world->threads_running_counter!=num_threads) SLEEP(0);
	
	while(world->run)
	{
		exe->Process();
		
#ifndef WINDOWS
		interprocess_root_ptr->Exchange();
#endif

		// let the world know that this thread is finished
		AtomicIncrement(&world->threads_finished_counter);
		
		// enter the finished queue
		while(world->threads_finished_counter!=0) SLEEP(0);

		// thread indicates that it has left the finished queue
		AtomicIncrement(&world->threads_running_counter);

		// thread waits until all threads have left the finished queue to begin actual operation
		while(world->threads_running_counter!=num_threads) SLEEP(0);
	}

	return 0;
}


template<typename ComponentType>
void Execution_Object::Load_Register(Conditional conditional,Event p_event,int start)	
{
	Revision starting_iteration;
	starting_iteration.iteration=start;
	starting_iteration.sub_iteration=0;
	
	if(world_ptr->run)
	{
		if(starting_iteration.iteration <= iteration) starting_iteration.iteration=iteration+1;

		Revision this_revision;
		this_revision.iteration=iteration;
		this_revision.sub_iteration=sub_iteration;

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
					execution_type->tex_next_next_revision.iteration=LONG_MAX;
					execution_type->tex_next_next_revision.sub_iteration=0;
					}
			}
		}
		
		execution_type->tex_lock=0; // unlock the TEX

		// Following makes PTEX aware


		// Must first locate PTEX corresponding to self

		// we are assured to be at the top of the object, so cast to BYTEs
		Byte* _this=(Byte*)this;
		
		// next, re-adjust to the start of the page
		
		int dist=(int)(_this-(Byte*)memory_root_ptr->pages);

		_this=((dist/Page_Size)*Page_Size+(Byte*)memory_root_ptr->pages);
		
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
				execution_page->ptex_next_next_revision.iteration=LONG_MAX;
				execution_page->ptex_next_next_revision.sub_iteration=0;
			}
		}

		execution_page->ptex_lock=0; // unlock the page

		// Following makes OPTEX aware
		// there should be no problem making this assignment as it doesn't matter whether it is acknowledged or not this revision
		
		next_iteration=starting_iteration.iteration;
		

		//============================END==========================



	}
	else
	{

		if(starting_iteration <= execution_root_ptr->ex_next_revision)
		{
			execution_root_ptr->ex_next_revision=starting_iteration;
			execution_root_ptr->ex_next_next_revision.iteration=LONG_MAX;
			execution_root_ptr->ex_next_next_revision.sub_iteration=0;
		}
		else if(starting_iteration < execution_root_ptr->ex_next_next_revision)
		{
			execution_root_ptr->ex_next_next_revision=starting_iteration;
		}


		
		Typed_Execution_Pages<ComponentType>* execution_type=(Typed_Execution_Pages<ComponentType>*)ComponentType::singleton_reference;

		if(starting_iteration <= execution_type->tex_next_revision)
		{
			execution_type->tex_next_revision=starting_iteration;
			execution_type->tex_next_next_revision.iteration=LONG_MAX;
			execution_type->tex_next_next_revision.sub_iteration=0;
		}
		else if(starting_iteration < execution_type->tex_next_next_revision)
		{
			execution_type->tex_next_next_revision=starting_iteration;
		}


		// Must first locate PTEX corresponding to self
		Byte* _this=(Byte*)this;
		int dist=(int)(_this-(Byte*)memory_root_ptr->pages);
		_this=((dist/Page_Size)*Page_Size+(Byte*)memory_root_ptr->pages);
		Typed_Execution_Page<ComponentType>* execution_page=(Typed_Execution_Page<ComponentType>*)_this;

		if(starting_iteration <= execution_page->ptex_next_revision)
		{
			execution_page->ptex_next_revision=starting_iteration;
			execution_page->ptex_next_next_revision.iteration=LONG_MAX;
			execution_page->ptex_next_next_revision.sub_iteration=0;
		}
		else if(starting_iteration < execution_page->ptex_next_next_revision)
		{
			execution_page->ptex_next_next_revision=starting_iteration;
		}


		next_iteration=starting_iteration.iteration;
	}
	

	conditional_register=conditional;
	event_register=p_event;
}

template<typename DataType>
DataType* Typed_Execution_Pages<DataType>::Allocate()
{
	if((world_ptr)->run) while(AtomicExchange(&mem_lock,1)) SLEEP(0); // lock the mem
	
	if(pages_with_free_cells.size()==0)
	{
		if(!type_activated && !world_ptr->run)
		{
			execution_root_ptr->Activate_Type((Typed_Execution_Pages<>*)this);
			type_activated=true;
		}
		else if(!type_activated) return nullptr;

		Typed_Execution_Page<DataType>* new_page=(Typed_Execution_Page<DataType>*)memory_root_ptr->Allocate();

		new_page->Initialize();
		
		pages_with_free_cells.push_back(new_page);
		
		active_pages.push_back(new_page);
	}
	
	Byte* return_value=(Byte*)(pages_with_free_cells.front()->Allocate());

	if((Byte*)pages_with_free_cells.front()->first_free_cell==((Byte*)pages_with_free_cells.front()+sizeof(Typed_Execution_Page<DataType>))+num_cells*stride)
	{
		pages_with_free_cells.pop_front();
	}

	new (return_value) DataType();

	if(world_ptr->run) mem_lock=0; // unlock the mem

	return (DataType*)return_value;
}

template<typename DataType>
DataType* Typed_Data_Pages<DataType>::Allocate()
{
	while(AtomicExchange(&mem_lock,1)) SLEEP(0); // lock the page

	if(pages_with_free_cells.size()==0)
	{
		//if(!type_activated && !world_ptr->run)
		//{
		//	data_root_ptr->Activate_Type((Typed_Data_Pages<>*)this);
		//	type_activated=true;
		//}
		//else assert(false);

		Typed_Data_Page<DataType>* new_page=(Typed_Data_Page<DataType>*)memory_root_ptr->Allocate();

		new_page->Initialize();
		
		pages_with_free_cells.push_back(new_page);
		
		active_pages.push_back(new_page);
	}

	Byte* return_value=(Byte*)(pages_with_free_cells.front()->Allocate());
		
	new (return_value) DataType();

	if((Byte*)pages_with_free_cells.front()->first_free_cell==((Byte*)pages_with_free_cells.front()+sizeof(Typed_Data_Page<DataType>))+num_cells*stride)
	{
		pages_with_free_cells.pop_front();
	}
		
	mem_lock=0; // unlock the page

	return (DataType*)return_value;
}