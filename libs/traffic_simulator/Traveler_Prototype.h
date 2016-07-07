#pragma once
#include "User_Space_Includes.h"

namespace Traveler_Components
{
	namespace Types
	{
	}
	
	namespace Prototypes
	{
		prototype struct Traveler
		{
			tag_as_prototype;

			accessor(router, NONE, NONE);
			accessor(vehicle, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(internal_id, NONE, NONE);
			accessor(plan, NONE, NONE);

			void Schedule_New_Departure(int departed_time){this_component()->Schedule_New_Departure(departed_time);}
			void Schedule_Wehicle_Write(){this_component()->Schedule_Wehicle_Write();}
		};
	}
}

using namespace Traveler_Components::Prototypes;
