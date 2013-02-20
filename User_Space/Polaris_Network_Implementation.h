#pragma once
#include "Network_Prototype.h"

namespace Network_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Network_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Network_Implementation),MasterType,Execution_Object,ParentType>
		{
			member_data(float, max_free_flow_speed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_container(vector<typename MasterType::intersection_type*>, intersections_container, none, none);

			member_container(vector<typename MasterType::link_type*>, links_container, none, none);

			member_container(vector<typename MasterType::routable_network_type*>, routable_networks_container, none, none);

			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType routable_network(){return (TargetType)(_routable_networks_container[_thread_id]);}tag_getter_as_available(routable_network);

			member_container(vector<typename MasterType::turn_movement_type*>, turn_movements_container, none, none);

			member_container(vector<typename MasterType::activity_location_type*>, activity_locations_container, none, none);

			member_container(vector<typename MasterType::zone_type*>, zones_container, none, none);
			
			member_container(vector<typename MasterType::movement_type*>, movements_container, none, none);

			member_component(typename MasterType::scenario_type, scenario_reference, none, none);

			member_container(vector<typename MasterType::intersection_control_type*>, intersection_controls_container, none, none);

			feature_implementation void initialize_intersection_control()
			{
				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				define_component_interface(_Intersection_Control_Interface, typename _Intersection_Interface::get_type_of(intersection_control), Intersection_Control_Components::Prototypes::Intersection_Control_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				typename _Intersections_Container_Interface::iterator intersection_itr;
				for(intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					_Intersection_Interface* intersection = (_Intersection_Interface*)(*intersection_itr);
					intersection->template intersection_control<_Intersection_Control_Interface*>()->template network_reference<_Network_Interface*>((_Network_Interface*)this);
					intersection->template intersection_control<_Intersection_Control_Interface*>()->template set_node_control_plan_index<NULLTYPE>(scenario_reference<ComponentType,CallerType,_Scenario_Interface*>()->template simulation_start_time<int>());
				}
			}

			feature_implementation void simulation_initialize()
			{
				initialize_intersection_control<ComponentType,CallerType,TargetType>();
				initialize_links<ComponentType,CallerType,TargetType>();
				initialize_intersections<ComponentType,CallerType,TargetType>();
				construct_network_cost<ComponentType,CallerType,TargetType>();
				construct_routable_network<ComponentType,CallerType,TargetType>();
				initialize_network_agent<ComponentType,CallerType,TargetType>();
			}

			feature_implementation void initialize_network_agent()
			{
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				load_event(ComponentType,End_Iteration_Conditional,End_Iteration_Handler, ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION,NULLTYPE);
			}

			declare_feature_conditional(End_Iteration_Conditional)
			{
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, typename MasterType::network_type);
				
				if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION)
				{
					((typename MasterType::network_type*)_this)->Swap_Event((Event)&End_Iteration_Handler<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration + ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION;
				} 
				else
				{
					assert(false);
					cout << "Should never reach here in network conditional!" << endl;
				}
			}

			declare_feature_event(End_Iteration_Handler)
			{
				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, typename MasterType::network_type);
				
				_Network_Interface* _this_ptr = (_Network_Interface*)_this;
				((typename MasterType::network_type*)_this)->template printResults<NULLTYPE,NULLTYPE,NULLTYPE>();
				if (_this_ptr->template start_of_current_simulation_interval_absolute<int>() > _this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_end_time<int>() && _this_ptr->template scenario_reference<_Scenario_Interface*>()->template network_in_network_vehicles<int>() == 0)
				{
					_this_ptr->template scenario_reference<_Scenario_Interface*>()->template close_output_files<NULLTYPE>();
					exit(0);
				}
			}

			feature_implementation void initialize_links()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typename _Links_Container_Interface::iterator links_itr;
				for (links_itr = _links_container.begin(); links_itr != _links_container.end(); links_itr++)
				{
					((_Link_Interface*)(*links_itr))->template initialize_features<ComponentType*>(this_component());
				}
			}

			feature_implementation void initialize_intersections()
			{
				//determine minimum merge rate
				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typename _Intersections_Container_Interface::iterator intersection_itr;
				for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					((_Intersection_Interface*)(*intersection_itr))->template initialize_features<ComponentType*>(this_component());
				}
			}


			feature_implementation void construct_network_cost()
			{
				// break up between links and movements
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, ComponentType);
				typename _Links_Container_Interface::iterator links_itr;
				_max_free_flow_speed = -1.0;			
				for (links_itr = _links_container.begin(); links_itr != _links_container.end(); links_itr++)
				{			
					_Link_Interface* link = (_Link_Interface*)(*links_itr);
					float free_flow_speed = (float) ((5280.0) * link->template free_flow_speed<float>()/3600.0); // feet per second
					float link_travel_time = float (link->template length<float>() / free_flow_speed);
					
					_max_free_flow_speed = max(_max_free_flow_speed,free_flow_speed);

					link_travel_time = max((float)1.0,link_travel_time);
					link->template travel_time<float>(link_travel_time);
				}
		
				define_container_and_value_interface_unqualified_container(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, type_of(turn_movements_container), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				typename _Turn_Movements_Container_Interface::iterator turn_movements_itr;
				for (turn_movements_itr = _turn_movements_container.begin(); turn_movements_itr != _turn_movements_container.end(); turn_movements_itr++)
				{
					_Turn_Movement_Interface* mvmt_itf=(_Turn_Movement_Interface*)(*turn_movements_itr);

					_Link_Interface* inboundLink = mvmt_itf->template inbound_link<_Link_Interface*>();
					_Link_Interface* outboundLink = mvmt_itf->template outbound_link<_Link_Interface*>();

					float turn_travel_penalty = 0.0f;
					if (mvmt_itf->template movement_rule<int>() == Turn_Movement_Components::Types::PROHIBITED)
					{
						turn_travel_penalty = INFINITY_FLOAT;
					}
					float forward_link_turn_travel_time = inboundLink->template travel_time<float>() + turn_travel_penalty;
					
					mvmt_itf->template turn_travel_penalty<float>(turn_travel_penalty);
					mvmt_itf->template forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
					for (int j = 0; j < (int)mvmt_itf->template replicas_container<_Turn_Movements_Container_Interface&>().size(); j++)
					{
						_Turn_Movement_Interface* replica = (_Turn_Movement_Interface*)mvmt_itf->template replicas_container<_Turn_Movements_Container_Interface&>()[j];
						replica->template forward_link_turn_travel_time<float>(forward_link_turn_travel_time);
				}
			}
			}

			// call it from initilization; move to implementation
			feature_implementation void construct_routable_network()
			{
				typedef Network_Prototype<typename MasterType::network_type> _Regular_Network_Interface;
				typedef Network_Components::Types::Network_Initialization_Type<Network_Components::Types::Regular_Network,_Regular_Network_Interface*> Net_IO_Type;
				define_container_and_value_interface_unqualified_container(_Routable_Networks_Container_Interface, _Routable_Network_Interface, type_of(routable_networks_container), Random_Access_Sequence_Prototype, Network_Components::Prototypes::Network_Prototype, ComponentType);
				for(int i=0;i<_num_threads;i++)
				{
					_Routable_Network_Interface* routable_network = (_Routable_Network_Interface*)Allocate<typename MasterType::routable_network_type>();
					routable_network->template read_network_data<Net_IO_Type>((_Regular_Network_Interface*)this);
					_routable_networks_container.push_back((typename MasterType::routable_network_type*)routable_network);
				}
			}

			//==================================================================================================================
			/// Write out intermediate simulation results
			//------------------------------------------------------------------------------------------------------------------
			feature_implementation void printResults() 
			{
						
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Prototype<typename MasterType::network_type> _Network_Interface;
				_Scenario_Interface* scenario = scenario_reference<ComponentType,CallerType,_Scenario_Interface*>();
				_Network_Interface* _this_ptr = (_Network_Interface*)this;
				printf("%s, ", convert_seconds_to_hhmmss(_this_ptr->template start_of_current_simulation_interval_absolute<int>()).c_str());
				printf("loaded=%7d, departed=%7d, arrived=%7d, in_network=%7d\n",scenario->template network_cumulative_loaded_vehicles<int>(),scenario->template network_cumulative_departed_vehicles<int>(),scenario->template network_cumulative_arrived_vehicles<int>(),scenario->template network_in_network_vehicles<int>());

				//write_node_control_state<NULLTYPE>();
				//write_vehicle_trajectory<NULLTYPE>();
				//write_network_link_flow<NULLTYPE>();
				//write_network_link_turn_time<ComponentType,CallerType,TargetType>();
				write_output_summary<ComponentType,CallerType,TargetType>();
			}

			feature_implementation void write_vehicle_trajectory();

			feature_implementation void write_network_link_flow();

			feature_implementation void write_network_link_turn_time();

			feature_implementation void write_node_control_state();

			feature_implementation void write_output_summary();

			//==================================================================================================================
			/// Convert network data from Polaris structure to C++ data structure to be written to a file
			//------------------------------------------------------------------------------------------------------------------
			feature_implementation void write_network_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_intersection_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_link_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_turn_movement_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_activity_location_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void write_zone_data(network_models::network_information::network_data_information::NetworkData& network_data);

			//==================================================================================================================
			/// read from database
			//------------------------------------------------------------------------------------------------------------------

			feature_implementation void read_network_data(Network_Components::Types::Network_IO_Maps& net_io_maps); 

			feature_implementation void read_intersection_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps);

			feature_implementation void read_link_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps&);

			feature_implementation void read_turn_movement_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps);

			feature_implementation void read_activity_location_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps);

			//==================================================================================================================
			/// Convert network data from C++ data structure to Plaris structure
			//------------------------------------------------------------------------------------------------------------------
			feature_implementation void read_network_data(network_models::network_information::network_data_information::NetworkData& network_data); tag_feature_as_available(read_network_data);

			feature_implementation void read_intersection_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_link_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_turn_movement_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_activity_location_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_zone_data(network_models::network_information::network_data_information::NetworkData& network_data);
		};

		implementation struct Integrated_Polaris_Network_Implementation : public Polaris_Network_Implementation<MasterType,ParentType,APPEND_CHILD(Integrated_Polaris_Network_Implementation)>
		{
			member_component(typename MasterType::network_skim_type, skimming_faculty,none,none);

			feature_implementation typename TargetType::ReturnType Get_LOS(typename TargetType::ParamType Origin, typename TargetType::ParamType Destination, typename TargetType::Param2Type Mode_Indicator)
			{
				Polaris_Network_Implementation<MasterType,ParentType>* base_this = (Polaris_Network_Implementation<MasterType,ParentType>*)this;
				
				define_component_interface(skim_faculty_itf,type_of(skimming_faculty),Network_Skimming_Components::Prototypes::Network_Skimming_Prototype,CallerType);

				skim_faculty_itf* skim_faculty = this->skimming_faculty<ComponentType,CallerType,skim_faculty_itf*>();

				return (TargetType::ReturnType)(skim_faculty->template Get_Current_LOS<TargetType>(Origin,Destination,Mode_Indicator));
			}
		
		};
	}
}
