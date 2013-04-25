#pragma once
#include "../Scenario_Manager/cfg_reader.h"
#include "User_Space_Includes.h"
#include "../File_IO/network_scenario_data.h"


namespace Scenario_Components
{
	namespace Types
	{
		struct Hard_Coded_Scenario{};
		struct ODB_Scenario{};
		struct File_Scenario{};

			enum Assignment_Simulation_Mode_Keys
			{
				ONE_SHOT_ASSIGNMENT_SIMULATION_MODE=0,
				ITERATIVE_ASSIGNMENT_SIMULATION_MODE,
			};

			enum IO_Source_Keys
			{
				HARD_CODED_IO_SOURCE=0,
				ODB_IO_SOURCE,
				FILE_IO_SOURCE,
			};

			enum Type_Sub_Iteration_keys
			{
				START_OF_ITERATION = 0,
				NETWORK_SNAPSHOT_SUB_ITERATION,
				EVENTS_UPDATE_SUB_ITERATION,
				ROUTING_SUB_ITERATION,
				TRAVELER_SET_DEPARTURE_SUB_ITERATION,
				CONTROL_SUB_ITERATION,
				LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION,
				INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION,
				INTERSECTION_ORIGIN_LINK_LOADING_SUB_ITERATION,
				LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION,
				INTERSECTION_NETWORK_STATE_UPDATE_SUB_ITERATION,
				MOE_COMPUTATION_SUB_ITERATION,
				MOE_VISUALIZATION_SUB_ITERATIONS,
				END_OF_ITERATION=40,
			};
	}

	namespace Concepts
	{
		concept struct Has_Popsyn_Configuration_Data
		{
			check_getter(has_percent_to_synthesize_p, Component_Type::percent_to_synthesize);
			check_getter(has_ipf_tolerance_p, Component_Type::ipf_tolerance);
			check_getter(has_marginal_tolerance_p, Component_Type::marginal_tolerance);
			check_getter(has_maximum_iterations_p, Component_Type::maximum_iterations);
			check_getter(has_write_marginal_output_p, Component_Type::write_marginal_output);
			check_getter(has_write_full_output_p, Component_Type::write_full_output);
			check_getter(has_popsyn_control_file_name_p, Component_Type::popsyn_control_file_name);

			check_getter(has_percent_to_synthesize, percent_to_synthesize);
			check_getter(has_ipf_tolerance, ipf_tolerance);
			check_getter(has_marginal_tolerance, marginal_tolerance);
			check_getter(has_maximum_iterations, maximum_iterations);
			check_getter(has_write_marginal_output,write_marginal_output);
			check_getter(has_write_full_output,write_full_output);
			check_getter(has_popsyn_control_file_name,popsyn_control_file_name);
			define_sub_check(Has_Popsyn_Configuration_Data_Prototype, has_percent_to_synthesize_p && has_ipf_tolerance_p && has_marginal_tolerance_p && has_maximum_iterations_p && has_write_marginal_output_p && has_write_full_output_p && has_popsyn_control_file_name_p);
			define_sub_check(Has_Popsyn_Configuration_Data_Component, has_percent_to_synthesize && has_ipf_tolerance && has_marginal_tolerance && has_maximum_iterations && has_write_marginal_output && has_write_full_output && has_popsyn_control_file_name);
			define_default_check(Has_Popsyn_Configuration_Data_Prototype || Has_Popsyn_Configuration_Data_Component);
		};
	}
	
	namespace Prototypes
	{
		prototype struct Scenario_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;



			feature_accessor(simulation_interval_length, none, none);
			feature_accessor(assignment_interval_length, none, none);


			feature_accessor(planning_horizon, none, none);
			feature_accessor(simulation_start_time, none, none);
			feature_accessor(simulation_end_time, none, none);

			feature_accessor(intersection_control_flag, none, none);
			
			feature_accessor(num_simulation_intervals, none, none);
			feature_accessor(num_assignment_intervals, none, none);
			feature_accessor(num_simulation_intervals_per_assignment_interval, none, none);

			feature_accessor(assignment_mode, none, none);
			feature_accessor(iseed, none, none);
						
			feature_accessor(demand_od_flag, none, none);
			feature_accessor(io_source_flag, none, none);

			feature_accessor(path_calculation_interval_length, none, none);


