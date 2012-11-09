#pragma once
#include "User_Space_Includes.h"
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
			//member_data(int, current_simulation_interval_index);
			
			member_data(int, simulation_interval_length, none, none);
			member_data(int, assignment_interval_length, none, none);

			feature_implementation TargetType current_simulation_interval_index(){return (TargetType)_iteration/_simulation_interval_length;}
			tag_getter_as_available(current_simulation_interval_index);

			//feature_implementation TargetType current_simulation_interval_index(requires(!check(TargetType,is_arithmetic))) {static_assert(false,"Your TargetType is not an arithmetic type.");}

			feature_implementation TargetType current_time(){return (TargetType)_iteration;}
			tag_getter_as_available(current_time);

			//feature_implementation TargetType current_time(requires(!check(TargetType,is_arithmetic))) {static_assert(false,"Your TargetType is not an arithmetic type.");}

			member_data(int, planning_horizon, none, none);
			member_data(int, simulation_start_time, none, none);
			member_data(int, simulation_end_time, none, none);

			member_data(int, num_simulation_intervals, none, none);
			member_data(int, num_assignment_intervals, none, none);
			member_data(int, num_simulation_intervals_per_assignment_interval, none, none);

			member_data(int, assignment_mode, none, none);

			member_data(int, iseed, none, none);

			member_data(ofstream, output_writer, none, none);

			member_data(int, network_cumulative_loaded_vehicles, none, none);
			member_data(int, network_cumulative_departed_vehicles, none, none);
			member_data(int, network_in_network_vehicles, none, none);
			member_data(int, network_cumulative_arrived_vehicles, none, none);
		};
	}

	namespace Components
	{
	}
}