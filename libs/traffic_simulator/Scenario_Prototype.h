#pragma once
#include "../Scenario_Manager/cfg_reader.h"
#include "User_Space_Includes.h"
//#include "../File_IO/network_scenario_data.h"





namespace Scenario_Components
{
	namespace Prototypes
	{
		prototype struct Scenario ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(output_results_database_name, NONE, NONE);
			accessor(output_demand_database_name, NONE, NONE);
			accessor(output_popsyn_database_name, NONE, NONE);
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

			accessor(assignment_mode, NONE, NONE);
			accessor(iseed, NONE, NONE);
						
			accessor(demand_od_flag, NONE, NONE);
			accessor(io_source_flag, NONE, NONE);

			accessor(demand_reduction_factor, NONE, NONE);

			accessor(path_calculation_interval_length, NONE, NONE);


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
			accessor(write_activity_output, NONE, NONE);
			accessor(aggregate_routing, NONE, NONE);
			accessor(do_planner_routing, NONE, NONE);
			accessor(write_demand_to_database, NONE, NONE);
			accessor(read_demand_from_database, NONE, NONE);

			//===============================================
			// Popsyn parameters
			//-----------------------------------------------
			accessor(percent_to_synthesize, NONE, NONE);
			accessor(ipf_tolerance, NONE, NONE);
			accessor(marginal_tolerance, NONE, NONE);
			accessor(maximum_iterations, NONE, NONE);
			accessor(write_marginal_output, NONE, NONE);
			accessor(write_full_output, NONE, NONE);
			accessor(popsyn_control_file_name, NONE, NONE);

			accessor(database_name, NONE, NONE);

			accessor(snapshot_period, NONE, NONE);

			//===============================================
			// i/o parameters
			//===============================================
			accessor(num_threads, NONE, NONE);
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

			accessor(compare_with_historic_moe, NONE, NONE);
			accessor(historic_network_moe_file_path_name, NONE, NONE);
			accessor(historic_link_moe_file_path_name, NONE, NONE);
			accessor(read_normal_day_link_moe, NONE, NONE);
			accessor(time_dependent_routing, NONE, NONE);
			accessor(normal_day_link_moe_file_path_name, NONE, NONE);

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

			/// antares visualization parameters
			accessor(buildings_geometry_file, NONE, NONE);
			accessor(use_buildings,NONE,NONE);
			accessor(color_cars_randomly,NONE,NONE);

			// tile imagery parameters
			accessor(tile_imagery_file,NONE,NONE);
			accessor(use_tile_imagery,NONE,NONE);
			accessor(tile_imagery_alpha_level,NONE,NONE);

