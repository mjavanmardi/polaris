#pragma once
#include "User_Space_Includes.h"
#include "Network_Event_Prototype.h"

namespace Advisory_Radio_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		//prototype struct Advisory_Radio:public ComponentType
		//{
		//	tag_as_prototype;
		//	
		//	feature static void Initialize_Type(TargetType configuration)
		//	{
		//		ComponentType::Initialize_Type<ComponentType,CallerType,TargetType>(configuration);
		//	}
		//	
		//	feature void Initialize(TargetType configuration)
		//	{
		//		ComponentType::Initialize<ComponentType,CallerType,TargetType>(configuration);
		//	}
		//	
		//	feature void Push_Network_Events(vector<Network_Event<TargetType,NT>*>& network_events)
		//	{
		//		this_component()->Accept_Network_Events<ComponentType,CallerType,TargetType>(network_events);
		//	}

		//	feature void Get_Messages(vector<Network_Event<TargetType,ComponentType>*>& bucket)
		//	{
		//		this_component()->Get_Messages<ComponentType,CallerType,TargetType>(bucket);
		//	}

		//	feature_accessor(x_position, none, none);
		//	feature_accessor(y_position, none, none);

		//	feature_accessor(active_properties, none, none);
		//};
	}
}

using namespace Advisory_Radio_Components::Prototypes;