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
	/// Polaris_Turn_Movement_Base
	//------------------------------------------------------------------------------------------------------------------
		//template<typename MasterType>
		//struct Polaris_Turn_Movement_Base
		//{
		//	member_data_basic(int, id);

		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	TargetType inbound_link(call_requirements(requires(ComponentType,Is_Dispatched))){return (TargetType)_inbound_link;} tag_getter_as_available(inbound_link);
		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	void inbound_link(TargetType value,call_requirements(requires(ComponentType,Is_Dispatched))){_inbound_link=(TargetType)value;} tag_setter_as_available(inbound_link);

		//	void* _inbound_link;

		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	TargetType outbound_link(call_requirements(requires(ComponentType,Is_Dispatched))){return (TargetType)_outbound_link;} tag_getter_as_available(outbound_link);
		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	void outbound_link(TargetType value,call_requirements(requires(ComponentType,Is_Dispatched))){_outbound_link=(TargetType)value;} tag_setter_as_available(outbound_link);

		//	void* _outbound_link;

		//	member_data_basic(Intersection_Components::Types::Turn_Movement_Type_Keys, turn_movement_type);
		//	member_data_basic(Intersection_Components::Types::Turn_Movement_Rule_Keys, turn_movement_rule);
		//	member_data_basic(float, turn_travel_penalty);
		//	member_data_basic(float, forward_link_turn_travel_time);

		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	TargetType replicas_container(call_requirements(requires(ComponentType,Is_Dispatched))){return (TargetType)_replicas_container;} tag_getter_as_available(replicas_container);
		//	vector<void*> _replicas_container;

		//};
		//
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


			member_component(typename MasterType::scenario_type, scenario_reference, none, none);
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



			feature_implementation void link_supply_update()
			{
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				_Scenario_Interface* scenario=scenario_reference<ComponentType,CallerType,_Scenario_Interface*>();

				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();


				//Newell's model
				if(true)
				{
		
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

			}
			
			feature_implementation void network_state_update()
			{
				define_component_interface(_Scenario_Interface, type_of(scenario_reference), Scenario_Components::Prototypes::Scenario_Prototype, ComponentType);
				_Scenario_Interface* scenario=scenario_reference<ComponentType,CallerType,_Scenario_Interface*>();

				int current_simulation_interval_index = scenario->template current_simulation_interval_index<int>();
				int simulation_interval_length = scenario->template simulation_interval_length<int>();

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
					t_cached_delay = (current_simulation_interval_index)%scenario->template num_simulation_intervals_per_assignment_interval<int>();
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

		};
			
	}

}