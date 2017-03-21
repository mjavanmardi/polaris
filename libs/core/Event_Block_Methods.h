#pragma once
///----------------------------------------------------------------------------------------------------
/// Event_Block_Methods.h - Block of execution objects in a contiguous memory space - where execution is sparse
///----------------------------------------------------------------------------------------------------

#include "Event_Block.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Update_Schedule - Thread safe update of the block's internal schedule
	///----------------------------------------------------------------------------------------------------
	
	void Event_Block::Update_Object_Schedule(Event_Object* updated_object)
	{
		//boost::intrusive::multiset<Event_Object,boost::intrusive::member_hook<Event_Object, boost::intrusive::set_member_hook<>, &Event_Object::_events_hook>>::iterator itr;

		// ptex lock makes it schedule safe
		LOCK( _ptex_lock );

			//itr = _event_schedule.iterator_to(*updated_object);

			//if(itr->_events_hook.is_linked()){ _event_schedule.erase(itr); }

			_event_schedule.insert( updated_object );

		UNLOCK( _ptex_lock );
	}


	///----------------------------------------------------------------------------------------------------
	/// Update_Schedule - Thread safe update of the block's internal schedule
	///----------------------------------------------------------------------------------------------------

	void Event_Block::Update_Schedule(const Revision& update_revision)
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
	DataType* Event_Block::Allocate()
	{
		// memory_lock shuts down all allocation and free operations for this block
		LOCK(_memory_lock);

			// allocate from the pool
			DataType* return_val = (DataType*)_first_free_cell;
			_first_free_cell = ((Event_Object*)_first_free_cell)->_next_free_cell;
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
	void Event_Block::Lazy_Free(DataType* cell)
	{
		// Safe mode locks the execution object for increased thread safety
		#ifdef SAFE_MODE
			LOCK(ptr->_optex_lock);
		#elif defined ENABLE_WARNINGS
			if( cell->Visiting() && _thread_processing!=__thread_id )
			{
				THROW_WARNING("Ureliable free of: " << typeid(DataType).name() << " at " << ptr << " Use of the self-free option, or rescheduling when it will be freed is recommended for this object");
			}
		#endif

		cell->_next_revision = FREE;

		#ifdef SAFE_MODE
			UNLOCK(ptr->_optex_lock);
		#endif
	}
	
	///----------------------------------------------------------------------------------------------------
	/// Update - Update the execution block to the state it should be in when visited next
	///----------------------------------------------------------------------------------------------------
	
	void Event_Block::Update()
	{
		// you should be guaranteed at this point that all simulation threads are locked out of this data
		_ptex_next_revision = _ptex_next_next_revision;
		_ptex_next_next_revision = __revision_omega;
	}

	///----------------------------------------------------------------------------------------------------
	/// Step - Advance the simulation one event step, return vote about when next event step should be
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Event_Block::Step(Revision& out_next_revision)
	{
		_thread_processing = __thread_id;

		// Thread logs an overly conservative standing guess for when the next event should happen
		Revision ptex_proposed_next_revision = __revision_omega;

		// log the current revision as a const
		const Revision this_revision = revision();
		
		Revision cached_next_revision;

		// prepare to iterate over contiguous memory
		Byte* object_itr = (((Byte*)this)+_data_offset); // get iterator, point to first object
		const Byte* const object_end = object_itr + _num_cells*_cell_size;

		//_mm_prefetch( object_itr + _cell_size , _MM_HINT_T0 ); // Fetch 1 step ahead

		Event_Response optex_conditional;
		optex_conditional.next = __revision_omega;

		//for(boost::intrusive::multiset<Event_Object,boost::intrusive::member_hook<Event_Object, boost::intrusive::set_member_hook<>, &Event_Object::_events_hook>>::iterator itr = _event_schedule.begin();itr!=_event_schedule.end();itr=_event_schedule.begin())
		//while(!_event_schedule.isEmpty())
		while(true)
		{
			//DataType* const current_object = (DataType* const) &(*itr);
			
			//DataType* const current_object = (DataType* const) _event_schedule.ExtractMin();
			
			DataType* const current_object = (DataType* const) _event_schedule.heap_front;

			cached_next_revision = current_object->_next_revision;

			// Safe mode locks the execution object for increased thread safety
			#ifdef SAFE_MODE
				LOCK(current_object->_optex_lock);
			#endif

			// Visit the OPTEX if it has requested to go this step
			// If it is the current iteration, it must be overwritten
			if( current_object->_next_revision == this_revision )
			{
				//_event_schedule.DeleteMin();
				_event_schedule.extract_min();

				// process the OPTEX
				current_object->_event_callback(current_object,optex_conditional);
				current_object->_next_revision = optex_conditional.next;
				
				optex_conditional.next = __revision_omega;

				#ifdef FORWARD_SIMULATION_MODE
					if(current_object->_next_revision <= revision() && current_object->_next_revision != FREE)
					{
						THROW_EXCEPTION("Simulation specified as Forward Simulation, error rescheduling object: " << typeid(DataType).name() << " at step: " << iteration() << "," << sub_iteration() << " to step: " << current_object->_next_revision._iteration << "," << current_object->_next_revision._sub_iteration);
					}
				#endif

				//Update_Object_Schedule(current_object);
				
				_event_schedule.insert(current_object);
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

			if(cached_next_revision._revision > this_revision._revision) break;

			#ifdef SAFE_MODE
				UNLOCK(current_object->_optex_lock);
			#endif

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