#pragma once
///----------------------------------------------------------------------------------------------------
/// World.h - Container for the Discrete Event Engine and Memory Allocator
///----------------------------------------------------------------------------------------------------

#include "Simulation_Engine.h"

namespace polaris
{
	static bool _world_is_running();

	///----------------------------------------------------------------------------------------------------
	/// Simulation_Configuration.h - Configuration Object for World
	///----------------------------------------------------------------------------------------------------

	struct Simulation_Configuration
	{
		Simulation_Configuration():_num_sim_threads(-1),_preallocation_bytes(-1),_num_iterations(-1),_execution_segments_per_thread(-1),
			_execution_objects_per_block(-1),_max_execution_objects_per_block(-1),_max_free_blocks(-1),_num_free_blocks_buffer(-1){}

		void Check_Configuration()
		{
			if(_num_sim_threads <= 0)
			{
				THROW_EXCEPTION("num_threads not configured");
			}

			if(_preallocation_bytes < 0)
			{
				THROW_EXCEPTION("pre_allocation_amount not configured");
			}

			if(_num_iterations <= 0)
			{
				THROW_EXCEPTION("num_iterations not configured");
			}

			if(_execution_segments_per_thread <= 0)
			{
				THROW_EXCEPTION("execution_blocks_per_thread not configured");
			}

			if(_execution_objects_per_block <= 0)
			{
				THROW_EXCEPTION("execution_objects_per_block not configured");
			}

			if(_max_execution_objects_per_block <= 0)
			{
				THROW_EXCEPTION("max_execution_objects_per_block not configured");
			}

			if(_max_free_blocks <= 0)
			{
				THROW_EXCEPTION("max_free_blocks not configured");
			}

			if(_num_free_blocks_buffer <= 0)
			{
				THROW_EXCEPTION("num_free_blocks_buffer not configured");
			}
		}

		void Single_Threaded_Setup(int num_iterations)
		{
			_num_sim_threads = 1;
			_preallocation_bytes = 0;
			_num_iterations = num_iterations;
			_execution_segments_per_thread = 20;
			_execution_objects_per_block = 100;
			_max_execution_objects_per_block = 1000;
			_max_free_blocks = 10;
			_num_free_blocks_buffer = 5;
		}
	
		void Multi_Threaded_Setup(int num_iterations,int num_threads)
		{
			_num_sim_threads = num_threads;
			_preallocation_bytes = 0;
			_num_iterations = num_iterations;
			_execution_segments_per_thread = 20;
			_execution_objects_per_block = 100;
			_max_execution_objects_per_block = 1000;
			_max_free_blocks = 10;
			_num_free_blocks_buffer = 5;
		}

		int _num_sim_threads;
		int _preallocation_bytes;
		int _num_iterations;
		int _execution_segments_per_thread;
		int _execution_objects_per_block;
		int _max_execution_objects_per_block;
		int _max_free_blocks;
		int _num_free_blocks_buffer;
	};

	///----------------------------------------------------------------------------------------------------
	/// World - Container for the Discrete Event Engine and Memory Allocator
	///----------------------------------------------------------------------------------------------------

	class World
	{
	public:
		//----------------------------------------------------------------------------------------------------
		// Construction / Destruction Functions
		//----------------------------------------------------------------------------------------------------
	
		void Initialize(Simulation_Configuration& cfg);

		void Terminate();

		//----------------------------------------------------------------------------------------------------
		// Simulation Functions
		//----------------------------------------------------------------------------------------------------
	
		void Start_Turning();
	
		__forceinline bool Is_Running(){return (bool)_running;}

		Simulation_Engine* simulation_engine(){ return _simulation_engine; }

		//----------------------------------------------------------------------------------------------------
		// Timing Coordination Functions
		//----------------------------------------------------------------------------------------------------

		void Send_Signal_To_World();
		void Send_Finished_Signal_To_World();
		void Wait_For_Signal_From_World();
		
		void Send_Signal_To_Threads();
		void Wait_For_Signal_From_Threads();

		__forceinline long Mark_Thread_As_Idle(){ return AtomicIncrement(&_threads_idle_counter); }
		__forceinline void Spin_Until_All_Threads_Idle(){ while(_threads_idle_counter!=_num_sim_threads) SLEEP(0); }

		__forceinline long Mark_Thread_As_Ready(){ return AtomicIncrement(&_threads_ready_counter); }
		__forceinline void Spin_Until_All_Threads_Ready(){ while(_threads_ready_counter!=_num_sim_threads) SLEEP(0); }

		//----------------------------------------------------------------------------------------------------
		// Global Accessor Functions
		//----------------------------------------------------------------------------------------------------

		unsigned int num_iterations(){return _num_iterations;}
		void num_iterations(unsigned int value){_num_iterations = value;}
		
