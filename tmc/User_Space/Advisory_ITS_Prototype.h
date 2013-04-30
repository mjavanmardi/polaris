#pragma once
#include "User_Space_Includes.h"

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
		prototype struct Advisory_ITS : public ComponentType
		{
			tag_as_prototype;
			
			feature static void Initialize_Type(TargetType configuration)
			{
				ComponentType::Initialize_Type<ComponentType,CallerType,TargetType>(configuration);
			}
			
			feature static void Initialize_Type()
			{
				ComponentType::Initialize_Type<ComponentType,CallerType,TargetType>();
			}

			feature void Initialize(TargetType configuration)
			{
				ComponentType::Initialize<ComponentType,CallerType,TargetType>(configuration);
			}
			
			feature void Push_Network_Events(vector<Network_Event<TargetType>*>& network_events)
			{
				this_component()->Accept_Network_Events<ComponentType,CallerType,TargetType>(network_events);
			}

			feature void Push_Displayed_Network_Events(vector<Network_Event<TargetType>*>& network_events)
			{
				this_component()->Accept_Displayed_Network_Events<ComponentType,CallerType,TargetType>(network_events);
			}


			feature void Get_Displayed_Messages(vector<Network_Event<TargetType>*>& bucket)
			{
				return this_component()->Get_Displayed_Messages<ComponentType,CallerType,TargetType>(bucket);
			}
		};
	}
}

using namespace Advisory_ITS_Components::Prototypes;