#pragma once
#include "User_Space_Includes.h"

namespace Traveler_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
		concept struct Is_Traveler_Prototype
		{
			check_getter(has_router,ComponentType::router);
			check_getter(has_vehicle,ComponentType::vehicle);
			define_default_check(has_router && has_vehicle);
		};
		concept struct Is_Traveler
		{
			check_getter(has_router,router);
			check_getter(has_vehicle,vehicle);
			check_concept(is_prototype, Is_Traveler_Prototype);
			define_default_check(is_prototype || (has_router && has_vehicle));
		};
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
					((ComponentType*)_this)->Swap_Event((Event)&Traveler_Prototype::Set_Departure<NULLTYPE>);
					response.result=true;
					response.next._iteration=END;
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION;
				}
			}

			declare_feature_event(Set_Departure)
			{
				// Create alias for this to use in conditional
				typedef Traveler_Prototype<ComponentType, ComponentType> _this_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_this_Interface* this_ptr=(_this_Interface*)_pthis;

				// Here to add code for setting departure of the traveler other than routing; routing is scheduled in Schedule_New_Departure
				define_component_interface(Vehicle_Itf, typename get_type_of(vehicle), Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(movement_itf, typename Vehicle_Itf::get_type_of(movement_plan),Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				define_component_interface(Routing_Itf, typename get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				define_component_interface(network_itf, typename Routing_Itf::get_type_of(network), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_container_and_value_interface(links, link_itf, typename network_itf::get_type_of(links_container),Containers::Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
	
				Vehicle_Itf* vehicle = this_ptr->template vehicle<Vehicle_Itf*>();
				link_itf* origin_link = vehicle->movement_plan<movement_itf*>()->origin<link_itf*>();
				
				if (!vehicle->movement_plan<movement_itf*>()->valid_trajectory<bool>())
				{
					return;
				}
				// Schedule the routing if the vehicle is not already in the network, otherwise return false
				if (vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED || vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::OUT_NETWORK)
				{
					origin_link->push_vehicle(vehicle);
				}
			}
		};
	}
}

using namespace Traveler_Components::Prototypes;