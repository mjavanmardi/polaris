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
		const int stride=sizeof(DataType);
		const int num_cells=(Page_Size-sizeof(Typed_Execution_Page<DataType>))/sizeof(DataType);
		
		ptex_current_revision=-1;
		ptex_next_revision.iteration=LONG_MAX;
		ptex_next_revision.sub_iteration=0;
		ptex_next_next_revision.iteration=LONG_MAX;
		ptex_next_next_revision.sub_iteration=0;
		ptex_lock=0;
		ptex_threads_counter=0;

		Execution_Object* current_cell=first_free_cell;

		for(int i=0;i<num_cells;i++)
		{
			current_cell->next_free_cell=(Execution_Object*)((Byte*)current_cell+stride);
			current_cell->next_iteration=INT_MAX;
			current_cell->current_revision=-1;
			current_cell=(Execution_Object*)((Byte*)current_cell+stride);
		}
	}

	Execution_Object* Allocate()
	{
		Execution_Object* return_val=first_free_cell;

		first_free_cell=first_free_cell->next_free_cell;
		
		return return_val;
	}

	Revision ptex_current_revision;
	Revision ptex_next_revision;
	Revision ptex_next_next_revision;

	volatile long ptex_threads_counter;
	volatile long ptex_lock;

	Execution_Object* first_free_cell;
};



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
	Typed_Execution_Pages(Execution_Directive execution_function=(Execution_Directive)Execution_Loop<(Page_Size-sizeof(Typed_Execution_Page<DataType>))/sizeof(DataType),sizeof(DataType)>):stride(sizeof(DataType)),type_activated(false),
	num_cells((Page_Size-sizeof(Typed_Execution_Page<DataType>))/sizeof(DataType)),
	process_directive(execution_function)
	{
		tex_lock=0;
		
		tex_current_revision=-1;
		tex_next_revision.iteration=LONG_MAX;
		tex_next_revision.sub_iteration=0;
		tex_next_next_revision.iteration=LONG_MAX;
		tex_next_next_revision.sub_iteration=0;
		
		//event_register=nullptr;
		//conditional_register=nullptr;

		tex_threads_counter=0;
	}
	
	inline Revision&& type_current_revision()
	{
		return Revision(tex_current_revision);
	}

	inline Revision&& type_next_check()
	{
		return Revision(tex_next_revision);
	}

	//template<typename ComponentType>
	//void Load_Register(Conditional conditional,Event p_event,int start);
	//{
	//	conditional_register=conditional;
	//	event_register=p_event;
	//}

	//__forceinline int next_iteration()
	//{
	//	return tex_next_next_revision.iteration;
	//}
	
	void Process(Revision& tex_response)
	{
		Revision this_revision;
		this_revision.sub_iteration = sub_iteration;
		this_revision.iteration = iteration;

		typename list<Typed_Execution_Page<DataType>*>::iterator itr;

		Byte* current_page;

		const int header_size=sizeof(Typed_Execution_Page<DataType>);
		
		Typed_Execution_Page<>* execution_page;
		
		for(itr=active_pages.begin();itr!=active_pages.end();++itr)
		{
			execution_page = (*itr);
			
			long process = AtomicIncrement(&execution_page->ptex_threads_counter);

			if(process == 1)
			{
				// allow one thread into this page per iteration / sub_iteration				
				
				Revision ptex_response=execution_page->ptex_next_revision;

				if(ptex_response == this_revision)
				{
					ptex_response=execution_page->ptex_next_next_revision;
					
					current_page=((Byte*)execution_page)+header_size;
					
					(*process_directive)(current_page,ptex_response);
					
					// extend shorthand iteration to include sub_iteration
					if(ptex_response.iteration == this_revision.iteration) ptex_response.sub_iteration=this_revision.sub_iteration+1;
					else ptex_response.sub_iteration=0;
					
					while(AtomicExchange(&execution_page->ptex_lock,1)) SLEEP(0); // lock the page
					
					if(ptex_response < execution_page->ptex_next_next_revision)
					{
						// PTEX wishes to return sooner in the future than already assumed
						// should be assumed that these are strictly ordered: execution_page->ptex_next_revision < execution_page->ptex_next_next_revision
						execution_page->ptex_next_next_revision=ptex_response;
					}
					
					execution_page->ptex_current_revision=this_revision;
					execution_page->ptex_next_revision=execution_page->ptex_next_next_revision;
					execution_page->ptex_next_next_revision.iteration=LONG_MAX;
					execution_page->ptex_next_next_revision.sub_iteration=0;
					
					execution_page->ptex_lock=0; // unlock the page
				}
				
				if(ptex_response < tex_response)
				{
					tex_response=ptex_response;
				}
			}
			
			if(process == num_threads)
			{
				execution_page->ptex_threads_counter=0;
			}
		}
	}
	
	DataType* Allocate();
	
	list<Typed_Execution_Page<DataType>*> active_pages;
	
	list<Typed_Execution_Page<DataType>*> pages_with_free_cells;
	
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
};


