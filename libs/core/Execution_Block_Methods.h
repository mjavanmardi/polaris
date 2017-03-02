#pragma once
///----------------------------------------------------------------------------------------------------
/// Execution_Block_Methods.h - Block of execution objects in a contiguous memory space
///----------------------------------------------------------------------------------------------------

#include "Execution_Block.h"

namespace polaris
{
	static bool _world_is_running();

	///----------------------------------------------------------------------------------------------------
	/// Update_Schedule - Thread safe update of the block's internal schedule
	///----------------------------------------------------------------------------------------------------

	void Execution_Block::Update_Schedule(const Revision& update_revision)
	{
		// Following makes PTEX aware, can catch PTEX in 2 meaningful states: A) Will Visit B) Won't Visit

		// ptex lock makes it schedule safe
		LOCK( _ptex_lock );

			if( !Visiting() )
			{
				// Work on the post-update data

				if(update_revision < _ptex_next_revision)
				{
					_ptex_next_revision = update_revision;
				}
			}
			else
			{
				// Work on the pre-update data

				if(update_revision < _ptex_next_next_revision)
				{
					_ptex_next_next_revision = update_revision;
				}
			}

		UNLOCK( _ptex_lock );
	}

	///----------------------------------------------------------------------------------------------------
	/// Allocate - Allocate a DataType object from the block's memory pool, this is a thread-safe function
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	DataType* Execution_Block::Allocate()
	{
		// memory_lock shuts down all allocation and free operations for this block
		LOCK(_memory_lock);

			// allocate from the pool
			DataType* return_val = (DataType*)_first_free_cell;
			_first_free_cell = ((DataType*)_first_free_cell)->_next_free_cell;
			++_num_allocated;

			// This operation may fill the block, if so it must be removed from consideration for future allocation by this thread
			// we need to lock this because otherwise another thread may empty an object from this block, making it non-full
			if( Full() )
			{
				// This method makes an assumption that this block it is at the front of this thread's free list
				DataType::component_manager->Pop_Front_Block_With_Free_Cells();
				_memory_managed_by = -1;
			}

		UNLOCK(_memory_lock);

		return return_val;
	}