		unsigned int num_sim_threads(){return _num_sim_threads;}
		void num_sim_threads(unsigned int value){_num_sim_threads = value;}
		
		unsigned int num_antares_threads(){return _num_antares_threads;}
		void num_antares_threads(unsigned int value){_num_antares_threads = value;}
		
		unsigned int execution_segments_per_thread(){return _execution_segments_per_thread;}
		void execution_segments_per_thread(unsigned int value){_execution_segments_per_thread = value;}	
		
		unsigned int execution_objects_per_block(){return _execution_objects_per_block;}
		void execution_objects_per_block(unsigned int value){_execution_objects_per_block = value;}	

		unsigned int max_execution_objects_per_block(){return _max_execution_objects_per_block;}
		void max_execution_objects_per_block(unsigned int value){_max_execution_objects_per_block = value;}	

		unsigned int max_free_blocks(){return _max_free_blocks;}
		void max_free_blocks(unsigned int value){_max_free_blocks = value;}	

		unsigned int num_free_blocks_buffer(){return _num_free_blocks_buffer;}
		void num_free_blocks_buffer(unsigned int value){_num_free_blocks_buffer = value;}	

		long long preallocation_bytes(){return _preallocation_bytes;}
		void preallocation_bytes(long long value){_preallocation_bytes = value;}

		int iteration(){return _revision._iteration;}
		void iteration(int value){_revision._iteration = value;}

		int sub_iteration(){return _revision._sub_iteration;}
		void sub_iteration(int value){_revision._sub_iteration = value;}

		const Revision& revision(){return _revision;}
		void revision(const Revision& value){_revision = value;}

	private:
		friend class Simulation_Engine;

		unsigned int _num_iterations;
		unsigned int _num_sim_threads;
		unsigned int _num_antares_threads;
		unsigned int _execution_segments_per_thread;
		unsigned int _execution_objects_per_block;
		unsigned int _max_execution_objects_per_block;

		unsigned int _max_free_blocks;
		unsigned int _num_free_blocks_buffer;

		long long _preallocation_bytes;

		volatile long _threads_idle_counter;
		volatile long _threads_ready_counter;
		volatile bool _running;
		Simulation_Engine* _simulation_engine;

		Revision _revision;

	#ifdef _MSC_VER
		HANDLE _threads_finished_event;
		HANDLE _threads_start_event;
		void** _threads;
	#else
		pthread_mutex_t _threads_finished_mutex;
		pthread_cond_t _threads_finished_conditional;
		pthread_mutex_t _threads_start_mutex;
		pthread_cond_t _threads_start_conditional;
		pthread_t* _threads;
	#endif
	};

	extern World* _world;

	static bool _world_is_running() { return _world->Is_Running(); }

	static unsigned int num_threads(){return _world->num_sim_threads()+1;}

	static int iteration(){return _world->iteration();}
	//static void iteration(unsigned int value){_world->iteration(value);}

	static int sub_iteration(){return _world->sub_iteration();}
	//static void sub_iteration(unsigned int value){_world->sub_iteration(value);}

	static const Revision& revision(){return _world->revision();}
	//static void revision(const Revision& value){_world->revision(value);}


	static unsigned int num_iterations(){return _world->num_iterations();}
	//static void num_iterations(unsigned int value){_world->num_iterations(value);}
	
	static unsigned int num_sim_threads(){return _world->num_sim_threads();}
	//static void num_sim_threads(unsigned int value){_world->num_sim_threads(value);}
	
	static unsigned int num_antares_threads(){return _world->num_antares_threads();}
	//static void num_antares_threads(unsigned int value){_world->num_antares_threads(value);}
	
	static unsigned int execution_segments_per_thread(){return _world->execution_segments_per_thread();}
	//static void execution_segments_per_thread(unsigned int value){_world->execution_segments_per_thread(value);}	

	static unsigned int execution_objects_per_block(){return _world->execution_objects_per_block();}
	//static void execution_objects_per_block(unsigned int value){_world->execution_objects_per_block(value);}	

	static unsigned int max_execution_objects_per_block(){return _world->max_execution_objects_per_block();}
	//static void max_execution_objects_per_block(unsigned int value){_world->max_execution_objects_per_block(value);}	

	static unsigned int max_free_blocks(){return _world->max_free_blocks();}
	//static void max_free_blocks(unsigned int value){_world->max_free_blocks(value);}	

	static unsigned int num_free_blocks_buffer(){return _world->num_free_blocks_buffer();}
	//static void num_free_blocks_buffer(unsigned int value){_world->num_free_blocks_buffer(value);}	

	static long long preallocation_bytes(){return _world->preallocation_bytes();}
	//static void preallocation_bytes(long long value){_world->preallocation_bytes(value);}

}