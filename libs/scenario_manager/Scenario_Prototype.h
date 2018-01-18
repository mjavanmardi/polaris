#pragma once

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>
#include <rapidjson/pointer.h>

namespace Scenario_Components
{
	namespace Types
	{
		struct ScenarioData
		{
			rapidjson::Document document;
			std::map<std::string, std::string> key_paths;
		};
	}

	namespace Prototypes
	{
		prototype struct Scenario ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef std::vector<int> IntArray;

			accessor(output_results_database_name, NONE, NONE);
			accessor(output_demand_database_name, NONE, NONE);
			accessor(output_popsyn_database_name, NONE, NONE);
			accessor(input_popsyn_database_name, NONE, NONE);
			accessor(historical_results_database_name, NONE, NONE);

			accessor(simulation_interval_length, NONE, NONE);
			accessor(assignment_interval_length, NONE, NONE);


			accessor(planning_horizon, NONE, NONE);
			accessor(simulation_start_time, NONE, NONE);
			accessor(simulation_end_time, NONE, NONE);

			accessor(intersection_control_flag, NONE, NONE);
			accessor(ramp_metering_flag, NONE, NONE);
			
			accessor(num_simulation_intervals, NONE, NONE);
			accessor(num_assignment_intervals, NONE, NONE);
			accessor(num_simulation_intervals_per_assignment_interval, NONE, NONE);

			accessor(iseed, NONE, NONE);
			
			accessor(demand_reduction_factor, NONE, NONE);

			accessor(path_calculation_interval_length, NONE, NONE);

			//=========================================================
			// CAV PARAMETERS
			// capacity adjustments by facility type
			accessor(capacity_adjustment_highway, NONE, NONE);
			accessor(capacity_adjustment_arterial, NONE, NONE);
			accessor(cacc_capacity_adjustment_alpha, NONE, NONE);
			accessor(cacc_capacity_adjustment_beta, NONE, NONE);
			accessor(simulate_cacc, NONE, NONE);
			accessor(cav_market_penetration, NONE, NONE);
			accessor(cav_vott_adjustment, NONE, NONE);
			accessor(automation_cost, NONE, NONE);
			accessor(vehicle_techchoice_beta_past_crashes, NONE, NONE);
			accessor(vehicle_techchoice_beta_smartphone, NONE, NONE);
			accessor(vehicle_techchoice_beta_carshare, NONE, NONE);
			accessor(vehicle_techchoice_beta_rideshare, NONE, NONE);
			accessor(vehicle_techchoice_beta_drive_alone_work, NONE, NONE);
			accessor(vehicle_techchoice_beta_drive_alone_other, NONE, NONE);
			accessor(vehicle_techchoice_beta_ln_vmt, NONE, NONE);
			accessor(vehicle_techchoice_beta_work_dist, NONE, NONE);
			accessor(vehicle_techchoice_beta_gender, NONE, NONE);
			accessor(vehicle_techchoice_beta_license, NONE, NONE);
			accessor(vehicle_techchoice_beta_num_children, NONE, NONE);
			accessor(vehicle_techchoice_beta_age, NONE, NONE);
			accessor(vehicle_techchoice_beta_empl_density, NONE, NONE);
			accessor(vehicle_techchoice_beta_hhincome, NONE, NONE);
			accessor(vehicle_techchoice_calibration, NONE, NONE);
			accessor(vehicle_techchoice_mu_1, NONE, NONE);
			accessor(vehicle_techchoice_mu_2, NONE, NONE);
			accessor(vehicle_techchoice_mu_3, NONE, NONE);
			

			accessor(current_day_index, NONE, NONE);

			/// If set to true, only the integrated vehicles are counted for the in-network output
			accessor(count_integrated_in_network_vehicles_only, NONE, NONE);

			//accessor(output_writer, NONE, NONE);
			
			accessor(network_cumulative_loaded_vehicles, NONE, NONE);
			accessor(network_cumulative_departed_vehicles, NONE, NONE);
			accessor(network_in_network_vehicles, NONE, NONE);
			accessor(network_cumulative_arrived_vehicles, NONE, NONE);
			accessor(network_cumulative_switched_decisions, NONE, NONE);
			accessor(network_average_trip_travel_time, NONE, NONE);
			accessor(network_cumulative_switched_decisions_excessive_delay, NONE, NONE);
			accessor(network_cumulative_switched_decisions_realtime_informed, NONE, NONE);
			accessor(network_cumulative_switched_decisions_ITS_informed, NONE, NONE);

			accessor(network_in_system_vehicles, NONE, NONE); // vehicles that are currently loaded

			accessor(input_dir_name, NONE, NONE);
			accessor(output_dir_name, NONE, NONE);
				
			// accessors for vehicle trajectory tracking, use either random selection or list-based selection for tracking vehicles
			accessor(vehicle_trajectory_file_name, NONE, NONE);
			accessor(vehicle_trajectory_file, NONE, NONE);
			accessor(vehicle_trajectory_sample_rate, NONE, NONE);
			accessor(vehicle_tracking_list_file_name, NONE, NONE);
			accessor(vehicle_tracking_list, NONE, NONE);
			accessor(use_vehicle_tracking_list, NONE, NONE);

			accessor(routed_path_file_name, NONE, NONE);
			accessor(routed_path_file, NONE, NONE);

			accessor(network_link_flow_file_name, NONE, NONE);
			accessor(network_link_flow_file, NONE, NONE);

			accessor(network_link_turn_time_file_name, NONE, NONE);
			accessor(network_link_turn_time_file, NONE, NONE);

			accessor(network_node_control_state_file_name, NONE, NONE);
			accessor(network_node_control_state_file, NONE, NONE);

			accessor(output_summary_file_name, NONE, NONE);
			accessor(output_summary_file, NONE, NONE);
			
			accessor(vehicle_transfer_file_name, NONE, NONE);
			accessor(vehicle_transfer_file, NONE, NONE);

			accessor(out_network_moe_file, NONE, NONE);
			accessor(out_link_moe_file, NONE, NONE);
			accessor(out_movement_moe_file, NONE, NONE);

			accessor(out_realtime_network_moe_file, NONE, NONE);
			accessor(out_realtime_link_moe_file, NONE, NONE);
			accessor(out_realtime_movement_moe_file, NONE, NONE);
			
			accessor(output_network_snapshots_file, NONE, NONE);
			accessor(input_network_snapshots_file, NONE, NONE);

			accessor(reference_realtime_network_moe_file, NONE, NONE);
			accessor(historic_link_moe_file, NONE, NONE);
			accessor(normal_day_link_moe_file, NONE, NONE);

			accessor(assignment_time_in_seconds, NONE, NONE);
			accessor(simulation_time_in_seconds, NONE, NONE);
			accessor(operation_time_in_seconds, NONE, NONE);
			accessor(output_time_in_seconds, NONE, NONE);
			accessor(condition_time_in_seconds, NONE, NONE);

			accessor(calculate_realtime_moe, NONE, NONE);
			accessor(rng_type, NONE, NONE);
			accessor(merging_mode, NONE, NONE);

			accessor(write_visualizer_snapshot,NONE,NONE);
			
			//===============================================
			// Demand model parameters
			//-----------------------------------------------
			accessor(write_activity_output, NONE, NONE);			// deprecated
			accessor(aggregate_routing, NONE, NONE);				// do routing calculations at aggregate time-steps
			accessor(do_planner_routing, NONE, NONE);				// calculate route when planning trips in place of skims
			accessor(write_demand_to_database, NONE, NONE);			//
			accessor(read_demand_from_database, NONE, NONE);		// 
			accessor(read_population_from_database, NONE, NONE);	// run model with pre-existing synthetic population *must reference <db>-Popsyn.sqlite database
			
			accessor(flexible_work_percentage, NONE, NONE);
			//===============================================
			// Vehicle Choice parameters
			//-----------------------------------------------
			accessor(vehicle_distribution_file_name, NONE, NONE);	// file containing the vehicle type distribution by taz

			//===============================================
			// Popsyn parameters
			//-----------------------------------------------
			accessor(percent_to_synthesize, NONE, NONE);			// percent to synthesize 0-1.0, setting to 0 will run only fixed demand
			accessor(ipf_tolerance, NONE, NONE);	
			accessor(marginal_tolerance, NONE, NONE);
			accessor(maximum_iterations, NONE, NONE);
			accessor(write_marginal_output, NONE, NONE);
			accessor(write_full_output, NONE, NONE);
			accessor(popsyn_control_file_name, NONE, NONE);			// reference to popsyn linker file

			accessor(database_name, NONE, NONE);

			accessor(snapshot_period, NONE, NONE);

			//===============================================
			// i/o parameters
			//===============================================
			accessor(write_db_input_to_files, NONE, NONE); // to control whether database input is written to files
			accessor(run_simulation_for_db_input, NONE, NONE); // to control whether to run simulation for database input

			accessor(write_node_control_state, NONE, NONE);
			accessor(write_vehicle_trajectory, NONE, NONE);
			accessor(write_network_link_flow, NONE, NONE);
			accessor(write_network_link_turn_time, NONE, NONE);
			accessor(write_output_summary, NONE, NONE);
			
			accessor(write_network_snapshots, NONE, NONE);
			accessor(read_network_snapshots, NONE, NONE);
			accessor(routing_with_snapshots, NONE, NONE);
			accessor(input_network_snapshots_file_path_name, NONE, NONE);	

			// SKIMMING INFORMATION
			accessor(write_skim_tables, NONE, NONE);
			accessor(read_skim_tables, NONE, NONE);
			accessor(input_highway_skim_file_path_name, NONE, NONE);	
			accessor(output_highway_skim_file_path_name, NONE, NONE);	
			accessor(input_highway_cost_skim_file_path_name, NONE, NONE);	
			accessor(output_highway_cost_skim_file_path_name, NONE, NONE);
			accessor(input_transit_skim_file_path_name, NONE, NONE);	
			accessor(output_transit_skim_file_path_name, NONE, NONE);
			accessor(skim_interval_endpoint_minutes, NONE, NONE);
			accessor(skim_interval_length_minutes, NONE, NONE);
			accessor(do_skimming, NONE, NONE);
			accessor(use_skim_intervals, NONE, NONE);
			accessor(use_skim_intervals_from_previous, NONE, NONE);
			accessor(skim_averaging_factor, NONE, NONE); // factor to weight current skim when updating skim matrices -> defaults to 1.0 (no use of the previous skim info)