	///----------------------------------------------------------------------------------------------------
	/// Free - Deallocate an object from the block's memory pool, this is a thread safe function
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Execution_Block::Free(DataType* cell)
	{
		// Safe mode locks the execution object for increased thread safety
#ifdef SAFE_MODE
		LOCK(cell->_optex_lock);
#elif defined ENABLE_WARNINGS
		if (cell->Visiting() && _thread_processing != __thread_id)
		{
			THROW_WARNING("Ureliable free of: " << typeid(DataType).name() << " at " << cell << " Use of the self-free option, or rescheduling when it will be freed is recommended for this object");
		}
#endif

		// deschedule the cell immediately to minimize collisions
		cell->_next_revision = __revision_omega;

		// destruct the cell
		cell->DataType::~DataType();

#ifdef SAFE_MODE
		UNLOCK(cell->_optex_lock);
#endif

		//if (_world->Is_Running())
		if ( _world_is_running() )
		{
			// _memory_lock ensures that nothing will be allocated (or freed) from the memory pool
			LOCK(_memory_lock);

			// Start from the "middle", provided cell may be before or after the first free cell
			DataType* current_cell = (DataType*)_first_free_cell;

			if (Full())
			{
				// block is fully allocated - so set first free cell to this one

				_first_free_cell = (Byte*)cell;
				((DataType*)_first_free_cell)->_next_free_cell = nullptr;
			}
			else if (((Byte*)cell) < _first_free_cell)
			{
				// cell exists before the first free cell, it becomes ffc and previous ffc links to this cell

				_first_free_cell = (Byte*)cell;
				((DataType*)_first_free_cell)->_next_free_cell = (Byte*)current_cell;
			}
			else
			{
				// cell exists after the first free cell, forward scan the list to find where it fits

				while (current_cell->_next_free_cell && ((Byte*)cell) > current_cell->_next_free_cell) current_cell = (DataType*)current_cell->_next_free_cell;

				// link cell to the cell after it in the free list (current_cell), link that cell to cell
				cell->_next_free_cell = current_cell->_next_free_cell;
				current_cell->_next_free_cell = (Byte*)cell;
			}

			// if this block can now be allocated to, current thread takes responsibility for it
			// we need to lock this because otherwise another thread may empty an object from this block, making it non-full
			if (Full())
			{
				DataType::component_manager->Push_Block_With_Free_Cells(this);
				_memory_managed_by = __thread_id;
			}

			--_num_allocated;

			// This method may have emptied the block
			// we need to lock this because otherwise another thread may add an object to this block, making it non-empty
			// note however that the block cannot be newly with free cells or else the Full() condition above would have caught it
			if (Empty())
			{
				// Need to note that the "managed_by" thread has an empty block lying around
				DataType::component_manager->Add_Empty_Block(_memory_managed_by);
				// Need to remove this block from the execution stream
				DataType::component_manager->Deactivate_Block(this);

				LOCK(_ptex_lock);

				// deschedule the block
				_ptex_next_revision = __revision_omega;
				_ptex_next_next_revision = __revision_omega;

				UNLOCK(_ptex_lock);
			}

			UNLOCK(_memory_lock);
		}
		else
		{
			// Start from the "middle", provided cell may be before or after the first free cell
			DataType* current_cell = (DataType*)_first_free_cell;

			if (((Byte*)cell) < _first_free_cell)
			{
				// cell exists before the first free cell, it becomes ffc and previous ffc links to this cell

				_first_free_cell = (Byte*)cell;
				((DataType*)_first_free_cell)->_next_free_cell = (Byte*)current_cell;
			}
			else
			{
				// cell exists after the first free cell, forward scan the list to find where it fits

				while (((Byte*)cell) > current_cell->_next_free_cell) current_cell = (DataType*)current_cell->_next_free_cell;

				// link cell to the cell after it in the free list (current_cell), link that cell to cell
				cell->_next_free_cell = current_cell->_next_free_cell;
				current_cell->_next_free_cell = (Byte*)cell;
			}

			// if this block can now be allocated to, current thread takes responsibility for it
			// we need to lock this because otherwise another thread may empty an object from this block, making it non-full
			if (Full())
			{
				DataType::component_manager->Push_Block_With_Free_Cells(this);
				_memory_managed_by = __thread_id;
			}

			--_num_allocated;

			// This method may have emptied the block
			// we need to lock this because otherwise another thread may add an object to this block, making it non-empty
			if (Empty())
			{
				// Need to note that this thread has an empty block lying around
				DataType::component_manager->Add_Empty_Block(_memory_managed_by);
				// Need to remove this block from the execution stream
				DataType::component_manager->Deactivate_Block(this);
			}
		}
	}

	///----------------------------------------------------------------------------------------------------
	/// Lazy_Free - Deschedule an execution object, it will be freed later, this is a thread safe function
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Execution_Block::Lazy_Free(DataType* cell)
	{
		// Safe mode locks the execution object for increased thread safety
		#ifdef SAFE_MODE
			LOCK(cell->_optex_lock);
		#elif defined ENABLE_WARNINGS
			if( cell->Visiting() && _thread_processing!=__thread_id )
			{
				THROW_WARNING("Ureliable free of: " << typeid(DataType).name() << " at " << ptr << " Use of the self-free option, or rescheduling when it will be freed is recommended for this object");
			}
		#endif

		cell->_next_revision = FREE;

		#ifdef SAFE_MODE
			UNLOCK(cell->_optex_lock);
		#endif
	}


	///----------------------------------------------------------------------------------------------------
	/// Update - Update the execution block to the state it should be in when visited next
	///----------------------------------------------------------------------------------------------------
	
	void Execution_Block::Update()
	{
		// you should be guaranteed at this point that all simulation threads are locked out of this data
		_ptex_next_revision = _ptex_next_next_revision;
		_ptex_next_next_revision = __revision_omega;
	}

