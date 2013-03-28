#pragma once
#include "User_Space_Includes.h"

namespace Depot_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Depot:public ComponentType
		{
			tag_as_prototype;

			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);

			feature_accessor(active_properties, none, none);
		};
	}
}

using namespace Depot_Components::Prototypes;