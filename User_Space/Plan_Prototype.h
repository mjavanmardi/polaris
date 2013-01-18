#pragma once
#include "User_Space_Includes.h"

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
		prototype struct Plan_Prototype
		{
			tag_as_prototype;
			feature_accessor(movement_plan, none,none);
			feature_accessor(traveler, none, none);
		};
	}
}

using namespace Plan_Components::Prototypes;