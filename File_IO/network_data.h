#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#include "network_node_data.h"
#include "network_link_data.h"
#include "network_turn_movement_data.h"
#include "network_activity_location_data.h"
#include "network_zone_data.h"
#include "network_route_data.h"

#include "utilities.h"
//
#ifdef LINUX
// for map
using namespace __gnu_cxx;
#endif



using namespace std;
using namespace network_models::network_information::route_data_information;

namespace network_models
{
	//#define INFINITY_FLOAT 9999999
	namespace network_information
	{
		namespace network_data_information
		{
			struct NetworkData
			{
				int network_node_size;
				int network_link_size;
				int network_turn_movement_size;
				int network_activity_location_size;
				int network_zone_size;
				int network_path_size;

				float max_free_flow_speed;

				///topology
				vector<NodeData> node_data_array;
				vector<LinkData> link_data_array;
				vector<TurnMovementData> turn_movement_data_array;
				vector<ActivityLocationData> activity_location_data_array;
				vector<ZoneData> zone_data_array;

				//map
				map<int,int> node_id_index_map;
				map<int,int>::iterator node_id_index_map_itr;
				map<int,int> link_id_index_map;
				map<int,int>::iterator link_id_index_map_itr;
				map<int,int> turn_movement_id_index_map;
				map<int,int>::iterator turn_movement_id_index_map_itr;
				map<int,int> zone_id_index_map;
				map<int,int>::iterator zone_id_index_map_itr;
				map<int,int> activity_location_id_index_map;
				map<int,int>::iterator activity_location_id_index_map_itr;

				//
				map<Node_Type_Keys,string> node_type_int_string_map;
				map<string,Node_Type_Keys> node_type_string_int_map;
				map<Link_Type_Keys,string> link_type_int_string_map;
				map<string,Link_Type_Keys> link_type_string_int_map;
				map<Turn_Movement_Type_Keys,string> turn_movement_type_int_string_map;
				map<string,Turn_Movement_Type_Keys> turn_movement_type_string_int_map;
				map<Turn_Movement_Rule_Keys,string> turn_movement_rule_int_string_map;
				map<string,Turn_Movement_Rule_Keys> turn_movement_rule_string_int_map;

				//node - link map
				map<pair<int,int>,int> node_link_map;
				map<pair<int,int>,int>::iterator node_link_map_itr;

				//link - turn movement map
				map<pair<int,int>,int> link_turn_movement_map;
				map<pair<int,int>,int>::iterator link_turn_movement_map_itr;

				///route
				vector<RouteData> route_data_array;

				///travel time
				vector<float> link_travel_time_array;				//in seconds
				vector<float> turn_travel_penalty_array;			//in seconds
				vector<float> forward_link_turn_travel_time_array;	//in seconds

				vector<float> td_link_travel_time_array;
				vector<float> td_forward_link_turn_travel_time_array;

				//origin vehicles
				vector<vector<int>> link_origin_vehicle_index_array;		//origin link n(a,t)
				vector<int> link_origin_cumulative_arrival_vehicles_array;  //origin link A(a,t)
			
				//cumulative curves
				vector<int> link_upstream_cumulative_arrival_curve_array;
				vector<int> link_upstream_cumulative_depature_curve_array;
				vector<int> link_downstream_cumulative_arrival_curve_array;
				vector<int> link_downstream_cumulative_depature_curve_array;
				vector<int> link_origin_cumulative_depature_curve_array;
				vector<int> link_destination_cumulative_arrival_curve_array;
			
				//queues
				vector<int> link_num_vehicles_in_queue_array;

				//
				int network_cumulative_generated_vehicles;	//generated vehicles
				int network_cumulative_loaded_vehicles;		// in entry queue
				int network_cumulative_departed_vehicles;	//departed link
				int network_cumulative_arrived_vehicles;	//out of network
				int network_in_network_vehicles;			//in network

				//
			};

			//
			void network_data_initialization(NetworkData& network_data);

			void read_network_data_transims(NetworkData& network_data);
			void read_network_data_merged_highway(NetworkData& network_data);
			void read_network_data_two_links(NetworkData& network_data);

			void construct_network_cost(NetworkData& network_data);	
			
			//input from file
			void read_node(string input_dir_name, NetworkData& network_data);
			void read_link(string input_dir_name, NetworkData& network_data);
			void read_turn_movement(string input_dir_name, NetworkData& network_data);
			void read_zone(string input_dir_name, NetworkData& network_data);
			void read_activity_location(string input_dir_name, NetworkData& network_data);
			void read_network_data(string input_dir_name, NetworkData& network_data);

			//input from dsp
			void read_network_data_dsp(string input_dir_name, NetworkData& network_data);
			void read_network_dat_data_dsp(string input_dir_name, NetworkData& network_data);
			void read_movement_dat_data_dsp(string input_dir_name, NetworkData& network_data);
			void read_xy_dat_data_dsp(string input_dir_name, NetworkData& network_data);
			void read_zone_dat_data_dsp(string input_dir_name, NetworkData& network_data);
			void read_origin_dat_data_dsp(string input_dir_name, NetworkData& network_data);
			void read_destination_dat_data_dsp(string input_dir_name, NetworkData& network_data);
			void read_control_dat_data_dsp(string input_dir_name, NetworkData& network_data);

			//output network
			void write_network_data(string output_dir_name, NetworkData& network_data);
			void write_node(string output_dir_name, NetworkData& network_data);
			void write_link(string output_dir_name, NetworkData& network_data);
			void write_turn_movement(string output_dir_name, NetworkData& network_data);
			void write_zone(string output_dir_name, NetworkData& network_data);
			void write_activity_location(string output_dir_name, NetworkData& network_data);
		}
	}
}
#endif