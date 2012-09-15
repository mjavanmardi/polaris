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
		template<typename MasterType>
		struct Polaris_Scenario_Base
		{
			member_data_basic(int, current_simulation_interval_index);
			member_data_basic(int, simulation_interval_length);
			member_data_basic(int, assignment_interval_length);
			member_data_basic(int, current_time);

			member_data_basic(int, planning_horizon);
			member_data_basic(int, simulation_start_time);
			member_data_basic(int, simulation_end_time);

			member_data_basic(int, num_simulation_intervals);
			member_data_basic(int, num_assignment_intervals);
			member_data_basic(int, num_simulation_intervals_per_assignment_interval);

			member_data_basic(int, assignment_mode);

			member_data_basic(int, iseed);
		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Scenario_Component
		{
			typedef Polaris_Component<Interfaces::Scenario_Interface, Bases::Polaris_Scenario_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}
}