#pragma once
#include "Core_Includes.h"

///============================================================================
/// Quick_Stack
/// A high performance stack of non-decreasing size
///============================================================================

template<typename DataType>
class Quick_Stack
{
public:
	Quick_Stack(int size = 4096 / sizeof(Stack_Cell) )
	{
		stack_cells=new Stack_Cell[size];
		num_cells=size;

		first_free_cell = stack_cells;
		first_allocated_cell = nullptr;
		last_allocated_cell = nullptr;
		//last_cell = stack_cells+(num_cells-1);
		//stack_cells->id=0;

		Initialize_Memory(stack_cells,num_cells);
	}

	DataType* Push(DataType& val)
	{
		if( first_free_cell == nullptr )
		{
			//full, make more room
			Expand_Stack();
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

		last_allocated_cell->next_free_cell = nullptr; // previous first_free_cell->next_free_cell

		last_allocated_cell->data = val;

		return &last_allocated_cell->data;
	}

	void Pop()
	{
		if( first_allocated_cell == nullptr ) return;

		Stack_Cell* current_cell = first_free_cell;

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
	}
	
	size_t Size()
	{
		return num_cells;
	}
	
	bool Empty()
	{
		return first_allocated_cell == nullptr;
	}

	DataType& First()
	{
		return first_allocated_cell->data;
	}

	//void Print_Stack_Contents()
	//{
	//	cout << endl << "first_free_cell: ";
	//	
	//	if(first_free_cell==nullptr)
	//	{
	//		cout << "-1" << endl;
	//	}
	//	else
	//	{
	//		cout << first_free_cell->id << endl;
	//	}

	//	cout << "first_allocated_cell: ";
	//	
	//	if(first_allocated_cell==nullptr)
	//	{
	//		cout << "-1" << endl;
	//	}
	//	else
	//	{
	//		cout << first_allocated_cell->id << endl;
	//	}

	//	cout << "last_allocated_cell: ";
	//	
	//	if(last_allocated_cell==nullptr)
	//	{
	//		cout << "-1" << endl;
	//	}
	//	else
	//	{
	//		cout << last_allocated_cell->id << endl;
	//	}

	//	cout << "last_cell: ";
	//	
	//	if(last_cell==nullptr)
	//	{
	//		cout << "-1" << endl;
	//	}
	//	else
	//	{
	//		cout << last_cell->id << endl;
	//	}

	//	cout << endl;
	//	
	//	Stack_Cell* current_cell=stack_cells;

	//	for(int i=0;i<num_cells;i++)
	//	{
	//		cout << "Cell " << i << ":" << endl;

	//		cout << "\tnext_cell: ";
	//		
	//		if(current_cell->next_cell==nullptr)
	//		{
	//			cout << "-1" << endl;
	//		}
	//		else
	//		{
	//			cout << current_cell->next_cell->id << endl;
	//		}

	//		cout << "\tnext_free_cell: ";
	//		
	//		if(current_cell->next_free_cell==nullptr)
	//		{
	//			cout << "-1" << endl;
	//		}
	//		else
	//		{
	//			cout << current_cell->next_free_cell->id << endl;
	//		}

	//		cout << "\tnext_allocated_cell: ";
	//		
	//		if(current_cell->next_allocated_cell==nullptr)
	//		{
	//			cout << "-1" << endl;
	//		}
	//		else
	//		{
	//			cout << current_cell->next_allocated_cell->id << endl;
	//		}

	//		current_cell=current_cell->next_cell;
	//	}
	//}

private:
	struct Stack_Cell
	{
		Stack_Cell* next_free_cell;
		Stack_Cell* next_allocated_cell;

		//Stack_Cell* next_cell;
		//int id;

		DataType data;
	};

	void Initialize_Memory(Stack_Cell* start_ptr,int numcells)
	{
		Stack_Cell* init_ptr = start_ptr;
		Stack_Cell* prev_ptr = init_ptr;
		//int counter = start_ptr->id;

		while( ++init_ptr < ( start_ptr+numcells ) )
		{
			prev_ptr->next_allocated_cell = nullptr;
			prev_ptr->next_free_cell = init_ptr;
			//prev_ptr->next_cell = init_ptr;
			//prev_ptr->id = counter;
			//counter++;

			prev_ptr = init_ptr;
		}

		// end of list
		prev_ptr->next_free_cell = nullptr;
		prev_ptr->next_allocated_cell = nullptr;
		
		//prev_ptr->next_cell = nullptr;
		//prev_ptr->id = counter;
	}

	void Expand_Stack()
	{
		Stack_Cell* new_cells=new Stack_Cell[num_cells];

		num_cells*=2;

		first_free_cell = new_cells;

		//last_cell->next_cell = new_cells;

		//last_cell = new_cells+(num_cells/2-1);
		
		//new_cells->id = num_cells/2;

		Initialize_Memory(new_cells,num_cells/2);
	}

	Stack_Cell* stack_cells;
	
	int num_cells;

	Stack_Cell* first_free_cell;
	Stack_Cell* first_allocated_cell;
	Stack_Cell* last_allocated_cell;

	//Stack_Cell* last_cell;
};