			template<typename TargetType> void read_scenario_data(char* filename)
			{
				CfgReader cfgReader;
				char* path = filename;
				bool result = cfgReader.initialize(path);

				if (!result) THROW_EXCEPTION("Scenario file '"<<filename<<"' was not able to be opened.");
				
				//===============================================
				// set start time
				string start_time_in_string;
				int start_time;
				if (cfgReader.getParameter("starting_time_hh_mm", &start_time_in_string) == PARAMETER_FOUND)
				{
					start_time_in_string += ":00";
					start_time = convert_hhmmss_to_seconds(start_time_in_string);
					//assert(start_time == 0); // to be done for start time > 0
				}
				else start_time = 0;
				simulation_start_time<int>(start_time);
				
				//===============================================
				// set start time
				string end_time_in_string;
				int end_time;
				if (cfgReader.getParameter("ending_time_hh_mm", &end_time_in_string) == PARAMETER_FOUND)
				{
					end_time_in_string += ":00";
					end_time = convert_hhmmss_to_seconds(end_time_in_string);
				}
				else end_time = 60*60*24*2;
				simulation_end_time<int>(end_time);

				planning_horizon<int>(end_time - start_time);

				//===============================================
				// set interval length
				if (cfgReader.getParameter("simulation_interval_length_in_second", simulation_interval_length<int*>()) != PARAMETER_FOUND) simulation_interval_length<int>(6);


				//===============================================
				// set the demand reduction factor, used to reduce the demand read from the database
				if (cfgReader.getParameter("demand_reduction_factor", demand_reduction_factor<double*>()) != PARAMETER_FOUND) demand_reduction_factor<double>(1.0);
				cout << endl << "demand reduction factor: " << demand_reduction_factor<float>();

				//===============================================
				// set sim_interval per assignment interval
				int assignment_intervals;
				if (cfgReader.getParameter("num_simulation_intervals_per_assignment_interval", &assignment_intervals) != PARAMETER_FOUND) assignment_intervals = 50;
				assignment_interval_length<int>(assignment_intervals*simulation_interval_length<int>());

				//===============================================
				// set assignment mode
				string assignment_mode_string;
				if (cfgReader.getParameter("assignment_mode", &assignment_mode_string) != PARAMETER_FOUND) assignment_mode_string = "ONE_SHOT_ASSIGNMENT_SIMULATION_MODE";
				if (assignment_mode_string.compare("ONE_SHOT_ASSIGNMENT_SIMULATION_MODE") == 0)
				{
					assignment_mode<int>(Scenario_Components::Types::Assignment_Simulation_Mode_Keys::ONE_SHOT_ASSIGNMENT_SIMULATION_MODE);
				}
				else if (assignment_mode_string.compare("ITERATIVE_ASSIGNMENT_SIMULATION_MODE") == 0)
				{
					assignment_mode<int>(Scenario_Components::Types::Assignment_Simulation_Mode_Keys::ITERATIVE_ASSIGNMENT_SIMULATION_MODE);
				}
				else
				{
					cout << "Assignment mode not supported" << endl;
					assert(false);
				}

				//===============================================
				// set rng type
				string rng_type_string;
				if (cfgReader.getParameter("rng_type", &rng_type_string) != PARAMETER_FOUND) rng_type_string = "DETERMINISTIC";
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

				//===============================================
				// set merging mode
				string merging_mode_string;
				if (cfgReader.getParameter("merging_mode", &merging_mode_string) != PARAMETER_FOUND) merging_mode_string = "PROPORTION_TO_DEMAND";
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

				//===============================================
				// set control parameters
				if (cfgReader.getParameter("seed", iseed<unsigned long*>()) != PARAMETER_FOUND) iseed<unsigned long>(0);
				if (cfgReader.getParameter("node_control_flag", intersection_control_flag<int*>())!= PARAMETER_FOUND) intersection_control_flag<int>(0.0);
				if (cfgReader.getParameter("ramp_metering_flag", ramp_metering_flag<bool*>())!= PARAMETER_FOUND) ramp_metering_flag<bool>(false);
				if (cfgReader.getParameter("demand_od_flag", demand_od_flag<int*>())!= PARAMETER_FOUND) demand_od_flag<int>(1);
				if (cfgReader.getParameter("snapshot_period", snapshot_period<int*>())!=PARAMETER_FOUND) snapshot_period<int>(300);

				//===============================================
				// tile imagery parameters
				if (cfgReader.getParameter("tile_imagery_file", tile_imagery_file<string*>())!= PARAMETER_FOUND)
				{
					use_tile_imagery<bool>(false);
					tile_imagery_file<string>("");
				}
				else
				{
					use_tile_imagery<bool>(true);
					if (cfgReader.getParameter("tile_imagery_alpha_level", tile_imagery_alpha_level<int*>())!= PARAMETER_FOUND) tile_imagery_alpha_level<int>(255);
				}

				if (cfgReader.getParameter("color_cars_randomly", color_cars_randomly<bool*>())!= PARAMETER_FOUND) color_cars_randomly(false);

				//===============================================
				// Demand model parameters 
				if (cfgReader.getParameter("write_activity_output", this->template write_activity_output<bool*>()) != PARAMETER_FOUND) this->template write_activity_output<bool>(false);
				if (cfgReader.getParameter("aggregate_routing", this->template aggregate_routing<bool*>()) != PARAMETER_FOUND) this->template aggregate_routing<bool>(false);
				if (cfgReader.getParameter("do_planner_routing", this->template do_planner_routing<bool*>()) != PARAMETER_FOUND) this->template do_planner_routing<bool>(false);		
				if (cfgReader.getParameter("write_demand_to_database", this->template write_demand_to_database<bool*>()) != PARAMETER_FOUND) this->template write_demand_to_database<bool>(false);
				if (cfgReader.getParameter("read_demand_from_database", this->template read_demand_from_database<bool*>()) != PARAMETER_FOUND) this->template read_demand_from_database<bool>(false);

				//=======================================================================================================================================================
				// PopSyn parameters
				if (cfgReader.getParameter("percent_to_synthesize", this->template percent_to_synthesize<double*>()) != PARAMETER_FOUND) this->template percent_to_synthesize<double>(1.0);
				if (cfgReader.getParameter("ipf_tolerance", this->template ipf_tolerance<double*>()) != PARAMETER_FOUND) this->template ipf_tolerance<double>(0.01);
				if (cfgReader.getParameter("marginal_tolerance", this->template marginal_tolerance<int*>()) != PARAMETER_FOUND) this->template marginal_tolerance<int>(5);
				if (cfgReader.getParameter("maximum_iterations", this->template maximum_iterations<int*>()) != PARAMETER_FOUND) this->template maximum_iterations<int>(100);
				if (cfgReader.getParameter("write_marginal_output", this->template write_marginal_output<bool*>()) != PARAMETER_FOUND) this->template write_marginal_output<bool>(false);
				if (cfgReader.getParameter("write_full_output", this->template write_full_output<bool*>()) != PARAMETER_FOUND) this->template write_full_output<bool>(false);
				string popsyn_control_string;
				if (cfgReader.getParameter("popsyn_control_file", this->template popsyn_control_file_name<string*>()) != PARAMETER_FOUND) this->template popsyn_control_file_name<string>((string)"popsyn_control_file.txt");



				if (cfgReader.getParameter("write_visualizer_snapshot", this->template write_visualizer_snapshot<bool*>()) != PARAMETER_FOUND) this->template write_visualizer_snapshot<bool>(false);


				//===============================================
				// set control parameters
				string io_source_string;
				if (cfgReader.getParameter("io_source_flag", &io_source_string)!= PARAMETER_FOUND) io_source_string = "ODB_IO_SOURCE";
				if (io_source_string.compare("FILE_IO_SOURCE") == 0)
				{
					io_source_flag<int>(Scenario_Components::Types::IO_Source_Keys::FILE_IO_SOURCE);
				} 
				else if (io_source_string.compare("HARD_CODED_IO_SOURCE") == 0)
				{
					io_source_flag<int>(Scenario_Components::Types::IO_Source_Keys::HARD_CODED_IO_SOURCE);
				} 
				else if (io_source_string.compare("ODB_IO_SOURCE") == 0)
				{
					io_source_flag<int>(Scenario_Components::Types::IO_Source_Keys::ODB_IO_SOURCE);
					if (cfgReader.getParameter("database_name", database_name<string*>())!= PARAMETER_FOUND) database_name<std::string>("chicago");
					if (cfgReader.getParameter("historical_results_database_name", historical_results_database_name<string*>())!= PARAMETER_FOUND) historical_results_database_name<std::string>("");
				} 
				else
				{
					cout << "IO mode not supported" << endl;
					assert(false);
				}


				num_simulation_intervals<int>(planning_horizon<int>()/simulation_interval_length<int>());
				num_assignment_intervals<int>(planning_horizon<int>()/assignment_interval_length<int>());
				num_simulation_intervals_per_assignment_interval<int>(assignment_interval_length<int>()/simulation_interval_length<int>());
				

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

				// set I/O parameters		
				if (cfgReader.getParameter("count_integrated_in_network_vehicles_only", count_integrated_in_network_vehicles_only<bool*>())!= PARAMETER_FOUND) count_integrated_in_network_vehicles_only<bool>(false);

				if (cfgReader.getParameter("output_dir_name", output_dir_name<string*>())!= PARAMETER_FOUND) output_dir_name<string>("");
				if (cfgReader.getParameter("num_threads", num_threads<int*>())!= PARAMETER_FOUND) num_threads<int>(1);
				if (cfgReader.getParameter("write_db_input_to_files", write_db_input_to_files<bool*>())!= PARAMETER_FOUND) write_db_input_to_files<bool>(false);
				if (cfgReader.getParameter("run_simulation_for_db_input", run_simulation_for_db_input<bool*>())!= PARAMETER_FOUND) run_simulation_for_db_input<bool>(true);
				if (cfgReader.getParameter("write_node_control_state", write_node_control_state<bool*>())!= PARAMETER_FOUND) write_node_control_state<bool>(false);
				if (cfgReader.getParameter("write_network_link_flow", write_network_link_flow<bool*>())!= PARAMETER_FOUND) write_network_link_flow<bool>(false);
				if (cfgReader.getParameter("write_network_link_turn_time", write_network_link_turn_time<bool*>())!= PARAMETER_FOUND) write_network_link_turn_time<bool>(false);
				if (cfgReader.getParameter("write_output_summary", write_output_summary<bool*>())!= PARAMETER_FOUND) write_output_summary<bool>(true);
				if (cfgReader.getParameter("output_link_moe_for_simulation_interval", output_link_moe_for_simulation_interval<bool*>())!= PARAMETER_FOUND) output_link_moe_for_simulation_interval<bool>(false);
				if (cfgReader.getParameter("output_turn_movement_moe_for_simulation_interval", output_turn_movement_moe_for_simulation_interval<bool*>())!= PARAMETER_FOUND) output_turn_movement_moe_for_simulation_interval<bool>(false);
				if (cfgReader.getParameter("output_network_moe_for_simulation_interval", output_network_moe_for_simulation_interval<bool*>())!= PARAMETER_FOUND) output_network_moe_for_simulation_interval<bool>(false);
				if (cfgReader.getParameter("write_network_snapshots", write_network_snapshots<bool*>())!= PARAMETER_FOUND) write_network_snapshots<bool>(false);
				if (cfgReader.getParameter("read_network_snapshots", read_network_snapshots<bool*>())!= PARAMETER_FOUND) read_network_snapshots<bool>(false);
				if (cfgReader.getParameter("routing_with_snapshots", routing_with_snapshots<bool*>())!= PARAMETER_FOUND) routing_with_snapshots<bool>(false);

				if (cfgReader.getParameter("input_network_snapshots_file_path_name", input_network_snapshots_file_path_name<string*>())!= PARAMETER_FOUND) input_network_snapshots_file_path_name<string>("input_network_snapshots");
				
				//===============================================
				// Vehicle trajectory tracking parameters
				if (cfgReader.getParameter("write_vehicle_trajectory", write_vehicle_trajectory<bool*>())!= PARAMETER_FOUND) write_vehicle_trajectory<bool>(false);
				if (cfgReader.getParameter("vehicle_trajectory_sample_rate", vehicle_trajectory_sample_rate<double*>())!= PARAMETER_FOUND) vehicle_trajectory_sample_rate<double>(1.0);
				if (cfgReader.getParameter("vehicle_tracking_list_file_name", vehicle_tracking_list_file_name<string*>())!= PARAMETER_FOUND)
				{
					use_vehicle_tracking_list<bool>(false);
				}
				else
				{
					vehicle_trajectory_sample_rate<double>(0.0);
					use_vehicle_tracking_list<bool>(true);
					File_IO::File_Reader fr;
					fr.Open(this->vehicle_tracking_list_file_name<string>(),false);
					while (fr.Read())
					{
						int vehid = 0;
						fr.Get_Data<int>(vehid,0);
						std::unordered_set<int>::iterator itr = vehicle_tracking_list<std::unordered_set<int>&>().find(vehid);
						if (itr == vehicle_tracking_list<std::unordered_set<int>&>().end()) this->vehicle_tracking_list<std::unordered_set<int>&>().insert(vehid);
					}
					cout <<endl<<"Tracking "<<vehicle_tracking_list<std::unordered_set<int>&>().size()<<" vehicles."<<endl;
				}

				// GET NETWORK SKIMMING PARAMETERS
				if (cfgReader.getParameter("write_skim_tables", this->template write_skim_tables<bool*>()) != PARAMETER_FOUND) this->template write_skim_tables<bool>(false);
				if (cfgReader.getParameter("read_skim_tables", this->template read_skim_tables<bool*>()) != PARAMETER_FOUND) this->template read_skim_tables<bool>(false);
				if (cfgReader.getParameter("input_highway_skim_file_path_name", this->template input_highway_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->template input_highway_skim_file_path_name<string>((string)"");
				if (cfgReader.getParameter("output_highway_skim_file_path_name", this->template output_highway_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->template output_highway_skim_file_path_name<string>((string)"highway_skim_file_out.txt");
				if (cfgReader.getParameter("input_highway_cost_skim_file_path_name", this->template input_highway_cost_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->template input_highway_cost_skim_file_path_name<string>((string)"");
				if (cfgReader.getParameter("output_highway_cost_skim_file_path_name", this->template output_highway_cost_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->template output_highway_cost_skim_file_path_name<string>((string)"highway_cost_skim_file_out.txt");
				if (cfgReader.getParameter("input_transit_skim_file_path_name", this->template input_transit_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->template input_transit_skim_file_path_name<string>((string)"");
				if (cfgReader.getParameter("output_transit_skim_file_path_name", this->template output_transit_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->template output_transit_skim_file_path_name<string>((string)"transit_skim_file_out.txt");
				if (cfgReader.getParameter("skim_interval_endpoint_minutes", this->template skim_interval_endpoint_minutes<IntArray*>()) != PARAMETER_FOUND)
				{
					use_skim_intervals<bool>(false);
					if (cfgReader.getParameter("skim_interval_length_minutes", this->template skim_interval_length_minutes<int*>()) != PARAMETER_FOUND)
					{
						do_skimming<bool>(false);
						if (this->template write_skim_tables<bool>())
						{
							THROW_EXCEPTION("ERROR: the 'write_skim_tables' parameters has been set to true, but no skim interval has been defined.  Use the 'skim_interval_endpoint_minutes' (endpoint of each interval) or 'skim_interval_length_minutes' (fixed lenght for all intervals) keywords to specify the skim intervals.");
						}
					}
					else do_skimming<bool>(true);
				}
				else
				{
					use_skim_intervals<bool>(true);
					do_skimming<bool>(true);
				}
				

				if (cfgReader.getParameter("compare_with_historic_moe", compare_with_historic_moe<bool*>())!= PARAMETER_FOUND) compare_with_historic_moe<bool>(false);
				if (cfgReader.getParameter("historic_network_moe_file_path_name", historic_network_moe_file_path_name<string*>())!= PARAMETER_FOUND) historic_network_moe_file_path_name<string>("historic_realtime_moe_network.csv");
				if (cfgReader.getParameter("historic_link_moe_file_path_name", historic_link_moe_file_path_name<string*>())!= PARAMETER_FOUND) historic_link_moe_file_path_name<string>("historic_moe_link.csv");
				if (cfgReader.getParameter("read_normal_day_link_moe", read_normal_day_link_moe<bool*>())!= PARAMETER_FOUND) read_normal_day_link_moe<bool>(false);
				if (cfgReader.getParameter("normal_day_link_moe_file_path_name", normal_day_link_moe_file_path_name<string*>())!= PARAMETER_FOUND) normal_day_link_moe_file_path_name<string>("normal_day_moe_link.csv");
				if (cfgReader.getParameter("historic_demand_moe_directory", historic_demand_moe_directory<string*>())!= PARAMETER_FOUND) historic_demand_moe_directory<string>("");

				if (cfgReader.getParameter("output_link_moe_for_assignment_interval", output_link_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) output_link_moe_for_assignment_interval<bool>(false);
				if (cfgReader.getParameter("output_turn_movement_moe_for_assignment_interval", output_turn_movement_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) output_turn_movement_moe_for_assignment_interval<bool>(false);
				if (cfgReader.getParameter("output_network_moe_for_assignment_interval", output_network_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) output_network_moe_for_assignment_interval<bool>(false);
				if (cfgReader.getParameter("output_analzye_link_group_moe_for_assignment_interval", output_analzye_link_group_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) output_analzye_link_group_moe_for_assignment_interval<bool>(false);
				if (cfgReader.getParameter("load_analyze_link_groups_from_file", load_analyze_link_groups_from_file<bool*>())!= PARAMETER_FOUND) load_analyze_link_groups_from_file<bool>(false);
				if (cfgReader.getParameter("analyze_link_groups_file_path_name", analyze_link_groups_file_path_name<string*>())!= PARAMETER_FOUND) analyze_link_groups_file_path_name<string>("analyze_link_groups");

				if (cfgReader.getParameter("DB_output_link_moe_for_assignment_interval", DB_output_link_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) DB_output_link_moe_for_assignment_interval<bool>(false);
				if (cfgReader.getParameter("write_ttime_distribution_from_network_model", write_ttime_distribution_from_network_model<bool*>())!= PARAMETER_FOUND) write_ttime_distribution_from_network_model<bool>(false);
				if (cfgReader.getParameter("vehicle_trajectory_output_threshold", vehicle_trajectory_output_threshold<int*>())!= PARAMETER_FOUND) vehicle_trajectory_output_threshold<int>(-1.0);

				if (cfgReader.getParameter("use_tmc", use_tmc<bool*>())!= PARAMETER_FOUND) use_tmc<bool>(false);
				if (cfgReader.getParameter("use_network_events", use_network_events<bool*>())!= PARAMETER_FOUND) use_network_events<bool>(false);

				if (cfgReader.getParameter("jam_density_constraints_enforced", jam_density_constraints_enforced<bool*>())!= PARAMETER_FOUND) jam_density_constraints_enforced<bool>(true);
				if (cfgReader.getParameter("maximum_flow_rate_constraints_enforced", maximum_flow_rate_constraints_enforced<bool*>())!= PARAMETER_FOUND) maximum_flow_rate_constraints_enforced<bool>(true);
				if (cfgReader.getParameter("vehicle_taking_action", vehicle_taking_action<bool*>())!= PARAMETER_FOUND) vehicle_taking_action<bool>(false);

				///enroute switching pretrip_informed_market_share
				if (cfgReader.getParameter("pretrip_informed_market_share", pretrip_informed_market_share<double*>())!= PARAMETER_FOUND) pretrip_informed_market_share<double>(0.5);
				if (cfgReader.getParameter("realtime_informed_vehicle_market_share", realtime_informed_vehicle_market_share<double*>())!= PARAMETER_FOUND) realtime_informed_vehicle_market_share<double>(0.5);
				if (cfgReader.getParameter("information_compliance_rate_mean", information_compliance_rate_mean<double*>())!= PARAMETER_FOUND) information_compliance_rate_mean<double>(1.0);
				if (cfgReader.getParameter("information_compliance_rate_standard_deviation", information_compliance_rate_standard_deviation<double*>())!= PARAMETER_FOUND) information_compliance_rate_standard_deviation<double>(0.5);
				if (cfgReader.getParameter("relative_indifference_band_route_choice_mean", relative_indifference_band_route_choice_mean<double*>())!= PARAMETER_FOUND) relative_indifference_band_route_choice_mean<double>(0.1);
				if (cfgReader.getParameter("minimum_travel_time_saving_mean", minimum_travel_time_saving_mean<double*>())!= PARAMETER_FOUND) minimum_travel_time_saving_mean<double>(1.0); // in minutes
				if (cfgReader.getParameter("minimum_travel_time_saving_standard_deviation", minimum_travel_time_saving_standard_deviation<double*>())!= PARAMETER_FOUND) minimum_travel_time_saving_standard_deviation<double>(1.0); // in minutes
				if (cfgReader.getParameter("enroute_switching_enabled", enroute_switching_enabled<bool*>())!= PARAMETER_FOUND) enroute_switching_enabled<bool>(true); 
				if (cfgReader.getParameter("use_realtime_travel_time_for_enroute_switching", use_realtime_travel_time_for_enroute_switching<bool*>())!= PARAMETER_FOUND) use_realtime_travel_time_for_enroute_switching<bool>(false);
				if (cfgReader.getParameter("minimum_delay_ratio_for_enroute_switching", minimum_delay_ratio_for_enroute_switching<double*>())!= PARAMETER_FOUND) minimum_delay_ratio_for_enroute_switching<double>(3.0f);
				if (cfgReader.getParameter("minimum_delay_seconds_for_enroute_switching", minimum_delay_seconds_for_enroute_switching<double*>())!= PARAMETER_FOUND) minimum_delay_seconds_for_enroute_switching<double>(300.0f);
				if (cfgReader.getParameter("enroute_switching_on_excessive_delay", enroute_switching_on_excessive_delay<bool*>())!= PARAMETER_FOUND) enroute_switching_on_excessive_delay<bool>(true);
				if (cfgReader.getParameter("multimodal_network_input", multimodal_network_input<bool*>())!= PARAMETER_FOUND) multimodal_network_input<bool>(false);
				if (cfgReader.getParameter("enroute_excessive_delay_factor", enroute_excessive_delay_factor<double*>())!= PARAMETER_FOUND) enroute_excessive_delay_factor<double>(1.0);
				if (cfgReader.getParameter("minimum_seconds_from_arrival_for_enroute_switching", minimum_seconds_from_arrival_for_enroute_switching<double*>())!= PARAMETER_FOUND) minimum_seconds_from_arrival_for_enroute_switching<double>(300.0f);

				if (cfgReader.getParameter("time_dependent_routing", time_dependent_routing<bool*>())!= PARAMETER_FOUND) time_dependent_routing<bool>(false);
				
				if (cfgReader.getParameter("accident_event_duration_reduction", accident_event_duration_reduction<double*>())!= PARAMETER_FOUND) accident_event_duration_reduction<double>(1.0);
				

				if (cfgReader.getParameter("calculate_realtime_moe", calculate_realtime_moe<bool*>())!= PARAMETER_FOUND) calculate_realtime_moe<bool>(true);
				
				if (cfgReader.getParameter("buildings_geometry_file", buildings_geometry_file<string*>())!= PARAMETER_FOUND)
				{
					use_buildings(false);
					buildings_geometry_file<string&>()="";
				}
				else use_buildings(true);
				

				//output_dir_name<string&>() = "";
				input_dir_name<string&>() = "";
				open_output_files<NULLTYPE>();
				open_input_files<NULLTYPE>();
			}

			// template<typename TargetType> void read_scenario_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data)
			// {
				// //network_models::network_information::scenario_data_information::read_scenario_data("", scenario_data);
				// simulation_interval_length<int>(scenario_data.simulation_interval_length);	// in second
				// assignment_interval_length<int>(scenario_data.assignment_interval_length); //in second
				// path_calculation_interval_length<int>(scenario_data.path_calculation_interval_length);
				
				// num_threads<int>(scenario_data.num_threads);
				// demand_od_flag<int>(scenario_data.demand_od_flag);

				// simulation_start_time<int>(scenario_data.simulation_start_time);	//
				// simulation_end_time<int>(scenario_data.simulation_end_time);	//

				// planning_horizon<int>(scenario_data.planning_horizon); //
				// num_simulation_intervals<int>(scenario_data.num_simulation_intervals);
				// num_assignment_intervals<int>(scenario_data.num_assignment_intervals);
				// num_simulation_intervals_per_assignment_interval<int>(scenario_data.num_simulation_intervals_per_assignment_interval);

				// assignment_mode<int>(scenario_data.assignment_mode);

				// iseed<unsigned long>(scenario_data.iseed);

				// intersection_control_flag<int>(scenario_data.node_control_flag);

				// output_dir_name<string&>() = "";
				// open_output_files<NULLTYPE>();
				// open_input_files<NULLTYPE>();

				// network_cumulative_loaded_vehicles<int>(0.0);
				// network_cumulative_departed_vehicles<int>(0.0);
				// network_in_network_vehicles<int>(0.0);
				// network_cumulative_arrived_vehicles<int>(0.0);
				// network_cumulative_switched_decisions<int>(0.0);
				// network_average_trip_travel_time<float>(0.0f);

				// network_in_system_vehicles<int>(0.0);

				// assignment_time_in_seconds<double>(0.0);
			    // simulation_time_in_seconds<double>(0.0);
			    // operation_time_in_seconds<double>(0.0);
			    // output_time_in_seconds<double>(0.0);

				// snapshot_period<int>(300);
				
				// ///enroute swtiching
				// realtime_informed_vehicle_market_share<double>(0.5);
				// information_compliance_rate_mean<double>(0.5);
				// information_compliance_rate_standard_deviation<double>(0.5);
				// relative_indifference_band_route_choice_mean<double>(0.1);
				// relative_indifference_band_route_choice_standard_deviation<double>(0.1);
				// minimum_travel_time_saving_mean<double>(1.0);	// in minutes
				// minimum_travel_time_saving_standard_deviation<double>(1.0);	// in minutes

			// }

			// template<typename TargetType> void write_scenario_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data)
			// {

				// network_models::network_information::scenario_data_information::scenario_data_initialization(scenario_data);

				// scenario_data.simulation_interval_length = simulation_interval_length<int>();	// in second
				// scenario_data.assignment_interval_length = assignment_interval_length<int>(); //in second
				// scenario_data.path_calculation_interval_length = path_calculation_interval_length<int>();
			
				// scenario_data.simulation_start_time = simulation_start_time<int>();	//
				// scenario_data.simulation_end_time = simulation_end_time<int>();	//

				// scenario_data.planning_horizon = planning_horizon<int>(); //
				// scenario_data.num_simulation_intervals = num_simulation_intervals<int>();
				// scenario_data.num_assignment_intervals = num_assignment_intervals<int>();
				// scenario_data.num_simulation_intervals_per_assignment_interval = num_simulation_intervals_per_assignment_interval<int>();

				// scenario_data.num_threads = num_threads<int>();
				// scenario_data.demand_od_flag = 0;

				// scenario_data.assignment_mode = assignment_mode<network_models::network_information::scenario_data_information::Assignment_Simulation_Mode_Keys>();

				// scenario_data.iseed = iseed<unsigned long>();

				// scenario_data.node_control_flag = intersection_control_flag<int>();
			// }

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
				result_db_ptr<shared_ptr<odb::database>>(db_ptr);

				//----------------------
				// demand database
				results_name = output_dir_name<string>().append(this->database_name<string&>());
				unique_ptr<odb::database> db2(create_sqlite_database(results_name, polaris::io::db_inventory[2]));
				this->output_demand_database_name(polaris::io::make_name(results_name, polaris::io::db_inventory[2]));
				odb::transaction t2(db2->begin());
				t2.commit();

				//----------------------
				// synthetic population database
				results_name = output_dir_name<string>().append(this->database_name<string&>());
				unique_ptr<odb::database> db3(create_sqlite_database(results_name, polaris::io::db_inventory[5]));
				this->output_popsyn_database_name(polaris::io::make_name(results_name, polaris::io::db_inventory[5]));
				odb::transaction t3(db3->begin());
				t3.commit();


				//vehicle trajectory
				if (this->template write_vehicle_trajectory<bool>())
				{
					vehicle_trajectory_file_name<string&>().assign(output_dir_name<string&>() + "vehicle_trajectory.csv");
					vehicle_trajectory_file<fstream&>().open(vehicle_trajectory_file_name<string&>(),fstream::out);
					if(vehicle_trajectory_file<fstream&>().is_open()) 
					{ 
						//vehicle_trajectory_file<fstream&>() 
						//	<< "vehicle" <<  ","
						//	<< "origin_zone " <<  ","
						//	<< "destination_zone" << ","
						//	<< "origin_activity_location" << ","
						//	<< "destination_activity_location" << ","
						//	<< "origin_link" << ","
						//	<< "destination_link" << ","
						//	<< "num_links" << ","
						//	<< "departure_time" << ","
						//	<< "arrival_time" << ","
						//	<< "travel_time" << ","
						//	<< "routed_travel_time" << ","
						//	<< "travel_time_ratio" << ","
						//	<< "trip_length" << ","
						//	<< "num_switches" << ","
						//	<< "loading_delay" << ","
						//	<< "entry_queue_length" << ","
						//	<<endl;

						vehicle_trajectory_file<fstream&>() 
							<< "vehicle" <<  ","
							<< "link_number" <<  ","
							<< "link_id" <<  ","
							<< "link_dir" << ","
							<< "entering_time" << ","
							<< "travel_time" << ","
							<< "start_position" << ","
							<< "length" << ","
							//<< "delayed_time"<< ","
							<< "actual_speed"<< ","
							<< "free_flow_speed"<< ","
							<< "stopped_time"<<","
							<< "stop_position"
							<<endl;
					}
					else
					{
						cout << "Cannot open file - "
							<< vehicle_trajectory_file_name<string&>()
							<< endl;
					}
				}

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
		};
	}
}

using namespace Scenario_Components::Prototypes;
