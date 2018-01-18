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
		implementation struct Scenario_Implementation:public Polaris_Component<MasterType,INHERIT(Scenario_Implementation),Data_Object>
		{
			m_data(string, output_results_database_name, NONE, NONE);
			m_data(string, output_demand_database_name, NONE, NONE);
			m_data(string, output_popsyn_database_name, NONE, NONE);
			m_data(string, input_popsyn_database_name, NONE, NONE);
			m_data(string, historical_results_database_name, NONE, NONE);

			m_data(int, simulation_interval_length, NONE, NONE);
			m_data(int, assignment_interval_length, NONE, NONE);
			m_data(int, simulation_start_time, NONE, NONE);
			m_data(int, simulation_end_time, NONE, NONE);
			m_data(int, intersection_control_flag, NONE, NONE);
			m_data(bool, ramp_metering_flag, NONE, NONE);

			m_data(unsigned long, iseed, NONE, NONE);


			m_data(double, demand_reduction_factor, NONE, NONE);
			m_data(bool, count_integrated_in_network_vehicles_only, NONE, NONE);

			m_data(int, planning_horizon, NONE, NONE);
			m_data(int, num_simulation_intervals, NONE, NONE);
			m_data(int, num_assignment_intervals, NONE, NONE);
			m_data(int, num_simulation_intervals_per_assignment_interval, NONE, NONE);

			m_data(int, path_calculation_interval_length, NONE, NONE);

			m_data(int, current_day_index, NONE, NONE);

			//=================================================================================
			// CAV SIMULATION VARIABLES
			m_data(double, capacity_adjustment_highway,NONE,NONE);
			m_data(double, capacity_adjustment_arterial,NONE,NONE);

			m_data(bool, simulate_cacc,NONE,NONE);  // ***SD*** change from double based on read_scenario_data
			m_data(double, cacc_capacity_adjustment_alpha, NONE, NONE);
			m_data(double, cacc_capacity_adjustment_beta, NONE, NONE);
			m_data(double, automation_cost, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_past_crashes, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_smartphone, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_carshare, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_rideshare, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_drive_alone_work, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_drive_alone_other, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_ln_vmt, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_work_dist, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_gender, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_license, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_num_children, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_age, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_empl_density, NONE, NONE);
			m_data(double, vehicle_techchoice_beta_hhincome, NONE, NONE);
			m_data(double, vehicle_techchoice_calibration, NONE, NONE);
			m_data(double, vehicle_techchoice_mu_1, NONE, NONE);
			m_data(double, vehicle_techchoice_mu_2, NONE, NONE);
			m_data(double, vehicle_techchoice_mu_3, NONE, NONE);

			m_data(double, cav_market_penetration, NONE, NONE);
			m_data(double, cav_vott_adjustment, NONE, NONE);

			//=================================================================================
			// FILENAMES

			m_data(string, input_dir_name, NONE, NONE);
			m_data(string, output_dir_name, NONE, NONE);
				
			m_data(string, vehicle_trajectory_file_name, NONE, NONE);
			m_data(fstream, vehicle_trajectory_file, NONE, NONE);
			m_data(double, vehicle_trajectory_sample_rate, NONE, NONE);
			m_data(string, vehicle_tracking_list_file_name, NONE, NONE);
			m_container(std::unordered_set<int>, vehicle_tracking_list, NONE, NONE);
			m_data(bool, use_vehicle_tracking_list, NONE, NONE);

			m_data(string, routed_path_file_name, NONE, NONE);
			m_data(fstream, routed_path_file, NONE, NONE);


			m_data(string, network_link_flow_file_name, NONE, NONE);
			m_data(fstream, network_link_flow_file, NONE, NONE);

			m_data(string, network_link_turn_time_file_name, NONE, NONE);
			m_data(fstream, network_link_turn_time_file, NONE, NONE);

			m_data(string, network_node_control_state_file_name, NONE, NONE);
			m_data(fstream, network_node_control_state_file, NONE, NONE);

			m_data(string, output_summary_file_name, NONE, NONE);
			m_data(fstream, output_summary_file, NONE, NONE);

			m_data(string, vehicle_transfer_file_name, NONE, NONE);
			m_data(fstream, vehicle_transfer_file, NONE, NONE);

			m_data(fstream, out_realtime_network_moe_file, NONE, NONE);
			m_data(fstream, out_realtime_link_moe_file, NONE, NONE);
			m_data(fstream, out_realtime_movement_moe_file, NONE, NONE);

			m_data(fstream, out_network_moe_file, NONE, NONE);
			m_data(fstream, out_link_moe_file, NONE, NONE);
			m_data(fstream, out_movement_moe_file, NONE, NONE);
			m_data(fstream, reference_realtime_network_moe_file, NONE, NONE);
			m_data(fstream, historic_link_moe_file, NONE, NONE);
			m_data(fstream, normal_day_link_moe_file, NONE, NONE);
			m_data(fstream, output_network_snapshots_file, NONE, NONE);
			m_data(fstream, input_network_snapshots_file, NONE, NONE);

			m_data(bool, write_visualizer_snapshot, NONE, NONE);

			m_data(bool, calculate_realtime_moe, NONE, NONE);
			
			m_data(double, flexible_work_percentage, NONE, NONE);

			m_data(bool, read_population_from_database, NONE, NONE);
			m_data(bool, read_demand_from_database, NONE, NONE);
			m_data(bool, write_demand_to_database, NONE, NONE);
			m_data(bool, write_activity_output, NONE, NONE);
			m_data(bool, aggregate_routing, NONE, NONE);
			m_data(bool, do_planner_routing, NONE, NONE);
			m_data(bool, write_skim_tables, NONE, NONE);
			m_data(bool, read_skim_tables, NONE, NONE);
			m_data(string, input_highway_skim_file_path_name, NONE, NONE);
			m_data(string, output_highway_skim_file_path_name, NONE, NONE);
			m_data(string, input_highway_cost_skim_file_path_name, NONE, NONE);
			m_data(string, output_highway_cost_skim_file_path_name, NONE, NONE);
			m_data(string, input_transit_skim_file_path_name, NONE, NONE);
			m_data(string, output_transit_skim_file_path_name, NONE, NONE);
			m_container(std::vector<float>, skim_interval_endpoint_minutes, NONE, NONE);
			m_data(int, skim_interval_length_minutes, NONE, NONE);
			m_data(bool, do_skimming, NONE, NONE);
			m_data(bool, use_skim_intervals, NONE, NONE);
			m_data(bool, use_skim_intervals_from_previous, NONE, NONE);
			m_data(double, skim_averaging_factor, NONE, NONE);

			m_data(bool, time_dependent_routing, NONE, NONE);
			m_data(double, time_dependent_routing_weight_shape, NONE, NONE);
			m_data(double, time_dependent_routing_weight_scale, NONE, NONE);
			m_data(double, time_dependent_routing_weight_factor, NONE, NONE);

			m_data(string, timing_choice_model_file, NONE, NONE);

			m_data(bool, multimodal_routing, NONE, NONE);
			m_data(double, multimodal_routing_weight_shape, NONE, NONE);
			m_data(double, multimodal_routing_weight_scale, NONE, NONE);
			m_data(double, multimodal_routing_weight_factor, NONE, NONE);
			
			m_data(string, activity_start_time_model_file_name, NONE, NONE);

			m_data(string, vehicle_distribution_file_name, NONE, NONE);

			m_data(double, percent_to_synthesize, NONE, NONE);
			m_data(double, ipf_tolerance, NONE, NONE);
			m_data(int, marginal_tolerance, NONE, NONE);
			m_data(int, maximum_iterations, NONE, NONE);
			m_data(bool, write_marginal_output, NONE, NONE);
			m_data(bool, write_full_output, NONE, NONE);
			m_data(string, popsyn_control_file_name, NONE, NONE);

			//m_data(ofstream, output_writer, NONE, NONE);

			m_data(_lock, statistics_update_lock, NONE, NONE);

			m_data(int, network_cumulative_loaded_vehicles, NONE, NONE);
			m_data(int, network_cumulative_departed_vehicles, NONE, NONE);
			m_data(int, network_in_network_vehicles, NONE, NONE);
			m_data(int, network_cumulative_arrived_vehicles, NONE, NONE);
			m_data(int, network_cumulative_switched_decisions, NONE, NONE);
			m_data(float, network_average_trip_travel_time, NONE, NONE);
			m_data(float, network_cumulative_switched_decisions_excessive_delay, NONE, NONE);
			m_data(float, network_cumulative_switched_decisions_realtime_informed, NONE, NONE);
			m_data(float, network_cumulative_switched_decisions_ITS_informed, NONE, NONE);
			
			m_data(int, network_in_system_vehicles, NONE, NONE);

			m_data(int, rng_type, NONE, NONE);

			m_data(int, merging_mode, NONE, NONE);

			m_data(int, snapshot_period, NONE, NONE); // in second

			m_data(bool, write_db_input_to_files, NONE, NONE); // to control whether database input is written to files
			m_data(bool, run_simulation_for_db_input, NONE, NONE); // to control whether to run simulation for database input

			m_data(bool, write_network_snapshots, NONE, NONE);
			m_data(bool, read_network_snapshots, NONE, NONE);
			m_data(bool, routing_with_snapshots, NONE, NONE);
			m_data(string, input_network_snapshots_file_path_name, NONE, NONE);

			m_data(bool, compare_with_historic_moe, NONE, NONE);
			m_data(string, historic_network_moe_file_path_name, NONE, NONE);
			m_data(string, historic_link_moe_file_path_name, NONE, NONE);
			m_data(string, historic_demand_moe_directory, NONE, NONE);
			m_data(bool, read_normal_day_link_moe, NONE, NONE);
			m_data(string, normal_day_link_moe_file_path_name, NONE, NONE);

			m_data(bool, write_node_control_state, NONE, NONE);
			m_data(bool, write_vehicle_trajectory, NONE, NONE);
			m_data(bool, write_network_link_flow, NONE, NONE);
			m_data(bool, write_network_link_turn_time, NONE, NONE);
			m_data(bool, write_output_summary, NONE, NONE);
			m_data(bool, output_link_moe_for_assignment_interval, NONE, NONE);
			m_data(bool, output_turn_movement_moe_for_assignment_interval, NONE, NONE);
			m_data(bool, output_network_moe_for_assignment_interval, NONE, NONE);
			m_data(bool, output_analzye_link_group_moe_for_assignment_interval, NONE, NONE);
			m_data(bool, load_analyze_link_groups_from_file, NONE, NONE);
			m_data(string, analyze_link_groups_file_path_name, NONE, NONE);
			m_data(fstream, analyze_link_groups_file, NONE, NONE);
			
			m_data(shared_ptr<odb::database>, result_db_ptr, NONE, NONE);
			m_data(shared_ptr<odb::database>, demand_db_ptr, NONE, NONE);
			m_data(shared_ptr<odb::database>, popsyn_db_ptr, NONE, NONE);
			m_data(string, output_result_database_name, NONE, NONE);

			//m_data(bool, DB_output_link_moe_for_assignment_interval, NONE, NONE);
			m_data(bool, write_ttime_distribution_from_network_model, NONE, NONE);
			m_data(fstream, ttime_distribution_file, NONE, NONE);
			m_data(bool, output_link_moe_for_simulation_interval, NONE, NONE);
			m_data(bool, output_turn_movement_moe_for_simulation_interval, NONE, NONE);
			m_data(bool, output_network_moe_for_simulation_interval, NONE, NONE);
			
			m_data(int, vehicle_trajectory_output_threshold, NONE, NONE);
			
			m_data(bool, use_tmc, NONE, NONE);
			m_data(bool, use_network_events, NONE, NONE);

			m_data(bool, jam_density_constraints_enforced, NONE, NONE);
			m_data(bool, maximum_flow_rate_constraints_enforced, NONE, NONE);
			
			m_data(bool, vehicle_taking_action, NONE, NONE);

			/// enroute switching parameters pretrip_informed_market_share
			m_data(double, pretrip_informed_market_share, NONE, NONE);
			m_data(double, realtime_informed_vehicle_market_share, NONE, NONE);
			m_data(double, information_compliance_rate_mean, NONE, NONE);
			m_data(double, information_compliance_rate_standard_deviation, NONE, NONE);
			m_data(double, relative_indifference_band_route_choice_mean, NONE, NONE);
			m_data(double, relative_indifference_band_route_choice_standard_deviation, NONE, NONE);
			m_data(double, minimum_travel_time_saving_mean, NONE, NONE);
			m_data(double, minimum_travel_time_saving_standard_deviation, NONE, NONE);
			m_data(bool, enroute_switching_enabled, NONE, NONE);
			m_data(bool, use_realtime_travel_time_for_enroute_switching, NONE, NONE);
			m_data(double, minimum_delay_ratio_for_enroute_switching, NONE, NONE);
			m_data(double, minimum_delay_seconds_for_enroute_switching, NONE, NONE);
			m_data(bool, enroute_switching_on_excessive_delay, NONE, NONE);
			m_data(double, enroute_excessive_delay_factor, NONE, NONE);
			m_data(double, minimum_seconds_from_arrival_for_enroute_switching, NONE, NONE);

			m_data(bool, use_link_based_routing, NONE, NONE);

			m_data(bool, multimodal_network_input, NONE, NONE);

			m_data(string, buildings_geometry_file, NONE, NONE);
			m_data(bool, use_buildings,NONE,NONE);

			m_data(bool, color_cars_randomly,NONE,NONE);

			m_data(string, tile_imagery_file, NONE, NONE);
			m_data(bool, use_tile_imagery, NONE, NONE);
			m_data(int, tile_imagery_alpha_level, NONE, NONE);

			m_data(double, accident_event_duration_reduction, NONE, NONE);

			Scenario_Implementation()
			{
				UNLOCK(_statistics_update_lock);
			}

			template<typename TargetType> void increase_network_cumulative_loaded_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_cumulative_loaded_vehicles++;
				UNLOCK(_statistics_update_lock);
			}

			template<typename TargetType> void increase_network_cumulative_departed_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_cumulative_departed_vehicles++;
				UNLOCK(_statistics_update_lock);			
			}

			template<typename TargetType> void increase_network_in_network_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_in_network_vehicles++;
				UNLOCK(_statistics_update_lock);			
			}

			template<typename TargetType> void increase_network_in_system_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_in_system_vehicles++;
				UNLOCK(_statistics_update_lock);			
			}

			template<typename TargetType> void increase_network_cumulative_arrived_vehicles(float travel_time)
			{
				LOCK(_statistics_update_lock);
				_network_average_trip_travel_time = (_network_average_trip_travel_time * _network_cumulative_arrived_vehicles + travel_time) / ((float)_network_cumulative_arrived_vehicles + 1.0f);
				_network_cumulative_arrived_vehicles++;
				UNLOCK(_statistics_update_lock);			
			}
			
			template<typename TargetType> void increase_network_cumulative_switched_decisions(int cause_for_switching)
			{
				LOCK(_statistics_update_lock);
				_network_cumulative_switched_decisions++;
				switch(cause_for_switching)
				{
				case Scenario_Components::Types::Cause_For_Enroute_Switching::EXCESSIVE_DELAY:
					_network_cumulative_switched_decisions_excessive_delay++;
					break;
				case Scenario_Components::Types::Cause_For_Enroute_Switching::REALTIME_INFORMED:
					_network_cumulative_switched_decisions_realtime_informed++;
					break;
				case Scenario_Components::Types::Cause_For_Enroute_Switching::ITS_INFORMED:
					_network_cumulative_switched_decisions_ITS_informed++;
					break;
				default:
					cout << "Unknown cause for switching." << endl;
					assert(false);
					break;
				}

				UNLOCK(_statistics_update_lock);			
			}

			template<typename TargetType> void decrease_network_in_network_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_in_network_vehicles--;
				UNLOCK(_statistics_update_lock);			
			}

			template<typename TargetType> void decrease_network_in_system_vehicles()
			{
				LOCK(_statistics_update_lock);
				_network_in_system_vehicles--;
				UNLOCK(_statistics_update_lock);			
			}

			m_data(double, assignment_time_in_seconds, NONE, NONE);
			m_data(double, simulation_time_in_seconds, NONE, NONE);
			m_data(double, operation_time_in_seconds, NONE, NONE);
			m_data(double, output_time_in_seconds, NONE, NONE);
			m_data(double, condition_time_in_seconds, NONE, NONE);
			m_data(string, database_name, NONE, NONE);

			m_data(string, mode_choice_model_file, NONE, NONE);
			m_data(string, destination_choice_model_file, NONE, NONE);
			m_data(string, telecommute_choice_model_file, NONE, NONE);
			m_data(string, cav_wtp_model_file, NONE, NONE);
			m_data(string, multimodal_routing_model_file, NONE, NONE);
		};
	}

}
