#pragma once
#include "Repository_User_Includes.h"

namespace Traveler_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Interfaces
	{
		template<typename ThisType,typename CallerType>
		struct Traveler_Interface
		{
			facet_accessor(router);
			facet_accessor(vehicle);

			facet void Schedule_New_Departure(int departure_time)
			{

			}
		};
	}
}