			accessor(compare_with_historic_moe, NONE, NONE);
			accessor(historic_network_moe_file_path_name, NONE, NONE);
			accessor(historic_link_moe_file_path_name, NONE, NONE);
			accessor(read_normal_day_link_moe, NONE, NONE);
			accessor(time_dependent_routing, NONE, NONE);
			accessor(time_dependent_routing_weight_shape, NONE, NONE);
			accessor(time_dependent_routing_weight_scale, NONE, NONE);
			accessor(time_dependent_routing_weight_factor, NONE, NONE);
			accessor(normal_day_link_moe_file_path_name, NONE, NONE);

			accessor(multimodal_routing, NONE, NONE);
			accessor(multimodal_routing_weight_shape, NONE, NONE);
			accessor(multimodal_routing_weight_scale, NONE, NONE);
			accessor(multimodal_routing_weight_factor, NONE, NONE);
			
			accessor(historic_demand_moe_directory, NONE, NONE);

			accessor(output_link_moe_for_simulation_interval, NONE, NONE);
			accessor(output_turn_movement_moe_for_simulation_interval, NONE, NONE);
			accessor(output_network_moe_for_simulation_interval, NONE, NONE);
			accessor(output_link_moe_for_assignment_interval, NONE, NONE);
			accessor(output_turn_movement_moe_for_assignment_interval, NONE, NONE);
			accessor(output_network_moe_for_assignment_interval, NONE, NONE);
			accessor(output_analzye_link_group_moe_for_assignment_interval, NONE, NONE);
			accessor(load_analyze_link_groups_from_file, NONE, NONE);
			accessor(analyze_link_groups_file_path_name, NONE, NONE);
			accessor(analyze_link_groups_file, NONE, NONE);

			accessor(result_db_ptr, NONE, NONE);
			accessor(demand_db_ptr, NONE, NONE);
			accessor(popsyn_db_ptr, NONE, NONE);
			accessor(output_result_database_name, NONE, NONE);

			accessor(use_tmc, NONE, NONE);
			accessor(use_network_events, NONE, NONE);
			accessor(accident_event_duration_reduction, NONE, NONE);

			accessor(DB_output_link_moe_for_assignment_interval, NONE, NONE);
			
			accessor(write_ttime_distribution_from_network_model, NONE, NONE);
			accessor(ttime_distribution_file, NONE, NONE);
			accessor(vehicle_trajectory_output_threshold, NONE, NONE);

			accessor(jam_density_constraints_enforced, NONE, NONE);
			accessor(maximum_flow_rate_constraints_enforced, NONE, NONE);

			accessor(vehicle_taking_action, NONE, NONE);
			/// enroute switching parameters
			accessor(pretrip_informed_market_share, NONE, NONE);
			accessor(realtime_informed_vehicle_market_share, NONE, NONE);
			accessor(information_compliance_rate_mean, NONE, NONE);
			accessor(information_compliance_rate_standard_deviation, NONE, NONE);
			accessor(relative_indifference_band_route_choice_mean, NONE, NONE);
			accessor(relative_indifference_band_route_choice_standard_deviation, NONE, NONE);
			accessor(minimum_travel_time_saving_mean, NONE, NONE);
			accessor(minimum_travel_time_saving_standard_deviation, NONE, NONE);
			accessor(enroute_switching_enabled, NONE, NONE);
			accessor(use_realtime_travel_time_for_enroute_switching, NONE, NONE);
			accessor(minimum_delay_ratio_for_enroute_switching, NONE, NONE);
			accessor(minimum_delay_seconds_for_enroute_switching, NONE, NONE);
			accessor(minimum_seconds_from_arrival_for_enroute_switching, NONE, NONE);
			accessor(multimodal_network_input, NONE, NONE);
			accessor(enroute_switching_on_excessive_delay, NONE, NONE);
			accessor(enroute_excessive_delay_factor, NONE, NONE);
			accessor(use_link_based_routing,NONE,NONE);

			/// antares visualization parameters
			accessor(buildings_geometry_file, NONE, NONE);
			accessor(use_buildings,NONE,NONE);
			accessor(color_cars_randomly,NONE,NONE);

			// tile imagery parameters
			accessor(tile_imagery_file,NONE,NONE);
			accessor(use_tile_imagery,NONE,NONE);
			accessor(tile_imagery_alpha_level,NONE,NONE);

			//Parameter files
			accessor(mode_choice_model_file, NONE, NONE);
			accessor(destination_choice_model_file, NONE, NONE);
			accessor(telecommute_choice_model_file, NONE, NONE);
			accessor(cav_wtp_model_file, NONE, NONE);
			accessor(timing_choice_model_file, NONE, NONE);
			accessor(multimodal_routing_model_file, NONE, NONE);

			void default_static_initializer()
			{
				simulation_start_time<int>(0);
				simulation_end_time<int>(172800);
				simulation_interval_length<int>(6);
				demand_reduction_factor<double>(1.0);
				num_simulation_intervals_per_assignment_interval<int>(50);
				use_link_based_routing<bool>(false);
				rng_type<int>(Scenario_Components::Types::RNG_Type_Keys::DETERMINISTIC);
				merging_mode<int>(Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_DEMAND);
				iseed<unsigned long>(0.0);
				intersection_control_flag<int>(0);
				ramp_metering_flag<bool>(false);
				snapshot_period<int>(300);
				database_name<string>((string)"");
				historical_results_database_name<string>((string)"");
				input_popsyn_database_name<string>((string)"");
				tile_imagery_file<string>((string)"");
				tile_imagery_alpha_level<int>(255);
				color_cars_randomly<bool>(false);
				write_activity_output<bool>(false);
				aggregate_routing<bool>(false);
				do_planner_routing<bool>(false);
				write_demand_to_database<bool>(false);
				read_demand_from_database<bool>(false);
				read_population_from_database<bool>(false);
				cav_market_penetration<double>(0.0);
				cav_vott_adjustment<double>(1.0);
				vehicle_distribution_file_name<string>((string)"vehicle_distribution.txt");
				automation_cost<double>(FLT_MAX);
				percent_to_synthesize<double>(1.0);
				ipf_tolerance<double>(0.01);
				marginal_tolerance<int>(5);
				maximum_iterations<int>(100);
				write_marginal_output<bool>(false);
				write_full_output<bool>(false);
				popsyn_control_file_name<string>((string)"popsyn_control_file.txt");
				activity_start_time_model_file_name<string>((string)"start_time_duration_data_new.txt");
				write_visualizer_snapshot<bool>(false);
				count_integrated_in_network_vehicles_only<bool>(false);
				output_dir_name<string>((string)"");
				write_db_input_to_files<bool>(false);
				run_simulation_for_db_input<bool>(true);
				write_node_control_state<bool>(false);
				write_network_link_flow<bool>(false);
				write_network_link_turn_time<bool>(false);
				write_output_summary<bool>(true);
				output_link_moe_for_simulation_interval<bool>(false);
				output_turn_movement_moe_for_simulation_interval<bool>(false);
				output_network_moe_for_simulation_interval<bool>(false);
				write_network_snapshots<bool>(false);
				read_network_snapshots<bool>(false);
				routing_with_snapshots<bool>(false);
				input_network_snapshots_file_path_name<string>((string)"input_network_snapshots");
				capacity_adjustment_highway<double>(1.0);
				capacity_adjustment_arterial<double>(1.0);
				simulate_cacc<bool>(false);
				flexible_work_percentage<double>(0.12);
				write_vehicle_trajectory<bool>(false);
				vehicle_trajectory_sample_rate<double>(1.0);
				vehicle_tracking_list_file_name<string>("");
				write_skim_tables<bool>(false);
				read_skim_tables<bool>(false);
				input_highway_skim_file_path_name<string>((string)"");
				output_highway_skim_file_path_name<string>((string)"highway_skim_file_out.txt");
				input_highway_cost_skim_file_path_name<string>((string)"");
				output_highway_cost_skim_file_path_name<string>((string)"highway_cost_skim_file_out.txt");
				input_transit_skim_file_path_name<string>((string)"");
				output_transit_skim_file_path_name<string>((string)"transit_skim_file_out.txt");
				skim_interval_endpoint_minutes<float>(0.0);
				skim_averaging_factor<double>(0.5);
				skim_interval_length_minutes<int>(1440);
				compare_with_historic_moe<bool>(false);
				historic_network_moe_file_path_name<string>((string)"historic_realtime_moe_network.csv");
				historic_link_moe_file_path_name<string>((string)"historic_moe_link.csv");
				read_normal_day_link_moe<bool>(false);
				normal_day_link_moe_file_path_name<string>((string)"normal_day_moe_link.csv");
				historic_demand_moe_directory<string>((string)"");
				output_link_moe_for_assignment_interval<bool>(false);
				output_turn_movement_moe_for_assignment_interval<bool>(false);
				output_network_moe_for_assignment_interval<bool>(false);
				output_analzye_link_group_moe_for_assignment_interval<bool>(false);
				load_analyze_link_groups_from_file<bool>(false);
				analyze_link_groups_file_path_name<string>((string)"analyze_link_groups");
				write_ttime_distribution_from_network_model<bool>(false);
				vehicle_trajectory_output_threshold<int>(-1);
				use_tmc<bool>(false);
				use_network_events<bool>(false);
				jam_density_constraints_enforced<bool>(true);
				maximum_flow_rate_constraints_enforced<bool>(true);
				vehicle_taking_action<bool>(false);
				pretrip_informed_market_share<double>(0.5);
				realtime_informed_vehicle_market_share<double>(0.5);
				information_compliance_rate_mean<double>(1.0);
				information_compliance_rate_standard_deviation<double>(0.5);
				relative_indifference_band_route_choice_mean<double>(0.1);
				minimum_travel_time_saving_mean<double>(1.0);
				minimum_travel_time_saving_standard_deviation<double>(1.0);
				enroute_switching_enabled<bool>(true);
				use_realtime_travel_time_for_enroute_switching<bool>(false);
				minimum_delay_ratio_for_enroute_switching<double>(3.0);
				minimum_delay_seconds_for_enroute_switching<double>(300.0);
				enroute_switching_on_excessive_delay<bool>(true);
				multimodal_network_input<bool>(false);
				enroute_excessive_delay_factor<double>(1.0);
				minimum_seconds_from_arrival_for_enroute_switching<double>(300.0);
				time_dependent_routing<bool>(false);
				time_dependent_routing_weight_shape<double>(2.5);
				time_dependent_routing_weight_scale<double>(1800.0);
				time_dependent_routing_weight_factor<double>(1.0);
				accident_event_duration_reduction<double>(1.0);
				calculate_realtime_moe<bool>(true);
				buildings_geometry_file<string>((string)"");
				mode_choice_model_file<string>((string)"");
				destination_choice_model_file<string>((string)"");
				telecommute_choice_model_file<string>((string)"");
				cav_wtp_model_file<string>((string)"");
				cacc_capacity_adjustment_alpha<double>(1.0121);
				cacc_capacity_adjustment_beta<double>(2.4697);
				multimodal_routing<bool>(false);
			}

