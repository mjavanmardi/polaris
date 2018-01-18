#pragma once
///----------------------------------------------------------------------------------------------------
/// Event_Component_Manager_Methods.h - type-specific managers for event objects
///----------------------------------------------------------------------------------------------------

#include "Event_Block_Methods.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Event_Component_Manager - Implements type-specific allocation and event stepping behavior
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	class Event_Component_Manager : public Execution_Component_Manager_Base
	{
	public:
		void Update();

		//----------------------------------------------------------------------------------------------------
		// Memory-related functions
		//----------------------------------------------------------------------------------------------------

		void Deactivate_Block( Event_Block* block );

		void Add_Empty_Block( int thread_id ){ AtomicIncrement(&_num_empty_blocks[thread_id]); }
		void Remove_Empty_Block( int thread_id ){ AtomicDecrement(&_num_empty_blocks[thread_id]); }

		void Push_Block_With_Free_Cells(Event_Block* block){ _blocks_with_free_cells[__thread_id].push_back(block); }
		void Pop_Front_Block_With_Free_Cells(){ _blocks_with_free_cells[__thread_id].pop_front(); }


		typedef DataType managed_type;

		Event_Component_Manager(){_name = typeid(DataType).name();}

		virtual void Initialize()
		{
			Execution_Component_Manager_Base::Initialize();

			_blocks_with_free_cells = new std::deque<Event_Block*>[num_sim_threads() + 1];
			_num_empty_blocks = new _atomic_counter[num_sim_threads() + 1]();

			 // build objects which are rounded to the nearest cache line size for fastest possible striding
			_ideal_cell_size = (sizeof(DataType) / __cache_line_size + 1) * __cache_line_size;
		}

		DataType* Allocate( int pid = -1, bool bInPlaceNew = true );
		DataType* Allocate_Array( unsigned int num );

		inline void Clean_Up_Thread_Memory( );

		inline void Free( DataType* ptr );
		inline void Lazy_Free( DataType* ptr );
		inline void Free_Array( DataType* ptr );

		virtual void Step( Revision& out_next_revision );

		//boost::intrusive::list<Event_Block> _active_blocks;
		std::deque<Event_Block*> _active_blocks;

		boost::intrusive::list<Event_Block> _queued_activated_blocks;
		std::deque<Event_Block*> _queued_deactivated_blocks;
		std::deque<Event_Block*>* _blocks_with_free_cells;

		_atomic_counter* _num_empty_blocks;
	};
}