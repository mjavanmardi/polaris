#pragma once
///----------------------------------------------------------------------------------------------------
/// Execution_Object.h - Top level execution object which records revision, and memory information
///----------------------------------------------------------------------------------------------------

#include "Runtime_Checking.h"

namespace polaris
{
	// Forward Declaration of Execution_Block
	class Execution_Block;

	///----------------------------------------------------------------------------------------------------
	/// Base_Object - Common base object for the purposes of enabling local type identification
	///----------------------------------------------------------------------------------------------------

	class Base_Object
	{
	public:
		Base_Object():_component_id(-1){}
		
		Base_Object(size_t component_id,int pid): _component_id((unsigned int)component_id),_pid(pid){}

		//virtual void Allocate_Vtable(){}

		unsigned int _component_id;
		int _pid;
	private:
		Base_Object(const Base_Object&);
		void operator=(const Base_Object&);
	};
	
	///----------------------------------------------------------------------------------------------------
	/// Execution_Object - Object which records revision, and memory information
	///----------------------------------------------------------------------------------------------------

	class Execution_Object : public Base_Object
	{
	public:
		Execution_Object():Base_Object(){ _next_revision = __revision_omega; }
		Execution_Object(size_t component_id,int pid):Base_Object(component_id, pid){ _next_revision = __revision_omega; }
		
		//----------------------------------------------------------------------------------------------------
		// Simulation functions
		//----------------------------------------------------------------------------------------------------

		__forceinline void Swap_Event(Event new_event){ _event_callback = new_event; }

		template<typename ComponentType>
		void Load_Event(void (*p_event)(ComponentType*,Event_Response&),int start_iteration,int start_sub_iteration);

		template<typename ComponentType>
		void Reschedule(int start_iteration,int start_sub_iteration);

		inline bool Visiting(){ return _next_revision == revision(); }

		inline const Revision& next_revision(){ return _next_revision; }

		//----------------------------------------------------------------------------------------------------
		// Memory-related functions
		//----------------------------------------------------------------------------------------------------

		inline Execution_Block* execution_block(void){ return _execution_block; }
		void execution_block(Execution_Block* const pExBlock) { _execution_block = pExBlock; }

	private:
		friend class Execution_Block;

		#ifdef SAFE_MODE
			_lock _optex_lock;
		#endif
		
		Revision _next_revision;

		//%%%RLW - Why is there a union here? Well...
		// _event_callback is only used if the Obejct/Cell is allocated
		// _next_free_cell is only used if it isn't allocated
		// this MAY save a pointer's worth of bytes for each object/cell - depends on cell sizing
		union
		{
			Event _event_callback;
			Byte* _next_free_cell;
		};

		Execution_Block* _execution_block;
	};
}