#ifndef NETWORK_VEHICLE_DATA_H
#define NETWORK_VEHICLE_DATA_H
#include "utilities.h"

using namespace std;

///vehicle
namespace network_models
{

	namespace network_information
	{
		namespace demand_data_information
		{
			enum Vehicle_Type_Keys
			{
				SOV=0,
				HOV,
				TRUCK,
				BUS,
				RAIL,
			};

			enum Vehicle_Technology_Type_Keys
			{
				GASOLINE_VEHICLE = 0,
				DIESEL_VEHICLE,
				PLUG_IN_HYBRID_ELECTRIC_VEHICLE,
				PLUG_IN_ALL_ELECTRIC_VEHICLE,
				HYBRID_ELECTRIC_VEHICLE,
			};

			enum Route_Choice_Use_Class_Keys
			{
				UNRESPONSIVE=0,
				ENROUTE_SWITCHING,
				USER_EQUILIBRIUM,
				SYSTEM_OPTIMAL,
			};

			enum Simulation_Vehicle_Status_Keys
			{
				UNLOADED=0,
				IN_ENTRY_QUEUE,
				IN_NETWORK,
				OUT_NETWORK,
			};

			struct VehicleTrajectoryData
			{
				int link_index;
				int enter_time;		//in second
				int delayed_time;	//in second = travel time - free flow travel time
				int enter_interval_index; 
				
				// 0.1 second
				//vector<float> position_array;
			};

			struct ActivityChainData
			{
				int activity_index;
				int activity_location_index;
				int activity_link_index;

				int activity_type;
				int activity_duration;
				int activity_preferred_starting_time;
				int activity_preferred_ending_time;
				int activity_mode;
				int activity_vehicle_index;
			};

			class TravelerData
			{
				int traveler_index;
				int traveler_id;
				int household_id;

				vector<ActivityChainData> activity_chain_data_array;
			
			};

			class VehicleData
			{
			private:
				int vehicle_id;
				int vehicle_index;
				int origin_activity_location_index;
				int destination_activity_location_index;
				
				int departure_assignment_interval_index;	//
				int departure_simulation_interval_index;

				int departure_time;
				int arrival_time;
				int arrival_simulation_interval_index;
				int arrival_assignment_interval_index;

				int origin_link_index;
				int destination_link_index;

				int origin_zone_index;
				int destination_zone_index;
				
				int current_link;
				Simulation_Vehicle_Status_Keys simulation_status;
				int route_link_size;
				vector<VehicleTrajectoryData> vehicle_trajectory_data_array;
				
				RngStream g;
				int route_index;

				Route_Choice_Use_Class_Keys route_choice_user_class;
				Vehicle_Type_Keys vehicle_type;
				
				//vector<int> traveler_index_array;

			public:	
				
				double get_random_number();

				void set_vehicle_id(int _vehicle_id);
				int get_vehicle_id();

				void set_vehicle_index(int _vehicle_index);
				int get_vehicle_index();

				void set_origin_zone_index(int _origin_zone_index);
				int get_origin_zone_index();

				void set_destination_zone_index(int _destination_zone_index);
				int get_destination_zone_index();

				void set_origin_link_index(int _origin_link_index);
				int get_origin_link_index();

				void set_destination_link_index(int _destination_link_index);
				int get_destination_link_index();

				void set_origin_activity_location_index(int _activity_location_index);
				int get_origin_activity_location_index();

				void set_destination_activity_location_index(int _destination_activity_location_index);
				int get_destination_activity_location_index();

				void set_departure_assignment_interval_index(int _departure_assignment_interval_index);
				int get_departure_assignment_interval_index();

				void set_departure_simulation_interval_index(int _departure_simulation_interval_index);
				int get_departure_simulation_interval_index();

				void set_departure_time(int _departure_time);
				int get_departure_time();

				void set_arrival_time(int _arrival_time);
				int get_arrival_time();

				void set_arrival_simulation_interval_index(int _arrival_simulation_interval_index);
				int get_arrival_simulation_interval_index();

				void set_arrival_assignment_interval_index(int _arrival_assignment_interval_index);
				int get_arrival_assignment_interval_index();

				void set_simulation_status(Simulation_Vehicle_Status_Keys _simulation_status);
				Simulation_Vehicle_Status_Keys get_simulation_status();

				void init();
				void set_seed(int iseed);
				void clear();
				void load_to_entry_queue();
				void load_to_origin_link(int simulation_interval_index, int simulation_interval_length);
				void arrive_to_destination_link(int simulation_interval_index, int simulation_interval_length);
				void transfer_to_next_link(int simulation_interval_index, int simulation_interval_length, int delayed_time);
			
				void set_route_index(int _route_index,int _route_link_size, int * _route_link_array_ptr);
				void set_route_links(int origin_index, int destination_index,vector<int> route_link_array);
			
				int get_route_index();
				int get_next_link_index();
				int get_route_link_size();

				int get_current_link_index();
				int get_current_link_enter_time();
				int get_current_link_enter_interval_index();

				int get_route_link_index(int link);
				int get_route_link_enter_time(int link);
				int get_route_link_delayed_time(int link);
				int get_route_link_exit_time(int link);

				void clear_vehicle_trajectory();
			};
		}
	}
}
#endif