			void get_KV_paths(std::map<string,string>& key_paths, const rapidjson::Value &obj, std::string path, size_t indent = 0)
			{
				if (obj.IsObject())  //check if object
				{
					for (rapidjson::Value::ConstMemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr)    //iterate through object   
					{
						const rapidjson::Value& objName = obj[itr->name.GetString()]; //make object value
						if (itr->value.IsObject())
						{
							std::string new_path;
							if (path == "")
								new_path = itr->name.GetString();
							else
								new_path = path + std::string("/") + itr->name.GetString();
								
							get_KV_paths(key_paths, objName, new_path, indent + 1); //if couldn't find in object, enter object and repeat process recursively 
						}
						else
						{
							std::string key = itr->name.GetString();
							//std::cout << "path to " << key << "=" << path << std::endl;
							key_paths[key] = path;
						}
					}
				}
			}

			template<typename TargetType> void read_scenario_data(const char* filename)
			{
				// set the base values
				default_static_initializer();

				// now see if there are config file changes
				Scenario_Components::Types::ScenarioData document;

				if (!parse_option_file(document, filename))
					THROW_EXCEPTION("Scenario file '" << filename << "' was not able to be opened.");

				//===============================================
				// set start time
				std::string start_time_in_string;
				int start_time;
				if (set_parameter<std::string>(document, "starting_time_hh_mm", start_time_in_string))
				{
					start_time_in_string += ":00";
					start_time = convert_hhmmss_to_seconds(start_time_in_string);
					//assert(start_time == 0); // to be done for start time > 0
				}
				else start_time = 0;
				simulation_start_time<int>(start_time);

				//===============================================
				// set end time
				std::string end_time_in_string;
				int end_time;
				if(set_parameter<std::string>(document, "ending_time_hh_mm", end_time_in_string))
				{
					end_time_in_string += ":00";
					end_time = convert_hhmmss_to_seconds(end_time_in_string);
				}
				else end_time = 60 * 60 * 24 * 2;
				simulation_end_time<int>(end_time);

				planning_horizon<int>(end_time - start_time);

				//===============================================
				// set interval length
				set_parameter<int>(document, "simulation_interval_length_in_second", simulation_interval_length<int &>());

				//===============================================
				// set the demand reduction factor, used to reduce the demand read from the database
				set_parameter<double>(document, "demand_reduction_factor", demand_reduction_factor<double &>());
				cout << endl << "demand reduction factor: " << demand_reduction_factor<float>() << endl;

				//===============================================
				// set sim_interval per assignment interval
				int assignment_intervals = num_simulation_intervals_per_assignment_interval<int>();
				set_parameter<int>(document, "num_simulation_intervals_per_assignment_interval", assignment_intervals);
				assignment_interval_length<int>(assignment_intervals*simulation_interval_length<int>());
				

				// Flag for link-based routing: defaults to false.  Set to true if want to restrict routing to activity locations to the link+dir defined in database
				// If left to false, then we can route to an activity location on a link using either DIR (i.e. left hand turns into activity location ) [RECOMMENDED]
				set_parameter<bool>(document, "use_link_based_routing", use_link_based_routing<bool &>());

				//===============================================
				// set rng type
				std::string rng_type_string;
				if(set_parameter<std::string>(document, "rng_type", rng_type_string))
				{
					if (rng_type_string.compare("DETERMINISTIC") == 0)
					{
						rng_type<int>(Scenario_Components::Types::RNG_Type_Keys::DETERMINISTIC);
					}
					else if (rng_type_string.compare("RANDOM") == 0)
					{
						rng_type<int>(Scenario_Components::Types::RNG_Type_Keys::RANDOM);
					}
					else
					{
						cout << "Rng type not supported" << endl;
						assert(false);
					}
				}

				//===============================================
				// set merging mode
				std::string merging_mode_string;
				if (set_parameter<std::string>(document, "merging_mode", merging_mode_string))
				{
					if (merging_mode_string.compare("DRIVING_RULE") == 0)
					{
						merging_mode<int>(Scenario_Components::Types::Merging_Mode_Keys::DRIVING_RULE);
					}
					else if (merging_mode_string.compare("PROPORTION_TO_DEMAND") == 0)
					{
						merging_mode<int>(Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_DEMAND);
					}
					else if (merging_mode_string.compare("PROPORTION_TO_LINK") == 0)
					{
						merging_mode<int>(Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_LINK);
					}
					else if (merging_mode_string.compare("PROPORTION_TO_LANE") == 0)
					{
						merging_mode<int>(Scenario_Components::Types::Merging_Mode_Keys::PROPORTION_TO_LANE);
					}
					else
					{
						cout << "Merging mode not supported" << endl;
						assert(false);
					}
				}

				//===============================================
				// set control parameters
				set_parameter<unsigned long>(document, "seed", iseed<unsigned long &>());
				set_parameter<int>(document, "node_control_flag", intersection_control_flag<int &>());
				set_parameter<bool>(document, "ramp_metering_flag", ramp_metering_flag<bool &>());
				set_parameter<int>(document, "snapshot_period", snapshot_period<int &>());

				if (!set_parameter<std::string>(document, "database_name", database_name<std::string &>()))
				{
					THROW_EXCEPTION("ERROR: Input database name required.");
				}

				set_parameter<std::string>(document, "historical_results_database_name", historical_results_database_name<std::string &>());

				if (set_parameter<std::string>(document, "input_popsyn_database_name", input_popsyn_database_name<std::string &>()))
				{
					this->read_population_from_database(true);
				}
				else
				{
					this->read_population_from_database(false);
				}


				num_simulation_intervals<int>(planning_horizon<int>() / simulation_interval_length<int>());
				num_assignment_intervals<int>(planning_horizon<int>() / assignment_interval_length<int>());
				num_simulation_intervals_per_assignment_interval<int>(assignment_interval_length<int>() / simulation_interval_length<int>());


				//typedef typename ComponentType::output_writer_type OutputWriterType;
				//output_writer<OutputWriterType&>().open("Output.log");

				network_cumulative_loaded_vehicles<int>(0.0);
				network_cumulative_departed_vehicles<int>(0.0);
				network_in_network_vehicles<int>(0.0);
				network_cumulative_arrived_vehicles<int>(0.0);
				network_cumulative_switched_decisions<int>(0.0);
				network_average_trip_travel_time<float>(0.0f);
				network_in_system_vehicles<int>(0.0);

				assignment_time_in_seconds<double>(0.0);
				simulation_time_in_seconds<double>(0.0);
				operation_time_in_seconds<double>(0.0);
				output_time_in_seconds<double>(0.0);

				//===============================================
				// tile imagery parameters
				if (set_parameter<std::string>(document, "tile_imagery_file", tile_imagery_file<std::string &>()))
				{
					use_tile_imagery<bool>(true);
					set_parameter<int>(document, "tile_imagery_alpha_level", tile_imagery_alpha_level<int &>());
				}
				else
				{
					use_tile_imagery<bool>(false);
				}

				set_parameter<bool>(document, "color_cars_randomly", color_cars_randomly<bool &>());

				//===============================================
				// Demand model parameters 
				set_parameter<bool>(document, "write_activity_output", this->template write_activity_output<bool &>());
				set_parameter<bool>(document, "aggregate_routing", this->template aggregate_routing<bool &>());
				set_parameter<bool>(document, "do_planner_routing", this->template do_planner_routing<bool &>());
				set_parameter<bool>(document, "write_demand_to_database", this->template write_demand_to_database<bool &>());
				set_parameter<bool>(document, "read_demand_from_database", this->template read_demand_from_database<bool &>());
				set_parameter<bool>(document, "read_population_from_database", this->template read_population_from_database<bool &>());
				set_parameter<double>(document, "cav_market_penetration", this->template cav_market_penetration<double &>());
				set_parameter<double>(document, "cav_vott_adjustment", this->template cav_vott_adjustment<double &>());

				//=======================================================================================================================================================
				// Vehicle Choice Model parameters
				// Start time model parameters
				set_parameter<std::string>(document, "vehicle_distribution_file_name", this->template vehicle_distribution_file_name<std::string &>());
				set_parameter<double>(document, "automation_cost", this->template automation_cost<double &>());


				//=======================================================================================================================================================
				// PopSyn parameters
				set_parameter<double>(document, "percent_to_synthesize", this->template percent_to_synthesize<double &>());
				set_parameter<double>(document, "ipf_tolerance", this->template ipf_tolerance<double &>());
				set_parameter<int>(document, "marginal_tolerance", this->template marginal_tolerance<int &>());
				set_parameter<int>(document, "maximum_iterations", this->template maximum_iterations<int &>());
				set_parameter<bool>(document, "write_marginal_output", this->template write_marginal_output<bool &>());
				set_parameter<bool>(document, "write_full_output", this->template write_full_output<bool &>());
				set_parameter<std::string>(document, "popsyn_control_file", this->template popsyn_control_file_name<std::string &>());

				// Start time model parameters
				set_parameter<std::string>(document, "activity_start_time_model_file_name", this->template activity_start_time_model_file_name<std::string &>());
				set_parameter<bool>(document, "write_visualizer_snapshot", this->template write_visualizer_snapshot<bool &>());


				// set I/O parameters		
				set_parameter<bool>(document, "count_integrated_in_network_vehicles_only", count_integrated_in_network_vehicles_only<bool &>());
				set_parameter<std::string>(document, "output_dir_name", output_dir_name<std::string &>());
				set_parameter<bool>(document, "write_db_input_to_files", write_db_input_to_files<bool &>());
				set_parameter<bool>(document, "run_simulation_for_db_input", run_simulation_for_db_input<bool &>());
				set_parameter<bool>(document, "write_node_control_state", write_node_control_state<bool &>());
				set_parameter<bool>(document, "write_network_link_flow", write_network_link_flow<bool &>());
				set_parameter<bool>(document, "write_network_link_turn_time", write_network_link_turn_time<bool &>());
				set_parameter<bool>(document, "write_output_summary", write_output_summary<bool &>());
				set_parameter<bool>(document, "output_link_moe_for_simulation_interval", output_link_moe_for_simulation_interval<bool &>());
				set_parameter<bool>(document, "output_turn_movement_moe_for_simulation_interval", output_turn_movement_moe_for_simulation_interval<bool &>());
				set_parameter<bool>(document, "output_network_moe_for_simulation_interval", output_network_moe_for_simulation_interval<bool &>());
				set_parameter<bool>(document, "write_network_snapshots", write_network_snapshots<bool &>());
				set_parameter<bool>(document, "read_network_snapshots", read_network_snapshots<bool &>());
				set_parameter<bool>(document, "routing_with_snapshots", routing_with_snapshots<bool &>());
				set_parameter<std::string>(document, "input_network_snapshots_file_path_name", input_network_snapshots_file_path_name<std::string &>());

				// read capacity adjustments 
				set_parameter<double>(document, "capacity_adjustment_highway", capacity_adjustment_highway<double &>());
				set_parameter<double>(document, "capacity_adjustment_arterial", capacity_adjustment_arterial<double &>());
				set_parameter<bool>(document, "simulate_cacc", simulate_cacc<bool &>());
				set_parameter<double>(document, "cacc_capacity_adjustment_alpha", cacc_capacity_adjustment_alpha<double &>());
				set_parameter<double>(document, "cacc_capacity_adjustment_beta", cacc_capacity_adjustment_beta<double &>());
				set_parameter<double>(document, "flexible_work_percentage", flexible_work_percentage<double &>());

				//===============================================
				// Vehicle trajectory tracking parameters
				set_parameter<bool>(document, "write_vehicle_trajectory", write_vehicle_trajectory<bool &>());
				set_parameter<double>(document, "vehicle_trajectory_sample_rate", vehicle_trajectory_sample_rate<double &>());
				if (set_parameter<std::string>(document, "vehicle_tracking_list_file_name", vehicle_tracking_list_file_name<std::string &>()))
				{
					vehicle_trajectory_sample_rate<double>(0.0);
					use_vehicle_tracking_list<bool>(true);
					File_IO::File_Reader fr;
					fr.Open(this->vehicle_tracking_list_file_name<string>(), false);
					while (fr.Read())
					{
						int vehid = 0;
						fr.Get_Data<int>(vehid, 0);
						std::unordered_set<int>::iterator itr = vehicle_tracking_list<std::unordered_set<int>&>().find(vehid);
						if (itr == vehicle_tracking_list<std::unordered_set<int>&>().end()) this->vehicle_tracking_list<std::unordered_set<int>&>().insert(vehid);
					}
					cout << endl << "Tracking " << vehicle_tracking_list<std::unordered_set<int>&>().size() << " vehicles." << endl;
				}
				else
				{
					use_vehicle_tracking_list<bool>(false);
				}

				// GET NETWORK SKIMMING PARAMETERS
				set_parameter<bool>(document, "write_skim_tables", this->template write_skim_tables<bool &>());

				set_parameter<std::string>(document, "input_highway_skim_file_path_name", this->template input_highway_skim_file_path_name<std::string &>());
				set_parameter<std::string>(document, "output_highway_skim_file_path_name", this->template output_highway_skim_file_path_name<std::string &>());
				set_parameter<std::string>(document, "input_highway_cost_skim_file_path_name", this->template input_highway_cost_skim_file_path_name<std::string &>());
				set_parameter<std::string>(document, "output_highway_cost_skim_file_path_name", this->template output_highway_cost_skim_file_path_name<std::string &>());
				set_parameter<std::string>(document, "input_transit_skim_file_path_name", this->template input_transit_skim_file_path_name<std::string &>());
				set_parameter<std::string>(document, "output_transit_skim_file_path_name", this->template output_transit_skim_file_path_name<std::string &>());

				// If we are reading skim tables, use the intervals defined in that previous skim file, as it is required to be he same
				set_parameter<bool>(document, "read_skim_tables", this->template read_skim_tables<bool &>());
				set_parameter<double>(document, "skim_averaging_factor", this->template skim_averaging_factor<double &>());
				if (this->template read_skim_tables<bool>())
				{
					use_skim_intervals_from_previous<bool>(true);
					do_skimming<bool>(true);
				}
				else if (set_parameter<std::vector<float>>(document, "skim_interval_endpoint_minutes", this->template skim_interval_endpoint_minutes<std::vector<float>&>()))
				{
					use_skim_intervals<bool>(true);
					use_skim_intervals_from_previous<bool>(false);
					do_skimming<bool>(true);
				}
				else
				{
					use_skim_intervals<bool>(false);
					if (set_parameter<int>(document, "skim_interval_length_minutes", this->template skim_interval_length_minutes<int &>()))
					{
						do_skimming<bool>(true);
					}
					else
					{
						//do_skimming<bool>(false);
						//if (this->template write_skim_tables<bool>())
						//{
						//	THROW_EXCEPTION("ERROR: the 'write_skim_tables' parameters has been set to true, but no skim interval has been defined.  Use the 'skim_interval_endpoint_minutes' (endpoint of each interval) or 'skim_interval_length_minutes' (fixed lenght for all intervals) keywords to specify the skim intervals.");
						//}
					}
				}

				set_parameter<bool>(document, "compare_with_historic_moe", compare_with_historic_moe<bool &>());
				set_parameter<std::string>(document, "historic_network_moe_file_path_name", historic_network_moe_file_path_name<std::string &>());
				set_parameter<std::string>(document, "historic_link_moe_file_path_name", historic_link_moe_file_path_name<std::string &>());
				set_parameter<bool>(document, "read_normal_day_link_moe", read_normal_day_link_moe<bool &>());
				set_parameter<std::string>(document, "normal_day_link_moe_file_path_name", normal_day_link_moe_file_path_name<std::string &>());
				set_parameter<std::string>(document, "historic_demand_moe_directory", historic_demand_moe_directory<std::string &>());
				set_parameter<bool>(document, "output_link_moe_for_assignment_interval", output_link_moe_for_assignment_interval<bool &>());
				set_parameter<bool>(document, "output_turn_movement_moe_for_assignment_interval", output_turn_movement_moe_for_assignment_interval<bool &>());
				set_parameter<bool>(document, "output_network_moe_for_assignment_interval", output_network_moe_for_assignment_interval<bool &>());
				set_parameter<bool>(document, "output_analzye_link_group_moe_for_assignment_interval", output_analzye_link_group_moe_for_assignment_interval<bool &>());
				set_parameter<bool>(document, "load_analyze_link_groups_from_file", load_analyze_link_groups_from_file<bool &>());
				set_parameter<std::string>(document, "analyze_link_groups_file_path_name", analyze_link_groups_file_path_name<std::string &>());

				//if (cfgReader.getParameter("DB_output_link_moe_for_assignment_interval", DB_output_link_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) DB_output_link_moe_for_assignment_interval<bool>(false);
				set_parameter<bool>(document, "write_ttime_distribution_from_network_model", write_ttime_distribution_from_network_model<bool &>());
				set_parameter<int>(document, "vehicle_trajectory_output_threshold", vehicle_trajectory_output_threshold<int &>());
				set_parameter<bool>(document, "use_tmc", use_tmc<bool &>());
				set_parameter<bool>(document, "use_network_events", use_network_events<bool &>());
				set_parameter<bool>(document, "jam_density_constraints_enforced", jam_density_constraints_enforced<bool &>());
				set_parameter<bool>(document, "maximum_flow_rate_constraints_enforced", maximum_flow_rate_constraints_enforced<bool &>());
				set_parameter<bool>(document, "vehicle_taking_action", vehicle_taking_action<bool &>());

				///enroute switching pretrip_informed_market_share
				set_parameter<double>(document, "pretrip_informed_market_share", pretrip_informed_market_share<double &>());
				set_parameter<double>(document, "realtime_informed_vehicle_market_share", realtime_informed_vehicle_market_share<double &>());
				set_parameter<double>(document, "information_compliance_rate_mean", information_compliance_rate_mean<double &>());
				set_parameter<double>(document, "information_compliance_rate_standard_deviation", information_compliance_rate_standard_deviation<double &>());
				set_parameter<double>(document, "relative_indifference_band_route_choice_mean", relative_indifference_band_route_choice_mean<double &>());
				set_parameter<double>(document, "minimum_travel_time_saving_mean", minimum_travel_time_saving_mean<double &>()); // in minutes
				set_parameter<double>(document, "minimum_travel_time_saving_standard_deviation", minimum_travel_time_saving_standard_deviation<double &>()); // in minutes
				set_parameter<bool>(document, "enroute_switching_enabled", enroute_switching_enabled<bool &>());
				set_parameter<bool>(document, "use_realtime_travel_time_for_enroute_switching", use_realtime_travel_time_for_enroute_switching<bool &>());
				set_parameter<double>(document, "minimum_delay_ratio_for_enroute_switching", minimum_delay_ratio_for_enroute_switching<double &>());
				set_parameter<double>(document, "minimum_delay_seconds_for_enroute_switching", minimum_delay_seconds_for_enroute_switching<double &>());
				set_parameter<bool>(document, "enroute_switching_on_excessive_delay", enroute_switching_on_excessive_delay<bool &>());
				set_parameter<bool>(document, "multimodal_network_input", multimodal_network_input<bool &>());
				set_parameter<double>(document, "enroute_excessive_delay_factor", enroute_excessive_delay_factor<double &>());
				set_parameter<double>(document, "minimum_seconds_from_arrival_for_enroute_switching", minimum_seconds_from_arrival_for_enroute_switching<double &>());
				set_parameter<bool>(document, "time_dependent_routing", time_dependent_routing<bool &>());
				set_parameter<double>(document, "time_dependent_routing_weight_shape", time_dependent_routing_weight_shape<double &>());
				set_parameter<double>(document, "time_dependent_routing_weight_scale", time_dependent_routing_weight_scale<double &>());
				set_parameter<double>(document, "time_dependent_routing_weight_factor", time_dependent_routing_weight_factor<double &>());
				set_parameter<bool>(document, "multimodal_routing", multimodal_routing<bool &>());
				set_parameter<double>(document, "accident_event_duration_reduction", accident_event_duration_reduction<double &>());
				set_parameter<bool>(document, "calculate_realtime_moe", calculate_realtime_moe<bool &>());
				
				if (set_parameter<std::string>(document, "buildings_geometry_file", buildings_geometry_file<std::string &>()))
				{
					use_buildings(true);
				}
				else
				{
					use_buildings(false);
					buildings_geometry_file<string&>()="";
				}

				set_parameter<std::string>(document, "mode_choice_model_file", mode_choice_model_file<std::string &>());
				set_parameter<std::string>(document, "destination_choice_model_file", destination_choice_model_file<std::string &>());
				set_parameter<std::string>(document, "telecommute_choice_model_file", telecommute_choice_model_file<std::string &>());
				set_parameter<std::string>(document, "cav_wtp_model_file", cav_wtp_model_file<std::string &>());

				if (multimodal_routing<bool>()) set_parameter<std::string>(document, "multimodal_routing_model_file", multimodal_routing_model_file<std::string &>());
				set_parameter<std::string>(document, "timing_choice_model_file", timing_choice_model_file<std::string &>());

				//output_dir_name<string&>() = "";
				input_dir_name<string&>() = "";
				open_output_files<NULLTYPE>();
				open_input_files<NULLTYPE>();

				//-----------------------
				// copy scenario file
				string scenario_out_name = output_dir_name<string>().append(filename);

				std::ifstream src(filename,std::ios::binary);
				std::ofstream dst(scenario_out_name,std::ios::binary);
				dst<<src.rdbuf();

				src.close();
				dst.close();
			}