			feature_accessor(current_day_index, none, none);
			

			feature_accessor(output_writer, none, none);

			feature_accessor(network_cumulative_loaded_vehicles, none, none);
			feature_accessor(network_cumulative_departed_vehicles, none, none);
			feature_accessor(network_in_network_vehicles, none, none);
			feature_accessor(network_cumulative_arrived_vehicles, none, none);
			feature_accessor(network_cumulative_switched_decisions, none, none);

			feature_accessor(input_dir_name, none, none);
			feature_accessor(output_dir_name, none, none);
				
			feature_accessor(vehicle_trajectory_file_name, none, none);
			feature_accessor(vehicle_trajectory_file, none, none);

			feature_accessor(routed_path_file_name, none, none);
			feature_accessor(routed_path_file, none, none);

			feature_accessor(network_link_flow_file_name, none, none);
			feature_accessor(network_link_flow_file, none, none);

			feature_accessor(network_link_turn_time_file_name, none, none);
			feature_accessor(network_link_turn_time_file, none, none);

			feature_accessor(network_node_control_state_file_name, none, none);
			feature_accessor(network_node_control_state_file, none, none);

			feature_accessor(output_summary_file_name, none, none);
			feature_accessor(output_summary_file, none, none);
			
			feature_accessor(vehicle_transfer_file_name, none, none);
			feature_accessor(vehicle_transfer_file, none, none);

			feature_accessor(out_network_moe_file, none, none);
			feature_accessor(out_link_moe_file, none, none);
			feature_accessor(out_movement_moe_file, none, none);

			feature_accessor(out_realtime_network_moe_file, none, none);
			feature_accessor(out_realtime_link_moe_file, none, none);
			feature_accessor(out_realtime_movement_moe_file, none, none);
			
			feature_accessor(output_network_snapshots_file, none, none);
			feature_accessor(input_network_snapshots_file, none, none);

			feature_accessor(reference_realtime_network_moe_file, none, none);

			feature_accessor(assignment_time_in_seconds, none, none);
			feature_accessor(simulation_time_in_seconds, none, none);
			feature_accessor(operation_time_in_seconds, none, none);
			feature_accessor(output_time_in_seconds, none, none);
			feature_accessor(condition_time_in_seconds, none, none);

			//===============================================
			// Demand model parameters
			//-----------------------------------------------
			feature_accessor(write_activity_output,none,none);


			//===============================================
			// Popsyn parameters
			//-----------------------------------------------
			feature_accessor(percent_to_synthesize, none, none);
			feature_accessor(ipf_tolerance, none,none);
			feature_accessor(marginal_tolerance, none,none);
			feature_accessor(maximum_iterations, none,none);
			feature_accessor(write_marginal_output,none,none);
			feature_accessor(write_full_output,none,none);
			feature_accessor(popsyn_control_file_name,none,none);

			feature_accessor(database_name, none, none);

			feature_accessor(snapshot_period, none, none);

			//===============================================
			// i/o parameters
			//===============================================
			feature_accessor(num_threads, none, none);
			feature_accessor(write_db_input_to_files, none, none); // to control whether database input is written to files
			feature_accessor(run_simulation_for_db_input, none, none); // to control whether to run simulation for database input

			feature_accessor(write_node_control_state, none, none);
			feature_accessor(write_vehicle_trajectory, none, none);
			feature_accessor(write_network_link_flow, none, none);
			feature_accessor(write_network_link_turn_time, none, none);
			feature_accessor(write_output_summary, none, none);
			
			feature_accessor(write_network_snapshots, none, none);
			feature_accessor(read_network_snapshots, none, none);

			feature_accessor(output_moe_for_assignment_interval, none, none);
			feature_accessor(output_link_moe_for_simulation_interval, none, none);
			feature_accessor(output_turn_movement_moe_for_simulation_interval, none, none);
			feature_accessor(output_network_moe_for_simulation_interval, none, none);
			
			feature_accessor(use_event_manager, none, none);

			/// enroute switching parameters
			feature_accessor(realtime_informed_vehicle_market_share, none, none);
			feature_accessor(information_compliance_rate_mean, none, none);
			feature_accessor(information_compliance_rate_standard_deviation, none, none);
			feature_accessor(relative_indifference_bound_route_choice_mean, none, none);
			feature_accessor(relative_indifference_bound_route_choice_standard_deviation, none, none);
			feature_accessor(minimum_travel_time_saving_mean, none, none);
			feature_accessor(minimum_travel_time_saving_standard_deviation, none, none);

