#pragma once
#include "Repository_User_Includes.h"
#include "Scenario_Prototype.h"

namespace Scenario_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Scenario_Implementation
		{
			//member_data_basic(int, current_simulation_interval_index);
			
			member_data_basic(int, simulation_interval_length);
			member_data_basic(int, assignment_interval_length);

			feature_implementation TargetType current_simulation_interval_index(){return iteration/_simulation_interval_length;}
			tag_getter(current_simulation_interval_index);

			feature_implementation TargetType current_time(){return iteration;}
			tag_getter(current_time);

			member_data_basic(int, planning_horizon);
			member_data_basic(int, simulation_start_time);
			member_data_basic(int, simulation_end_time);

			member_data_basic(int, num_simulation_intervals);
			member_data_basic(int, num_assignment_intervals);
			member_data_basic(int, num_simulation_intervals_per_assignment_interval);

			member_data_basic(int, assignment_mode);

			member_data_basic(int, iseed);

			member_data_basic(ofstream, output_writer);

			member_data_basic(int, network_cumulative_loaded_vehicles);
			member_data_basic(int, network_cumulative_departed_vehicles);
			member_data_basic(int, network_in_network_vehicles);
			member_data_basic(int, network_cumulative_arrived_vehicles);
		};
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Scenario_Component{ typedef Polaris_Component<Implementations::Polaris_Scenario_Implementation, MasterType> type; };
	}
}