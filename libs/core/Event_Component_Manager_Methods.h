#pragma once
///----------------------------------------------------------------------------------------------------
/// Event_Component_Manager_Methods.h - Base and type-specific managers for all objects of that type
///----------------------------------------------------------------------------------------------------

#include "World.h"

namespace polaris
{

	///----------------------------------------------------------------------------------------------------
	/// Deactivate_Block - deactivate the provided execution block
	///----------------------------------------------------------------------------------------------------	
	
	template<typename DataType>
	void Event_Component_Manager<DataType>::Deactivate_Block( Event_Block* block )
	{
		LOCK(_tex_lock);

			// Critical check here is whether all threads have passed this type in the execution or not
			
			if( !Visiting() )
			{
				// If no more threads will visit this type, then we can safely remove it from the active blocks
				// if this block has moved to empty, we assume some other "blocks_with_free_cells" will already have a reference to it
				//_active_blocks.erase( _active_blocks.iterator_to(*block) );
					
				// linear search when deque is used as fundamental container
				for(std::deque<Event_Block*>::iterator active_itr = _active_blocks.begin();active_itr!=_active_blocks.end();active_itr++)
				{
					if((*active_itr) == block)
					{
						_active_blocks.erase(active_itr);
						break;
					}
				}

				// Account for the block my informing others that it is not in the _active_blocks list
				block->activated(false);
			}
			else
			{
				// If threads may still be striding this type, we need to be more cautious and move it to a deactivation queue
				_queued_deactivated_blocks.push_back(block);
			}

		UNLOCK(_tex_lock);
	}

	///----------------------------------------------------------------------------------------------------
	/// Allocate - Allocate an execution object of given type in a multi-threaded paradigm
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	DataType* Event_Component_Manager<DataType>::Allocate(int pid, bool bInPlaceNew)
	{
		// Check whether thread has memory of this type available
		if( _blocks_with_free_cells[__thread_id].empty() )
		{
			// Activation occurs once
			//if( !AtomicCompareExchange(&_activated,1,0) ) _world->simulation_engine()->Activate_Type( this );
			if (!Activate()) _world->simulation_engine()->Activate_Type(this);

			// Block size is chosen as the nearest page size (or power of 2 if less than page size) which can accommodate the desired objects per block

			unsigned int suggested_block_size = Event_Block::_data_offset + _ideal_cell_size * _objects_per_block_hint;

			unsigned int block_size;	

			if( suggested_block_size > __page_size / 2 )
			{
				block_size = ((suggested_block_size / __page_size) + 1) * __page_size;
			}
			else if(_objects_per_block_hint == 1)
			{
				// Make an exception here, user clearly has very few of the objects and they must be threaded
				if(suggested_block_size % __cache_line_size != 0) block_size = (suggested_block_size/__cache_line_size + 1)*__cache_line_size;
				else block_size = suggested_block_size;
			}
			else
			{
				// Unroll a loop which finds the nearest power of 2 fractional page size

				if( suggested_block_size <= __page_size / 4 )
				{
					if( suggested_block_size <= __page_size / 8 )
					{
						if( suggested_block_size <= __page_size / 16 )
						{
								block_size = __page_size / 16;
						}
						else block_size = __page_size / 8;
					}
					else block_size = __page_size / 4;
				}
				else block_size = __page_size / 2;		
			}

			unsigned int num_cells = (block_size - Event_Block::_data_offset) / _ideal_cell_size;

			// Global allocation and construction of the execution block memory
			Byte* block_memory = new Byte[block_size];

			// Construction locks in cell_size and num_cells for this particular block
			new (block_memory) Event_Block( _ideal_cell_size, num_cells );

			Event_Block* new_block=(Event_Block*)block_memory;

			// Initialization of the new block
			new_block->Initialize( );

			// Thread takes ownership of the block and pushes it into its' free cells of type
			new_block->memory_managed_by(__thread_id);
			_blocks_with_free_cells[__thread_id].push_back(new_block);

			// Noted as an empty block for accounting purposes
			Add_Empty_Block(__thread_id);
		}

		Event_Block* const free_block = _blocks_with_free_cells[__thread_id].front();

		// If the free block is currently empty, then the following state applies:
		// - The block is not in the _active_blocks list <- Actually we are not guaranteed this! It could be queued for deactivation, but still open for allocating to
		// - _num_empty_blocks has already been incremented for this block

		if( free_block->Empty() )
		{
			if( _world->Is_Running() )
			{
				// We need to decide whether type is pre or post-process to decide whether to queue the blocks or not

				// Locking tex is sufficient to prevent a conflict with active_blocks or an Update step
				// A lock is undesirable, but not too bad because active/queued blocks are intrusive

				LOCK(_tex_lock);

					// If this type is not being visited, simply append to active blocks, otherwise queue it
					if(!Visiting())
					{
						_active_blocks.push_back( free_block );

						// Mark as active to inform the deactivation routines about its' list membership
						free_block->activated(true);
					}
					else
					{
						// We must distinguish the situation whether this is currently queued for de-activation (meaning it is already active) or not
						if(!free_block->activated())
						{
							_queued_activated_blocks.push_back( *free_block );
						}
						else
						{
							// Do the slow (but most conceptually simple) thing and remove it from the de-activation queue
							// This condition almost never gets hit in practice, so it is ok to spend some time here

							for(std::deque<Event_Block*>::iterator itr = _queued_deactivated_blocks.begin();itr!=_queued_deactivated_blocks.end();itr++)
							{
								if((*itr) == free_block)
								{
									_queued_deactivated_blocks.erase(itr);
									break;
								}
							}
						}
					}
					
				UNLOCK(_tex_lock);

				// Indicate that this thread will have one less empty block
				Remove_Empty_Block( __thread_id );
			}
			else
			{
				// Immediately move this into active
				_active_blocks.push_back( free_block );
				
				// Mark as active to inform the deactivation routines about its' list membership
				free_block->activated(true);

				// Indicate that this thread will have one less empty block, save some performance over "Remove_Empty_Block" since threads aren't running
				--_num_empty_blocks[__thread_id];
			}
		}

		// allocate an object from the block itself
		Byte* return_memory = (Byte*)free_block->Allocate<DataType>();

		if (bInPlaceNew)
		{
			new (return_memory) DataType();
			((DataType*)return_memory)->_pid = pid;
		}

		// add information about the pid
		if(pid !=-1) _object_repository[__thread_id][pid] = return_memory;

		return (DataType*)return_memory;
	}
	
