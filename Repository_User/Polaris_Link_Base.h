#pragma once
#include "Repository_User_Includes.h"
#include "Polaris_Scenario_Base.h"
#include "Link_Interface.h"
#include "Intersection_Interface.h"
#include "Polaris_Vehicle_Base.h"

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
		template<typename MasterType>
		struct Polaris_Turn_Movement_Base
		{
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_link(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_inbound_link;} tag_getter(inbound_link);
			template<typename ThisType, typename CallerType, typename TargetType>
			void inbound_link(TargetType value,call_requirements(requires(ThisType,Is_Dispatched))){_inbound_link=(TargetType)value;} tag_setter(inbound_link);

			void* _inbound_link;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_link(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_outbound_link;} tag_getter(outbound_link);
			template<typename ThisType, typename CallerType, typename TargetType>
			void outbound_link(TargetType value,call_requirements(requires(ThisType,Is_Dispatched))){_outbound_link=(TargetType)value;} tag_setter(outbound_link);

			void* _outbound_link;

			member_data_basic(Intersection_Components::Types::Turn_Movement_Type_Keys, turn_movement_type);
			member_data_basic(Intersection_Components::Types::Turn_Movement_Rule_Keys, turn_movement_rule);
			member_data_basic(float, turn_travel_penalty);
			member_data_basic(float, forward_link_turn_travel_time);
		};
		
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
			member_data_basic(float, travel_time);

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
			member_data_basic(int, link_upstream_cumulative_arrival_vehicles);
			member_data_basic(int, link_shifted_cumulative_arrival_vehicles);
			member_data_basic(int, link_upstream_cumulative_vehicles);
			member_data_basic(int, link_downstream_cumulative_vehicles);
			
			//cumulative - Begin/End-Trip
			member_data_basic(int, link_origin_cumulative_arrived_vehicles);
			member_data_basic(int, link_origin_cumulative_departure_vehicles);
			member_data_basic(int, link_destination_cumulative_arrival_vehicles);

			//cached cumulative state - Mid-Trip
			member_data_basic(int, cached_link_shifted_cumulative_arrival_vehicles);

			//other attributes
			member_data_basic(float, maximum_flow_rate);
			member_data_basic(float, free_flow_speed);
			member_data_basic(float, backward_wave_speed);
			member_data_basic(float, jam_density);
			member_data_basic(float, critical_density);



		//==================================================================================================================
		/// Inbound and Outbound Turn Movement Members
		//------------------------------------------------------------------------------------------------------------------
			typedef Polaris_Component<Interfaces::Turn_Movement_Interface,Polaris_Turn_Movement_Base<MasterType>,NULLTYPE,MasterType> turn_movements_type;

			typedef vector<Interfaces::Turn_Movement_Interface<turn_movements_type, NULLTYPE>*> turn_movements_container_type;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType inbound_turn_movements(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_inbound_turn_movements;}
			tag_getter(inbound_turn_movements);
			turn_movements_container_type _inbound_turn_movements;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType outbound_turn_movements(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_outbound_turn_movements;}
			tag_getter(outbound_turn_movements);
			turn_movements_container_type _outbound_turn_movements;


			
		//==================================================================================================================
		/// Scenario Reference
		//------------------------------------------------------------------------------------------------------------------
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType scenario_reference(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_scenario_reference);}
			tag_getter(scenario_reference);
			typedef typename MasterType::scenario_type scenario_type;
			void* _scenario_reference;



		//==================================================================================================================
		/// Upstream and Downstream Intersections Reference
		//------------------------------------------------------------------------------------------------------------------			
			typedef typename MasterType::intersection_type intersection_type;
			
			template<typename ThisType, typename CallerType, typename TargetType>
			void upstream_intersection(TargetType value,call_requirements(requires(ThisType,Is_Dispatched))){_upstream_intersection=(TargetType)value;}
			tag_setter(upstream_intersection);
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType upstream_intersection(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_upstream_intersection;}
			tag_getter(upstream_intersection);
			typedef intersection_type upstream_intersection_type;
			void* _upstream_intersection;

			template<typename ThisType, typename CallerType, typename TargetType>
			void downstream_intersection(TargetType value,call_requirements(requires(ThisType,Is_Dispatched))){_downstream_intersection=(TargetType)value;}
			tag_setter(downstream_intersection);
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType downstream_intersection(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)_downstream_intersection;}
			tag_getter(downstream_intersection);
			typedef intersection_type upstream_intersection_type;
			void* _downstream_intersection;
			



		//==================================================================================================================
		/// Containers of Cached Cumulative Vehicle Statistics
		//------------------------------------------------------------------------------------------------------------------
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_link_upstream_cumulative_vehicles_array(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,is_pointer) && requires(strip_modifiers(TargetType),is_integral)))
			{return (TargetType)&(_cached_link_upstream_cumulative_vehicles_array.front());}

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_link_upstream_cumulative_vehicles_array(call_requirements(requires(ThisType,Is_Dispatched) && !(requires(TargetType,is_pointer) && requires(strip_modifiers(TargetType),is_integral))))
			{return (TargetType&)(_cached_link_upstream_cumulative_vehicles_array);}
 
			tag_getter(cached_link_upstream_cumulative_vehicles_array);

			vector<int> _cached_link_upstream_cumulative_vehicles_array;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_link_downstream_cumulative_vehicles_array(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)&(_cached_link_downstream_cumulative_vehicles_array.front());}

			tag_getter(cached_link_downstream_cumulative_vehicles_array);

			vector<int> _cached_link_downstream_cumulative_vehicles_array;



		//==================================================================================================================
		/// Vehicle Origin Containers
		//------------------------------------------------------------------------------------------------------------------
			member_data_basic(int, link_origin_vehicle_current_position);

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType link_origin_vehicle_array(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_link_origin_vehicle_array);}

			tag_getter(link_origin_vehicle_array);
			typedef typename MasterType::vehicle_type vehicle_type;
			typedef vehicle_type link_origin_vehicle_array_element_type;
			typedef vector<void*> link_origin_vehicle_array_type;

			link_origin_vehicle_array_type _link_origin_vehicle_array;

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType link_origin_vehicle_queue(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_link_origin_vehicle_queue);}

			tag_getter(link_origin_vehicle_queue);
			typedef deque<void*> link_origin_vehicle_queue_type;

			link_origin_vehicle_queue_type _link_origin_vehicle_queue;

			
			
		//==================================================================================================================
		/// Current Vehicles Containers
		//------------------------------------------------------------------------------------------------------------------
			typedef vehicle_type current_vehicle_queue_element_type;
			typedef vector<void*> current_vehicle_queue_type;
			
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType current_vehicle_queue(call_requirements(requires(ThisType,Is_Dispatched))){return (TargetType)(_current_vehicle_queue);}tag_getter(current_vehicle_queue);

			current_vehicle_queue_type _current_vehicle_queue;
		};

	}

	namespace Components
	{
		template<typename MasterType>
		struct Polaris_Link_Component
		{
			typedef Polaris_Component_Execution<Interfaces::Link_Interface, Bases::Polaris_Link_Base<MasterType>,NULLTYPE,MasterType> type;
		};

		template<typename MasterType>
		struct Polaris_Turn_Movement_Component
		{
			typedef Polaris_Component<Interfaces::Turn_Movement_Interface, Bases::Polaris_Turn_Movement_Base<MasterType>,NULLTYPE,MasterType> type;
		};
	}	

}