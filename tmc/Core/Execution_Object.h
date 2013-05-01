#pragma once
#include "Memory_Engine.h"

///============================================================================
/// Forward declarations
///============================================================================

template<typename DataType=NULLCLASS>
class Typed_Execution_Pages;

class Execution_Root;

static Execution_Root* execution_root_ptr;

template<typename DataType=NULLCLASS>
struct Typed_Execution_Page;

///============================================================================
/// Execution_Object
/// An object chunk with associated type, which is available for allocation
/// Contains 2 registers for event handling:
/// One activated every step, another activated should the first return true
///============================================================================


struct Execution_Object
{
	Execution_Object()
	{
		next_revision._iteration=INT_MAX;
		next_revision._sub_iteration=0;
	}

	permit_state_checking;


	__forceinline void Swap_Event(Event new_event)
	{
		event_register=new_event;
	}
	
	template<typename ComponentType>
	void Load_Event(Conditional conditional,Event p_event,int start_iteration,int start_sub_iteration);
	
	template<typename DataType=NULLTYPE>
	struct Conditional_Holder
	{
		static Conditional conditional_register;
	};

	//Conditional conditional_register;

	///============================================================================
	/// packed_iteration - strips out 4th bit for internal purposes
	///============================================================================

	//struct packed_iteration
	//{
	//	inline int operator=(int value)
	//	{
	//		_value=value;
	//		return _value&0x7fffffff;
	//	}

	//	inline bool operator==(int value)
	//	{
	//		return (_value&0x7fffffff)==value;
	//	}

	//	inline bool operator<(int value)
	//	{
	//		return (_value&0x7fffffff)<value;
	//	}

	//	inline bool operator<=(int value)
	//	{
	//		return (_value&0x7fffffff)<=value;
	//	}

	//	inline bool operator>=(int value)
	//	{
	//		return (_value&0x7fffffff)>=value;
	//	}

	//	inline bool operator>(int value)
	//	{
	//		return (_value&0x7fffffff)>value;
	//	}

	//	inline operator int()
	//	{
	//		return (_value&0x7fffffff);
	//	}

	//	inline void queue_free()
	//	{
	//		_value=0x80000000;
	//	}
	//	
	//	inline void set_free()
	//	{
	//		_value=0x7fffffff;
	//	}

	//	int _value;
	//} next_iteration;

	//union Packed_Revision
	//{
	//	Packed_Revision():_revision(0){};
	//	Packed_Revision(int sub_revision,int revision):_sub_iteration(sub_revision){_iteration=revision;};
	//	Packed_Revision(Packed_Revision& copy):_revision(copy._revision){};

	//	inline void operator = (const long long val){_revision=val;}
	//	inline operator long long&(){return _revision;}
	//#ifdef WINDOWS
	//	struct
	//	{
	//		int _sub_iteration;
	//		packed_iteration _iteration;
	//	};
	//#else
	//	struct
	//	{
	//		packed_iteration _iteration;
	//		int _sub_iteration;
	//	};
	//#endif
	//	long long _revision;
	//} next_iteration;

	Revision next_revision;

	//int current_revision;
	
	union
	{
		Event event_register;

		Execution_Object* next_free_cell;
	};
};

template<typename DataType>
Conditional Execution_Object::Conditional_Holder<DataType>::conditional_register=&False_Condition;

typedef void (*Execution_Directive)(void*,Revision&);

///============================================================================
/// Tight loop function to process a given execution page
///============================================================================

template<typename DataType>
void Execution_Loop(void* page_in, Revision& ptex_response)
{
	Bytes<sizeof(DataType)>* page=(Bytes<sizeof(DataType)>*)page_in;
	
	const Bytes<sizeof(DataType)>* const __restrict end_page=page+(_Page_Size/DataType::page_factor-sizeof(Typed_Execution_Page<DataType>))/sizeof(DataType);
	
	const Revision this_revision=_revision;
	
	Conditional_Response optex_response;
	
	Revision object_revision;

	do
	{
		//not necessary if developer doesn't compare response initial values
		//response.next=INT_MAX;
		//response.result=false;

		Execution_Object* const current_page = ((Execution_Object*)page);

		object_revision._revision = current_page->next_revision._revision;

		if( object_revision._revision == this_revision._revision )
		{
			//((Execution_Object*)page)->conditional_register(page,optex_response);

			Execution_Object::Conditional_Holder<DataType>::conditional_register(current_page,optex_response);
			
			object_revision._revision = optex_response.next._revision;

			current_page->next_revision._revision = object_revision._revision;

			if(optex_response.result) current_page->event_register(current_page);
		}

		if(object_revision._revision < ptex_response._revision)
		{
			if(object_revision._revision >= 0)
			{
				ptex_response._revision=object_revision._revision;
			}
			else
			{
				(DataType::singleton_reference)->Free((DataType*)current_page);
			}
		}

	}
	while(++page < end_page);
}