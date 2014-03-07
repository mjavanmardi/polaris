#pragma once
#include "../Scenario_Manager/cfg_reader.h"
#include "User_Space_Includes.h"
//#include "../File_IO/network_scenario_data.h"

#define INFINITY_FLOAT 9999999

//left substring
string left(string s, int pos)
{
	s = s.substr(0,pos);
	return s;
};

//right substring
string right(string s, int pos)
{
	s = s.substr(pos,(int)s.size());
	return s;
};

//extract substring
string substring(string s, int pos1, int pos2)
{
	s = s.substr(pos1, pos2);
	return s;
};

//convert hh:mm:ss to seconds
int convert_hhmmss_to_seconds(string hhmmss)
{
	string hh,mm,ss;
	//int pos = (int) hhmmss.find(":");
	int pos = 2;
	hh = left(hhmmss,pos);
	hhmmss = right(hhmmss,pos+1);

	//pos = (int) hhmmss.find(":");
	pos = 2;
	mm = left(hhmmss,pos);
	ss = right(hhmmss,pos+1);
	int h = stoi(hh);
	int m = stoi(mm);
	int s = stoi(ss);

	int time_in_seconds = h*3600 + m*60 + s;
	
	return time_in_seconds;
};

//convert hh:mm:ss to seconds
int convert_hhmm_to_seconds(string hhmm)
{
	string hh,mm;
	int pos = 2;

	hh = left(hhmm,pos);
	mm = right(hhmm,pos+1);
	
	int h = stoi(hh);
	int m = stoi(mm);

	int time_in_seconds = h*3600 + m*60;
	
	return time_in_seconds;
};

void string_split(boost::container::vector<std::string>& results, const std::string &source, const int fields)
{
	results.clear();
	results.resize(fields);

	const char* s = source.c_str();
	const char* s2;
	int count;
	int pos = 0;
	while (pos < fields)
	{
		count = 0;
		while(*s != '\0' && (*s == '\t' || *s == ' ' || *s == '\r' || *s == ',')) s++;
		if (*s == '\0')
		{
			break;
		}
		s2 = s;
		while(*s2 != '\0' && *s2 != '\t' && *s2 != ' ' && *s2 != '\r' && *s2 != ',')
		{
			s2++;
			count++;
		}
		results[pos].assign(s, count);
		s = s2;
		pos++;
	}
};

void string_split(boost::container::vector<std::string>& results, const std::string &source)
{
	results.clear();

	const char* s = source.c_str();
	const char* s2;
	int count;
	while (true)
	{
		count = 0;
		while(*s != '\0' && (*s == '\t' || *s == ' ' || *s == '\r' || *s == ',')) s++;
		if (*s == '\0')
		{
			break;
		}
		s2 = s;
		while(*s2 != '\0' && *s2 != '\t' && *s2 != ' ' && *s2 != '\r' && *s2 != ',')
		{
			s2++;
			count++;
		}
		string str;
		str.assign(s, count);
		results.push_back(str);
		s = s2;
	}
};

//convert seconds to hh:mm:ss
string convert_seconds_to_hhmmss(int time_in_seconds)
{
	long long h = time_in_seconds/3600;
	time_in_seconds = time_in_seconds%3600;
	long long m = time_in_seconds/60;
	time_in_seconds = time_in_seconds%60;
	long long s = time_in_seconds;
	string hh = to_string(h);
	if (hh.size()==1)
	{
		hh = "0" + hh;
	}
	string mm = to_string(m);
	if (mm.size()==1)
	{
		mm = "0" + mm;
	}

	string ss = to_string(s);
	if (ss.size()==1)
	{
		ss = "0" + ss;
	}

	string hhmmss = hh + ":" + mm + ":" + ss;
	return hhmmss;
};

