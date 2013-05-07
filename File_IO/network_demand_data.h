#ifndef NETWORK_DEMAND_H
#define NETWORK_DEMAND_H
#include "network_data.h"
#include "network_scenario_data.h"
#include "network_vehicle_data.h"
#include "utilities.h"

#ifdef __GNUC__
// for map
using namespace __gnu_cxx;
#endif

using namespace std;
using namespace network_models::network_information::demand_data_information;
using namespace network_models::network_information::network_data_information;
using namespace network_models::network_information::scenario_data_information;

namespace network_models
{
	namespace network_information
	{
		namespace demand_data_information
		{
			///demand
			struct DemandData
			{
				int demand_vehicle_size;

				vector<VehicleData> vehilce_data_array;
				
				//
				//int first_loaded_vehicle_index;
				//int last_loaded_vehicle_index;
				int first_vehicle_departure_time;
				int last_vehicle_departure_time;
				//map
				map<int,int> vehicle_id_index_map;
				map<int,int>::iterator vehicle_id_index_map_itr;
				
				//time-varying vehicle demand
				vector<vector<int>> time_dependent_vehicle_index_array;

				//
				vector<float> time_of_day_hourly_pattern;

			};

			void read_demand_data(ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);
			void read_demand_data_two_links(ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);
			void read_demand_data_transims(ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);

			void read_demand_od(string input_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);
			
			void read_demand_od_tod(string input_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);

			int origin_activity_location_choice_model(int origin_zone_index, NetworkData& network_data, float r1);
			int destination_activity_location_choice_model(int destination_zone_index, NetworkData& network_data, float r1);

			int origin_link_choice_model(int origin_activity_location_index, NetworkData& network_data, float r1);
			int destination_link_choice_model(int destination_activity_location_index, NetworkData& network_data, float r1);

			void read_demand_vehicle_dsp(string input_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);
			void read_vehicle_dat_dsp(string input_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);

			void read_demand_vehicle(string input_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);
			void write_demand_vehicle(string output_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);
			void write_vehicle_path(string output_dir_name,ScenarioData& scenario_data,DemandData& demand_data,NetworkData &network_data);
		}
	}
}
#endif