			template<typename TargetType> void open_output_files()
			{
				//===================================================
				// manage output directory
				//---------------------------------------------------
				int count = 1;
				stringstream dir_id("");
				string temp_dir_name = output_dir_name<string>();
			#ifdef _WIN32		
				while (_mkdir(temp_dir_name.c_str())==-1)
			#else
				while (mkdir(temp_dir_name.c_str(), 0777)==-1)
			#endif
				{
					dir_id.str(""); 
					count++;
					dir_id<<count; 
					temp_dir_name = output_dir_name<string>();
					temp_dir_name.append(dir_id.str().c_str()); 
				}		
				output_dir_name<string>(temp_dir_name);
				output_dir_name<string&>().append("//");
				//---------------------------------------------------

				string out_skim_path = output_dir_name<string>().append(this->output_highway_skim_file_path_name<string&>());
				this->output_highway_skim_file_path_name<string&>(out_skim_path);
				out_skim_path = output_dir_name<string>().append(this->output_highway_cost_skim_file_path_name<string&>());
				this->output_highway_cost_skim_file_path_name<string&>(out_skim_path);
				out_skim_path = output_dir_name<string>().append(this->output_transit_skim_file_path_name<string&>());
				this->output_transit_skim_file_path_name<string&>(out_skim_path);


				//----------------------
				// results database
				string results_name = output_dir_name<string>().append(this->database_name<string&>());
				unique_ptr<odb::database> db(create_sqlite_database(results_name, polaris::io::db_inventory[1]));
				this->output_results_database_name(polaris::io::make_name(results_name, polaris::io::db_inventory[1]));
				odb::transaction t(db->begin());
				t.commit();
				shared_ptr<odb::database> db_ptr = open_sqlite_database_single<shared_ptr<odb::database> > (output_results_database_name<string>());				
				this->template result_db_ptr<shared_ptr<odb::database>>(db_ptr);

				//----------------------
				// demand database
				// copy default demand tables - vehicle type, class....
				string name(this->database_name<string&>());
				unique_ptr<odb::database> db_demand(open_sqlite_database<unique_ptr<odb::database> > (name));
				odb::transaction td(db_demand->begin());

				///// ---- COPY QUERY RESULTS FROM ORIGINAL DEMAND DATABASE INTO TEMP STRUCTURE ---- CAN"T HAVE 2 TRANSACTIONS OPEN AT SAME TIME.....
				vector<shared_ptr<polaris::io::Automation_Type>> a_vec;
				vector<shared_ptr<polaris::io::Connectivity_Type>> c_vec;
				vector<shared_ptr<polaris::io::Fuel_Type>> f_vec;
				vector<shared_ptr<polaris::io::Powertrain_Type>> p_vec;
				vector<shared_ptr<polaris::io::Vehicle_Class>> v_vec;
				vector<shared_ptr<polaris::io::Vehicle_Type>> vt_vec;

				odb::result<polaris::io::Automation_Type> a_result = db_demand->query<polaris::io::Automation_Type>(odb::query<polaris::io::Automation_Type>::true_expr);
				odb::result<polaris::io::Connectivity_Type> c_result = db_demand->query<polaris::io::Connectivity_Type>(odb::query<polaris::io::Connectivity_Type>::true_expr);
				odb::result<polaris::io::Fuel_Type> f_result = db_demand->query<polaris::io::Fuel_Type>(odb::query<polaris::io::Fuel_Type>::true_expr);
				odb::result<polaris::io::Powertrain_Type> p_result = db_demand->query<polaris::io::Powertrain_Type>(odb::query<polaris::io::Powertrain_Type>::true_expr);
				odb::result<polaris::io::Vehicle_Class> v_result = db_demand->query<polaris::io::Vehicle_Class>(odb::query<polaris::io::Vehicle_Class>::true_expr);
				odb::result<polaris::io::Vehicle_Type> vt_result = db_demand->query<polaris::io::Vehicle_Type>(odb::query<polaris::io::Vehicle_Type>::true_expr);

				for (typename odb::result<polaris::io::Automation_Type>::iterator a_itr = a_result.begin(); a_itr != a_result.end(); ++a_itr) a_vec.push_back(a_itr.load());		
				for (typename odb::result<polaris::io::Connectivity_Type>::iterator c_itr = c_result.begin(); c_itr != c_result.end(); ++c_itr) c_vec.push_back(c_itr.load());
				for (typename odb::result<polaris::io::Fuel_Type>::iterator f_itr = f_result.begin(); f_itr != f_result.end(); ++f_itr) f_vec.push_back(f_itr.load());
				for (typename odb::result<polaris::io::Powertrain_Type>::iterator p_itr = p_result.begin(); p_itr != p_result.end(); ++p_itr) p_vec.push_back(p_itr.load());
				for (typename odb::result<polaris::io::Vehicle_Class>::iterator v_itr = v_result.begin(); v_itr != v_result.end(); ++v_itr) v_vec.push_back(v_itr.load());
				for (typename odb::result<polaris::io::Vehicle_Type>::iterator vt_itr = vt_result.begin(); vt_itr != vt_result.end(); ++vt_itr) vt_vec.push_back(vt_itr.load());
				td.commit();

				// Create demand database
				unique_ptr<odb::database> db2(create_sqlite_database(results_name, polaris::io::db_inventory[2]));
				this->output_demand_database_name(polaris::io::make_name(results_name, polaris::io::db_inventory[2]));
				odb::transaction t2(db2->begin());
				for (typename vector<shared_ptr<polaris::io::Automation_Type>>::iterator a_itr = a_vec.begin(); a_itr != a_vec.end(); ++a_itr) db2->persist(*a_itr);
				for (typename vector<shared_ptr<polaris::io::Connectivity_Type>>::iterator c_itr = c_vec.begin(); c_itr != c_vec.end(); ++c_itr) db2->persist(*c_itr);
				for (typename vector<shared_ptr<polaris::io::Fuel_Type>>::iterator f_itr = f_vec.begin(); f_itr != f_vec.end(); ++f_itr) db2->persist(*f_itr);
				for (typename vector<shared_ptr<polaris::io::Powertrain_Type>>::iterator p_itr = p_vec.begin(); p_itr != p_vec.end(); ++p_itr) db2->persist(*p_itr);
				for (typename vector<shared_ptr<polaris::io::Vehicle_Class>>::iterator v_itr = v_vec.begin(); v_itr != v_vec.end(); ++v_itr) db2->persist(*v_itr);
				for (typename vector<shared_ptr<polaris::io::Vehicle_Type>>::iterator vt_itr = vt_vec.begin(); vt_itr != vt_vec.end(); ++vt_itr) db2->persist(*vt_itr);
				t2.commit();	
				shared_ptr<odb::database> db2_ptr = open_sqlite_database_single<shared_ptr<odb::database> >(output_demand_database_name<string>());
				this->template demand_db_ptr<shared_ptr<odb::database>>(db2_ptr);

				//----------------------
				// synthetic population database
				results_name = output_dir_name<string>().append(this->database_name<string&>());
				unique_ptr<odb::database> db3(create_sqlite_database(results_name, polaris::io::db_inventory[5]));
				this->output_popsyn_database_name(polaris::io::make_name(results_name, polaris::io::db_inventory[5]));
				odb::transaction t3(db3->begin());
				t3.commit();
				shared_ptr<odb::database> db3_ptr = open_sqlite_database_single<shared_ptr<odb::database> >(output_popsyn_database_name<string>());
				this->template popsyn_db_ptr<shared_ptr<odb::database>>(db3_ptr);


				//vehicle trajectory
				//if (this->template write_vehicle_trajectory<bool>())
				//{
				//	vehicle_trajectory_file_name<string&>().assign(output_dir_name<string&>() + "vehicle_trajectory.csv");
				//	vehicle_trajectory_file<fstream&>().open(vehicle_trajectory_file_name<string&>(),fstream::out);
				//	if(vehicle_trajectory_file<fstream&>().is_open()) 
				//	{ 
				//		//vehicle_trajectory_file<fstream&>() 
				//		//	<< "vehicle" <<  ","
				//		//	<< "origin_zone " <<  ","
				//		//	<< "destination_zone" << ","
				//		//	<< "origin_activity_location" << ","
				//		//	<< "destination_activity_location" << ","
				//		//	<< "origin_link" << ","
				//		//	<< "destination_link" << ","
				//		//	<< "num_links" << ","
				//		//	<< "departure_time" << ","
				//		//	<< "arrival_time" << ","
				//		//	<< "travel_time" << ","
				//		//	<< "routed_travel_time" << ","
				//		//	<< "travel_time_ratio" << ","
				//		//	<< "trip_length" << ","
				//		//	<< "num_switches" << ","
				//		//	<< "loading_delay" << ","
				//		//	<< "entry_queue_length" << ","
				//		//	<<endl;

				//		vehicle_trajectory_file<fstream&>() 
				//			<< "vehicle" <<  ","
				//			<< "link_number" <<  ","
				//			<< "link_id" <<  ","
				//			<< "link_dir" << ","
				//			<< "entering_time" << ","
				//			<< "travel_time" << ","
				//			<< "start_position" << ","
				//			<< "length" << ","
				//			//<< "delayed_time"<< ","
				//			<< "actual_speed"<< ","
				//			<< "free_flow_speed"<< ","
				//			<< "stopped_time"<<","
				//			<< "stop_position"
				//			<<endl;
				//	}
				//	else
				//	{
				//		cout << "Cannot open file - "
				//			<< vehicle_trajectory_file_name<string&>()
				//			<< endl;
				//	}
				//}

				//routed path
				//routed_path_file_name<string&>().assign(output_dir_name<string&>() + "routed_path.csv");
				//routed_path_file<fstream&>().open(routed_path_file_name<string&>(),fstream::out);
				//if(routed_path_file<fstream&>().is_open()) 
				//{ 
				//	routed_path_file<fstream&>() 
				//		<< "vehicle" <<  ","
				//		<< "origin_zone " <<  ","
				//		<< "destination_zone" << ","
				//		<< "origin_activity_location" << ","
				//		<< "destination_activity_location" << ","
				//		<< "origin_link" << ","
				//		<< "destination_link" << ","
				//		<< "num_links" << ","
				//		<< "departure_time" << ","
				//		<< "arrival_time" << ","
				//		<< "travel_time"
				//		<<endl;

				//	routed_path_file<fstream&>() 
				//		<< "link_number" <<  ","
				//		<< "link_id " <<  ","
				//		<< "entering_time" << ","
				//		<< "travel_time" << ","
				//		<< "delayed_time"
				//		<<endl;
				//}
				//else
				//{
				//	cout << "Cannot open file - "
				//		<< vehicle_trajectory_file_name<string&>()
				//		<< endl;
				//}

				//link flow pattern
				if (this->template write_network_link_flow<bool>())
				{
					network_link_flow_file_name<string&>().assign(output_dir_name<string&>() + "network_link_flow.csv");
					network_link_flow_file<fstream&>().open(network_link_flow_file_name<string&>(),fstream::out);

					if(network_link_flow_file<fstream&>().is_open())
					{
						network_link_flow_file<fstream&>() 
							<< "time" <<  ","
							<< "link" <<  ","
							<< "fftt" << ","
							<< "bwtt" << ","
							<< "origin_A" << ","
							<< "origin_D" << ","
							<< "destination_A" << ","
							<< "upstream_A" << ","
							<< "upstream_D" << ","
							<< "downstream_A" << ","
							<< "downstream_D" << ","
							<< "queue_length" 
							<<endl;
					}
					else
					{
						cout << "Cannot open file - "
							<< network_link_flow_file_name<string&>()
							<< endl;
	
					}
				}

				//link turn times
				if (this->template write_network_link_turn_time<bool>())
				{
					network_link_turn_time_file_name<string&>() = output_dir_name<string&>() + "network_link_turn_time.csv";
					network_link_turn_time_file<fstream&>().open(network_link_turn_time_file_name<string&>(),fstream::out);

					if(network_link_turn_time_file<fstream&>().is_open())
					{
						network_link_turn_time_file<fstream&>() 
							<< "clock" <<  ","
							<< "time" <<  ","
							<< "node" << ","
							<< "turn_movement" <<  ","
							<< "inbound_link" <<  ","
							<< "outbound_link" <<  ","
							<< "fftt" << ","
							<< "inbound_link_fftt" << ","
							<< "inbound_link_supply" << ","
							<< "outbound_link_supply" << ","
							<< "inbound_link_origin_arrived_vehicles" << ","
							<< "outbound_link_origin_arrived_vehicles" << ","
							<< "inbound_link_origin_departed_vehicles" << ","
							<< "outbound_link_origin_departed_vehicles" << ","
							<< "turn_penalty"  << ","
							<< "green_time" << ","
							<< "capacity" << ","
							<< "demand" << ","
							<< "supply" << ","
							<< "flow" << ","
							<< "transfered_veicles" << ","
							<< "avg_turn_penalty"  << ","
							<< "avg_link_turn_time"
							<<endl;
					}
					else
					{
						cout << "Cannot open file - "
							<< network_link_turn_time_file_name<string&>()
							<< endl;
	
					}
				}


				//operation control
				if (this->template write_node_control_state<bool>())
				{
					network_node_control_state_file_name<string&>() = output_dir_name<string&>() + "node_control_state.csv";
					network_node_control_state_file<fstream&>().open(network_node_control_state_file_name<string&>(),fstream::out);
					if(network_node_control_state_file<fstream&>().is_open()) 
					{ 
						network_node_control_state_file<fstream&>() 
							<< "time" <<  ","
							<< "sim_int" <<  ","
							<< "sim_time" << ","
							<< "node" << ","
							<< "control_plan_id" << ","
							<< "control_time" << ","
							<< "cp_s_time" << ","
							<< "cp_e_time" << ","
							<< "n_approaches" << ","
							<< "cycle_index" << ","
							<< "cycle_length" << ","
							<< "c_s_time" << ","
							<< "c_e_time" << ","
							<< "n_phases" << ","
							<< "phase" << ","
							<< "green_s_time" << ","
							<< "yellow_s_time" << ","
							<< "red_s_time" << ","
							<< "phase_end_time" << ","
							<< "phase" << ","
							<< "green_s_time" << ","
							<< "yellow_s_time" << ","
							<< "red_s_time" << ","
							<< "phase_end_time" << ","
							<< "phase" << ","
							<< "green_s_time" << ","
							<< "yellow_s_time" << ","
							<< "red_s_time" << ","
							<< "phase_end_time" << ","
							<< "phase" << ","
							<< "green_s_time" << ","
							<< "yellow_s_time" << ","
							<< "red_s_time" << ","
							<< "phase_end_time"
							<<endl;
					}
					else
					{
						cout << "Cannot open file - "
							<< network_node_control_state_file_name<string&>()
							<< endl;
					}
				}

				//operation control
				output_summary_file_name<string&>() = output_dir_name<string&>() + "summary.csv";
				output_summary_file<fstream&>().open(output_summary_file_name<string&>(),fstream::out);
				if(output_summary_file<fstream&>().is_open()) 
				{ 
					output_summary_file<fstream&>() 
						<< "time" <<  ","
						<< "loaded" <<  ","
						<< "departed" << ","
						<< "arrived"  << ","
						<< "in_network" << ","
						<< "switched" << ","
						//<< "assignment_time_in_seconds" << ","
						//<< "simulation_time_in_seconds" << ","
						//<< "operation_time_in_seconds" << ","
						//<< "output_time_in_seconds"
						<< "VMT" << ","
						<< "VHT" << ","
						<< "avg_travel_time" << ","
						<< "delay_inducted_switches" << ","
						<< "realtime_inducted_switches" << ","
						<< "its_inducted_switches" << ","
						<< "wallclock_time" << ","
						<< "simulated_time" << ","
						<< "physical_memory_usage" << ","
						<< "physical_memory_percentage" 
						<<endl;
				}
				else
				{
					cout << "Cannot open file - "
						<< output_summary_file_name<string&>()
						<< endl;
				}

				//vehicle_transfer
				/*if (this->template write_vehicle_transfer_file<bool>())
				{
					vehicle_transfer_file_name<string&>() = output_dir_name<string&>() + "vehicle_transfer.csv";
					vehicle_transfer_file<fstream&>().open(vehicle_transfer_file_name<string&>(),fstream::out);
					if(vehicle_transfer_file<fstream&>().is_open()) 
					{ 
						vehicle_transfer_file<fstream&>() 
							<< "clock" << ","
							<< "time" <<  ","
							<< "node" << ","
							<< "turn_movement" <<  ","
							<< "inbound_link" <<  ","
							<< "outbound_link" <<  ","
							<< "link_supply"<< ","
							<< "upstream_arrived_vehicles" << ","
							<< "num_departed_vehicles" << ","
							<< "num_link_origin_departed_vehicles_allowed" << ","
							<< "link_origin_arrived_vehicles" << ","
							<< "link_origin_departed_vehicles" << ","
							<< "cumulative_arrival_vehicles" << ","
							<< "vehicle_id" << ","
							<< "vehicle_index" << ","
							<< "path_size" << ","
							<< "current_pos" << ","
							<< "links" << ","
							<<endl;
					}
					else
					{
						cout << "Cannot open file - "
							<< vehicle_transfer_file_name<string&>()
							<< endl;
					}
				}*/

				//real-time moe
				if (this->template output_network_moe_for_simulation_interval<bool>())
				{
					string out_realtime_network_moe_file_name = output_dir_name<string&>() + "realtime_moe_network.csv";
					out_realtime_network_moe_file<fstream&>().open(out_realtime_network_moe_file_name, fstream::out);
					out_realtime_network_moe_file<fstream&>() << "clock,time,num_loaded_vehicle,num_departed_vehicle,num_arrived_vehicle,avg_link_time_in_min,avg_link_speed_in_mph,avg_link_density_in_vpmpl,avg_link_in_volume,avg_link_out_volume,avg_link_time_ratio,avg_link_speed_ratio,avg_link_density_ratio,avg_link_queue_length,network_vmt,network_vht,network_cumulative_loaded_vehicles,network_cumulative_departed_vehicles,network_in_network_vehicles,network_cumulative_arrived_vehicles\n";
				}
				//link
				if (this->template output_link_moe_for_simulation_interval<bool>())
				{
					string out_realtime_link_moe_file_name = output_dir_name<string&>() + "realtime_moe_link.csv";
					out_realtime_link_moe_file<fstream&>().open(out_realtime_link_moe_file_name, fstream::out);
					out_realtime_link_moe_file<fstream&>() << "clock,time,link,dbid,direction,unode,dnode,link_type,travel_time_in_min,travel_delay_in_min,queue_length,speed_in_mph,density_in_vpmpl,in_volume,out_volume,travel_time_ratio,speed_ratio,density_ratio\n";
				}
				//movement
				if (this->template output_turn_movement_moe_for_simulation_interval<bool>())
				{
					string out_realtime_movement_moe_file_name = output_dir_name<string&>() + "realtime_moe_movement.csv";
					out_realtime_movement_moe_file<fstream&>().open(out_realtime_movement_moe_file_name, fstream::out);
					out_realtime_movement_moe_file<fstream&>() << "clock,time,turn_movement,inbound_link,outbound_link,node,turn_penalty_in_min,turn_penalty_sd_in_min,inbound_link_turn_time_in_min,outbound_link_turn_time_in_min,movement_flow_rate_in_vphpl\n";
				}
				//moe
				//network
				if (this->template output_network_moe_for_assignment_interval<bool>())
				{
					string out_network_moe_file_name = output_dir_name<string&>() + "moe_network.csv";
					out_network_moe_file<fstream&>().open(out_network_moe_file_name, fstream::out);
					//out_network_moe_file<fstream&>() << "clock,time,num_loaded_vehicle,num_departed_vehicle,num_arrived_vehicle,avg_link_time_in_min,avg_link_speed_in_mph,avg_link_density_in_vpmpl,avg_link_in_flow_rate_in_vphpl,avg_link_out_flow_rate_in_vphpl,avg_link_in_volume,avg_link_out_volume,avg_link_time_ratio,avg_link_speed_ratio,avg_link_density_ratio,avg_link_in_flow_ratio,avg_link_out_flow_ratio,vht,vmt,assignment_calculation_time_in_second,simulation_calculation_time_in_second,operation_calculation_time_in_second,output_calculation_time_in_second\n";
					out_network_moe_file<fstream&>() << "clock,time,num_loaded_vehicle,num_departed_vehicle,num_arrived_vehicle,avg_link_time_in_min,avg_link_speed_in_mph,avg_link_density_in_vpmpl,avg_link_in_flow_rate_in_vphpl,avg_link_out_flow_rate_in_vphpl,avg_link_in_volume,avg_link_out_volume,avg_link_time_ratio,avg_link_speed_ratio,avg_link_density_ratio,avg_link_in_flow_ratio,avg_link_out_flow_ratio,vmt,vht\n";
				}
				//link
				if (this->template output_link_moe_for_assignment_interval<bool>())
				{
					string out_link_moe_file_name = output_dir_name<string&>() + "moe_link.csv";
					out_link_moe_file<fstream&>().open(out_link_moe_file_name, fstream::out);
					out_link_moe_file<fstream&>() << "clock,time,link,dbid,direction,unode,dnode,link_type,travel_time_in_min,travel_time_sd_in_min,travel_delay_in_min,travel_delay_sd_in_min,queue_length,speed_in_mph,density_in_vpmpl,in_flow_rate_in_vphpl,out_flow_rate_in_vphpl,in_volume,out_volume,travel_time_ratio,speed_ratio,density_ratio,in_flow_ratio,out_flow_ratio,vht,vmt\n";
				}
				//movement
				if (this->template output_turn_movement_moe_for_assignment_interval<bool>())
				{
					string out_movement_moe_file_name = output_dir_name<string&>() + "moe_movement.csv";
					out_movement_moe_file<fstream&>().open(out_movement_moe_file_name, fstream::out);
					out_movement_moe_file<fstream&>() << "clock,time,turn_movement,inbound_link,outbound_link,node,turn_penalty_in_min,turn_penalty_sd_in_min,inbound_link_turn_time_in_min,outbound_link_turn_time_in_min,movement_flow_rate_in_vphpl\n";
				}
				//routable network snapshot
				if (this->template write_network_snapshots<bool>())
				{
					string routable_network_snapshots_file_name = output_dir_name<string&>() + "output_network_snapshots";
					output_network_snapshots_file<fstream&>().open(routable_network_snapshots_file_name, fstream::out);
					output_network_snapshots_file<fstream&>() << "time\t maximum_free_flow_speed" << endl;
					output_network_snapshots_file<fstream&>() << "inbound_link_uuid\t inbound_link_dbid\t inbound_link_direction\t inbound_link_travel_time\t number_of_movements" << endl;
					output_network_snapshots_file<fstream&>() << "movement_uuid\t movement_forward_link_turn_travel_time";
				}
				if (write_ttime_distribution_from_network_model<bool>())
				{
					if(vehicle_transfer_file<fstream&>().is_open()) 
					{ 
						string ttime_distribution_file_name = output_dir_name<string&>() + "network_ttime_distribution.csv";
						ttime_distribution_file<fstream&>().open(ttime_distribution_file_name, fstream::out);
						if(ttime_distribution_file<fstream&>().is_open()) 
						{
							int num_epoches = 24*60/5 + 1;
							
							ttime_distribution_file<fstream&>() << "TIME(s)";
							for(int i = 1; i <= num_epoches; i++)
							{
								ttime_distribution_file<fstream&>() << "," << i * 5;
							}
							ttime_distribution_file<fstream&>() << endl;
						}
						else
						{
							cout << "Cannot open file - "
								<< ttime_distribution_file_name
								<< endl;
						}
					}
				}
			};

