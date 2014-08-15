#pragma once
///----------------------------------------------------------------------------------------------------
/// Event_Block.h - Block of execution objects in a contiguous memory space
///
/// SIMILAR TO EXECUTION BLOCK
/// Except uses intrusive binary heap to store execution schedule information (ordered by next revision)
/// Instead of striding over all objects to find ones which need to execute - uses the intrusive binary heap to get the ones which need to execute
/// Downside comes in building the IBH - has memory overhead (48 bytes per object) and performance overhead due to the insertion of objects in the heap ordering by execution time
/// Useful when execution by object is sparse - saves the striding over all objects
///----------------------------------------------------------------------------------------------------

#include "Event_Object.h"

namespace polaris
{
	class Event_Block : public boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::normal_link>>
	{
	public:
		Event_Block(unsigned int cell_size,unsigned int num_cells):_cell_size(cell_size),_num_cells(num_cells){}

		void Initialize();

		//----------------------------------------------------------------------------------------------------
		// Memory-related functions
		//----------------------------------------------------------------------------------------------------

		template<typename DataType>
		inline DataType* Allocate();
		
		template<typename DataType>
		inline void Free(DataType* ptr);
		
		template<typename DataType>
		inline void Lazy_Free(DataType* ptr);

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
		
		inline void Update_Object_Schedule(Event_Object* updated_object);

		inline bool Visiting(){ return _ptex_next_revision == revision(); }

		inline volatile unsigned int thread_processing(){return _thread_processing;}

		static const unsigned int _data_offset;

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

		//boost::intrusive::multiset<Event_Object,boost::intrusive::member_hook<Event_Object, boost::intrusive::set_member_hook<>, &Event_Object::_events_hook>> _event_schedule;
		//BinaryHeap<Event_Object> _event_schedule;
		Intrusive_Binary_Heap<Event_Object> _event_schedule;
	};
}