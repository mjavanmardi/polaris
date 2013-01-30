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

		prototype struct Outbound_Inbound_Movements_Prototype
		{
			tag_as_prototype;

			feature_accessor(outbound_link_reference, none, none);
			feature_accessor(inbound_movements, none, none);

			feature_prototype void update_state()
			{
				this_component()->update_state<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void supply_allocation_based_on_driving_rule()
			{
				this_component()->supply_allocation_based_on_driving_rule<ComponentType,CallerType,TargetType>();
			}

			feature_prototype void supply_allocation_proportion_to_demand()
			{
				this_component()->supply_allocation_proportion_to_demand<ComponentType,CallerType,TargetType>();
			}
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