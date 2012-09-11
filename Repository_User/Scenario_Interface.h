#pragma once
#include "Repository_User_Includes.h"

namespace Scenario_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Interfaces
	{
		template<typename ThisType,typename CallerType>
		struct Scenario_Interface
		{
			facet_accessor(current_simulation_interval_index);
			facet_accessor(simulation_interval_length);
			facet_accessor(current_time);
			facet_accessor(num_simulation_intervals_per_assignment_interval);
		};
	}
}