			feature_prototype void read_scenario_data()
			{
				CfgReader cfgReader;
				char* path = "scenario.json";
				bool result = cfgReader.initialize(path);
				
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
				if (cfgReader.getParameter("simulation_interval_length_in_second", simulation_interval_length<int*>()) != PARAMETER_FOUND)simulation_interval_length<int>(6);

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
				// set control parameters
				if (cfgReader.getParameter("seed", iseed<unsigned long*>()) != PARAMETER_FOUND) iseed<unsigned long>(0.0);
				if (cfgReader.getParameter("node_control_flag", intersection_control_flag<int*>())!= PARAMETER_FOUND) intersection_control_flag<int>(0.0);
				if (cfgReader.getParameter("demand_od_flag", demand_od_flag<int*>())!= PARAMETER_FOUND) demand_od_flag<int>(1);
				if (cfgReader.getParameter("snapshot_period", snapshot_period<int*>())!=PARAMETER_FOUND) snapshot_period<int>(300);


				//===============================================
				// Demand model parameters
				if (cfgReader.getParameter("write_activity_output", this->write_activity_output<bool*>()) != PARAMETER_FOUND) this->write_full_output<bool>(false);

				//=======================================================================================================================================================
				// PopSyn parameters
				if (cfgReader.getParameter("percent_to_synthesize", this->percent_to_synthesize<double*>()) != PARAMETER_FOUND) this->percent_to_synthesize<double>(1.0);
				if (cfgReader.getParameter("ipf_tolerance", this->ipf_tolerance<double*>()) != PARAMETER_FOUND) this->ipf_tolerance<double>(0.01);
				if (cfgReader.getParameter("marginal_tolerance", this->marginal_tolerance<int*>()) != PARAMETER_FOUND) this->marginal_tolerance<int>(5);
				if (cfgReader.getParameter("maximum_iterations", this->maximum_iterations<int*>()) != PARAMETER_FOUND) this->maximum_iterations<int>(100);
				if (cfgReader.getParameter("write_marginal_output", this->write_marginal_output<bool*>()) != PARAMETER_FOUND) this->write_marginal_output<bool>(false);
				if (cfgReader.getParameter("write_full_output", this->write_full_output<bool*>()) != PARAMETER_FOUND) this->write_full_output<bool>(false);
				string popsyn_control_string;
				if (cfgReader.getParameter("popsyn_control_file", this->popsyn_control_file_name<string*>()) != PARAMETER_FOUND) this->popsyn_control_file_name<string>((string)"popsyn_control_file.txt");

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
					if (cfgReader.getParameter("database_name", database_name<std::string*>())!= PARAMETER_FOUND) database_name<std::string>("chicago");
				} 
				else
				{
					cout << "IO mode not supported" << endl;
					assert(false);
				}

				num_simulation_intervals<int>(planning_horizon<int>()/simulation_interval_length<int>());
				num_assignment_intervals<int>(planning_horizon<int>()/assignment_interval_length<int>());
				num_simulation_intervals_per_assignment_interval<int>(assignment_interval_length<int>()/simulation_interval_length<int>());
				

				typedef typename ComponentType::output_writer_type OutputWriterType;
				output_writer<OutputWriterType&>().open("Polaris_Output.log");

				network_cumulative_loaded_vehicles<int>(0.0);
				network_cumulative_departed_vehicles<int>(0.0);
				network_in_network_vehicles<int>(0.0);
				network_cumulative_arrived_vehicles<int>(0.0);
				network_cumulative_switched_decisions<int>(0.0);

				assignment_time_in_seconds<double>(0.0);
			    simulation_time_in_seconds<double>(0.0);
			    operation_time_in_seconds<double>(0.0);
			    output_time_in_seconds<double>(0.0);

