#pragma once
#include "Turn_Movement_Prototype.h"

namespace Turn_Movement_Components
{
	namespace Types
	{
	
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Polaris_Movement_Implementation:public Polaris_Component_Class<Polaris_Movement_Implementation,MasterType,Data_Object,ParentType>
		{
			member_data(float, turn_travel_penalty, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(float, movement_capacity, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, movement_supply, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, movement_demand, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, movement_flow, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, movement_transferred, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(Turn_Movement_Components::Types::Turn_Movement_Type_Keys, movement_type, none, none);
			member_data(Turn_Movement_Components::Types::Turn_Movement_Rule_Keys, movement_rule, none, none);

			member_data(float, inbound_link_departed_time_based_experienced_link_turn_travel_delay, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, outbound_link_arrived_time_based_experienced_link_turn_travel_delay, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(int, turn_movement_cumulative_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, turn_movement_cumulative_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(int, turn_movement_cumulative_shifted_arrived_vehicles, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(float, minimum_merge_rate, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_container(vector<typename MasterType::routable_movement_type*>, replicas_container, none, none);


			//==================================================================================================================
			/// forward_link_turn_travel_time
			//------------------------------------------------------------------------------------------------------------------
            typedef Turn_Movement_Components::Prototypes::Movement_Prototype<typename replicas_container_type::unqualified_value_type,NULLTYPE> _Replica_Interface;
            typedef Random_Access_Sequence_Prototype<replicas_container_type,NULLTYPE,_Replica_Interface*> _Replicas_Container_Interface;
			
			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType forward_link_turn_travel_time(){return (TargetType)(_forward_link_turn_travel_time);} tag_getter_as_available(forward_link_turn_travel_time);
			template<typename ComponentType, typename CallerType, typename TargetType>
			void forward_link_turn_travel_time(TargetType set_value)
			{
				_forward_link_turn_travel_time = (float)set_value;
				// update replicas
				typename _Replicas_Container_Interface::iterator replica_itr;
				for (replica_itr=_replicas_container.begin(); replica_itr!=_replicas_container.end(); replica_itr++)
				{
					_Replica_Interface* replica = (_Replica_Interface*)(*replica_itr);
					replica->template forward_link_turn_travel_time<float>(_forward_link_turn_travel_time);
				}
			}
			tag_setter_as_available(forward_link_turn_travel_time);

			float _forward_link_turn_travel_time;

			member_container(vector<float>, cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array, none, none);
			member_container(vector<float>, cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array, none, none);
			member_container(vector<float>, cached_outbound_link_departed_time_based_experienced_link_turn_travel_delay_array, none, none);
			member_container(vector<int>, cached_turn_movement_cumulative_shifted_arrived_vehicles_array, none, none);

			member_container(deque<typename MasterType::vehicle_type*>, vehicles_container, none, none);
			member_container(deque<typename MasterType::vehicle_type*>, upstream_vehicles_container, none, none);

			member_component(typename MasterType::link_type, inbound_link, none, none);

			member_component(typename MasterType::link_type, outbound_link, none, none);

			//member_component_basic(typename MasterType::DETECTOR_TYPE,detector);

			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_data(int, merge_priority, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, green_time, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, inbound_link_green_cycle_ratio, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			
			member_data(_lock,mvmt_lock,none,none);

			feature_implementation void update_capacity(Intersection_Components::Types::Intersection_Type_Keys intersection_type)
			{
				define_component_interface(_Link_Interface, type_of(inbound_link), Link_Components::Prototypes::Link_Prototype,  ComponentType);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				float outbound_link_capacity = (float)(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() * ((_Link_Interface*)_outbound_link)->template num_lanes<int>() * ((_Link_Interface*)_outbound_link)->template maximum_flow_rate<float>()/3600.0);
				float inbound_link_capacity = ((_Link_Interface*)_inbound_link)->template link_capacity<float>(); 
				float green_time_ratio = _green_time*1.0f/((float)((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>());
				if(intersection_type == Intersection_Components::Types::Intersection_Type_Keys::NO_CONTROL)
				{
					_movement_capacity = min(inbound_link_capacity,outbound_link_capacity)*green_time_ratio;
				}
				else
				{
					_movement_capacity = min(inbound_link_capacity,outbound_link_capacity)*green_time_ratio;
				}
			}

			feature_implementation void update_flow()
			{
				define_component_interface(_Link_Interface, type_of(inbound_link), Link_Components::Prototypes::Link_Prototype,  ComponentType);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type,ComponentType> _Network_Interface;

				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();

				int t_minus_fftt = -1;

				int link_fftt_cached_simulation_interval = ((_Link_Interface*)_inbound_link)->template link_fftt_cached_simulation_interval_size<int>();

				if (current_simulation_interval_index >= link_fftt_cached_simulation_interval)
				{
					t_minus_fftt = (current_simulation_interval_index - link_fftt_cached_simulation_interval) % link_fftt_cached_simulation_interval;			
				}

				if (t_minus_fftt > -1)
				{
					int cached_turn_movement_cumulative_shifted_arrived_vehicles=_cached_turn_movement_cumulative_shifted_arrived_vehicles_array[t_minus_fftt];
					_turn_movement_cumulative_shifted_arrived_vehicles = cached_turn_movement_cumulative_shifted_arrived_vehicles;
				}
				else
				{
					_turn_movement_cumulative_shifted_arrived_vehicles = 0;
				}
				_movement_demand = _turn_movement_cumulative_shifted_arrived_vehicles - _turn_movement_cumulative_vehicles;
				if(_movement_demand < 0.0) 
				{
					_movement_demand = 0.0;
				}
				_movement_flow = (float) min(min((double)_movement_demand,(double)_movement_capacity),(double)_movement_supply);
			}

			feature_implementation void transfer_vehicles(User_Space::RngStream& rng_stream)
			{
				define_component_interface(_Link_Interface, type_of(inbound_link), Link_Components::Prototypes::Link_Prototype,  ComponentType);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;
				typedef Vehicle_Components::Prototypes::Vehicle_Prototype<typename MasterType::vehicle_type, ComponentType> _Vehicle_Interface;
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type, ComponentType> _Network_Interface;
				typedef Turn_Movement_Components::Prototypes::Movement_Prototype<typename MasterType::movement_type> _Turn_Movement_Interface;
				define_component_interface(_Movement_Plan_Interface, typename _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);

				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();

				_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = 0.0;
				_inbound_link_departed_time_based_experienced_link_turn_travel_delay = 0.0;

				//determine number of transfered vehicles
				float transfer_flow_turn_movement = _movement_flow;
						
				int num_transfer_vehicles_of_turn_movement = (int)transfer_flow_turn_movement;
				transfer_flow_turn_movement = transfer_flow_turn_movement - num_transfer_vehicles_of_turn_movement;
						
				if(transfer_flow_turn_movement > 0.0)
				{
					double rng = rng_stream.RandU01();
					if(rng<=transfer_flow_turn_movement)
					{//partial vehicle, incomplete implementation
						++num_transfer_vehicles_of_turn_movement;
					}
				}

				_movement_transferred = num_transfer_vehicles_of_turn_movement;
						
				if (num_transfer_vehicles_of_turn_movement>0)
				{
					push_vehicles_to_outbound_link<ComponentType,CallerType,NULLTYPE>(num_transfer_vehicles_of_turn_movement);
					float delay=_outbound_link_arrived_time_based_experienced_link_turn_travel_delay/((float)num_transfer_vehicles_of_turn_movement);
					_outbound_link_arrived_time_based_experienced_link_turn_travel_delay= delay;
				}
				else
				{
					if(_movement_rule == Types::Turn_Movement_Rule_Keys::PROHIBITED)
					{
						_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = INFINITY_FLOAT;
					}
					else
					{
						if(_movement_demand == 0)
						{//no demand
							_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = 0.0;
						}
						else
						{//no supply for this turn -- need a prediction -- need to work it later
							int t_minus_one = -1;
							if (current_simulation_interval_index>0)
							{
								t_minus_one = (current_simulation_interval_index-1)%((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();
								float delay = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>() + _cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array[t_minus_one];
								_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = delay;
							}
							else
							{
								_outbound_link_arrived_time_based_experienced_link_turn_travel_delay= 0.0;
							}
						}
					}
				}
			}

			feature_implementation void push_vehicles_to_outbound_link(int num_transfer_vehicles_of_turn_movement)
			{
				define_component_interface(_Link_Interface, type_of(inbound_link), Link_Components::Prototypes::Link_Prototype,  ComponentType);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;
				typedef Vehicle_Components::Prototypes::Vehicle_Prototype<typename MasterType::vehicle_type, ComponentType> _Vehicle_Interface;
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type, ComponentType> _Network_Interface;
				typedef Turn_Movement_Components::Prototypes::Movement_Prototype<typename MasterType::movement_type> _Turn_Movement_Interface;
				define_component_interface(_Movement_Plan_Interface, typename _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				//move vehicles for this turn movement
				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				for (int iv=0;iv<num_transfer_vehicles_of_turn_movement;iv++)
				{
					_Vehicle_Interface* vehicle=((_Turn_Movement_Interface*)this)->template pull_vehicle<_Vehicle_Interface*>();

					//update vehicle state: transfer to next link
					int enter_time=vehicle->template movement_plan<_Movement_Plan_Interface*>()->template get_current_link_enter_time<int>();
					int delayed_time = int((((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>() - enter_time) - ((_Link_Interface*)_inbound_link)->template link_fftt<float>());
					int enter_interval_index = enter_time / ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					int delayed_interval = current_simulation_interval_index - enter_interval_index;

					//update inbound link state: N(a',L,t)
					((_Link_Interface*)_inbound_link)->template link_downstream_cumulative_vehicles<int&>()++;
					((_Link_Interface*)_inbound_link)->template link_downstream_departed_vehicles<int&>()++;

					((_Link_Interface*)_outbound_link)->template link_upstream_arrived_vehicles<int&>()++;
					((_Link_Interface*)_outbound_link)->template link_upstream_cumulative_arrived_vehicles<int&>()++;
					//update turn movement state: N(a',a,t)
					_turn_movement_cumulative_vehicles++;

					//update link_turn_travel_delay
					_outbound_link_arrived_time_based_experienced_link_turn_travel_delay += delayed_time;

					//find the departed time
					int departed_time_position = -1;
					int num_intervals=((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();
					if (current_simulation_interval_index>=num_intervals)
					{
						departed_time_position = (current_simulation_interval_index+num_intervals-delayed_interval)%num_intervals;
					}
					else
					{
						departed_time_position = enter_interval_index;
					}
					_cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array[departed_time_position]+=delayed_interval*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					((_Link_Interface*)_outbound_link)->template push_vehicle<_Vehicle_Interface*>(vehicle);
				}
			}


			feature_implementation void update_state(int t_cached_delay)
			{
				define_component_interface(_Link_Interface, type_of(inbound_link), Link_Components::Prototypes::Link_Prototype,  ComponentType);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;
				typedef Network_Components::Prototypes::Network_Prototype<typename MasterType::network_type, ComponentType> _Network_Interface;

				int t_plus_fftt = -1;
				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				if (current_simulation_interval_index>0)
				{
					t_plus_fftt = (current_simulation_interval_index) % (((_Link_Interface*)_inbound_link)->template link_fftt_cached_simulation_interval_size<int>());
				}
				else
				{
					t_plus_fftt = 0;
				}

				//cached cumulative shifted arrived vehicles

				_cached_turn_movement_cumulative_shifted_arrived_vehicles_array[t_plus_fftt] = _turn_movement_cumulative_arrived_vehicles;

				_cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array[t_cached_delay] = _outbound_link_arrived_time_based_experienced_link_turn_travel_delay;

				if (((current_simulation_interval_index+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
				{	
					float turn_travel_penalty = 0.0;
					for (int t_cached_time=0;t_cached_time<((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();t_cached_time++)
					{
						turn_travel_penalty += _cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array[t_cached_time];
					}

					turn_travel_penalty = (float) ( turn_travel_penalty/((float)((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>()) );
					_turn_travel_penalty = turn_travel_penalty;
					forward_link_turn_travel_time<ComponentType,CallerType,float>(((_Link_Interface*)_outbound_link)->template travel_time<float>()+_turn_travel_penalty);
				}
			}



			feature_implementation void accept_vehicle(void* vehicle)
			{
				define_container_and_value_interface_unqualified_container(_Vehicles_Container_Interface, _Vehicle_Interface, type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Link_Interface, type_of(inbound_link), Link_Components::Prototypes::Link_Prototype,  ComponentType);

				LOCK(_mvmt_lock);

				vehicles_container<ComponentType,CallerType,_Vehicles_Container_Interface&>().push_back((_Vehicle_Interface*)vehicle);
				_turn_movement_cumulative_arrived_vehicles++;
				
				UNLOCK(_mvmt_lock);
			}

			feature_implementation void initialize_features()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;
				define_component_interface(_Link_Interface, type_of(inbound_link), Link_Components::Prototypes::Link_Prototype,  ComponentType);

				_movement_capacity = 0.0;
				_movement_demand = 0.0;
				_movement_supply = 0.0;
				_movement_flow = 0.0;
				_movement_transferred = 0;
				_turn_movement_cumulative_vehicles = 0;
				_turn_movement_cumulative_arrived_vehicles = 0;
				_turn_movement_cumulative_shifted_arrived_vehicles = 0;
							
				//_vehicles_container<_Vehicles_Queue_Interface&>().clear();
							
				_outbound_link_arrived_time_based_experienced_link_turn_travel_delay = 0.0;
				_inbound_link_departed_time_based_experienced_link_turn_travel_delay = 0.0;
				_cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array.clear();
				_cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array.clear();
				_cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array.resize(((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>());
				_cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array.resize(((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>());
				int j;

				for (j=0;j<((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<int>();j++)
				{
					_cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array[j] = 0.0;
					_cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array[j] = 0.0;
				}
									
				_cached_turn_movement_cumulative_shifted_arrived_vehicles_array.clear();
				_cached_turn_movement_cumulative_shifted_arrived_vehicles_array.resize(((_Link_Interface*)_inbound_link)->template link_fftt_cached_simulation_interval_size<int>());
				for (j=0;j<((_Link_Interface*)_inbound_link)->template link_fftt_cached_simulation_interval_size<int>();j++)
				{
					_cached_turn_movement_cumulative_shifted_arrived_vehicles_array[j] = 0; 
				}

				if (_movement_type == Turn_Movement_Components::Types::THROUGH_TURN)
				{
					_minimum_merge_rate= 1.0;
				}
				else
				{
					_minimum_merge_rate = 0.2;
				}
			}
		};
	}
}
