#pragma once
///----------------------------------------------------------------------------------------------------
/// Execution_Component_Manager.h - Base and type-specific managers for all objects of that type
///----------------------------------------------------------------------------------------------------

#include "Execution_Block_Methods.h"
#if (_MSC_VER >= 1900 || !_MSC_VER )
  #include <atomic>
#endif

namespace polaris
{
	///----------------------------------------------------------------------------------------------------
	/// Execution_Component_Manager_Base - Base manager defines members and provides general interface
	///----------------------------------------------------------------------------------------------------

	class Execution_Component_Manager_Base : public Component_Manager_Base
	{
	public:
		Execution_Component_Manager_Base() : _activated(false) 
		{
#if (_MSC_VER >= 1900 || !_MSC_VER )
			_activated_set.clear(); 
#endif
		}
		virtual void Initialize();
		virtual Component_Manager_Types Component_Manager_Type(){ return EXECUTION_MANAGER; }
		virtual void Terminate();

		//----------------------------------------------------------------------------------------------------
		// Simulation functions
		//----------------------------------------------------------------------------------------------------

		virtual void Step(Revision& out_next_revision){}

		const Revision& tex_next_revision(){ return _tex_next_revision; }
		void tex_next_revision(const Revision& value){ _tex_next_revision = value; }	
		
		inline bool Visiting(){ return _tex_next_revision == revision(); }
		
		
		inline void Update_Schedule(const Revision& update_revision);

#if (_MSC_VER >= 1900 || !_MSC_VER ) // %%%RLW - need to check this for Visual Studio 12 & 13
		inline bool Activate() { do {} while (std::atomic_flag_test_and_set(&_activated_set)); bool was_activated = _activated; if (!_activated) _activated = true; std::atomic_flag_clear(&_activated_set); return was_activated; }
#elif (_MSC_VER)
		inline bool Activate() { return AtomicCompareExchange(&_activated,1,0); }
#endif


		//----------------------------------------------------------------------------------------------------
		// Accessor functions
		//----------------------------------------------------------------------------------------------------

		unsigned int objects_per_block_hint(){ return _objects_per_block_hint; }
		void objects_per_block_hint(unsigned int value){ _objects_per_block_hint = value; }

		const char* name(){ return _name; }

	protected:
		Revision _tex_next_revision;
		Revision _tex_next_next_revision;
		
		_atomic_counter _tex_threads_counter;
		_lock _tex_lock;
		
		unsigned int _objects_per_block_hint;
		unsigned int _ideal_cell_size;
		
#if (_MSC_VER >= 1900 || !_MSC_VER )
		std::atomic_bool _activated;
		std::atomic_flag _activated_set;
#else
		_atomic_counter _activated;
#endif

		const char* _name;
	};

	///----------------------------------------------------------------------------------------------------
	/// Execution_Component_Manager - Implements type-specific allocation and event stepping behavior
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	class Execution_Component_Manager : public Execution_Component_Manager_Base
	{
	public:
		void Update();

		//----------------------------------------------------------------------------------------------------
		// Memory-related functions
		//----------------------------------------------------------------------------------------------------

		void Deactivate_Block( Execution_Block* block );

		void Add_Empty_Block( int thread_id ){ AtomicIncrement(&_num_empty_blocks[thread_id]); }
		void Remove_Empty_Block( int thread_id ){ AtomicDecrement(&_num_empty_blocks[thread_id]); }

		void Push_Block_With_Free_Cells(Execution_Block* block){ _blocks_with_free_cells[__thread_id].push_back(block); }
		void Pop_Front_Block_With_Free_Cells(){ _blocks_with_free_cells[__thread_id].pop_front(); }

		typedef DataType managed_type;

		Execution_Component_Manager(){_name = typeid(DataType).name();}

		virtual void Initialize()
		{
			Execution_Component_Manager_Base::Initialize();

			_blocks_with_free_cells = new std::deque<Execution_Block*>[num_sim_threads() + 1];
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

		//boost::intrusive::list<Execution_Block> _active_blocks;
		std::deque<Execution_Block*> _active_blocks;

		boost::intrusive::list<Execution_Block> _queued_activated_blocks;
		std::deque<Execution_Block*> _queued_deactivated_blocks;
		std::deque<Execution_Block*>* _blocks_with_free_cells;

		_atomic_counter* _num_empty_blocks;
	};
}
