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
		ptex_next_revision._iteration=INT_MAX;
		ptex_next_revision._sub_iteration=0;
		ptex_next_next_revision._iteration=INT_MAX;
		ptex_next_next_revision._sub_iteration=0;
		ptex_lock=0;
		ptex_threads_counter=0;
		num_allocated=0;

		Execution_Object* current_cell=first_free_cell;

		const Execution_Object* end=(Execution_Object*)((Byte*)first_free_cell+num_cells*stride);

		while(current_cell!=end)
		{
			current_cell->next_free_cell=(Execution_Object*)((Byte*)current_cell+stride);
			current_cell->next_revision._iteration=INT_MAX;
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
		
		++num_allocated;

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

		//cell->next_iteration.set_free();
		cell->next_revision._iteration=END;
		cell->next_revision._sub_iteration=0;

		--num_allocated;
	}
	
	bool Empty()
	{
		return num_allocated==0;
		//return ((Byte*)first_free_cell)==(((Byte*)this)+sizeof(Typed_Execution_Page<DataType>));
	}

	bool Full()
	{
		return num_allocated==num_cells;
		//return ((Byte*)first_free_cell)==(((Byte*)this)+sizeof(Typed_Execution_Page<DataType>))+num_cells*stride;
	}

	Revision ptex_current_revision;
	Revision ptex_next_revision;
	Revision ptex_next_next_revision;

	volatile unsigned int ptex_threads_counter;
	volatile unsigned int ptex_lock;

	Execution_Object* first_free_cell;

	static const int stride;
	static const int num_cells;

	unsigned int num_allocated;
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
		tex_next_revision._iteration=INT_MAX;
		tex_next_revision._sub_iteration=0;
		tex_next_next_revision._iteration=INT_MAX;
		tex_next_next_revision._sub_iteration=0;
		tex_threads_counter=0;
		page_stride=1;
	}

	void Process(Revision& tex_response)
	{
		Revision this_revision;
		this_revision._sub_iteration = _sub_iteration;
		this_revision._iteration = _iteration;

		typename Quick_List<Typed_Execution_Page<DataType>*>::List_Cell* itr=active_pages.Begin();

		Byte* current_page;

		const int header_size=sizeof(Typed_Execution_Page<DataType>);
		
		//const unsigned int pstride = page_stride;

		const unsigned int pstride = max( (int)(( active_pages.Size() )/( _Execution_Segments_Per_Thread * _num_threads )), 1 );

		Typed_Execution_Page<DataType>* execution_page;

		//cout << _iteration << "." << _sub_iteration << ": " << typeid(DataType).name() << endl;

		while(itr!=nullptr)
		{
			execution_page = itr->data;
			int process = AtomicIncrement(&execution_page->ptex_threads_counter);

			if(process == 1)
			{
				// allow one thread into this page per _iteration / _sub_iteration

				for(unsigned int i=0;i<pstride;i++)
				{
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
						execution_page->ptex_next_next_revision._iteration=INT_MAX;
						execution_page->ptex_next_next_revision._sub_iteration=0;
					
						execution_page->ptex_lock=0; // unlock the page
					}
				
					if(ptex_response < tex_response)
					{
						tex_response=ptex_response;
					}

					//Take responsibility for pages which only you visit
					if(i != 0 || _num_threads == 1)
					{
						//// inform type that this page is no longer relevant for the execution
						//if(execution_page->Empty())
						//{
						//	typename Quick_List<Typed_Execution_Page<DataType>*>::List_Cell* fitr=active_pages.Begin();

						//	// as long as you will be looping through structure, take the liberty of passing linking information
						//	typename Quick_List<Typed_Execution_Page<DataType>*>::List_Cell* litr=nullptr;

						//	while(fitr!=nullptr)
						//	{
						//		if(fitr->data==execution_page)
						//		{
						//			active_pages.Erase(fitr,litr);
						//			break;
						//		}

						//		litr=fitr;
						//		fitr=fitr->next_allocated_cell;
						//	}
						//}

						execution_page->ptex_threads_counter=0;
					}

					itr=itr->next_allocated_cell;
					if(itr==nullptr) break;

					execution_page = itr->data;
				}
			}
			else
			{
				//Take responsibility for shared page that many threads may visit
				if(process == _num_threads)
				{
					//// inform type that this page is no longer relevant for the execution
					//if(execution_page->Empty())
					//{
					//	typename Quick_List<Typed_Execution_Page<DataType>*>::List_Cell* fitr=active_pages.Begin();

					//	// as long as you will be looping through structure, take the liberty of passing linking information
					//	typename Quick_List<Typed_Execution_Page<DataType>*>::List_Cell* litr=nullptr;

					//	while(fitr!=nullptr)
					//	{
					//		if(fitr->data==execution_page)
					//		{
					//			active_pages.Erase(fitr,litr);
					//			break;
					//		}

					//		litr=fitr;
					//		fitr=fitr->next_allocated_cell;
					//	}
					//}

					execution_page->ptex_threads_counter=0;
				}

				// skip over page_stride pages
				for(unsigned int i=0;i<pstride;i++)
				{
					itr=itr->next_allocated_cell;
					if(itr==nullptr) break;
				}
			}
		}
	}
	
	DataType* Allocate();

	inline void Queue_Free(DataType* object)
	{
		//execution pages only mark memory to be freed
		((Execution_Object*)object)->next_revision=-1;
		//((Execution_Object*)object)->next_iteration.queue_free();
	}
	
	void Free(DataType* object);

	Quick_List<Typed_Execution_Page<DataType>*> active_pages;
	Quick_List<Typed_Execution_Page<DataType>*> pages_with_free_cells;

	_lock tex_lock;

	_lock mem_lock;

	volatile unsigned int tex_threads_counter;

	Revision tex_next_next_revision;
	Revision tex_next_revision;

	bool type_activated;

	const unsigned int stride;
	unsigned int page_stride;

	const unsigned int num_cells;
	
	const Execution_Directive process_directive;
	void (Typed_Execution_Pages::* type_process_directive)(Revision&);
};