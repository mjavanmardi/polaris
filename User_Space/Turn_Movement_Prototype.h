#pragma once
#include "User_Space.h"
#include "../File_IO/utilities.h"
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
		prototype struct Movement_Prototype
		{
			tag_as_prototype;

			// basic movement
			feature_accessor(internal_id, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(inbound_link, none, none);
			feature_accessor(outbound_link, none, none);

			// controled movement
			feature_accessor(detector, none, none);
			feature_accessor(merge_priority, none, none);
			feature_accessor(green_time, none, none);
			feature_accessor(inbound_link_green_cycle_ratio, none,none);
			feature_accessor(minimum_merge_rate, none, none);
			feature_accessor(movement_priority_type, none, none);

			// transportation movement
			feature_accessor(movement_type, none, none);
			feature_accessor(movement_rule, none, none);

			// routable movement
			feature_accessor(replicas_container, none, none);
			feature_accessor(turn_travel_penalty, none, none);
			feature_accessor(forward_link_turn_travel_time, none, none);

			// simulation movement
			feature_accessor(movement_capacity, none, none);
			feature_accessor(movement_supply, none, none);
			feature_accessor(movement_demand, none, none);
			feature_accessor(movement_flow, none, none);
			feature_accessor(movement_transferred, none, none);


			feature_accessor(vehicles_container, none, none);

			// queueing movement
			feature_accessor(cached_outbound_link_arrived_time_based_experienced_link_turn_travel_delay_array, none, none);
			feature_accessor(cached_inbound_link_departed_time_based_experienced_link_turn_travel_delay_array, none, none);
			feature_accessor(cached_outbound_link_departed_time_based_experienced_link_turn_travel_delay_array, none, none);
			feature_accessor(inbound_link_departed_time_based_experienced_link_turn_travel_delay, none, none);
			feature_accessor(outbound_link_arrived_time_based_experienced_link_turn_travel_delay, none, none);
			feature_accessor(turn_movement_cumulative_arrived_vehicles, none, none);
			feature_accessor(turn_movement_cumulative_vehicles, none, none);
			feature_accessor(cached_turn_movement_cumulative_shifted_arrived_vehicles_array, none, none);
			feature_accessor(turn_movement_cumulative_shifted_arrived_vehicles, none, none);
			feature_accessor(upstream_vehicles_container, none, none);

			feature_prototype TargetType pull_vehicle()
			{
				return offer_vehicle<TargetType>();
			}
			
			feature_prototype TargetType offer_vehicle()
			{

				define_container_and_value_interface(_Vehicles_Container_Interface, _Vehicle_Interface, get_type_of(vehicles_container), Back_Insertion_Sequence_Prototype, Vehicle_Components::Prototypes::Vehicle_Prototype, ComponentType);

				_Vehicle_Interface* veh=(_Vehicle_Interface*)vehicles_container<_Vehicles_Container_Interface&>().front();

				vehicles_container<_Vehicles_Container_Interface&>().pop_front();

				return (TargetType)veh;
			}
			
			feature_prototype void update_capacity(TargetType param)
			{
				this_component()->template update_capacity<ComponentType,CallerType,TargetType>(param);
			}

			feature_prototype void update_flow()
			{
				this_component()->template update_flow<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void transfer_vehicles(TargetType param)
			{
				this_component()->template transfer_vehicles<ComponentType,CallerType,TargetType>(param);
			}

			feature_prototype void update_state(TargetType param)
			{
				this_component()->template update_state<ComponentType,CallerType,TargetType>(param);
			}

			feature_prototype void accept_vehicle(TargetType param)
			{
				this_component()->template accept_vehicle<ComponentType,CallerType,TargetType>(param);
			}
			feature_prototype void initialize_features()
			{
				this_component()->template initialize_features<ComponentType,CallerType,TargetType>();
			}
		};
	}
}

using namespace Turn_Movement_Components::Prototypes;