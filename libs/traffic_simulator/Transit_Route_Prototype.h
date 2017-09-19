#pragma once
#include "Traffic_Simulator_Includes.h"

namespace Transit_Route_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}

	namespace Prototypes
	{
		prototype struct Transit_Route ADD_DEBUG_INFO
		{
			tag_as_prototype;

			accessor(internal_id, NONE, NONE);
			accessor(dbid, NONE, NONE);
			accessor(uuid, NONE, NONE);

			accessor(agency, NONE, NONE);
			accessor(shortname, NONE, NONE);
			accessor(longname, NONE, NONE);
			accessor(description, NONE, NONE);
			accessor(type, NONE, NONE);
		};


	}
}

using namespace Transit_Route_Components::Prototypes;
