#pragma once
#include "User_Space_Includes.h"

namespace Depot_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Tow_Truck
		{
			tag_as_prototype;
			
			feature static void Initialize_Type()
			{
				ComponentType::template Initialize_Type<ComponentType,CallerType,TargetType>();
			}

			feature_accessor(associated_depot, none, none);
			feature_accessor(truck_location, none, none);
		};

		prototype struct Depot:public ComponentType
		{
			tag_as_prototype;
			
			feature static void Initialize_Type(TargetType configuration)
			{
				ComponentType::template Initialize_Type<ComponentType,CallerType,TargetType>(configuration);
			}
			
			feature static void Initialize_Type()
			{
				ComponentType::template Initialize_Type<ComponentType,CallerType,TargetType>();
			}

			feature void Initialize(TargetType configuration)
			{
				ComponentType::template Initialize<ComponentType,CallerType,TargetType>(configuration);
			}

			feature void Push_Network_Events(vector<Network_Event_Components::Prototypes::Network_Event<TargetType>*>& network_events)
			{
				this_component()->template Accept_Network_Events<ComponentType,CallerType,TargetType>(network_events);
			}

			feature_accessor(covered_area, none, none);
			
			feature_accessor(current_events, none, none);

			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Depot_Components::Prototypes;