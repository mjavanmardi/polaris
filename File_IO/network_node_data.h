#ifndef NETWORK_NODE_DATA_H
#define NETWORK_NODE_DATA_H
#include "utilities.h"

using namespace std;
namespace network_models
{

	namespace network_information
	{
		namespace network_data_information
		{
			enum Node_Type_Keys
			{
				NO_CONTROL=0,
				YIELD_SIGN,
				ALL_WAY_STOP_SIGN,
				TWO_WAY_STOP_SIGN,
				PRE_TIMED_SIGNAL_CONTROL, 
				ACTUATED_SIGNAL_CONTROL, 
				ADAPTIVE_SIGNAL_CONTROL,
			};
			struct NodeData
			{
				int uuid;
				int node_index;

				float x;
				float y;

				//topology
				int inbound_link_size;
				int outbound_link_size;
				vector<int> inbound_link_index_array;
				vector<int> outbound_link_index_array;

				//geometry
				vector<int> inbound_bearing;
				vector<int> outbound_bearing;

				//
				Node_Type_Keys node_type;

				int thread_id;
				RngStream rng;
			};
		}
	}
}
#endif