				// set I/O parameters
				if (cfgReader.getParameter("num_threads", num_threads<int*>())!= PARAMETER_FOUND) num_threads<int>(1);
				if (cfgReader.getParameter("write_db_input_to_files", write_db_input_to_files<bool*>())!= PARAMETER_FOUND) write_db_input_to_files<bool>(false);
				if (cfgReader.getParameter("run_simulation_for_db_input", run_simulation_for_db_input<bool*>())!= PARAMETER_FOUND) run_simulation_for_db_input<bool>(true);
				if (cfgReader.getParameter("write_node_control_state", write_node_control_state<bool*>())!= PARAMETER_FOUND) write_node_control_state<bool>(false);
				if (cfgReader.getParameter("write_vehicle_trajectory", write_vehicle_trajectory<bool*>())!= PARAMETER_FOUND) write_vehicle_trajectory<bool>(false);
				if (cfgReader.getParameter("write_network_link_flow", write_network_link_flow<bool*>())!= PARAMETER_FOUND) write_network_link_flow<bool>(false);
				if (cfgReader.getParameter("write_network_link_turn_time", write_network_link_turn_time<bool*>())!= PARAMETER_FOUND) write_network_link_turn_time<bool>(false);
				if (cfgReader.getParameter("write_output_summary", write_output_summary<bool*>())!= PARAMETER_FOUND) write_output_summary<bool>(true);
				if (cfgReader.getParameter("output_moe_for_assignment_interval", output_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) output_moe_for_assignment_interval<bool>(true);
				if (cfgReader.getParameter("output_link_moe_for_simulation_interval", output_link_moe_for_simulation_interval<bool*>())!= PARAMETER_FOUND) output_link_moe_for_simulation_interval<bool>(false);
				if (cfgReader.getParameter("output_turn_movement_moe_for_simulation_interval", output_turn_movement_moe_for_simulation_interval<bool*>())!= PARAMETER_FOUND) output_turn_movement_moe_for_simulation_interval<bool>(false);
				if (cfgReader.getParameter("output_network_moe_for_simulation_interval", output_network_moe_for_simulation_interval<bool*>())!= PARAMETER_FOUND) output_network_moe_for_simulation_interval<bool>(false);
				if (cfgReader.getParameter("write_network_snapshots", write_network_snapshots<bool*>())!= PARAMETER_FOUND) write_network_snapshots<bool>(false);
				if (cfgReader.getParameter("read_network_snapshots", read_network_snapshots<bool*>())!= PARAMETER_FOUND) read_network_snapshots<bool>(false);

				if (cfgReader.getParameter("use_event_manager", use_event_manager<bool*>())!= PARAMETER_FOUND) use_event_manager<bool>(false);

				output_dir_name<string&>() = "";
				input_dir_name<string&>() = "";
				open_output_files<NULLTYPE>();
				open_input_files<NULLTYPE>();

				///enroute switching
				realtime_informed_vehicle_market_share<double>(0.5);
				information_compliance_rate_mean<double>(1.0);
				information_compliance_rate_standard_deviation<double>(0.5);
				relative_indifference_bound_route_choice_mean<double>(0.1);
				relative_indifference_bound_route_choice_standard_deviation<double>(0.1);
				minimum_travel_time_saving_mean<double>(1.0);	// in minutes
				minimum_travel_time_saving_standard_deviation<double>(1.0);	// in minutes
			}

			feature_prototype void read_scenario_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data)
			{
				//network_models::network_information::scenario_data_information::read_scenario_data("", scenario_data);
				simulation_interval_length<int>(scenario_data.simulation_interval_length);	// in second
				assignment_interval_length<int>(scenario_data.assignment_interval_length); //in second
				path_calculation_interval_length<int>(scenario_data.path_calculation_interval_length);
				
				num_threads<int>(scenario_data.num_threads);
				demand_od_flag<int>(scenario_data.demand_od_flag);

				simulation_start_time<int>(scenario_data.simulation_start_time);	//
				simulation_end_time<int>(scenario_data.simulation_end_time);	//

				planning_horizon<int>(scenario_data.planning_horizon); //
				num_simulation_intervals<int>(scenario_data.num_simulation_intervals);
				num_assignment_intervals<int>(scenario_data.num_assignment_intervals);
				num_simulation_intervals_per_assignment_interval<int>(scenario_data.num_simulation_intervals_per_assignment_interval);

				assignment_mode<int>(scenario_data.assignment_mode);

				iseed<unsigned long>(scenario_data.iseed);

				intersection_control_flag<int>(scenario_data.node_control_flag);

				output_dir_name<string&>() = "";
				open_output_files<NULLTYPE>();
				open_input_files<NULLTYPE>();

				network_cumulative_loaded_vehicles<int>(0.0);
				network_cumulative_departed_vehicles<int>(0.0);
				network_in_network_vehicles<int>(0.0);
				network_cumulative_arrived_vehicles<int>(0.0);
				network_cumulative_switched_decisions<int>(0.0);

				assignment_time_in_seconds<double>(0.0);
			    simulation_time_in_seconds<double>(0.0);
			    operation_time_in_seconds<double>(0.0);
			    output_time_in_seconds<double>(0.0);

				snapshot_period<int>(300);
				
				///enroute swtiching
				realtime_informed_vehicle_market_share<double>(0.5);
				information_compliance_rate_mean<double>(0.5);
				information_compliance_rate_standard_deviation<double>(0.5);
				relative_indifference_bound_route_choice_mean<double>(0.1);
				relative_indifference_bound_route_choice_standard_deviation<double>(0.1);
				minimum_travel_time_saving_mean<double>(1.0);	// in minutes
				minimum_travel_time_saving_standard_deviation<double>(1.0);	// in minutes

			}

