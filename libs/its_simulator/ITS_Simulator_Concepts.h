#pragma once

#include "Core.h"


namespace Network_Event_Components
{
	namespace Concepts
	{
		concept struct Is_Weather_Event_Prototype
		{
			check_component_accessor_name(has_type,weather_type);
			check_component_accessor_name(has_precipitation_depth,precipitation_depth);
			check_component_accessor_name(has_visibility,visibility);
			check_component_accessor_name(has_temperature,temperature);
			check_component_accessor_name(has_wind_speed,wind_speed);
			define_default_check(has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
		};
		concept struct Is_Weather_Event
		{
			check_accessor_name(has_type,weather_type);
			check_accessor_name(has_precipitation_depth,precipitation_depth);
			check_accessor_name(has_visibility,visibility);
			check_accessor_name(has_temperature,temperature);
			check_accessor_name(has_wind_speed,wind_speed);
			check_concept(is_prototype, Is_Weather_Event_Prototype, T, V);
			define_sub_check(is_component, has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
			define_default_check(is_prototype || is_component);
		};
		concept struct Is_Accident_Event_Prototype
		{
			check_component_accessor_name(has_type,accident_type);
			check_component_accessor_name(has_severity,severity);
			check_component_accessor_name(has_lanes,lanes);
			define_default_check(has_type & has_severity & has_lanes);
		};
		concept struct Is_Accident_Event
		{
			check_accessor_name(has_type,accident_type);
			check_accessor_name(has_severity,severity);
			check_accessor_name(has_lanes,lanes);
			check_concept(is_prototype, Is_Accident_Event_Prototype, T, V);
			define_sub_check(is_component, has_type & has_severity & has_lanes);
			define_default_check(is_prototype || is_component);
		};
	}
}
