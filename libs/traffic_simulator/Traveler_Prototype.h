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

			template<typename TargetType> void Schedule_New_Departure(int departed_time)
			{
				// schedule routing
				typedef  Routing_Components::Prototypes::Routing< typename get_type_of(router)> _Routing_Interface;
				_Routing_Interface* itf= router<_Routing_Interface*>();
				itf->template Schedule_Route_Computation<NULLTYPE>(departed_time);
				
				// other stuff than routing

				this_component()->Load_Event<ComponentType>(&ComponentType::Departure_Conditional,departed_time,Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION);

				//load_event(ComponentType,Departure_Conditional,Set_Departure,departed_time,Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION,NULLTYPE);
			}


		};
	}
}

using namespace Traveler_Components::Prototypes;
