#pragma once
#include "User_Space_Includes.h"

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
		prototype struct Advisory_Radio:public ComponentType
		{
			tag_as_prototype;

			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);

			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Advisory_Radio_Components::Prototypes;