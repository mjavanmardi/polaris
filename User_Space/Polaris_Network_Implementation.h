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
		struct MOE_Data
		{
			//int starting_time;
			//int ending_time;

			double assignment_calculation_time;
			double simulation_calculation_time;
			double operation_calculation_time;
			double output_calculation_time;

			//vector<Link_MOE_Data> link_moe_data_by_type_array;

			int num_generated_vehicles;
			int num_loaded_vehicles;
			int num_departed_vehicles;
			int num_arrived_vehicles;
			float network_avg_link_travel_time;
			float network_avg_link_travel_delay;
			float network_avg_link_speed;
			float network_avg_link_density;
			float network_avg_link_in_flow_rate;
			float network_avg_link_out_flow_rate;
			float network_avg_link_in_volume;
			float network_avg_link_out_volume;
			float network_avg_link_queue_length;

			float network_avg_link_speed_ratio;
			float network_avg_link_in_flow_ratio;
			float network_avg_link_out_flow_ratio;
			float network_avg_link_density_ratio;
			float network_avg_link_travel_time_ratio;

		};

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

			member_associative_container(concat(dense_hash_map<int,typename MasterType::zone_type*>), zones_container, none, none);
			
			member_container(vector<typename MasterType::movement_type*>, movements_container, none, none);

			member_component(typename MasterType::scenario_type, scenario_reference, none, none);

			member_container(vector<typename MasterType::intersection_control_type*>, intersection_controls_container, none, none);

			member_data(float, network_vmt, none, none);

			member_data(float, network_vht, none,none);

            member_data(long,current_cpu_time_in_seconds,none,none);

			member_data(long,start_cpu_time_in_seconds,none,none);

			struct MOE_Data network_moe_data;

			struct MOE_Data realtime_network_moe_data;

			vector<struct MOE_Data> td_network_moe_data_array;

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
				td_network_moe_data_array.clear();
				_network_vht = 0.0;
				_network_vmt = 0.0;
				initialize_moe();
				initialize_network_agent<ComponentType,CallerType,TargetType>();
			}

			void initialize_moe()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type> _Network_Interface;

				//network_moe_data.starting_time = ((_Network_Interface*)this)->start_of_current_simulation_interval_absolute<int>();
				//network_moe_data.ending_time = network_moe_data.starting_time + ((_Scenario_Interface*)_global_scenario)->assignment_interval_length<int>();
	
				network_moe_data.assignment_calculation_time = 0.0;
				network_moe_data.simulation_calculation_time = 0.0f;
				network_moe_data.operation_calculation_time = 0.0f;
				network_moe_data.output_calculation_time = 0.0f;

				network_moe_data.num_arrived_vehicles = 0;
				network_moe_data.num_departed_vehicles = 0;
				network_moe_data.num_generated_vehicles = 0;
				network_moe_data.num_loaded_vehicles = 0;
	
				network_moe_data.network_avg_link_density = 0.0f;
				network_moe_data.network_avg_link_density_ratio = 0.0f;
				network_moe_data.network_avg_link_in_flow_rate = 0.0f;
				network_moe_data.network_avg_link_in_flow_ratio = 0.0f;
				network_moe_data.network_avg_link_out_flow_rate = 0.0f;
				network_moe_data.network_avg_link_out_flow_ratio = 0.0f;
				network_moe_data.network_avg_link_queue_length = 0.0f;
				network_moe_data.network_avg_link_speed = 0.0f;
				network_moe_data.network_avg_link_speed_ratio = 0.0f;
				network_moe_data.network_avg_link_travel_delay = 0.0f;
				network_moe_data.network_avg_link_travel_time = 0.0f;
				network_moe_data.network_avg_link_travel_time_ratio = 0.0f;

				network_moe_data.num_arrived_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>();
				network_moe_data.num_departed_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>();
				network_moe_data.num_loaded_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>();
			}

			feature_implementation void initialize_network_agent()
			{
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				load_event(ComponentType,End_Iteration_Conditional,End_Iteration_Handler, ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::END_OF_ITERATION,NULLTYPE);
                _start_cpu_time_in_seconds = (long)get_current_cpu_time_in_seconds();
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
				if (_this_ptr->template start_of_current_simulation_interval_absolute<int>() > _this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_end_time<int>())
				{
					_this_ptr->template scenario_reference<_Scenario_Interface*>()->template close_output_files<NULLTYPE>();
					exit(0);
				}

				((typename MasterType::network_type*)_this)->template calculate_moe<NULLTYPE,NULLTYPE,NULLTYPE>();
				((typename MasterType::network_type*)_this)->template printResults<NULLTYPE,NULLTYPE,NULLTYPE>();
				//if (_this_ptr->template start_of_current_simulation_interval_absolute<int>() > _this_ptr->template scenario_reference<_Scenario_Interface*>()->template simulation_end_time<int>() && _this_ptr->template scenario_reference<_Scenario_Interface*>()->template network_in_network_vehicles<int>() == 0)
			}

			feature_implementation void calculate_moe()
			{
				define_container_and_value_interface_unqualified_container(_Intersections_Container_Interface, _Intersection_Interface, type_of(intersections_container), Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type, ComponentType> _Network_Interface;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;

				typename _Intersections_Container_Interface::iterator intersection_itr;
				for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
				{
					((_Intersection_Interface*)(*intersection_itr))->template calculate_moe_for_simulation_interval<NULLTYPE>();
				}
				
				calculate_realtime_network_moe();
				//output_moe_for_simulation_interval<ComponentType, CallerType, TargetType>();

				if (((((_Network_Interface*)this)->template current_simulation_interval_index<int>()+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				{
					for (intersection_itr = _intersections_container.begin(); intersection_itr != _intersections_container.end(); intersection_itr++)
					{
						((_Intersection_Interface*)(*intersection_itr))->template calculate_moe_for_assignment_interval<NULLTYPE>();
					}
					update_moe_for_assignment_interval_with_links();
					update_moe_for_assignment_interval();
					output_moe_for_assignment_interval<ComponentType, CallerType, TargetType>();
					reset_moe_for_assignment_interval();
				}
			}

			void calculate_realtime_network_moe()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, NULLTYPE);
				typename _Links_Container_Interface::iterator link_itr;
				typedef typename MasterType::link_type _link_component_type;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;

				realtime_network_moe_data.network_avg_link_density = 0.0;
				realtime_network_moe_data.network_avg_link_out_flow_rate = 0.0;
				realtime_network_moe_data.network_avg_link_in_flow_rate = 0.0;
				realtime_network_moe_data.network_avg_link_out_volume = 0.0;
				realtime_network_moe_data.network_avg_link_in_volume = 0.0;
				realtime_network_moe_data.network_avg_link_queue_length = 0.0;
				realtime_network_moe_data.network_avg_link_speed = 0.0;
				realtime_network_moe_data.network_avg_link_travel_time = 0.0;
				realtime_network_moe_data.network_avg_link_travel_delay = 0.0;

				realtime_network_moe_data.network_avg_link_density_ratio = 0.0;
				realtime_network_moe_data.network_avg_link_out_flow_ratio = 0.0;
				realtime_network_moe_data.network_avg_link_in_flow_ratio = 0.0;
				realtime_network_moe_data.network_avg_link_speed_ratio = 0.0;
				realtime_network_moe_data.network_avg_link_travel_time_ratio = 0.0;

				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
						
					_link_component_type* link_component = (_link_component_type*)(*link_itr);

					realtime_network_moe_data.network_avg_link_density += link_component->realtime_link_moe_data.link_density;
					realtime_network_moe_data.network_avg_link_out_flow_rate += link_component->realtime_link_moe_data.link_out_flow_rate;
					realtime_network_moe_data.network_avg_link_in_flow_rate += link_component->realtime_link_moe_data.link_in_flow_rate;
					realtime_network_moe_data.network_avg_link_out_volume += link_component->realtime_link_moe_data.link_out_volume;
					realtime_network_moe_data.network_avg_link_in_volume += link_component->realtime_link_moe_data.link_in_volume;
					realtime_network_moe_data.network_avg_link_queue_length += link_component->realtime_link_moe_data.link_queue_length;
					realtime_network_moe_data.network_avg_link_speed += link_component->realtime_link_moe_data.link_speed;
					realtime_network_moe_data.network_avg_link_travel_time += link_component->realtime_link_moe_data.link_travel_time;
					realtime_network_moe_data.network_avg_link_travel_delay += link_component->realtime_link_moe_data.link_travel_delay;

					realtime_network_moe_data.network_avg_link_density_ratio += link_component->realtime_link_moe_data.link_density_ratio;
					realtime_network_moe_data.network_avg_link_out_flow_ratio += link_component->realtime_link_moe_data.link_out_flow_ratio;
					realtime_network_moe_data.network_avg_link_in_flow_ratio += link_component->realtime_link_moe_data.link_in_flow_ratio;
					realtime_network_moe_data.network_avg_link_speed_ratio += link_component->realtime_link_moe_data.link_speed_ratio;
					realtime_network_moe_data.network_avg_link_travel_time_ratio += link_component->realtime_link_moe_data.link_travel_time_ratio;
				}

				realtime_network_moe_data.network_avg_link_density /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_out_flow_rate /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_in_flow_rate /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_out_volume /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_in_volume /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_queue_length /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_speed /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_travel_time /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_travel_delay /= float(_links_container.size());

				realtime_network_moe_data.network_avg_link_density_ratio /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_out_flow_ratio /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_in_flow_ratio /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_speed_ratio /= float(_links_container.size());
				realtime_network_moe_data.network_avg_link_travel_time_ratio /= float(_links_container.size());

				realtime_network_moe_data.num_arrived_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>();
				realtime_network_moe_data.num_departed_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>();
				realtime_network_moe_data.num_loaded_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>();
			}


			void update_moe_for_assignment_interval_with_links()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, NULLTYPE);
				typename _Links_Container_Interface::iterator link_itr;
				typedef typename MasterType::link_type _link_component_type;

				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
						
					_link_component_type* link_component = (_link_component_type*)(*link_itr);
					network_moe_data.network_avg_link_travel_delay += link_component->link_moe_data.link_travel_delay;
					network_moe_data.network_avg_link_travel_time += link_component->link_moe_data.link_travel_time;
					network_moe_data.network_avg_link_speed += link_component->link_moe_data.link_speed;
					network_moe_data.network_avg_link_density += link_component->link_moe_data.link_density;
					network_moe_data.network_avg_link_in_flow_rate += link_component->link_moe_data.link_in_flow_rate;
					network_moe_data.network_avg_link_out_flow_rate += link_component->link_moe_data.link_out_flow_rate;

					network_moe_data.network_avg_link_density_ratio +=link_component->link_moe_data.link_density_ratio;
					network_moe_data.network_avg_link_in_flow_ratio += link_component->link_moe_data.link_in_flow_ratio;
					network_moe_data.network_avg_link_out_flow_ratio += link_component->link_moe_data.link_out_flow_ratio;
					network_moe_data.network_avg_link_speed_ratio += link_component->link_moe_data.link_speed_ratio;
					network_moe_data.network_avg_link_travel_time_ratio += link_component->link_moe_data.link_travel_time_ratio;

					_network_vmt += link_component->_link_vmt;
					_network_vht += link_component->_link_vht;
				}
			}

			void update_moe_for_assignment_interval()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;

				network_moe_data.num_arrived_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int>() - network_moe_data.num_arrived_vehicles;
				network_moe_data.num_departed_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_departed_vehicles<int>() - network_moe_data.num_departed_vehicles;
				network_moe_data.num_loaded_vehicles = ((_Scenario_Interface*)_global_scenario)->template network_cumulative_loaded_vehicles<int>() - network_moe_data.num_loaded_vehicles;

				//performance
				network_moe_data.network_avg_link_travel_time /= float(_links_container.size()); 
				network_moe_data.network_avg_link_speed /= float(_links_container.size()); 
				network_moe_data.network_avg_link_density /= float(_links_container.size()); 
				network_moe_data.network_avg_link_in_flow_rate  /= float(_links_container.size()); 
				network_moe_data.network_avg_link_out_flow_rate  /= float(_links_container.size()); 
				network_moe_data.network_avg_link_queue_length  /= float(_links_container.size()); 

				network_moe_data.network_avg_link_travel_time_ratio /= float(_links_container.size());
				network_moe_data.network_avg_link_speed_ratio /= float(_links_container.size());
				network_moe_data.network_avg_link_density_ratio /= float(_links_container.size());
				network_moe_data.network_avg_link_in_flow_ratio /= float(_links_container.size());
				network_moe_data.network_avg_link_out_flow_ratio /= float(_links_container.size());

				td_network_moe_data_array.push_back(network_moe_data);
			}

			void reset_moe_for_assignment_interval()
			{
				define_container_and_value_interface_unqualified_container(_Links_Container_Interface, _Link_Interface, type_of(links_container), Random_Access_Sequence_Prototype, Link_Components::Prototypes::Link_Prototype, NULLTYPE);
				define_container_and_value_interface_unqualified_container(_Turn_Movements_Container_Interface, _Turn_Movement_Interface, type_of(turn_movements_container), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, NULLTYPE);
				// reset network moe				
				initialize_moe();
				// reset link moe
				typename _Links_Container_Interface::iterator link_itr;
				typedef typename MasterType::link_type _link_component_type;
				for (link_itr = _links_container.begin(); link_itr != _links_container.end(); link_itr++)
				{
					_link_component_type* link_component = (_link_component_type*)(*link_itr);
					link_component->initialize_moe();
				}
				// reset turn movement moe
				typename _Turn_Movements_Container_Interface::iterator movement_itr;
				typedef typename MasterType::turn_movement_type _movement_component_type;
				for (movement_itr = _turn_movements_container.begin(); movement_itr != _turn_movements_container.end(); movement_itr++)
				{
					_movement_component_type* movement_component = (_movement_component_type*)(*movement_itr);
					movement_component->initialize_moe();
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
				printf("loaded=%7d, departed=%7d, arrived=%7d, in_network=%7d, VMT=%7f, VHT=%7f\n",scenario->template network_cumulative_loaded_vehicles<int>(),scenario->template network_cumulative_departed_vehicles<int>(),scenario->template network_cumulative_arrived_vehicles<int>(),scenario->template network_in_network_vehicles<int>(),_network_vmt, _network_vht);

				//write_node_control_state<NULLTYPE>();
				//write_vehicle_trajectory<NULLTYPE>();
				//write_network_link_flow<NULLTYPE>();
				//write_network_link_turn_time<ComponentType,CallerType,TargetType>();
				//write_output_summary<ComponentType,CallerType,TargetType>();
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
#ifndef FOR_LINUX_PORTING
			//==================================================================================================================
			/// read from database
			//------------------------------------------------------------------------------------------------------------------
			//feature_implementation void read_network_data(Network_Components::Types::Network_IO_Maps& net_io_maps); 

			//feature_implementation void read_intersection_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps);

			//feature_implementation void read_link_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps&);

			//feature_implementation void read_turn_movement_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps);

			//feature_implementation void read_activity_location_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps);

			//feature_implementation void read_zone_data(auto_ptr<odb::database>& db, Network_Components::Types::Network_IO_Maps& net_io_maps);
#endif
			//==================================================================================================================
			/// Convert network data from C++ data structure to Plaris structure
			//------------------------------------------------------------------------------------------------------------------
			feature_implementation void read_network_data(network_models::network_information::network_data_information::NetworkData& network_data); tag_feature_as_available(read_network_data);

			feature_implementation void read_intersection_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_link_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_turn_movement_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_activity_location_data(network_models::network_information::network_data_information::NetworkData& network_data);

			feature_implementation void read_zone_data(network_models::network_information::network_data_information::NetworkData& network_data);

			//==================================================================================================================
			/// output moe
			//------------------------------------------------------------------------------------------------------------------			
			feature_implementation void output_moe_for_simulation_interval();
			feature_implementation void output_moe_for_assignment_interval(); 
		};

		implementation struct Integrated_Polaris_Network_Implementation : public Polaris_Network_Implementation<MasterType,ParentType,APPEND_CHILD(Integrated_Polaris_Network_Implementation)>
		{
			member_data_component(typename MasterType::network_db_reader_type, db_reader, none,none);

			member_component(typename MasterType::network_skim_type, skimming_faculty,none,none);

			feature_implementation void read_network_data(Network_Components::Types::Network_IO_Maps& net_io_maps)
			{
				define_component_interface(_DB_Interface,type_of(db_reader),Prototypes::Network_DB_Reader_Prototype,ComponentType);
				_DB_Interface* db = (_DB_Interface*)&_db_reader;
				db->network_reference<ComponentType*>((ComponentType*)this);
				db->read_network_data<Network_Components::Types::Network_IO_Maps&>(net_io_maps);
			}

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

using namespace Network_Components::Implementations;