#pragma once
#include "Polaris_Component.h"

#ifdef WINDOWS
DWORD WINAPI Thread_Loop(LPVOID package_ptr);
#else
static void* Thread_Loop(void* package_ptr);
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
	World(World** world_pt)
	{
		memory_root_ptr=new Memory_Root();
		execution_root_ptr=new Execution_Root();
		data_root_ptr=new Data_Root();
		interprocess_root_ptr=new Interprocess_Engine();
		*world_pt=this;
		run=false;

		// no threads have work yet
		threads_finished_counter=_num_threads;
	
		// no threads running
		threads_running_counter=0;

		for(int i=0;i<_num_threads;i++)
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
	
	void Start_Turning()
	{
		run=true;

		// instruct threads to leave the finished queue
		threads_finished_counter=0;

		while(true)
		{
			// wait until all threads have entered the finished queue

			while(threads_finished_counter!=_num_threads) SLEEP(0);
		
			// can safely say no threads are running at this point
			threads_running_counter=0;

			tick_tock=!tick_tock;

			_iteration++;

			//cout << "Beginning iteration: " << _iteration << endl;

			_sub_iteration=0;

			if(_iteration == (_num_iterations-1))
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

	Thread_Package packages[_num_threads];
#ifdef WINDOWS
	void* threads[_num_threads];
#else
	pthread_t threads[_num_threads];
#endif
};

static World* world_ptr=nullptr;
static World* world=new World(&world_ptr);

#ifdef WINDOWS
static DWORD WINAPI Thread_Loop(LPVOID package_ptr)
#else
static void* Thread_Loop(void* package_ptr)
#endif
{
	World* world=((Thread_Package*)package_ptr)->world_ptr;
	int id=((Thread_Package*)package_ptr)->id;

	_thread_id=id;

	Execution_Root* const exe=execution_root_ptr;
	Interprocess_Engine* const iexe=interprocess_root_ptr;

	// thread enters in the finished queue
	while(world->threads_finished_counter!=0) SLEEP(0);

	// thread indicates that it has left the finished queue
	AtomicIncrement(&world->threads_running_counter);

	// thread waits until all threads have left the finished queue to begin actual operation
	while(world->threads_running_counter!=_num_threads) SLEEP(0);
	
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
		while(world->threads_running_counter!=_num_threads) SLEEP(0);
	}

	return 0;
}
