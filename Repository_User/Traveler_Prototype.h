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
	
	namespace Prototypes
	{
		template<typename ComponentType,typename CallerType>
		struct Traveler_Prototype
		{
			feature_accessor(router);
			feature_accessor(vehicle);
			feature_accessor(uuid);

			feature void Schedule_New_Departure(int departed_time)
			{
				define_component_interface_local(_Routing_Interface, Routing_Prototype, router, ComponentType);
				_Routing_Interface* itf=router<_Routing_Interface*>();
				
				itf->Schedule_Route_Computation<NULLTYPE>(departed_time);
			}
		};
	}
}

using namespace Traveler_Components::Prototypes;