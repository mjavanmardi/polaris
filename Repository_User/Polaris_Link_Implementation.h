#pragma once
#include "Repository_User_Includes.h"
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

	namespace Bases
	{
	//==================================================================================================================
	/// Polaris_Turn_Movement_Base
	//------------------------------------------------------------------------------------------------------------------
		//template<typename MasterType>
		//struct Polaris_Turn_Movement_Base
		//{
		//	member_data_basic(int, id);

		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	TargetType inbound_link(call_requirements(requires(ComponentType,Is_Dispatched))){return (TargetType)_inbound_link;} tag_getter(inbound_link);
		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	void inbound_link(TargetType value,call_requirements(requires(ComponentType,Is_Dispatched))){_inbound_link=(TargetType)value;} tag_setter(inbound_link);

		//	void* _inbound_link;

		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	TargetType outbound_link(call_requirements(requires(ComponentType,Is_Dispatched))){return (TargetType)_outbound_link;} tag_getter(outbound_link);
		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	void outbound_link(TargetType value,call_requirements(requires(ComponentType,Is_Dispatched))){_outbound_link=(TargetType)value;} tag_setter(outbound_link);

		//	void* _outbound_link;

		//	member_data_basic(Intersection_Components::Types::Turn_Movement_Type_Keys, turn_movement_type);
		//	member_data_basic(Intersection_Components::Types::Turn_Movement_Rule_Keys, turn_movement_rule);
		//	member_data_basic(float, turn_travel_penalty);
		//	member_data_basic(float, forward_link_turn_travel_time);

		//	template<typename ComponentType, typename CallerType, typename TargetType>
		//	TargetType replicas_container(call_requirements(requires(ComponentType,Is_Dispatched))){return (TargetType)_replicas_container;} tag_getter(replicas_container);
		//	vector<void*> _replicas_container;

		//};
		//
	//==================================================================================================================
	/// Polaris_Link_Base
	//------------------------------------------------------------------------------------------------------------------
		template<typename MasterType>
		struct Polaris_Link_Base
		{
		//==================================================================================================================
		/// Simple Link Members
		//------------------------------------------------------------------------------------------------------------------
			member_data_basic(int, uuid);

			member_data_basic(int, num_lanes);
			member_data_basic(float, length);
			member_data_basic(float, speed_limit);
			
			member_data_basic(Link_Components::Types::Link_Type_Keys, link_type);
			
			member_data_basic(int, num_left_turn_bays);
			member_data_basic(int, num_right_turn_bays);
			
			member_data_basic(float, left_turn_bay_length);
			member_data_basic(float, right_turn_bay_length);

			member_data_basic(int, inbound_turn_movement_size);
			member_data_basic(int, outbound_turn_movement_size);
			
			//link state			
			member_data_basic(float, link_fftt);
			member_data_basic(float, link_bwtt);
			member_data_basic(int, link_fftt_cached_simulation_interval_size);
			member_data_basic(int, link_bwtt_cached_simulation_interval_size);
			
			//current interval
			member_data_basic(float, link_capacity);
			member_data_basic(float, link_supply);
			
			member_data_basic(int, link_upstream_arrived_vehicles);
			member_data_basic(int, link_downstream_departed_vehicles);
			
			member_data_basic(int, link_origin_arrived_vehicles);
			member_data_basic(int, link_origin_departed_vehicles);
			
			member_data_basic(int, link_destination_arrived_vehicles);

			//cumulative - Mid-Trip
			member_data_basic(int, link_upstream_cumulative_arrived_vehicles);
			member_data_basic(int, link_shifted_cumulative_arrived_vehicles);
			member_data_basic(int, link_upstream_cumulative_vehicles);
			member_data_basic(int, link_downstream_cumulative_vehicles);
			
			//cumulative - Begin/End-Trip
			member_data_basic(int, link_origin_cumulative_arrived_vehicles);
			member_data_basic(int, link_origin_cumulative_departed_vehicles);
			member_data_basic(int, link_destination_cumulative_arrived_vehicles);

			//cached cumulative state - Mid-Trip
			member_data_basic(int, cached_link_shifted_cumulative_arrived_vehicles);

			//other attributes
			member_data_basic(float, maximum_flow_rate);
			member_data_basic(float, free_flow_speed);
			member_data_basic(float, backward_wave_speed);
			member_data_basic(float, jam_density);
			member_data_basic(float, critical_density);

			member_data_basic(Types::Link_Simulation_Status, link_simulation_status);

		//==================================================================================================================
		/// Inbound and Outbound Turn Movement Members
		//------------------------------------------------------------------------------------------------------------------

			member_container_basic(vector<typename MasterType::turn_movement_type*>, inbound_turn_movements);
			member_container_basic(vector<typename MasterType::turn_movement_type*>, outbound_turn_movements);


			member_component_basic(typename MasterType::scenario_type, scenario_reference);


		//==================================================================================================================
		/// Upstream and Downstream Intersections Reference
		//------------------------------------------------------------------------------------------------------------------			

			
			member_component_basic(typename MasterType::intersection_type, upstream_intersection);
			member_component_basic(typename MasterType::intersection_type, downstream_intersection);



		//==================================================================================================================
		/// Containers of Cached Cumulative Vehicle Statistics
		//------------------------------------------------------------------------------------------------------------------
			
			member_container_basic(vector<int>, cached_link_upstream_cumulative_vehicles_array);
			member_container_basic(vector<int>, cached_link_downstream_cumulative_vehicles_array);

		//==================================================================================================================
		/// Vehicle Origin Containers
		//------------------------------------------------------------------------------------------------------------------
			member_data_basic(int, link_origin_vehicle_current_position);

			member_container_basic(vector<typename MasterType::vehicle_type*>, link_origin_vehicle_array);

			member_container_basic(deque<typename MasterType::vehicle_type*>, link_origin_vehicle_queue);
	
		//==================================================================================================================
		/// Current Vehicles Containers
		//------------------------------------------------------------------------------------------------------------------
			member_container_basic(vector<typename MasterType::vehicle_type*>, current_vehicle_queue);


		//==================================================================================================================
		/// Replicas Containers
		//------------------------------------------------------------------------------------------------------------------
			member_container_basic(vector<typename MasterType::routable_link_type*>, replicas_container);


		//==================================================================================================================
		/// travel_time
		//------------------------------------------------------------------------------------------------------------------
			typedef Link_Prototype<typename MasterType::routable_link_type, NULLTYPE> replica_interface;
			template<typename ComponentType, typename CallerType, typename TargetType>
			TargetType travel_time(){return (TargetType)(_travel_time);} tag_getter(travel_time);
			template<typename ComponentType, typename CallerType, typename TargetType>
			void travel_time(TargetType set_value)
			{
				_travel_time = (int)set_value;
				// update replicas
				replicas_container_type::iterator replica_itr;
				for (replica_itr=_replicas_container.begin(); replica_itr!=_replicas_container.end(); replica_itr++)
				{
					replica_interface* replica = (replica_interface*)(*replica_itr);
					replica->travel_time<float>(_travel_time);
				}
			}
			tag_setter(travel_time);

			int _travel_time;
		};
			
	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Link_Component{typedef Polaris_Component_Execution<Bases::Polaris_Link_Base, MasterType> type; };
	}	

}