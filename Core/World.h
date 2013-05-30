#pragma once
#include "Polaris_Component.h"

#ifdef _MSC_VER
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
#ifdef WITH_WAIT
		threads_finished_event = CreateEvent(NULL,FALSE,FALSE,NULL);
		threads_start_event = CreateEvent(NULL,TRUE,FALSE,NULL);
#elif defined WITH_WAIT_LINUX
		pthread_mutex_init(&threads_finished_mutex, NULL);
		pthread_cond_init(&threads_finished_conditional, NULL);
		
		pthread_mutex_init(&threads_start_mutex, NULL);
		pthread_cond_init(&threads_start_conditional, NULL);
#endif
		for(int i=0;i<_num_threads;i++)
		{
			packages[i].id=i;
			packages[i].world_ptr=this;
	#ifdef _MSC_VER
			threads[i]=CreateThread(NULL,0,Thread_Loop,&packages[i],0,NULL);
	#else
			pthread_create(&threads[i],NULL,Thread_Loop,&packages[i]);
	#endif
		}
	}
	
	void Start_Turning()
	{
		Execution_Root* const exe=execution_root_ptr;
#ifdef PROFILING		
		ofstream stats("thread_stats.csv");
#endif
		run=true;
#ifdef WITH_WAIT_LINUX
		// lock this before threads get going, to ensure they don't finish improperly
		pthread_mutex_lock(&threads_finished_mutex);
#endif	
		// instruct threads to leave the finished queue
		threads_finished_counter=0;

		while(true)
		{
			// wait until all threads have entered the finished queue

#ifdef WITH_WAIT
			WaitForSingleObject(threads_finished_event,INFINITE);
#elif defined WITH_WAIT_LINUX
			//threads_finished_mutex becomes unlocked (and allows threads to finish properly), waits for signal
			pthread_cond_wait(&threads_finished_conditional,&threads_finished_mutex);
			//world has threads_finished_mutex locked - it won't unlock until it returns to above statement
#else
			while(threads_finished_counter!=_num_threads) SLEEP(0);
#endif
			// can safely say no threads are running at this point
			threads_running_counter=0;

			_iteration = exe->ex_next_revision._iteration;
#ifdef PROFILING
			if(_iteration % 60 == 0)
			{
				unordered_map<long long,long long>::iterator itr;

				for(itr = iteration_timer.begin();itr != iteration_timer.end();itr++)
				{
					stats << itr->first << "," << ((float)itr->second) << endl;
				}
			}
#endif
			//_iteration++;

			//_sub_iteration=0;

			if( (unsigned int)_iteration >= (_num_iterations-1) )
			{
				AtomicDecrement(&run);

				// instruct threads to leave the finished queue (and the simulation)
				threads_finished_counter=0;

				// let all threads terminate before exiting
				while(threads_finished_counter!=_num_threads) SLEEP(0);

				break;
			}
			else
			{
#ifdef WITH_WAIT
				ResetEvent(execution_root_ptr->ex_threads_finished_event);
				threads_finished_counter=0;
				SetEvent(threads_start_event);
#elif defined WITH_WAIT_LINUX
				//ResetEvent(execution_root_ptr->ex_threads_finished_event);
				threads_finished_counter=0;
				
				// must toggle start lock in order to signal properly
				pthread_mutex_lock(&threads_start_mutex);
				pthread_cond_broadcast(&threads_start_conditional);
				pthread_mutex_unlock(&threads_start_mutex);
#else
				// instruct threads to leave the finished queue
				threads_finished_counter=0;
#endif
			}
		}
	}

	volatile unsigned int run;

#ifdef WITH_WAIT
	HANDLE threads_finished_event;
	HANDLE threads_start_event;
#elif defined WITH_WAIT_LINUX
	pthread_mutex_t threads_finished_mutex;
	pthread_cond_t threads_finished_conditional;
	
	pthread_mutex_t threads_start_mutex;
	pthread_cond_t threads_start_conditional;
#endif

	volatile unsigned int threads_finished_counter;
	volatile unsigned int threads_running_counter;

	//LARGE_INTEGER frequency;
	//LARGE_INTEGER start;
	//LARGE_INTEGER end;

	Thread_Package packages[_num_threads];
#ifdef _MSC_VER
	void* threads[_num_threads];
#else
	pthread_t threads[_num_threads];
#endif
};

static World* world_ptr=nullptr;
static World* world=new World(&world_ptr);

#ifdef _MSC_VER
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

#ifdef INTERPROCESS
		interprocess_root_ptr->Exchange();
#endif

#ifdef WITH_WAIT
		// let the world know that this thread is finished
		if(AtomicIncrement(&world->threads_finished_counter) == _num_threads)
		{
			SetEvent(world->threads_finished_event);
		}
#elif defined WITH_WAIT_LINUX
		pthread_mutex_lock(&world->threads_start_mutex);

		// let the world know that this thread is finished
		if(AtomicIncrement(&world->threads_finished_counter) == _num_threads)
		{
			// must toggle mutex in order to get conditional functionality
			pthread_mutex_lock(&world->threads_finished_mutex);
			pthread_cond_signal(&world->threads_finished_conditional);
			pthread_mutex_unlock(&world->threads_finished_mutex);
			// world has threads_finished_mutex at this point
		}
#else
		AtomicIncrement(&world->threads_finished_counter);
#endif

#ifdef WITH_WAIT
#ifdef PROFILING
		long long my_revision = _sub_iteration;
				
		long long time_advanced = 0;
				
		QueryPerformanceCounter(&thread_start_timers[_thread_id]);
#endif
		WaitForSingleObject(world->threads_start_event,INFINITE);
#ifdef PROFILING
		QueryPerformanceCounter(&thread_stop_timers[_thread_id]);

		LOCK(iteration_timer_lock);

		if(! iteration_timer.count(my_revision))
		{
			iteration_timer[my_revision] = 0;
		}

		iteration_timer[my_revision] += (thread_stop_timers[_thread_id].QuadPart - thread_start_timers[_thread_id].QuadPart);

		UNLOCK(iteration_timer_lock);
#endif
#elif defined WITH_WAIT_LINUX
#ifdef PROFILING
		long long my_revision = _sub_iteration;
		
		long long time_advanced = 0;
		
		start_timer(process_timer[_thread_id]);
#endif
		// world will broadcast when ready to move on
		pthread_cond_wait(&world->threads_start_conditional,&world->threads_start_mutex);
		// mutex automatically locked, but since many threads are being released, must unlock for each
		pthread_mutex_unlock(&world->threads_start_mutex);
#ifdef PROFILING
		end_timer(process_timer[_thread_id],time_advanced);

		LOCK(iteration_timer_lock);

		if(! iteration_timer.count(my_revision))
		{
			iteration_timer[my_revision] = 0;
		}

		iteration_timer[my_revision] += time_advanced;

		UNLOCK(iteration_timer_lock);
#endif
#else
		// enter the finished queue
		while(world->threads_finished_counter!=0) SLEEP(0);
#endif
		// thread indicates that it has left the finished queue
		AtomicIncrement(&world->threads_running_counter);

		// thread waits until all threads have left the finished queue to begin actual operation
		while(world->threads_running_counter!=_num_threads) SLEEP(0);

#ifdef WITH_WAIT
		ResetEvent(world->threads_start_event);
#elif defined WITH_WAIT_LINUX
		
#endif
	}

	// let the world know that this thread is completely finished
	AtomicIncrement(&world->threads_finished_counter);

	return 0;
}
