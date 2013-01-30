#pragma once
#include "Execution_Object.h"

///============================================================================
/// Typed_Execution_Page
/// Memory page with information about the type it holds
/// Holds additional information about the execution status of the page
///============================================================================

template<typename DataType>
struct Typed_Execution_Page
{
	void Initialize()
	{
		first_free_cell=(Execution_Object*)((Byte*)this+sizeof(Typed_Execution_Page<DataType>));

		ptex_current_revision=-1;
		ptex_next_revision._iteration=LONG_MAX;
		ptex_next_revision._sub_iteration=0;
		ptex_next_next_revision._iteration=LONG_MAX;
		ptex_next_next_revision._sub_iteration=0;
		ptex_lock=0;
		ptex_threads_counter=0;

		Execution_Object* current_cell=first_free_cell;

		const Execution_Object* end=(Execution_Object*)((Byte*)first_free_cell+num_cells*stride);

		while(current_cell!=end)
		{
			current_cell->next_free_cell=(Execution_Object*)((Byte*)current_cell+stride);
			current_cell->next_revision._iteration=LONG_MAX;
			current_cell->next_revision._sub_iteration=0;
			//current_cell->event_register=nullptr;//shared with first free list pointer, cannot initialize
			//current_cell->conditional_register=&False_Condition;
			current_cell=(Execution_Object*)((Byte*)current_cell+stride);
		}
	}

	Execution_Object* Allocate()
	{
		Execution_Object* return_val=first_free_cell;

		first_free_cell=first_free_cell->next_free_cell;
		
		return return_val;
	}
	
	void Free(Execution_Object* cell)
	{
		Execution_Object* current_cell=first_free_cell;

		if(cell<first_free_cell)
		{
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

		//TODO
		//cell->next_iteration.set_free();
	}
	
	bool Empty()
	{
		return ((Byte*)first_free_cell)==(((Byte*)this)+sizeof(Typed_Execution_Page<DataType>));
	}

	bool Full()
	{
		return ((Byte*)first_free_cell)==(((Byte*)this)+sizeof(Typed_Execution_Page<DataType>))+num_cells*stride;
	}

	Revision ptex_current_revision;
	Revision ptex_next_revision;
	Revision ptex_next_next_revision;

	volatile long ptex_threads_counter;
	volatile long ptex_lock;

	Execution_Object* first_free_cell;

	static const int stride;
	static const int num_cells;
};

template<typename DataType>
const int Typed_Execution_Page<DataType>::num_cells=(_Page_Size/DataType::page_factor-sizeof(Typed_Execution_Page<DataType>))/sizeof(DataType);

template<typename DataType>
const int Typed_Execution_Page<DataType>::stride=sizeof(DataType);

///============================================================================
/// Typed_Execution_Pages
/// Singleton object by type, holds information about pages currently assigned
/// Additionally holds information about the execution status of the type
/// Performs execution for the associate type
///============================================================================

template<typename DataType>
class Typed_Execution_Pages
{
public:
	Typed_Execution_Pages(Execution_Directive execution_function=(Execution_Directive)Execution_Loop<DataType>):stride(sizeof(DataType)),type_activated(false),
	num_cells((_Page_Size/DataType::page_factor-sizeof(Typed_Execution_Page<DataType>))/sizeof(DataType)),
	process_directive(execution_function)
	{
		tex_lock=0;
		mem_lock=0;
		type_process_directive=&Typed_Execution_Pages::Process;
		tex_current_revision=-1;
		tex_next_revision._iteration=LONG_MAX;
		tex_next_revision._sub_iteration=0;
		tex_next_next_revision._iteration=LONG_MAX;
		tex_next_next_revision._sub_iteration=0;
		tex_threads_counter=0;
	}
	
	//inline long type_current_revision()
	//{
	//	return tex_current_revision._iteration;
	//}

	//inline long type_next_check()
	//{
	//	return tex_next_revision._iteration;
	//}

	void Process(Revision& tex_response)
	{
		Revision this_revision;
		this_revision._sub_iteration = _sub_iteration;
		this_revision._iteration = _iteration;

		Quick_List<Typed_Execution_Page<DataType>*>::List_Cell* itr=active_pages.Begin();

		Byte* current_page;

		const int header_size=sizeof(Typed_Execution_Page<DataType>);
		
		Typed_Execution_Page<DataType>* execution_page;

		while(itr!=nullptr)
		{
			execution_page = itr->data;
			long process = AtomicIncrement(&execution_page->ptex_threads_counter);

			if(process == 1)
			{
				// allow one thread into this page per _iteration / _sub_iteration				
				
				Revision ptex_response=execution_page->ptex_next_revision;

				if(ptex_response == this_revision)
				{
					ptex_response=execution_page->ptex_next_next_revision;
					
					current_page=((Byte*)execution_page)+header_size;

					(*process_directive)(current_page,ptex_response);

					// extend shorthand _iteration to include _sub_iteration
					//if(ptex_response._iteration == this_revision._iteration) ptex_response._sub_iteration=this_revision._sub_iteration+1;
					//else ptex_response._sub_iteration=0;
					
					while(AtomicExchange(&execution_page->ptex_lock,1)) SLEEP(0); // lock the page
					
					if(ptex_response < execution_page->ptex_next_next_revision)
					{
						// PTEX wishes to return sooner in the future than already assumed
						// should be assumed that these are strictly ordered: execution_page->ptex_next_revision < execution_page->ptex_next_next_revision
						execution_page->ptex_next_next_revision=ptex_response;
					}
					
					execution_page->ptex_current_revision=this_revision;
					execution_page->ptex_next_revision=execution_page->ptex_next_next_revision;
					execution_page->ptex_next_next_revision._iteration=LONG_MAX;
					execution_page->ptex_next_next_revision._sub_iteration=0;
					
					execution_page->ptex_lock=0; // unlock the page
				}
				
				if(ptex_response < tex_response)
				{
					tex_response=ptex_response;
				}
			}
			
			if(process == _num_threads)
			{
				// inform type that this page is no longer relevant for the execution
				if(execution_page->Empty())
				{
					Quick_List<Typed_Execution_Page<DataType>*>::List_Cell* fitr=active_pages.Begin();
					while(fitr!=nullptr)
					{
						if(fitr->data==execution_page)
						{
							active_pages.Erase(fitr);
							break;
						}
						fitr=fitr->next_allocated_cell;
					}
				}

				execution_page->ptex_threads_counter=0;
			}
			itr=itr->next_allocated_cell;
		}
	}
	
	DataType* Allocate();

	void Queue_Free(DataType* object)
	{
		//execution pages only mark memory to be freed
		((Execution_Object*)object)->next_iteration.queue_free();
	}
	
	void Free(DataType* object);

	Quick_List<Typed_Execution_Page<DataType>*> active_pages;
	Quick_List<Typed_Execution_Page<DataType>*> pages_with_free_cells;

	volatile long tex_lock;

	volatile long mem_lock;

	volatile long tex_threads_counter;

	Revision tex_next_next_revision;
	Revision tex_next_revision;
	Revision tex_current_revision;

	bool type_activated;

	const unsigned int stride;
	
	const unsigned int num_cells;
	
	const Execution_Directive process_directive;
	void (Typed_Execution_Pages::* type_process_directive)(Revision&);
};