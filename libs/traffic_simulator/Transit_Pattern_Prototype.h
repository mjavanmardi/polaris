#pragma once
#include "Traffic_Simulator_Includes.h"

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
			accessor(dbid, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(route, NONE, NONE);
			accessor(pattern_stops, NONE, NONE);
			accessor(pattern_links, NONE, NONE);
			accessor(pattern_edge_ids, NONE, NONE);
			accessor(pattern_trips, NONE, NONE);
		};


	}
}

using namespace Transit_Pattern_Components::Prototypes;
