#pragma once
#include "Traveler_Prototype.h"

namespace Traveler_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Traveler_Implementation:public Polaris_Component<MasterType,INHERIT(Traveler_Implementation),Execution_Object>
		{
			static void Departure_Conditional(ComponentType* _this,Event_Response& response)
			{
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION)
				{
					//((ComponentType*)_this)->Swap_Event((Event)&Traveler::Set_Departure<NULLTYPE>);
					//response.result=true;

					_this->Set_Departure<NULLTYPE>();
					response.next._iteration=END;
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION;
				}
			}

			template<typename TargetType> void Set_Departure()
			{
				// Create alias for this to use in conditional
				typedef Traveler<ComponentType> _this_Interface;
				ComponentType* _pthis = (ComponentType*)this;
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
					origin_link->push_vehicle_from_origin(vehicle);
				}
			}

			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, internal_id, NONE, NONE);

			m_prototype(Null_Prototype<typename MasterType::routing_type>, router, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::vehicle_type>, vehicle, NONE, NONE);
			m_prototype(Null_Prototype<typename MasterType::plan_type>, plan, NONE, NONE);
		};
	}

}
