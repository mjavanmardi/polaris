///----------------------------------------------------------------------------------------------------
/// Globals.cpp - Global Constructs not related to simulation
///----------------------------------------------------------------------------------------------------

#include "Globals.h"

namespace polaris
{
#ifdef _MSC_VER
	__declspec(thread) int __thread_id;
#else
	__thread int __thread_id;
#endif

}