			feature_prototype void write_scenario_data(network_models::network_information::scenario_data_information::ScenarioData& scenario_data)
			{

				network_models::network_information::scenario_data_information::scenario_data_initialization(scenario_data);

				scenario_data.simulation_interval_length = simulation_interval_length<int>();	// in second
				scenario_data.assignment_interval_length = assignment_interval_length<int>(); //in second
				scenario_data.path_calculation_interval_length = path_calculation_interval_length<int>();
			
				scenario_data.simulation_start_time = simulation_start_time<int>();	//
				scenario_data.simulation_end_time = simulation_end_time<int>();	//

				scenario_data.planning_horizon = planning_horizon<int>(); //
				scenario_data.num_simulation_intervals = num_simulation_intervals<int>();
				scenario_data.num_assignment_intervals = num_assignment_intervals<int>();
				scenario_data.num_simulation_intervals_per_assignment_interval = num_simulation_intervals_per_assignment_interval<int>();

				scenario_data.num_threads = num_threads<int>();
				scenario_data.demand_od_flag = 0;

				scenario_data.assignment_mode = assignment_mode<network_models::network_information::scenario_data_information::Assignment_Simulation_Mode_Keys>();

				scenario_data.iseed = iseed<unsigned long>();

				scenario_data.node_control_flag = intersection_control_flag<int>();
			}

			feature_prototype void open_output_files()
			{
				//vehicle trajectory
				vehicle_trajectory_file_name<string&>().assign(output_dir_name<string&>() + "vehicle_trajecotry.csv");
				vehicle_trajectory_file<fstream&>().open(vehicle_trajectory_file_name<string&>(),fstream::out);
				if(vehicle_trajectory_file<fstream&>().is_open()) 
				{ 
					vehicle_trajectory_file<fstream&>() 
						<< "vehicle" <<  ","
						<< "origin_zone " <<  ","
						<< "destination_zone" << ","
						<< "origin_activity_location" << ","
						<< "destination_activity_location" << ","
						<< "origin_link" << ","
						<< "destination_link" << ","
						<< "num_links" << ","
						<< "departure_time" << ","
						<< "arrival_time" << ","
						<< "travel_time"
						<<endl;

					vehicle_trajectory_file<fstream&>() 
						<< "link_number" <<  ","
						<< "link_id " <<  ","
						<< "entering_time" << ","
						<< "travel_time" << ","
						<< "delayed_time"
						<<endl;
				}
				else
				{
					cout << "Cannot open file - "
						<< vehicle_trajectory_file_name<string&>()
						<< endl;
				}

				//routed path
				routed_path_file_name<string&>().assign(output_dir_name<string&>() + "routed_path.csv");
				routed_path_file<fstream&>().open(routed_path_file_name<string&>(),fstream::out);
				if(routed_path_file<fstream&>().is_open()) 
				{ 
					routed_path_file<fstream&>() 
						<< "vehicle" <<  ","
						<< "origin_zone " <<  ","
						<< "destination_zone" << ","
						<< "origin_activity_location" << ","
						<< "destination_activity_location" << ","
						<< "origin_link" << ","
						<< "destination_link" << ","
						<< "num_links" << ","
						<< "departure_time" << ","
						<< "arrival_time" << ","
						<< "travel_time"
						<<endl;

					routed_path_file<fstream&>() 
						<< "link_number" <<  ","
						<< "link_id " <<  ","
						<< "entering_time" << ","
						<< "travel_time" << ","
						<< "delayed_time"
						<<endl;
				}
				else
				{
					cout << "Cannot open file - "
						<< vehicle_trajectory_file_name<string&>()
						<< endl;
				}

				//link flow pattern
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

				//link turn times
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


				//operation control
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
						<< "wallclock_time" << ","
						<< "simulated_time" 
						<<endl;
				}
				else
				{
					cout << "Cannot open file - "
						<< output_summary_file_name<string&>()
						<< endl;
				}