			template<typename TargetType> void open_input_files()
			{
				//reference network moe file
				if (compare_with_historic_moe<bool>())
				{
					reference_realtime_network_moe_file<fstream&>().open(historic_network_moe_file_path_name<string&>(), fstream::in);
					if (!reference_realtime_network_moe_file<fstream&>().is_open())
					{
						THROW_EXCEPTION(endl << "compare_with_historic_moe is enabled but reference network MOE file " << historic_network_moe_file_path_name<string&>() << " cannot be openned." << endl);
					}

					historic_link_moe_file<fstream&>().open(historic_link_moe_file_path_name<string&>(), fstream::in);
					if (!historic_link_moe_file<fstream&>().is_open())
					{
						THROW_EXCEPTION(endl << "compare_with_historic_moe is enabled but reference link MOE file " << historic_link_moe_file_path_name<string&>() << " cannot be openned." << endl);
					}
					else
					{
						string aLine;
						getline(historic_link_moe_file<fstream&>(),aLine); // skip the first line
					}
				}
				if (read_network_snapshots<bool>() || routing_with_snapshots<bool>())
				{
					input_network_snapshots_file<fstream&>().open(input_network_snapshots_file_path_name<string&>(), fstream::in);
					if (!input_network_snapshots_file<fstream&>().is_open())
					{
						THROW_EXCEPTION(endl << "read_network_snapshots/routing_with_snapshots is enabled but network snapshots file " << input_network_snapshots_file_path_name<string&>() << " cannot be opened." << endl);
					}
				}
				if (read_normal_day_link_moe<bool>())
				{
					normal_day_link_moe_file<fstream&>().open(normal_day_link_moe_file_path_name<string&>(), fstream::in);
					if (!normal_day_link_moe_file<fstream&>().is_open())
					{
						THROW_EXCEPTION(endl << "read_normal_day_link_moe is enabled but reference link MOE file " << normal_day_link_moe_file_path_name<string&>() << " cannot be openned." << endl);
					}
					else
					{
						string aLine;
						getline(normal_day_link_moe_file<fstream&>(),aLine); // skip the first line
					}
				}
				if (load_analyze_link_groups_from_file<bool>())
				{
					analyze_link_groups_file<fstream&>().open(analyze_link_groups_file_path_name<string&>(), fstream::in);
					if (!analyze_link_groups_file<fstream&>().is_open())
					{
						THROW_EXCEPTION(endl << "load_analyze_link_groups_from_file is enabled but analyze link groups file " << analyze_link_groups_file_path_name<string&>() << " cannot be openned." << endl);
					}
				}
			}

