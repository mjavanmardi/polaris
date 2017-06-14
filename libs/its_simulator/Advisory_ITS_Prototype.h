#pragma once
#include "ITS_Simulator_Includes.h"

namespace Advisory_ITS_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Advisory_ITS ADD_DEBUG_INFO
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

			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();
			}

			template<typename TargetType> void Initialize(TargetType configuration)
			{
				this_component()->template Initialize<TargetType>(configuration);
			}
			
			template<typename TargetType> void Push_Network_Events(std::vector<Network_Event_Components::Prototypes::Network_Event<TargetType>*>& network_events)
			{
				this_component()->template Accept_Network_Events<TargetType>(network_events);
			}

			template<typename TargetType> void Push_Displayed_Network_Events(std::vector<Network_Event_Components::Prototypes::Network_Event<TargetType>*>& network_events)
			{
				this_component()->template Accept_Displayed_Network_Events<TargetType>(network_events);
			}


			template<typename TargetType> void Get_Displayed_Messages(std::vector<Network_Event_Components::Prototypes::Network_Event<TargetType>*>& bucket)
			{
				return this_component()->template Get_Displayed_Messages<TargetType>(bucket);
			}
		};
	}
}

using namespace Advisory_ITS_Components::Prototypes;
