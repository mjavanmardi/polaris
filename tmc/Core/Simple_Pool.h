#pragma once
#include "Core_Includes.h"

///============================================================================
/// Simple_Pool
/// A fixed size memory pool which simply allocates and frees memory
///============================================================================

template<typename ValType>
class Simple_Pool
{
public:
	union Memory_Cell
	{
		Memory_Cell* next_free_ptr;
		ValType data;
	};

	Simple_Pool()
	{
		first_free_ptr=nullptr;
		pool=nullptr;
		end_pool_itr=nullptr;
		ptr_storage=nullptr;
	}
	
	void Initialize(unsigned int _size)
	{
		size=_size;

		//---- data is held in size pool elements (which are of the same size as Data) ----

		pool=new Memory_Cell[size];
		end_pool_itr=pool+size;

		first_free_ptr=pool;

		ptr_storage=pool;

		furthest_allocated=pool;

		for(;ptr_storage!=end_pool_itr;ptr_storage++)
		{
			ptr_storage->next_free_ptr=ptr_storage+1;
		}
	}

	void Reset()
	{
		first_free_ptr=pool;

		ptr_storage=pool;

		for(;ptr_storage!=(furthest_allocated+1);ptr_storage++)
		{
			ptr_storage->next_free_ptr=ptr_storage+1;
		}

		furthest_allocated=pool;
	}

	ValType* Allocate()
	{
		if(first_free_ptr > furthest_allocated) furthest_allocated=first_free_ptr;

		assert(first_free_ptr!=end_pool_itr);

		ValType* return_val=&first_free_ptr->data;
		
		first_free_ptr=first_free_ptr->next_free_ptr;
		
		return return_val;
	}

	//void Free(Page* page)
	//{
	//	Memory_Page* current_page=first_free_page;

	//	if((Memory_Page*)page<first_free_page)
	//	{
	//		current_page=first_free_page;

	//		first_free_page=(Memory_Page*)page;
	//		first_free_page->next_free_page=current_page;
	//	}
	//	else
	//	{
	//		while((Memory_Page*)page>current_page->next_free_page)
	//		{
	//			current_page=current_page->next_free_page;
	//		}

	//		((Memory_Page*)page)->next_free_page=current_page->next_free_page;

	//		current_page->next_free_page=((Memory_Page*)page);
	//	}
	//}

	Memory_Cell* pool;
	Memory_Cell* first_free_ptr;
	Memory_Cell* end_pool_itr;
	Memory_Cell* furthest_allocated;
	
	Memory_Cell* ptr_storage;

	int size;
};