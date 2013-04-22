#pragma once
#include "Vehicle_Prototype.h"

namespace Vehicle_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		implementation struct Polaris_Switch_Decision_Data_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Switch_Decision_Data_Implementation),MasterType,Data_Object,ParentType>
		{
			member_data(int, switch_decision_index, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_container(vector<typename MasterType::link_type*>, route_links_container, none, none);
		};

		implementation struct Polaris_Vehicle_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Vehicle_Implementation),MasterType,Execution_Object,ParentType>
		{
			member_data(Vehicle_Components::Types::Vehicle_Status_Keys, simulation_status, none, none);
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			
			member_component(typename MasterType::movement_plan_type, movement_plan, none, none);
#ifndef EXCLUDE_DEMAND
			member_component(typename MasterType::person_type, traveler, none, none);
#else
			member_component(typename MasterType::traveler_type, traveler, none, none);
#endif
			member_data(float, distance_to_stop_bar, none, none);
			member_data(float, local_speed, none, none);

			member_container(vector<typename MasterType::switch_decision_data_type*>, switch_decisions_container, none, none);
			member_data(RNG_Components::RngStream, rng_stream, none, none);
			member_data(Vehicle_Components::Types::Enroute_Information_Keys, enroute_information_type, none, none);
			member_data(double, information_compliance_rate, none, none);
			member_data(double, relative_indifference_bound_route_choice, none, none);
			member_data(double, minimum_travel_time_saving, none, none);

			feature_implementation void load(requires(check_2(TargetType,typename Types::Load_To_Entry_Queue,is_same)))
			{
				_simulation_status = Types::Vehicle_Status_Keys::IN_ENTRY_QUEUE;
			}

			feature_implementation void load(requires(check_2(TargetType,typename Types::Load_To_Origin_Link,is_same)))
			{
				typedef Movement_Plan_Components::Prototypes::Movement_Plan_Prototype<typename MasterType::movement_plan_type,ComponentType> _Movement_Plan_Interface;
				_simulation_status =  Types::Vehicle_Status_Keys::IN_NETWORK;
				((_Movement_Plan_Interface*)_movement_plan)->template initialize_trajectory<NULLTYPE>();
				((typename MasterType::vehicle_type*)this)->template load_register<ComponentType,CallerType,TargetType>();
			}

			feature_implementation void load(requires(!check_2(TargetType,typename Types::Load_To_Origin_Link,is_same) && !check_2(TargetType,typename Types::Load_To_Entry_Queue,is_same)))
			{
                assert_check_2(TargetType,typename Vehicle_Components::Types::Load_To_Origin_Link,is_same,"no match");
                assert_check_2(TargetType,typename Vehicle_Components::Types::Load_To_Entry_Queue,is_same,"no match");
			}

			feature_implementation void unload()
			{
				_simulation_status = Types::Vehicle_Status_Keys::OUT_NETWORK;
			}

			feature_implementation void load_register()
			{
				// do nothing for polaris vehicle
			}

			feature_implementation void update_enroute_switch_decisions()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				define_component_interface(_Movement_Plan_Interface, type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, _Movement_Plan_Interface::get_type_of(trajectory_container), Back_Insertion_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);
				//define_component_interface(_Link_Interface, _Trajectory_Unit_Interface::get_type_of(link), Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Switch_Decision_Data_Container_Interface, _Switch_Decision_Data_Interface, typename type_of(switch_decisions_container), Random_Access_Sequence_Prototype, Switch_Decision_Data_Prototype, ComponentType);
				define_container_and_value_interface(_Links_Container_Interface, _Link_Interface, _Switch_Decision_Data_Interface::get_type_of(route_links_container), Random_Access_Sequence_Prototype, Link_Prototype, ComponentType);

				_Trajectory_Container_Interface& trajectory= ((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>();

				//	cout<< "before change: current link " <<((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()  <<endl;
				_Switch_Decision_Data_Interface* switch_decision_data = (_Switch_Decision_Data_Interface*)Allocate<typename MasterType::switch_decision_data_type>();
				
				switch_decision_data->template switch_decision_index<int>(int(_switch_decisions_container.size()));

				_Trajectory_Container_Interface::iterator itr;

				for (itr = (trajectory.begin() + ((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()); itr != trajectory.end(); itr++)
				{
					_Trajectory_Unit_Interface* trajectory_unit = (_Trajectory_Unit_Interface*)(*itr);
					_Link_Interface* route_link = trajectory_unit->template link<_Link_Interface*>();
					_Links_Container_Interface& links_container = switch_decision_data->template route_links_container<_Links_Container_Interface&>();
					links_container.push_back(route_link);
					//switch_decision_data->template route_links_container<_Links_Container_Interface&>().push_back(route_link);
				}
				switch_decisions_container<ComponentType,CallerType,_Switch_Decision_Data_Container_Interface&>().push_back(switch_decision_data);

				((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_switched_decisions<NULLTYPE>();
			}

			feature_implementation void enroute_switching()
			{
				//typedef Routing_Components::Prototypes::Routing_Prototype<typename MasterType::routing_type> _Routing_Interface;
				define_component_interface(_Traveler_Interface, type_of(traveler), Traveler_Components::Prototypes::Traveler_Prototype, ComponentType);
				define_component_interface(_Routing_Interface, _Traveler_Interface::get_type_of(router), Routing_Components::Prototypes::Routing_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);

				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::routable_network_type> _Routable_Network_Interface;
				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface, _Routable_Network_Interface::get_type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);

				_Routing_Interface* router = traveler<ComponentType,CallerType,_Traveler_Interface*>()->template router<_Routing_Interface*>();

				//define_container_and_value_interface(_Routable_Links_Container_Interface, _Routable_Link_Interface, typename _Routable_Network_Interface::get_type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Reversed_Path_Container_Interface, _Regular_Link_Interface, typename MasterType::routable_network_type::get_type_of(reversed_path_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				define_container_and_value_interface(_Trajectory_Container_Interface, _Trajectory_Unit_Interface, _Movement_Plan_Interface::get_type_of(trajectory_container), Back_Insertion_Sequence_Prototype, Trajectory_Unit_Prototype, ComponentType);

				///en-route switching
				//cout<< "enroute switching..." <<endl;
				
				_Regular_Link_Interface* origin_link = ((_Movement_Plan_Interface*)_movement_plan)->template current_link<_Regular_Link_Interface*>();
				_Regular_Link_Interface* destination_link = ((_Movement_Plan_Interface*)_movement_plan)->template destination<_Regular_Link_Interface*>();
				
				router->template routable_origin<_Regular_Link_Interface*>(origin_link);
				router->template routable_destination<_Regular_Link_Interface*>(destination_link);

				//_Routable_Network_Interface* routable_network_ptr = router->template realtime_routable_network<_Routable_Network_Interface*>();
				_Routable_Network_Interface* routable_network_ptr = router->template routable_network<_Routable_Network_Interface*>();

				bool pathFound = router->template one_to_one_link_based_least_time_path_a_star<_Routable_Network_Interface*>(routable_network_ptr);

				///find a new route using shortest path algorithm
				if (pathFound)
				{	
					((_Movement_Plan_Interface*)_movement_plan)->template valid_trajectory<bool>(true);
					
					if (routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() == 0)
					{
						THROW_WARNING(endl << "Error: path size is: " << routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>().size() << endl);
						THROW_EXCEPTION(endl << "no path between origin link uuid " << origin_link->uuid<int>() << " and destination link uuid " << destination_link->uuid<int>());
					}
					
					//cout<< "before change: current link " <<((_Movement_Plan_Interface*)_movement_plan)->template current_trajectory_position<int&>()  <<endl;
					//cout<< "before change: current path size " <<((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>().size() <<endl;
					
					update_enroute_switch_decisions<ComponentType, CallerType, TargetType>();

					((_Movement_Plan_Interface*)_movement_plan)->template update_trajectory<_Reversed_Path_Container_Interface>(routable_network_ptr->template reversed_path_container<_Reversed_Path_Container_Interface&>());
					
					//cout<< "after change : current path size " <<((_Movement_Plan_Interface*)_movement_plan)->template trajectory_container<_Trajectory_Container_Interface&>().size() <<endl;
				}
			}

		};
	}

}