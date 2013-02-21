#ifndef NETWORK_ZONE_DATA_H
#define NETWORK_ZONE_DATA_H
#include "utilities.h"

using namespace std;

namespace network_models
{

	namespace network_information
	{
		namespace network_data_information
		{
			struct ZoneData
			{
				int uuid;
				int zone_index;

				double x;
				double y;
				int population;

				//topology
				int num_origin_activity_locations;
				int num_destination_activity_locations;
				vector<int> origin_activity_location_index_array;
				vector<int> destination_activity_location_index_array;

				//choice probability
				vector<float> origin_activity_location_choice_cdf_array;
				vector<float> destination_activity_location_choice_cdf_array;
			};
		}
	}
}
#endif