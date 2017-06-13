#pragma once
#include "Traffic_Simulator_Includes.h"

namespace Plan_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Plan ADD_DEBUG_INFO
		{
			tag_as_prototype;
			accessor(movement_plan, NONE, NONE);
			accessor(traveler, NONE, NONE);
		};
	}
}

using namespace Plan_Components::Prototypes;
