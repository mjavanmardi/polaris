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
		const int stride=sizeof(DataType);
		const int num_cells=(_Page_Size-sizeof(Typed_Data_Page<DataType>))/sizeof(DataType);

		Data_Object* current_cell=first_free_cell;

		for(int i=0;i<num_cells;i++)
		{
			current_cell->next_free_cell=(Data_Object*)((Byte*)current_cell+stride);
			current_cell=(Data_Object*)((Byte*)current_cell+stride);
		}
	}

	Data_Object* Allocate()
	{
		Data_Object* return_val=first_free_cell;

		first_free_cell=first_free_cell->next_free_cell;
		
		return return_val;
	}

//	void Free(Data_Object<DataType>* cell)
//	{
//		Data_Object<DataType>* current_cell=first_free_cell;
//
//		if(cell<first_free_cell)
//		{
//			current_cell=first_free_cell;
//
//			first_free_cell=cell;
//			first_free_cell->next_free_cell=current_cell;
//		}
//		else
//		{
//			while(cell>current_cell->next_free_cell)
//			{
//				current_cell=current_cell->next_free_cell;
//			}
//
//			cell->next_free_cell=current_cell->next_free_cell;
//			current_cell->next_free_cell=cell;
//		}
//	}

	Data_Object* first_free_cell;
};

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


	//	void Free(Data_Object<DataType>* cell)
	//	{
	//		Data_Object<DataType>* current_cell=first_free_cell;
	//
	//		if(cell<first_free_cell)
	//		{
	//			current_cell=first_free_cell;
	//
	//			first_free_cell=cell;
	//			first_free_cell->next_free_cell=current_cell;
	//		}
	//		else
	//		{
	//			while(cell>current_cell->next_free_cell)
	//			{
	//				current_cell=current_cell->next_free_cell;
	//			}
	//
	//			cell->next_free_cell=current_cell->next_free_cell;
	//			current_cell->next_free_cell=cell;
	//		}
	//	}

	list<Typed_Data_Page<DataType>*> active_pages;
	
	list<Typed_Data_Page<DataType>*> pages_with_free_cells;
	
	//bool type_activated;
	
	//volatile long tde_lock;
	
	volatile long mem_lock;

	const unsigned int stride;
	
	const unsigned int num_cells;
};












///============================================================================
/// Typed_Data_Page
/// Memory page with information about the type it holds
///============================================================================

//template<typename DataType=NULLCLASS>
//struct Typed_Data_Page
//{
//	void Initialize()
//	{
//		first_free_cell=(Data_Object<DataType>*)((Byte*)this+sizeof(Typed_Data_Page<DataType>));
//		const int stride=sizeof(DataType);
//		const int num_cells=(_Page_Size-sizeof(Typed_Data_Page<DataType>))/sizeof(DataType);
//
//		Data_Object<DataType>* current_cell=first_free_cell;
//
//		for(int i=0;i<num_cells;i++)
//		{
//			current_cell->next_free_cell=(Data_Object<DataType>*)((Byte*)current_cell+stride);
//			current_cell=(Data_Object<DataType>*)((Byte*)current_cell+stride);
//		}
//	}
//
//	Data_Object<DataType>* Allocate()
//	{
//		Data_Object<DataType>* return_val=first_free_cell;
//
//		first_free_cell=first_free_cell->next_free_cell;
//		
//		return return_val;
//	}
//
//	void Free(Data_Object<DataType>* cell)
//	{
//		Data_Object<DataType>* current_cell=first_free_cell;
//
//		if(cell<first_free_cell)
//		{
//			current_cell=first_free_cell;
//
//			first_free_cell=cell;
//			first_free_cell->next_free_cell=current_cell;
//		}
//		else
//		{
//			while(cell>current_cell->next_free_cell)
//			{
//				current_cell=current_cell->next_free_cell;
//			}
//
//			cell->next_free_cell=current_cell->next_free_cell;
//			current_cell->next_free_cell=cell;
//		}
//	}
//	
//	Data_Object<DataType>* first_free_cell;
//};

///============================================================================
/// Typed_Data_Pages
/// Singleton object by type, holds information about pages currently assigned
///============================================================================

//template<typename DataType>
//class Typed_Data_Pages
//{
//public:
//	Typed_Data_Pages():stride(sizeof(DataType)),num_cells((_Page_Size-sizeof(Typed_Data_Page<DataType>))/sizeof(DataType)){};
//
//	DataType* Allocate(void* argument)
//	{
//		if(pages_with_free_cells.size()==0)
//		{
//			Typed_Data_Page<DataType>* new_page=(Typed_Data_Page<DataType>*)memory_root_ptr->Allocate();
//
//			new_page->Initialize();
//			
//			pages_with_free_cells.push_back(new_page);
//			
//			active_pages.push_back(new_page);
//		}
//
//		Byte* return_value=(Byte*)(pages_with_free_cells.front()->Allocate());
//
//		new (return_value) DataType(argument);
//
//		if((Byte*)pages_with_free_cells.front()->first_free_cell==((Byte*)pages_with_free_cells.front()+sizeof(Typed_Data_Page<DataType>))+num_cells*stride)
//		{
//			pages_with_free_cells.pop_front();
//		}
//		
//		return (DataType*)return_value;
//	}
//
//	DataType* Allocate()
//	{
//		if(pages_with_free_cells.size()==0)
//		{
//			Typed_Data_Page<DataType>* new_page=(Typed_Data_Page<DataType>*)memory_root_ptr->Allocate();
//
//			new_page->Initialize();
//			
//			pages_with_free_cells.push_back(new_page);
//			
//			active_pages.push_back(new_page);
//		}
//
//		Byte* return_value=(Byte*)(pages_with_free_cells.front()->Allocate());
//
//		new (return_value) DataType();
//
//		if((Byte*)pages_with_free_cells.front()->first_free_cell==((Byte*)pages_with_free_cells.front()+sizeof(Typed_Data_Page<DataType>))+num_cells*stride)
//		{
//			pages_with_free_cells.pop_front();
//		}
//		
//		return (DataType*)return_value;
//	}
//
//	void Free(void* value)
//	{
//		Typed_Data_Page<DataType>* typed_data_page=(Typed_Data_Page<DataType>*)(memory_root_ptr->pages+((Byte*)value-(Byte*)memory_root_ptr->pages)/_Page_Size);
//
//		if(typed_data_page->first_free_cell==(Data_Object<DataType>*)(((Byte*)typed_data_page+sizeof(Typed_Data_Page<DataType>))+num_cells*stride))
//		{
//			pages_with_free_cells.push_back(typed_data_page);
//		}
//
//		typed_data_page->Free((Data_Object<DataType>*)value);
//
//		if(typed_data_page->first_free_cell==(Data_Object<DataType>*)((Byte*)typed_data_page+sizeof(Typed_Data_Page<DataType>)))
//		{
//			list<Typed_Data_Page<DataType>*>::iterator itr;
//
//			for(itr=active_pages.begin();itr!=active_pages.end();itr++)
//			{
//				if((*itr)==typed_data_page)
//				{
//					active_pages.erase(itr);
//					break;
//				}
//			}
//		}
//	}
//
//	list<Typed_Data_Page<DataType>*> active_pages;
//	
//	list<Typed_Data_Page<DataType>*> pages_with_free_cells;
//
//	const unsigned int stride;
//	
//	const unsigned int num_cells;
//};
