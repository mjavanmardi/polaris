#ifndef NETWORK_SCENARIO_H
#define NETWORK_SCENARIO_H

#include "utilities.h"

//#include <map>

//#ifdef __GNUC__
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

			enum Length_Unit_Keys
			{
				LENGTH_IN_FOOT=0,
				LENGTH_IN_METER,
				LENGTH_IN_MILE,
				LENGTH_IN_KILOMETER,
			};

			enum Speed_Unit_Keys
			{
				SPEED_IN_MILES_PER_HOUR=0,
				SPEED_IN_KILOMETERS_PER_HOUR,
				SPEED_IN_METERS_PER_SECOND,
			};

			enum Time_Unit_Keys
			{
				TIME_IN_MINUTE = 0,
				TIME_IN_SECOND,
				TIME_IN_HOUR,
			};

			enum Flow_Unit_Keys
			{
				FLOW_IN_VEHICLES_PER_HOUR_PER_LANE = 0,
				FLOW_IN_PCU_PER_HOUR_PER_LANE,
			};

			enum Density_Unit_Keys
			{
				DENSITY_IN_VEHICLES_PER_MILE_PER_LANE = 0,
				DENSITY_IN_VEHICLES_PER_KILOMETER_PER_LANE = 0,
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

				//units
				Length_Unit_Keys input_length_unit;
				Speed_Unit_Keys input_speed_unit;
				Time_Unit_Keys input_time_unit;
				Flow_Unit_Keys input_flow_unit;
				Density_Unit_Keys input_density_unit;


				map<Length_Unit_Keys,string> input_length_unit_int_string_map;
				map<string,Length_Unit_Keys> input_length_unit_string_int_map;

				map<Speed_Unit_Keys,string> input_speed_unit_int_string_map;
				map<string,Speed_Unit_Keys> input_speed_unit_string_int_map;

				map<Time_Unit_Keys,string> input_time_unit_int_string_map;
				map<string,Time_Unit_Keys> input_time_unit_string_int_map;

				map<Flow_Unit_Keys,string> input_flow_unit_int_string_map;
				map<string,Flow_Unit_Keys> input_flow_unit_string_int_map;

				map<Density_Unit_Keys,string> input_density_unit_int_string_map;
				map<string,Density_Unit_Keys> input_density_unit_string_int_map;

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