#pragma once
#include "User_Space.h"

namespace Zone_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Zone_Prototype
		{
			tag_as_prototype;

			feature_accessor(origin_activity_locations, none, none);
			feature_accessor(destination_activity_locations, none, none);
			feature_accessor(origin_activity_location_choice_cdfs, none, none);
			feature_accessor(destination_activity_location_choice_cdfs, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
		};
	}
}

using namespace Zone_Components::Prototypes;