#pragma once
#include "ITS_Simulator_Includes.h"
#include "Link_Control_Prototype.h"
#include "Advisory_Radio_Prototype.h"
#include "Depot_Prototype.h"
#include "Variable_Message_Sign_Prototype.h"
#include "Sensor_Prototype.h"

namespace Traffic_Management_Center_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Traffic_Management_Center ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(network_event_manager, NONE, NONE);

			template<typename TargetType> void Initialize()
			{
				this_component()->template Initialize<TargetType>();
			}

		};
	}
}

using namespace Traffic_Management_Center_Components::Prototypes;