				//vehicle_transfer
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

				//real-time moe
				//network
				string out_realtime_network_moe_file_name = output_dir_name<string&>() + "realtime_moe_network.csv";
				out_realtime_network_moe_file<fstream&>().open(out_realtime_network_moe_file_name, fstream::out);
				out_realtime_network_moe_file<fstream&>() << "clock,time,num_loaded_vehicle,num_departed_vehicle,num_arrived_vehicle,avg_link_time_in_min,avg_link_speed_in_mph,avg_link_density_in_vpmpl,avg_link_in_volume,avg_link_out_volume,avg_link_time_ratio,avg_link_speed_ratio,avg_link_density_ratio,avg_link_queue_length,network_vmt,network_vht,network_cumulative_loaded_vehicles,network_cumulative_departed_vehicles,network_in_network_vehicles,network_cumulative_arrived_vehicles\n";

				//link
				string out_realtime_link_moe_file_name = output_dir_name<string&>() + "realtime_moe_link.csv";
				out_realtime_link_moe_file<fstream&>().open(out_realtime_link_moe_file_name, fstream::out);
				out_realtime_link_moe_file<fstream&>() << "clock,time,link,unode,dnode,link_type,travel_time_in_min,travel_delay_in_min,queue_length,speed_in_mph,density_in_vpmpl,in_volume,out_volume,travel_time_ratio,speed_ratio,density_ratio\n";

				//movement
				string out_realtime_movement_moe_file_name = output_dir_name<string&>() + "realtime_moe_movement.csv";
				out_realtime_movement_moe_file<fstream&>().open(out_realtime_movement_moe_file_name, fstream::out);
				out_realtime_movement_moe_file<fstream&>() << "clock,time,turn_movement,inbound_link,outbound_link,node,turn_penalty_in_min,turn_penalty_sd_in_min,inbound_link_turn_time_in_min,outbound_link_turn_time_in_min,movement_flow_rate_in_vphpl\n";

				//moe
				//network
				string out_network_moe_file_name = output_dir_name<string&>() + "moe_network.csv";
				out_network_moe_file<fstream&>().open(out_network_moe_file_name, fstream::out);
				//out_network_moe_file<fstream&>() << "clock,time,num_loaded_vehicle,num_departed_vehicle,num_arrived_vehicle,avg_link_time_in_min,avg_link_speed_in_mph,avg_link_density_in_vpmpl,avg_link_in_flow_rate_in_vphpl,avg_link_out_flow_rate_in_vphpl,avg_link_in_volume,avg_link_out_volume,avg_link_time_ratio,avg_link_speed_ratio,avg_link_density_ratio,avg_link_in_flow_ratio,avg_link_out_flow_ratio,vht,vmt,assignment_calculation_time_in_second,simulation_calculation_time_in_second,operation_calculation_time_in_second,output_calculation_time_in_second\n";
				out_network_moe_file<fstream&>() << "clock,time,num_loaded_vehicle,num_departed_vehicle,num_arrived_vehicle,avg_link_time_in_min,avg_link_speed_in_mph,avg_link_density_in_vpmpl,avg_link_in_flow_rate_in_vphpl,avg_link_out_flow_rate_in_vphpl,avg_link_in_volume,avg_link_out_volume,avg_link_time_ratio,avg_link_speed_ratio,avg_link_density_ratio,avg_link_in_flow_ratio,avg_link_out_flow_ratio,vht,vmt\n";

