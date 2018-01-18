#pragma once
///----------------------------------------------------------------------------------------------------
/// Simulation_Control.h - Global API for Simulation Control
///----------------------------------------------------------------------------------------------------

#include "Globals.h"
#include "Polaris_Variables.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Global Accessors
	///----------------------------------------------------------------------------------------------------

	static unsigned int num_iterations();
	static unsigned int num_sim_threads();
	static unsigned int num_threads();
	static unsigned int num_antares_threads();
	static unsigned int execution_segments_per_thread();
	static unsigned int execution_objects_per_block();
	static unsigned int max_execution_objects_per_block();
	static unsigned int max_free_blocks();
	static unsigned int num_free_blocks_buffer();
	static long long preallocation_bytes();

	typedef Basic_Units::Time_Variables::Time_Seconds Simulation_Timestep_Increment;

	///----------------------------------------------------------------------------------------------------
	/// Object_Lookup - look up any object of any type by pid
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	DataType* Object_Lookup(int pid)
	{
		bool object_found = false;

		for(unsigned int i=0;i<(num_sim_threads()+1);i++)
		{
			if(DataType::component_manager->_object_repository[i].count(pid))
			{
				return (DataType*)DataType::component_manager->_object_repository[i][pid];
			}
		}
		
		return nullptr;
	}

	///----------------------------------------------------------------------------------------------------
	/// Average_Execution_Objects_Hint - provide a hint to the engine about number of objects expected
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Average_Execution_Objects_Hint(unsigned int value);

	///----------------------------------------------------------------------------------------------------
	/// Component_Manager_Base - base class for all component managers
	///----------------------------------------------------------------------------------------------------

	enum Component_Manager_Types
	{
		EXECUTION_MANAGER,
		EVENT_MANAGER,
		DATA_MANAGER,
		NULL_MANAGER
	};

	class Component_Manager_Base : public boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>>
	{
	public:
		virtual void Initialize() = 0;
		virtual void Terminate() = 0;
		virtual Component_Manager_Types Component_Manager_Type(){ return NULL_MANAGER; }

		std::unordered_map<int,pair<int,void(NULLTYPE::*)(void)>> function_map;
		std::unordered_map<int,void*>* _object_repository;
	};

	extern int __component_counter;
	extern std::unordered_map<size_t,Component_Manager_Base*>* __all_components;

	///----------------------------------------------------------------------------------------------------
	/// INITIALIZE_SIMULATION macro which must be called to initialize the simulation
	///----------------------------------------------------------------------------------------------------

	#define INITIALIZE_SIMULATION(CONFIGURATION_OBJECT)\
		_world = new World();\
		_world->Initialize(CONFIGURATION_OBJECT);\
		for(std::unordered_map<size_t,Component_Manager_Base*>::iterator itr=__all_components->begin();itr!=__all_components->end();itr++){ (itr->second)->Initialize(); }

	///----------------------------------------------------------------------------------------------------
	/// TERMINATE_SIMULATION macro which must be called to clean up the simulation
	///----------------------------------------------------------------------------------------------------

	#define TERMINATE_SIMULATION()\
		_world->Terminate();\
		delete _world;\
		for(std::unordered_map<size_t,Component_Manager_Base*>::iterator itr=__all_components->begin();itr!=__all_components->end();itr++) (itr->second)->Terminate();

	///----------------------------------------------------------------------------------------------------
	/// START macro which must be called to begin the simulation
	///----------------------------------------------------------------------------------------------------

	#define START() _world->Start_Turning();

	///----------------------------------------------------------------------------------------------------
	/// Revision - class for tracking event steps
	///----------------------------------------------------------------------------------------------------
	
	union Revision
	{
		Revision():_revision(-1){};
		Revision(int revision,int sub_revision):_iteration(revision),_sub_iteration(sub_revision){};
		Revision(const Revision& copy):_revision(copy._revision){};
		Revision(const Revision&& copy):_revision(copy._revision){};

		inline bool operator == (const Revision& other) const{return _revision == other._revision;}
		inline bool operator != (const Revision& other) const{return _revision != other._revision;}
		inline bool operator < (const Revision& other) const{return _revision < other._revision;}
		inline bool operator > (const Revision& other) const{return _revision > other._revision;}
		inline bool operator <= (const Revision& other) const{return _revision <= other._revision;}
		inline bool operator >= (const Revision& other) const{return _revision >= other._revision;}

		inline void operator = (const long long val){_revision=val;}
		inline void operator = (const Revision& other){_revision=other._revision;}

		struct
		{
			int _sub_iteration;
			int _iteration;
		};

		long long _revision;
	};
	
	///----------------------------------------------------------------------------------------------------
	/// Global Simulation State Accessors
	///----------------------------------------------------------------------------------------------------

	static int iteration();
	static int sub_iteration();
	static const Revision& revision();

	///----------------------------------------------------------------------------------------------------
	/// Global event step tracking variables
	///----------------------------------------------------------------------------------------------------

	extern const Revision __revision_omega;

	// indicate end of simulation
	//#define END Revision(num_iterations() + 1,0)
	#define END num_iterations() + 1

	// indicate agent to be freed
	extern const Revision __revision_free;

	///----------------------------------------------------------------------------------------------------
	/// Event_Response - user response representing "when" an agent would like to go next
	///----------------------------------------------------------------------------------------------------
	
	struct Event_Response
	{
		Event_Response(){next._iteration=INT_MAX;next._sub_iteration=0;};

		Revision next;
	};

	typedef void (*Event)(void*,Event_Response&);

}
