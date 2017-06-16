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
			typedef Polaris_Component<MasterType,INHERIT(Traveler_Implementation),Execution_Object> Base_t;
			typedef typename Base_t::ComponentType ComponentType;

			static void Initiate_Departure_Conditional(ComponentType* _this,Event_Response& response)
			{
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION)
				{
					_this->Initiate_Departure();
					response.next._iteration=END;
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION;
				}
			}

			void Initiate_Departure()
			{
				typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename remove_pointer<vehicle_type>::type::get_type_of(movement_plan)> Movement_Interface;
				typedef  Link_Components::Prototypes::Link<typename Movement_Interface::get_type_of(origin)>  Link_Interface;
				
				Movement_Interface* attached_movement_plan = _vehicle->template movement_plan<Movement_Interface*>();

				if( attached_movement_plan->template valid_trajectory<bool>() )
				{
					// Schedule the routing if the vehicle is not already in the network, otherwise return false
					if (_vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::UNLOADED || _vehicle->template simulation_status<Vehicle_Components::Types::Vehicle_Status_Keys>() == Vehicle_Components::Types::Vehicle_Status_Keys::OUT_NETWORK)
					{
						Link_Interface* origin_link = attached_movement_plan->template origin<Link_Interface*>();

						origin_link->push_vehicle_from_origin(_vehicle);
					}
				}
			}

			void Schedule_New_Departure(int departed_time)
			{
				// schedule routing
				_router->Schedule_Route_Computation(departed_time);

				// other stuff than routing
				this->template Load_Event<ComponentType>(&Initiate_Departure_Conditional,departed_time,Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION);
			}

			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, internal_id, NONE, NONE);

			m_prototype(Routing_Components::Prototypes::Routing,typename MasterType::routing_type, router, NONE, NONE);
			m_prototype(Vehicle_Components::Prototypes::Vehicle,typename MasterType::vehicle_type, vehicle, NONE, NONE);
			//m_prototype(Null_Prototype,typename MasterType::plan_type>, plan, NONE, NONE);
		};
	}

}
