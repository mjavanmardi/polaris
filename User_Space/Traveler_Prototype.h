#pragma once
#include "User_Space_Includes.h"

/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */
namespace Routing_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Routing_Prototype;
	}
};
/*------------------------------------------------*/

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
		prototype struct Traveler_Prototype
		{
			tag_as_prototype;

			feature_accessor(router, none, none);
			feature_accessor(vehicle, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(plan, none, none);
			
			feature_prototype void Schedule_New_Departure(int departed_time)
			{
				define_component_interface(_Routing_Interface, get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				_Routing_Interface* itf=router<_Routing_Interface*>();
				
				itf->template Schedule_Route_Computation<NULLTYPE>(departed_time);
			}
		};
	}
}

using namespace Traveler_Components::Prototypes;