	///----------------------------------------------------------------------------------------------------
	/// Step - Advance the simulation one event step, return vote about when next event step should be
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Execution_Block::Step(Revision& out_next_revision)
	{
		_thread_processing = __thread_id;

		// Thread logs an overly conservative standing guess for when the next event should happen
		Revision ptex_proposed_next_revision = __revision_omega;

		// log the current revision as a const
		const Revision this_revision = revision();
		
		// prepare to iterate over contiguous memory
		Byte* object_itr = (((Byte*)this)+_data_offset);
		const Byte* const object_end = object_itr + _num_cells*_cell_size;

		//_mm_prefetch( object_itr + _cell_size , _MM_HINT_T0 ); // Fetch 1 step ahead

		Event_Response optex_conditional;
		optex_conditional.next = __revision_omega;

		while( object_itr != object_end )
		{
			DataType* const current_object = (DataType* const)object_itr;
			
			// Safe mode locks the execution object for increased thread safety
			#ifdef SAFE_MODE
				LOCK(current_object->_optex_lock);
			#endif

			// Visit the OPTEX if it has requested to go this step (Object-Page-Type-Execution Type list)
			// If it is the current iteration, it must be overwritten
			if( current_object->_next_revision == this_revision )
			{
				// process the OPTEX
				current_object->_event_callback(current_object,optex_conditional);	// Call user-defined event function
				current_object->_next_revision = optex_conditional.next;			// update this objects next revision from the event response
				
				optex_conditional.next = __revision_omega;

				#ifdef FORWARD_SIMULATION_MODE
					if(current_object->_next_revision <= revision() && current_object->_next_revision != FREE)
					{
						THROW_EXCEPTION("Simulation specified as Forward Simulation, error rescheduling object: " << typeid(DataType).name() << " at step: " << iteration() << "," << sub_iteration() << " to step: " << current_object->_next_revision._iteration << "," << current_object->_next_revision._sub_iteration);
					}
				#endif
			}

			// Here locking is not necessary because local copies are being updated
			if( current_object->_next_revision < ptex_proposed_next_revision )
			{
				if( current_object->_next_revision != FREE )
				{
					#ifdef FORWARD_SIMULATION_MODE
						if(current_object->_next_revision <= revision() /* we already know it isn't "FREE" */)
						{
							THROW_EXCEPTION("Core Error allowing reschedule by OPTEX: " << typeid(DataType).name() << " at step: " << iteration() << "," << sub_iteration() << " to step: " << current_object->_next_revision._iteration << "," << current_object->_next_revision._sub_iteration);
						}
					#endif

					// OPTEX has indicated that it wishes to go sooner than proposed, update local copy
					ptex_proposed_next_revision = current_object->_next_revision;
				}
				else
				{
					// Perform an internal free operation
					Free( (DataType*)current_object );
				}
			}

			#ifdef SAFE_MODE
				UNLOCK(current_object->_optex_lock);
			#endif

			object_itr += _cell_size;

			//_mm_prefetch( object_itr + _cell_size , _MM_HINT_T0 ); // Fetch 1 step ahead
		}

		LOCK(_ptex_lock);

			if(ptex_proposed_next_revision < _ptex_next_next_revision)
			{
				// PTEX wishes to return sooner in the future than already assumed
				_ptex_next_next_revision = ptex_proposed_next_revision;
			}

			Update();

			out_next_revision = _ptex_next_revision;

		UNLOCK(_ptex_lock);

		_thread_processing = -1;
	}

	///----------------------------------------------------------------------------------------------------
	/// Initialize - Initialize the execution block, set execution characteristics to an unscheduled state
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
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

		while (current_cell != end_cell)
		{
			((DataType*)current_cell)->_execution_block = this;
			((DataType*)current_cell)->_next_free_cell = current_cell + _cell_size;
			((DataType*)current_cell)->_next_revision = __revision_omega;

#ifdef SAFE_MODE
			UNLOCK(((Execution_Object*)current_cell)->_optex_lock);
#endif
			Byte* next_cell = current_cell + _cell_size;
			if (next_cell == end_cell)
				((DataType*)current_cell)->_next_free_cell = (Byte*)0;

			current_cell = next_cell;
		}

		//Test - let's iterate through the cells
		Byte* pCell = _first_free_cell;
		int num = 0;
		while (pCell != (Byte*)0)
		{
			DataType* pObj = (DataType*)pCell;
			//printf("%d : At cell %p - next=%p\n", num, pCell, pObj->_next_free_cell);
			pCell = pObj->_next_free_cell;
			if (num == _num_cells)
				assert(pCell == (Byte*)0);

			++num;
		}

	}

}