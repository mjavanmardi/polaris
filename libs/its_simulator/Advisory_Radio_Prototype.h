#pragma once
#include "ITS_Simulator_Includes.h"
//#include "Network_Event_Prototype.h"

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
		//	template<typename TargetType> static void Initialize_Type(TargetType configuration)
		//	{
		//		ComponentType::template Initialize_Type<TargetType>(configuration);
		//	}
		//	
		//	template<typename TargetType> void Initialize(TargetType configuration)
		//	{
		//		ComponentType::template Initialize<TargetType>(configuration);
		//	}
		//	
		//	template<typename TargetType> void Push_Network_Events(std::vector<Network_Event<TargetType,NT>*>& network_events)
		//	{
		//		this_component()->template Accept_Network_Events<TargetType>(network_events);
		//	}

		//	template<typename TargetType> void Get_Messages(std::vector<Network_Event<TargetType>*>& bucket)
		//	{
		//		this_component()->template Get_Messages<TargetType>(bucket);
		//	}

		//	accessor(x_position, NONE, NONE);
		//	accessor(y_position, NONE, NONE);

		//	accessor(active_properties, NONE, NONE);
		//};
	}
}

using namespace Advisory_Radio_Components::Prototypes;
