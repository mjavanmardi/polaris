#pragma once
#include "User_Space.h"

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
				// schedule routing
				define_component_interface(_Routing_Interface, typename get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				_Routing_Interface* itf= router<_Routing_Interface*>();
				itf->template Schedule_Route_Computation<NULLTYPE>(departed_time);
				
				// other stuff than routing
				load_event(ComponentType,Departure_Conditional,Set_Departure,departed_time,Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION,NULLTYPE);
			}

			declare_feature_conditional(Departure_Conditional)
			{
				if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION)
				{
					((ComponentType*)_this)->Swap_Event((Event_Callback)&Traveler_Prototype::Set_Departure<NULLTYPE>);
					response.result=true;
					response.next._iteration=END;
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION;
				}
			}

			declare_feature_event(Set_Departure)
			{
				// Here to add code for setting departure of the traveler other than routing; routing is scheduled in Schedule_New_Departure
			}
		};
	}
}

using namespace Traveler_Components::Prototypes;