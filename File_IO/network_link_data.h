#ifndef NETWORK_LINK_DATA_H
#define NETWORK_LINK_DATA_H
#include "utilities.h"

using namespace std;

namespace network_models
{
	namespace network_information
	{
		namespace network_data_information
		{
			enum Link_Type_Keys
			{
				FREEWAY=0,
				ON_RAMP,
				OFF_RAMP,
				EXPRESSWAY,
				ARTERIAL,
			};
			struct LinkData
			{
				int uuid;
				int link_index;

				int unode_index;
				int dnode_index;
				int oppersite_link_index; //
		
				Link_Type_Keys link_type; ///
				int num_lanes;
				float length; ///in feet;
				float speed_limit; ///in miles per hour
				float maximum_flow_rate; /// in vehicles per hour per lane

				int num_left_turn_bays;
				int num_right_turn_bays;
				float left_turn_bay_length;
				float right_turn_bay_length;
		
				float free_flow_speed; ///in miles per hour
				float backward_wave_speed; ///in miles per hour
				float jam_density;
				float critical_density;

				float original_free_flow_speed; ///in miels per hour

				//topology
				int inbound_turn_movement_size;
				int outbound_turn_movement_size;
				vector<int> inbound_turn_movement_index_array;
				vector<int> outbound_turn_movement_index_array;
			};
		}
	}
}
#endif