string convert_seconds_to_hhmm(int time_in_seconds)
{
	long long h = time_in_seconds/3600;
	time_in_seconds = time_in_seconds%3600;
	long long m = time_in_seconds/60;
	time_in_seconds = time_in_seconds%60;
	long long s = time_in_seconds;
	string hh = to_string(h);
	if (hh.size()==1)
	{
		hh = "0" + hh;
	}
	string mm = to_string(m);
	if (mm.size()==1)
	{
		mm = "0" + mm;
	}

	string ss = to_string(s);
	if (ss.size()==1)
	{
		ss = "0" + ss;
	}

	string hhmm = hh + ":" + mm;
	return hhmm;
};

void calculate_mean_standard_deviation(const boost::container::vector<float>& data_array, float& mean, float& standard_deviation)
{
	int array_size = int(data_array.size());
	if (array_size>1)
	{
		mean = 0.0f;
		for (int i=0;i<array_size;i++)
		{
			mean += data_array[i];
		}
		mean = float (mean/(float(array_size)));
		standard_deviation = 0.0f;
		for (int i=0;i<array_size;i++)
		{
			standard_deviation += (data_array[i] - mean)*(data_array[i] - mean);
		}

		standard_deviation = float(sqrt(double(standard_deviation/float(array_size-1))));
	}
	else
	{
		if (array_size ==1)
		{
			mean = data_array[0];
			standard_deviation = 0.0f;
		}
		else
		{
			mean = 0.0f;
			standard_deviation = 0.0f;
		}
	}
};

void calculate_mean(const boost::container::vector<float>& data_array, float& mean)
{
	int array_size = int(data_array.size());
	if (array_size>1)
	{
		mean = 0.0f;
		for (int i=0;i<array_size;i++)
		{
			mean += data_array[i];
		}
		mean = float (mean/(float(array_size)));
	}
	else
	{
		if (array_size ==1)
		{
			mean = data_array[0];
		}
		else
		{
			mean = 0.0f;
		}
	}
};

#ifdef __GNUC__
	double get_current_cpu_time_in_seconds()
	{
		struct timespec current_cpu_time;
		double current_cpu_time_in_seconds;
		clock_gettime(CLOCK_REALTIME, &current_cpu_time);

		current_cpu_time_in_seconds = (double)((current_cpu_time.tv_sec * 1000000000 + current_cpu_time.tv_nsec)) / 1000000000.0;
		return current_cpu_time_in_seconds;
	};
#else
	double get_current_cpu_time_in_seconds()
	{
		LARGE_INTEGER current_cpu_time;
		LARGE_INTEGER query_performance_frequency;
		double current_cpu_time_in_seconds;

		QueryPerformanceFrequency(&query_performance_frequency);
		QueryPerformanceCounter(&current_cpu_time);

		current_cpu_time_in_seconds =(double) (current_cpu_time.QuadPart/((double)query_performance_frequency.QuadPart));	
		return current_cpu_time_in_seconds;
	};
#endif

// #ifdef __GNUC__
// #include <unistd.h>
// #include <ios>
// #include <iostream>
// #include <fstream>
// #include <string>
// void mem_info(long long& totalPhysicalMemory, long long& physicalMemoryUsedByProcess)
// {
   // using ios_base;
   // using ifstream;
   // using string;

   // // 'file' stat seems to give the most reliable results
   // //
   // ifstream stat_stream("/proc/self/stat",ios_base::in);

   // // dummy vars for leading entries in stat that we don't care about
   // //
   // string pid, comm, state, ppid, pgrp, session, tty_nr;
   // string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   // string utime, stime, cutime, cstime, priority, nice;
   // string O, itrealvalue, starttime;

   // // the two fields we want
   // //
   // unsigned long vsize;
   // long rss;

   // stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
			   // >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
			   // >> utime >> stime >> cutime >> cstime >> priority >> nice
			   // >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   // stat_stream.close();

   // long page_size = sysconf(_SC_PAGE_SIZE) ; // in case x86-64 is configured to use 2MB pages
   // totalPhysicalMemory = sysconf(_SC_PHYS_PAGES) * page_size;
   // physicalMemoryUsedByProcess = rss * page_size;
