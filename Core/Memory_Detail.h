#pragma once
#include "World.h"


template<typename DataType>
DataType* Typed_Execution_Pages<DataType>::Allocate()
{
	if((world_ptr)->run) while(AtomicExchange(&mem_lock,1)) SLEEP(0); // lock the mem

	if( pages_with_free_cells.Empty() )
	{
		//if(!type_activated && !world_ptr->run)
		//{
		//	execution_root_ptr->Activate_Type((Typed_Execution_Pages<>*)this);
		//	type_activated=true;
		//}
		//else if(!type_activated) return nullptr;

		if(!type_activated)
		{
			execution_root_ptr->Activate_Type((Typed_Execution_Pages<>*)this);
			type_activated=true;
		}

		Typed_Execution_Page<DataType>* new_pages=(Typed_Execution_Page<DataType>*)memory_root_ptr->Allocate();

		for(unsigned int i=0;i<DataType::page_factor;i++)
		{
			new_pages->Initialize();
			pages_with_free_cells.Push(new_pages);
			new_pages=(Typed_Execution_Page<DataType>*)(((Byte*)new_pages)+(unsigned int)(_Page_Size/DataType::page_factor));
		}
	}

	if(pages_with_free_cells.First()->Empty())
	{
		active_pages.Push(pages_with_free_cells.First());
	}

	Byte* return_value=(Byte*)(pages_with_free_cells.First()->Allocate());

	if(pages_with_free_cells.First()->Full())
	{
		pages_with_free_cells.Pop();
	}

	new (return_value) DataType();

	if(world_ptr->run) mem_lock=0; // unlock the mem

	return (DataType*)return_value;
}

template<typename DataType>
void Typed_Execution_Pages<DataType>::Free(DataType* _object)
{
	//only called when thread has control over this object's execution domain

	while(AtomicExchange(&mem_lock,1)) SLEEP(0); // lock the page
	
	// find the page which this object resides in
	Byte* object=(Byte*)_object;
	long long dist=(long long)(object-(Byte*)memory_root_ptr->pages);
	object=((dist/(_Page_Size/DataType::page_factor))*(_Page_Size/DataType::page_factor)+(Byte*)memory_root_ptr->pages);
	Typed_Execution_Page<DataType>*exe_page=(Typed_Execution_Page<DataType>*)object;

	// check if need to inform type that this page has space now
	bool notify_type=false;

	if(exe_page->Full()){notify_type=true;}
	
	exe_page->Free(_object);

	// inform type that this page has space
	if(notify_type)
	{
		pages_with_free_cells.Push(exe_page);
	}

	mem_lock=0; // unlock the page
}

template<typename DataType>
DataType* Typed_Data_Pages<DataType>::Allocate()
{
	return new DataType();
}

//template<typename DataType>
//DataType* Typed_Data_Pages<DataType>::Allocate()
//{
//	//Note for greater parallelization: certain states can allow multiple threads through this section of code
//	//	Hand out a certain number of tokens before restricting access, i.e. if 4 empty cells left, allow 3 threads in here in parallel
//
//	while(AtomicExchange(&mem_lock,1)) SLEEP(0); // lock the page
//	
//	if( pages_with_free_cells.Empty() )
//	//if(pages_with_free_cells.size()==0)
//	{
//		
//		Typed_Data_Page<DataType>* new_pages=(Typed_Data_Page<DataType>*)memory_root_ptr->Allocate();
//
//		for(unsigned int i=0;i<DataType::page_factor;i++)
//		{
//			new_pages->Initialize();
//			pages_with_free_cells.Push(new_pages);
//			new_pages=(Typed_Data_Page<DataType>*)(((Byte*)new_pages)+(unsigned int)(_Page_Size/DataType::page_factor));
//		}
//	}
//
//	Byte* return_value=(Byte*)(pages_with_free_cells.First()->Allocate());
//	//Byte* return_value=(Byte*)(pages_with_free_cells.front()->Allocate());
//
//	new (return_value) DataType();
//
//	if(pages_with_free_cells.First()->Full())
//	//if(pages_with_free_cells.front()->Full())
//	{
//		pages_with_free_cells.Pop();
//		//pages_with_free_cells.pop_front();
//	}
//	
//	mem_lock=0; // unlock the page
//
//	return (DataType*)return_value;
//}

template<typename DataType>
void Typed_Data_Pages<DataType>::Free(DataType* _object)
{
	delete _object;
}

//template<typename DataType>
//void Typed_Data_Pages<DataType>::Free(DataType* _object)
//{
//	while(AtomicExchange(&mem_lock,1)) SLEEP(0); // lock the page
//
//	// data pages fully free the object whenever requested
//
//	// find the page which this object resides in
//	Byte* object=(Byte*)_object;
//	long long dist=(long long)(object-(Byte*)memory_root_ptr->pages);
//	object=((dist/(_Page_Size/DataType::page_factor))*(_Page_Size/DataType::page_factor)+(Byte*)memory_root_ptr->pages);
//	Typed_Data_Page<DataType>* data_page=(Typed_Data_Page<DataType>*)object;
//
//	// check if need to inform type that this page has space now
//	bool notify_type=false;
//
//	if(data_page->Full()) notify_type=true;
//	
//	data_page->Free(_object);
//
//	// inform type that this page has space
//	if(notify_type)
//	{
//		pages_with_free_cells.Push(data_page);
//		//pages_with_free_cells.push_back(data_page);
//	}
//
//	mem_lock=0; // unlock the page
//}