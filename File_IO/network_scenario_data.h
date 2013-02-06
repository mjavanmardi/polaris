#ifndef NETWORK_SCENARIO_H
#define NETWORK_SCENARIO_H

#include "utilities.h"

//#include <map>

//#ifdef LINUX
//// for map
//using namespace __gnu_cxx;
//#endif
using namespace std;

namespace network_models
{

	namespace network_information
	{
		namespace scenario_data_information
		{
			enum Assignment_Simulation_Mode_Keys
			{
				ONE_SHOT_ASSIGNMENT_SIMULATION_MODE=0,
				ITERATIVE_ASSIGNMENT_SIMULATION_MODE,
				CONVERT_DATA_MODE,
			};

			enum Input_Data_Format_Keys
			{
				INPUT_FROM_OD = 0,
				INPUT_FROM_VEHICLE,
				INPUT_FROM_DSP,
				INPUT_FROM_TSM,
				INPUT_FROM_DB,
			};

			struct ScenarioData
			{
				//time
				int simulation_interval_length;	// in second
				int assignment_interval_length; //in second
				int path_calculation_interval_length;

				int simulation_start_time;	//
				int simulation_end_time;	//

				int planning_horizon; //
				int num_simulation_intervals;
				int num_assignment_intervals;
				int num_simulation_intervals_per_assignment_interval;

				int current_simulation_interval_index;
				int current_assignment_interval_index;
				int current_time;	//from 00::00:00
				int current_simulation_time; // from simulation_start_time
				int current_day_index;

				Assignment_Simulation_Mode_Keys assignment_mode;

				map<Assignment_Simulation_Mode_Keys,string> assignment_simulation_mode_int_string_map;
				map<string,Assignment_Simulation_Mode_Keys> assignment_simulation_mode_string_int_map;
				map<Input_Data_Format_Keys,string> input_data_format_int_string_map;
				map<string,Input_Data_Format_Keys> input_data_format_string_int_map;

				unsigned long iseed;

				int num_threads;

				int node_control_flag;

				//directory
				string input_dir_name;
				string output_dir_name;
				
				//files
				string vehicle_trajectory_file_name;
				fstream vehicle_trajectory_file;

				string network_link_flow_file_name;
				fstream network_link_flow_file;

				string network_link_turn_time_file_name;
				fstream network_link_turn_time_file;

				string network_node_control_state_file_name;
				fstream network_node_control_state_file;

				string error_file_name;
				fstream error_file;

				string warning_file_name;
				fstream warning_file;

				string summary_file_name;
				fstream summary_file;

				string demand_vehicle_file_name;
				fstream demand_vehicle_file;

				string vehicle_path_file_name;
				fstream vehicle_path_file;

				Input_Data_Format_Keys input_data_format;

				int demand_od_flag;
			};

			void scenario_data_initialization(ScenarioData& scenario_data);

			void read_scenario_data_test(ScenarioData& scenario_data);

			void read_scenario_data(string input_dir_name,ScenarioData& scenario_data);
			void write_scenario_data(ScenarioData& scenario_data);

			void read_scenario_data_dsp(string input_dir_name,ScenarioData& scenario_data);
		}
	}
}
#endif