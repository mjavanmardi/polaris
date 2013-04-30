#ifndef NETWORK_SENSOR_DATA_H
#define NETWORK_SENSOR_DATA_H
#include "utilities.h"
namespace network_models
{
	namespace network_information
	{
		namespace sensor_data_information
		{
			enum Sensor_Type_Keys
			{
				PRESENCE=1,
				PASSEGE,
			};

			struct PointObservationData
			{
				int vehicle_count;
			};

			struct SensorData
			{
				int sensor_id;

				int sensor_index;
				int link_index;

				float distance_to_downstream_node;
				Sensor_Type_Keys sensor_type;

				//traffic data
				int collection_start_time;
				int collection_interval_length;
				int num_collection_intervals;
				vector<PointObservationData> point_observation_data_array;

			};
		}
	}
}
#endif