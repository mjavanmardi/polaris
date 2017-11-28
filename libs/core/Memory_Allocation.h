#pragma once
///----------------------------------------------------------------------------------------------------
/// Memory_Allocation.h - Memory_Allocation routines
///----------------------------------------------------------------------------------------------------
#include "m_array.h"
#include "Generic.h"
//#include "windows\gperftools\tcmalloc.h"


//#define ENABLE_MEMORY_LOGGING


namespace polaris
{
	typedef char Byte;
	extern const unsigned int __page_size;
	extern const unsigned int __cache_line_size;
	
	// indicate agent to be freed
	#define FREE __revision_free

#ifdef ENABLE_MEMORY_LOGGING
	extern matrix<long long> _type_counter;
#endif

	///----------------------------------------------------------------------------------------------------
	/// Allocate, Allocate_Array - pass through to the component_manager
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	DataType* Allocate(int pid = -1, bool bInPlaceNew = true)
	{
#ifdef ENABLE_MEMORY_LOGGING
		int i=(int)DataType::component_id;
		int j=thread_id();
		_type_counter(i,j)+=sizeof(DataType);
#endif
		return (DataType*)((DataType::component_manager)->Allocate(pid, bInPlaceNew));
	}

	template<typename DataType>
	DataType* Allocate_Array( unsigned int num )
	{
		return (DataType*)((DataType::component_manager)->Allocate_Array(num));
	}
	
	///----------------------------------------------------------------------------------------------------
	/// Free, Free_Array - pass through to the component_manager
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Free( DataType* ptr )
	{
#ifdef ENABLE_MEMORY_LOGGING
		int i=(int)DataType::component_id;
		int j=thread_id();
		_type_counter(i,j)-=sizeof(DataType);
#endif
		((DataType::component_manager)->Free(ptr));
		ptr = nullptr;
	}

	template<typename DataType>
	void Free_Array( DataType* ptr )
	{
		((DataType::component_manager)->Free_Array(ptr));
		ptr = nullptr;
	}
	
	///----------------------------------------------------------------------------------------------------
	/// Lazy_Free - delayed deconstruction
	///----------------------------------------------------------------------------------------------------

	template<typename DataType>
	void Lazy_Free( DataType* ptr )
	{
		((DataType::component_manager)->Lazy_Free(ptr));
		ptr = nullptr;
	}
}

extern void* operator new ( size_t size );
extern void* operator new[] ( size_t size );

extern void operator delete ( void* ptr );
extern void operator delete[] ( void* ptr );