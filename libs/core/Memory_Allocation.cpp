///----------------------------------------------------------------------------------------------------
/// Execution_Component_Manager.h - Base and type-specific managers for all objects of that type
///----------------------------------------------------------------------------------------------------

#include "Memory_Allocation.h"

namespace polaris
{
	const unsigned int __page_size = 4096;
	const unsigned int __cache_line_size = 64;

#ifdef ENABLE_MEMORY_LOGGING
	matrix<long long> _type_counter;
#endif
}

//void* operator new ( size_t size ) { return tc_malloc( size ); }
//void* operator new[] ( size_t size ) { return tc_malloc( size ); }

//void operator delete ( void* ptr ) { tc_free( ptr ); }
//void operator delete[] ( void* ptr ) { tc_free( ptr ); }