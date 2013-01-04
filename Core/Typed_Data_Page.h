#pragma once
#include "Data_Object.h"

///============================================================================
/// Typed_Data_Page
/// Memory page with information about the type it holds
///============================================================================

template<typename DataType=NULLCLASS>
struct Typed_Data_Page
{
	void Initialize()
	{
		first_free_cell=(Data_Object*)((Byte*)this+sizeof(Typed_Data_Page<DataType>));
		//stride=sizeof(DataType);
		//num_cells=(_Page_Size-sizeof(Typed_Data_Page<DataType>))/sizeof(DataType);

		Data_Object* current_cell=first_free_cell;

		const Data_Object* end=(Data_Object*)((Byte*)first_free_cell+num_cells*stride);

		//for(int i=0;i<num_cells;i++)
		while(current_cell!=end)
		{
			current_cell->next_free_cell=(Data_Object*)((Byte*)current_cell+stride);
			current_cell=(Data_Object*)((Byte*)current_cell+stride);
		}
	}

	bool Empty()
	{
		return ((Byte*)first_free_cell)==(((Byte*)this)+sizeof(Typed_Data_Page<DataType>));
	}

	bool Full()
	{
		return ((Byte*)first_free_cell)==(((Byte*)this)+sizeof(Typed_Data_Page<DataType>))+num_cells*stride;
	}

	Data_Object* Allocate()
	{
		Data_Object* return_val=first_free_cell;

		first_free_cell=first_free_cell->next_free_cell;
		
		return return_val;
	}

	void Free(Data_Object* cell)
	{
		Data_Object* current_cell=first_free_cell;

		if(cell<first_free_cell)
		{
			current_cell=first_free_cell;

			first_free_cell=cell;
			first_free_cell->next_free_cell=current_cell;
		}
		else
		{
			while(cell>current_cell->next_free_cell)
			{
				current_cell=current_cell->next_free_cell;
			}

			cell->next_free_cell=current_cell->next_free_cell;
			current_cell->next_free_cell=cell;
		}
	}

	Data_Object* first_free_cell;
	const static int num_cells;
	const static int stride;
};

template<typename DataType>
const int Typed_Data_Page<DataType>::num_cells=(_Page_Size-sizeof(Typed_Data_Page<DataType>))/sizeof(DataType);

template<typename DataType>
const int Typed_Data_Page<DataType>::stride=sizeof(DataType);

///============================================================================
/// Typed_Data_Pages
/// Singleton object by type, holds information about pages currently assigned
///============================================================================

template<typename DataType>
class Typed_Data_Pages
{
public:
	Typed_Data_Pages():stride(sizeof(DataType)),num_cells((_Page_Size-sizeof(Typed_Data_Page<DataType>))/sizeof(DataType))
	{
		mem_lock=0;
	}
	
	DataType* Allocate();

	void Queue_Free(DataType* object){Free(object);}

	void Free(DataType* object);

	Quick_List<Typed_Data_Page<DataType>*> pages_with_free_cells;
	//deque<Typed_Data_Page<DataType>*> pages_with_free_cells;

	//vector<Typed_Data_Page<DataType>*> active_pages;
	//bool type_activated;
	//volatile long tde_lock;
	
	volatile long mem_lock;

	const unsigned int stride;
	
	const unsigned int num_cells;
};