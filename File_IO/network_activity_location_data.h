#ifndef NETWORK_ACTIVITY_LOCATION_DATA_H
#define NETWORK_ACTIVITY_LOCATION_DATA_H
#include "utilities.h"

using namespace std;
namespace network_models
{
	namespace network_information
	{
		namespace network_data_information
		{
			struct ActivityLocationData
			{
				int uuid;
				int activity_location_index;

				//topology
				int num_origin_links;
				int num_destination_links;
				vector<int> origin_link_index_array;
				vector<int> destination_link_index_array;

				//choice probability
				vector<float> origin_link_choice_cdf_array;
				vector<float> destination_link_choice_cdf_array;

				int zone_index;
			};
		}

	}
}
#endif