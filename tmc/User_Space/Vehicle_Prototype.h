#pragma once
#include "User_Space_Includes.h"

namespace Vehicle_Components
{
	namespace Types
	{
		enum Vehicle_Type_Keys
		{
			SOV=0,
			HOV,
			TRUCK,
			BUS,
			RAIL,
		};

		enum Route_Choice_Use_Class_Keys
		{
			UNRESPONSIVE=0,
			ENROUTE_SWITCHING,
			USER_EQUILIBRIUM,
			SYSTEM_OPTIMAL,
		};
		
		enum Enroute_Information_Keys
		{
			NO_REALTIME_INFORMATION = 0,
			WITH_REALTIME_INFORMATION,
		};

		enum Vehicle_Status_Keys
		{
			UNLOADED=0,
			IN_ENTRY_QUEUE,
			IN_NETWORK,
			OUT_NETWORK,
		};

		struct Load_To_Entry_Queue{};
		struct Load_To_Origin_Link{};
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{

		prototype struct Switch_Decision_Data_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;
			feature_accessor(switch_decision_index, none, none);
			feature_accessor(route_links_container, none, none);
		};

		prototype struct Vehicle_Prototype ADD_DEBUG_INFO
		{
			tag_as_prototype;
			feature_accessor(simulation_status, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(movement_plan, none, none);
			feature_accessor(vehicle_information, none, none);
			feature_accessor(traveler, none, none);
			feature_accessor(distance_to_stop_bar, none, none);
			feature_accessor(local_speed, none, none);
			
			//enroute swtiching
			feature_accessor(rng_stream, none, none);
			feature_accessor(enroute_information_type, none, none);
			feature_accessor(information_compliance_rate, none, none);
			feature_accessor(relative_indifference_bound_route_choice, none, none);
			feature_accessor(minimum_travel_time_saving, none, none);


			feature_prototype void advance()
			{
				define_component_interface(_Movement_Plan_Interface, typename get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				movement_plan<_Movement_Plan_Interface*>()->template advance_trajectory<NULLTYPE>();
			}

			feature_prototype void load()
			{
				this_component()->template load<ComponentType,CallerType, TargetType>();
			}

			feature_prototype void unload()
			{
				define_component_interface(_Movement_Plan_Interface, typename get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				this_component()->template unload<ComponentType,CallerType, TargetType>();
				movement_plan<_Movement_Plan_Interface*>()->template arrive_to_destination<NULLTYPE>();
			}
			
			feature_prototype void enroute_switching()
			{
				this_component()->template enroute_switching<ComponentType,CallerType, TargetType>();
			}

			feature_prototype bool exploit_events_set(TargetType events_set)
			{
				return this_component()->template exploit_events_set<ComponentType,CallerType, TargetType>(events_set);
			}

			feature_prototype void update_vehicle_position()
			{
				return this_component()->template update_vehicle_position<ComponentType,CallerType, TargetType>();
			}
			feature_prototype void display_vehicle_position()
			{
				return this_component()->template update_vehicle_position<ComponentType,CallerType, TargetType>();
			}

			feature_prototype void initialize()
			{
				this_component()->template initialize<ComponentType,CallerType, TargetType>();
			}
		};
	}
}

using namespace Vehicle_Components::Prototypes;