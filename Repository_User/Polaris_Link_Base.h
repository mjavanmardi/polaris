#pragma once
#include "Repository_User_Includes.h"
#include "Polaris_Scenario_Base.h"
#include "Link_Interface.h"
#include "Polaris_Vechicle_Base.h"

namespace Link_Components
{ 
	namespace Types
	{
		enum Link_Type_Keys
		{
			FREEWAY=0,
			ON_RAMP,
			OFF_RAMP,
			EXPRESSWAY,
			ARTERIAL,
		};

	}

	namespace Concepts
	{
	}
	
	namespace Bases
	{
		struct Polaris_Link_Base
		{
			member_data_basic(int, uuid);

			member_data_basic(int, scenario_reference);

			member_data_basic(int, upstream_intersection);
			member_data_basic(int, downstream_intersection);
			
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
			member_data_basic(int, link_destination_departed_vehicles);//Not Used


			//cumulative - Mid-Trip
			member_data_basic(int, link_upstream_cumulative_arrival_vehicles);
			member_data_basic(int, link_downstream_cumulative_departure_vehicles);//Not Used

			member_data_basic(int, link_shifted_cumulative_arrival_vehicles);
			member_data_basic(int, link_shifted_cumulative_departure_vehicles);//Not Used

			member_data_basic(int, link_upstream_cumulative_vehicles);
			member_data_basic(int, link_downstream_cumulative_vehicles);
			
			//cumulative - Begin/End-Trip
			member_data_basic(int, link_origin_cumulative_arrived_vehicles);
			member_data_basic(int, link_origin_cumulative_departure_vehicles);

			member_data_basic(int, link_destination_cumulative_arrival_vehicles);
			member_data_basic(int, link_destination_cumulative_departure_vehicles);//Not Used

			//cached cumulative state - Mid-Trip
			member_data_basic(int, cached_link_upstream_cumulative_arrival_vehicles);//Not Used
			member_data_basic(int, cached_link_downstream_cumulative_departure_vehicles);//Not Used

			member_data_basic(int, cached_link_shifted_cumulative_arrival_vehicles);
			member_data_basic(int, cached_link_shifted_cumulative_departure_vehicles);//Not Used

			//========================================================================================================
			//start implementation of memeber_data(vector<int>, cached_link_upstream_cumulative_vehicles_array)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_link_upstream_cumulative_vehicles_array(call_requirements(requires(ThisType,Is_Dispatched) && requires(TargetType,is_pointer) && requires(strip_modifiers(TargetType),is_integral))))
			{
				return (TargetType)&(_cached_link_upstream_cumulative_vehicles_array.front());
			}

			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_link_upstream_cumulative_vehicles_array(call_requirements(requires(ThisType,Is_Dispatched) && !(requires(TargetType,is_pointer) && requires(strip_modifiers(TargetType),is_integral))))
			{
				return (TargetType&)(_cached_link_upstream_cumulative_vehicles_array);
			}
 
			tag_getter(cached_link_upstream_cumulative_vehicles_array);
	
		protected:
			vector<int> _cached_link_upstream_cumulative_vehicles_array;
			//end
			//--------------------------------------------------------------------------------------------------------

			//========================================================================================================
			//start implementation of memeber_data(vector<int>, cached_link_downstream_cumulative_vehicles_array)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType cached_link_downstream_cumulative_vehicles_array(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)&(_cached_link_downstream_cumulative_vehicles_array.front());
			}

			tag_getter(cached_link_downstream_cumulative_vehicles_array);
	
		protected:
			vector<int> _cached_link_downstream_cumulative_vehicles_array;
			//end
			//--------------------------------------------------------------------------------------------------------

			member_data_basic(int, link_origin_vehicle_current_position);
			
			//========================================================================================================			
			//start implementation of memeber_data(vector<Vehicle_Interface*>, link_origin_vechile_index_array)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType link_origin_vehicle_index_array(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_link_origin_vehicle_index_array);
			}

			tag_getter(link_origin_vehicle_index_array);
			typedef vector<Vehicle_Components::Interfaces::Vehicle_Interface<Vehicle_Components::Components::Polaris_Vehicle_Component, NULLTYPE>*> link_origin_vehicle_index_arrary_type;

		protected:
			link_origin_vehicle_index_arrary_type _link_origin_vehicle_index_array;
			//end
			//--------------------------------------------------------------------------------------------------------

		
			//========================================================================================================
			//start implementation of memeber_data(vector<Vehicle_Interface*>, link_origin_vehicle_queue)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType link_origin_vehicle_queue(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_link_origin_vehicle_queue);
			}

			tag_getter(link_origin_vehicle_queue);
			queue<int> a;
			typedef queue<Vehicle_Components::Interfaces::Vehicle_Interface<Vehicle_Components::Components::Polaris_Vehicle_Component, NULLTYPE>*> link_origin_vehicle_index_queue_type;

		protected:
			link_origin_vehicle_index_queue_type _link_origin_vehicle_queue;
			//end
			//--------------------------------------------------------------------------------------------------------
			
			

			//========================================================================================================
			//start implementation of memeber_data(vector<Vehicle_Interface*>, current_vehicle_queue)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType current_vehicle_queue(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_current_vehicle_queue);
			}

			tag_getter(current_vehicle_queue);
			typedef Vehicle_Components::Interfaces::Vehicle_Interface<Vehicle_Components::Components::Polaris_Vehicle_Component, NULLTYPE>* current_vehicle_queue_element_type;
			typedef vector<current_vehicle_queue_element_type> current_vehicle_queue_type;
		protected:
			current_vehicle_queue_type _current_vehicle_queue;
			//end
			//--------------------------------------------------------------------------------------------------------
			
			member_data_basic(float, maximum_flow_rate);
			member_data_basic(float, free_flow_speed);
			member_data_basic(float, backward_wave_speed);
			member_data_basic(float, jam_density);
			member_data_basic(float, critical_density);
			
			//========================================================================================================
			//start implementation of memeber_data(Scenario_Interface*, scenario_reference)
			//--------------------------------------------------------------------------------------------------------
		public:
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType scenario_reference(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)(_scenario_reference);
			}

			tag_getter(scenario_reference);
			typedef Scenario_Components::Interfaces::Scenario_Interface<Scenario_Components::Components::Polaris_Scenario_Component, NULLTYPE>* scenario_reference_type;
		protected:
			scenario_reference_type _scenario_reference;

		};
		//--------------------------------------------------------------------------------------------------------
	}

	namespace Components
	{
		typedef Polaris_Component<Link_Components::Interfaces::Link_Interface, Link_Components::Bases::Polaris_Link_Base> Polaris_Link_Component;
	}	

}