			template<typename TargetType> void close_files()
			{
				close_output_files<NULLTYPE>();
				close_input_files<NULLTYPE>();
			}

			template<typename TargetType> void close_output_files()
			{
				vehicle_trajectory_file<fstream&>().close();
				routed_path_file<fstream&>().close();
				network_link_flow_file<fstream&>().close();
				network_link_turn_time_file<fstream&>().close();
				network_node_control_state_file<fstream&>().close();
				output_summary_file<fstream&>().close();
				vehicle_transfer_file<fstream&>().close();

				out_realtime_link_moe_file<fstream&>().close();
				out_realtime_movement_moe_file<fstream&>().close();
				out_realtime_network_moe_file<fstream&>().close();

				out_link_moe_file<fstream&>().close();
				out_movement_moe_file<fstream&>().close();
				out_network_moe_file<fstream&>().close();

				output_network_snapshots_file<fstream&>().close();

				if (write_ttime_distribution_from_network_model<bool>()) ttime_distribution_file<fstream&>().close();
			};

			template<typename TargetType> void close_input_files()
			{
				reference_realtime_network_moe_file<fstream&>().close();
				input_network_snapshots_file<fstream&>().close();
			}


			template<typename TargetType> void increase_network_cumulative_loaded_vehicles()
			{
				this_component()->template increase_network_cumulative_loaded_vehicles<TargetType>();
			}

