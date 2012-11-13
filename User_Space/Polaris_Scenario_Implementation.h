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
			
			member_data(int, simulation_interval_length, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, assignment_interval_length, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			feature_implementation TargetType current_simulation_interval_index(){return (TargetType)_iteration/_simulation_interval_length;}
			tag_getter_as_available(current_simulation_interval_index);

			//feature_implementation TargetType current_simulation_interval_index(requires(!check(TargetType,is_arithmetic))) {static_assert(false,"Your TargetType is not an arithmetic type.");}

			feature_implementation TargetType current_time(){return (TargetType)_iteration;}
			tag_getter_as_available(current_time);

			//feature_implementation TargetType current_time(requires(!check(TargetType,is_arithmetic))) {static_assert(false,"Your TargetType is not an arithmetic type.");}

			member_data(int, planning_horizon, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, simulation_start_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, simulation_end_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, num_simulation_intervals, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_assignment_intervals, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_simulation_intervals_per_assignment_interval, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, assignment_mode, none, none);

			member_data(int, iseed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(ofstream, output_writer, none, none);

			member_data(int, network_cumulative_loaded_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_cumulative_departed_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_in_network_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
		};
	}

}