#pragma once
///----------------------------------------------------------------------------------------------------
/// Execution_Block_Methods.h - Block of execution objects in a contiguous memory space
///----------------------------------------------------------------------------------------------------

#include "Execution_Block.h"

namespace polaris
{

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
			_first_free_cell = ((Execution_Object*)_first_free_cell)->_next_free_cell;
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
}