#pragma once

#include "Link_Prototype.h"
#include "Link_Implementation.h"
#include "Scenario_Implementation.h"
#include "Vehicle_Prototype.h"
#include "Traveler_Prototype.h"
#include "../Repository/RNG_Implementations.h"
#ifndef EXCLUDE_DEMAND
#include "activity_simulator\Person_Prototype.h"
#endif
#include "Network_Event_Prototype.h"
#include "Depot_Prototype.h"
#include "Link_Control_Prototype.h"
#include "Advisory_ITS_Prototype.h"
#include "Sensor_Prototype.h"
#include "Movement_Plan_Prototype.h"

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
		implementation struct Visual_Link_Implementation:public Polaris_Component<MasterType,INHERIT(Visual_Link_Implementation),Data_Object>
		{
			 
			typedef typename Polaris_Component<MasterType,INHERIT(Visual_Link_Implementation),Data_Object>::Component_Type ComponentType;

		//==================================================================================================================
		/// Simple Link Members
		//------------------------------------------------------------------------------------------------------------------
			m_data(int, uuid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, internal_id, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, dbid, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, direction, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, num_lanes, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, length, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, speed_limit, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			
			m_container(std::vector<typename MasterType::activity_location_type*>, activity_locations, NONE, NONE);

			m_data(Link_Components::Types::Link_Type_Keys, link_type, NONE, NONE);
			
			m_data(int, num_left_turn_bays, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, num_right_turn_bays, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			
			m_data(float, left_turn_bay_length, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, right_turn_bay_length, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//m_data(int, inbound_turn_movement_size, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			//m_data(int, outbound_turn_movement_size, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			m_data(int, num_inbound_turn_lanes, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			
			//link state			
			m_data(float, link_fftt, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, link_bwtt, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_fftt_cached_simulation_interval_size, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_bwtt_cached_simulation_interval_size, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			
			//current interval
			m_data(float, link_capacity, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, link_supply, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			
			m_data(int, link_upstream_arrived_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_downstream_departed_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			
			m_data(int, link_origin_arrived_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_origin_departed_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_origin_loaded_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, link_origin_loaded_capacity_leftover, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			m_data(int, link_destination_arrived_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//cumulative - Mid-Trip
			m_data(int, link_upstream_cumulative_arrived_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			//m_data(int, link_shifted_cumulative_arrived_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_upstream_cumulative_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_downstream_cumulative_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_downstream_cumulative_arrived_vehicles, NONE, NONE);
			
			//cumulative - Begin/End-Trip
			m_data(int, link_origin_cumulative_arrived_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_origin_cumulative_departed_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, link_destination_cumulative_arrived_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//cached cumulative state - Mid-Trip
			//m_data(int, cached_link_shifted_cumulative_arrived_vehicles, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			//other attributes
			m_data(float, maximum_flow_rate, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, free_flow_speed, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, backward_wave_speed, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, jam_density, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, critical_density, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, num_vehicles_under_jam_density, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, original_free_flow_speed, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, original_maximum_flow_rate, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(float, original_speed_limit, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(int, original_num_lanes, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_data(bool, shoulder_opened, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_prototype(Null_Prototype,typename MasterType::ramp_metering_type, ramp_meter, NONE, NONE);


			m_data(float, speed_adjustment_factor_due_to_weather, NONE, NONE);
			m_data(float, speed_adjustment_factor_due_to_accident, NONE, NONE);
			m_data(float, capacity_adjustment_factor_due_to_weather, NONE, NONE);
			m_data(float, capacity_adjustment_factor_due_to_accident, NONE, NONE);
			m_data(float, lane_adjustment_due_to_accident, NONE, NONE);

		//==================================================================================================================
		/// Inbound and Outbound Turn Movement Members
		//------------------------------------------------------------------------------------------------------------------

			m_container(std::vector<typename MasterType::turn_movement_type*>, inbound_turn_movements, NONE, NONE);
			m_container(std::vector<typename MasterType::turn_movement_type*>, outbound_turn_movements, NONE, NONE);

			m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);

		//==================================================================================================================
		/// Upstream and Downstream Intersections Reference
		//------------------------------------------------------------------------------------------------------------------			

			m_prototype(Intersection_Components::Prototypes::Intersection,typename MasterType::intersection_type, upstream_intersection, NONE, NONE);
			m_prototype(Intersection_Components::Prototypes::Intersection,typename MasterType::intersection_type, downstream_intersection, NONE, NONE);

		//==================================================================================================================
		/// Containers of Cached Cumulative Vehicle Statistics
		//------------------------------------------------------------------------------------------------------------------
			
			m_container(std::vector<int>, cached_link_upstream_cumulative_vehicles_array, NONE, NONE);
			m_container(std::vector<int>, cached_link_downstream_cumulative_vehicles_array, NONE, NONE);

		//==================================================================================================================
		/// Vehicle Origin Containers
		//------------------------------------------------------------------------------------------------------------------
			m_data(int, link_origin_vehicle_current_position, NONE, NONE);
			//m_container(std::vector<typename MasterType::vehicle_type*>, link_origin_vehicle_array, NONE, NONE);
			m_container(std::deque<typename MasterType::vehicle_type*>, link_origin_vehicle_queue, NONE, NONE);
	
		//==================================================================================================================
		/// Current Vehicles Containers
		//------------------------------------------------------------------------------------------------------------------
			m_container(std::vector<typename MasterType::vehicle_type*>, current_vehicle_queue, NONE, NONE);

		//==================================================================================================================
		/// Replicas Container
		//------------------------------------------------------------------------------------------------------------------

			//TODO:ROUTING
			//m_container(std::vector<typename MasterType::routable_link_type*>, replicas_container, NONE, NONE);
			//m_container(std::vector<typename MasterType::routable_link_type*>, realtime_replicas_container, NONE, NONE);

		//==================================================================================================================
		/// Replicas Containers
		//------------------------------------------------------------------------------------------------------------------

			m_prototype(Null_Prototype,typename MasterType::approach_type, approach, NONE, NONE);
			m_data(int, link_num_vehicles_in_queue, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));
			m_container(std::deque<typename MasterType::vehicle_type*>, link_destination_vehicle_queue, NONE, NONE);
			m_data(int, num_vehicles_on_link, check(strip_modifiers(TargetType), is_arithmetic), check(strip_modifiers(TargetType), is_arithmetic));

			m_data(_lock,link_lock, NONE, NONE);

			m_data(float, link_vmt, NONE, NONE);
			
			m_data(Pocket_Data, pocket_data, NONE, NONE);

			m_data(float, link_vht, NONE, NONE);
			m_data(float, C, NONE, NONE);
			m_data(float, Q, NONE, NONE);

			struct Link_MOE_Data link_moe_data;
			struct Link_MOE_Data non_volatile_link_moe_data;
			struct Link_MOE_Data normal_day_link_moe_data;
			struct Link_MOE_Data realtime_link_moe_data;

			//std::vector<struct Link_MOE_Data> td_link_moe_data_array;
		//==================================================================================================================
		/// travel_time
		//------------------------------------------------------------------------------------------------------------------
	
			template<typename TargetType> TargetType travel_time(null_requirement){return (TargetType)(_travel_time);}
			template<typename TargetType> void travel_time(TargetType set_value,null_requirement)
			{
				_travel_time = (float)set_value;

				//TODO:ROUTING_OPERATION
				//// update replicas
				//typedef Link<typename MasterType::routable_link_type> replica_interface;
				//typename replicas_container_type::iterator replica_itr;
				//for (replica_itr=_replicas_container.begin(); replica_itr!=_replicas_container.end(); replica_itr++)
				//{
				//	replica_interface* replica = (replica_interface*)(*replica_itr);
				//	replica->template travel_time<float>(_travel_time);
				//}
			}

			float _travel_time;


			template<typename TargetType> TargetType realtime_travel_time(null_requirement){return (TargetType)(_realtime_travel_time);}
			template<typename TargetType> void realtime_travel_time(TargetType set_value,null_requirement)
			{
				_realtime_travel_time = (float)set_value;

				////TODO:ROUTING_OPERATION
				//// update replicas
				//typedef Link<typename MasterType::routable_link_type> replica_interface;
				//typename replicas_container_type::iterator replica_itr;
				//for (replica_itr=_realtime_replicas_container.begin(); replica_itr!=_realtime_replicas_container.end(); replica_itr++)
				//{
				//	replica_interface* replica = (replica_interface*)(*replica_itr);
				//	replica->template travel_time<float>(_realtime_travel_time);
				//}
			}

			float _realtime_travel_time;

		//==================================================================================================================
		/// Events
		//------------------------------------------------------------------------------------------------------------------

			m_data(bool, weather_event_to_process, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::weather_network_event_type, current_weather_event, NONE, NONE);
			m_data(bool, accident_event_to_process, NONE, NONE);
			m_prototype(Null_Prototype,typename MasterType::accident_network_event_type, current_accident_event, NONE, NONE);

			typedef typename MasterType::base_network_event_type base_network_event_type;
			typedef Network_Event<base_network_event_type> _Network_Event_Interface;
			m_container(std::vector<_Network_Event_Interface*>, advisory_radio_events, NONE, NONE);
			

		//==================================================================================================================
		/// ITS
		//------------------------------------------------------------------------------------------------------------------

			m_prototype(Advisory_ITS_Components::Prototypes::Advisory_ITS, typename MasterType::advisory_radio_type, advisory_radio, NONE, NONE);
			m_prototype(Depot_Components::Prototypes::Depot, typename MasterType::depot_type, depot, NONE, NONE);
			m_prototype(Advisory_ITS_Components::Prototypes::Advisory_ITS, typename MasterType::variable_word_sign_type, variable_word_sign, NONE, NONE);
			m_prototype(Advisory_ITS_Components::Prototypes::Advisory_ITS, typename MasterType::variable_speed_sign_type, variable_speed_sign, NONE, NONE);
			m_prototype(Null_Prototype, typename MasterType::link_sensor_type, link_sensor, NONE, NONE);



			typedef Network_Components::Prototypes::Network<typename MasterType::network_type> _Network_Interface;
			typedef Scenario_Components::Prototypes::Scenario<typename MasterType::scenario_type> _Scenario_Interface;
			typedef Vehicle_Components::Prototypes::Vehicle<typename MasterType::vehicle_type> _Vehicle_Interface;
			typedef Vehicle_Components::Prototypes::Vehicle<typename remove_pointer<typename type_of(link_origin_vehicle_queue)::value_type>::type>  _Vehicle_Interface1;
			typedef Random_Access_Sequence<type_of(link_origin_vehicle_queue), _Vehicle_Interface1*> _Vehicles_Container_Interface;

			//typedef  Movement_Plan_Components::Prototypes::Movement_Plan< typename _Vehicle_Interface::get_type_of(movement_plan)> _Movement_Plan_Interface;
			typedef Movement_Plan_Components::Prototypes::Movement_Plan<typename MasterType::movement_plan_type> _Movement_Plan_Interface;
			typedef Movement_Plan_Components::Prototypes::Trajectory_Unit<typename remove_pointer< typename _Movement_Plan_Interface::get_type_of(trajectory_container)::value_type>::type>  _Trajectory_Unit_Interface;
			typedef Random_Access_Sequence< typename _Movement_Plan_Interface::get_type_of(trajectory_container), _Trajectory_Unit_Interface*> _Trajectory_Container_Interface;

			typedef Network_Event<typename MasterType::weather_network_event_type> _Weather_Network_Event_Interface;
			typedef Network_Event<typename MasterType::accident_network_event_type> _Accident_Network_Event_Interface;				
			typedef Turn_Movement_Components::Prototypes::Movement<typename remove_pointer<typename  type_of(outbound_turn_movements)::value_type>::type>  _Movement_Interface;
			typedef Random_Access_Sequence<type_of(outbound_turn_movements), _Movement_Interface*> _Movements_Container_Interface;

			typedef Link_Components::Prototypes::Link<typename MasterType::link_type> _Link_Interface;
			typedef Intersection_Components::Prototypes::Intersection<type_of(upstream_intersection)> _Intersection_Interface;
			typedef Network_Event_Components::Prototypes::Network_Event_Manager< typename _Network_Interface::get_type_of(network_event_manager)> _Network_Event_Manager_Interface;
			
			Visual_Link_Implementation()
			{
				UNLOCK(_link_lock);
			}

			// update link supply
			template<typename TargetType> void link_supply_update()
			{
				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				//Newell's model
				int link_upstream_cumulative_vehicles_by_t_minus_one = 0;	
				int link_downstream_cumulative_vehicles_by_t_minus_bwtt = 0;	
				//N(a,0,t-1)
				if (current_simulation_interval_index>0)
				{
					int t_minus_one_fftt = (current_simulation_interval_index-1)%_link_fftt_cached_simulation_interval_size;
					link_upstream_cumulative_vehicles_by_t_minus_one = _cached_link_upstream_cumulative_vehicles_array[t_minus_one_fftt];
				}
				
				//N(a,L(a),t-bwtt)
				if (current_simulation_interval_index >= _link_bwtt_cached_simulation_interval_size)
				{
					int t_minus_bwtt = (current_simulation_interval_index - _link_bwtt_cached_simulation_interval_size)%_link_bwtt_cached_simulation_interval_size;
					link_downstream_cumulative_vehicles_by_t_minus_bwtt = _cached_link_downstream_cumulative_vehicles_array[t_minus_bwtt];
				}
				
				//supply(a,t) = Kj(a,t)*L(a)*nlanes(a,t) + N(a,L(a),t-bwtt) -N(a,0,t-1) = backward wave propogation in link
				float link_available_spaces = _num_vehicles_under_jam_density + link_downstream_cumulative_vehicles_by_t_minus_bwtt - link_upstream_cumulative_vehicles_by_t_minus_one;
				if (((_Scenario_Interface*)_global_scenario)->template jam_density_constraints_enforced<bool>())
				{
					link_available_spaces = min((float)link_available_spaces, float(_num_vehicles_under_jam_density - _num_vehicles_on_link));
				}

				_link_supply = max(0.0,(double)link_available_spaces);

				float current_link_capacity = 0.0;
				current_link_capacity =  (float) (simulation_interval_length * _num_lanes * _maximum_flow_rate/3600.0);

				_link_capacity = current_link_capacity;
			}
			
			// update network state
			template<typename TargetType> void network_state_update()
			{
				int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();

				int link_shifted_cumulative_arrived_vehicles = 0;

				if((current_simulation_interval_index) >= _link_fftt_cached_simulation_interval_size)
				{
					int t_minus_fftt = (current_simulation_interval_index - _link_fftt_cached_simulation_interval_size) % _link_fftt_cached_simulation_interval_size;
					link_shifted_cumulative_arrived_vehicles = _cached_link_upstream_cumulative_vehicles_array[t_minus_fftt];
				}
				else
				{
					link_shifted_cumulative_arrived_vehicles = 0;
				}

				_C = _link_upstream_cumulative_vehicles + _link_capacity;
				int link_downstream_cumulative_vehicles_by_t_minus_bwtt = 0;	
				//N(a,L(a),t-bwtt)
				if (current_simulation_interval_index >= _link_bwtt_cached_simulation_interval_size)
				{
					int t_minus_bwtt = (current_simulation_interval_index - _link_bwtt_cached_simulation_interval_size)%_link_bwtt_cached_simulation_interval_size;
					link_downstream_cumulative_vehicles_by_t_minus_bwtt = _cached_link_downstream_cumulative_vehicles_array[t_minus_bwtt];
				}
				_Q = _num_vehicles_under_jam_density + link_downstream_cumulative_vehicles_by_t_minus_bwtt;

				int upstream_cumulative_departed_vehicles = 0;
				upstream_cumulative_departed_vehicles = 
					_link_upstream_cumulative_arrived_vehicles + 
					_link_origin_cumulative_departed_vehicles - 
					_link_destination_cumulative_arrived_vehicles;

				upstream_cumulative_departed_vehicles = max(0,upstream_cumulative_departed_vehicles);

				if (((_Scenario_Interface*)_global_scenario)->template jam_density_constraints_enforced<bool>())
				{
					_link_upstream_cumulative_vehicles = upstream_cumulative_departed_vehicles;
				}
				else
				{
					int t_minus_bwtt=-1;

					if(current_simulation_interval_index >= _link_bwtt_cached_simulation_interval_size)
					{
						t_minus_bwtt = (current_simulation_interval_index - _link_bwtt_cached_simulation_interval_size)%_link_bwtt_cached_simulation_interval_size;
					}

					if (t_minus_bwtt>-1)
					{
						int jam_vehicles = (int) (_num_lanes * _length * _jam_density/5280.0f);
						jam_vehicles = max(_num_lanes*2,jam_vehicles);
						int cached=_cached_link_downstream_cumulative_vehicles_array[t_minus_bwtt]+jam_vehicles;
						_link_upstream_cumulative_vehicles = min(upstream_cumulative_departed_vehicles,cached);
					}
					else
					{
						_link_upstream_cumulative_vehicles = upstream_cumulative_departed_vehicles;
					}
				}

				int t_fftt = (current_simulation_interval_index)%_link_fftt_cached_simulation_interval_size;
				int t_bwtt = (current_simulation_interval_index)%_link_bwtt_cached_simulation_interval_size;

				_cached_link_upstream_cumulative_vehicles_array[t_fftt] = _link_upstream_cumulative_vehicles;
				_cached_link_downstream_cumulative_vehicles_array[t_bwtt] = _link_downstream_cumulative_vehicles;

				_link_num_vehicles_in_queue = link_shifted_cumulative_arrived_vehicles - _link_downstream_cumulative_vehicles;
				_num_vehicles_on_link = _link_upstream_cumulative_vehicles - _link_downstream_cumulative_vehicles;

				_link_downstream_cumulative_arrived_vehicles = link_shifted_cumulative_arrived_vehicles;


				typedef typename remove_pointer<network_reference_type>::type::Component_Type::routable_networks_type routable_networks_type;

				Prototype_Random_Access_Sequence<routable_networks_type,Routable_Network>* routable_networks = _network_reference->routable_networks<routable_networks_type>();


				//Note, this variable is the delay for link + turn delay
				//_outbound_link_arrived_time_based_experienced_link_turn_travel_delay

				unsigned int outbound_turn_index = 0;

				for(std::vector<typename MasterType::turn_movement_type*>::iterator itr = _outbound_turn_movements.begin();itr!=_outbound_turn_movements.end();itr++,outbound_turn_index++)
				{
					Movement<typename MasterType::turn_movement_type>* turn_movement = (Movement<typename MasterType::turn_movement_type>*) *itr;


					turn_movement->update_state<NT>();
					
					int current_simulation_interval_index = ((_Network_Interface*)_global_network)->template current_simulation_interval_index<int>();

					//if (((current_simulation_interval_index+1)*((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>())%((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<int>() == 0)
					{	
						float cost_update = turn_movement->forward_link_turn_travel_time<float>();

						//if(_dbid == 16435 && turn_movement->outbound_link<Link<typename MasterType::link_type>*>()->dbid<int>() == 85584)
						//{
						//	cout << cost_update << endl;
						//}

						for(Prototype_Random_Access_Sequence<routable_networks_type,Routable_Network>::iterator routable_itr = routable_networks->begin();routable_itr != routable_networks->end();routable_itr++)
						{
							Routable_Network<typename MasterType::routable_network_type>* current_network = *routable_itr;

							current_network->update_edge_turn_cost(_uuid, _link_fftt, outbound_turn_index, cost_update-_link_fftt);
						}
					}

				}

			}

// #ifndef EXCLUDE_DEMAND
			// template<typename TargetType> void accept_vehicle(TargetType veh,requires(TargetType,!check_2(typename MasterType::movement_type,is_same) && !check_2(ComponentType,is_same) && !check_2(typename MasterType::vehicle_type,is_same) && !check_2(typename MasterType::routing_type,is_same) && !check(Traveler_Components::Concepts::Is_Traveler) && !check(Person_Components::Concepts::Is_Person_Mover)))
// #else
			// template<typename TargetType> void accept_vehicle(TargetType veh,requires(TargetType,!check_2(typename MasterType::movement_type,is_same) && !check_2(ComponentType,is_same) && !check_2(typename MasterType::vehicle_type,is_same) && !check_2(typename MasterType::routing_type,is_same) && !check(Traveler_Components::Concepts::Is_Traveler)))
// #endif
			// {
				// assert_check_2(typename MasterType::movement_type,is_same,"Invalid CallerType");
				// assert_check_2(typename MasterType::vehicle_type,is_same,"Invalid CallerType");
				// assert_check_2(typename MasterType::routing_type,is_same,"Invalid CallerType");
				// assert_check_2(ComponentType,is_same,"Invalid CallerType");
			// }


			template<typename TargetType> void accept_vehicle_from_origin(TargetType veh)
			{
				_Vehicle_Interface* vehicle = (_Vehicle_Interface*)veh;

				LOCK(_link_lock);
				_link_origin_vehicle_queue.push_back((typename MasterType::vehicle_type*)vehicle);
				UNLOCK(_link_lock);
				vehicle->template load<Vehicle_Components::Types::Load_To_Entry_Queue>();
			}

			template<typename TargetType> void accept_vehicle_from_network(TargetType veh)
			{
				int current_simulation_interval_index = ((_Network_Interface*)_network_reference)->template current_simulation_interval_index<int>();
				int simulation_interval_length = ((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
				_Vehicle_Interface* vehicle=(_Vehicle_Interface*)veh;
				_Movement_Plan_Interface* mp = vehicle->template movement_plan<_Movement_Plan_Interface*>();

				if(_internal_id == (mp->template destination<_Link_Interface*>())->template internal_id<int>())
				{
					//vehicle->template unload<NULLTYPE>();
					_link_destination_cumulative_arrived_vehicles++;
					_link_destination_arrived_vehicles++;

					if (((_Scenario_Interface*)_global_scenario)->template write_vehicle_trajectory<bool>())
					{
						LOCK(_link_lock);
						_link_destination_vehicle_queue.push_back((typename MasterType::vehicle_type*)vehicle);
						UNLOCK(_link_lock);
					}
					int departure_time = mp->template departed_time<Time_Seconds>();
					int arrival_time = mp->template arrived_time<Time_Seconds>();
					float travel_time = float ((arrival_time - departure_time)/60.0f);
					((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_arrived_vehicles<NULLTYPE>(travel_time);
					((_Network_Interface*)_global_network)->template update_ttime_distribution<NT>((int)travel_time);
					
					if(vehicle->is_integrated<bool>()) ((_Scenario_Interface*)_global_scenario)->template decrease_network_in_network_vehicles<NULLTYPE>();
					
					((_Scenario_Interface*)_global_scenario)->template decrease_network_in_system_vehicles<NULLTYPE>();
					
					Free((typename MasterType::movement_plan_type*)mp);
				}
				else
				{
					///set up downstream preferred departure time
					int current_time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_absolute<int>();
					int pdt = current_time + _link_fftt;

					LOCK(_link_lock);
					if (_current_vehicle_queue.size() >= 1)
					{
						_Vehicle_Interface* last_vehicle = (_Vehicle_Interface*)_current_vehicle_queue.back();
						int last_vehicle_pdt = last_vehicle->template downstream_preferred_departure_time<int>();
						pdt = max(float(last_vehicle_pdt), float(pdt));
					}
					vehicle->template downstream_preferred_departure_time<int>(pdt);
					_current_vehicle_queue.push_back((typename MasterType::vehicle_type*)vehicle);

					UNLOCK(_link_lock);
				}
			}

			template<typename TargetType> void link_moving()
			{
				typename std::vector<typename MasterType::vehicle_type*>::iterator vehicle_itr;

				for(vehicle_itr=_current_vehicle_queue.begin();vehicle_itr!=_current_vehicle_queue.end();vehicle_itr++)
				{
					((_Intersection_Interface*)_downstream_intersection)->template push_vehicle<NULLTYPE>((*vehicle_itr));
				}
				_current_vehicle_queue.clear();
			}

			template<typename TargetType> void origin_link_loading()
			{

				_link_origin_departed_vehicles = 0;
				_link_origin_loaded_vehicles = 0;
				//arrived vehicles at current interval

				queue_vehicles<TargetType>();


				//have demand
				if(_link_origin_arrived_vehicles>0)
				{
					//check available spaces
					float link_available_spaces = _link_supply - _link_upstream_arrived_vehicles;
				
					int num_link_origin_departed_vehicles_allowed = 0 ;// max(0,int(link_available_spaces));
					
					if (((_Scenario_Interface*)_global_scenario)->template rng_type<int>() == Scenario_Components::Types::RNG_Type_Keys::DETERMINISTIC)
					{///deterministic
						float link_origin_departed_flow_allowed = link_available_spaces;
						link_origin_departed_flow_allowed = max(0.0f,link_origin_departed_flow_allowed);
						//link_origin_departed_flow_allowed = min(link_origin_departed_flow_allowed,_link_capacity);	//////////////////////////capacity
						
						link_origin_departed_flow_allowed += _link_origin_loaded_capacity_leftover;

						if (link_origin_departed_flow_allowed < 0.0f)
						{
							num_link_origin_departed_vehicles_allowed = 0;
							_link_origin_loaded_capacity_leftover = link_origin_departed_flow_allowed;
						}
						else
						{
							// borrow from next simulation interval
							num_link_origin_departed_vehicles_allowed = (int)link_origin_departed_flow_allowed;
							num_link_origin_departed_vehicles_allowed++;
							_link_origin_loaded_capacity_leftover = link_origin_departed_flow_allowed - num_link_origin_departed_vehicles_allowed;
						}
					}
					else
					{
						float link_origin_departed_flow_allowed = max(0.0f,link_available_spaces);
						//link_origin_departed_flow_allowed = min(link_origin_departed_flow_allowed,_link_capacity);	////////////////////capacity

						num_link_origin_departed_vehicles_allowed = int(link_origin_departed_flow_allowed);
						if (link_origin_departed_flow_allowed - num_link_origin_departed_vehicles_allowed > 0.005)
							num_link_origin_departed_vehicles_allowed++;
						link_origin_departed_flow_allowed -= float(num_link_origin_departed_vehicles_allowed);

						if (link_origin_departed_flow_allowed>0.0)
						{//partial vehicle
							double rng = Uniform_RNG.Next_Rand<double>();//rng_stream.RandU01();
							if(rng<=link_origin_departed_flow_allowed)
							{//partial vehicle, incomplete implementation
								++num_link_origin_departed_vehicles_allowed;
							}
						}
					}

					////
					if(num_link_origin_departed_vehicles_allowed>0)
					{
						int num_departed_vehicles = min(_link_origin_arrived_vehicles,num_link_origin_departed_vehicles_allowed);
						load_vehicles<TargetType>(num_departed_vehicles);
					}
				}
				_link_origin_vehicle_current_position = (int)_link_origin_vehicle_queue.size();
			}
			
			template<typename TargetType> void load_vehicles(int num_departed_vehicles)
			{
				_Vehicle_Interface* vehicle;
				for (int iv=0;iv<num_departed_vehicles;iv++)
				{
					vehicle=(_Vehicle_Interface*)_link_origin_vehicle_queue.front();
					_link_origin_vehicle_queue.pop_front();
					//update vehicle state
					//vehicle->template load<Vehicle_Components::Types::Load_To_Origin_Link>();
					//update link state
					_link_origin_cumulative_departed_vehicles++;
					_link_origin_departed_vehicles++;
					_link_origin_arrived_vehicles--;
					//vehicle->template distance_to_stop_bar<float>(((_Link_Interface*)this)->template length<float>());
					//((_Link_Interface*)this)->template push_vehicle<_Vehicle_Interface*>(vehicle);
					if (((_Scenario_Interface*)_global_scenario)->template vehicle_taking_action<bool>())
					{
						vehicle->template make_suggestion<Vehicle_Components::Types::Type_Vehicle_Action_keys>(Vehicle_Components::Types::MOVE_TO_ORIGIN_LINK);
					}
					else
					{
						vehicle->template move_to_origin_link<NT>();
					}

					((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_departed_vehicles<NULLTYPE>();

					if(vehicle->is_integrated<bool>())
					{
						((_Scenario_Interface*)_global_scenario)->template increase_network_in_network_vehicles<NULLTYPE>();
					}

				}
			}

			template<typename TargetType> void queue_vehicles()
			{
				int new_origin_arrived = (int)_link_origin_vehicle_queue.size() - _link_origin_vehicle_current_position;
				_link_origin_arrived_vehicles += new_origin_arrived;
				_link_origin_loaded_vehicles += new_origin_arrived;
				_link_origin_cumulative_arrived_vehicles += new_origin_arrived;

				for (int i = 0; i < new_origin_arrived; i++)
				{
					((_Scenario_Interface*)_global_scenario)->template increase_network_cumulative_loaded_vehicles<NULLTYPE>();
				}
			}

			template<typename TargetType> void initialize_features(void* network)
			{
				//network_data
				//_link_origin_cumulative_arrived_vehicles = 0;
				//_link_origin_vehicle_array.clear();

				_network_reference = (network_reference_type)network;

				//init link simulation model
				_link_capacity = 0;
				_link_upstream_arrived_vehicles = 0;
				_link_downstream_departed_vehicles = 0;
				_link_origin_arrived_vehicles = 0;
				_link_origin_departed_vehicles = 0;
				_link_origin_loaded_vehicles = 0;
				_link_destination_arrived_vehicles = 0;
				_link_origin_vehicle_current_position = 0;
				_link_origin_loaded_capacity_leftover = 0.0;
				
				//supply
				//_num_vehicles_under_jam_density = _num_lanes * _length * _jam_density/5280.0f;
				//_num_vehicles_under_jam_density = max(_num_lanes * 2.0f,_num_vehicles_under_jam_density) + 30;
				//_num_vehicles_under_jam_density = max(_num_lanes * 2.0f,_num_vehicles_under_jam_density);

				//if(_link_type == Link_Components::Types::LOCAL || _link_type == Link_Components::Types::ARTERIAL)
				//{
				//	// num_lanes += (int)min(ceil(2.0f*(60.0f/link->template length<float>())),2.0f);

				//	// assuming an average pocket length of 60 meters and a pocket lane on either side
				//	float adjusted_lanes = ((float)_num_lanes) + min(2.0f*(196.85f/_length),2.0f);
				//	_num_vehicles_under_jam_density = adjusted_lanes * _length * _jam_density/5280.0f;
				//	_num_vehicles_under_jam_density = max(((float)_num_lanes) * 2.0f,_num_vehicles_under_jam_density);

				//	//cout << adjusted_lanes << "," << _num_lanes << endl;
				//}
				//else
				//{
				//	_num_vehicles_under_jam_density = ((float)_num_lanes) * _length * _jam_density/5280.0f;
				//	_num_vehicles_under_jam_density = max(((float)_num_lanes) * 2.0f,_num_vehicles_under_jam_density);
				//}

				float adjusted_lanes = ((float)_num_lanes) + ((float)_pocket_data.num_pockets)*( _pocket_data.pocket_length/_length );

				_num_vehicles_under_jam_density = adjusted_lanes * _length * _jam_density/5280.0f; //convert mile to feet
				_num_vehicles_under_jam_density = max(((float)_num_lanes) * 2.0f,_num_vehicles_under_jam_density);

				_link_supply = _num_vehicles_under_jam_density;
					
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
				
				//circular queues for the simulation
				_link_fftt_cached_simulation_interval_size = max(1,(int(ceil(float(fftt/(float(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>())))))));
				_link_bwtt_cached_simulation_interval_size = max(1,(int(ceil(float(bwtt/(float(((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>())))))));

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

				//td_link_moe_data_array.clear();
				initialize_moe();
				_weather_event_to_process = false;
				_current_weather_event = nullptr;
				_accident_event_to_process = false;
				_current_accident_event = nullptr;

				_capacity_adjustment_factor_due_to_accident = 1.0;
				_speed_adjustment_factor_due_to_accident = 1.0;
				_capacity_adjustment_factor_due_to_weather = 1.0;
				_speed_adjustment_factor_due_to_weather = 1.0;

				_advisory_radio = nullptr;
				_depot = nullptr;
				_variable_word_sign = nullptr;
				_variable_speed_sign = nullptr;

				_shoulder_opened = false;

				_Q = 0.0;
				_C = 0.0;
			}

			void initialize_moe()
			{
				link_moe_data.link_density = 0.0f;
				link_moe_data.link_in_flow_rate = 0.0f;
				link_moe_data.link_out_flow_rate = 0.0f;
				link_moe_data.link_in_volume = 0.0f;
				link_moe_data.link_out_volume = 0.0f;
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

			template<typename TargetType> void update_vmt_vht()
			{
				//_link_vmt = link_moe_data.link_out_flow_rate * ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<float>() * _num_lanes * _length / 5280.0f / 3600.0f;
				//_link_vht = link_moe_data.link_out_flow_rate * ((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<float>() * _num_lanes * link_moe_data.link_travel_time / 60.0f / 3600.0f;
				_link_vmt = link_moe_data.link_out_volume * (_length / 5280.0f);
				_link_vht = link_moe_data.link_out_volume * (link_moe_data.link_travel_time / 60.0f);
			}

			template<typename TargetType> void calculate_moe_for_simulation_interval()
			{
				realtime_link_moe_data.link_in_flow_rate = (realtime_link_moe_data.link_in_volume * 3600.0f / (((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>())) / _num_lanes;
				realtime_link_moe_data.link_out_flow_rate = (realtime_link_moe_data.link_out_volume * 3600.0f / (((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<float>())) / _num_lanes;

				realtime_link_moe_data.link_out_flow_ratio = realtime_link_moe_data.link_out_flow_rate / _original_maximum_flow_rate;
				realtime_link_moe_data.link_in_flow_ratio = realtime_link_moe_data.link_in_flow_rate / _original_maximum_flow_rate;
				realtime_link_moe_data.link_queue_length = _link_num_vehicles_in_queue;

				realtime_link_moe_data.link_travel_time_ratio = realtime_link_moe_data.link_travel_time / (_length/5280.0f/_original_free_flow_speed * 60.0f);
				
				//link speed
				realtime_link_moe_data.link_speed = (_length/5280.0f) * 60.0f /	realtime_link_moe_data.link_travel_time;

				realtime_link_moe_data.link_speed_ratio = realtime_link_moe_data.link_speed / _original_free_flow_speed;

				realtime_link_moe_data.link_density = (realtime_link_moe_data.num_vehicles_in_link /	(_length/5280.0f))/ float(_num_lanes);
			
				realtime_link_moe_data.link_density_ratio =	realtime_link_moe_data.link_density / _jam_density;
			}

			template<typename TargetType> void calculate_moe_for_assignment_interval()
			{
				link_moe_data.link_in_flow_rate = (link_moe_data.link_in_volume * 3600.0f / (((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<float>())) / _num_lanes;
				
				link_moe_data.link_queue_length = link_moe_data.link_queue_length / ((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<float>();
				
				link_moe_data.link_travel_time = float(_link_fftt / 60.0) + link_moe_data.link_travel_delay;
					
				link_moe_data.link_travel_time_standard_deviation = link_moe_data.link_travel_delay_standard_deviation;

				//link travel speed in mph
				link_moe_data.link_speed = (_length / 5280.0f)/ (link_moe_data.link_travel_time / 60.0f);

				//out flow rate in vehicles per hour per lane
				
				link_moe_data.link_out_flow_rate = link_moe_data.link_out_volume * 3600.0f / float(((_Scenario_Interface*)_global_scenario)->template assignment_interval_length<float>() * 
				(float)_num_lanes); // vehicles per hour per lane
				
				update_vmt_vht<TargetType>();
				
				//density
				link_moe_data.link_density =	link_moe_data.link_density / float(((_Scenario_Interface*)_global_scenario)->template num_simulation_intervals_per_assignment_interval<float>());

				//ratio
				link_moe_data.link_density_ratio = link_moe_data.link_density / _jam_density;
				
				link_moe_data.link_in_flow_ratio = link_moe_data.link_in_flow_rate / _original_maximum_flow_rate;
				
				link_moe_data.link_out_flow_ratio = link_moe_data.link_out_flow_rate / _original_maximum_flow_rate;

				link_moe_data.link_speed_ratio = link_moe_data.link_speed /	_original_free_flow_speed;

				link_moe_data.link_travel_time_ratio = link_moe_data.link_travel_time /	(_length/5280.0f/(_original_free_flow_speed/60.0f));

				//td_link_moe_data_array.push_back(link_moe_data);

			}

			template<typename TargetType> void Initialize()
			{
				//Load_Event<ComponentType>(&ComponentType::Newells_Conditional,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::EVENTS_UPDATE_SUB_ITERATION);
			
				//TODO
				//load_event(ComponentType,ComponentType::template Newells_Conditional,ComponentType::template Update_Events,((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>()-1,Scenario_Components::Types::Type_Sub_Iteration_keys::EVENTS_UPDATE_SUB_ITERATION,NULLTYPE);
			}

			static void Newells_Conditional(ComponentType* _this,Event_Response& response)
			{
				_Link_Interface* _this_ptr=(_Link_Interface*)_this;
				if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::EVENTS_UPDATE_SUB_ITERATION)
				{
					//((typename MasterType::link_type*)_this)->Swap_Event((Event)&Update_Events<NULLTYPE>);
					//response.result=true;
					_this->Update_Events();
					response.next._iteration=iteration();
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_SUPPLY_UPDATE_SUB_ITERATION)
				{
					//((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Flow_Supply_Update<NULLTYPE>);
					//response.result=true;
					_this->Compute_Step_Flow_Supply_Update();
					response.next._iteration=iteration();
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_ORIGIN_LINK_LOADING_SUB_ITERATION;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_ORIGIN_LINK_LOADING_SUB_ITERATION)
				{
					//((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Origin_Link_Loading<NULLTYPE>);
					//response.result=true;
					_this->Compute_Step_Origin_Link_Loading();
					response.next._iteration=iteration();
					response.next._sub_iteration = Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION;
				}
				else if(sub_iteration() == Scenario_Components::Types::Type_Sub_Iteration_keys::LINK_COMPUTE_STEP_FLOW_LINK_MOVING_SUB_ITERATION)
				{
					//((typename MasterType::link_type*)_this)->Swap_Event((Event)&Compute_Step_Flow_Link_Moving<NULLTYPE>);
					//response.result=true;
					_this->Compute_Step_Flow_Link_Moving();
					response.next._iteration=iteration()+((_Scenario_Interface*)_global_scenario)->template simulation_interval_length<int>();
					response.next._sub_iteration=Scenario_Components::Types::Type_Sub_Iteration_keys::EVENTS_UPDATE_SUB_ITERATION;
				}
				else
				{
					cout << "Should never reach here in link conditional!" << endl;
					assert(false);
				}
			}

			//declare_event(Update_Events)
			void Update_Events()
			{
				_Link_Interface* _this_ptr=(_Link_Interface*)this;
				_this_ptr->template handle_events<NT>();
			}

			template<typename TargetType> void get_events_from_har(TargetType events_set)
			{
				if (_advisory_radio != nullptr)
				{
					std::vector<_Network_Event_Interface*> events;
					
					_advisory_radio->template Get_Displayed_Messages<typename MasterType::base_network_event_type>(events);
					typename std::vector<_Network_Event_Interface*>::iterator event_itr;
					for (event_itr = events.begin(); event_itr != events.end(); event_itr++)
					{
						_Network_Event_Interface* event = (_Network_Event_Interface*)(*event_itr);
						events_set.insert(event);
					}
				}
			}

			template<typename TargetType> void get_events_from_vms(TargetType events_set)
			{
				if (_variable_word_sign != nullptr)
				{
					std::vector<_Network_Event_Interface*> events;
					
					_variable_word_sign->template Get_Displayed_Messages<typename MasterType::base_network_event_type>(events);
					typename std::vector<_Network_Event_Interface*>::iterator event_itr;
					for (event_itr = events.begin(); event_itr != events.end(); event_itr++)
					{
						_Network_Event_Interface* event = (_Network_Event_Interface*)(*event_itr);
						events_set.insert(event);
					}
				}
			}

			static void Weather_Event_Notification(void* link, Network_Event<typename MasterType::weather_network_event_type>* net_event)
			{
				typedef typename MasterType::link_type _Link_Component;
				((_Link_Component*)link)->record_weather_event(net_event);
			}

			static void Accident_Event_Notification(void* link, Network_Event<typename MasterType::accident_network_event_type>* net_event)
			{
				typedef typename MasterType::link_type _Link_Component;
				((_Link_Component*)link)->record_accident_event(net_event);
			}

			void record_weather_event(Network_Event<typename MasterType::weather_network_event_type>* net_event)
			{
				_weather_event_to_process = true;
				_current_weather_event = (Null_Prototype<typename MasterType::weather_network_event_type>*)net_event; 
			}
				
			void record_accident_event(Network_Event<typename MasterType::accident_network_event_type>* net_event)
			{
				_accident_event_to_process = true;
				_current_accident_event = (Null_Prototype<typename MasterType::accident_network_event_type>*)net_event; 
			}

			template<typename TargetType> void handle_events()
			{
				//typedef Network<typename MasterType::network_type> _Network_Interface;
				int time = ((_Network_Interface*)_global_network)->template start_of_current_simulation_interval_relative<int>();
				if (!_weather_event_to_process && !_accident_event_to_process)
				{
					return;
				}
				if (_weather_event_to_process)
				{
					_weather_event_to_process = false;
					if (((typename MasterType::weather_network_event_type*)_current_weather_event)->_active)
					{
						process_weather_event<typename MasterType::weather_network_event_type*>();
					}
					else
					{
						revert_weather_event<typename MasterType::weather_network_event_type*>();
						_current_weather_event = NULL;
					}
				}
				
				if (_accident_event_to_process)
				{
					_accident_event_to_process = false;
					if (((typename MasterType::accident_network_event_type*)_current_accident_event)->_active)
					{
						process_accident_event<typename MasterType::accident_network_event_type*>();
					}
					else
					{
						revert_accident_event<typename MasterType::accident_network_event_type*>();
						_current_accident_event = NULL;
					}
				}
			}

			//declare_event(Compute_Step_Flow_Supply_Update)
			void Compute_Step_Flow_Supply_Update()
			{
				_Link_Interface* _this_ptr=(_Link_Interface*)this;
				//step 1: link supply update based on a given traffic flow model
				_this_ptr->template link_supply_update<NULLTYPE>();
			}

			//declare_event(Compute_Step_Origin_Link_Loading)
			void Compute_Step_Origin_Link_Loading()
			{
				_Link_Interface* _this_ptr=(_Link_Interface*)this;
				//step 6: origin link loading
				_this_ptr->template origin_link_loading<RNG_Components::RngStream&>();
			}

			//declare_event(Compute_Step_Flow_Link_Moving)
			void Compute_Step_Flow_Link_Moving()
			{
				_Link_Interface* _this_ptr=(_Link_Interface*)this;
				//step 7: link moving -- no link moving in Newell's simplified model -- it can be used to determine turn bay curve
				_this_ptr->template link_moving<NULLTYPE>();
				//step 8: link network state update
				_this_ptr->template network_state_update<NULLTYPE>();
			}
			
			template<typename TargetType> void Accept_ITS( typename MasterType::variable_speed_sign_type* vss)
			{
				_variable_speed_sign = (variable_speed_sign_type)vss;
			}
			
			template<typename TargetType> void Accept_ITS( typename MasterType::variable_word_sign_type* vws)
			{
				_variable_word_sign = (variable_word_sign_type)vws;
			}
			
			template<typename TargetType> void Accept_ITS( typename MasterType::advisory_radio_type* har)
			{
				_advisory_radio = (advisory_radio_type)har;
			}

			template<typename TargetType> void Accept_ITS( typename MasterType::depot_type* depot)
			{
				_depot = (depot_type)depot;
			}
			
			template<typename TargetType> void Accept_ITS( typename MasterType::link_sensor_type* link_sensor)
			{
				_link_sensor = (link_sensor_type)link_sensor;
			}


			template<typename TargetType> static void subscribe_events()
			{
				// event subscription
				_Network_Event_Manager_Interface* network_event_manager = ((_Network_Interface*)_global_network)->template network_event_manager<_Network_Event_Manager_Interface*>();
				network_event_manager->template Push_Subscriber<typename MasterType::weather_network_event_type>(&Weather_Event_Notification,(int)component_id);
				network_event_manager->template Push_Subscriber<typename MasterType::accident_network_event_type>(&Accident_Event_Notification,(int)component_id);
			}

			//void subscribe_events_local()
			//{
			//	// event subscription
			//	_Network_Event_Manager_Interface* network_event_manager = ((_Network_Interface*)_global_network)->template network_event_manager<_Network_Event_Manager_Interface*>();
			//	network_event_manager->template Push_Subscriber<typename MasterType::weather_network_event_type>(&Weather_Event_Notification);
			//	network_event_manager->template Push_Subscriber<typename MasterType::accident_network_event_type>(&Accident_Event_Notification);
			//}

			template<typename TargetType> void get_link_moe(int& start_time, int& end_time, int& volume, float& speed, float& density)
			{
				start_time = non_volatile_link_moe_data.start_time;
				end_time = non_volatile_link_moe_data.end_time;
				volume = non_volatile_link_moe_data.link_out_volume;
				speed = non_volatile_link_moe_data.link_speed;
				density = non_volatile_link_moe_data.link_density;
			}
			template<typename TargetType> void get_prevailing_link_moe(int& volume, float& speed, float& density)
			{
				volume = non_volatile_link_moe_data.link_out_volume;
				speed = non_volatile_link_moe_data.link_speed;
				density = non_volatile_link_moe_data.link_density;
			}

			template<typename TargetType> bool get_normal_day_link_moe(int& volume, float& speed, float& density)
			{
				if (((_Scenario_Interface*)_global_scenario)->template read_normal_day_link_moe<bool>())
				{
					volume = normal_day_link_moe_data.link_out_volume;
					speed = normal_day_link_moe_data.link_speed;
					density = normal_day_link_moe_data.link_density;
					return true;
				}
				else
				{
					return false;
				}
			}

			template<typename TargetType> void open_shoulder()
			{
				if (!_shoulder_opened)
				{
					_shoulder_opened = true;
					_num_lanes = _original_num_lanes + 1;
				}
			}

			template<typename TargetType> void close_shoulder()
			{
				if (_shoulder_opened)
				{
					_shoulder_opened = false;
					_num_lanes = _original_num_lanes;
				}
			}

			template<typename TargetType> void change_speed_limit(float a_speed_limit)
			{
				_speed_limit = a_speed_limit;
				//_free_flow_speed = min(_free_flow_speed, _speed_limit + 5.0f);
				_free_flow_speed = free_flow_speed_estimate<TargetType>() * _speed_adjustment_factor_due_to_accident;
				_free_flow_speed *= _speed_adjustment_factor_due_to_weather;
				_link_fftt = (float) (_length/(_free_flow_speed*5280.0/3600.0)); //in seconds
			}

			template<typename TargetType> float free_flow_speed_estimate()
			{
				float ffspd = -1.0f;
				if (_speed_limit < 40) 
					ffspd = _speed_limit + 5;
				else if (_speed_limit >= 40 && _speed_limit < 50) 
					ffspd = _speed_limit + 7;
				else if (_speed_limit >= 50) 
					ffspd = _speed_limit + 5;
				else
				{
					cout << "Invalid speed limit number!" << endl;
					assert(false);
				}
				return ffspd;
			}

			template<typename TargetType> float speed_limit_estimate()
			{
				float spd;

				if(_free_flow_speed >= (50+7))
				{
					spd = _free_flow_speed - 10;
				}
				else if(_free_flow_speed >= (40+7) && _free_flow_speed < (50+7))
				{
					spd = _free_flow_speed - 7;
				}
				else if(_free_flow_speed < (40+7))
				{
					spd = _free_flow_speed - 5;
				}
				
				int intspd = (int)spd;

				intspd/=5;
				intspd*=5;
				intspd+=5;

				spd = (float)intspd;

				return spd;
			}

			template<typename TargetType> void process_weather_event(){};
			template<typename TargetType> void revert_weather_event(){};
			template<typename TargetType> float find_free_flow_speed_adjustment_rate_for_weather(int weather_index){return 0.0;};
			template<typename TargetType> int get_weather_index(TargetType weather_event){return 0.0;};
			template<typename TargetType> void process_accident_event(){};
			template<typename TargetType> void revert_accident_event(){};
			template<typename TargetType> void reset_features(){};
		
			static float link_capacity_adjustment_factors_for_weather[19];
			static float link_free_flow_speed_adjustment_factors_for_weather[19][5];
			static float link_capacity_adjustment_factors_for_accident[8][5];
		};
	}
}
