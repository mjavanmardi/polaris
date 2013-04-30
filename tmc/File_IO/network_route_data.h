#ifndef NETWORK_ROUTE_H
#define NETWORK_ROUTE_H
#include "utilities.h"
using namespace std;

namespace network_models
{

	namespace network_information
	{
		namespace route_data_information
		{
			///route
			struct RouteData
			{
				int route_index;
				int origin_link_index;
				int destination_link_index;
		
				int origin_activity_location_index;
				int destination_activity_location_index;

				int route_link_sum;
				float route_travel_time;

				int route_link_size;
				vector<int> route_link_index_array;
			};
		}
	}
}
#endif