#pragma once

#include "Link_Prototype.h"
#include "Polaris_Scenario_Implementation.h"
#include "Vehicle_Prototype.h"
#include "Traveler_Prototype.h"
#include "Person_Prototype.h"

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
		struct Link_MOE_Data
		{
			float link_travel_time;
			float link_travel_time_standard_deviation;
			float link_queue_length;
			float link_travel_delay;
			float link_travel_delay_standard_deviation;
			float link_speed;
			float link_density;
			float link_in_flow_rate;
			float link_out_flow_rate;

			float link_in_volume;
			float link_out_volume;

			float link_speed_ratio;
			float link_in_flow_ratio;
			float link_out_flow_ratio;
			float link_density_ratio;
			float link_travel_time_ratio;

			float num_vehicles_in_link;
		};
	//==================================================================================================================
	/// Polaris_Link_Base
	//------------------------------------------------------------------------------------------------------------------
		implementation struct Polaris_Link_Implementation:public Polaris_Component<APPEND_CHILD(Polaris_Link_Implementation),MasterType,Execution_Object,ParentType>
		{
		//==================================================================================================================
		/// Simple Link Members
		//------------------------------------------------------------------------------------------------------------------
			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
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
			member_data(float, original_free_flow_speed, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

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
		/// Replicas Container
		//------------------------------------------------------------------------------------------------------------------
			member_container(vector<typename MasterType::routable_link_type*>, replicas_container, none, none);

		//==================================================================================================================
		/// Replicas Containers
		//------------------------------------------------------------------------------------------------------------------

			member_component(typename MasterType::approach_type, approach, none, none);
			member_data(int, link_num_vehicles_in_queue, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_container(deque<typename MasterType::vehicle_type*>, link_destination_vehicle_queue, none, none);

			member_data(_lock,link_lock,none,none);

			member_data(float, link_vmt, none, none);
			
			member_data(float, link_vht, none, none);

			struct Link_MOE_Data link_moe_data;

			struct Link_MOE_Data realtime_link_moe_data;

			vector<struct Link_MOE_Data> td_link_moe_data_array;

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

			// update link supply
			feature_implementation void link_supply_update()
			{
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				_Network_Interface* network = (_Network_Interface*)_network_reference;
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

				float current_link_capacity = 0.0;
				current_link_capacity =  (float) (simulation_interval_length * _num_lanes * _maximum_flow_rate/3600.0);
				_link_capacity = current_link_capacity;
			}
			
			// update network state
			feature_implementation void network_state_update()
			{
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				_Network_Interface* network = (_Network_Interface*)_network_reference;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type,ComponentType> _Scenario_Interface;

				int current_simulation_interval_index = network->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();

				//calculate upstream cumulative vehicles using the three detector method
				// put this as a subfunction
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
				// put this as a sub-function
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

			feature_implementation void accept_vehicle(TargetType veh,requires(!check_2(CallerType,typename MasterType::movement_type,is_same) && !check_2(CallerType,ComponentType,is_same) && !check_2(CallerType,typename MasterType::routing_type,is_same) && !check(CallerType,Traveler_Components::Concepts::Is_Traveler) && !check(CallerType,Person_Components::Concepts::Is_Person_Mover)))
			{
				assert_check_2(CallerType,typename MasterType::movement_type,is_same,"Invalid CallerType");
				assert_check_2(CallerType,ComponentType,is_same,"Invalid CallerType");
				assert_check_2(CallerType,typename MasterType::routing_type,is_same,"Invalid CallerType");
			}



			feature_implementation void accept_vehicle(TargetType veh,requires(check_2(CallerType,typename MasterType::routing_type,is_same) || check(CallerType,Traveler_Components::Concepts::Is_Traveler) || check(CallerType,Person_Components::Concepts::Is_Person_Mover)))
			{
				define_container_and_value_interface_unqualified_container(_Link_Origin_Vehicles_Container_Interface, _Vehicle_Interface,type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				_link_origin_cumulative_arrived_vehicles++;
				_Vehicle_Interface* vehicle = (_Vehicle_Interface*)veh;

				LOCK(_link_lock);

				_link_origin_vehicle_array.push_back((typename MasterType::vehicle_type*)vehicle);
				vehicle->template load<Vehicle_Components::Types::Load_To_Entry_Queue>();

				UNLOCK(_link_lock);
			}

			feature_implementation void accept_vehicle(TargetType veh,requires(check_2(CallerType,typename MasterType::movement_type,is_same) || check_2(CallerType,ComponentType,is_same)))
			{
				define_container_and_value_interface_unqualified_container(_Vehicle_Queue_Interface, _Vehicle_Interface, type_of(current_vehicle_queue), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Destination_Vehicle_Queue_Interface, _Vehicle_Interface_2, type_of(link_destination_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_component_interface(_Movement_Plan_Interface, typename _Vehicle_Interface::get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);				
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				define_component_interface(_Scenario_Interface, typename _Network_Interface::get_type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);

				typedef Link_Prototype<ComponentType, ComponentType> _Link_Interface;
				
				int current_simulation_interval_index = ((_Network_Interface*)_network_reference)->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				_Vehicle_Interface* vehicle=(_Vehicle_Interface*)veh;
				_Movement_Plan_Interface* mp = vehicle->template movement_plan<_Movement_Plan_Interface*>();
				float a_delayed_time;

				if (mp->template current_trajectory_position<int>() == -1)
				{
					a_delayed_time = 0.0;
				}
				else
				{
					a_delayed_time = (int)((((_Network_Interface*)_network_reference)->template start_of_current_simulation_interval_relative<int>() - mp->template get_current_link_enter_time<int>()) - _link_fftt);
				}

				if (mp->trajectory_size<int>() == 0)
				{
					THROW_EXCEPTION("Error, empty trajectory for vehicle " << vehicle->uuid<int>());
				}
				mp->template transfer_to_next_link<NULLTYPE>(a_delayed_time);

				if(_internal_id == (mp->template destination<_Link_Interface*>())->template internal_id<int>())
				{
					vehicle->template unload<NULLTYPE>();
					_link_destination_cumulative_arrived_vehicles++;
					_link_destination_arrived_vehicles++;

					_link_destination_vehicle_queue.push_back((typename MasterType::vehicle_type*)vehicle);

					((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_arrived_vehicles<NULLTYPE>();
					((_Scenario_Interface*)_global_scenario)->template decrease_network_in_network_vehicles<NULLTYPE>();
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

			feature_implementation void origin_link_loading(RNG_Components::RngStream& rng_stream)
			{
				define_container_and_value_interface_unqualified_container(_Movements_Container_Interface, _Movement_Interface, type_of(outbound_turn_movements), Random_Access_Sequence_Prototype, Turn_Movement_Components::Prototypes::Movement_Prototype, ComponentType);
				define_container_and_value_interface_unqualified_container(_Vehicles_Container_Interface, _Vehicle_Interface, _Movement_Interface::get_type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				_link_origin_departed_vehicles = 0;
				_link_origin_loaded_vehicles = 0;
				//arrived vehicles at current interval
				int current_position = _link_origin_vehicle_current_position;
				if(current_position<(int)_link_origin_vehicle_array.size())
				{
					queue_vehicles<ComponentType,CallerType,TargetType>(current_position);
				}

				//have demand
				if(_link_origin_arrived_vehicles>0)
				{
					//check available spaces
					//float link_available_spaces = _link_supply - _link_upstream_arrived_vehicles;
					//int num_link_origin_departed_vehicles_allowed = (int) link_available_spaces;

					int num_vehicles_in_link = 0;
				    typename _Movements_Container_Interface::iterator outbound_itr;
					_Movement_Interface* outbound_movement;
					
					for(outbound_itr=_outbound_turn_movements.begin();outbound_itr!=_outbound_turn_movements.end();outbound_itr++)
					{
						outbound_movement=(_Movement_Interface*)(*outbound_itr);
						num_vehicles_in_link += int(outbound_movement->template vehicles_container<_Vehicles_Container_Interface&>().size());
					}
					num_vehicles_in_link += (int)_current_vehicle_queue.size();
					//float link_available_spaces = (float)(this->link_supply_array[outbound_link_index]- this->link_upstream_arrived_vehicles_array[outbound_link_index]);
					float link_available_spaces = (float)(_link_supply - num_vehicles_in_link);
				
					int num_link_origin_departed_vehicles_allowed = max(0,int(link_available_spaces));

					//
					float link_origin_departed_flow_allowed = (float) (link_available_spaces - num_link_origin_departed_vehicles_allowed);
					
					if (link_origin_departed_flow_allowed>0.0)
					{//partial vehicle
						double rng = rng_stream.RandU01();
						if(rng<=link_origin_departed_flow_allowed)
						{//partial vehicle, incomplete implementation
							++num_link_origin_departed_vehicles_allowed;
						}
					}

					if(num_link_origin_departed_vehicles_allowed>0)
					{
						int num_departed_vehicles = min(_link_origin_arrived_vehicles,num_link_origin_departed_vehicles_allowed);
						load_vehicles<ComponentType,CallerType,TargetType>(num_departed_vehicles);
					}
				}
			}
			
			feature_implementation void load_vehicles(int num_departed_vehicles)
			{
				define_container_and_value_interface_unqualified_container(_Vehicles_Origin_Container_Interface, _Vehicle_Interface, type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				typedef Link_Components::Prototypes::Link_Prototype<typename MasterType::link_type, ComponentType> _Link_Interface;
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;

				_Vehicle_Interface* vehicle;
				for (int iv=0;iv<num_departed_vehicles;iv++)
				{
					vehicle=(_Vehicle_Interface*)_link_origin_vehicle_queue.front();
					_link_origin_vehicle_queue.pop_front();
					//update vehicle state
					vehicle->template load<Vehicle_Components::Types::Load_To_Origin_Link>();
					//update link state
					_link_origin_cumulative_departed_vehicles++;
					_link_origin_departed_vehicles++;
					_link_origin_arrived_vehicles--;
					((_Link_Interface*)this)->template push_vehicle<_Vehicle_Interface*>(vehicle);
					((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_departed_vehicles<NULLTYPE>();
					((_Scenario_Interface*)_global_scenario)->template increase_network_in_network_vehicles<NULLTYPE>();
				}
			}

			feature_implementation void queue_vehicles(int current_position)
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;
				define_container_and_value_interface_unqualified_container(_Vehicles_Origin_Container_Interface, _Vehicle_Interface, type_of(link_origin_vehicle_array), Random_Access_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);
				define_container_interface_unqualified_container(_Vehicle_Origin_Queue_Interface, type_of(link_origin_vehicle_queue), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				for(int iv=current_position;iv<(int)_link_origin_vehicle_array.size();iv++)
				{
					//update current position
					_link_origin_vehicle_current_position++;
					_Vehicle_Interface* vehicle = (_Vehicle_Interface*)link_origin_vehicle_array<ComponentType,CallerType,_Vehicles_Origin_Container_Interface&>()[iv];
					link_origin_vehicle_queue<ComponentType,CallerType,_Vehicle_Origin_Queue_Interface&>().push_back(vehicle);
					_link_origin_arrived_vehicles++;
					_link_origin_loaded_vehicles++;
					_link_origin_cumulative_arrived_vehicles++;
					((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_loaded_vehicles<NULLTYPE>();
				}
			}

			feature_implementation void initialize_features(void* network)
			{
				define_component_interface(_Network_Interface, type_of(network_reference), Network_Components::Prototypes::Network_Prototype, ComponentType);
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;
				//network_data
				_link_origin_cumulative_arrived_vehicles = 0;
				_link_origin_vehicle_array.clear();

				_network_reference = (typename MasterType::network_type*)network;

				//init link simulation model
				_link_capacity = 0;
				_link_upstream_arrived_vehicles = 0;
				_link_downstream_departed_vehicles = 0;
				_link_origin_arrived_vehicles = 0;
				_link_origin_departed_vehicles = 0;
				_link_origin_loaded_vehicles = 0;
				_link_destination_arrived_vehicles = 0;
				_link_origin_vehicle_current_position = 0;
					
				//supply
				_link_supply = _num_lanes * _length * _jam_density;
					
				//cumulative vehicles
				_link_destination_arrived_vehicles = 0;
				_link_origin_cumulative_arrived_vehicles = 0;
				_link_origin_cumulative_departed_vehicles = 0;
				_link_upstream_cumulative_arrived_vehicles = 0;
				_link_upstream_cumulative_vehicles = 0;
				
				////bwtt and fftt
				float bwtt = (float) (_length/(_backward_wave_speed*5280.0/3600.0)); // in seconds
				float fftt = (float) (_length/(_free_flow_speed*5280.0/3600.0)); //in seconds
				_link_fftt = fftt;
				_link_bwtt = bwtt;
				
				_link_fftt_cached_simulation_interval_size = (int(ceil(float(fftt/(float(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()))))));
				_link_bwtt_cached_simulation_interval_size = (int(ceil(float(bwtt/(float(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()))))));

				//downstream cumulative vehicles
				_cached_link_downstream_cumulative_vehicles_array.clear();
				_cached_link_downstream_cumulative_vehicles_array.resize(_link_bwtt_cached_simulation_interval_size);
					
				int j;
				for (j = 0; j < (int)_cached_link_downstream_cumulative_vehicles_array.size(); j++)
				{
					_cached_link_downstream_cumulative_vehicles_array[j] = 0;
				}

				//upstream cumulative vehicles
				_cached_link_upstream_cumulative_vehicles_array.clear();
				_cached_link_upstream_cumulative_vehicles_array.resize(_link_fftt_cached_simulation_interval_size);
				for (j = 0; j < (int)_cached_link_upstream_cumulative_vehicles_array.size(); j++)
				{
					_cached_link_upstream_cumulative_vehicles_array[j] = 0;
				}

				td_link_moe_data_array.clear();
				initialize_moe();
			}

			void initialize_moe()
			{
				link_moe_data.link_density = 0.0f;
				link_moe_data.link_in_flow_rate = 0.0f;
				link_moe_data.link_out_flow_rate = 0.0f;
				link_moe_data.link_queue_length = 0.0f;
				link_moe_data.link_speed = 0.0f;
				link_moe_data.link_travel_delay = 0.0f;
				link_moe_data.link_travel_delay_standard_deviation = 0.0f;
				link_moe_data.link_travel_time = 0.0f;
				link_moe_data.link_travel_time_standard_deviation = 0.0f;
		
				link_moe_data.link_density_ratio = 0.0f;
				link_moe_data.link_in_flow_ratio = 0.0f;
				link_moe_data.link_out_flow_ratio = 0.0f;
				link_moe_data.link_speed_ratio = 0.0f;
				link_moe_data.link_travel_time_ratio = 0.0f;
			}

			feature_implementation void update_vmt_vht()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;
				_link_vmt = link_moe_data.link_out_flow_rate * ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<float>() * _num_lanes * _length / 5280.0f / 3600.0f;
				_link_vht = link_moe_data.link_out_flow_rate * ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<float>() * _num_lanes * link_moe_data.link_travel_time / 60.0f / 3600.0f;
			}

			feature_implementation void calculate_moe_for_simulation_interval()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;
				realtime_link_moe_data.link_in_flow_rate = realtime_link_moe_data.link_in_volume * 3600.0f / (((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()) / _num_lanes;
				realtime_link_moe_data.link_out_flow_rate = realtime_link_moe_data.link_out_volume * 3600.0f / (((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>()) / _num_lanes;

				realtime_link_moe_data.link_out_flow_ratio = realtime_link_moe_data.link_out_flow_rate / _maximum_flow_rate;
				realtime_link_moe_data.link_in_flow_ratio = realtime_link_moe_data.link_in_flow_rate / _maximum_flow_rate;
				realtime_link_moe_data.link_queue_length = _link_num_vehicles_in_queue;

				realtime_link_moe_data.link_travel_time_ratio = realtime_link_moe_data.link_travel_time / (_length/5280.0f/_original_free_flow_speed * 60.0f);
				
				//link speed
				realtime_link_moe_data.link_speed = (_length/5280.0f) * 60.0f /	realtime_link_moe_data.link_travel_time;

				realtime_link_moe_data.link_speed_ratio = realtime_link_moe_data.link_speed / _original_free_flow_speed;

				realtime_link_moe_data.link_density = realtime_link_moe_data.num_vehicles_in_link /	(_length/5280.0f)/ float(_num_lanes);
			
				realtime_link_moe_data.link_density_ratio =	realtime_link_moe_data.link_density / _jam_density;
			}

			feature_implementation void calculate_moe_for_assignment_interval()
			{
				typedef Scenario_Components::Prototypes::Scenario_Prototype<typename MasterType::scenario_type, ComponentType> _Scenario_Interface;

				link_moe_data.link_in_flow_rate = 
				link_moe_data.link_queue_length = link_moe_data.link_queue_length / ((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<float>();
				
				link_moe_data.link_travel_time = float(_link_fftt / 60.0) + link_moe_data.link_travel_delay;
					
				link_moe_data.link_travel_time_standard_deviation = link_moe_data.link_travel_delay_standard_deviation;

				//link travel speed in mph
				link_moe_data.link_speed = (_length / 5280.0f)/ (link_moe_data.link_travel_time / 60.0f);

				//out flow rate in vehicles per hour per lane
				
				link_moe_data.link_out_flow_rate = link_moe_data.link_out_volume * 3600.0f / float(((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<float>() * 
				(float)_num_lanes); // vehicles per hour per lane
				
				update_vmt_vht<ComponentType,CallerType,TargetType>();
				
				//density
				link_moe_data.link_density =	link_moe_data.link_density / 	float(((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<float>());

				//ratio
				link_moe_data.link_density_ratio = link_moe_data.link_density / _jam_density;
				
				link_moe_data.link_in_flow_ratio = link_moe_data.link_in_flow_rate / _maximum_flow_rate;
				
				link_moe_data.link_out_flow_ratio = link_moe_data.link_out_flow_rate / _maximum_flow_rate;

				link_moe_data.link_speed_ratio = link_moe_data.link_speed /	_original_free_flow_speed;

				link_moe_data.link_travel_time_ratio = link_moe_data.link_travel_time /	(_length/5280.0f/(_original_free_flow_speed/60.0f));

				td_link_moe_data_array.push_back(link_moe_data);

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
				_this_ptr->template link_supply_update<NULLTYPE>();
			}

			declare_feature_event(Compute_Step_Flow_Link_Moving)
			{
				typedef Link_Prototype<typename MasterType::link_type> _Link_Interface;
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;

				//step 7: link moving -- no link moving in Newell's simplified model -- it can be used to determine turn bay curve
				_this_ptr->template link_moving<NULLTYPE>();

				//step 8: link network state update
				_this_ptr->template network_state_update<NULLTYPE>();
			}
		};
		

	}

}