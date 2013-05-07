#pragma once
#include "User_Space_Includes.h"

namespace Sensor_Components
{
	namespace Types
	{
		struct Reading
		{
			int time;
			double speed;
			double occupancy;
			int volume;
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Sensor:public ComponentType
		{
			tag_as_prototype;
			feature_accessor(link, none, none);
			feature_accessor(delta_t, none, none);
			feature_accessor(sigma_speed, none, none);
			feature_accessor(sigma_occupancy, none, none);
			feature_accessor(sigma_volume, none, none);
			feature_accessor(reading_container, none, none);
		};

	}
}

using namespace Sensor_Components::Prototypes;