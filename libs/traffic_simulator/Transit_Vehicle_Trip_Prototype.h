#pragma once
#include "Traffic_Simulator_Includes.h"

namespace Transit_Vehicle_Trip_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}

	namespace Prototypes
	{
		prototype struct Transit_Vehicle_Trip ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(internal_id, NONE, NONE);
			accessor(dbid, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(direction, NONE, NONE);
			accessor(pattern, NONE, NONE);
			accessor(arrival_seconds, NONE, NONE);
			accessor(departure_seconds, NONE, NONE);
		};


	}
}

using namespace Transit_Vehicle_Trip_Components::Prototypes;
