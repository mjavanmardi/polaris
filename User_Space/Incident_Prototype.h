#pragma once
#include "User_Space_Includes.h"
#include "Scenario_Prototype.h"

namespace Incident_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Incident:public ComponentType
		{
			tag_as_prototype;

			feature int Identify(){return this_component()->Identify();}

			feature_accessor(incident_type,none,none);
			feature_accessor(geometry,none,none);
			feature_accessor(time_detected,none,none);
			feature_accessor(start_time,none,none);
			feature_accessor(expected_duration,none,none);
			feature_accessor(expired,none,none);
			feature_accessor(end_time,none,none);

			//Weather properties
			feature_accessor(weather_type,none,none);
			feature_accessor(precipitation_rate,none,none);
			feature_accessor(snow_depth,none,none);
			feature_accessor(visibility,none,none);
			feature_accessor(temperature,none,none);

			//Accident properties
			feature_accessor(lanes,none,none);
			feature_accessor(severity,none,none);
			feature_accessor(accident_type,none,none);
			feature_accessor(vehicles_involved,none,none);
			feature_accessor(vehicle_type,none,none);
			feature_accessor(injury_type,none,none);

			//Congestion properties
			feature_accessor(travel_time,none,none);
			feature_accessor(slowdown_index,none,none);

			//Lane closure properties
			feature_accessor(lanes,none,none);
		};

		prototype struct Incident_Manager:public ComponentType
		{
			tag_as_prototype;

			feature void Push_Incident(Incident<TargetType,NT>* incident)
			{
				this_component()->Accept_Incident<ComponentType,CallerType,TargetType>(incident);
			}

			feature typename TargetType::ReturnType Get_Incidents(typename TargetType::ParamType param1)
			{
				return this_component()->Get_Incidents<ComponentType,CallerType,TargetType>(param1);
			}

			feature typename TargetType::ReturnType Get_Incidents(typename TargetType::ParamType param1,typename TargetType::Param2Type param2)
			{
				return this_component()->Get_Incidents<ComponentType,CallerType,TargetType>(param1,param2);
			}
		};
	}
}

using namespace Incident_Components::Prototypes;