				//link
				string out_link_moe_file_name = output_dir_name<string&>() + "moe_link.csv";
				out_link_moe_file<fstream&>().open(out_link_moe_file_name, fstream::out);
				out_link_moe_file<fstream&>() << "clock,time,link,unode,dnode,link_type,travel_time_in_min,travel_time_sd_in_min,travel_delay_in_min,travel_delay_sd_in_min,queue_length,speed_in_mph,density_in_vpmpl,in_flow_rate_in_vphpl,out_flow_rate_in_vphpl,in_volume,out_volume,travel_time_ratio,speed_ratio,density_ratio,in_flow_ratio,out_flow_ratio,vht,vmt\n";

				//movement
				string out_movement_moe_file_name = output_dir_name<string&>() + "moe_movement.csv";
				out_movement_moe_file<fstream&>().open(out_movement_moe_file_name, fstream::out);
				out_movement_moe_file<fstream&>() << "clock,time,turn_movement,inbound_link,outbound_link,node,turn_penalty_in_min,turn_penalty_sd_in_min,inbound_link_turn_time_in_min,outbound_link_turn_time_in_min,movement_flow_rate_in_vphpl\n";

				//routable network snapshot
				string routable_network_snapshots_file_name = output_dir_name<string&>() + "output_network_snapshots";
				output_network_snapshots_file<fstream&>().open(routable_network_snapshots_file_name, fstream::out);
				output_network_snapshots_file<fstream&>() << "time\t maximum_free_flow_speed" << endl;
				output_network_snapshots_file<fstream&>() << "inbound_link_uuid\t inbound_link_travel_time" << endl;
				output_network_snapshots_file<fstream&>() << "movement_uuid\t movement_forward_link_turn_travel_time";
			};

			feature_prototype void open_input_files()
			{
				//reference network moe file
				string reference_realtime_network_moe_file_name = input_dir_name<string&>() + "realtime_moe_network_to_compare.csv";
				reference_realtime_network_moe_file<fstream&>().open(reference_realtime_network_moe_file_name, fstream::in);
				if (!reference_realtime_network_moe_file<fstream&>().is_open())
				{
					cout << "Reference network MOE file not found. There will be no hisotorical side-by-side plotting in Antares." << endl;
				}

				if (read_network_snapshots<bool>())
				{
					string routable_network_snapshots_file_name = input_dir_name<string&>() + "input_network_snapshots";
					input_network_snapshots_file<fstream&>().open(routable_network_snapshots_file_name, fstream::in);
					if (!input_network_snapshots_file<fstream&>().is_open())
					{
						THROW_EXCEPTION(endl << "read_network_snapshots is enabled but network snapshots file cannot be opened." << endl);
					}
				}
			}

			feature_prototype void close_files()
			{
				close_output_files<NULLTYPE>();
				close_input_files<NULLTYPE>();
			}

			feature_prototype void close_output_files()
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
			};

			feature_prototype void close_input_files()
			{
				reference_realtime_network_moe_file<fstream&>().close();
				input_network_snapshots_file<fstream&>().close();
			}


			feature_prototype void increase_network_cumulative_loaded_vehicles()
			{
				this_component()->template increase_network_cumulative_loaded_vehicles<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void increase_network_cumulative_departed_vehicles()
			{
				this_component()->template increase_network_cumulative_departed_vehicles<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void increase_network_in_network_vehicles()
			{
				this_component()->template increase_network_in_network_vehicles<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void decrease_network_in_network_vehicles()
			{
				this_component()->template decrease_network_in_network_vehicles<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void increase_network_cumulative_switched_decisions()
			{
				this_component()->template increase_network_cumulative_switched_decisions<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void increase_network_cumulative_arrived_vehicles()
			{
				this_component()->template increase_network_cumulative_arrived_vehicles<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void output(std::string s)
			{
				typedef typename ComponentType::output_writer_type OutputWriterType;
				//output_writer<OutputWriterType&>() << s;
				cout << s;
				cout.flush();
			}

			// convert from meter to foot
			feature_prototype float meterToFoot(float meterValue)
			{
				// 1 meter = 3.28084 feet
				return 3.28084 * meterValue;
			}

			// convert from meters per second to miles per hour
			feature_prototype float mepsToMiph(float mepsValue)
			{
				// 1 meter = 0.000621371 mile
				return (3.28084 * mepsValue * 3600.0 / 5280.0);
			}

			feature_prototype int moe_update_interval()
			{
				return _assignment_interval_length;
			}
		};
	}
}

using namespace Scenario_Components::Prototypes;