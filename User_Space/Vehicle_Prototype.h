#pragma once
#include "User_Space.h"

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

		prototype struct Vehicle_Prototype
		{
			tag_as_prototype;
			feature_accessor(simulation_status, none, none);
			feature_accessor(uuid, none, none);
			feature_accessor(internal_id, none, none);
			feature_accessor(movement_plan, none, none);
			feature_accessor(vehicle_information, none, none);
			feature_accessor(traveler, none, none);
			
			feature_prototype void advance()
			{
				define_component_interface(_Movement_Plan_Interface, typename get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				movement_plan<_Movement_Plan_Interface*>()->template advance_trajectory<NULLTYPE>();
			}

			feature_prototype void load()
			{
				this_component()->template load<CallerType, TargetType>();
			}

			feature_prototype void unload()
			{
				define_component_interface(_Movement_Plan_Interface, typename get_type_of(movement_plan), Movement_Plan_Components::Prototypes::Movement_Plan_Prototype, ComponentType);
				this_component()->template unload<CallerType, TargetType>();
				movement_plan<_Movement_Plan_Interface*>()->template arrive_to_destination<NULLTYPE>();
			}
		};
	}
}

using namespace Vehicle_Components::Prototypes;