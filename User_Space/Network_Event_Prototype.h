#pragma once
#include "User_Space_Includes.h"
#include "Scenario_Prototype.h"

namespace Network_Event_Components
{
	namespace Concepts
	{
		concept struct Is_Weather_Event_Prototype
		{
			check_getter(has_type,Component_Type::weather_type);
			check_getter(has_precipitation_depth,Component_Type::precipitation_depth);
			check_getter(has_visibility,Component_Type::visibility);
			check_getter(has_temperature,Component_Type::temperature);
			check_getter(has_wind_speed,Component_Type::wind_speed);
			define_default_check(has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
		};
		concept struct Is_Weather_Event
		{
			check_getter(has_type,weather_type);
			check_getter(has_precipitation_depth,precipitation_depth);
			check_getter(has_visibility,visibility);
			check_getter(has_temperature,temperature);
			check_getter(has_wind_speed,wind_speed);
			check_concept(is_prototype, Is_Weather_Event_Prototype);
			define_sub_check(is_component, has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
			define_default_check(is_prototype || is_component);
		};
		concept struct Is_Accident_Event_Prototype
		{
			check_getter(has_type,Component_Type::accident_type);
			check_getter(has_severity,Component_Type::severity);
			check_getter(has_lanes,Component_Type::lanes);
			define_default_check(has_type & has_severity & has_lanes);
		};
		concept struct Is_Accident_Event
		{
			check_getter(has_type,accident_type);
			check_getter(has_severity,severity);
			check_getter(has_lanes,lanes);
			check_concept(is_prototype, Is_Accident_Event_Prototype);
			define_sub_check(is_component, has_type & has_severity & has_lanes);
			define_default_check(is_prototype || is_component);
		};
	}
	
	namespace Types
	{
	}

	namespace Prototypes
	{
		prototype struct Network_Event:public ComponentType
		{
			tag_as_prototype;
			
			feature void Start()
			{
				this_component()->template Start<ComponentType,CallerType,TargetType>();
			}
			
			feature void Initialize(TargetType configuration)
			{
				this_component()->template Initialize<ComponentType,CallerType,TargetType>(configuration);
			}
			
			feature static void Initialize_Type(TargetType configuration)
			{
				ComponentType::template Initialize_Type<ComponentType,CallerType,TargetType>(configuration);
			}
			
			feature static void Push_Subscriber(TargetType callback,int subscriber)
			{
				ComponentType::template Accept_Subscriber<ComponentType,CallerType,TargetType>(callback,subscriber);
			}

			//Basic properties
			feature_accessor(network_event_type,none,none);
			feature_accessor(affected_links,none,none);
			feature_accessor(affected_zones,none,none);
			feature_accessor(affected_locations,none,none);
			feature_accessor(unaffected_locations,none,none);
			feature_accessor(time_detected,none,none);
			feature_accessor(start_time,none,none);
			feature_accessor(expected_duration,none,none);
			feature_accessor(expired,none,none);
			feature_accessor(end_time,none,none);
			feature_accessor(active,none,none);
			feature_accessor(notes,none,none);

			//Weather properties
			feature_accessor(weather_type,none,none);
			feature_accessor(precipitation_rate,none,none);
			feature_accessor(precipitation_depth,none,none);
			feature_accessor(wind_speed,none,none);
			feature_accessor(snow_depth,none,none);
			feature_accessor(visibility,none,none);
			feature_accessor(temperature,none,none);

			//Accident properties
			feature_accessor(lanes,none,none);
			feature_accessor(lanes_closed,none,none);
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
		
		template<typename EventType>
		struct Network_Event_Callback
		{
			typedef void (*type)(void*,Prototypes::Network_Event<EventType,NT>*);
		};

		prototype struct Network_Event_Manager:public ComponentType
		{
			tag_as_prototype;
			
			// Create new network event automatically starts it upon allocation

			feature void Initialize()
			{
				this_component()->template Initialize<ComponentType,CallerType,TargetType>();
			}

			feature void Push_Network_Event(Network_Event<TargetType>* network_event)
			{
				this_component()->template Accept_Network_Event<ComponentType,CallerType,TargetType>(network_event);
			}
			
			feature void Remove_Network_Event(Network_Event<TargetType>* network_event)
			{
				this_component()->template Remove_Network_Event<ComponentType,CallerType,TargetType>(network_event);
			}

			feature void Get_Network_Events(int link,vector< Network_Event<TargetType,CallerType>* >& container)
			{
				this_component()->template Get_Network_Events<ComponentType,CallerType,TargetType>(link,container);
			}
			
			feature void Get_Network_Events(vector< Network_Event<TargetType,CallerType>* >& container)
			{
				this_component()->template Get_Network_Events<ComponentType,CallerType,TargetType>(container);
			}

			feature void Push_Subscriber(typename Network_Event_Callback<TargetType>::type callback)
			{
				this_component()->template Push_Subscriber<ComponentType,CallerType,TargetType>(callback);
			}
		};
	}
}

using namespace Network_Event_Components::Prototypes;