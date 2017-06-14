#pragma once
#include "ITS_Simulator_Includes.h"
//#include "Scenario_Prototype.h"

namespace Network_Event_Components
{

	namespace Types
	{
	}

	namespace Prototypes
	{
		prototype struct Network_Event ADD_DEBUG_INFO
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
			
			template<typename TargetType> void Initialize(int start_time, int end_time, std::vector<typename TargetType::link_type*>& affected_links)
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

		prototype struct Network_Event_Manager ADD_DEBUG_INFO
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

			template<typename TargetType> void Get_Network_Events(int link,std::vector< Network_Event<TargetType>* >& container)
			{
				this_component()->template Get_Network_Events<TargetType>(link,container);
			}
			
			template<typename TargetType> void Get_Network_Events(std::vector< Network_Event<TargetType>* >& container)
			{
				this_component()->template Get_Network_Events<TargetType>(container);
			}

			template<typename TargetType> void Push_Subscriber(typename Network_Event_Callback<TargetType>::type callback,int component_id)
			{
				this_component()->template Push_Subscriber<TargetType>(callback,component_id);
			}
		};
	}
}

using namespace Network_Event_Components::Prototypes;
