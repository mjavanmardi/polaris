#pragma once
#include "User_Space_Includes.h"
#include "Scenario_Prototype.h"

namespace Network_Event_Components
{


	namespace Concepts
	{
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
				this_component()->Start<ComponentType,CallerType,TargetType>();
			}
			
			feature void Initialize(TargetType configuration)
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>(configuration);
			}
			
			feature static void Initialize_Type(TargetType configuration)
			{
				ComponentType::Initialize_Type<ComponentType,CallerType,TargetType>(configuration);
			}
			
			feature static void Push_Subscriber(TargetType callback,int subscriber)
			{
				ComponentType::Accept_Subscriber<ComponentType,CallerType,TargetType>(callback,subscriber);
			}

			feature_accessor(network_event_type,none,none);
			feature_accessor(affected_links,none,none);
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
			
			feature void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}

			feature void Push_Network_Event(Network_Event<TargetType>* network_event)
			{
				this_component()->Accept_Network_Event<ComponentType,CallerType,TargetType>(network_event);
			}
			
			feature void Remove_Network_Event(Network_Event<TargetType>* network_event)
			{
				this_component()->Remove_Network_Event<ComponentType,CallerType,TargetType>(network_event);
			}

			feature void Get_Network_Events(int link,vector< Network_Event<TargetType,CallerType>* >& container)
			{
				this_component()->Get_Network_Events<ComponentType,CallerType,TargetType>(link,container);
			}
			
			feature void Get_Network_Events(vector< Network_Event<TargetType,CallerType>* >& container)
			{
				this_component()->Get_Network_Events<ComponentType,CallerType,TargetType>(container);
			}

			feature void Push_Subscriber(typename Network_Event_Callback<TargetType>::type callback)
			{
				this_component()->Push_Subscriber<ComponentType,CallerType,TargetType>(callback);
			}
		};
	}
}

using namespace Network_Event_Components::Prototypes;