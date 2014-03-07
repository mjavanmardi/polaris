#pragma once
///----------------------------------------------------------------------------------------------------
/// Execution_Component_Manager.cpp - Base and type-specific managers for all objects of that type
///----------------------------------------------------------------------------------------------------

#include "World.h"

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Initialize - initialize the component manager
	///----------------------------------------------------------------------------------------------------	

	void Execution_Component_Manager_Base::Initialize()
	{
		_activated = false;

		_blocks_with_free_cells = new boost::container::deque<Execution_Block*>[num_sim_threads() + 1];
		_num_empty_blocks = new _atomic_counter[num_sim_threads() + 1]();

		_object_repository = new boost::unordered_map<int,void*>[num_sim_threads() + 1];

		_tex_next_revision = __revision_omega;
		_tex_next_next_revision = __revision_omega;

		_tex_threads_counter = 0;

		_objects_per_block_hint = execution_objects_per_block();

		UNLOCK(_tex_lock);
	}
	
	///----------------------------------------------------------------------------------------------------
	/// Terminate - terminate the component manager
	///----------------------------------------------------------------------------------------------------	

	void Execution_Component_Manager_Base::Terminate()
	{
		while(_active_blocks.size())
		{
			Execution_Block* block = _active_blocks.front();

			_active_blocks.pop_front();

			delete[] ((Byte*)block);
		}
		
		_active_blocks.clear();

		while(_queued_activated_blocks.size())
		{
			Execution_Block* block = &_queued_activated_blocks.front();

			_queued_activated_blocks.pop_front();

			delete[] ((Byte*)block);
		}

		_queued_activated_blocks.clear();

		_queued_deactivated_blocks.clear();

		delete[] _blocks_with_free_cells;
		delete[] _num_empty_blocks;
	}

	///----------------------------------------------------------------------------------------------------
	/// Deactivate_Block - deactivate the provided execution block
	///----------------------------------------------------------------------------------------------------	

	void Execution_Component_Manager_Base::Deactivate_Block( Execution_Block* block )
	{
		LOCK(_tex_lock);

			// Critical check here is whether all threads have passed this type in the execution or not
			
			if( !Visiting() )
			{
				// If no more threads will visit this type, then we can safely remove it from the active blocks
				// if this block has moved to empty, we assume some other "blocks_with_free_cells" will already have a reference to it
				//_active_blocks.erase( _active_blocks.iterator_to(*block) );
					
				// linear search when deque is used as fundamental container
				for(boost::container::deque<Execution_Block*>::iterator active_itr = _active_blocks.begin();active_itr!=_active_blocks.end();active_itr++)
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
	/// Update - Update to be performed after all threads have finished visiting this component type
	///----------------------------------------------------------------------------------------------------

	void Execution_Component_Manager_Base::Update()
	{
		if(_queued_deactivated_blocks.size())
		{
			// remove queued deactivated blocks, they are locked against _tex, so you are ok here

			for(boost::container::deque<Execution_Block*>::iterator itr = _queued_deactivated_blocks.begin(); itr != _queued_deactivated_blocks.end(); itr++)
			{
				if((*itr)->activated())
				{
					//_active_blocks.erase( _active_blocks.iterator_to( *(*itr) ) );

					// linear search when deque is used as fundamental container
					for(boost::container::deque<Execution_Block*>::iterator active_itr = _active_blocks.begin();active_itr!=_active_blocks.end();active_itr++)
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
				Execution_Block* current_block = &_queued_activated_blocks.front();

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