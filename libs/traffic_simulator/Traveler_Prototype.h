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
			check_template_method_name(has_router,ComponentType::router);
			check_template_method_name(has_vehicle,ComponentType::vehicle);
			define_default_check(has_router && has_vehicle);
		};
		concept struct Is_Traveler
		{
			check_template_method_name(has_router,router);
			check_template_method_name(has_vehicle,vehicle);
			check_concept(is_prototype, Is_Traveler_Prototype, T, V);
			define_default_check(is_prototype || (has_router && has_vehicle));
		};
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
				//TODO
//load_event(ComponentType,Departure_Conditional,Set_Departure,departed_time,Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION,NULLTYPE);
			}

			template<typename TargetType> void Departure_Conditional()
			{
				if(_subiteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION)
				{
					((ComponentType*)_this)->Swap_Event((Event)&Traveler::Set_Departure<NULLTYPE>);
					response.result=true;
					response.next.iteration()=END;
					response.next._subiteration()=Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION;
				}
			}

			declare_event(Set_Departure)
			{
				// Create alias for this to use in conditional
				typedef Traveler<ComponentType> _this_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_this_Interface* this_ptr=(_this_Interface*)_pthis;

				// Here to add code for setting departure of the traveler other than routing; routing is scheduled in Schedule_New_Departure
				typedef  Vehicle_Components::Prototypes::Vehicle< typename get_type_of(vehicle)> Vehicle_Itf;
				typedef Movement_Plan_Components::Prototypes::Movement_Plan< typename Vehicle_Itf::get_type_of(movement_plan)> movement_itf;
				typedef  Routing_Components::Prototypes::Routing< typename get_type_of(router)> Routing_Itf;
				typedef  Network_Components::Prototypes::Network< typename Routing_Itf::get_type_of(network)> network_itf;
				typedef  Link_Components::Prototypes::Link<typename remove_pointer< typename network_itf::get_type_of(links_container)::value_type>::type>  link_itf;
				typedef Random_Access_Sequence< typename network_itf::get_type_of(links_container), link_itf*> links;

	
				Vehicle_Itf* vehicle = this_ptr->template vehicle<Vehicle_Itf*>();
				link_itf* origin_link = vehicle->template movement_plan<movement_itf*>()->template origin<link_itf*>();
				
				if (!vehicle->template movement_plan<movement_itf*>()->template valid_trajectory<bool>())
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
