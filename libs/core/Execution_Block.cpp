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

	///----------------------------------------------------------------------------------------------------
	/// Free - Deallocate an object from the block's memory pool, this is a thread safe function
	///----------------------------------------------------------------------------------------------------

//	template<typename DataType>
//	void Execution_Block::Free(DataType* cell)
//	{
//		// Safe mode locks the execution object for increased thread safety
//#ifdef SAFE_MODE
//		LOCK(cell->_optex_lock);
//#elif defined ENABLE_WARNINGS
//		if( cell->Visiting() && _thread_processing!=__thread_id)
//			{
//				THROW_WARNING("Ureliable free of: " << typeid(DataType).name() << " at " << cell << " Use of the self-free option, or rescheduling when it will be freed is recommended for this object");
//			}
//#endif
//
//		// deschedule the cell immediately to minimize collisions
//		cell->_next_revision = __revision_omega;
//
//		// destruct the cell
//		cell->DataType::~DataType();
//
//#ifdef SAFE_MODE
//		UNLOCK(cell->_optex_lock);
//#endif
//
//		if( _world->Is_Running() )
//		{
//			// _memory_lock ensures that nothing will be allocated (or freed) from the memory pool
//			LOCK(_memory_lock);
//
//			// Start from the "middle", provided cell may be before or after the first free cell
//			DataType* current_cell = (DataType*)_first_free_cell;
//
//			if( ((Byte*)cell) < _first_free_cell )
//			{
//				// cell exists before the first free cell, it becomes ffc and previous ffc links to this cell
//
//				_first_free_cell = (Byte*)cell;
//				((DataType*)_first_free_cell)->_next_free_cell = (Byte*)current_cell;
//			}
//			else
//			{
//				// cell exists after the first free cell, forward scan the list to find where it fits
//
//				while(((Byte*)cell) > current_cell->_next_free_cell) current_cell = (DataType*)current_cell->_next_free_cell;
//
//				// link cell to the cell after it in the free list (current_cell), link that cell to cell
//				cell->_next_free_cell = current_cell->_next_free_cell;
//				current_cell->_next_free_cell = (Byte*)cell;
//			}
//
//			// if this block can now be allocated to, current thread takes responsibility for it
//			// we need to lock this because otherwise another thread may empty an object from this block, making it non-full
//			if( Full() )
//			{
//				DataType::component_manager->Push_Block_With_Free_Cells( this );
//				_memory_managed_by = __thread_id;
//			}
//
//			--_num_allocated;
//
//			// This method may have emptied the block
//			// we need to lock this because otherwise another thread may add an object to this block, making it non-empty
//			// note however that the block cannot be newly with free cells or else the Full() condition above would have caught it
//			if( Empty() )
//			{
//				// Need to note that the "managed_by" thread has an empty block lying around
//				DataType::component_manager->Add_Empty_Block( _memory_managed_by );
//				// Need to remove this block from the execution stream
//				DataType::component_manager->Deactivate_Block( this );
//
//				LOCK( _ptex_lock );
//
//				// deschedule the block
//				_ptex_next_revision = __revision_omega;
//				_ptex_next_next_revision = __revision_omega;
//
//				UNLOCK( _ptex_lock );
//			}
//
//			UNLOCK(_memory_lock);
//		}
//		else
//		{
//			// Start from the "middle", provided cell may be before or after the first free cell
//			DataType* current_cell = (DataType*)_first_free_cell;
//
//			if( ((Byte*)cell) < _first_free_cell )
//			{
//				// cell exists before the first free cell, it becomes ffc and previous ffc links to this cell
//
//				_first_free_cell = (Byte*)cell;
//				((DataType*)_first_free_cell)->_next_free_cell = (Byte*)current_cell;
//			}
//			else
//			{
//				// cell exists after the first free cell, forward scan the list to find where it fits
//
//				while(((Byte*)cell) > current_cell->_next_free_cell) current_cell = (DataType*)current_cell->_next_free_cell;
//
//				// link cell to the cell after it in the free list (current_cell), link that cell to cell
//				cell->_next_free_cell = current_cell->_next_free_cell;
//				current_cell->_next_free_cell = (Byte*)cell;
//			}
//
//			// if this block can now be allocated to, current thread takes responsibility for it
//			// we need to lock this because otherwise another thread may empty an object from this block, making it non-full
//			if( Full() )
//			{
//				DataType::component_manager->Push_Block_With_Free_Cells( this );
//				_memory_managed_by = __thread_id;
//			}
//
//			--_num_allocated;
//
//			// This method may have emptied the block
//			// we need to lock this because otherwise another thread may add an object to this block, making it non-empty
//			if( Empty() )
//			{
//				// Need to note that this thread has an empty block lying around
//				DataType::component_manager->Add_Empty_Block( _memory_managed_by );
//				// Need to remove this block from the execution stream
//				DataType::component_manager->Deactivate_Block( this );
//			}
//		}
//	}

}