			template<typename TargetType> void increase_network_cumulative_departed_vehicles()
			{
				this_component()->template increase_network_cumulative_departed_vehicles<TargetType>();
			}

			template<typename TargetType> void increase_network_in_network_vehicles()
			{
				this_component()->template increase_network_in_network_vehicles<TargetType>();
			}

			template<typename TargetType> void increase_network_in_system_vehicles()
			{
				this_component()->template increase_network_in_system_vehicles<TargetType>();
			}

			template<typename TargetType> void decrease_network_in_network_vehicles()
			{
				this_component()->template decrease_network_in_network_vehicles<TargetType>();
			}

			template<typename TargetType> void decrease_network_in_system_vehicles()
			{
				this_component()->template decrease_network_in_system_vehicles<TargetType>();
			}

			template<typename TargetType> void increase_network_cumulative_switched_decisions(int cause_for_switching)
			{
				this_component()->template increase_network_cumulative_switched_decisions<TargetType>(cause_for_switching);
			}

			template<typename TargetType> void increase_network_cumulative_arrived_vehicles(float travel_time)
			{
				this_component()->template increase_network_cumulative_arrived_vehicles<TargetType>(travel_time);
			}

			//template<typename TargetType> void output(string s)
			//{
			//	typedef typename ComponentType::output_writer_type OutputWriterType;
			//	//output_writer<OutputWriterType&>() << s;
			//	cout << s;
			//	cout.flush();
			//}

			// convert from meter to foot
			template<typename TargetType> float meterToFoot(float meterValue)
			{
				// 1 meter = 3.28084 feet
				return 3.28084 * meterValue;
			}

			// convert from meters per second to miles per hour
			template<typename TargetType> float mepsToMiph(float mepsValue)
			{
				// 1 meter = 0.000621371 mile
				return (3.28084 * mepsValue * 3600.0 / 5280.0);
			}

			template<typename TargetType> int moe_update_interval()
			{
				return assignment_interval_length<int>();
			}

