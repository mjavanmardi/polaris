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
				START_OF_ITERATION=0,
				TRAVELER_SET_DEPARTURE_SUB_ITERATION,
				ROUTING_SUB_ITERATION,
				CONTROL_SUB_ITERATION,
				LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION,
				INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION,
				LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION,
				INTERSECTION_NETWORK_STATE_UPDATE_SUB_ITERATION,
				END_OF_ITERATION,
			};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Scenario_Prototype
		{
			tag_as_prototype;


			feature_accessor(current_simulation_interval_index, none, none);
			feature_accessor(simulation_interval_length, none, none);
			feature_accessor(assignment_interval_length, none, none);
			feature_accessor(current_time, none, none);

			feature_accessor(planning_horizon, none, none);
			feature_accessor(simulation_start_time, none, none);
			feature_accessor(simulation_end_time, none, none);

			feature_accessor(intersection_control_flag, none, none);
			
			feature_accessor(num_simulation_intervals, none, none);
			feature_accessor(num_assignment_intervals, none, none);
			feature_accessor(num_simulation_intervals_per_assignment_interval, none, none);

			feature_accessor(assignment_mode, none, none);
			feature_accessor(num_threads, none, none);
			feature_accessor(iseed, none, none);
						
			feature_accessor(demand_od_flag, none, none);
			feature_accessor(io_source_flag, none, none);

			feature_accessor(path_calculation_interval_length, none, none);
			feature_accessor(current_assignment_interval_index, none, none);
			feature_accessor(current_simulation_time, none, none);
			feature_accessor(current_day_index, none, none);
			

			feature_accessor(output_writer, none, none);

			feature_accessor(network_cumulative_loaded_vehicles, none, none);
			feature_accessor(network_cumulative_departed_vehicles, none, none);
			feature_accessor(network_in_network_vehicles, none, none);
			feature_accessor(network_cumulative_arrived_vehicles, none, none);

			feature_accessor(input_dir_name, none, none);
			feature_accessor(output_dir_name, none, none);
				
			feature_accessor(vehicle_trajectory_file_name, none, none);
			feature_accessor(vehicle_trajectory_file, none, none);

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

			feature_accessor(assignment_time_in_seconds, none, none);
			feature_accessor(simulation_time_in_seconds, none, none);
			feature_accessor(operation_time_in_seconds, none, none);
			feature_accessor(output_time_in_seconds, none, none);
			feature_accessor(condition_time_in_seconds, none, none);

			feature_prototype void read_scenario_data()
			{
				CfgReader cfgReader;
				cfgReader.initialize("scenario.json");

				string start_time_in_string;
				cfgReader.getParameter("starting_time_hh_mm", &start_time_in_string);
				start_time_in_string += ":00";
				int start_time = convert_hhmmss_to_seconds(start_time_in_string);
				//assert(start_time == 0); // to be done for start time > 0
				simulation_start_time<int>(start_time);
				
				string end_time_in_string;
				cfgReader.getParameter("ending_time_hh_mm", &end_time_in_string);
				end_time_in_string += ":00";
				int end_time = convert_hhmmss_to_seconds(end_time_in_string);
				simulation_end_time<int>(end_time);

				planning_horizon<int>(end_time - start_time);

				cfgReader.getParameter("simulation_interval_length_in_second", simulation_interval_length<int*>());
				int assignment_intervals;
				cfgReader.getParameter("num_simulation_intervals_per_assignment_interval", &assignment_intervals);
				assignment_interval_length<int>(assignment_intervals*simulation_interval_length<int>());

				string assignment_mode_string;
				cfgReader.getParameter("assignment_mode", &assignment_mode_string);
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

				cfgReader.getParameter("seed", iseed<unsigned long*>());

				cfgReader.getParameter("num_threads", num_threads<int*>());

				cfgReader.getParameter("node_control_flag", intersection_control_flag<int*>());

				cfgReader.getParameter("demand_od_flag", demand_od_flag<int*>());

				string io_source_string;
				cfgReader.getParameter("io_source_flag", &io_source_string);
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

				assignment_time_in_seconds<double>(0.0);
			    simulation_time_in_seconds<double>(0.0);
			    operation_time_in_seconds<double>(0.0);
			    output_time_in_seconds<double>(0.0);
				
				output_dir_name<string&>() = "";
				open_output_files<NULLTYPE>();
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

				current_simulation_interval_index<int>(scenario_data.current_simulation_interval_index);
				current_assignment_interval_index<int>(scenario_data.current_assignment_interval_index);
				current_day_index<int>(scenario_data.current_day_index);

				assignment_mode<int>(scenario_data.assignment_mode);

				iseed<unsigned long>(scenario_data.iseed);

				intersection_control_flag<int>(scenario_data.node_control_flag);

				output_dir_name<string&>() = "";
				open_output_files<NULLTYPE>();

				network_cumulative_loaded_vehicles<int>(0.0);
				network_cumulative_departed_vehicles<int>(0.0);
				network_in_network_vehicles<int>(0.0);
				network_cumulative_arrived_vehicles<int>(0.0);

				assignment_time_in_seconds<double>(0.0);
			    simulation_time_in_seconds<double>(0.0);
			    operation_time_in_seconds<double>(0.0);
			    output_time_in_seconds<double>(0.0);
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
				scenario_data.current_simulation_interval_index = current_simulation_interval_index<int>();
				scenario_data.current_assignment_interval_index = current_assignment_interval_index<int>();
				scenario_data.current_time = current_time<int>();	//from 00::00:00
				scenario_data.current_simulation_time = current_simulation_time<int>(); // from simulation_start_time
				scenario_data.current_day_index = current_day_index<int>();

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
						<< "assignment_time_in_seconds" << ","
						<< "simulation_time_in_seconds" << ","
						<< "operation_time_in_seconds" << ","
						<< "output_time_in_seconds"
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


			};

			feature_prototype void close_output_files()
			{
				vehicle_trajectory_file<fstream&>().close();
				network_link_flow_file<fstream&>().close();
				network_link_turn_time_file<fstream&>().close();
				network_node_control_state_file<fstream&>().close();
				output_summary_file<fstream&>().close();
				vehicle_transfer_file<fstream&>().close();
			};

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
				return meterValue * 3.28084;
			}

			// convert from meters per second to miles per hour
			feature_prototype float mepsToMiph(float mepsValue)
			{
				// 1 meter = 0.000621371 mile
				return mepsValue * 0.000621371 * 3600;
			}

		};
	}
}

using namespace Scenario_Components::Prototypes;
static void *_global_scenario;