#pragma once
#include "User_Space_Includes.h"
#include "Link_Prototype.h"
#include "Vehicle_Prototype.h"
#include "Polaris_Link_Implementation.h"
#include "Intersection_Prototype.h"

namespace Intersection_Components
{
	namespace Types
	{
	
	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{

		implementation struct Polaris_Movement_Implementation
		{
		public:


			member_data(float, turn_travel_penalty, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(float, movement_capacity, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, movement_supply, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, movement_demand, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, movement_flow, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, movement_transferred, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(Intersection_Components::Types::Turn_Movement_Type_Keys, movement_type, none, none);
			member_data(Intersection_Components::Types::Turn_Movement_Rule_Keys, movement_rule, none, none);

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
			//define_container_and_value_interface(_Replicas_Container_Interface, _Replica_Interface, replicas_container_type, Random_Access_Sequence_Prototype, Intersection_Components::Prototypes::Movement_Prototype, NULLTYPE);
            typedef Intersection_Components::Prototypes::Movement_Prototype<typename replicas_container_type::unqualified_value_type,NULLTYPE> _Replica_Interface;
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

			//typedef deque<typename MasterType::vehicle_type*> vehicles_container_type;
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
		};
		


		implementation struct Polaris_Outbound_Inbound_Movements_Implementation
		{


			member_component(typename MasterType::link_type, outbound_link_reference, none, none);

			member_container(vector<typename MasterType::movement_type*>, inbound_movements, none, none);

		};
		
		implementation struct Polaris_Inbound_Outbound_Movements_Implementation
		{

			member_component(typename MasterType::link_type, inbound_link_reference, none, none);

			member_container(vector<typename MasterType::movement_type*>, outbound_movements, none, none);

		};
		
		implementation struct Polaris_Intersection_Implementation
		{

			

//			member_component(typename MasterType::SIGNAL_TYPE,signal, none, none);

			member_data(int, uuid, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(int, internal_id, none, none);
			member_data(float, x_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, y_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));
			member_data(float, z_position, check(ReturnValueType, is_arithmetic), check(SetValueType, is_arithmetic));

			member_data(Intersection_Components::Types::Intersection_Type_Keys, intersection_type, none, none);

			member_data(Types::Intersection_Simulation_Status, intersection_simulation_status, none, none);


			member_container(vector<typename MasterType::link_type*>, inbound_links, none, none);


			member_container(vector<typename MasterType::link_type*>, outbound_links, none, none);

			member_container(vector<typename MasterType::outbound_inbound_movements_type*>, outbound_inbound_movements, none, none);
			member_container(vector<typename MasterType::inbound_outbound_movements_type*>, inbound_outbound_movements, none, none);

			member_component(typename MasterType::scenario_type, scenario_reference, none, none);


			member_data(User_Space::RngStream, rng_stream, none, none);
			member_component(typename MasterType::network_type, network_reference, none, none);

			member_component(typename MasterType::intersection_control_type, intersection_control, none, none);

			typedef typename MasterType::vehicle_type vehicle_type;

		};
	}

}