			bool print_error_msg(rapidjson::Document& document, std::string json_file)
			{
				// Get the parse error and offset
				rapidjson::ParseErrorCode parse_error = document.GetParseError();
				size_t parse_offset = document.GetErrorOffset();

				cout << "ERROR: ";

				if (parse_error == rapidjson::kParseErrorDocumentEmpty)   // Empty document
				{
					cout << "JSON file \'" << json_file << "\'" << " is empty" << endl;
					return false;
				}

				switch (parse_error)
				{
				case (rapidjson::kParseErrorDocumentRootNotSingular):
					cout << "JSON roots error";
					break;
				case (rapidjson::kParseErrorValueInvalid):
					cout << "Invalid value";
					break;
				case (rapidjson::kParseErrorObjectMissName):
					cout << "Object member is missing a name";
					break;
				case (rapidjson::kParseErrorObjectMissColon):
					cout << "Object member is missing a colon after name";
					break;
				case (rapidjson::kParseErrorObjectMissCommaOrCurlyBracket):
					cout << "Object member is missing a comma or \'}\'";
					break;
				case (rapidjson::kParseErrorArrayMissCommaOrSquareBracket):
					cout << "Array element is missing a comma or \']\'";
					break;
				case (rapidjson::kParseErrorStringUnicodeEscapeInvalidHex):
					cout << "String has incorrect hex digit after escape";
					break;
				case (rapidjson::kParseErrorStringUnicodeSurrogateInvalid):
					cout << "String has an invalid surrogate pair";
					break;
				case (rapidjson::kParseErrorStringEscapeInvalid):
					cout << "String has an invalid escape character";
					break;
				case (rapidjson::kParseErrorStringMissQuotationMark):
					cout << "String is missing a closing quotation mark";
					break;
				case (rapidjson::kParseErrorStringInvalidEncoding):
					cout << "String has invalid encoding";
					break;
				case (rapidjson::kParseErrorNumberTooBig):
					cout << "Number is too big to be stored as double";
					break;
				case (rapidjson::kParseErrorNumberMissFraction):
					cout << "Number is missing the fraction part";
					break;
				case (rapidjson::kParseErrorNumberMissExponent):
					cout << "Number is missing the exponent part";
					break;
				case (rapidjson::kParseErrorTermination):
					cout << "Rapid JSON parsing terminated";
					break;
				case (rapidjson::kParseErrorUnspecificSyntaxError):
					cout << "Unspecific syntax error";
					break;
				default:
					cout << "Unknown";
				}
				cout << " starting at " << parse_offset << endl;

				return false;
			}

			int find_line_begin(std::ifstream& json_string, int start_pos)
			{
				int pos = start_pos;
				const int move_size = 5;
				char buffer[move_size];

				// Look for the beginning of the line that includes the given position
				while (true)
				{
					// Move back from prev_pos
					pos -= move_size;

					// Check that position is a positive value
					if (pos > 0)
					{
						json_string.seekg(pos);

						// Read bytes
						json_string.read(buffer, move_size);

						// Look for a newline byte, which terminates previous line
						int eol_pos;
						for (eol_pos = sizeof(buffer) - 1; eol_pos >= 0; --eol_pos)
						{
							if (buffer[eol_pos] == '\n')
							{
								break;
							}
						}

						// If found newline or got to beginning of file - done looking
						if (eol_pos >= 0 || pos < move_size)
						{
							pos += eol_pos + 1;
							break;
						}
					}
					else
					{
						pos = 0;
						break;
					}
				}
				return pos;
			}

			//bool parse_option_file(rapidjson::Document& document, std::string option_file)
			bool parse_option_file(Scenario_Components::Types::ScenarioData& scenario, std::string option_file)
			{
				bool ret_val = true;

				// Check file name exists
				if (option_file.length() < 1)
				{
					cout << "Warning: option file was not specified" << endl;
					return true;
				}

				// Open JSON file
				std::ifstream ifs(option_file);
				if (!ifs.good())
				{
					cout << "ERROR: unbale to open option file \'" << option_file << "\'" << endl;
					return false;
				}

				// Convert std::ifstream to RapidJSON input stream
				rapidjson::IStreamWrapper isw(ifs);

				// Parse and check for errors
				if (scenario.document.ParseStream(isw).HasParseError())
				{
					ret_val = print_error_msg(scenario.document, option_file);

					// Find size of file
					std::streamoff json_len;
					ifs.clear();
					ifs.seekg(0, ifs.end);
					json_len = ifs.tellg();

					// check if file has content
					if (json_len > 0)
					{
						// print text around error location
						int print_len = 40;
						std::string json_string;
						json_string.resize(print_len);

						// check if file length is less than amount to print
						if (json_len < print_len)
						{
							ifs.seekg(0, ifs.beg);
						}
						else
						{
							size_t half_len = (size_t)(0.5*(float)print_len);
							int start_index, line_index;
							size_t parse_offset = scenario.document.GetErrorOffset();

							// determine start index for printing
							if (parse_offset < half_len)
							{
								line_index = 0;
							}
							else if (parse_offset > (json_len - half_len))
							{
								start_index = (int)json_len - print_len;
								ifs.seekg(start_index);
								line_index = find_line_begin(ifs, start_index);
							}
							else
							{
								start_index = parse_offset - half_len;
								ifs.seekg(start_index);
								line_index = find_line_begin(ifs, start_index);
							}
							ifs.seekg(line_index);
						}
						// print
						for (int cntr = 0; cntr < 3; cntr++)
						{
							std::getline(ifs, json_string);
							cout << json_string << endl;
						}
					}
					return ret_val;
				}

				if (!scenario.document.IsObject())
				{
					cout << "ERROR: \'" << option_file << "\' is not a valid option file" << endl;
					return false;
				}

				get_KV_paths(scenario.key_paths, scenario.document, "");

				return true;
			}

			std::vector<std::string> split_section(const char *str, char c = '/')
			{
				std::vector<std::string> result;

				do
				{
					const char* begin = str;

					while (*str != c && *str)
					{
						str++;
					}

					result.push_back(std::string(begin, str));
				} while (0 != *str++);

				return result;
			}

			template <class T>
			bool set_parameter(Scenario_Components::Types::ScenarioData& scenario, const std::string& key, T& parameter)
			{
				std::string section;
				rapidjson::Document& document = scenario.document;

				std::map<std::string, std::string>::iterator it = scenario.key_paths.find(key);
				if (it != scenario.key_paths.end())
					section = it->second;

				return set_parameter<T>(document, section, key, parameter);
			}
				
			template <class T>
			bool set_parameter(rapidjson::Document& document, const std::string& section, const std::string& key, T& parameter)
			{
				rapidjson::Value* value;

				// add "/" in front of key for Pointer
				std::string str_key = "/" + key;
				const char* char_key = str_key.c_str();

				// if section is not defined then use key
				if (section.compare("") == 0)
				{
					// check if key value is not found
					value = rapidjson::Pointer(char_key).Get(document);
					if (!value)
					{
						cout << "Unable to locate key \'" << key << "\'" << endl;
						return false;
					}
				}
				else
				{
					std::vector<string> section_tokens;
					section_tokens = split_section(section.c_str(), '/');

					// add "/" to each token for Pointer
					for (auto& element : section_tokens)
					{
						element = "/" + element;
					}

					// check if first section is not found
					//cout << "section token 0 is " << section_tokens[0] << endl;
					value = rapidjson::Pointer(section_tokens[0].c_str()).Get(document);
					if (!value)
					{
						cout << "Unable to locate section \'" << section_tokens[0] << "\'";
						if (section_tokens.size() > 0)
						{
							cout << " from \'" << section << "\'" << endl;
						}
						return false;
					}

					// loop for each token element
					for (unsigned i = 1; i < section_tokens.size(); ++i)
					{
						//cout << section_tokens[i] << endl;

						// check if next section is not found
						value = rapidjson::Pointer(section_tokens[i].c_str()).Get(*value);
						if (!value)
						{
							cout << "Unable to locate sub section \'" << section_tokens[i] << "\' from \'" << section << "\'" << endl;
							return false;
						}
					}

					// if key is defined
					if (key.compare("") != 0)
					{
						// check if key value is not found
						value = rapidjson::Pointer(char_key).Get(*value);
						if (!value)
						{
							cout << "Unable to locate key \'" << key << "\' from \'" << section << "\'" << endl;
							return false;
						}
					}
				}

				// get parameter
				return set_parameter(*value, parameter);
			}

			bool set_parameter(rapidjson::Value& value, std::string& parameter)
			{
				if (value.IsString())
				{
					parameter = value.GetString();
				}
				else
				{
					cout << "Value is not set as string value. (" << value.GetString() << ")" << endl;
					return false;
				}
				return true;
			}

			bool set_parameter(rapidjson::Value& value, int& parameter)
			{
				if (value.IsInt())
				{
					parameter = value.GetInt();
				}
				else
				{
					cout << "Value is not set as integer value. (" << value.GetString() << ")" << endl;
					return false;
				}
				return true;
			}

			bool set_parameter(rapidjson::Value& value, unsigned long& parameter)
			{
				if (value.IsUint64())
				{
					parameter = value.GetUint64();
				}
				else
				{
					cout << "Value is not set as unsigned long value. (" << value.GetString() << ")" << endl;
					return false;
				}
				return true;
			}

			bool set_parameter(rapidjson::Value& value, double& parameter)
			{
				if (value.IsDouble())
				{
					parameter = value.GetDouble();
				}
				else if (value.IsFloat())
				{
					parameter = (double)value.GetFloat();
				}
				else if (value.IsInt())
				{
					parameter = (double)value.GetInt();
				}
				else
				{
					cout << "Value is not set as double value. (" << value.GetString() << ")" << endl;
					return false;
				}
				return true;
			}

			bool set_parameter(rapidjson::Value& value, float& parameter)
			{
				if (value.IsFloat())
				{
					parameter = value.GetFloat();
				}
				else if (value.IsInt())
				{
					parameter = (float)value.GetInt();
				}
				else
				{
					cout << "Value is not set as float value. (" << value.GetString() << ")" << endl;
					return false;
				}
				return true;
			}

			bool set_parameter(rapidjson::Value& value, bool& parameter)
			{
				if (value.IsBool())
				{
					parameter = value.GetBool();
				}
				else
				{
					cout << "Value is not set as bool value. (" << value.GetString() << ")" << endl;
					return false;
				}
				return true;
			}

			bool set_parameter(rapidjson::Value& value, std::vector<int>& parameter)
			{
				if (value.IsArray())
				{
					for(rapidjson::SizeType i=0; i<value.Size(); ++i)
						parameter.push_back(value[i].GetInt());
				}
				else
				{
					cout << "Value is not set as Array value. (" << value.GetString() << ")" << endl;
					return false;
				}
				return true;
			}

			bool set_parameter(rapidjson::Value& value, std::vector<float>& parameter)
			{
				if (value.IsArray())
				{
					for (rapidjson::SizeType i = 0; i<value.Size(); ++i)
						parameter.push_back((float)value[i].GetDouble());
				}
				else
				{
					cout << "Value is not set as Array value. (" << value.GetString() << ")" << endl;
					return false;
				}
				return true;
			}
		};
	}
}

using namespace Scenario_Components::Prototypes;
