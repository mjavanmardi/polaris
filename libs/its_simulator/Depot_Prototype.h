#pragma once
#include "ITS_Simulator_Includes.h"

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
		prototype struct Tow_Truck ADD_DEBUG_INFO
		{
			tag_as_prototype;
			
			template<typename TargetType> static void Initialize_Type()
			{
				ComponentType::template Initialize_Type<TargetType>();
			}

			accessor(associated_depot, NONE, NONE);
			accessor(truck_location, NONE, NONE);
		};

		prototype struct Depot ADD_DEBUG_INFO
		{
			tag_as_prototype;
			
			template<typename TargetType> static void Initialize_Type(TargetType configuration)
			{
				ComponentType::template Initialize_Type<TargetType>(configuration);
			}
			
			template<typename TargetType> static void Initialize_Type()
			{
				ComponentType::template Initialize_Type<TargetType>();
			}

			template<typename TargetType> void Initialize(TargetType configuration)
			{
				//ComponentType::template Initialize<TargetType>(configuration);
			}

			template<typename TargetType> void Push_Network_Events(std::vector<Network_Event_Components::Prototypes::Network_Event<TargetType>*>& network_events)
			{
				this_component()->template Accept_Network_Events<TargetType>(network_events);
			}

			accessor(covered_area, NONE, NONE);
			
			accessor(current_events, NONE, NONE);

			accessor(active_properties, NONE, NONE);
		};
	}
}

using namespace Depot_Components::Prototypes;
