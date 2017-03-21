#pragma once
///----------------------------------------------------------------------------------------------------
/// Execution_Block.h - Block of execution objects in a contiguous memory space
///
///----------------------------------------------------------------------------------------------------

#include "Execution_Object.h"

namespace polaris
{
	class Execution_Block : public boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>>
	{
	public:
		Execution_Block(unsigned int cell_size,unsigned int num_cells):_cell_size(cell_size),_num_cells(num_cells){}

		template<typename DataType>
		void Initialize(void* component_manager_ptr);

		//----------------------------------------------------------------------------------------------------
		// Memory-related functions
		//----------------------------------------------------------------------------------------------------

		template<typename DataType>
		DataType* Allocate();
		
		template<typename DataType>
		void Free(DataType* ptr);
		
		template<typename DataType>
		void Lazy_Free(DataType* ptr);

		bool Empty(){ return _num_allocated == 0; }

		bool Full(){ return _num_allocated == _num_cells; }

		void memory_managed_by(int value){ _memory_managed_by = value; }
		int memory_managed_by(){ return _memory_managed_by; }

		//----------------------------------------------------------------------------------------------------
		// Accessor functions
		//----------------------------------------------------------------------------------------------------
		
		const Revision& ptex_next_revision(){ return _ptex_next_revision; }
		void ptex_next_revision(const Revision& value){ _ptex_next_revision = value; }

		void ptex_threads_counter(unsigned int value){ _ptex_threads_counter=value; }

		bool activated(){return _activated;}
		void activated(bool value){_activated = value;}

		unsigned int num_cells(){ return _num_cells; }

		//----------------------------------------------------------------------------------------------------
		// Simulation functions
		//----------------------------------------------------------------------------------------------------
		
		__forceinline unsigned int Reserve_Block(){ return AtomicIncrement(&_ptex_threads_counter); }
		__forceinline void Return_Block(){ _ptex_threads_counter = 0; }

		template<typename DataType>
		inline void Step(Revision& out_next_revision); // Encourage inlining this function, it will be called many times

		inline void Update_Schedule(const Revision& update_revision);

		inline bool Visiting(){ return _ptex_next_revision == revision(); }

		inline volatile unsigned int thread_processing(){return _thread_processing;}

		static const unsigned int _data_offset;

		inline void* component_manager(void){ return _component_manager; }

	private:

		inline void Update(); // Encourage inlining this function, it will be called many times

		Revision _ptex_next_revision;
		Revision _ptex_next_next_revision;

		Byte* _first_free_cell;

		_atomic_counter _ptex_threads_counter;
		_lock _ptex_lock;

		volatile unsigned int _thread_processing;

		_lock _memory_lock;
		const unsigned int _cell_size; // _cell_size >= object_size
		
		const unsigned int _num_cells;

		volatile int _memory_managed_by;

		unsigned int _num_allocated;
		bool _activated;

		void* _component_manager;
	};
}