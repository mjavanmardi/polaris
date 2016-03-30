///----------------------------------------------------------------------------------------------------
/// Execution_Block.cpp - Block of execution objects in a contiguous memory space
///----------------------------------------------------------------------------------------------------

#include "Execution_Block.h"
#include "World.h"

namespace polaris
{
	const unsigned int Execution_Block::_data_offset = ((sizeof(Execution_Block)/__cache_line_size)+1)*__cache_line_size;

	///----------------------------------------------------------------------------------------------------
	/// Initialize - Initialize the execution block, set execution characteristics to an unscheduled state
	///----------------------------------------------------------------------------------------------------

	void Execution_Block::Initialize(void* component_manager_ptr)
	{
		// Set up the block to an unscheduled state
		_ptex_next_revision = __revision_omega;
		_ptex_next_next_revision = __revision_omega;
		_activated = false;

		_component_manager = component_manager_ptr;

		// Set up block's members
		_ptex_threads_counter = 0;
		UNLOCK(_ptex_lock);
		_thread_processing = -1; // Not being processed by anyone
		_memory_managed_by = -1; // Not managed by anyone

		// Set up block's memory-related members 
		_num_allocated = 0;
		UNLOCK(_memory_lock);

		// Execution Object memory begins just after this object ends
		_first_free_cell = ((Byte*)this) + _data_offset;

		// Initialize all execution objects and the free list
		Byte* current_cell = _first_free_cell;
		const Byte* const end_cell = _first_free_cell + _num_cells*_cell_size;

		while( current_cell != end_cell )
		{
			((Execution_Object*)current_cell)->_execution_block = this;
			((Execution_Object*)current_cell)->_next_free_cell = current_cell + _cell_size;
			((Execution_Object*)current_cell)->_next_revision = __revision_omega;
			
			#ifdef SAFE_MODE
				UNLOCK(((Execution_Object*)current_cell)->_optex_lock);
			#endif

			current_cell += _cell_size;
		}
	}


}