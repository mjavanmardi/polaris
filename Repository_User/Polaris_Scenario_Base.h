#pragma once
#include "Repository_User_Includes.h"
#include "Scenario_Interface.h"

namespace Scenario_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{
		struct Polaris_Scenario_Base
		{
			member_data_basic(int, current_simulation_interval_index);
			member_data_basic(int, simulation_interval_length);
			member_data_basic(int, current_time);
			member_data_basic(int, num_simulation_intervals_per_assignment_interval);
		};
	}

	namespace Components
	{
		typedef Polaris_Component<Scenario_Components::Interfaces::Scenario_Interface, Scenario_Components::Bases::Polaris_Scenario_Base> Polaris_Scenario_Component;
	}
}