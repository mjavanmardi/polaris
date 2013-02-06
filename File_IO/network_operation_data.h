#ifndef NETWORK_OPERATION_DATA_H
#define NETWORK_OPERATION_DATA_H
#include "network_data.h"
#include "network_scenario_data.h"
#include "network_sensor_data.h"
#include "network_node_control_data.h"
#include "network_link_management_data.h"

#include "utilities.h"

using namespace std;
using namespace network_models::network_information::network_data_information;
using namespace network_models::network_information::scenario_data_information;
using namespace network_models::network_information::sensor_data_information;
using namespace network_models::network_information::operation_data_information;

namespace network_models
{
	namespace network_information
	{
		namespace operation_data_information
		{
			struct OperationData
			{
				int node_control_data_size;
				int sensor_data_size;
				int ramp_metering_data_size;
				
				vector<NodeControlData> node_control_data_array;
				vector<SensorData> sensor_data_array;
				vector<RampMeteringData> ramp_metering_data_array;

				//
				vector<int> turn_movement_merge_priority_array; //by movement
				vector<float> turn_movement_green_time_array; //by movement
				vector<float> link_green_cycle_ratio_array; //by in bound link

				map<int,string> movement_priority_type_int_string_map;
				map<string,int> movement_priority_type_string_int_map;

				map<int,string> approach_type_int_string_map;
				map<string,int> approach_type_string_int_map;

			};

			//
			void opearation_data_initialization(OperationData& operation_data);

			//data interface
			void read_operation_data(string input_dir_name,ScenarioData& scenario_data, OperationData& operation_data, NetworkData &network_data);
			
			void read_node_control_data(string input_dir_name,ScenarioData& scenario_data, OperationData & operation_data, NetworkData &network_data);
			void read_sensor_data(string input_dir_name, OperationData & operation_data, NetworkData &network_data);
			
			void write_operation_data(string output_dir_name,ScenarioData& scenario_data, OperationData & operation_data, NetworkData &network_data);
			void write_node_control_data(string output_dir_name,ScenarioData& scenario_data, OperationData & operation_data, NetworkData &network_data);

			void read_control_dat_dsp(string input_dir_name,ScenarioData& scenario_data, OperationData& operation_data, NetworkData &network_data);
			void read_operation_data_dsp(string input_dir_name,ScenarioData& scenario_data, OperationData& operation_data, NetworkData &network_data);
		}
	}
}
#endif