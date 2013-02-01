#pragma once
#include "User_Space.h"

namespace Location_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Location:public ComponentType
		{
			tag_as_prototype;

			feature_accessor(link,none,none);
			feature_accessor(lane,none,none);
			feature_accessor(offset,none,none);

			feature_accessor(x,none,none);
			feature_accessor(y,none,none);
			feature_accessor(z,none,none);

			feature_accessor(latitude,none,none);
			feature_accessor(longitude,none,none);

		};
	}
}

using namespace Location_Components::Prototypes;