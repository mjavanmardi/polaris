#pragma once
#include "Traffic_Simulator_Includes.h"

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
		prototype struct Location ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(link, NONE, NONE);
			accessor(lane, NONE, NONE);
			accessor(offset, NONE, NONE);

			accessor(x, NONE, NONE);
			accessor(y, NONE, NONE);
			accessor(z, NONE, NONE);

			accessor(latitude, NONE, NONE);
			accessor(longitude, NONE, NONE);

		};
	}
}

using namespace Location_Components::Prototypes;
