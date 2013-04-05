#pragma once
#include "User_Space_Includes.h"
#include "Scenario_Prototype.h"

namespace Network_Event_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Network_Event:public ComponentType
		{
			tag_as_prototype;
			
			feature void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}
			
			feature void Setup(TargetType configuration)
			{
				this_component()->Setup<ComponentType,CallerType,TargetType>(configuration);
			}

			feature_accessor(network_event_type,none,none);
			feature_accessor(affected_links,none,none);
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
			//feature_accessor(lanes,none,none);
		};

		prototype struct Network_Event_Manager:public ComponentType
		{
			tag_as_prototype;
			
			feature void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}

			feature void Push_Network_Event(Network_Event<TargetType,NT>* network_event)
			{
				this_component()->Accept_Network_Event<ComponentType,CallerType,TargetType>(network_event);
			}
			
			feature void Erase_Network_Event(Network_Event<TargetType,NT>* network_event)
			{
				this_component()->Erase_Network_Event<ComponentType,CallerType,TargetType>(network_event);
			}

			feature void Get_Network_Events(int link,vector< Network_Event<TargetType,NT>* >& container)
			{
				return this_component()->Get_Network_Events<ComponentType,CallerType,TargetType>(link,container);
			}

			feature_accessor(db_name,none,none);
		};
	}
}

using namespace Network_Event_Components::Prototypes;