// }
// #else
// #include "windows.h"
// #include "psapi.h"
// void mem_info(long long& totalPhysicalMemory, long long& physicalMemoryUsedByProcess)
// {
	// PROCESS_MEMORY_COUNTERS_EX pmc;
	// MEMORYSTATUSEX memInfo;
	// if (!GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
	// {
		// physicalMemoryUsedByProcess = -1;
	// }
	// else
	// {
		// physicalMemoryUsedByProcess = (long long)pmc.WorkingSetSize;
		
	// }
	// memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	// if (!GlobalMemoryStatusEx(&memInfo))
	// {
		// totalPhysicalMemory = -1;
	// }
	// else
	// {
		// totalPhysicalMemory = (long long)memInfo.ullTotalPhys;
	// }
// }
// #endif



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
				RAMP_METERING_SUB_ITERATION,
				CONTROL_SUB_ITERATION,
				/* update link supply */
				LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION,
				/* 1. update allowed flow for each turn movement.
				 * 2. transfer vehicle */
				INTERSECTION_COMPUTE_STEP_FLOW_SUB_ITERATION,
				/* vehicles take action for transferring */
				VEHICLE_ACTION_TRANSFER_SUB_ITERATION,
				/* origin link loading */
				LINK_ORIGIN_LINK_LOADING_SUB_ITERATION,
				/* vehicles take action for origin loading */
				VEHICLE_ACTION_ORIGIN_LOADING_SUB_ITERATION,
				/* link moving */
				LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION,
				INTERSECTION_NETWORK_STATE_UPDATE_SUB_ITERATION,
				INTERSECTION_REALTIME_MOE_COMPUTATION_SUB_ITERATION,
				INTERSECTION_MOE_COMPUTATION_SUB_ITERATION,
				MOE_COMPUTATION_SUB_ITERATION,
				MOE_VISUALIZATION_SUB_ITERATIONS,
				END_OF_ITERATION=40,
			};
			enum Demand_Sub_Iteration_keys
			{
				PRETRIP_INFORMATION_ACQUISITION = 14,
				PRETRIP_PLANNING_SUB_ITERATION,
				PRETRIP_ROUTING_SUB_ITERATION,
				ACTIVITY_ATTRIBUTE_PLANNING_SUB_ITERATION,
			};
			enum RNG_Type_Keys
			{
				DETERMINISTIC = 0,
				RANDOM,
			};

			enum Merging_Mode_Keys
			{
				DRIVING_RULE = 0,
				PROPORTION_TO_DEMAND,
				PROPORTION_TO_LINK,
				PROPORTION_TO_LANE,
			};

			enum Cause_For_Enroute_Switching
			{
				EXCESSIVE_DELAY = 0,
				REALTIME_INFORMED,
				ITS_INFORMED
			};
	}

	namespace Prototypes
	{
		prototype struct Scenario ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(output_results_database_name, NONE, NONE);
			accessor(output_demand_database_name, NONE, NONE);


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
				
			accessor(vehicle_trajectory_file_name, NONE, NONE);
			accessor(vehicle_trajectory_file, NONE, NONE);

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
			accessor(write_skim_tables, NONE, NONE);
			accessor(read_skim_tables, NONE, NONE);
			accessor(input_highway_skim_file_path_name, NONE, NONE);	
			accessor(output_highway_skim_file_path_name, NONE, NONE);	
			accessor(input_highway_cost_skim_file_path_name, NONE, NONE);	
			accessor(output_highway_cost_skim_file_path_name, NONE, NONE);
			accessor(input_transit_skim_file_path_name, NONE, NONE);	
			accessor(output_transit_skim_file_path_name, NONE, NONE);

			accessor(compare_with_historic_moe, NONE, NONE);
			accessor(historic_network_moe_file_path_name, NONE, NONE);
			accessor(historic_link_moe_file_path_name, NONE, NONE);
			accessor(read_normal_day_link_moe, NONE, NONE);
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
				if (cfgReader.getParameter("merging_mode", &merging_mode_string) != PARAMETER_FOUND) merging_mode_string = "PROPORTION_TO_LANE";
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
				// Demand model parameters 
				if (cfgReader.getParameter("write_activity_output", this->write_activity_output<bool*>()) != PARAMETER_FOUND) this->write_activity_output<bool>(false);
				if (cfgReader.getParameter("aggregate_routing", this->aggregate_routing<bool*>()) != PARAMETER_FOUND) this->aggregate_routing<bool>(false);
				if (cfgReader.getParameter("do_planner_routing", this->do_planner_routing<bool*>()) != PARAMETER_FOUND) this->do_planner_routing<bool>(false);		
				if (cfgReader.getParameter("write_demand_to_database", this->write_demand_to_database<bool*>()) != PARAMETER_FOUND) this->write_demand_to_database<bool>(false);
				if (cfgReader.getParameter("read_demand_from_database", this->read_demand_from_database<bool*>()) != PARAMETER_FOUND) this->read_demand_from_database<bool>(false);

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
					if (cfgReader.getParameter("database_name", database_name<string*>())!= PARAMETER_FOUND) database_name<std::string>("chicago");
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
				if (cfgReader.getParameter("output_dir_name", output_dir_name<string*>())!= PARAMETER_FOUND) output_dir_name<string>("");
				if (cfgReader.getParameter("num_threads", num_threads<int*>())!= PARAMETER_FOUND) num_threads<int>(1);
				if (cfgReader.getParameter("write_db_input_to_files", write_db_input_to_files<bool*>())!= PARAMETER_FOUND) write_db_input_to_files<bool>(false);
				if (cfgReader.getParameter("run_simulation_for_db_input", run_simulation_for_db_input<bool*>())!= PARAMETER_FOUND) run_simulation_for_db_input<bool>(true);
				if (cfgReader.getParameter("write_node_control_state", write_node_control_state<bool*>())!= PARAMETER_FOUND) write_node_control_state<bool>(false);
				if (cfgReader.getParameter("write_vehicle_trajectory", write_vehicle_trajectory<bool*>())!= PARAMETER_FOUND) write_vehicle_trajectory<bool>(false);
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
				
				if (cfgReader.getParameter("write_skim_tables", this->write_skim_tables<bool*>()) != PARAMETER_FOUND) this->write_skim_tables<bool>(false);
				if (cfgReader.getParameter("read_skim_tables", this->read_skim_tables<bool*>()) != PARAMETER_FOUND) this->read_skim_tables<bool>(false);
				if (cfgReader.getParameter("input_highway_skim_file_path_name", this->input_highway_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->input_highway_skim_file_path_name<string>((string)"");
				if (cfgReader.getParameter("output_highway_skim_file_path_name", this->output_highway_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->output_highway_skim_file_path_name<string>((string)"highway_skim_file_out.txt");
				if (cfgReader.getParameter("input_highway_cost_skim_file_path_name", this->input_highway_cost_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->input_highway_cost_skim_file_path_name<string>((string)"");
				if (cfgReader.getParameter("output_highway_cost_skim_file_path_name", this->output_highway_cost_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->output_highway_cost_skim_file_path_name<string>((string)"highway_cost_skim_file_out.txt");
				if (cfgReader.getParameter("input_transit_skim_file_path_name", this->input_transit_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->input_transit_skim_file_path_name<string>((string)"");
				if (cfgReader.getParameter("output_transit_skim_file_path_name", this->output_transit_skim_file_path_name<string*>()) != PARAMETER_FOUND) this->output_transit_skim_file_path_name<string>((string)"transit_skim_file_out.txt");


				if (cfgReader.getParameter("compare_with_historic_moe", compare_with_historic_moe<bool*>())!= PARAMETER_FOUND) compare_with_historic_moe<bool>(false);
				if (cfgReader.getParameter("historic_network_moe_file_path_name", historic_network_moe_file_path_name<string*>())!= PARAMETER_FOUND) historic_network_moe_file_path_name<string>("historic_realtime_moe_network.csv");
				if (cfgReader.getParameter("historic_link_moe_file_path_name", historic_link_moe_file_path_name<string*>())!= PARAMETER_FOUND) historic_link_moe_file_path_name<string>("historic_moe_link.csv");
				if (cfgReader.getParameter("read_normal_day_link_moe", read_normal_day_link_moe<bool*>())!= PARAMETER_FOUND) read_normal_day_link_moe<bool>(false);
				if (cfgReader.getParameter("normal_day_link_moe_file_path_name", normal_day_link_moe_file_path_name<string*>())!= PARAMETER_FOUND) normal_day_link_moe_file_path_name<string>("normal_day_moe_link.csv");
				if (cfgReader.getParameter("historic_demand_moe_directory", historic_demand_moe_directory<string*>())!= PARAMETER_FOUND) historic_demand_moe_directory<string>("");

				if (cfgReader.getParameter("output_link_moe_for_assignment_interval", output_link_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) output_link_moe_for_assignment_interval<bool>(false);
				if (cfgReader.getParameter("output_turn_movement_moe_for_assignment_interval", output_turn_movement_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) output_turn_movement_moe_for_assignment_interval<bool>(false);
				if (cfgReader.getParameter("output_network_moe_for_assignment_interval", output_network_moe_for_assignment_interval<bool*>())!= PARAMETER_FOUND) output_network_moe_for_assignment_interval<bool>(true);
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
				if (cfgReader.getParameter("vehicle_taking_action", vehicle_taking_action<bool*>())!= PARAMETER_FOUND) vehicle_taking_action<bool>(true);

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
				if (cfgReader.getParameter("minimum_delay_ratio_for_enroute_switching", minimum_delay_ratio_for_enroute_switching<double*>())!= PARAMETER_FOUND) minimum_delay_ratio_for_enroute_switching<double>(2.0f);
				if (cfgReader.getParameter("minimum_delay_seconds_for_enroute_switching", minimum_delay_seconds_for_enroute_switching<double*>())!= PARAMETER_FOUND) minimum_delay_seconds_for_enroute_switching<double>(300.0f);
				if (cfgReader.getParameter("enroute_switching_on_excessive_delay", enroute_switching_on_excessive_delay<bool*>())!= PARAMETER_FOUND) enroute_switching_on_excessive_delay<bool>(true);
				if (cfgReader.getParameter("multimodal_network_input", multimodal_network_input<bool*>())!= PARAMETER_FOUND) multimodal_network_input<bool>(false);
				if (cfgReader.getParameter("enroute_excessive_delay_factor", enroute_excessive_delay_factor<double*>())!= PARAMETER_FOUND) enroute_excessive_delay_factor<double>(1.0);
				if (cfgReader.getParameter("minimum_seconds_from_arrival_for_enroute_switching", minimum_seconds_from_arrival_for_enroute_switching<double*>())!= PARAMETER_FOUND) minimum_seconds_from_arrival_for_enroute_switching<double>(120.0f);

				


				if (cfgReader.getParameter("calculate_realtime_moe", calculate_realtime_moe<bool*>())!= PARAMETER_FOUND) calculate_realtime_moe<bool>(true);
				
				if (cfgReader.getParameter("buildings_geometry_file", buildings_geometry_file<string*>())!= PARAMETER_FOUND) buildings_geometry_file<string&>()="";
				

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


				//vehicle trajectory
				vehicle_trajectory_file_name<string&>().assign(output_dir_name<string&>() + "vehicle_trajectory.csv");
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
						<< "travel_time" << ","
						<< "routed_travel_time" << ","
						<< "travel_time_ratio" << ","
						<< "trip_length" << ","
						<< "num_switches" << ","
						<< "loading_delay" << ","
						<< "entry_queue_length" << ","
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
				out_realtime_link_moe_file<fstream&>() << "clock,time,link,dbid,direction,unode,dnode,link_type,travel_time_in_min,travel_delay_in_min,queue_length,speed_in_mph,density_in_vpmpl,in_volume,out_volume,travel_time_ratio,speed_ratio,density_ratio\n";

				//movement
				string out_realtime_movement_moe_file_name = output_dir_name<string&>() + "realtime_moe_movement.csv";
				out_realtime_movement_moe_file<fstream&>().open(out_realtime_movement_moe_file_name, fstream::out);
				out_realtime_movement_moe_file<fstream&>() << "clock,time,turn_movement,inbound_link,outbound_link,node,turn_penalty_in_min,turn_penalty_sd_in_min,inbound_link_turn_time_in_min,outbound_link_turn_time_in_min,movement_flow_rate_in_vphpl\n";

				//moe
				//network
				string out_network_moe_file_name = output_dir_name<string&>() + "moe_network.csv";
				out_network_moe_file<fstream&>().open(out_network_moe_file_name, fstream::out);
				//out_network_moe_file<fstream&>() << "clock,time,num_loaded_vehicle,num_departed_vehicle,num_arrived_vehicle,avg_link_time_in_min,avg_link_speed_in_mph,avg_link_density_in_vpmpl,avg_link_in_flow_rate_in_vphpl,avg_link_out_flow_rate_in_vphpl,avg_link_in_volume,avg_link_out_volume,avg_link_time_ratio,avg_link_speed_ratio,avg_link_density_ratio,avg_link_in_flow_ratio,avg_link_out_flow_ratio,vht,vmt,assignment_calculation_time_in_second,simulation_calculation_time_in_second,operation_calculation_time_in_second,output_calculation_time_in_second\n";
				out_network_moe_file<fstream&>() << "clock,time,num_loaded_vehicle,num_departed_vehicle,num_arrived_vehicle,avg_link_time_in_min,avg_link_speed_in_mph,avg_link_density_in_vpmpl,avg_link_in_flow_rate_in_vphpl,avg_link_out_flow_rate_in_vphpl,avg_link_in_volume,avg_link_out_volume,avg_link_time_ratio,avg_link_speed_ratio,avg_link_density_ratio,avg_link_in_flow_ratio,avg_link_out_flow_ratio,vmt,vht\n";

				//link
				string out_link_moe_file_name = output_dir_name<string&>() + "moe_link.csv";
				out_link_moe_file<fstream&>().open(out_link_moe_file_name, fstream::out);
				out_link_moe_file<fstream&>() << "clock,time,link,dbid,direction,unode,dnode,link_type,travel_time_in_min,travel_time_sd_in_min,travel_delay_in_min,travel_delay_sd_in_min,queue_length,speed_in_mph,density_in_vpmpl,in_flow_rate_in_vphpl,out_flow_rate_in_vphpl,in_volume,out_volume,travel_time_ratio,speed_ratio,density_ratio,in_flow_ratio,out_flow_ratio,vht,vmt\n";

				//movement
				string out_movement_moe_file_name = output_dir_name<string&>() + "moe_movement.csv";
				out_movement_moe_file<fstream&>().open(out_movement_moe_file_name, fstream::out);
				out_movement_moe_file<fstream&>() << "clock,time,turn_movement,inbound_link,outbound_link,node,turn_penalty_in_min,turn_penalty_sd_in_min,inbound_link_turn_time_in_min,outbound_link_turn_time_in_min,movement_flow_rate_in_vphpl\n";

				//routable network snapshot
				string routable_network_snapshots_file_name = output_dir_name<string&>() + "output_network_snapshots";
				output_network_snapshots_file<fstream&>().open(routable_network_snapshots_file_name, fstream::out);
				output_network_snapshots_file<fstream&>() << "time\t maximum_free_flow_speed" << endl;
				output_network_snapshots_file<fstream&>() << "inbound_link_uuid\t inbound_link_dbid\t inbound_link_direction\t inbound_link_travel_time\t number_of_movements" << endl;
				output_network_snapshots_file<fstream&>() << "movement_uuid\t movement_forward_link_turn_travel_time";

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
