#pragma once
#include "User_Space_Includes.h"
#include "Polaris_Scenario_Implementation.h"
#include "Link_Prototype.h"
#include "Intersection_Prototype.h"
#include "Polaris_Vehicle_Implementation.h"

namespace Link_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Components
	{
	}

	namespace Implementations
	{

	//==================================================================================================================
	/// Polaris_Link_Base
	//------------------------------------------------------------------------------------------------------------------
		implementation struct Polaris_Link_Implementation:public Polaris_Component_Class<Polaris_Link_Implementation,MasterType,Execution_Object,ParentType>
		{
		//==================================================================================================================
		/// Simple Link Members
		//------------------------------------------------------------------------------------------------------------------
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_data(int, num_lanes, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, length, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, speed_limit, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(Link_Components::Types::Link_Type_Keys, link_type, none, none);
			
			member_data(int, num_left_turn_bays, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, num_right_turn_bays, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(float, left_turn_bay_length, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, right_turn_bay_length, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, inbound_turn_movement_size, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, outbound_turn_movement_size, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			//link state			
			member_data(float, link_fftt, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, link_bwtt, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_fftt_cached_simulation_interval_size, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_bwtt_cached_simulation_interval_size, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			//current interval
			member_data(float, link_capacity, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, link_supply, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(int, link_upstream_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_downstream_departed_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(int, link_origin_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_origin_departed_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_origin_loaded_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(int, link_destination_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//cumulative - Mid-Trip
			member_data(int, link_upstream_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_shifted_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_upstream_cumulative_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_downstream_cumulative_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			//cumulative - Begin/End-Trip
			member_data(int, link_origin_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_origin_cumulative_departed_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, link_destination_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//cached cumulative state - Mid-Trip
			member_data(int, cached_link_shifted_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			//other attributes
			member_data(float, maximum_flow_rate, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, free_flow_speed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, backward_wave_speed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, jam_density, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, critical_density, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

		//==================================================================================================================
		/// Inbound and Outbound Turn Movement Members
		//------------------------------------------------------------------------------------------------------------------

			member_container(vector<typename MasterType::turn_movement_type*>, inbound_turn_movements, none, none);
			member_container(vector<typename MasterType::turn_movement_type*>, outbound_turn_movements, none, none);

			member_component(typename MasterType::network_type, network_reference, none, none);


		//==================================================================================================================
		/// Upstream and Downstream Intersections Reference
		//------------------------------------------------------------------------------------------------------------------			

			
			member_component(typename MasterType::intersection_type, upstream_intersection, none, none);
			member_component(typename MasterType::intersection_type, downstream_intersection, none, none);



		//==================================================================================================================
		/// Containers of Cached Cumulative Vehicle Statistics
		//------------------------------------------------------------------------------------------------------------------
			
			member_container(vector<int>, cached_link_upstream_cumulative_vehicles_array, none, none);
			member_container(vector<int>, cached_link_downstream_cumulative_vehicles_array, none, none);

		//==================================================================================================================
		/// Vehicle Origin Containers
		//------------------------------------------------------------------------------------------------------------------
			member_data(int, link_origin_vehicle_current_position, none, none);

			member_container(vector<typename MasterType::vehicle_type*>, link_origin_vehicle_array, none, none);

			member_container(deque<typename MasterType::vehicle_type*>, link_origin_vehicle_queue, none, none);
	
		//==================================================================================================================
		/// Current Vehicles Containers
		//------------------------------------------------------------------------------------------------------------------
			member_container(vector<typename MasterType::vehicle_type*>, current_vehicle_queue, none, none);


		//==================================================================================================================
		/// Replicas Containers
		//------------------------------------------------------------------------------------------------------------------
			member_container(vector<typename MasterType::routable_link_type*>, replicas_container, none, none);


		//==================================================================================================================
		/// travel_time
		//------------------------------------------------------------------------------------------------------------------
			typedef Link_Prototype<typename MasterType::routable_link_type, NULLTYPE> replica_interface;
			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType travel_time(){return (TargetType)(_travel_time);} tag_getter_as_available(travel_time);
			template<typename ComponentType, typename CallerType, typename TargetType>
			void travel_time(TargetType set_value)
			{
				_travel_time = (float)set_value;
				// update replicas
				typename replicas_container_type::iterator replica_itr;
				for (replica_itr=_replicas_container.begin(); replica_itr!=_replicas_container.end(); replica_itr++)
				{
					replica_interface* replica = (replica_interface*)(*replica_itr);
					replica->template travel_time<float>(_travel_time);
				}
			}
			tag_setter_as_available(travel_time);

			float _travel_time;

			member_component(typename MasterType::approach_type, approach, none, none);
			member_data(int, link_num_vehicles_in_queue, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_container(deque<typename MasterType::vehicle_type*>, link_destination_vehicle_queue, none, none);


			// convert this to a service which returns or stores supply in user-provided container
			feature_implementation void link_supply_update(int& supply_container, int& capacity_container)
			{
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				_Network_Interface* network = network_reference<ComponentType,CallerType,_Network_Interface*>();
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;

				// time unification
				int current_simulation_interval_index = network->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();

				//Newell's model

				//jam density * length * num_lanes = Kj(a,t)*L(a)*nlanes(a,t)
					
				float num_vehicles_under_jam_density_on_inbound_link = _num_lanes * _jam_density * _length/5280.0;

				int t_minus_one_fftt = -1;
				int link_upstream_cumulative_vehicles_by_t_minus_one = 0;	
					
				int t_minus_bwtt = -1;
				int link_downstream_cumulative_vehicles_by_t_minus_bwtt = 0;	

				float link_available_spaces = -1;

				//N(a,0,t-1)
				if (current_simulation_interval_index>0)
				{
					t_minus_one_fftt = (current_simulation_interval_index-1)%_link_fftt_cached_simulation_interval_size;
					link_upstream_cumulative_vehicles_by_t_minus_one = _cached_link_upstream_cumulative_vehicles_array[t_minus_one_fftt];
				}
					
				//N(a,L(a),t-bwtt)
				if (current_simulation_interval_index >= _link_bwtt_cached_simulation_interval_size)
				{
					t_minus_bwtt = (current_simulation_interval_index + 1 - _link_bwtt_cached_simulation_interval_size)%_link_bwtt_cached_simulation_interval_size;
					link_downstream_cumulative_vehicles_by_t_minus_bwtt = _cached_link_downstream_cumulative_vehicles_array[t_minus_bwtt];
				}
					
				//supply(a,t) = Kj(a,t)*L(a)*nlanes(a,t) + N(a,L(a),t-bwtt) -N(a,0,t-1) = backward wave propogation in link
				link_available_spaces = num_vehicles_under_jam_density_on_inbound_link + link_downstream_cumulative_vehicles_by_t_minus_bwtt - link_upstream_cumulative_vehicles_by_t_minus_one;
				_link_supply = max(0.0,(double)link_available_spaces);
				supply_container = _link_supply;

				float current_link_capacity = 0.0;
				current_link_capacity =  (float) (simulation_interval_length * _num_lanes * _maximum_flow_rate/3600.0);
				_link_capacity = current_link_capacity;
				capacity_container = _link_capacity;
			}
			
			// add many comments
			feature_implementation void network_state_update()
			{
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				_Network_Interface* network = network_reference<ComponentType,CallerType,_Network_Interface*>();
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;

				int current_simulation_interval_index = network->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();

				//calculate upstream cumulative vehicles using the three detector method
				int t_minus_bwtt=-1;			
				
				int bwtt_cached_simulation_interval_size=_link_bwtt_cached_simulation_interval_size;
				int fftt_cached_simulation_interval_size=_link_fftt_cached_simulation_interval_size;
				
				if(current_simulation_interval_index >= bwtt_cached_simulation_interval_size)
				{
					t_minus_bwtt = (current_simulation_interval_index + 1- bwtt_cached_simulation_interval_size)%bwtt_cached_simulation_interval_size;
				}

				int upstream_cumulative_departed_vehicles = 0;
				upstream_cumulative_departed_vehicles = 
					_link_upstream_cumulative_arrived_vehicles + 
					_link_origin_cumulative_departed_vehicles - 
					_link_destination_cumulative_arrived_vehicles;

				upstream_cumulative_departed_vehicles = max(0,upstream_cumulative_departed_vehicles);

				if (t_minus_bwtt>-1)
				{
					int jam_vehicles = (int) (_num_lanes * _length * _jam_density);

					int cached=_cached_link_downstream_cumulative_vehicles_array[t_minus_bwtt]+jam_vehicles;
					_link_upstream_cumulative_vehicles = min(upstream_cumulative_departed_vehicles,cached);
				}
				else
				{
					_link_upstream_cumulative_vehicles = upstream_cumulative_departed_vehicles;
				}

				int t_fftt = -1;
				int t_bwtt = -1;
				int t_cached_delay = -1;

				if (current_simulation_interval_index>0)
				{
					t_fftt = (current_simulation_interval_index)%fftt_cached_simulation_interval_size;
					t_bwtt = (current_simulation_interval_index)%bwtt_cached_simulation_interval_size;
					t_cached_delay = (current_simulation_interval_index)%((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();
				}
				else
				{
					t_fftt = 0;
					t_bwtt = 0;
					t_cached_delay = 0;
				}

				_cached_link_upstream_cumulative_vehicles_array[t_fftt] = _link_upstream_cumulative_vehicles;
				_cached_link_downstream_cumulative_vehicles_array[t_bwtt] = _link_downstream_cumulative_vehicles;

				//network data
				int t_minus_fftt = -1;
				int link_shifted_cumulative_arrived_vehicles = 0;

				int cached_fftt=_link_fftt_cached_simulation_interval_size;

				if((current_simulation_interval_index+1) >= cached_fftt)
				{
					t_minus_fftt = (current_simulation_interval_index + 1 - cached_fftt) % cached_fftt;
					link_shifted_cumulative_arrived_vehicles = _cached_link_upstream_cumulative_vehicles_array[t_minus_fftt];
				}
				else
				{
					link_shifted_cumulative_arrived_vehicles = 0;
				}
				
				_link_num_vehicles_in_queue = link_shifted_cumulative_arrived_vehicles - _link_downstream_cumulative_vehicles;

			}


			feature_implementation void accept_vehicle(TargetType veh,requires(check_2(CallerType,typename MasterType::routing_type,is_same)))
			{
				define_container_and_value_interface(_Link_Origin_Vehicles_Container_Interface, _Vehicle_Interface,type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				_link_origin_cumulative_arrived_vehicles++;
				_Vehicle_Interface* vehicle = (_Vehicle_Interface*)veh;
				_link_origin_vehicle_array.push_back((typename MasterType::vehicle_type*)vehicle);
				vehicle->template load<Vehicle_Components::Types::Load_To_Entry_Queue>();
			}

			feature_implementation void accept_vehicle(TargetType veh,requires(check_2(CallerType,type_of(upstream_intersection),is_same)))
			{
				define_container_and_value_interface(_Vehicle_Queue_Interface, _Vehicle_Interface, type_of(current_vehicle_queue), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_container_and_value_interface(_Destination_Vehicle_Queue_Interface, _Vehicle_Interface_2, type_of(link_destination_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);				
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, _Network_Interface::get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				typedef Link_Prototype<ComponentType, ComponentType> _Link_Interface;
				
				int current_simulation_interval_index = network_reference<ComponentType,CallerType,_Network_Interface*>()->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->simulation_interval_length<int>();
				_Vehicle_Interface* vehicle=(_Vehicle_Interface*)veh;
				_Movement_Plan_Interface* mp = vehicle->movement_plan<_Movement_Plan_Interface*>();
				float a_delayed_time;

				if (mp->template current_trajectory_position<int>() == -1)
				{
					a_delayed_time = 0.0;
				}
				else
				{
					a_delayed_time = (int)((network_reference<ComponentType,CallerType,_Network_Interface*>()->template start_of_current_simulation_interval_relative<int>() - mp->template get_current_link_enter_time<int>()) - _link_fftt);
				}
				mp->template transfer_to_next_link<NULLTYPE>(a_delayed_time);

				if(_internal_id == (mp->template destination<_Link_Interface*>())->template internal_id<int>())
				{
					vehicle->template unload<NULLTYPE>();
					_link_destination_cumulative_arrived_vehicles++;
					_link_destination_arrived_vehicles++;
					_link_destination_vehicle_queue.push_back((typename MasterType::vehicle_type*)vehicle);
					((_Scenario_Interface*)_global_scenario)->template network_cumulative_arrived_vehicles<int&>()++;
					((_Scenario_Interface*)_global_scenario)->template network_in_network_vehicles<int&>()--;
				}
				else
				{
					_current_vehicle_queue.push_back((typename MasterType::vehicle_type*)vehicle);
				}
			}

			feature_implementation void link_moving()
			{
				define_component_interface(_Intersection_Interface, type_of(upstream_intersection), Intersection_Components::Prototypes::Intersection_Prototype, ComponentType);
				typename vector<typename MasterType::vehicle_type*>::iterator vehicle_itr;

				for(vehicle_itr=_current_vehicle_queue.begin();vehicle_itr!=_current_vehicle_queue.end();vehicle_itr++)
				{
					((_Intersection_Interface*)_downstream_intersection)->template push_vehicle<NULLTYPE>((*vehicle_itr));
				}

				_current_vehicle_queue.clear();
			}

			feature_implementation void Initialize()
			{
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				load_event(ComponentType,Newells_Conditional,Compute_Step_Flow_Supply_Update,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION,NULLTYPE);
			}

			declare_feature_conditional(Newells_Conditional)
			{
				typedef Scenario_Prototype<typename MasterType::scenario_type> _Scenario_Interface;
				typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;
				if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION)
				{
					((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Flow_Supply_Update<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration;
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION;
				}
				else if(_sub_iteration == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION)
				{
					((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Flow_Link_Moving<NULLTYPE>);
					response.result=true;
					response.next._iteration=_iteration+((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION;
				}
				else
				{
					assert(false);
					cout << "Should never reach here in link conditional!" << endl;
				}
			}

			declare_feature_event(Compute_Step_Flow_Supply_Update)
			{

				typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;
				//step 1: link supply update based on a given traffic flow model
				//_this_ptr->template link_moving<ComponentType>();
				int supply;
				int capacity;
				_this_ptr->template link_supply_update<Target_Type<void,int&,int&>>(supply,capacity);
			}

			declare_feature_event(Compute_Step_Flow_Link_Moving)
			{
				
				typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;

				_Link_Interface* _this_ptr=(_Link_Interface*)_this;

				//step 7: load vehicles to origin links
				//_this_ptr->template origin_link_loading<ComponentType>();
				
				//step 7.5: link moving -- no link moving in Newell's simplified model -- it can be used to determine turn bay curve


				_this_ptr->template link_moving<NULLTYPE>();

				//step 8: link network state update
				_this_ptr->template network_state_update<NULLTYPE>();
			}
		};
			
	}

}