	///----------------------------------------------------------------------------------------------------
	/// Allocate_Array
	///----------------------------------------------------------------------------------------------------

//	template<typename DataType>
//	DataType* Event_Component_Manager<DataType>::Allocate_Array( unsigned int num )
//	{
//		static_assert(false,"Array allocation not supported for execution components");
//	}

	///----------------------------------------------------------------------------------------------------
	/// Lazy_Free - Deschedule the execution object, execution engine will deallocate when convenient
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Event_Component_Manager<DataType>::Lazy_Free( DataType* ptr )
	{
		ptr->_execution_block->Lazy_Free(ptr);
	}

	///----------------------------------------------------------------------------------------------------
	/// Free - Free an Event object of given type in a multi-threaded paradigm
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Event_Component_Manager<DataType>::Free( DataType* ptr )
	{
		ptr->execution_block()->Free(ptr);
	}

	///----------------------------------------------------------------------------------------------------
	/// Free_Array
	///----------------------------------------------------------------------------------------------------

	//template<typename DataType>
	//void Event_Component_Manager<DataType>::Free_Array( DataType* ptr )
	//{
	//	static_assert("Array deallocation not supported for execution components");
	//}

	///----------------------------------------------------------------------------------------------------
	/// Clean_Up_Thread_Memory - return thread memory to the global allocator
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Event_Component_Manager<DataType>::Clean_Up_Thread_Memory()
	{
		// This routine may miss blocks which are emptied by other threads while it is running, but it will remain thread safe under these conditions

		const int max_free = max_free_blocks();
		const int num_free_buffer = num_free_blocks_buffer();

		// Decide whether there is clean up to do

		if(_num_empty_blocks[__thread_id] > max_free)
		{
			// This iteration is safe because only this thread can grow this list

			for(std::deque<Event_Block*>::iterator itr = _blocks_with_free_cells[__thread_id].begin(); itr!=_blocks_with_free_cells[__thread_id].end();)
			{
				// This check is safe because only this thread can make these blocks non-empty or (by extension) activated
				// Skip blocks which are "activated", they must be in the deactivation queue (or at least they should be), and must be removed from the intrusive list in the update step

				if((*itr)->Empty() && !(*itr)->activated())
				{
					delete[] ((Byte*)(*itr));

					itr = _blocks_with_free_cells[__thread_id].erase(itr);

					// You are done when you have removed sufficient blocks to have an acceptable buffer size
					if(AtomicDecrement(&_num_empty_blocks[__thread_id]) <= num_free_buffer) break;
				}
				else
				{
					++itr;
				}
			}
		}
	}

