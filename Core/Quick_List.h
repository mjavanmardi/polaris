#pragma once
#include "Core_Includes.h"

///============================================================================
/// Quick_List
/// A high performance list of non-decreasing size
///============================================================================

template<typename DataType>
class Quick_List
{
public:
	struct List_Cell
	{
		List_Cell* next_allocated_cell;

		union
		{
			List_Cell* next_free_cell;
			DataType data;
		};
	};

	Quick_List(const int size = 4096000 / sizeof(List_Cell) )
	{
		stack_cells=new List_Cell[size];
		num_cells=size;

		first_free_cell = stack_cells;
		first_allocated_cell = nullptr;
		last_allocated_cell = nullptr;
		num_allocated = 0;

		Initialize_Memory(stack_cells,num_cells);
	}

	DataType* Push(DataType& val)
	{
		if( num_allocated == num_cells )
		{
			//full, make more room
			Expand_List();
		}

		if( first_allocated_cell == nullptr )
		{
			// allocating to an empty stack
			first_allocated_cell = first_free_cell;
		}
		else
		{
			// allocating to a non-empty stack
			last_allocated_cell->next_allocated_cell = first_free_cell;
		}

		last_allocated_cell = first_free_cell;
		
		first_free_cell = first_free_cell->next_free_cell;

		//last_allocated_cell->next_free_cell = nullptr; // previous first_free_cell->next_free_cell

		last_allocated_cell->data = val;

		++num_allocated;

		return &last_allocated_cell->data;
	}

	void Pop()
	{
		if( num_allocated == 0 ) return;

		List_Cell* current_cell = first_free_cell;

		if( first_allocated_cell < first_free_cell || first_free_cell == nullptr )
		{
			// link to front
			first_allocated_cell->next_free_cell = first_free_cell;
			first_free_cell = first_allocated_cell;
		}
		else
		{
			// link to middle
			while( current_cell->next_free_cell != nullptr && first_allocated_cell > current_cell->next_free_cell )
			{
				current_cell = current_cell->next_free_cell;
			}

			first_allocated_cell->next_free_cell = current_cell->next_free_cell;
			current_cell->next_free_cell = first_allocated_cell;
		}

		if( first_allocated_cell == last_allocated_cell )
		{
			// no allocated cells
			last_allocated_cell = nullptr;
			first_allocated_cell->next_allocated_cell = nullptr;
			first_allocated_cell = nullptr;
		}
		else
		{
			// at least one allocated cell to link to
			current_cell = first_allocated_cell;
			first_allocated_cell = first_allocated_cell->next_allocated_cell;
			current_cell->next_allocated_cell = nullptr;
		}

		--num_allocated;
	}
	
	size_t Size()
	{
		return num_allocated;
	}
	
	bool Empty()
	{
		return num_allocated == 0;
	}

	DataType& First()
	{
		return first_allocated_cell->data;
	}

	List_Cell* Begin()
	{
		return first_allocated_cell;
	}

	// Not yet implemented
	void Erase(List_Cell*){assert(false);}

private:
	void Initialize_Memory(List_Cell* start_ptr,int numcells)
	{
		List_Cell* init_ptr = start_ptr;
		List_Cell* prev_ptr = init_ptr;

		while( ++init_ptr < ( start_ptr + numcells ) )
		{
			prev_ptr->next_allocated_cell = nullptr;
			prev_ptr->next_free_cell = init_ptr;

			prev_ptr = init_ptr;
		}

		// end of list
		prev_ptr->next_free_cell = nullptr;
		prev_ptr->next_allocated_cell = nullptr;
	}

	void Expand_List()
	{
		unsigned int alloc_cells=0;

		const unsigned int megabyte=1048576;
		const unsigned int num_megabytes=200;

		if(sizeof(List_Cell)*num_cells < megabyte*num_megabytes)
		{
			//if allocation amount under 200 MB, double the container size
			alloc_cells=num_cells;
		}
		else
		{
			//if allocation amount over 200 MB, just add another 200 MB
			alloc_cells=(megabyte*num_megabytes)/sizeof(List_Cell);
		}

		List_Cell* new_cells=new List_Cell[alloc_cells];
		
		num_cells+=alloc_cells;

		first_free_cell = new_cells;

		Initialize_Memory(new_cells,alloc_cells);
	}

	List_Cell* stack_cells;
	
	unsigned int num_cells;
	unsigned int num_allocated;

	List_Cell* first_free_cell;
	List_Cell* first_allocated_cell;
	List_Cell* last_allocated_cell;
};