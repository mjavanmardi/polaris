///----------------------------------------------------------------------------------------------------
/// Simulation_Control.cpp - Global API for Simulation Control
///----------------------------------------------------------------------------------------------------

#include "Simulation_Control.h"

namespace polaris
{
#ifdef _MSC_VER
#else
	const int inc_val = 1;
#endif

	int __component_counter = -1;
	std::unordered_map<size_t, Component_Manager_Base*>* __all_components = nullptr;

	const Revision __revision_omega = Revision(INT_MAX,0);
	const Revision __revision_free = Revision();
}
