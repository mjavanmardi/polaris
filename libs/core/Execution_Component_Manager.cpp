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

		_object_repository = new std::unordered_map<int,void*>[num_sim_threads() + 1];

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
		//while(_active_blocks.size())
		//{
		//	Execution_Block* block = _active_blocks.front();

		//	_active_blocks.pop_front();

		//	delete[] ((Byte*)block);
		//}
		//
		//_active_blocks.clear();

		//while(_queued_activated_blocks.size())
		//{
		//	Execution_Block* block = &_queued_activated_blocks.front();

		//	_queued_activated_blocks.pop_front();

		//	delete[] ((Byte*)block);
		//}

		//_queued_activated_blocks.clear();

		//_queued_deactivated_blocks.clear();

		//delete[] _blocks_with_free_cells;
		//delete[] _num_empty_blocks;
	}

}