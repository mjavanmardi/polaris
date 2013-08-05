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
			member_data(bool, ramp_metering_flag, none, none);

			member_data(int, demand_od_flag, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, io_source_flag,  check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, assignment_mode, none, none);
			member_data(unsigned long, iseed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_threads, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(double, demand_reduction_factor, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

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

			member_data(string, routed_path_file_name, none, none);
			member_data(fstream, routed_path_file, none, none);


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
			member_data(fstream, reference_realtime_network_moe_file, none, none);
			member_data(fstream, historic_link_moe_file, none, none);
			member_data(fstream, normal_day_link_moe_file, none, none);
			member_data(fstream, output_network_snapshots_file, none, none);
			member_data(fstream, input_network_snapshots_file, none, none);

			member_data(bool, calculate_realtime_moe, none, none);
			
			member_data(bool, read_demand_from_database, none,none);
			member_data(bool, write_demand_to_database, none,none);
			member_data(bool, write_activity_output, none, none);
			member_data(bool, aggregate_routing, none,none);
			member_data(bool, do_planner_routing, none,none);
			member_data(bool, write_skim_tables, none, none);
			member_data(bool, read_skim_tables, none, none);
			member_data(string, input_skim_file_path_name, none, none);
			member_data(string, output_skim_file_path_name, none, none);

			member_data(double, percent_to_synthesize, none,none);
			member_data(double, ipf_tolerance, none,none);
			member_data(int, marginal_tolerance, none,none);
			member_data(int, maximum_iterations, none,none);
			member_data(bool, write_marginal_output,none,none);
			member_data(bool, write_full_output,none,none);
			member_data(string, popsyn_control_file_name,none,none);

			//member_data(ofstream, output_writer, none, none);

			member_data(_lock, statistics_update_lock, none, none);

			member_data(int, network_cumulative_loaded_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_cumulative_departed_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_in_network_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, network_cumulative_switched_decisions, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, rng_type, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, merging_mode, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, snapshot_period, none, none); // in second

			member_data(bool, write_db_input_to_files, none, none); // to control whether database input is written to files
			member_data(bool, run_simulation_for_db_input, none, none); // to control whether to run simulation for database input

			member_data(bool, write_network_snapshots, none, none);
			member_data(bool, read_network_snapshots, none, none);
			member_data(bool, routing_with_snapshots, none, none);
			member_data(string, input_network_snapshots_file_path_name, none, none);

			member_data(bool, compare_with_historic_moe, none, none);
			member_data(string, historic_network_moe_file_path_name, none, none);
			member_data(string, historic_link_moe_file_path_name, none, none);
			member_data(string, historic_demand_moe_directory, none, none);
			member_data(bool, read_normal_day_link_moe, none, none);
			member_data(string, normal_day_link_moe_file_path_name, none, none);

			member_data(bool, write_node_control_state, none, none);
			member_data(bool, write_vehicle_trajectory, none, none);
			member_data(bool, write_network_link_flow, none, none);
			member_data(bool, write_network_link_turn_time, none, none);
			member_data(bool, write_output_summary, none, none);
			member_data(bool, output_link_moe_for_assignment_interval, none, none);
			member_data(bool, output_turn_movement_moe_for_assignment_interval, none, none);
			member_data(bool, output_network_moe_for_assignment_interval, none, none);
			member_data(bool, output_analzye_link_group_moe_for_assignment_interval, none, none);
			member_data(bool, load_analyze_link_groups_from_file, none, none);
			member_data(string, analyze_link_groups_file_path_name, none, none);
			member_data(fstream, analyze_link_groups_file, none, none);
			
			member_data(bool, DB_output_link_moe_for_assignment_interval, none, none);

			member_data(bool, output_link_moe_for_simulation_interval, none, none);
			member_data(bool, output_turn_movement_moe_for_simulation_interval, none, none);
			member_data(bool, output_network_moe_for_simulation_interval, none, none);

			member_data(bool, use_tmc, none, none);
			member_data(bool, use_network_events, none, none);

			/// enroute switching parameters pretrip_informed_market_share
			member_data(double, pretrip_informed_market_share, none, none);
			member_data(double, realtime_informed_vehicle_market_share, none, none);
			member_data(double, information_compliance_rate_mean, none, none);
			member_data(double, information_compliance_rate_standard_deviation, none, none);
			member_data(double, relative_indifference_band_route_choice_mean, none, none);
			member_data(double, relative_indifference_band_route_choice_standard_deviation, none, none);
			member_data(double, minimum_travel_time_saving_mean, none, none);
			member_data(double, minimum_travel_time_saving_standard_deviation, none, none);
			member_data(bool, enroute_switching_enabled, none, none);
			member_data(bool, use_realtime_travel_time_for_enroute_switching, none, none);
			member_data(double, arrival_delay_ratio_threshold_for_enroute_switching, none, none);

			member_data(bool, multimodal_network_input, none, none);

			

			Polaris_Scenario_Implementation()
			{
				UNLOCK(_statistics_update_lock);
			}

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
			
			feature_implementation void increase_network_cumulative_switched_decisions()
			{
				LOCK(_statistics_update_lock);
				_network_cumulative_switched_decisions++;
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