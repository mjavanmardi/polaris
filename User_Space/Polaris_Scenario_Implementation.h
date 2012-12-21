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
			member_data(int, simulation_start_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, simulation_end_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, intersection_control_flag, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, demand_od_flag, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, io_source_flag,  check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, assignment_mode, none, none);
			member_data(unsigned long, iseed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_threads, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));


			feature_implementation TargetType current_simulation_interval_index()
			{
				int adjusted_iteration = _iteration == 0? 0 : _iteration - (_simulation_interval_length - 1);
				int current_time = adjusted_iteration + _simulation_start_time;
				return (TargetType)(current_time / _simulation_interval_length);
			}
			tag_getter_as_available(current_simulation_interval_index);

			feature_implementation void current_simulation_interval_index(TargetType setValue) {}
			tag_setter_as_available(current_simulation_interval_index);

			//feature_implementation TargetType current_simulation_interval_index(requires(!check(TargetType,is_arithmetic))) {static_assert(false,"Your TargetType is not an arithmetic type.");}

			feature_implementation TargetType current_time()
			{
				int adjusted_iteration = _iteration == 0? 0 : _iteration - (_simulation_interval_length - 1);
				return adjusted_iteration + _simulation_start_time;
			}
			tag_getter_as_available(current_time);

			feature_implementation void current_time(TargetType setValue)
			{
				//assert(false);
				//cerr << "current_time in Scenario should never be set!" << endl;
			}
			tag_setter_as_available(current_time);
			//feature_implementation TargetType current_time(requires(!check(TargetType,is_arithmetic))) {static_assert(false,"Your TargetType is not an arithmetic type.");}

			member_data(int, planning_horizon, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));


			member_data(int, num_simulation_intervals, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_assignment_intervals, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_simulation_intervals_per_assignment_interval, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));



			member_data(int, path_calculation_interval_length, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, current_assignment_interval_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, current_simulation_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, current_day_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));


			member_data(string, input_dir_name, none, none);
			member_data(string, output_dir_name, none, none);
				
			member_data(string, vehicle_trajectory_file_name, none, none);
			member_data(fstream, vehicle_trajectory_file, none, none);

			member_data(string, network_link_flow_file_name, none, none);
			member_data(fstream, network_link_flow_file, none, none);

			member_data(string, network_link_turn_time_file_name, none, none);
			member_data(fstream, network_link_turn_time_file, none, none);

			member_data(string, network_node_control_state_file_name, none, none);
			member_data(fstream, network_node_control_state_file, none, none);

			member_data(string, output_summary_file_name, none, none);
			member_data(fstream, output_summary_file, none, none);

			member_data(string, vehicle_transfer_file_name, none, none);
			member_data(fstream, vehicle_transfer_file, none, none);

			member_data(ofstream, output_writer, none, none);



			member_data(int, network_cumulative_loaded_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_cumulative_departed_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_in_network_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(double, assignment_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(double, simulation_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(double, operation_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(double, output_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(double, condition_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
		};
	}

}