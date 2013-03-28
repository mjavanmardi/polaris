#pragma once
#include "User_Space_Includes.h"

namespace Link_Control_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Link_Control:public ComponentType
		{
			tag_as_prototype;

			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);

			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Link_Control_Components::Prototypes;