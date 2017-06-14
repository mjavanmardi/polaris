#pragma once
#include "Traffic_Simulator_Includes.h"
//#include "../File_IO/utilities.h"
//#include "Signal_Prototypes.h"

/*------------------------------------------------*/

namespace Turn_Movement_Components
{
	namespace Types
	{
		enum Turn_Movement_Type_Keys
		{
			LEFT_TURN=0,
			THROUGH_TURN,
			RIGHT_TURN,
			U_TURN,
			DRIVE_TO_TRANSIT,
			DRIVE_TO_WALK,
			TRANSIT_TO_DRIVE,
			TRANSIT_TO_TRANSIT,
			TRANSIT_TO_WALK,
			WALK_TO_DRIVE,
			WALK_TO_TRANSIT,
			WALK_TO_WALK
		};

		enum Turn_Movement_Rule_Keys
		{
			PROHIBITED=0,
			ALLOWED,
		};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Movement ADD_DEBUG_INFO
		{
			tag_as_prototype;

			// basic movement
			accessor(internal_id, NONE, NONE);
			accessor(uuid, NONE, NONE);
			accessor(inbound_link, NONE, NONE);
			accessor(outbound_link, NONE, NONE);

			// controled movement
			accessor(detector, NONE, NONE);
			accessor(merge_priority, NONE, NONE);
			accessor(green_time, NONE, NONE);
			accessor(inbound_link_green_cycle_ratio, NONE, NONE);
			accessor(minimum_merge_rate, NONE, NONE);
			accessor(movement_priority_type, NONE, NONE);

			// transportation movement
			accessor(movement_type, NONE, NONE);
			accessor(movement_rule, NONE, NONE);
			accessor(num_turn_lanes, NONE, NONE);

			// routable movement
			accessor(replicas_container, NONE, NONE);
			accessor(realtime_replicas_container, NONE, NONE);
			accessor(turn_travel_penalty, NONE, NONE);
			accessor(forward_link_turn_travel_time, NONE, NONE);
			accessor(realtime_forward_link_turn_travel_time, NONE, NONE);

			// simulation movement
			accessor(movement_capacity, NONE, NONE);
			accessor(movement_supply, NONE, NONE);
			accessor(movement_demand, NONE, NONE);
			accessor(movement_flow, NONE, NONE);
			accessor(movement_transferred, NONE, NONE);
			accessor(movement_capacity_leftover, NONE, NONE);

			accessor(vehicles_container, NONE, NONE);

			accessor(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array, NONE, NONE);
			accessor(outbound_link_arrived_time_based_experienced_link_turn_travel_delay, NONE, NONE);

			template<typename TargetType> TargetType pull_vehicle()
			{
				return offer_vehicle<TargetType>();
			}
			
			template<typename TargetType> TargetType offer_vehicle()
			{

				typedef  Vehicle_Components::Prototypes::Vehicle<typename remove_pointer< typename get_type_of(vehicles_container)::value_type>::type>  _Vehicle_Interface;
				typedef  Back_Insertion_Sequence< typename get_type_of(vehicles_container), _Vehicle_Interface*> _Vehicles_Container_Interface;


				_Vehicle_Interface* veh=(_Vehicle_Interface*)vehicles_container<_Vehicles_Container_Interface&>().front();
				vehicles_container<_Vehicles_Container_Interface&>().pop_front();

				return (TargetType)veh;
			}
			
			template<typename TargetType> void update_capacity(TargetType param)
			{
				this_component()->template update_capacity<TargetType>(param);
			}

			template<typename TargetType> void update_demand()
			{
				this_component()->template update_demand<TargetType>();
			}

			template<typename TargetType> void update_flow()
			{
				this_component()->template update_flow<TargetType>();
			}

			template<typename TargetType> void transfer_vehicles()
			{
				this_component()->template transfer_vehicles<TargetType>();
			}

			template<typename TargetType> void update_state()
			{
				this_component()->template update_state<TargetType>();
			}

			template<typename TargetType> void accept_vehicle(TargetType param)
			{
				this_component()->template accept_vehicle<TargetType>(param);
			}
			
			template<typename TargetType> void initialize_features()
			{
				this_component()->template initialize_features<TargetType>();
			}
			
			template<typename TargetType> void calculate_moe_for_simulation_interval_from_outbound_link()
			{
				this_component()->template calculate_moe_for_simulation_interval_from_outbound_link<TargetType>();
			}

			template<typename TargetType> void calculate_moe_for_simulation_interval_from_inbound_link()
			{
				this_component()->template calculate_moe_for_simulation_interval_from_inbound_link<TargetType>();
			}

			template<typename TargetType> void calculate_moe_for_assignment_interval_from_outbound_link()
			{
				this_component()->template calculate_moe_for_assignment_interval_from_outbound_link<TargetType>();
			}

			template<typename TargetType> void calculate_moe_for_assignment_interval_from_inbound_link()
			{
				this_component()->template calculate_moe_for_assignment_interval_from_inbound_link<TargetType>();
			}
		};


	}
}

using namespace Turn_Movement_Components::Prototypes;
