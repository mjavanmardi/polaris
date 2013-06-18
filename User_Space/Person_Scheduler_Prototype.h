#pragma once

#include "User_Space_Includes.h"
//#include "Person_Prototype.h"


//---------------------------------------------------------
//	Person Scheduler - Handles Activity Scheduling for the Planner Class
//---------------------------------------------------------

namespace Person_Components
{
	namespace Types
	{
	}

	namespace Concepts
	{
	}
	
	namespace Prototypes
	{
		prototype struct Person_Scheduler ADD_DEBUG_INFO
		{
			tag_as_prototype;

			typedef Person_Scheduler<ComponentType, CallerType> This_Type;
			
			// accessor to parent class
			feature_accessor(Parent_Person,none,none);
			// accessors to parent class members
			feature_accessor(Activity_Container,none,none);
			feature_accessor(Movement_Plans_Container,none,none);

			feature_method_void(Initialize,none);

			feature_accessor(current_movement_plan,none,none);
			feature_accessor(current_activity_plan,none,none);
			
			feature_method_1_arg(next_activity_plan, current_time, check(typename TargetType::ParamType,Is_Time_Value) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			feature_method_1_arg(previous_activity_plan, current_time, check(typename TargetType::ParamType,Is_Time_Value) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			feature_method_1_arg(next_location, current_activity, check_as_given(typename TargetType::ParamType,is_pointer) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			feature_method_1_arg(previous_location, current_activity, check_as_given(typename TargetType::ParamType,is_pointer) /*&& check(typename TargetType::ReturnType,Activity_Components::Concepts::Is_Activity_Plan_Prototype)*/);
			
			//feature_method_1_arg(Resolve_Timing_Conflict, current_activity, check_2(typename TargetType::ReturnType,bool,is_same));

			feature_prototype bool Resolve_Timing_Conflict(TargetType current_activity, bool update_movement_plans=false)
			{
				return this_component()->Resolve_Timing_Conflict<ComponentType,CallerType,TargetType>(current_activity, update_movement_plans);
			}
			feature_prototype void Add_Movement_Plan(TargetType movement_plan)
			{
				this_component()->template Add_Movement_Plan<ComponentType,CallerType,TargetType>(movement_plan);
			}
			feature_prototype void Add_Activity_Plan(TargetType activity_plan)
			{
				this_component()->template Add_Activity_Plan<ComponentType,CallerType,TargetType>(activity_plan);
			}
			feature_prototype void Remove_Activity_Plan(TargetType activity_plan)
			{
				this_component()->template Remove_Activity_Plan<ComponentType,CallerType,TargetType>(activity_plan);
			}

			
		};
	}

}