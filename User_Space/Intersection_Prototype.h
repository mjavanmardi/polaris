#pragma once
#include "User_Space_Includes.h"
#include "Intersection_Control_Prototype.h"
#include "Vehicle_Prototype.h"
#include "../File_IO/utilities.h"
//#include "Signal_Prototypes.h"

/*
 * Needed for Linux compatability. Do not hurt Windows compilation.
 */
namespace Scenario_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Scenario_Prototype;
	}
};
namespace Link_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Link_Prototype;
	}
};
namespace Vehicle_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Vehicle_Prototype;
	}
};
namespace Network_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Network_Prototype;
	}
};
namespace Intersection_Control_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Intersection_Control_Prototype;
	}
};
namespace Movement_Plan_Components {
	namespace Prototypes
	{
		forward_declare_prototype struct Movement_Plan_Prototype;
	}
};
/*------------------------------------------------*/

namespace Intersection_Components
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

		enum Intersection_Type_Keys
		{
			NO_CONTROL=0,
			YIELD_SIGN,
			ALL_WAY_STOP_SIGN,
			TWO_WAY_STOP_SIGN,
			PRE_TIMED_SIGNAL_CONTROL, 
			ACTUATED_SIGNAL_CONTROL, 
			ADAPTIVE_SIGNAL_CONTROL,
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

		};

		prototype struct Outbound_Inbound_Movements_Prototype
		{
			tag_as_prototype;

			feature_accessor(outbound_link_reference, none, none);
			feature_accessor(inbound_movements, none, none);
		};

		prototype struct Inbound_Outbound_Movements_Prototype
		{
			tag_as_prototype;

			feature_accessor(inbound_link_reference, none, none);
			feature_accessor(lane_groups, none, none);
			feature_accessor(outbound_movements, none, none);
		};

		prototype struct Intersection_Prototype
		{
			tag_as_prototype;

			// basic intersection
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(x_position, none, none);
			feature_accessor(y_position, none, none);
			feature_accessor(z_position, none, none);
			feature_accessor(inbound_links, none, none);
			feature_accessor(outbound_links, none, none);
			feature_accessor(intersection_type, none, none);
			feature_accessor(network_reference, none, none);

			// transportation intersection
			feature_accessor(inbound_outbound_movements, none, none);
			feature_accessor(outbound_inbound_movements, none, none);

			// controlled intersection
			feature_accessor(intersection_control, none, none);
			feature_accessor(signal, none, none);

			// simulation intersection
			feature_accessor(rng_stream, none, none);

			feature_prototype void push_vehicle(void* vehicle)
			{
				accept_vehicle<TargetType>(vehicle);
			}

			feature_prototype void accept_vehicle(void* vehicle)
			{
				this_component()->accept_vehicle<ComponentType,CallerType,TargetType>(vehicle);
			}

			feature_prototype void turn_movement_capacity_update()
			{
				this_component()->turn_movement_capacity_update<ComponentType,CallerType,TargetType>();
			}

			//implementation distinguishes
			//feature_prototype void turn_movement_supply_allocation(requires(check_2(get_type_of(intersection_control),Yield_Type,is_same)))
			//{
			//	this_component()->turn_movement_supply_allocation<ComponentType,CallerType,TargetType>(); //supply_allocation_based_on_driving_rule<NULLTYPE>();
			//}

			feature_prototype void turn_movement_supply_allocation()
			{
				this_component()->turn_movement_supply_allocation<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void turn_movement_flow_calculation()
			{
				this_component()->turn_movement_flow_calculation<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void node_transfer()
			{
				this_component()->node_transfer<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void network_state_update()
			{
				this_component()->network_state_update<ComponentType,CallerType,TargetType>();
			}
			
			feature_prototype void origin_link_loading()
			{
				this_component()->origin_link_loading<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void Initialize()
			{
				this_component()->Initialize<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void construct_routable_from_regular(typename TargetType::ParamType regular_intersection_param, typename TargetType::Param2Type linksMap_param)
			{
				this_component()->construct_routable_from_regular<ComponentType,CallerType,TargetType>(regular_intersection_param, linksMap_param);
			}
		};
	}
}

using namespace Intersection_Components::Prototypes;