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
			void Write_Vehicle_Data()
			{
				
				typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
				_Scenario_Interface* scenario = (_Scenario_Interface*)_global_scenario;
				float temp = _vehicle->template cacc_vmt<float&>();
				if (temp > 0)
				{
					fstream& vehicle_cacc_vmt_file = scenario->template vehicle_cacc_vmt_file<fstream&>();
					vehicle_cacc_vmt_file
						<< _vehicle->template trip_id<int>()  << ","
						<< _vehicle->template cacc_vmt<float>() << "\n";
				}

			}
			static void Initiate_Departure_Conditional(ComponentType* _this,Event_Response& response)
			{
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION)
				{
					_this->Initiate_Departure();
					response.next._iteration=std::max(iteration()+1,(int)END-2);
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_COMPUTATION_SUB_ITERATION;
				}
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_COMPUTATION_SUB_ITERATION)
				{
					_this->Write_Vehicle_Data();
					response.next._iteration=END;
					response.next._sub_iteration=END;
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
				Load_Event<ComponentType>(&Initiate_Departure_Conditional,departed_time,Scenario_Components::Types::Type_Sub_Iteration_keys::TRAVELER_SET_DEPARTURE_SUB_ITERATION);
				
			}

			void Schedule_Wehicle_Write()
			{
				//Load_Event<ComponentType>(&Write_Vehicle_Data,END,Scenario_Components::Types::Type_Sub_Iteration_keys::MOE_COMPUTATION_SUB_ITERATION);
			}

			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, internal_id, NONE, NONE);

			m_prototype(Routing_Components::Prototypes::Routing,typename MasterType::routing_type, router, NONE, NONE);
			m_prototype(Vehicle_Components::Prototypes::Vehicle,typename MasterType::vehicle_type, vehicle, NONE, NONE);
			//m_prototype(Null_Prototype,typename MasterType::plan_type>, plan, NONE, NONE);
		};
	}

}
