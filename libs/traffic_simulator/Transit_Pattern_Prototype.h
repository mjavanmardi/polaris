#pragma once
#include "User_Space_Includes.h"

namespace Transit_Pattern_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}

	namespace Prototypes
	{
		prototype struct Transit_Pattern ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(internal_id, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(route, NONE, NONE);
			accessor(pattern_stops, NONE, NONE);
			accessor(pattern_links, NONE, NONE);
		};


	}
}

using namespace Transit_Pattern_Components::Prototypes;
