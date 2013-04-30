#pragma once
#include "User_Space_Includes.h"
#include "Link_Control_Prototype.h"
#include "Advisory_Radio_Prototype.h"
#include "Depot_Prototype.h"
#include "Variable_Message_Sign_Prototype.h"

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
		prototype struct Traffic_Management_Center:public ComponentType
		{
			tag_as_prototype;

			feature_accessor(network_event_manager,none,none);

			feature void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}

		};
	}
}

using namespace Traffic_Management_Center_Components::Prototypes;