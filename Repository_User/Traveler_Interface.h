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
			facet_accessor(uuid);

			facet void Schedule_New_Departure(int departed_time)
			{
				typedef typename ThisType::router_type RouterType;
				typedef Routing_Components::Interfaces::Routing_Interface<RouterType,ThisType> Routing_Interface;

				Routing_Interface* itf=router<Routing_Interface*>();

				itf->Schedule_Route_Computation<NULLTYPE>(departed_time);
			}
		};
	}
}