	///----------------------------------------------------------------------------------------------------
	/// Step - Advance the simulation one event step, return vote about when next event step should be
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Event_Component_Manager<DataType>::Step(Revision& out_next_revision)
	{
		// Thread logs an overly conservative standing guess for when the next event should happen
		Revision tex_proposed_next_revision = __revision_omega;

		// log the current revision as a const
		const Revision this_revision = revision();

		Event_Block* current_block;

		// Computing blocks here is safe because they will not be added or removed until later
		const unsigned int active_blocks_size = _active_blocks.size();
		const unsigned int suggested_blocks_per_execution_segment = (int)(((float)active_blocks_size)/((float)execution_segments_per_thread() * (float)num_sim_threads()));
		const unsigned int min_blocks = 1;
		const unsigned int blocks_per_execution_segment = max(suggested_blocks_per_execution_segment,min_blocks);
		unsigned int index = 0;

		// loop over the execution blocks which are active this event step
		//boost::intrusive::list<Event_Block>::iterator itr = _active_blocks.begin();
		std::deque<Event_Block*>::iterator itr = _active_blocks.begin();

		while(true)
		{
			current_block = (*itr);
			unsigned int visitor_count = current_block->Reserve_Block();
			
			// if you are the first visitor, you have reserved a number of blocks equal to blocks_per_execution_segment

			if(visitor_count == 1)
			{
				for(unsigned int i=0; i < blocks_per_execution_segment; ++i)
				{
					// Thread logs the PTEX's current guess as an initial guess to report back to the TEX
					Revision ptex_proposed_next_revision = current_block->ptex_next_revision();

					// Visit the PTEX if it has requested to go this step
					// If it is the current iteration, it must be overwritten
					if(ptex_proposed_next_revision == this_revision)
					{
						// process the entire PTEX
						current_block->Step<DataType>(ptex_proposed_next_revision);
					}

					// Here locking is not necessary because local copies are being updated
					if(ptex_proposed_next_revision < tex_proposed_next_revision)
					{
						#ifdef FORWARD_SIMULATION_MODE
							if(ptex_proposed_next_revision <= revision())
							{
								THROW_EXCEPTION("Core Error allowing reschedule by PTEX: " << typeid(DataType).name() << " at step: " << iteration() << "," << sub_iteration() << " to step: " << ptex_proposed_next_revision._iteration << "," << ptex_proposed_next_revision._sub_iteration);
							}
						#endif

						// PTEX has indicated that it wishes to go sooner than proposed, update local copy
						tex_proposed_next_revision = ptex_proposed_next_revision;
					}

					// Reserving thread has responsiblity of making inner blocks available for future event steps
					if(i != 0 || num_sim_threads() == 1)
					{
						current_block->Return_Block();		
					}

					++index;

					// leave if all blocks have been visited
					if(index >= active_blocks_size) break;

					 ++itr;

					// move to next reserved block
					current_block = (*itr);
				}

				if(index >= active_blocks_size) break;
			}
			else
			{
				// Last thread has responsibility of making outer block available for future event steps
				if(visitor_count == num_sim_threads())
				{
					current_block->Return_Block();
				}
				
				index += blocks_per_execution_segment;

				if(index >= active_blocks_size) break;

				//for(unsigned int i=0; i < blocks_per_execution_segment; ++i){++itr;}

				itr = itr + blocks_per_execution_segment;

				//// skip over blocks_per_execution_segment blocks
				//for(unsigned int i=0; i < blocks_per_execution_segment; ++i)
				//{
				//	// leave if all blocks have been visited
				//	if(++itr == _active_blocks.end()) break;
				//}
			}
		}

		Clean_Up_Thread_Memory();

		// Here locking is necessary because global copies may overwrite the true minimum proposed revision
		LOCK(_tex_lock);

			if(tex_proposed_next_revision < _tex_next_next_revision)
			{
				// TEX slice wishes to return sooner in the future than already assumed
				_tex_next_next_revision = tex_proposed_next_revision;
			}

			if(++_tex_threads_counter == num_sim_threads())
			{
				// No more simulation threads will visit this TEX, update for next step
				Update();

				_tex_threads_counter = 0;

				out_next_revision = _tex_next_revision;
			}
			else
			{
				// the tex_next_revision hasn't been set yet to _tex_next_next_revision
				out_next_revision = _tex_next_next_revision;
			}
			

		UNLOCK(_tex_lock);
	}


	///----------------------------------------------------------------------------------------------------
	/// Update - Update to be performed after all threads have finished visiting this component type
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Event_Component_Manager<DataType>::Update()
	{
		if(_queued_deactivated_blocks.size())
		{
			// remove queued deactivated blocks, they are locked against _tex, so you are ok here

			for(std::deque<Event_Block*>::iterator itr = _queued_deactivated_blocks.begin(); itr != _queued_deactivated_blocks.end(); itr++)
			{
				if((*itr)->activated())
				{
					//_active_blocks.erase( _active_blocks.iterator_to( *(*itr) ) );

					// linear search when deque is used as fundamental container
					for(std::deque<Event_Block*>::iterator active_itr = _active_blocks.begin();active_itr!=_active_blocks.end();active_itr++)
					{
						if((*active_itr) == (*itr))
						{
							_active_blocks.erase(active_itr);
							break;
						}
					}

					(*itr)->activated(false);
				}
				else
				{
					_queued_activated_blocks.erase( _queued_activated_blocks.iterator_to( *(*itr) ) );
				}
			}

			_queued_deactivated_blocks.clear();
		}

		if(_queued_activated_blocks.size())
		{
			// activate queued blocks, they are locked against _tex, so you are ok here

			while(_queued_activated_blocks.size())
			{
				Event_Block* current_block = &_queued_activated_blocks.front();

				_queued_activated_blocks.pop_front();
								
				_active_blocks.push_back( current_block );
				current_block->activated(true);
			}
		}

		// you should be guaranteed at this point that all simulation threads are locked out of this data
		
		_tex_next_revision = _tex_next_next_revision;
		_tex_next_next_revision = __revision_omega;
	}
}