#pragma once
#include "Data_Structures.h"
#include "State_Checking.h"

///============================================================================
/// Memory_Root
/// A singleton object responsible for allocating memory pages
///============================================================================

class Memory_Root
{
public:
	union Memory_Page
	{
		Page page;
		Memory_Page* next_free_page;
	};

	Memory_Root()
	{
#ifdef WINDOWS
		pages=(Memory_Page*)VirtualAlloc(nullptr,_Page_Size*_Max_Pages,MEM_COMMIT,PAGE_READWRITE);
#else
		pages=(Memory_Page*)malloc(_Page_Size*_Max_Pages);
#endif
		end_page=pages+_Max_Pages;
		first_free_page=pages;
		
		Memory_Page* current_page=first_free_page;

		for(int i=0;i<_Max_Pages;i++)
		{
			current_page->next_free_page=current_page+1;
			current_page++;
		}
	}
	
	Page* Allocate()
	{
		Page* return_val=&first_free_page->page;
		
		first_free_page=first_free_page->next_free_page;
		
		return return_val;
	}

	void Free(Page* page)
	{
		Memory_Page* current_page=first_free_page;

		if((Memory_Page*)page<first_free_page)
		{
			current_page=first_free_page;

			first_free_page=(Memory_Page*)page;
			first_free_page->next_free_page=current_page;
		}
		else
		{
			while((Memory_Page*)page>current_page->next_free_page)
			{
				current_page=current_page->next_free_page;
			}

			((Memory_Page*)page)->next_free_page=current_page->next_free_page;

			current_page->next_free_page=((Memory_Page*)page);
		}
	}

	Memory_Page* pages;
	Memory_Page* first_free_page;
	Memory_Page* end_page;
};

static Memory_Root* memory_root_ptr;