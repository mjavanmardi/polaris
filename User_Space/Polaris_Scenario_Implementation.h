#pragma once

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
		implementation struct Polaris_Scenario_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Scenario_Implementation),MasterType,Data_Object,ParentType>
		{
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

			member_data(int, planning_horizon, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_simulation_intervals, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_assignment_intervals, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_simulation_intervals_per_assignment_interval, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, path_calculation_interval_length, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

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

			member_data(fstream, out_realtime_network_moe_file, none, none);
			member_data(fstream, out_realtime_link_moe_file, none, none);
			member_data(fstream, out_realtime_movement_moe_file, none, none);

			member_data(fstream, out_network_moe_file, none, none);
			member_data(fstream, out_link_moe_file, none, none);
			member_data(fstream, out_movement_moe_file, none, none);

			//member_pointer(FILE*, out_network_moe_file, none, none);
			//member_pointer(FILE*, out_link_moe_file, none, none);
			//member_pointer(FILE*, out_movement_moe_file, none, none);

			member_data(ofstream, output_writer, none, none);

			member_data(_lock, statistics_update_lock, none, none);

			member_data(int, network_cumulative_loaded_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_cumulative_departed_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_in_network_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, snapshot_period, none, none); // in second

			feature_implementation void increase_network_cumulative_loaded_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_cumulative_loaded_vehicles++;
				UNLOCK(_statistics_update_lock);
			}

			feature_implementation void increase_network_cumulative_departed_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_cumulative_departed_vehicles++;
				UNLOCK(_statistics_update_lock);			
			}

			feature_implementation void increase_network_in_network_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_in_network_vehicles++;
				UNLOCK(_statistics_update_lock);			
			}

			feature_implementation void increase_network_cumulative_arrived_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_cumulative_arrived_vehicles++;
				UNLOCK(_statistics_update_lock);			
			}
			
			feature_implementation void decrease_network_in_network_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_in_network_vehicles--;
				UNLOCK(_statistics_update_lock);			
			}

			member_data(double, assignment_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(double, simulation_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(double, operation_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(double, output_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(double, condition_time_in_seconds, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(std::string, database_name, none, none);
		};
	}

}