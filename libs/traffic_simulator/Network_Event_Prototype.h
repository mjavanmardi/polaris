#pragma once
#include "User_Space_Includes.h"
#include "Scenario_Prototype.h"

namespace Network_Event_Components
{
	namespace Concepts
	{
		concept struct Is_Weather_Event_Prototype
		{
			check_template_method_name(has_type,Component_Type::weather_type);
			check_template_method_name(has_precipitation_depth,Component_Type::precipitation_depth);
			check_template_method_name(has_visibility,Component_Type::visibility);
			check_template_method_name(has_temperature,Component_Type::temperature);
			check_template_method_name(has_wind_speed,Component_Type::wind_speed);
			define_default_check(has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
		};
		concept struct Is_Weather_Event
		{
			check_template_method_name(has_type,weather_type);
			check_template_method_name(has_precipitation_depth,precipitation_depth);
			check_template_method_name(has_visibility,visibility);
			check_template_method_name(has_temperature,temperature);
			check_template_method_name(has_wind_speed,wind_speed);
			check_concept(is_prototype, Is_Weather_Event_Prototype, T, V);
			define_sub_check(is_component, has_type & has_precipitation_depth & has_visibility & has_temperature & has_wind_speed);
			define_default_check(is_prototype || is_component);
		};
		concept struct Is_Accident_Event_Prototype
		{
			check_template_method_name(has_type,Component_Type::accident_type);
			check_template_method_name(has_severity,Component_Type::severity);
			check_template_method_name(has_lanes,Component_Type::lanes);
			define_default_check(has_type & has_severity & has_lanes);
		};
		concept struct Is_Accident_Event
		{
			check_template_method_name(has_type,accident_type);
			check_template_method_name(has_severity,severity);
			check_template_method_name(has_lanes,lanes);
			check_concept(is_prototype, Is_Accident_Event_Prototype, T, V);
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
			
			template<typename TargetType> void Start()
			{
				this_component()->template Start<TargetType>();
			}
			
			template<typename TargetType> void Initialize(TargetType configuration)
			{
				this_component()->template Initialize<TargetType>(configuration);
			}
			
			template<typename TargetType> void Initialize(int start_time, int end_time, boost::container::vector<typename TargetType::link_type*>& affected_links)
			{
				this_component()->template Initialize<TargetType>(start_time,end_time,affected_links);
			}

			template<typename TargetType> static void Initialize_Type(TargetType configuration)
			{
				ComponentType::template Initialize_Type<TargetType>(configuration);
			}
			
			template<typename TargetType> static void Push_Subscriber(TargetType callback,int subscriber)
			{
				ComponentType::template Accept_Subscriber<TargetType>(callback,subscriber);
			}

			//Basic properties
			accessor(network_event_type, NONE, NONE);
			accessor(affected_links, NONE, NONE);
			accessor(affected_zones, NONE, NONE);
			accessor(affected_locations, NONE, NONE);
			accessor(unaffected_locations, NONE, NONE);
			accessor(time_detected, NONE, NONE);
			accessor(start_time, NONE, NONE);
			accessor(expected_duration, NONE, NONE);
			accessor(expired, NONE, NONE);
			accessor(end_time, NONE, NONE);
			accessor(active, NONE, NONE);
			accessor(notes, NONE, NONE);

			//Weather properties
			accessor(weather_type, NONE, NONE);
			accessor(precipitation_rate, NONE, NONE);
			accessor(precipitation_depth, NONE, NONE);
			accessor(wind_speed, NONE, NONE);
			accessor(snow_depth, NONE, NONE);
			accessor(visibility, NONE, NONE);
			accessor(temperature, NONE, NONE);

			//Accident properties
			accessor(lanes, NONE, NONE);
			accessor(lanes_closed, NONE, NONE);
			accessor(severity, NONE, NONE);
			accessor(accident_type, NONE, NONE);
			accessor(vehicles_involved, NONE, NONE);
			accessor(vehicle_type, NONE, NONE);
			accessor(injury_type, NONE, NONE);

			//Congestion properties
			accessor(travel_time, NONE, NONE);
			accessor(slowdown_index, NONE, NONE);

			//Lane closure properties
			//accessor(lanes, NONE, NONE);
		};
		
		template<typename EventType>
		struct Network_Event_Callback
		{
			typedef void (*type)(void*,Prototypes::Network_Event<EventType>*);
		};

		prototype struct Network_Event_Manager:public ComponentType
		{
			tag_as_prototype;
			
			// Create new network event automatically starts it upon allocation

			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();
			}

			template<typename TargetType> void Push_Network_Event(Network_Event<TargetType>* network_event)
			{
				this_component()->template Accept_Network_Event<TargetType>(network_event);
			}
			
			template<typename TargetType> void Remove_Network_Event(Network_Event<TargetType>* network_event)
			{
				this_component()->template Remove_Network_Event<TargetType>(network_event);
			}

			template<typename TargetType> void Get_Network_Events(int link,boost::container::vector< Network_Event<TargetType>* >& container)
			{
				this_component()->template Get_Network_Events<TargetType>(link,container);
			}
			
			template<typename TargetType> void Get_Network_Events(boost::container::vector< Network_Event<TargetType>* >& container)
			{
				this_component()->template Get_Network_Events<TargetType>(container);
			}

			template<typename TargetType> void Push_Subscriber(typename Network_Event_Callback<TargetType>::type callback)
			{
				this_component()->template Push_Subscriber<TargetType>(callback);
			}
		};
	}
}

using namespace Network_Event_Components::Prototypes;
